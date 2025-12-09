import json
import struct
from pathlib import Path

BASE_DIR = Path(__file__).parent
LANG_DIR = BASE_DIR / "languages"
OUTPUT_BIN = BASE_DIR / "language.bin"

# Load language blocks from separate JSON files
language_blocks = {}
lang_names = {}

for file in sorted(LANG_DIR.glob("*.json")):
    lang_code = file.stem  # filename without .json
    with open(file, "r", encoding="utf-8") as f:
        content = json.load(f)

    lang_name = content.get("name", lang_code)
    entries = content.get("entries", {})

    lang_names[lang_code] = lang_name

    block = bytearray()
    block += struct.pack("<I", len(entries))  # entry count

    for key, val in entries.items():
        key_b = key.encode("utf-8")
        val_b = val.encode("utf-8")

        block += struct.pack("<H", len(key_b))
        block += key_b
        block += struct.pack("<H", len(val_b))
        block += val_b

    language_blocks[lang_code] = bytes(block)

# ---- Build Header ----
# [uint32 langCount]
# For each language:
# [uint8 langCodeLen][char[] langCode]
# [uint8 langNameLen][char[] langName]
# [uint32 offset][uint32 size]

header = bytearray()
header += struct.pack("<I", len(language_blocks))

# Calculate offset start (after header)
offset = 4
for lang, block in language_blocks.items():
    name_b = lang.encode("utf-8")
    display_b = lang_names[lang].encode("utf-8")
    offset += 1 + len(name_b) + 1 + len(display_b) + 8

data_start = offset

# Build final header with correct offsets
final_header = bytearray()
final_header += struct.pack("<I", len(language_blocks))

current_offset = data_start
for lang, block in language_blocks.items():
    name_b = lang.encode("utf-8")
    display_b = lang_names[lang].encode("utf-8")

    final_header += struct.pack("<B", len(name_b))
    final_header += name_b
    final_header += struct.pack("<B", len(display_b))
    final_header += display_b
    final_header += struct.pack("<II", current_offset, len(block))

    current_offset += len(block)

# ---- Write the binary output file ----
with open(OUTPUT_BIN, "wb") as out:
    out.write(final_header)
    for lang in language_blocks:
        out.write(language_blocks[lang])

print(f"Created {OUTPUT_BIN.name} with {len(language_blocks)} languages")
