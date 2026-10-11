# Experimental partial source build

This recipe builds the selected published C++ sources into a relocatable EE object.
It does not produce a game executable or a playable release. The unresolved imports
are written to `unresolved-imports.json`; they are not supplied by stubs or original
code. The interpreter and software arithmetic drafts are excluded.

The plan contains 262 authored translation units in snapshot order. Every unit's
language and flags were derived from its retained compile record. These happen to
share one C++17 profile; this is not a guessed replacement profile. All 858 included
source/header files are hash checked, including the authored floating-point models
and conversion primitive. No SDK, host standard library or runtime library is added.

Validate the published sources without running target tools:

```sh
python3 tools/build_partial.py --source-root experimental --plan experimental/build-plan.json --check-plan
```

Build using an explicitly supplied EE installation and a new output directory:

```sh
python3 tools/build_partial.py --source-root experimental --plan experimental/build-plan.json --toolchain /path/to/reviewed/ee --output work/portable-partial-first
```

The first authored-only build succeeded from a clean Git archive of revision
`5eaeb7d46735b5f3df43973c5231913087ca72ea` with this exact driver and plan added.
It compiled all 262 units and performed one relocatable link, producing 386
function bodies, 231,132 body bytes, and 836 unresolved symbols. No original game
input, retained private object, upstream source, alias, or retry was used. The
[sanitized receipt](../experimental/build-receipt.json) records the actual source,
recipe, and output hashes. This proves a working partial source-build path; it
does not establish a repeat-build byte identity, a game executable, or gameplay.

The driver compiles each unit once, retains preprocessed source and assembly,
then performs one relocatable link. A failure stops the driver, leaves the failed outputs and command log in
place, and does not trigger a retry or change any source or flags. Existing output
directories are rejected. Metadata queries are counted separately from source
compiler invocations and successful implicit assembler runs.

The initial toolchain profile requires the exact reviewed macOS arm64 EE binaries
(GCC 15.2.0 and binutils 2.45.1), their compiler subtools, specs and floating-point
macro identity. Paths are supplied at runtime and no contributor-specific absolute
path appears in the public plan. This is path-portable, not a claim that unreviewed
host toolchains produce equivalent output. A different host build requires a
reviewed successor toolchain identity/profile; there is no skip-hash switch. See
`docs/TOOLCHAINS.md` and the public EE toolchain source build record.

## Optional upstream floorf and ceilf

The default build leaves these two implementations absent. To compile all 264
snapshot inputs, supply an existing Newlib 1.10.0 tree:

```sh
python3 tools/build_partial.py --source-root experimental --plan experimental/build-plan.json --toolchain /path/to/reviewed/ee --newlib-root /path/to/newlib-1.10.0 --output work/portable-partial-with-newlib
```

Nothing is fetched or vendored. The plan pins both upstream C files, their retained
header closure, `COPYING.NEWLIB` and the reviewed compiler builtin headers. Upstream
Sun/Cygnus notices and `COPYING.NEWLIB` apply to those files; they are not relicensed
as MIT. Their distinct GNU C17 profile preserves
`_FLT_LARGEST_EXPONENT_IS_NORMAL` and `_FLT_NO_DENORMALS` exactly. The existing ceil
preprocessing record establishes its header closure, with byte-identical headers
in the retained floor tree. The floor preprocessing record was not retained; its
actual fresh dependencies must pass the closure check on the first portable build.
Any unexpected dependency stops the build for review.

## Outputs and boundaries

Each source unit receives its own ordered output directory, command stdout/stderr,
preprocessed source, assembly, object and actual ELF inventory. The driver checks
the emitted function names/extents against the selected snapshot input and checks
the GPR64/FP32/single-float ABI. It writes the linked map, partial object, unresolved
symbol list, all object inventories, a status record and artifact hashes. This
inventory is not an instruction-equivalence audit or gameplay test.

The link uses only `-m elf32lr5900 -r --warn-common`. It imports no absolute game
addresses and applies no symbol aliases. In particular, the historical private
V43 binding of `memset` to `norrath_block_memory_set` is not automatically applied;
`memset` remains an explicit unresolved import in this recipe. The output therefore
has its own identity and is not claimed to reproduce the private V43 object hash or
its import counts. No original executable, assets, BIOS, compiled private object,
held draft or proprietary tool is read by the public driver.
