import os
import re
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
ADDRESSES_HEADER = SCRIPT_DIR / "Includes/Address/Addresses.hpp"

address_usage_pattern = re.compile(
    r"\bAddress(?:\s+\w+)?\s*\(\s*(0x[0-9A-Fa-f]+)\s*\)"
)

array_pattern = re.compile(
    r"\{\s*(0x[0-9A-Fa-f]+)\s*,"
)


# --- comment stripping ---

def strip_block_comments(text: str) -> str:
    return re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)


def strip_all_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    text = re.sub(r"//.*", "", text)
    return text


# --- header scan ---

def collect_addresses_from_header():
    addresses = set()

    if not ADDRESSES_HEADER.exists():
        print(f"ERROR: {ADDRESSES_HEADER} not found!")
        return addresses

    with open(ADDRESSES_HEADER, "r", encoding="utf-8", errors="ignore") as f:
        content = strip_block_comments(f.read())

    for match in array_pattern.finditer(content):
        addresses.add(match.group(1).lower())

    return addresses


# --- code scan ---

def collect_used_addresses():
    used = set()

    for root, _, files in os.walk("."):
        for file in files:
            if not file.endswith((".cpp", ".hpp", ".h")):
                continue

            path = Path(root) / file

            if path.resolve() == ADDRESSES_HEADER.resolve():
                continue

            with open(path, "r", encoding="utf-8", errors="ignore") as f:
                content = strip_all_comments(f.read())

            for match in address_usage_pattern.finditer(content):
                used.add(match.group(1).lower())

    return used


# --- main ---

def main():
    print("Scanning for unused addresses...")

    header_addresses = collect_addresses_from_header()
    used_addresses = collect_used_addresses()

    unused = sorted(header_addresses - used_addresses)

    print(f"Total in header: {len(header_addresses)}")
    print(f"Used in code: {len(used_addresses)}")
    print(f"Unused: {len(unused)}")

    if unused:
        print("\nUnused addresses:")
        for addr in unused:
            print(f"  {addr}")
        exit_code = 1
    else:
        print("\nNo unused addresses found!")
        exit_code = 0

    sys.exit(exit_code)


if __name__ == "__main__":
    main()

