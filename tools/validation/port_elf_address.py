#!/usr/bin/env python3
"""
Port memory addresses between ELF binaries with the same architecture.

Starts from one source ELF and resolves one source virtual address into one or
many target ELFs using deterministic matching first, then stricter fallback
heuristics.

Resolver order (high level):
1) Direct masked pattern match (section-aware)
2) .bss/NOBITS: literal-xref first, then section-offset/page-delta fallback
3) .text ARM disambiguation: callsite-xref, then caller-majority vote
4) Optional text heuristics: local micro-window, adaptive direct-window,
     ARM/Thumb block match, thunk-target, nearby-anchor refinement
5) Byte-backed non-text: literal-xref before nearby-data-anchor

Key behavior:
- ARM/Thumb branch words and embedded pointers are masked in signatures.
- Text clone/sibling ambiguities are resolved via mapped caller evidence when
    possible.
- Ambiguous or unsupported cases return unresolved instead of blind guesses.
"""

from __future__ import annotations

import argparse
import struct
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, List, Optional, Sequence, Tuple

from elftools.elf.elffile import ELFFile


@dataclass(frozen=True)
class SectionInfo:
    name: str
    addr: int
    size: int
    flags: int
    sh_type: str
    data: bytes

    @property
    def has_bytes(self) -> bool:
        return self.sh_type != "SHT_NOBITS" and self.size > 0

    @property
    def end(self) -> int:
        return self.addr + self.size

    @property
    def is_alloc(self) -> bool:
        # SHF_ALLOC == 0x2
        return bool(self.flags & 0x2)


@dataclass(frozen=True)
class Match:
    section: str
    section_offset: int
    va: int


@dataclass
class PortResult:
    target: str
    address: Optional[int]
    method: str
    confidence: str
    note: str


class ElfImage:
    def __init__(self, path: Path):
        self.path = path
        with path.open("rb") as f:
            elf = ELFFile(f)
            sections: List[SectionInfo] = []
            for sec in elf.iter_sections():
                hdr = sec.header
                size = int(hdr["sh_size"])
                sh_type = str(hdr["sh_type"])
                data = b""
                if sh_type != "SHT_NOBITS" and size > 0:
                    data = sec.data()
                sections.append(
                    SectionInfo(
                        name=sec.name,
                        addr=int(hdr["sh_addr"]),
                        size=size,
                        flags=int(hdr["sh_flags"]),
                        sh_type=sh_type,
                        data=data,
                    )
                )
        self.sections = sections
        self.alloc_sections = [s for s in sections if s.is_alloc and s.size > 0]

    def find_section_for_va(self, va: int) -> Optional[Tuple[SectionInfo, int]]:
        for sec in self.alloc_sections:
            if sec.addr <= va < sec.end:
                return sec, va - sec.addr
        return None

    def read_u32(self, va: int) -> Optional[int]:
        located = self.find_section_for_va(va)
        if located is None:
            return None
        sec, off = located
        if not sec.has_bytes or off + 4 > len(sec.data):
            return None
        return struct.unpack_from("<I", sec.data, off)[0]

    def all_alloc_ranges(self) -> Sequence[Tuple[int, int]]:
        return [(s.addr, s.end) for s in self.alloc_sections]


def parse_address(text: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"Invalid address: {text}") from exc


def bytes_match_at(buf: bytes, pattern: bytes, strict_positions: Sequence[int], offset: int) -> bool:
    for pos in strict_positions:
        if buf[offset + pos] != pattern[pos]:
            return False
    return True


def count_strict_matches_at(buf: bytes, pattern: bytes, strict_positions: Sequence[int], offset: int) -> int:
    matches = 0
    for pos in strict_positions:
        if buf[offset + pos] == pattern[pos]:
            matches += 1
    return matches


def longest_strict_run(mask: bytes) -> Tuple[int, int]:
    best_start = -1
    best_len = 0
    cur_start = -1
    cur_len = 0

    for i, m in enumerate(mask):
        if m:
            if cur_len == 0:
                cur_start = i
            cur_len += 1
            if cur_len > best_len:
                best_len = cur_len
                best_start = cur_start
        else:
            cur_len = 0

    return best_start, best_len


def find_masked_matches(
    data: bytes,
    pattern: bytes,
    mask: bytes,
    step: int = 1,
) -> List[int]:
    if len(pattern) == 0 or len(pattern) > len(data):
        return []

    strict_positions = [i for i, m in enumerate(mask) if m]
    if not strict_positions:
        return []

    out: List[int] = []

    # Fast path for exact matching.
    if len(strict_positions) == len(mask):
        start = 0
        while True:
            idx = data.find(pattern, start)
            if idx == -1:
                break
            out.append(idx)
            start = idx + 1
        return out

    # Anchor on the longest strict run, then verify full strict positions.
    run_start, run_len = longest_strict_run(mask)
    if run_start >= 0 and run_len >= 4:
        anchor = pattern[run_start : run_start + run_len]
        search_start = 0
        max_off = len(data) - len(pattern)
        while True:
            idx = data.find(anchor, search_start)
            if idx == -1:
                break
            candidate = idx - run_start
            if (
                0 <= candidate <= max_off
                and candidate % step == 0
                and bytes_match_at(data, pattern, strict_positions, candidate)
            ):
                out.append(candidate)
            search_start = idx + 1
        return out

    # Fallback for sparse masks with no useful strict run.
    first_pos = strict_positions[0]
    first_byte = bytes([pattern[first_pos]])
    search_start = 0
    max_off = len(data) - len(pattern)
    while True:
        idx = data.find(first_byte, search_start)
        if idx == -1:
            break
        candidate = idx - first_pos
        if (
            0 <= candidate <= max_off
            and candidate % step == 0
            and bytes_match_at(data, pattern, strict_positions, candidate)
        ):
            out.append(candidate)
        search_start = idx + 1

    return out


def is_arm_branch_or_call(word: int) -> bool:
    # ARM mode B / BL: cond(31:28), 1010/1011 at bits (27:24)
    op = word & 0x0F000000
    return op == 0x0A000000 or op == 0x0B000000


def decode_arm_branch_target(insn_va: int, word: int) -> Optional[int]:
    if not is_arm_branch_or_call(word):
        return None
    imm24 = word & 0x00FFFFFF
    if imm24 & 0x00800000:
        imm24 |= ~0x00FFFFFF
    offset = imm24 << 2
    return (insn_va + 8 + offset) & 0xFFFFFFFF


def is_thumb_branch16(halfword: int) -> bool:
    # Common 16-bit Thumb branch encodings.
    if (halfword & 0xF800) == 0xE000:  # B (T2)
        return True
    if (halfword & 0xF000) == 0xD000:  # B<cond> (T1) / SVC class
        return True
    return False


def is_thumb_branch32_prefix(h0: int, h1: int) -> bool:
    # Broad mask for 32-bit Thumb branch/link encodings (B.W / BL / BLX forms).
    if (h0 & 0xF800) in (0xF000, 0xF800) and (h1 & 0xC000) in (0x8000, 0xC000):
        return True
    return False


def looks_like_pointer(word: int, ranges: Sequence[Tuple[int, int]]) -> bool:
    for start, end in ranges:
        if start <= word < end:
            return True
    return False


