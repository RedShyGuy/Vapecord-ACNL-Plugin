# Address Porting Usage

## Script

This script ports one source address from one ELF into one or more target ELFs.

## Parameters

Required:

- source address
- `--source-elf`
- `--target-elfs` (one or more files)

Optional:

- `--context-before` (Default: 48)
- `--context-after` (Default: 48)
- `--max-literal-refs` (Default: 64)
- `--strict`

## Examples

Single target:

```powershell
py -3 port_elf_address.py 0x30CB34 \
  --source-elf gameUSA.elf \
  --target-elfs gameEUR.elf
```

Multiple targets:

```powershell
py -3 port_elf_address.py 0x30CB34 \
  --source-elf gameUSA.elf \
  --target-elfs gameEUR.elf gameJPN.elf gameKOR.elf
```

With a larger search window:

```powershell
py -3 port_elf_address.py 0x30CB34 \
  --source-elf gameUSA.elf \
  --target-elfs gameEUR.elf \
  --context-before 96 --context-after 96
```

## Output

- `Address`: mapped VA or `-`
- `Method`: heuristic used
- `Conf`: internal confidence (`high`, `medium`, `low`)

Exit Codes:

- `0`: all targets resolved
- `1`: at least one unresolved target
- `2`: input/file error
