import struct
from pathlib import Path

BASE_DIR = Path(__file__).parent

INPUT = BASE_DIR / "item.txt"
OUTPUT = BASE_DIR / "item.bin"

with open(INPUT, "r", encoding="utf-8") as fin, open(OUTPUT, "wb") as fout:
    for line_num, line in enumerate(fin, 1):
        line = line.rstrip("\n")  # remove line break 
        if not line:
            continue

        if len(line) < 5 or line[4] != " ":
            print(f"Line {line_num} invalid: {line}")
            continue

        # ID from the first 4 symbols (16bit)
        try:
            id_val = int(line[:4], 16)
        except ValueError:
            print(f"Line {line_num} invalid ID: {line[:4]}")
            continue

        # After id the name starts
        name = line[5:].encode("utf-8")

        # [uint16_t id][uint16_t len][bytes...]
        fout.write(struct.pack("<HH", id_val, len(name)))
        fout.write(name)

    print(f"Finished creating binary at: {OUTPUT}")