def build_pattern_window(
    sec: SectionInfo,
    offset_in_sec: int,
    alloc_ranges: Sequence[Tuple[int, int]],
    context_before: int,
    context_after: int,
    mask_branch_words: bool,
    thumb_mode: bool,
) -> Tuple[bytes, bytes, int]:
    start = max(0, offset_in_sec - context_before)
    end = min(len(sec.data), offset_in_sec + context_after)
    pattern = bytearray(sec.data[start:end])
    mask = bytearray([1] * len(pattern))

    # Mask known-unstable words.
    for abs_off in range(start, end - 3):
        if abs_off % 4 != 0:
            continue
        i = abs_off - start
        word = struct.unpack_from("<I", sec.data, abs_off)[0]

        if ".text" in sec.name:
            if mask_branch_words and is_arm_branch_or_call(word):
                # Full branch-word masking is intentionally broad: branch
                # opcodes and encodings vary across regions and compilers.
                mask[i : i + 4] = b"\x00\x00\x00\x00"
            elif looks_like_pointer(word, alloc_ranges):
                # Literal-pool pointers embedded in .text can differ by region.
                mask[i : i + 4] = b"\x00\x00\x00\x00"
        else:
            # In data-like sections, pointers commonly differ across regions.
            if looks_like_pointer(word, alloc_ranges):
                mask[i : i + 4] = b"\x00\x00\x00\x00"

    # Additional Thumb-aware masking for mixed ARM/Thumb code areas.
    if ".text" in sec.name and mask_branch_words and thumb_mode:
        for abs_off in range(start, end - 1):
            if abs_off % 2 != 0:
                continue

            i = abs_off - start
            hw = struct.unpack_from("<H", sec.data, abs_off)[0]
            if is_thumb_branch16(hw):
                mask[i : i + 2] = b"\x00\x00"

            if abs_off + 3 < len(sec.data) and i + 3 < len(mask):
                hw2 = struct.unpack_from("<H", sec.data, abs_off + 2)[0]
                if is_thumb_branch32_prefix(hw, hw2):
                    mask[i : i + 4] = b"\x00\x00\x00\x00"

    # Keep center strict for .text. For data-like sections, mask center word
    # because the value at the target address is often region-dependent.
    center = offset_in_sec - start
    if ".text" in sec.name and 0 <= center < len(mask):
        # Keep existing decision for text center byte. If it was masked as part
        # of an unstable branch/pointer region, do not force it back to strict.
        pass
    elif 0 <= center < len(mask):
        word_start = center - (center % 4)
        if 0 <= word_start <= len(mask) - 4:
            mask[word_start : word_start + 4] = b"\x00\x00\x00\x00"

    return bytes(pattern), bytes(mask), center


def section_preference(section_name: str) -> int:
    if ".text" in section_name:
        return 0
    if ".rodata" in section_name:
        return 1
    if ".data" in section_name:
        return 2
    if ".bss" in section_name:
        return 3
    return 9


def unique_match_in_target(
    target: ElfImage,
    source_section: SectionInfo,
    pattern: bytes,
    mask: bytes,
) -> List[Match]:
    candidates: List[Match] = []

    for sec in target.alloc_sections:
        if not sec.has_bytes:
            continue

        # Prefer same section class first, but still search other sections.
        same_class = section_preference(sec.name) == section_preference(source_section.name)
        offsets = find_masked_matches(sec.data, pattern, mask)
        for off in offsets:
            candidates.append(Match(section=sec.name, section_offset=off, va=sec.addr + off))

        # Cheap optimization: if a same-class section already has one unique candidate,
        # keep gathering globally anyway; ambiguity is handled by caller.
        _ = same_class

    return candidates


def build_arm_block_signature(
    sec: SectionInfo,
    source_offset: int,
    before_instructions: int,
    after_instructions: int,
) -> Tuple[bytes, bytes, int]:
    start = max(0, source_offset - before_instructions * 4)
    end = min(len(sec.data), source_offset + after_instructions * 4)
    start -= start % 4
    end -= (end - start) % 4
    pattern = bytearray(sec.data[start:end])
    mask = bytearray([1] * len(pattern))

    # Normalize branch displacements but keep opcode+condition byte.
    for i in range(0, len(pattern) - 3, 4):
        word = struct.unpack_from("<I", pattern, i)[0]
        if is_arm_branch_or_call(word):
            mask[i : i + 3] = b"\x00\x00\x00"

    center = source_offset - start
    return bytes(pattern), bytes(mask), center


def build_thumb_block_signature(
    sec: SectionInfo,
    source_offset: int,
    before_halfwords: int,
    after_halfwords: int,
) -> Tuple[bytes, bytes, int]:
    start = max(0, source_offset - before_halfwords * 2)
    end = min(len(sec.data), source_offset + after_halfwords * 2)
    start -= start % 2
    end -= (end - start) % 2
    pattern = bytearray(sec.data[start:end])
    mask = bytearray([1] * len(pattern))

    for i in range(0, len(pattern) - 1, 2):
        hw = struct.unpack_from("<H", pattern, i)[0]
        if is_thumb_branch16(hw):
            mask[i : i + 2] = b"\x00\x00"
            continue

        if i + 3 < len(pattern):
            hw2 = struct.unpack_from("<H", pattern, i + 2)[0]
            if is_thumb_branch32_prefix(hw, hw2):
                mask[i : i + 4] = b"\x00\x00\x00\x00"

    center = source_offset - start
    return bytes(pattern), bytes(mask), center


def port_by_arm_block_match(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
) -> Tuple[Optional[int], str, str]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return None, "arm-block", "source address not mapped"
    src_sec, src_off = located

    if ".text" not in src_sec.name or (source_va & 0x3) != 0:
        return None, "arm-block", "not ARM text-aligned source"

    # Stage 1: legacy forward signature from source instruction; this path was
    # empirically reliable for many ARM-aligned function addresses.
    for insn_count in (20, 32, 48, 64):
        pattern, mask, center = build_arm_block_signature(
            src_sec,
            src_off,
            before_instructions=0,
            after_instructions=insn_count,
        )
        if len(pattern) < 16:
            continue

        candidates: List[int] = []
        for sec in dst.alloc_sections:
            if not sec.has_bytes or ".text" not in sec.name:
                continue
            offs = find_masked_matches(sec.data, pattern, mask, step=4)
            for off in offs:
                candidates.append(sec.addr + off + center)

        if len(candidates) == 1:
            return candidates[0], "arm-block", f"unique ARM block match ({insn_count} insn forward)"

        if len(candidates) > 1:
            local = [c for c in candidates if abs((c - src_sec.addr) - src_off) <= 0x4000]
            if len(local) == 1:
                return local[0], "arm-block-local", f"unique local ARM match ({insn_count} insn forward)"

    # Stage 2: centered variants for cases where source instruction sits in the
    # middle of a more characteristic surrounding block.
    for before, after in ((12, 12), (16, 16), (20, 20), (24, 24), (8, 24), (24, 8)):
        pattern, mask, center = build_arm_block_signature(
            src_sec,
            src_off,
            before_instructions=before,
            after_instructions=after,
        )
        if len(pattern) < 16:
            continue

        candidates: List[int] = []
        for sec in dst.alloc_sections:
            if not sec.has_bytes or ".text" not in sec.name:
                continue
            offs = find_masked_matches(sec.data, pattern, mask, step=4)
            for off in offs:
                candidates.append(sec.addr + off + center)

        if len(candidates) == 1:
            return candidates[0], "arm-block", f"unique ARM block match ({before}+{after} insn)"

        if len(candidates) > 1:
            local = [c for c in candidates if abs((c - src_sec.addr) - src_off) <= 0x4000]
            if len(local) == 1:
                return local[0], "arm-block-local", f"unique local ARM match ({before}+{after} insn)"

    return None, "arm-block", "no unique ARM instruction block match"


