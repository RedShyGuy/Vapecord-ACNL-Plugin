import json
import struct
from pathlib import Path

BASE_DIR = Path(__file__).parent
INPUT_JSON = BASE_DIR / "addresses.json"
OUTPUT_BIN = BASE_DIR / "addresses.bin"

with open(INPUT_JSON, "r", encoding="utf-8") as f:
    data = json.load(f)

addresses = data["addresses"]

# Group by region
by_region = {}
for symbol, regions in addresses.items():
    for region, addr in regions.items():
        by_region.setdefault(region, {})[symbol] = int(addr, 16)

# Build data blocks
region_blocks = {}
for region, symbols in by_region.items():
    block = bytearray()
    block += struct.pack("<H", len(symbols))  # entryCount (u16)
    for key, addr_value in symbols.items():
        key_b = key.encode("utf-8")
        block += struct.pack("<B", len(key_b))  # keyLen (u8)
        block += key_b
        block += struct.pack("<I", addr_value)  # address (u32)
    region_blocks[region] = bytes(block)

header = bytearray()
header += struct.pack("<H", len(region_blocks))  # regionCount (u16)

# Calculate header offsets
offset = 2  # after region count
for region in region_blocks:
    offset += 1 + len(region.encode("utf-8")) + 8  # nameLen + name + offset + size
data_start = offset

# Create header
current_offset = data_start
for region, block in region_blocks.items():
    name_b = region.encode("utf-8")
    header += struct.pack("<B", len(name_b))
    header += name_b
    header += struct.pack("<II", current_offset, len(block))
    current_offset += len(block)

# Write to file
with open(OUTPUT_BIN, "wb") as out:
    out.write(header)
    for region in region_blocks:
        out.write(region_blocks[region])

print(f"Created {OUTPUT_BIN.name} with {len(region_blocks)} regions")
