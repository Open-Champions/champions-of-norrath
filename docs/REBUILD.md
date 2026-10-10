# Reconstructing the hash-pinned Norrath target

This workflow checks independently authored C++ against the private executable
identified in [baseline.json](../targets/SLUS-20565/baseline.json). Its exact SHA-256
is the build identity. `SLUS_205.65` and boot `VER=1.02` are observed metadata;
the retail pressing is still unknown. A different executable hash is a different
target and must not silently reuse this target's evidence.

The retained ELF symbol table supplies candidate boundaries. It is not recovered
source. A nonzero function symbol establishes a declared extent, which must still
be checked for aliases, overlaps, section membership and behavior. Main `.text`,
VU code and the separate `.srs` executable section have different scopes; do not
combine their counts into a whole-game completion percentage.

The [inventory summary](../targets/SLUS-20565/symbol-summary.json) records 21,415
symbol entries checked against LLVM 20.1.8. Main `.text` has 5,390 nonzero
function-symbol extents with no aliases or overlaps. The separate `.srs` section
has 8,980 extents with 28 connected overlap groups; these must not be blindly
treated as independent complete functions. Two further function symbols are
undefined. VU sections have no function-symbol coverage in this inventory.

## Reproducible commands

Use Python 3.10 or newer. Original inputs and inventory output stay private.

```sh
python3 -m unittest discover -s tools -p 'test_*.py' -v
python3 tools/link_inventory.py work/private/SLUS_205.65 --expected-sha256 9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0 --output work/new-link-layout.json
python3 tools/symbol_inventory.py work/private/SLUS_205.65 --expected-sha256 9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0 --output work/symbols/new-inventory.json
python3 tools/rebuild.py verify --game work/private/SLUS_205.65 --compiler /absolute/path/to/clang --ee-compiler /absolute/path/to/ee/bin/mips64r5900el-ps2-elf-g++ --output-dir work/rebuild --report work/rebuild/verification.json
python3 tools/rebuild.py check --report targets/SLUS-20565/verification.json --require-matches
```

Select the existing trusted compiler explicitly. `verify` compiles and compares;
`check` validates ledger/source/receipt fingerprints without accessing original
game bytes. `--require-matches` additionally rejects records lacking a match for
each banked function. Public CI runs synthetic tests and this recorded-match
check; it does not contain or freshly compare the private game.

The version-2 sanitized receipt records baseline, verifier, source, compiler, object and
complete-function hashes, plus the effective compiler arguments. The verifier
uses a fresh source snapshot and object location, rejects source/compiler changes
during compilation, and rejects unrecorded compiler-option injection. Receipts
are evidence records, not signatures or independent proof of execution.

Each ledger entry selects its toolchain; omitted `toolchain` means Clang.
`ee-gcc` requires the reviewed public-source GCC 15.2.0 build layout and fixed
R5900/EABI flags. Its identity binds the driver, C++ frontend, assembler and
built-in specs, with external specs rejected and identities checked around every
compile. Macro and object-header checks reject the wrong target data model.
The [toolchain build record](TOOLCHAINS.md) documents source pins and local build
configuration. A mixed ledger requires both explicit compiler paths.

## Bounded reconstruction loop

1. Check the repository status and active coordination issue before taking a
   candidate. Record its symbol, address and file ownership in an ignored local
   claim. Avoid another contributor's assigned area.
2. Verify the private executable hash before analysis. Inspect the candidate's
   complete declared extent, neighboring symbols, input/output behavior and ABI.
3. Write readable, self-contained C++ in `reconstructed/`. Do not substitute raw
   assembly, generated decompilation, byte arrays or a shortened function body.
4. Add the supported candidate to the target ledger. Compile it afresh with an
   explicitly selected compiler and compare its whole extent to the exact input.
5. Review the source and sanitized receipt. Commit one verified function at a
   time, then release the claim. Preserve mismatches as private evidence rather
   than editing a receipt into a pass.

