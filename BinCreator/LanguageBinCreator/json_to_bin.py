import json
import struct
from pathlib import Path

BASE_DIR = Path(__file__).parent
INPUT_JSON = BASE_DIR / "language.json"
OUTPUT_BIN = BASE_DIR / "language.bin"

with open(INPUT_JSON, "r", encoding="utf-8") as f:
    data = json.load(f)

# Each Language gets written into memory, before creating the language headers
language_blocks = {}
for lang, entries in data.items():
    block = bytearray()
    block += struct.pack("<I", len(entries))
    for key, val in entries.items():
        key_b = key.encode("utf-8")
        val_b = val.encode("utf-8")

        # use 2 bytes (unsigned short) for lengths instead of 1 byte
        block += struct.pack("<H", len(key_b))  # uint16
        block += key_b
        block += struct.pack("<H", len(val_b))  # uint16
        block += val_b

    language_blocks[lang] = bytes(block)

# Building Header
# Structure:
# [uint32 langCount]
# For each language:
# [uint8 nameLen][char[] langName][uint32 offset][uint32 size]

header = bytearray()
header += struct.pack("<I", len(language_blocks))

# Placeholder for header
header_entries = []
offset = 4  # start after [uint32 langCount]

for lang, block in language_blocks.items():
    name_b = lang.encode("utf-8")
    entry = {
        "name_b": name_b,
        "offset_pos": offset + 1 + len(name_b),
        "size": len(block)
    }
    header_entries.append(entry)
    offset += 1 + len(name_b) + 8  # nameLen + name + offset + size

# Data begins after header
data_start = offset

# Create header
final_header = bytearray()
final_header += struct.pack("<I", len(language_blocks))

current_offset = data_start
for lang, block in language_blocks.items():
    name_b = lang.encode("utf-8")
    final_header += struct.pack("<B", len(name_b))
    final_header += name_b
    final_header += struct.pack("<II", current_offset, len(block))
    current_offset += len(block)

# Write to file
with open(OUTPUT_BIN, "wb") as out:
    out.write(final_header)
    for lang in language_blocks:
        out.write(language_blocks[lang])

print(f"Created {OUTPUT_BIN.name} with {len(language_blocks)} languages (16-bit lengths for keys/values)")