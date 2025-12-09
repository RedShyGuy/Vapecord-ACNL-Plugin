import os
import re
from pathlib import Path

# CONFIG
SCRIPT_DIR = Path(__file__).parent
ADDRESSES_HEADER = SCRIPT_DIR / "Includes/Address/Addresses.hpp"

# regex for Address(...) and Address name(...)
address_pattern = re.compile(
    r"\bAddress(?:\s+\w+)?\s*\(\s*(0x[0-9A-Fa-f]+)\s*\)"
)

# regex entries for address array
array_pattern = re.compile(
    r"\{\s*(0x[0-9A-Fa-f]+)"
)

def collect_addresses_from_header():
    found = set()
    if not os.path.exists(ADDRESSES_HEADER):
        print(f"ERROR: {ADDRESSES_HEADER} not found!")
        return found

    with open(ADDRESSES_HEADER, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            match = array_pattern.search(line)
            if match:
                found.add(match.group(1).lower())
    return found


def scan_code_files(allowed_set):
    missing = {}
    found_all = set()

    for root, _, files in os.walk("."):
        for file in files:
            if not file.endswith((".cpp", ".hpp", ".h")):
                continue

            path = os.path.join(root, file)
            with open(path, "r", encoding="utf-8", errors="ignore") as f:
                for idx, line in enumerate(f, 1):
                    match = address_pattern.search(line)
                    if match:
                        hex_value = match.group(1).lower()
                        if hex_value not in allowed_set:
                            missing.setdefault(path, []).append((idx, hex_value))
                        found_all.add(hex_value)

    return found_all, missing


def main():
    print("Scanning...")
    allowed = collect_addresses_from_header()
    print(f"Known addresses: {len(allowed)}")

    found, missing = scan_code_files(allowed)

    print(f"Total referenced in code: {len(found)}")

    if missing:
        print("\nMissing addresses:")
        for file, entries in missing.items():
            print(f"\n{file}:")
            for line, val in entries:
                print(f"  Line {line}: {val}")
    else:
        print("\nAll Address-Entries are ported!")

if __name__ == "__main__":
    main()
