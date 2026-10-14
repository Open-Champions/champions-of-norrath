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
python3 tools/rebuild.py verify --game work/private/SLUS_205.65 --compiler /absolute/path/to/clang --ee-compiler /absolute/path/to/ee/bin/mips64r5900el-ps2-elf-g++ --historical-compiler work/toolchains/build-gcc-2.95.2/gcc/xgcc --historical-assembler work/toolchains/ee/bin/mips64r5900el-ps2-elf-as --output-dir work/rebuild --report work/rebuild/verification.json
python3 tools/rebuild.py check --report targets/SLUS-20565/verification.json --require-matches
```

Select the existing trusted compiler explicitly. `verify` compiles and compares;
`check` validates ledger/source/receipt fingerprints without accessing original
game bytes. `--require-matches` additionally rejects records lacking a match for
each banked function and declared data owner. Public CI runs synthetic tests and this recorded-match
check; it does not contain or freshly compare the private game.

The sanitized receipt records baseline, verifier, source, compiler, object and
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

### Historical integer leaves

The optional `gcc-2.95.2-mips3-leaf` toolchain uses the published
[host build recipe](../toolchains/gcc-2.95.2/README.md) and explicit
`historical-integer-leaf-v1` mode. Its generic MIPS3/o64 object flags stay
`0x20002001`; this mode makes no EE ABI claim. Pass both
`--historical-compiler work/toolchains/build-gcc-2.95.2/gcc/xgcc` and
`--historical-assembler work/toolchains/ee/bin/mips64r5900el-ps2-elf-as`
when the ledger contains such an entry. Other entries still need their selected
compilers. The fixed layout and configured prefix must match this checkout.

The verifier binds the driver, preprocessor, C++ frontend, assembler, built-in
specifications and layout descriptor. It rejects external specifications and
shadow components, uses fresh private working and temporary directories, checks
the integer/pointer model, then captures newly compiled assembly and feeds those
same bytes directly to the assembler. Stored or hand-edited assembly cannot be
used as source. Compiler identity and source snapshots are checked again after
compilation and at the end of the run.

A separate instruction audit permits only stackless integer leaves with forward
conditional branches, including correctly modeled branch-likely delay slots, and
one terminal return. It rejects calls, loops, protected registers, floating-point,
MMI and runtime relocations. Original boundaries and executable load mappings
remain strict. The exact two zero-sized GNU compiler markers are permitted only
as historical object metadata, never as dependencies or reconstructed data.
Pointer lifetimes, aliasing and source-level type contracts still need independent
review. The old compiler has no `-fwrapv` support; undefined signed overflow cannot
be justified by copying the modern compiler's options.

Ledgers containing this mode, without scalar owners, produce schema 4 receipts binding the assembly,
object flags, source, model, instruction policy and component identities. They
cannot supply dependencies to the EE direct-call mode. A clean isolated rebuild
reproduced the recipe and a 52-byte comparison, while its host binaries differed;
this establishes neither a reproducible compiler binary nor a complete game build.

### Source-backed scalar state

The separately reviewed `ee-gprel-scalar-v1` mode permits a four-byte, explicitly
initialized unsigned zero in file-backed `.sdata` and complete read, write, or
set-to-one consumers. It requires the same-run fresh source owner, exact GP
metadata and startup context, one zero-addend `R_MIPS_GPREL16` relocation, and
complete original/relocated body equality. Owner storage, symbol identity, load
mappings, source contracts and component stability are checked separately.

The [global-state contract](GLOBAL_STATE.md) describes its narrow scope and
local-to-external comparison adaptation. Ledger schema 2 records `data_objects`;
receipt schema 5 binds their fresh results to each consumer. Function counts and
code bytes remain separate from data objects and initialized bytes. Runtime
initialization, GP preservation and whole-program linkage remain unverified.

The explicitly versioned `ee-gprel-scalar-v2` mode uses ledger schema 3, receipt
schema 6 and per-owner review schema 2. It allows at most two distinct source
owners and a closed unsigned add-ten body with two relocations to the same owner.
Every owner remains independently compiled and matched; each consumer binds its
selected owner's current result. The existing v1 mode retains its one-owner and
single-access limits. Migration requires new operation-bound reviews and fresh
comparison, not reinterpretation of a saved v1 receipt. See the
[two-owner contract](GLOBAL_STATE.md#two-independent-owners-and-a-bounded-update).

### Separate byte-store policy

The reviewed `ee-gprel-byte-store-v1` mode accepts only a complete eight-byte
return with an A0-to-GP byte store in its delay slot. Its separate source owner
must be one unsigned-char byte, naturally aligned, initialized to file-backed
zero in `.sdata`. The fixed profile uses `-G8` and
`-fno-zero-initialized-in-bss`; existing word and float profiles remain unchanged.
One zero-addend relocation at offset four must bind that same freshly compiled
owner. The [byte-state contract](GLOBAL_STATE.md#source-owned-byte-store) covers
exact storage, source-model, metadata and review checks.

This mode requires ledger schema 5 and receipt schema 8: exactly one byte owner,
exactly one float-store owner, and up to two scalar-v2 owners. Its owner review
uses schema 4; existing scalar and float review versions remain unchanged.
Adding verifier support does not itself admit another source function; source
review and fresh comparison are separate requirements.
Older policy lanes remain supported, but any verifier hash change invalidates
saved receipts. Refresh them through a fresh comparison with their existing
ledger; do not edit their recorded hashes into agreement.

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
The [runtime-library evidence](RUNTIME_LIBRARIES.md) records the compatible public
reentrancy-state initializer and the remaining source-backed data requirements.

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

### Source-backed direct calls

The optional `ee-direct-call-v1` comparison mode supports a narrow straight-line
integer caller with ordinary `R_MIPS_26` JAL relocations. Every target must be
another ledger function freshly compiled and matched in the same run, with the
same reviewed EE compiler options. Dependencies must form an acyclic graph;
an address in the original executable or an older success receipt cannot supply
a missing reconstructed callee. No current banked game function uses this mode.

A caller's `call_bindings` explicitly records each callee's ledger ID, object and
original symbol names, address, complete size and original body hash, together
with sorted call offsets. Original functions must have unambiguous global symbol
boundaries and a unique executable load mapping. Each original call instruction
must independently target the declared callee.

The verifier accepts only zero-addend JAL relocations at exactly those sites,
checks alignment and the jump region selected by PC+4, then compares every byte
of the complete relocated body. It never masks differing bytes. Object symbols,
relocation ownership and allocated sections remain restricted. The only extra
symbol metadata accepted is the observed local section/file form; it cannot
satisfy a dependency.

All graph members pass a closed instruction audit allowing selected ordinary
integer instructions, declared JAL calls, and a terminal return with its delay
slot. Branches, indirect calls, unknown instructions, floating-point and MMI code
remain unsupported. This checks instruction/control-flow shape, not memory safety,
return-stack provenance, or compatible C++ declarations. Those contracts still
require source and caller evidence before a function is banked.

Graph receipts use schema 3 and bind the raw body hash, relocated body hash,
callsite evidence, policy and complete fresh callee result records. Source and
compiler identity must remain stable through the entire graph run. Leaf-only
ledgers without the historical profile continue producing schema 2 receipts. Synthetic tests exercise dependency
failure, stale records and malformed objects; a separate authored two-function
fixture has also been compiled with the reviewed EE compiler and compared with
the GNU linker's result. Neither fixture is game reconstruction progress.

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
reference to the candidate. Leaf objects are compared without rewriting code;
the separate direct-call and scalar modes above support only their declared
code relocations. General runtime-data relocations remain unsupported. Source validation also rejects
directive and extension tokens conservatively, including when they occur in
comments or strings, so unrecorded external source cannot supply a candidate.

Current source progress is **eleven complete functions and 160 matching code bytes**,
plus **four separately verified initialized data objects totaling thirteen bytes**.
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

The sixth complete body, `lineToY(int)` at `0x001d3990`, maps a dialog line to
`18 * line + 295`, with 32-bit wrapping defined by `-fwrapv`. Its complete
20-byte body freshly matches through EE GCC. Seven direct calls in the dialog
paths pass 1 or 6 and use the resulting 313 or 403 for font positioning or
scissoring. Independent review and 65,723 arithmetic checks covered signed limits,
overflow boundaries and deterministic inputs. The surrounding four-byte alignment
gaps are excluded; dialog rendering itself has not been executed.

The seventh complete body, `lightDeactivate(Light*)` at `0x0013eb28`, removes a
light from an intrusive list and clears its links. The corrected source models
its second field as a pointer to a next-pointer slot: the slot can be a
standalone list head or a predecessor's first field. Original `lightAdd` and
`lightActivate` paths establish both forms; a pointer to an entire previous Light
would wrongly require a larger object at standalone heads.

The complete 52-byte body matches through the historical integer-leaf profile.
The original has no aliases or overlaps, and four alignment bytes on each side
are excluded. Review found 29 direct call sites. All 96,000 exhaustive three-node
and standalone-head configurations agreed between original instruction
interpretation and native compiled source with ASan/UBSan, including self-links,
shared neighbors, cycles and surrounding memory guards. The authored view covers
only the eight-byte link prefix; full Light class layout, allocation lifetime,
lighting behavior and actual game execution remain outside this function claim.

The eighth complete body, `lightDisableVolumetricLighting()` at `0x0013e720`,
sets a shared lighting flag to one. Its entire 12-byte body matches after the
reviewed GP-relative relocation binds a separately compiled source definition
to the original local four-byte `.sdata` object at `0x00483aa8`. The source
owner's complete file-backed zero initializer also matches. The original local
linkage is explicitly adapted to one external comparison definition; this is
not a full translation-unit or subsystem reconstruction.

Independent review, GNU linker comparison and 65,542 native source/original-model
cases passed, including initial zero and a manual harness reset, with ASan/UBSan.
The original reset function remains unreconstructed. The
[source review](../targets/SLUS-20565/disable-volumetric-lighting-review.json) binds
owner and consumer hashes to the fixed GP evidence. Two observed drawing callers
have different reset conditions, so unconditional reset and runtime preservation
are not inferred. Other state users and actual lighting behavior remain unverified.

The ninth complete body, `lightDisableLocalLights()` at `0x0013e808`, adds ten
to the four-byte `interfaceMode` state at `0x00483aac`, preserving modulo 2^32
behavior for every input bit pattern. Its full 16-byte body matches under the
v2 scalar profile: both GP-relative fixups bind the same independently compiled
owner. The separate file-backed zero initializer also matches. Four preceding
alignment bytes are excluded; the next function begins immediately afterward.

The [source review](../targets/SLUS-20565/interface-mode-review.json) records the
unsigned storage/update adapter and its limits. Other observed users compare the
state as signed; their push, pop and reset behavior is not redefined. Independent
review and 65,634 native/original-model cases passed with ASan/UBSan. A combined
GNU link matched both scalar functions and both owners, while 65,680 separate-TU
host interaction cases checked that each update preserves the other state. The
existing setter explicitly migrated to v2 with unchanged source and a fresh review.
These checks do not establish original translation-unit linkage, actual caller
sequencing, initialization execution, concurrent access or runtime lighting.

The tenth complete body, `lightGlobalCharLightScale(float)` at `0x0013eac0`,
stores its supplied float in the independently authored mutable state at
`0x00483ab4`. All eight function bytes and the separate four-byte `1.0f`
initializer match. The explicit float-store policy verifies the complete return
and F12 store, one GP-relative relocation, the selected fresh owner, and the
actual compiler float model and ABI records. This brings initialized source
data coverage to three objects totaling twelve bytes.

The [source review](../targets/SLUS-20565/global-char-light-scale-review.json)
separates finite common-representable source values from the target instruction's
raw four-byte store. Independent isolated GNU links and 65,662 host interaction
cases passed with ASan/UBSan: 65,364 finite cases and 298 exceptional patterns
observed on the host only. A separate guarded store model covered 65,545 raw
patterns. None of these tests establishes PS2 execution or general exceptional
floating-point semantics. The observed clear caller supplies `1.0f`, but its
complete compiled body still differs from the original, as does the larger
optional-reset candidate. They receive no match credit. The original local owner
uses an explicit external-linkage comparison adapter; full source integration
and runtime initialization remain unfinished.

The eleventh complete body, `worldSetHalfFrustumMode(int)` at `0x0017dcb8`,
stores the argument's low byte in the independently authored clipping-mode state
at `0x00484134`. All eight function bytes and the separate one-byte zero
initializer match. The byte-store profile checks the full return and delay-slot
store, one GP-relative relocation, and the fresh source owner with natural
one-byte alignment. Coverage is eleven functions totaling 160 code bytes and
four initialized objects totaling thirteen bytes.

The [source review](../targets/SLUS-20565/half-frustum-mode-review.json) describes
the defined int32-to-unsigned-char8 conversion modulo 256. Independent fresh
compilation and linking reproduced the objects and complete function. Native
ASan/UBSan and original/generated instruction checks passed 133,140 cases,
including every signed16 input and signed32 boundaries. Another 2,048 raw64
register cases checked only the machine store's low-byte behavior. This does not
establish the original storage's signedness, return declaration, translation-unit
linkage, storage lifetime or runtime GP preservation. Full camera behavior and
PS2 execution remain unverified.
