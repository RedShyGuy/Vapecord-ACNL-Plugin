import os
import re
import json
from pathlib import Path

BASE_DIR = Path(__file__).parent

# Suchpfad
SEARCH_DIR = "C:/Users/Standalone/CLionProjects/Vapecord_ACNL_Plugin"
OUTPUT_FILE = BASE_DIR / "output.json"

# Regex für Address Deklarationen
pattern = re.compile(
    r'Address\s+(\w+)\s*\(\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*(0x[0-9A-Fa-f]+)\s*\)'
)

# Region Keys in der richtigen Reihenfolge
regions = ["USA", "USAWA", "EUR", "EURWA", "JPN", "JPNWA", "KOR", "KORWA"]

result = {"addresses": {}}

for root, _, files in os.walk(SEARCH_DIR):
    for file in files:
        if not file.endswith((".cpp", ".h", ".hpp")):
            continue
        path = os.path.join(root, file)
        try:
            with open(path, "r", encoding="utf-8") as f:
                for line in f:
                    match = pattern.search(line)
                    if match:
                        symbol = match.group(1)
                        values = match.groups()[1:]
                        result["addresses"][symbol.upper()] = {
                            regions[i]: values[i] for i in range(len(regions))
                        }
        except Exception as e:
            print(f"Fehler beim Lesen von {path}: {e}")

# JSON speichern
with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    json.dump(result, f, indent=2, ensure_ascii=False)

print(f"✅ Fertig! Gefundene Symbole: {len(result['addresses'])}")
print(f"Gespeichert in: {OUTPUT_FILE}")