def port_by_thumb_block_match(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
) -> Tuple[Optional[int], str, str]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return None, "thumb-block", "source address not mapped"
    src_sec, src_off = located

    # Tables often store Thumb sites as even addresses (LSB not set).
    # Use halfword/word alignment to classify Thumb-like positions.
    if ".text" not in src_sec.name or (source_va & 0x2) == 0:
        return None, "thumb-block", "not Thumb-aligned source"

    for before, after in ((16, 16), (24, 24), (32, 32), (12, 28), (28, 12)):
        pattern, mask, center = build_thumb_block_signature(
            src_sec,
            src_off,
            before_halfwords=before,
            after_halfwords=after,
        )
        if len(pattern) < 12:
            continue

        candidates: List[int] = []
        for sec in dst.alloc_sections:
            if not sec.has_bytes or ".text" not in sec.name:
                continue
            offs = find_masked_matches(sec.data, pattern, mask, step=2)
            for off in offs:
                candidates.append(sec.addr + off + center)

        if len(candidates) == 1:
            return candidates[0], "thumb-block", f"unique Thumb block match ({before}+{after} hwords)"

        if len(candidates) > 1:
            local = [c for c in candidates if abs((c - src_sec.addr) - src_off) <= 0x4000]
            if len(local) == 1:
                return local[0], "thumb-block-local", f"unique local Thumb match ({before}+{after} hwords)"

    return None, "thumb-block", "no unique Thumb instruction block match"


def find_arm_branch_callers(image: ElfImage, target_va: int) -> List[int]:
    callers: List[int] = []
    for sec in image.alloc_sections:
        if not sec.has_bytes or ".text" not in sec.name:
            continue
        for off in range(0, len(sec.data) - 3, 4):
            word = struct.unpack_from("<I", sec.data, off)[0]
            if not is_arm_branch_or_call(word):
                continue
            va = sec.addr + off
            dst = decode_arm_branch_target(va, word)
            if dst == target_va:
                callers.append(va)
    return callers


def is_arm_prologue_word(word: int) -> bool:
    # Common ARM function prologues (push/stmdb sp!, ...).
    if (word & 0xFFFF0000) == 0xE92D0000:
        return True
    if (word & 0xFFFF0000) == 0xE52D0000:
        return True
    return False


def find_arm_function_start(src: ElfImage, source_va: int, max_back: int = 0x300) -> Optional[int]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return None
    sec, off = located
    if ".text" not in sec.name or (source_va & 0x3) != 0:
        return None

    back = min(max_back, off)
    for rel in range(0, back + 1, 4):
        cur_off = off - rel
        if cur_off < 0 or cur_off + 4 > len(sec.data):
            continue
        word = struct.unpack_from("<I", sec.data, cur_off)[0]
        if is_arm_prologue_word(word):
            return sec.addr + cur_off
    return None


