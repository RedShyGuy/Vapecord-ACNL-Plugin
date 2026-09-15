#!/usr/bin/env python3
"""
ACNL convenience wrapper around the generic ELF porter.

This script keeps the region-based UX (USA_1_5, EUR_WA, ...) and internally
resolves those regions to ACNL ELF paths, then runs the generic resolver.
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import List, Sequence

from port_elf_address import resolve_targets


REGION_TO_FILE = {
    "USA_1_5": "ACNL_USA_1_5.elf",
    "USA_WA": "ACNL_USA_WA.elf",
    "EUR_1_5": "ACNL_EUR_1_5.elf",
    "EUR_WA": "ACNL_EUR_WA.elf",
    "JPN_1_5": "ACNL_JPN_1_5.elf",
    "JPN_WA": "ACNL_JPN_WA.elf",
    "KOR_1_5": "ACNL_KOR_1_5.elf",
    "KOR_WA": "ACNL_KOR_WA.elf",
}

REGION_ALIASES = {
    "USA1.5": "USA_1_5",
    "USA_1.5": "USA_1_5",
    "USA-1.5": "USA_1_5",
    "USA15": "USA_1_5",
    "USAWA": "USA_WA",
    "EUR1.5": "EUR_1_5",
    "EUR_1.5": "EUR_1_5",
    "EUR-1.5": "EUR_1_5",
    "EUR15": "EUR_1_5",
    "EURWA": "EUR_WA",
    "JPN1.5": "JPN_1_5",
    "JPN_1.5": "JPN_1_5",
    "JPN-1.5": "JPN_1_5",
    "JPN15": "JPN_1_5",
    "JPNWA": "JPN_WA",
    "KOR1.5": "KOR_1_5",
    "KOR_1.5": "KOR_1_5",
    "KOR-1.5": "KOR_1_5",
    "KOR15": "KOR_1_5",
    "KORWA": "KOR_WA",
}


def parse_address(text: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"Invalid address: {text}") from exc


def normalize_region(region: str) -> str:
    key = region.strip().upper().replace(" ", "").replace("-", "_")
    if key in REGION_TO_FILE:
        return key

    compact = key.replace("_", "")
    if compact in REGION_ALIASES:
        return REGION_ALIASES[compact]
    if key in REGION_ALIASES:
        return REGION_ALIASES[key]

    raise ValueError(f"Unknown region '{region}'. Valid: {', '.join(sorted(REGION_TO_FILE))}")


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    default_root = Path(__file__).resolve().parents[2]
    default_elf_dir = default_root / "ELF_FILES_ACNL"

    p = argparse.ArgumentParser(
        description="Port ACNL addresses via generic ELF porter",
    )
    p.add_argument("address", type=parse_address, help="Source virtual address")
    p.add_argument("--source-region", default="USA_1_5", help="Source region (default: USA_1_5)")
    p.add_argument("--target-regions", nargs="*", default=[], help="Target region list (default: all others)")
    p.add_argument("--elf-dir", type=Path, default=default_elf_dir, help=f"ACNL ELF directory (default: {default_elf_dir})")
    p.add_argument("--context-before", type=int, default=48, help="Pattern bytes before source address")
    p.add_argument("--context-after", type=int, default=48, help="Pattern bytes after source address")
    p.add_argument("--max-literal-refs", type=int, default=64, help="Max source literal refs for xref mode")
    p.add_argument("--strict", action="store_true", help="Disable risky text heuristics")
    return p.parse_args(argv)


def build_elf_path(elf_dir: Path, region: str) -> Path:
    path = elf_dir / REGION_TO_FILE[region]
    if not path.exists():
        raise FileNotFoundError(f"Missing ELF for {region}: {path}")
    return path


def main(argv: Sequence[str]) -> int:
    args = parse_args(argv)

    try:
        src_region = normalize_region(args.source_region)
    except ValueError as exc:
        print(f"ERROR: {exc}")
        return 2

    if args.target_regions:
        target_regions: List[str] = []
        for raw in args.target_regions:
            try:
                reg = normalize_region(raw)
            except ValueError as exc:
                print(f"ERROR: {exc}")
                return 2
            if reg == src_region:
                continue
            if reg not in target_regions:
                target_regions.append(reg)
    else:
        target_regions = [r for r in REGION_TO_FILE if r != src_region]

    try:
        source_elf = build_elf_path(args.elf_dir, src_region)
        target_elfs = [build_elf_path(args.elf_dir, r) for r in target_regions]
    except FileNotFoundError as exc:
        print(f"ERROR: {exc}")
        return 2

    print(f"Source region: {src_region} ({source_elf.name})")
    print(f"Source address: 0x{args.address:08X}")
    print()

    results = resolve_targets(
        source_elf_path=source_elf,
        target_elf_paths=target_elfs,
        source_va=args.address,
        context_before=args.context_before,
        context_after=args.context_after,
        max_literal_refs=args.max_literal_refs,
        allow_risky_heuristics=not args.strict,
    )

    region_w = max(10, max(len(r) for r in target_regions))
    print(f"{'Region':<{region_w}}  {'Address':<12}  {'Method':<24}  {'Conf':<6}  Note")
    print(f"{'-' * region_w}  {'-' * 12}  {'-' * 24}  {'-' * 6}  {'-' * 40}")
    for region, result in zip(target_regions, results):
        show = "-" if result.address is None else f"0x{result.address:08X}"
        print(
            f"{region:<{region_w}}  {show:<12}  {result.method:<24}"
            f"  {result.confidence:<6}  {result.note}"
        )

    unresolved = [row for row in results if row.address is None]
    if unresolved:
        print()
        print("Unresolved targets detected. Try larger windows:")
        print("  --context-before 96 --context-after 96")
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
