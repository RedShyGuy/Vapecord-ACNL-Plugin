import json
from pathlib import Path
import re
import sys

# Config
PROJECT_ROOT = Path(__file__).parent
LANG_JSON_DIR = PROJECT_ROOT / "LanguageBinCreator" / "languages"
TEXTID_ENUM_FILE = PROJECT_ROOT.parent / "Includes" / "TextID.hpp"

# Regex: match anything like TextID::SOME_KEY
TEXTID_PATTERN = re.compile(r'TextID::([A-Z0-9_]+)')

# Scan code files for enum keys
keys_used = set()

for file_path in PROJECT_ROOT.rglob("*"):
    if file_path.suffix in {".cpp", ".hpp", ".h", ".c"}:
        try:
            content = file_path.read_text(encoding="utf-8")
        except Exception as e:
            print(f"[WARNING] Could not read {file_path}: {e}")
            exit_code = 1
            continue

        matches = TEXTID_PATTERN.findall(content)
        keys_used.update(matches)

# Ignore NONE
keys_used.discard("NONE")

# Load all JSON entries (master_keys)
json_files = list(LANG_JSON_DIR.glob("*.json"))
json_entries_per_file = {}

for json_file in json_files:
    try:
        data = json.loads(json_file.read_text(encoding="utf-8"))
        entries = set(data.get("entries", {}).keys())
        json_entries_per_file[json_file.name] = entries
    except Exception as e:
        print(f"[WARNING] Could not parse {json_file}: {e}")
        exit_code = 1

# Check each key in every JSON
missing_keys_report = {}

for key in keys_used:
    missing_in = []
    for json_name, entries in json_entries_per_file.items():
        if key not in entries:
            missing_in.append(json_name)
    if missing_in:
        missing_keys_report[key] = missing_in

# Print results
if not missing_keys_report:
    print("All TextID keys exist in every JSON file!")
    exit_code = 0
else:
    print(f"Some TextID keys are missing in JSON files ({len(missing_keys_report)}):")
    for key, missing_files in sorted(missing_keys_report.items()):
        print(f"  - {key} missing in: {', '.join(missing_files)}")
    exit_code = 1

print(f"\nTotal TextID keys found in code: {len(keys_used)}")
print(f"JSON files scanned: {len(json_files)}")
sys.exit(exit_code)