Only a successful fresh comparison establishes a byte match. Public checks of a
saved receipt establish consistency with current records, not that a fresh
compiler run occurred. The original executable, symbol dumps, disassembly, object
files and private research stay under ignored `work/` or outside the repository.

## Whole-image layout

The [load-layout inventory](LINK_LAYOUT.md) preserves the observed file and memory
ranges, overlap diagnostics and nonallocated overlays. It accounts for layout
requirements; it supplies neither reconstructed code nor a linker script.

## Current compiler scope

The original executable's header records R5900/MIPS3/EABI64 flags. The available
Clang compiler is a candidate tool for individual supported leaf functions, not
the established original PS2 toolchain. Matching one simple function does not
establish general calling-convention, delay-slot scheduling, VU instruction,
relocation, linker or runtime equivalence.

The first verifier deliberately supports narrow, self-contained functions. A
function needing unsupported relocations, external data or additional generated
code must be parked or supported through a separately reviewed verifier change.
Do not strip these dependencies to make a comparison appear to pass.

A function match does not provide a linked PS2 executable or playable native game.
The shared Godot client remains a separate project with separate runtime tests.

## First recorded reconstruction

On October 10, 2026, the complete eight-byte function at `0x002a9e50`, retained
symbol `getLevelRequirement__21SkillLightningWeaponsi`, was freshly reconstructed
with Clang 20.1.8. It returns 12 without reading its inputs, accessing memory,
changing the stack or calling another function. The preceding symbol ends at its
start and the next symbol starts at its end; no aliases or overlaps were found.
A retained vtable entry points to it, but dispatch has not been executed.

The [authored source](../reconstructed/skill_lightning_weapons_level_requirement.cpp)
uses an opaque receiver and a 32-bit integer comparison adapter. The old member
name does not encode a return type, and this leaf does not exercise general
parameter ABI behavior. The [receipt](../targets/SLUS-20565/verification.json)
records equality across the entire original symbol extent, not a chosen prefix.

The compiler emits an `R_MIPS_32` relocation in nonallocated `.pdr` procedure
metadata. The verifier permits only that exact metadata shape, offset and
reference to the candidate; code and runtime-data relocations remain unsupported.
The object is compared without rewriting its code. Source validation also rejects
directive and extension tokens conservatively, including when they occur in
comments or strings, so unrecorded external source cannot supply a candidate.

Current source progress is **five complete functions and forty-four matching bytes**.
The second complete body, `SkillLightningWeapons::getSkillPointCost(int)` at
`0x002a9e48`, returns 1 independently of its inputs and adds eight freshly matched
bytes. Its complete symbol has no aliases or overlaps; the preceding function is
separated by four bytes of alignment padding and the next begins at its end.
A third complete body, `SkillResistPoison::getLevelRequirement(int)` at
`0x00313d18`, adds an eight-byte nonconstant shift. Its unsigned 32-bit comparison
adapter models wrapping for all input bit patterns; nine host boundary cases
passed an undefined-behavior check. Actual R5900 instructions match, while original
return type, caller ranges and whole-program ABI remain unverified.

These results do not reconstruct the classes, vtables, skill systems or gameplay. Next work
should take another independently supported function and preserve the same
fresh-comparison and one-function-per-commit discipline.

The fourth complete body, `Camera::GetYaw()` at `0x0026fc40`, is an eight-byte
signed halfword accessor at receiver offset `0x68`. The reviewed EE GCC lane
freshly matches its entire extent. An independent host check covered all 65,536
signed-short values without modifying the view. `CameraYawView` is an authored
comparison structure, not proof of the original class type or lifetime.

The fifth complete body, `SkillDarkness::getSkillPointCost(int)` at `0x002982f0`,
returns its second argument multiplied by three with 32-bit wrapping. Its entire
12-byte extent freshly matches through EE GCC. The adapter requires the recorded
`-fwrapv` option; 10,015 independent host cases covered signed limits and overflow
thresholds. Four alignment bytes on each side are excluded. Its retained vtable
pointer has been observed, but dispatch and gameplay remain unexecuted.
