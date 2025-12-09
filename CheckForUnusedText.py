import json
from pathlib import Path
import re

# Config
PROJECT_ROOT = Path(__file__).parent
LANG_JSON_DIR = PROJECT_ROOT / "LanguageBinCreator" / "languages"

# Regex: match anything inside get("...") or get('...')
LANG_PATTERN = re.compile(r'Language::getInstance\(\)->get\(\s*["\'](.+?)["\']\s*\)')

# MenuFolderExtras
MENUFOLDER_PATTERN = re.compile(r'MenuFolderExtras\(\s*["\'](.+?)["\']')

# MenuEntryExtras
MENUENTRY_PATTERN = re.compile(r'MenuEntryExtras\((.*?)\)')

# HotkeyExtras
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

# Check for unused keys in each JSON
unused_keys_report = {}

for json_name, entries in json_entries_per_file.items():
    unused_in_file = [key for key in entries if key not in keys_used]
    if unused_in_file:
        unused_keys_report[json_name] = unused_in_file

# Print results
if not unused_keys_report:
    print("All language keys in JSON files are used in the code!")
else:
    print(f"Unused language keys found ({sum(len(v) for v in unused_keys_report.values())}):")
    for json_name, keys in sorted(unused_keys_report.items()):
        print(f"\n{json_name} ({len(keys)} unused keys):")
        for key in sorted(keys):
            print(f"  - {key}")

print(f"\nTotal keys used in code: {len(keys_used)}")
print(f"JSON files scanned: {len(json_files)}")