def port_by_arm_function_offset(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    start_va = find_arm_function_start(src, source_va)
    if start_va is None:
        return None, "arm-func-off", "no ARM function prologue found"

    intra = source_va - start_va
    mapped_start, method, note = port_by_direct_pattern(
        src=src,
        dst=dst,
        source_va=start_va,
        context_before=max(context_before, 32),
        context_after=max(context_after, 64),
    )
    if mapped_start is None:
        mapped_start, method2, note2 = port_by_arm_block_match(
            src=src,
            dst=dst,
            source_va=start_va,
        )
        if mapped_start is None:
            return None, "arm-func-off", f"start map failed ({method}: {note}; {method2}: {note2})"

    mapped = mapped_start + intra
    mapped_loc = dst.find_section_for_va(mapped)
    if mapped_loc is None or ".text" not in mapped_loc[0].name:
        return None, "arm-func-off", "mapped intra-function address not in target text"

    return mapped, "arm-func-off", f"mapped via function start +0x{intra:X}"


def port_by_arm_thunk_target(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    src_word = src.read_u32(source_va)
    if src_word is None:
        return None, "arm-thunk", "unable to read source instruction"

    target_va = decode_arm_branch_target(source_va, src_word)
    if target_va is None:
        return None, "arm-thunk", "source is not ARM B/BL"

    mapped_target, _method, _note = port_by_direct_pattern(
        src=src,
        dst=dst,
        source_va=target_va,
        context_before=context_before,
        context_after=context_after,
    )
    if mapped_target is None:
        mapped_target, _method2, _note2 = port_by_arm_block_match(
            src=src,
            dst=dst,
            source_va=target_va,
        )
    if mapped_target is None:
        return None, "arm-thunk", "could not map thunk target function"

    src_loc = src.find_section_for_va(source_va)
    if src_loc is None:
        return None, "arm-thunk", "source location missing"
    src_sec, src_off = src_loc

    candidates: List[int] = []
    for sec in dst.alloc_sections:
        if not sec.has_bytes or ".text" not in sec.name:
            continue
        for off in range(0, len(sec.data) - 3, 4):
            va = sec.addr + off
            word = struct.unpack_from("<I", sec.data, off)[0]
            t = decode_arm_branch_target(va, word)
            if t == mapped_target:
                candidates.append(va)

    if not candidates:
        return None, "arm-thunk", "no thunk branch to mapped target"

    caller_votes: dict[int, int] = {}
    src_callers = find_arm_branch_callers(src, source_va)
    for src_caller in src_callers:
        mapped_caller, _m, _n = port_by_direct_pattern(
            src=src,
            dst=dst,
            source_va=src_caller,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_caller is None:
            mapped_caller, _m2, _n2 = port_by_arm_block_match(
                src=src,
                dst=dst,
                source_va=src_caller,
            )
        if mapped_caller is None:
            continue

        dst_word = dst.read_u32(mapped_caller)
        if dst_word is None:
            continue
        dst_target = decode_arm_branch_target(mapped_caller, dst_word)
        if dst_target in candidates:
            caller_votes[dst_target] = caller_votes.get(dst_target, 0) + 1

    if caller_votes:
        best = max(caller_votes, key=caller_votes.get)
        if caller_votes[best] >= 1:
            return best, "arm-thunk-xref", f"selected by mapped caller xrefs ({caller_votes[best]})"

    if len(candidates) > 1:
        local_candidates: List[Tuple[int, int]] = []
        for cand in candidates:
            dst_loc = dst.find_section_for_va(cand)
            if dst_loc is None:
                continue
            _dst_sec, dst_off = dst_loc
            local_candidates.append((cand, dst_off))

        if local_candidates:
            local_candidates.sort(key=lambda item: abs(item[1] - src_off))
            best_cand, best_off = local_candidates[0]
            best_diff = abs(best_off - src_off)
            second_diff = abs(local_candidates[1][1] - src_off) if len(local_candidates) > 1 else 0x7FFFFFFF
            if best_diff <= 0x4000 and (second_diff - best_diff) >= 0x100:
                return best_cand, "arm-thunk-local", f"selected by local offset (diff 0x{best_diff:X})"

    # If callsite xrefs are not sufficient, disambiguate by matching context
    # around the branch-site (with branch/pointer words masked).
    src_loc_ctx = src.find_section_for_va(source_va)
    if src_loc_ctx is not None:
        src_sec_ctx, src_off_ctx = src_loc_ctx
        if ".text" in src_sec_ctx.name:
            score_pattern, score_mask, _ = build_pattern_window(
                sec=src_sec_ctx,
                offset_in_sec=src_off_ctx,
                alloc_ranges=src.all_alloc_ranges(),
                context_before=max(64, context_before),
                context_after=max(64, context_after),
                mask_branch_words=True,
                thumb_mode=bool(source_va & 0x2),
            )
            strict_positions = [i for i, m in enumerate(score_mask) if m]
            if strict_positions:
                best_cand: Optional[int] = None
                best_score = -1
                second_score = -1
                for cand in candidates:
                    dst_loc_ctx = dst.find_section_for_va(cand)
                    if dst_loc_ctx is None:
                        continue
                    dst_sec_ctx, dst_off_ctx = dst_loc_ctx
                    if ".text" not in dst_sec_ctx.name:
                        continue

                    win_start = dst_off_ctx - max(64, context_before)
                    if win_start < 0:
                        continue
                    win_end = win_start + len(score_pattern)
                    if win_end > len(dst_sec_ctx.data):
                        continue

                    matched = count_strict_matches_at(
                        dst_sec_ctx.data,
                        score_pattern,
                        strict_positions,
                        win_start,
                    )
                    if matched > best_score:
                        second_score = best_score
                        best_score = matched
                        best_cand = cand
                    elif matched > second_score:
                        second_score = matched

                if best_cand is not None and best_score >= 8 and (best_score - second_score) >= 2:
                    return best_cand, "arm-thunk-context", f"selected by branch-site context ({best_score} strict bytes)"

    if len(candidates) == 1:
        return candidates[0], "arm-thunk-target", "unique thunk branch to mapped target"

    return None, "arm-thunk", f"ambiguous thunk matches ({len(candidates)})"


def find_section_local_match(
    src: ElfImage,
    dst: ElfImage,
    source_section: SectionInfo,
    source_offset: int,
    pattern: bytes,
    mask: bytes,
    center: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str]:
    preferred_sections = [s for s in dst.alloc_sections if s.has_bytes and s.name == source_section.name]
    if not preferred_sections:
        preferred_sections = [
            s
            for s in dst.alloc_sections
            if s.has_bytes and section_preference(s.name) == section_preference(source_section.name)
        ]

    # Most data/rodata drifts are small. Widen progressively when needed.
    radii = (0x80, 0x200, 0x800, 0x2000)
    for sec in preferred_sections:
        for radius in radii:
            search_start = max(0, source_offset - radius - context_before)
            search_end = min(len(sec.data), source_offset + radius + context_after)
            if search_end - search_start < len(pattern):
                continue

            sub = sec.data[search_start:search_end]
            offs = find_masked_matches(sub, pattern, mask)
            abs_offs = [search_start + o for o in offs]
            if len(abs_offs) == 1:
                return sec.addr + abs_offs[0] + center, f"unique near +/-0x{radius:X} in {sec.name}"

    return None, "no unique local match"


def port_by_direct_pattern(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return None, "none", "source address not mapped in source ELF"

    src_sec, src_off = located
    if not src_sec.has_bytes:
        return None, "none", f"source in {src_sec.name} (no raw bytes, likely .bss)"

    pattern, mask, center = build_pattern_window(
        sec=src_sec,
        offset_in_sec=src_off,
        alloc_ranges=src.all_alloc_ranges(),
        context_before=context_before,
        context_after=context_after,
        mask_branch_words=True,
        thumb_mode=bool(source_va & 0x2),
    )

    if ".text" not in src_sec.name:
        local_mapped, local_note = find_section_local_match(
            src=src,
            dst=dst,
            source_section=src_sec,
            source_offset=src_off,
            pattern=pattern,
            mask=mask,
            center=center,
            context_before=context_before,
            context_after=context_after,
        )
        if local_mapped is not None:
            return local_mapped, "section-local-pattern", local_note

    matches = unique_match_in_target(dst, src_sec, pattern, mask)
    if len(matches) == 1:
        mapped = matches[0].va + center
        return mapped, "direct-pattern", f"unique match in {matches[0].section}"

    # If ambiguous, try exact bytes (no masking) to disambiguate non-text/data cases.
    if len(matches) > 1:
        exact_mask = bytes([1] * len(mask))
        exact_matches = unique_match_in_target(dst, src_sec, pattern, exact_mask)
        if len(exact_matches) == 1:
            mapped = exact_matches[0].va + center
            return mapped, "direct-pattern-exact", "resolved ambiguity with exact bytes"
        return None, "ambiguous", f"{len(matches)} masked matches"

    return None, "no-match", "pattern not found"


def collect_direct_pattern_candidates(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> List[int]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return []

    src_sec, src_off = located
    if not src_sec.has_bytes:
        return []

    pattern, mask, center = build_pattern_window(
        sec=src_sec,
        offset_in_sec=src_off,
        alloc_ranges=src.all_alloc_ranges(),
        context_before=context_before,
        context_after=context_after,
        mask_branch_words=True,
        thumb_mode=bool(source_va & 0x2),
    )

    matches = unique_match_in_target(dst, src_sec, pattern, mask)
    out: List[int] = []
    for m in matches:
        out.append(m.va + center)
    return out


def iter_adaptive_text_windows(
    context_before: int,
    context_after: int,
) -> Iterable[Tuple[int, int]]:
    # Try narrower windows first to break ambiguous large-context matches.
    seen: set[Tuple[int, int]] = set()
    candidates = (
        (8, 8),
        (12, 12),
        (16, 16),
        (20, 20),
        (24, 24),
        (32, 32),
        (40, 40),
    )

    for before, after in candidates:
        key = (before, after)
        if key in seen or key == (context_before, context_after):
            continue
        seen.add(key)
        yield key


def port_by_text_local_micro_window(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
) -> Tuple[Optional[int], str, str]:
    src_loc = src.find_section_for_va(source_va)
    if src_loc is None:
        return None, "text-local-micro", "source address not mapped"

    src_sec, src_off = src_loc
    if ".text" not in src_sec.name or not src_sec.has_bytes:
        return None, "text-local-micro", "source is not byte-backed text"

    if (source_va & 0x3) != 0:
        return None, "text-local-micro", "source is not ARM word-aligned"

    src_word = src.read_u32(source_va)
    if src_word is not None and is_arm_branch_or_call(src_word):
        return None, "text-local-micro", "source is ARM branch/call"

    for before, after in ((8, 8), (12, 12), (16, 16)):
        pattern, mask, center = build_pattern_window(
            sec=src_sec,
            offset_in_sec=src_off,
            alloc_ranges=src.all_alloc_ranges(),
            context_before=before,
            context_after=after,
            mask_branch_words=True,
            thumb_mode=bool(source_va & 0x2),
        )
        if len(pattern) < 8:
            continue

        mapped, note = find_section_local_match(
            src=src,
            dst=dst,
            source_section=src_sec,
            source_offset=src_off,
            pattern=pattern,
            mask=mask,
            center=center,
            context_before=before,
            context_after=after,
        )
        if mapped is not None:
            return mapped, "text-local-micro", f"window -0x{before:X}/+0x{after:X}; {note}"

    return None, "text-local-micro", "no unique local micro-window text match"


def port_by_nobits_section_offset(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
) -> Tuple[Optional[int], str, str]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return None, "nobits-offset", "source address not mapped"

    src_sec, src_off = located
    if src_sec.has_bytes:
        return None, "nobits-offset", "source has raw bytes"

    same_name = [
        s for s in dst.alloc_sections
        if s.name == src_sec.name and s.size > src_off
    ]
    if len(same_name) == 1:
        raw_delta = same_name[0].addr - src_sec.addr
        page_delta = int(round(raw_delta / float(0x1000))) * 0x1000
        mapped = source_va + page_delta
        dst_loc = dst.find_section_for_va(mapped)
        if dst_loc is not None and dst_loc[0].name == same_name[0].name:
            return mapped, "nobits-page-delta", f"mapped by rounded section delta 0x{page_delta:+X}"

        mapped = same_name[0].addr + src_off
        return mapped, "nobits-offset", f"mapped by {src_sec.name} offset +0x{src_off:X}"

    same_kind = [
        s for s in dst.alloc_sections
        if s.sh_type == src_sec.sh_type and section_preference(s.name) == section_preference(src_sec.name) and s.size > src_off
    ]
    if len(same_kind) == 1:
        raw_delta = same_kind[0].addr - src_sec.addr
        page_delta = int(round(raw_delta / float(0x1000))) * 0x1000
        mapped = source_va + page_delta
        dst_loc = dst.find_section_for_va(mapped)
        if dst_loc is not None:
            return mapped, "nobits-page-delta-kind", f"mapped by rounded kind-delta 0x{page_delta:+X}"

        mapped = same_kind[0].addr + src_off
        return mapped, "nobits-offset-kind", f"mapped by section-kind offset +0x{src_off:X}"

    return None, "nobits-offset", "no unique target NOBITS section candidate"


def port_by_text_callsite_xref(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    src_loc = src.find_section_for_va(source_va)
    if src_loc is None:
        return None, "text-callsite-xref", "source address not mapped"
    src_sec, src_off = src_loc

    if ".text" not in src_sec.name or (source_va & 0x3) != 0:
        return None, "text-callsite-xref", "source is not ARM text"

    candidates = collect_direct_pattern_candidates(
        src=src,
        dst=dst,
        source_va=source_va,
        context_before=context_before,
        context_after=context_after,
    )

    uniq: List[int] = []
    seen: set[int] = set()
    for c in candidates:
        if c in seen:
            continue
        seen.add(c)
        loc = dst.find_section_for_va(c)
        if loc is None or ".text" not in loc[0].name:
            continue
        uniq.append(c)

    if len(uniq) < 2 or len(uniq) > 512:
        return None, "text-callsite-xref", f"candidate set size {len(uniq)} not suitable"

    cand_set = set(uniq)
    votes: dict[int, int] = {}
    src_callers = find_arm_branch_callers(src, source_va)
    if not src_callers:
        return None, "text-callsite-xref", "no ARM callers to source"

    for src_caller in src_callers[:256]:
        mapped_caller, _m, _n = port_by_direct_pattern(
            src=src,
            dst=dst,
            source_va=src_caller,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_caller is None:
            mapped_caller, _m2, _n2 = port_by_arm_block_match(
                src=src,
                dst=dst,
                source_va=src_caller,
            )
        if mapped_caller is None:
            continue

        dst_word = dst.read_u32(mapped_caller)
        if dst_word is None:
            continue
        dst_target = decode_arm_branch_target(mapped_caller, dst_word)
        if dst_target in cand_set:
            votes[dst_target] = votes.get(dst_target, 0) + 1

    if not votes:
        return None, "text-callsite-xref", "no mapped caller voted for candidates"

    ranked = sorted(votes.items(), key=lambda kv: kv[1], reverse=True)
    best_addr, best_votes = ranked[0]
    second_votes = ranked[1][1] if len(ranked) > 1 else 0

    if best_votes >= 2 and (best_votes - second_votes) >= 1:
        return best_addr, "text-callsite-xref", f"selected by mapped callers ({best_votes} votes)"

    tied = [addr for addr, v in ranked if v == best_votes]
    local_best = min(tied, key=lambda addr: abs((addr - dst.alloc_sections[0].addr) - (source_va - src.alloc_sections[0].addr)))
    return local_best, "text-callsite-xref-local", f"tie-broken locally ({best_votes} votes)"


def port_by_text_callsite_majority(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    src_loc = src.find_section_for_va(source_va)
    if src_loc is None:
        return None, "text-callsite-majority", "source address not mapped"
    src_sec, _src_off = src_loc

    if ".text" not in src_sec.name or (source_va & 0x3) != 0:
        return None, "text-callsite-majority", "source is not ARM text"

    src_callers = find_arm_branch_callers(src, source_va)
    if not src_callers:
        return None, "text-callsite-majority", "no ARM callers to source"

    votes: dict[int, int] = {}
    mapped_callers = 0
    adaptive_budget = 64

    for src_caller in src_callers[:1024]:
        mapped_caller, _m, _n = port_by_direct_pattern(
            src=src,
            dst=dst,
            source_va=src_caller,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_caller is None and adaptive_budget > 0:
            for adaptive_before, adaptive_after in iter_adaptive_text_windows(context_before, context_after):
                mapped_caller, _m2, _n2 = port_by_direct_pattern(
                    src=src,
                    dst=dst,
                    source_va=src_caller,
                    context_before=adaptive_before,
                    context_after=adaptive_after,
                )
                if mapped_caller is not None:
                    break
            adaptive_budget -= 1

        if mapped_caller is None:
            mapped_caller, _m3, _n3 = port_by_arm_block_match(
                src=src,
                dst=dst,
                source_va=src_caller,
            )
        if mapped_caller is None:
            continue

        mapped_callers += 1
        dst_word = dst.read_u32(mapped_caller)
        if dst_word is None:
            continue

        dst_target = decode_arm_branch_target(mapped_caller, dst_word)
        if dst_target is None:
            continue

        dst_target_loc = dst.find_section_for_va(dst_target)
        if dst_target_loc is None or ".text" not in dst_target_loc[0].name:
            continue

        votes[dst_target] = votes.get(dst_target, 0) + 1

    if not votes or mapped_callers < 2:
        return None, "text-callsite-majority", "insufficient mapped caller signal"

    ranked = sorted(votes.items(), key=lambda kv: kv[1], reverse=True)
    best_addr, best_votes = ranked[0]
    second_votes = ranked[1][1] if len(ranked) > 1 else 0
    ratio = best_votes / float(mapped_callers)

    if best_votes >= 2 and (best_votes - second_votes) >= 2 and ratio >= 0.85:
        return best_addr, "text-callsite-majority", f"mapped callers {mapped_callers}; votes {best_votes}"

    return None, "text-callsite-majority", f"ambiguous caller majority ({best_votes}/{mapped_callers})"


def iter_literal_occurrences(image: ElfImage, value: int) -> Iterable[Tuple[SectionInfo, int, int]]:
    needle = struct.pack("<I", value)
    for sec in image.alloc_sections:
        if not sec.has_bytes or len(sec.data) < 4:
            continue
        start = 0
        while True:
            idx = sec.data.find(needle, start)
            if idx == -1:
                break
            yield sec, idx, sec.addr + idx
            start = idx + 1


def port_by_literal_xref(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
    max_refs: int,
) -> Tuple[Optional[int], str, str]:
    refs = list(iter_literal_occurrences(src, source_va))
    if not refs:
        return None, "xref", "no literal 32-bit xrefs found in source ELF"

    refs = refs[:max_refs]
    mapped_values: List[int] = []

    for sec, lit_off, _lit_va in refs:
        pattern, mask, center = build_pattern_window(
            sec=sec,
            offset_in_sec=lit_off,
            alloc_ranges=src.all_alloc_ranges(),
            context_before=context_before,
            context_after=context_after,
            mask_branch_words=True,
            thumb_mode=False,
        )
        matches = unique_match_in_target(dst, sec, pattern, mask)
        if len(matches) != 1:
            continue

        literal_va_dst = matches[0].va + center
        maybe_ptr = dst.read_u32(literal_va_dst)
        if maybe_ptr is None:
            continue
        mapped_values.append(maybe_ptr)

    if not mapped_values:
        return None, "xref", "xrefs found, but no unique mapped literal site"

    # Majority vote for stability.
    best_val = max(set(mapped_values), key=mapped_values.count)
    vote_count = mapped_values.count(best_val)
    confidence = "strong" if vote_count >= 2 else "medium"
    return best_val, f"literal-xref-{confidence}", f"votes {vote_count}/{len(mapped_values)}"


def port_by_nearby_text_anchor(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    located = src.find_section_for_va(source_va)
    if located is None:
        return None, "nearby-anchor", "source address not mapped"

    src_sec, src_off = located
    if ".text" not in src_sec.name:
        return None, "nearby-anchor", "source not in text section"

    # Probe nearby Thumb/ARM instruction addresses and transfer relative offset.
    deltas = [
        d
        for radius in range(2, 0xA2, 2)
        for d in (-radius, radius)
    ]

    votes: dict[int, int] = {}
    reasons: dict[int, str] = {}
    adaptive_probe_budget = 12

    for delta in deltas:
        probe_off = src_off + delta
        if probe_off < 0 or probe_off >= src_sec.size:
            continue

        probe_va = source_va + delta
        mapped_probe, method, _note = port_by_direct_pattern(
            src=src,
            dst=dst,
            source_va=probe_va,
            context_before=context_before,
            context_after=context_after,
        )

        # Guard against combinatorial blow-ups in dense text areas.
        # Only spend limited adaptive probes, and only near the source.
        if (
            mapped_probe is None
            and adaptive_probe_budget > 0
            and abs(delta) <= 0x20
            and (delta & 0x3) == 0
        ):
            for adaptive_before, adaptive_after in ((12, 12), (16, 16), (20, 20), (24, 24)):
                mapped_probe, _m2, _n2 = port_by_direct_pattern(
                    src=src,
                    dst=dst,
                    source_va=probe_va,
                    context_before=adaptive_before,
                    context_after=adaptive_after,
                )
                if mapped_probe is not None:
                    method = "nearby-probe-adaptive"
                    adaptive_probe_budget -= 1
                    break

        if mapped_probe is None:
            continue

        mapped = mapped_probe - delta
        dst_loc = dst.find_section_for_va(mapped)
        if dst_loc is None:
            continue
        if ".text" not in dst_loc[0].name:
            continue

        votes[mapped] = votes.get(mapped, 0) + 1
        if mapped not in reasons:
            reasons[mapped] = f"delta {delta:+#x} via {method}"

    if not votes:
        return None, "nearby-anchor", "no nearby anchor resolved"

    ranked = sorted(votes.items(), key=lambda kv: kv[1], reverse=True)

    if (source_va & 0x3) == 0:
        src_callers = find_arm_branch_callers(src, source_va)
        if src_callers:
            candidate_set = {addr for addr, _v in ranked[:16]}
            caller_votes: dict[int, int] = {}

            for src_caller in src_callers[:256]:
                mapped_caller, _m, _n = port_by_direct_pattern(
                    src=src,
                    dst=dst,
                    source_va=src_caller,
                    context_before=context_before,
                    context_after=context_after,
                )
                if mapped_caller is None:
                    mapped_caller, _m2, _n2 = port_by_arm_block_match(
                        src=src,
                        dst=dst,
                        source_va=src_caller,
                    )
                if mapped_caller is None:
                    continue

                dst_word = dst.read_u32(mapped_caller)
                if dst_word is None:
                    continue

                dst_target = decode_arm_branch_target(mapped_caller, dst_word)
                if dst_target in candidate_set:
                    caller_votes[dst_target] = caller_votes.get(dst_target, 0) + 1

            if caller_votes:
                caller_ranked = sorted(caller_votes.items(), key=lambda kv: kv[1], reverse=True)
                caller_best_addr, caller_best_votes = caller_ranked[0]
                caller_second_votes = caller_ranked[1][1] if len(caller_ranked) > 1 else 0
                if caller_best_votes >= 2 and (caller_best_votes - caller_second_votes) >= 1:
                    return (
                        caller_best_addr,
                        "nearby-text-callsite-xref",
                        f"caller votes {caller_best_votes}; nearby vote {votes.get(caller_best_addr, 0)}",
                    )

    best_addr = ranked[0][0]
    best_votes = votes[best_addr]
    if best_votes < 2:
        return None, "nearby-anchor", "nearby anchors did not agree"

    # Second-stage refinement: among top vote candidates, pick the one with the
    # strongest local masked-context similarity around the source address.
    score_pattern, score_mask, _ = build_pattern_window(
        sec=src_sec,
        offset_in_sec=src_off,
        alloc_ranges=src.all_alloc_ranges(),
        context_before=max(64, context_before),
        context_after=max(64, context_after),
        mask_branch_words=True,
        thumb_mode=bool(source_va & 0x2),
    )
    strict_positions = [i for i, m in enumerate(score_mask) if m]

    if strict_positions:
        top_addrs = [addr for addr, _v in ranked[:16]]

        best_refined = best_addr
        best_refined_score = -1
        best_refined_votes = votes.get(best_addr, 0)

        step = 4 if (source_va & 0x3) == 0 else 2
        center_bias = max(64, context_before + context_after)

        for cand in top_addrs:
            loc = dst.find_section_for_va(cand)
            if loc is None:
                continue
            sec, off = loc
            if ".text" not in sec.name:
                continue

            local_best = -1
            local_best_abs_delta = 0x7FFFFFFF

            for d in range(-0x120, 0x121, step):
                probe_off = off + d
                win_start = probe_off - max(64, context_before)
                if win_start < 0:
                    continue
                win_end = win_start + len(score_pattern)
                if win_end > len(sec.data):
                    continue

                matched = count_strict_matches_at(sec.data, score_pattern, strict_positions, win_start)
                abs_delta = abs(d)
                if matched > local_best or (matched == local_best and abs_delta < local_best_abs_delta):
                    local_best = matched
                    local_best_abs_delta = abs_delta

            if local_best < 0:
                continue

            # Favor vote count first, then contextual similarity, then proximity.
            local_addr_delta = abs(off - src_off)
            combined = (
                local_best * 100_000
                + votes[cand] * 1_000
                + max(0, center_bias - local_best_abs_delta)
                + max(0, 0x6000 - local_addr_delta)
            )
            if combined > best_refined_score:
                best_refined_score = combined
                best_refined = cand
                best_refined_votes = votes[cand]

        if best_refined != best_addr:
            return (
                best_refined,
                "nearby-text-refined",
                f"vote {best_refined_votes}; {reasons.get(best_refined, 'refined selection')}"
            )

    return best_addr, "nearby-text-anchor", f"vote {best_votes}; {reasons[best_addr]}"


def port_by_nearby_data_anchor(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
) -> Tuple[Optional[int], str, str]:
    src_loc = src.find_section_for_va(source_va)
    if src_loc is None:
        return None, "nearby-data-anchor", "source address not mapped"
    src_sec, src_off = src_loc

    if ".text" in src_sec.name or not src_sec.has_bytes:
        return None, "nearby-data-anchor", "source is not byte-backed non-text"

    deltas = [d for radius in range(1, 0x181) for d in (-radius, radius)]

    votes: dict[int, int] = {}
    reasons: dict[int, str] = {}

    probes = 0
    for delta in deltas:
        probe_off = src_off + delta
        if probe_off < 0 or probe_off >= src_sec.size:
            continue

        probes += 1

        probe_va = source_va + delta
        probe_loc = src.find_section_for_va(probe_va)
        if probe_loc is None:
            continue
        probe_sec, probe_off = probe_loc

        pattern, mask, center = build_pattern_window(
            sec=probe_sec,
            offset_in_sec=probe_off,
            alloc_ranges=src.all_alloc_ranges(),
            context_before=min(24, context_before),
            context_after=min(24, context_after),
            mask_branch_words=False,
            thumb_mode=False,
        )

        mapped_probe, local_note = find_section_local_match(
            src=src,
            dst=dst,
            source_section=probe_sec,
            source_offset=probe_off,
            pattern=pattern,
            mask=mask,
            center=center,
            context_before=min(24, context_before),
            context_after=min(24, context_after),
        )
        method = "section-local-probe"
        if mapped_probe is None:
            continue

        cand = mapped_probe - delta
        dst_loc = dst.find_section_for_va(cand)
        if dst_loc is None:
            continue
        if dst_loc[0].name != src_sec.name:
            continue

        votes[cand] = votes.get(cand, 0) + 1
        if cand not in reasons:
            reasons[cand] = f"delta {delta:+#x} via {method} ({local_note})"

        if probes >= 16 and votes:
            ranked_live = sorted(votes.items(), key=lambda kv: kv[1], reverse=True)
            live_best = ranked_live[0][1]
            live_second = ranked_live[1][1] if len(ranked_live) > 1 else 0
            if live_best >= 8 and (live_best - live_second) >= 4:
                break

    if not votes:
        return None, "nearby-data-anchor", "no nearby data anchor resolved"

    ranked = sorted(votes.items(), key=lambda kv: kv[1], reverse=True)
    best_addr, best_votes = ranked[0]
    second_votes = ranked[1][1] if len(ranked) > 1 else 0
    if best_votes < 4:
        return None, "nearby-data-anchor", "insufficient data-anchor agreement"
    if (best_votes - second_votes) < 2:
        return None, "nearby-data-anchor", "data anchors too ambiguous"

    return best_addr, "nearby-data-anchor", f"vote {best_votes}; {reasons[best_addr]}"


def port_address_to_region(
    src: ElfImage,
    dst: ElfImage,
    source_va: int,
    context_before: int,
    context_after: int,
    max_literal_refs: int,
    allow_risky_heuristics: bool = True,
) -> Tuple[Optional[int], str, str, str]:
    mapped, method, note = port_by_direct_pattern(
        src=src,
        dst=dst,
        source_va=source_va,
        context_before=context_before,
        context_after=context_after,
    )
    if mapped is not None:
        return mapped, method, "high", note

    src_loc = src.find_section_for_va(source_va)

    if src_loc is not None and not src_loc[0].has_bytes:
        mapped_xref_nobits, method_xref_nobits, note_xref_nobits = port_by_literal_xref(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
            max_refs=max_literal_refs,
        )
        if mapped_xref_nobits is not None:
            return mapped_xref_nobits, method_xref_nobits, "high", note_xref_nobits

        mapped_nobits, method_nobits, note_nobits = port_by_nobits_section_offset(
            src=src,
            dst=dst,
            source_va=source_va,
        )
        if mapped_nobits is not None:
            return mapped_nobits, method_nobits, "high", note_nobits

    if src_loc is not None and ".text" in src_loc[0].name and (source_va & 0x3) == 0:
        src_word = src.read_u32(source_va)
        if src_word is not None and is_arm_prologue_word(src_word):
            mapped_func, method_func, note_func = port_by_arm_function_offset(
                src=src,
                dst=dst,
                source_va=source_va,
                context_before=context_before,
                context_after=context_after,
            )
            if mapped_func is not None:
                return mapped_func, method_func, "high", note_func

    if src_loc is not None and ".text" in src_loc[0].name and (source_va & 0x3) == 0:
        mapped_xref_text, method_xref_text, note_xref_text = port_by_text_callsite_xref(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_xref_text is not None:
            return mapped_xref_text, method_xref_text, "high", note_xref_text

        mapped_majority, method_majority, note_majority = port_by_text_callsite_majority(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_majority is not None:
            return mapped_majority, method_majority, "high", note_majority

    if method == "no-match" and src_loc is not None and ".text" in src_loc[0].name:
        mapped_micro, method_micro, note_micro = port_by_text_local_micro_window(
            src=src,
            dst=dst,
            source_va=source_va,
        )
        if mapped_micro is not None:
            return mapped_micro, method_micro, "high", note_micro

    if src_loc is not None and ".text" in src_loc[0].name:
        for adaptive_before, adaptive_after in iter_adaptive_text_windows(context_before, context_after):
            mapped_adaptive, method_adaptive, note_adaptive = port_by_direct_pattern(
                src=src,
                dst=dst,
                source_va=source_va,
                context_before=adaptive_before,
                context_after=adaptive_after,
            )
            if mapped_adaptive is not None:
                dst_loc_adaptive = dst.find_section_for_va(mapped_adaptive)
                if dst_loc_adaptive is None:
                    continue
                dst_sec_adaptive, dst_off_adaptive = dst_loc_adaptive
                src_sec_adaptive, src_off_adaptive = src_loc
                if dst_sec_adaptive.name != src_sec_adaptive.name:
                    continue
                if abs(dst_off_adaptive - src_off_adaptive) > 0x20000:
                    continue

                if (source_va & 0x3) == 0:
                    src_word = src.read_u32(source_va)
                    dst_word = dst.read_u32(mapped_adaptive)
                    src_is_branch = False if src_word is None else is_arm_branch_or_call(src_word)
                    dst_is_branch = False if dst_word is None else is_arm_branch_or_call(dst_word)
                    if src_is_branch and not dst_is_branch:
                        continue

                    if src_is_branch and dst_is_branch and src_word is not None and dst_word is not None:
                        src_target = decode_arm_branch_target(source_va, src_word)
                        dst_target = decode_arm_branch_target(mapped_adaptive, dst_word)
                        if src_target is not None and dst_target is not None:
                            mapped_target, _mt_method, _mt_note = port_by_direct_pattern(
                                src=src,
                                dst=dst,
                                source_va=src_target,
                                context_before=context_before,
                                context_after=context_after,
                            )
                            if mapped_target is not None and mapped_target != dst_target:
                                continue

                return (
                    mapped_adaptive,
                    "direct-pattern-adaptive",
                    "high",
                    f"{method_adaptive} with window -0x{adaptive_before:X}/+0x{adaptive_after:X}: {note_adaptive}",
                )

    if src_loc is not None and ".text" in src_loc[0].name and (source_va & 0x3) == 0:
        mapped_arm, method_arm, note_arm = port_by_arm_block_match(
            src=src,
            dst=dst,
            source_va=source_va,
        )
        if mapped_arm is not None:
            return mapped_arm, method_arm, "high", note_arm

    if src_loc is not None and ".text" in src_loc[0].name and (source_va & 0x2) != 0:
        mapped_thumb, method_thumb, note_thumb = port_by_thumb_block_match(
            src=src,
            dst=dst,
            source_va=source_va,
        )
        if mapped_thumb is not None:
            return mapped_thumb, method_thumb, "high", note_thumb

    if allow_risky_heuristics and src_loc is not None and ".text" in src_loc[0].name and (source_va & 0x3) == 0:
        mapped_thunk, method_thunk, note_thunk = port_by_arm_thunk_target(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_thunk is not None:
            return mapped_thunk, method_thunk, "high", note_thunk

    if allow_risky_heuristics and src_loc is not None and ".text" in src_loc[0].name:
        mapped_near, method_near, note_near = port_by_nearby_text_anchor(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_near is not None:
            return mapped_near, method_near, "medium", note_near

    if src_loc is not None and ".text" not in src_loc[0].name and src_loc[0].has_bytes:
        mapped_data_xref, method_data_xref, note_data_xref = port_by_literal_xref(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
            max_refs=max_literal_refs,
        )
        if mapped_data_xref is not None:
            return mapped_data_xref, method_data_xref, "high", note_data_xref

        mapped_data_near, method_data_near, note_data_near = port_by_nearby_data_anchor(
            src=src,
            dst=dst,
            source_va=source_va,
            context_before=context_before,
            context_after=context_after,
        )
        if mapped_data_near is not None:
            return mapped_data_near, method_data_near, "high", note_data_near

    mapped_xref, method_xref, note_xref = port_by_literal_xref(
        src=src,
        dst=dst,
        source_va=source_va,
        context_before=context_before,
        context_after=context_after,
        max_refs=max_literal_refs,
    )
    if mapped_xref is not None:
        return mapped_xref, method_xref, "medium", note_xref

    # Bubble up the most informative direct-match failure note.
    if method in ("ambiguous", "none"):
        return None, method, "low", f"direct failed: {note}; xref failed: {note_xref}"
    return None, "unresolved", "low", f"direct failed: {note}; xref failed: {note_xref}"


def format_addr(value: Optional[int]) -> str:
    return "-" if value is None else f"0x{value:08X}"


def resolve_targets(
    source_elf_path: Path,
    target_elf_paths: Iterable[Path],
    source_va: int,
    context_before: int,
    context_after: int,
    max_literal_refs: int,
    allow_risky_heuristics: bool,
) -> List[PortResult]:
    source_elf = ElfImage(source_elf_path)

    results: List[PortResult] = []
    for target_path in target_elf_paths:
        dst = ElfImage(target_path)
        mapped, method, confidence, note = port_address_to_region(
            src=source_elf,
            dst=dst,
            source_va=source_va,
            context_before=max(8, context_before),
            context_after=max(8, context_after),
            max_literal_refs=max(1, max_literal_refs),
            allow_risky_heuristics=allow_risky_heuristics,
        )
        results.append(
            PortResult(
                target=target_path.name,
                address=mapped,
                method=method,
                confidence=confidence,
                note=note,
            )
        )

    return results


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Port one source address from one ELF to one or many target ELFs",
    )
    p.add_argument(
        "address",
        type=parse_address,
        help="Source virtual address (e.g. 0x00123456)",
    )
    p.add_argument(
        "--source-elf",
        type=Path,
        required=True,
        help="Path to source ELF",
    )
    p.add_argument(
        "--target-elfs",
        type=Path,
        nargs="+",
        required=True,
        help="One or many target ELF paths",
    )
    p.add_argument(
        "--context-before",
        type=int,
        default=48,
        help="Pattern bytes before the source address (default: 48)",
    )
    p.add_argument(
        "--context-after",
        type=int,
        default=48,
        help="Pattern bytes after the source address (default: 48)",
    )
    p.add_argument(
        "--max-literal-refs",
        type=int,
        default=64,
        help="Max source literal refs checked in xref mode (default: 64)",
    )
    p.add_argument(
        "--strict",
        action="store_true",
        help="Disable nearby/thunk heuristics and prefer unresolved over heuristic guesses",
    )
    return p.parse_args(argv)


def main(argv: Sequence[str]) -> int:
    args = parse_args(argv)

    if not args.source_elf.exists():
        print(f"ERROR: source ELF not found: {args.source_elf}")
        return 2

    missing = [p for p in args.target_elfs if not p.exists()]
    if missing:
        print("ERROR: Missing target ELF file(s):")
        for p in missing:
            print(f"  - {p}")
        return 2

    source_elf = ElfImage(args.source_elf)
    print(f"Source ELF: {args.source_elf}")
    print(f"Source address: 0x{args.address:08X}")

    src_loc = source_elf.find_section_for_va(args.address)
    if src_loc is None:
        print("ERROR: Source address is not inside an allocated section in source ELF")
        return 1

    src_sec, src_off = src_loc
    print(f"Source section: {src_sec.name} +0x{src_off:X}")
    print()

    results = resolve_targets(
        source_elf_path=args.source_elf,
        target_elf_paths=args.target_elfs,
        source_va=args.address,
        context_before=args.context_before,
        context_after=args.context_after,
        max_literal_refs=args.max_literal_refs,
        allow_risky_heuristics=not args.strict,
    )

    target_w = max(10, max(len(r.target) for r in results))
    print(f"{'Target ELF':<{target_w}}  {'Address':<12}  {'Method':<24}  {'Conf':<6}  Note")
    print(f"{'-' * target_w}  {'-' * 12}  {'-' * 24}  {'-' * 6}  {'-' * 40}")
    for r in results:
        print(
            f"{r.target:<{target_w}}  {format_addr(r.address):<12}  {r.method:<24}"
            f"  {r.confidence:<6}  {r.note}"
        )

    unresolved = [r for r in results if r.address is None]
    if unresolved:
        print()
        print("Unresolved targets detected. Try larger windows:")
        print("  --context-before 96 --context-after 96")
        print("Or test a nearby code address and port relative offset manually.")
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
