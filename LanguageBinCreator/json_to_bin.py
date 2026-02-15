import json
import struct
from pathlib import Path

BASE_DIR = Path(__file__).parent
LANG_DIR = BASE_DIR / "languages"
OUTPUT_BIN = BASE_DIR / "language.bin"
ENUM_FILE = BASE_DIR.parent / "Includes/TextID.hpp"

BINARY_VERSION = 310

# Load languages
languages = {}
lang_names = {}
for file in sorted(LANG_DIR.glob("*.json")):
    lang_code = file.stem
    with open(file, "r", encoding="utf-8") as f:
        data = json.load(f)
    languages[lang_code] = data["entries"]
    lang_names[lang_code] = data.get("name", lang_code)

# Sort keys
master_keys = list(languages[next(iter(languages))].keys())
key_to_id = {key: i for i, key in enumerate(master_keys)}

# Build binary for each language
language_blocks = {}
for lang, entries in languages.items():
    blob = bytearray()
    offsets = []

    for key in master_keys:
        text = entries[key]
        offsets.append(len(blob))
        blob += text.encode("utf-8") + b"\0"

    # Header for each Language-Block: [count u16][offset u16*count][blob]
    block = struct.pack("<H", len(master_keys))
    for off in offsets:
        block += struct.pack("<H", off)  # NUR Offset
    block += blob
    language_blocks[lang] = block

# Build final header: [uint32 version][uint32 langCount][langs...]
final_header = bytearray()
final_header += struct.pack("<I", BINARY_VERSION)
final_header += struct.pack("<I", len(language_blocks))

offset = 8  # 4 bytes version + 4 bytes langCount
for lang, block in language_blocks.items():
    code_b = lang.encode("utf-8")
    name_b = lang_names[lang].encode("utf-8")
    offset += 1 + len(code_b) + 1 + len(name_b) + 8  # header size

data_start = offset

# Write language entries in header
for lang, block in language_blocks.items():
    code_b = lang.encode("utf-8")
    name_b = lang_names[lang].encode("utf-8")
    final_header += struct.pack("<B", len(code_b)) + code_b
    final_header += struct.pack("<B", len(name_b)) + name_b
    final_header += struct.pack("<II", data_start, len(block))
    data_start += len(block)

# Write final binary
with open(OUTPUT_BIN, "wb") as f:
    f.write(final_header)
    for lang in language_blocks:
        f.write(language_blocks[lang])

print(f"Created {OUTPUT_BIN.name} with {len(language_blocks)} languages (version {BINARY_VERSION})")

# Generate enum
with open(ENUM_FILE, "w", encoding="utf-8") as f:
    f.write("#pragma once\n\n")
    f.write("#include <cstdint>\n\n")
    f.write("enum class TextID : uint16_t {\n")
    for i, key in enumerate(master_keys):
        f.write(f"    {key} = {i},\n")
    f.write(f"    NONE = {len(master_keys)},\n")
    f.write("};\n")

print(f"Created {ENUM_FILE.name} with {len(master_keys)} keys")
