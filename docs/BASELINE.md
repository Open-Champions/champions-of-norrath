# Norrath executable intake — 2026-10-10

## Observed locally

The completed archive extracted its 8,319,303,680-byte ISO successfully with
7-Zip 26.01. The executable and boot configuration were extracted privately;
no original game data belongs in Git.

| Evidence | Value |
|---|---|
| Boot executable | `SLUS_205.65` |
| Boot `VER` / `VMODE` | `1.02` / `NTSC` |
| Executable size | 9,249,659 bytes |
| Executable SHA-256 | `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0` |
| Disc SHA-256 | `76c661a42d912cecfdf513ff809cd77621a4aaa5f8c63f336ef50ddfc5e6dd0b` |
| ELF | 32-bit, little-endian, MIPS executable |
| Entry point | `0x00100008` |
| ELF flags | `0x20924001` |
| Layout | 6 program headers and 45 section headers |
| Main `.text` | Address `0x00100000`, file offset `0x1000`, size 2,209,032 bytes |
| Symbol metadata | `.symtab` and `.strtab` are present; not yet inventoried or boundary-validated |

The authored intake tool passed on this private executable and its `SYSTEM.CNF`.
Its entry point, ELF flags, all 45 section records and all 6 program-header records
were independently compared with LLVM 20.1.8 `llvm-readobj` output and agreed.
The executable checksum also agreed with the system `shasum` utility.
The 12 synthetic tool tests passed locally on Apple Silicon macOS.
The new platform CI workflow has been authored but has not been run remotely.

## Interpretation and unresolved evidence

The [PCSX2 game database](https://github.com/PCSX2/pcsx2/blob/master/bin/resources/GameIndex.yaml)
maps `SLUS-20565` to Champions of Norrath, NTSC-U. That mapping corroborates
the title and region; it does not authenticate this download or distinguish its
exact retail revision. `SYSTEM.CNF`'s `VER=1.02` is boot metadata, not proof of a
particular pressing, release label or unmodified image. The hash identifies the
specific private input used here.

LLVM decodes the flags as MIPS3, R5900, EABI64 and noreorder. These are executable
header observations, not proof of the original compiler version or a validated
replacement toolchain. Additional executable sections include `.vutext` and
`.srs`; main `.text` size alone is not a whole-game reconstruction denominator.

7-Zip's ISO reader reports a primary volume of 4,199,907,328 bytes with
4,119,396,352 trailing bytes. The full ISO was hashed, but the trailing region's
filesystem and layer layout have **not** been validated. Successful executable
extraction is not proof of complete disc or asset coverage.

## Next bounded work

1. Resolve edition evidence and pin the exact input identity in a reviewed target.
2. Inspect the retained symbol table privately; distinguish function symbols,
   aliases, undefined entries and section coverage before reporting an inventory.
3. Manually validate one complete candidate boundary and its ABI requirements.
4. Adapt the shared verification design through review, then compile and compare
   one authored function against this exact executable.

No C++ functions have been reconstructed in this target. There is no linked PS2
build or playable native Norrath client. Asset, save, script and network behavior
remain independently unverified.
