import json
from pathlib import Path
import re

# Config
PROJECT_ROOT = Path(__file__).parent
LANG_JSON_DIR = PROJECT_ROOT / "LanguageBinCreator" / "languages"

# Regex: match anything inside get("...") or get('...')
LANG_PATTERN = re.compile(r'Language::getInstance\(\)->get\(\s*["\'](.+?)["\']\s*\)')

# MenuFolderExtras: first parameter
MENUFOLDER_PATTERN = re.compile(r'MenuFolderExtras\(\s*["\'](.+?)["\']')

# MenuEntryExtras: last string parameter

MENUENTRY_PATTERN = re.compile(r'MenuEntryExtras\((.*?)\)')

# HotkeyExtras: second parameter
HOTKEY_PATTERN = re.compile(r'HotkeyExtras\([^,]+,\s*["\'](.+?)["\']\s*\)')

# Scan code files for keys
keys_used = set()

for file_path in PROJECT_ROOT.rglob("*"):
    if file_path.suffix in {".cpp", ".hpp", ".h", ".c"}:
        try:
            content = file_path.read_text(encoding="utf-8")
        except Exception as e:
            print(f"[WARNING] Could not read {file_path}: {e}")
            continue

        matches = MENUFOLDER_PATTERN.findall(content) + \
          HOTKEY_PATTERN.findall(content) + \
          LANG_PATTERN.findall(content)

        keys_used.update(matches)

        params = MENUENTRY_PATTERN.findall(content)
        for param_str in params:
            string_keys = re.findall(r'["\'](.+?)["\']', param_str)
            keys_used.update(string_keys)

# Load all JSON entries
json_files = list(LANG_JSON_DIR.glob("*.json"))
json_entries_per_file = {}

for json_file in json_files:
    try:
        data = json.loads(json_file.read_text(encoding="utf-8"))
        entries = set(data.get("entries", {}).keys())
        json_entries_per_file[json_file.name] = entries
    except Exception as e:
        print(f"[WARNING] Could not parse {json_file}: {e}")

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
    print("All language keys exist in every JSON file!")
else:
    print(f"Some language keys are missing in JSON files ({len(missing_keys_report)}):")
    for key, missing_files in sorted(missing_keys_report.items()):
        print(f"  - {key} missing in: {', '.join(missing_files)}")

print(f"\nTotal keys found in code: {len(keys_used)}")
print(f"JSON files scanned: {len(json_files)}")
