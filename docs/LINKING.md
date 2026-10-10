# Linking reconstructed Norrath source

Linking is useful before the complete game is reconstructed: independently
compiled source files must resolve their references to the right source-owned
functions and data. A matching isolated function alone does not prove this.

`tools/link_smoke.py` now freshly compiles and combines four already admitted EE
state consumers with their four separately authored data definitions in one GNU
linked ELF. It uses the existing verifier's exact compiler profiles, object
checks, complete original extents, fixed GP evidence, and reviewed owner contracts.
It does not modify the reconstruction ledger or replace the reconstruction verifier.

| Existing function | Source-owned state | Code bytes | Data bytes |
| --- | --- | ---: | ---: |
| `lightDisableVolumetricLighting` | disable volumetric lighting | 12 | 4 |
| `lightDisableLocalLights` | interface mode | 16 | 4 |
| `lightGlobalCharLightScale` | global character light scale | 8 | 4 |
| `worldSetHalfFrustumMode` | half frustum mode | 8 | 1 |

The first link places each complete function and data object at its original
comparison address, with GP fixed to `0x004879f0`. GNU ld resolves five named
`R_MIPS_GPREL16` references against the fresh source definitions. All 44 code bytes
and 13 initialized data bytes match the private, hash-pinned baseline. The linked
ELF has no undefined symbols, COMMON storage, or remaining relocations.

A second link moves all four data objects forward by `0x100` while keeping GP
and function addresses fixed. All five references follow those definitions;
the remaining function bytes and all initializers are unchanged. This sensitivity
control checks that the linker is resolving names rather than depending on
hard-coded final addresses. It is intentionally not a retail-byte match.

The sanitized [link receipt](../targets/SLUS-20565/link-smoke.json) records source,
object, tool, script, and ELF hashes plus comparison results. It contains no
original bytes, disassembly, or private input paths. It records a local run, not
an independent certification. Generated binaries and logs remain in ignored `work/`.

## Reproduce privately

The output directory must be new; existing runs are not overwritten. Supply the
same reviewed EE installation for compiler and linker. The explicit `elf32lr5900`
linker emulation is required; the installation's default is a different ABI.

```sh
python3 tools/link_smoke.py \
  --game work/private/SLUS_205.65 \
  --ee-compiler work/toolchains/ee/bin/mips64r5900el-ps2-elf-g++ \
  --linker work/toolchains/ee/bin/mips64r5900el-ps2-elf-ld \
  --output-dir work/linking-dashboard-smoke/new-run
python3 -m unittest discover -s tools -p 'test_link_smoke.py' -v
```

The synthetic tests require neither a compiler nor original game files. They
reject incorrect complete extents, stale references after data movement, wrong
GP, wrong data address/width/initializer, nonwritable storage, undefined imports,
COMMON symbols, retained relocations, and unexpected allocated payload.

## Boundaries and next work

This is a small integration milestone for **existing reconstructed source**. It
adds zero functions or bytes to reconstruction progress. These four consumers do
not call each other; the result proves their data references, not real game
caller closure. Original local globals still use the documented external-symbol
comparison adapters, not recovered original translation-unit linkage.

No startup is supplied or executed. Setting an ELF entry symbol does not make
this a bootable game. Runtime initialization, GP preservation, real callers,
loader behavior, whole-game ABI compatibility, and gameplay remain unverified.
Clang and historical-compiler comparison objects are excluded from this EE group.

The next useful integration expansion is a reviewed complete caller with its
source dependencies, followed by a bounded subsystem with source-owned data and
startup contracts. Unmatched experimental functions and unresolved imports must
not be credited or hidden behind hard-coded original function addresses.
