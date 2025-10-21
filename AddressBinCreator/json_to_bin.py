import json
import struct

INPUT_JSON = "addresses.json"
OUTPUT_BIN = "addresses.bin"

with open(INPUT_JSON, "r", encoding="utf-8") as f:
    data = json.load(f)

addresses = data["addresses"]

by_region = {}
for symbol, regions in addresses.items():
    for region, addr in regions.items():
        by_region.setdefault(region, {})[symbol] = int(addr, 16)

region_names = sorted(by_region.keys())

region_headers = []  # (region_name_bytes, region_data_offset)
region_data_blocks = []
current_offset = 0

for region in region_names:
    entries = by_region[region]
    region_name_bytes = region.encode("utf-8")

    # Build datablocks for regions
    block = struct.pack("<H", len(entries))
    for symbol, addr_value in entries.items():
        symbol_bytes = symbol.encode("utf-8")
        block += struct.pack("<B", len(symbol_bytes))
        block += symbol_bytes
        block += struct.pack("<I", addr_value)

    region_data_blocks.append(block)
    region_headers.append((region_name_bytes, current_offset))
    current_offset += len(block)

with open(OUTPUT_BIN, "wb") as f:
    # Count of regions
    f.write(struct.pack("<H", len(region_names)))

    # Placeholder for RegionTableOffset
    region_table_offset = f.tell()
    f.write(struct.pack("<I", 0))

    region_table = b""
    data_offset = f.tell() + sum(2 + 1 + len(name) + 4 for name, _ in region_headers)

    # RegionTable: [Len][Name][DataOffset]
    running_offset = data_offset
    for name_bytes, rel_off in region_headers:
        region_table += struct.pack("<B", len(name_bytes))
        region_table += name_bytes
        region_table += struct.pack("<I", running_offset)
        running_offset += len(region_data_blocks[region_names.index(name_bytes.decode("utf-8"))])

    # Write RegionTable-Offset
    end_header = f.tell()
    f.seek(region_table_offset)
    f.write(struct.pack("<I", end_header))
    f.seek(end_header)

    f.write(region_table)

    # Write all datablocks next to next
    for block in region_data_blocks:
        f.write(block)

print(f"Binary successfully written: {OUTPUT_BIN}")
print(f"Regions: {len(region_names)}")
