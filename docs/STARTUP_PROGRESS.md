# Startup reconstruction checkpoints

## 2026-10-11: dependency measurement audit

The fresh authored-only partial build still has **848 unresolved imports**.
It has **zero COMMON/small-COMMON symbols**, and 5,495 relocation entries.
No startup stage has executed; the output remains ET_REL, not a game executable.

The inventory now counts both REL and RELA, validates their entry strides,
recognizes MIPS undefined symbols, reports tentative COMMON storage separately,
and rejects function symbols without concrete executable extents. Post-link
validation compares the complete multiset of function names and sizes, including
zero-size markers and duplicate local names, instead of only the sum of sizes.
214 synthetic tool tests passed, followed by a fresh 277-input source build.

The external startup report correctly identifies missing authored entry/main,
ordinary constructor dispatch and SRS launch bodies. Its claim that SHN_COMMON
must be added to unresolved imports is incorrect: COMMON declares tentative
storage that a final linker normally allocates. Storage ownership/initialization
still needs review, separately from undefined imports. No COMMON was present in
this actual partial object, so the earlier 848 measurement was not undercounted.

The report's other observations require narrower interpretation:

- `link_inventory.py` deliberately distinguishes declared VMA coverage from
  compatible file/zero-fill coverage. A conflicting mapping remains worth
  reporting even when a different segment supplies the bytes. The `.reginfo`
  zero-fill overlap is explicitly diagnosed; these fields are not a linker script.
- The smoke link's entry is an arbitrary admitted leaf, not game startup. Its
  entry/header checks can be strengthened before reusing that tool for executable
  validation; it is not being reused as a boot verifier here. Its metadata and
  MIPS-reserved-index limitations do not affect the actual partial import count.
- The partial driver's authored dependency scan is lexical, unlike its upstream
  preprocessor scan. This is a source-closure limitation, not evidence of missing
  imports in the compiled object. Compiler-driver counts are not observations
  of separately launched assembler processes.
- `launchSRS` at 0x002d01e0 is a complete 1,588-byte routine. Its heap reservation,
  subsystem shutdown/restoration, argument/state handling and post-return frontend
  setup cannot be replaced by only load/callback/run calls. No partial skeleton
  is admitted as its reconstruction.

Original executable identity was freshly checked against the pinned baseline.
Complete private startup extents were reviewed; raw instructions remain private.
The import triage's name clusters do not establish runtime ordering or ownership.
Floor drawing/init and engine add-task scheduler remain outside this work.

## C-runtime memory package

The default source build now resolves `memset`: **848 → 847** imports, with no
new imports and no aliases. Its complete source provider passed 196,992 guarded
host cases with ASan/UBSan and fresh target compilation without recursive calls.
The [review](../experimental/sources/runtime-memory/REVIEW.md) explains why the
standard source definition supersedes the optional alias in local commit 33bc826.
The [receipt](../experimental/runtime-memory-receipt.json) records the executed
build identities; later JSON ordering changes do not change the selected sources.
No entry, constructor, first-frame or gameplay stage has executed.

## Ordinary constructor package

The complete ordinary guard, reverse constructor walker and persistent destructor
walker are now selected authored source. Their dependency chain links internally,
but **no game startup stage has executed**. See [contracts and host checks](STARTUP_CTORS.md).
The fresh package build has **853 imports**: zero old imports closed and six newly
exposed startup dependencies (`__CTOR_LIST__`, `__DTOR_LIST__`, `__EH_FRAME_BEGIN__`,
`__register_frame_info`, `__deregister_frame_info`, `atexit`). COMMON remains zero.
The increase from 847 reflects new startup coverage, not a regression in closure.
The [receipt](../experimental/startup-ctors-receipt.json) pins this measurement.

## Entry package

A complete authored `_start` is now selected. Three complete original/authored
instruction-model traces and three deliberate-fault rejection checks passed.
The [entry review](STARTUP_ENTRY.md) states the exact CPU/memory/call contracts
and the model's limits. The [receipt](../experimental/startup-entry-receipt.json)
records the fresh partial build: **863 imports**, zero COMMON. This exposes ten
entry dependencies and closes none of the prior 853: kernel services, argument
storage, root callback, linker boundaries, `main`, and `exit`. No original code
or fixed address alias supplies them in the partial object. No entry/kernel
execution or first frame has been demonstrated.

## Kernel entry interface package

The root callback, startup cache wrapper and `_InitSys` dispatcher now resolve
three of the entry imports. The dispatcher exposes nine lower-level services,
so the fresh count is **869 imports** (863 − 3 + 9), with zero COMMON. The
[review](STARTUP_KERNEL.md) and [receipt](../experimental/startup-kernel-receipt.json)
record the checks and the C++ frame/ABI limitation. Source bootstrap and ordinary
constructor dispatch are now present; no kernel/startup stage has actually run.

## Clean export and optional Newlib math

A `git archive` of `9a5b33d` built with the reviewed EE toolchain and no
`--newlib-root` reproduced the kernel receipt: **869 unresolved imports**,
409 function bodies, zero COMMON, and the same partial-object SHA-256
`08414e0077626c1a45e8a39162d4c98275a2be0c1d7efe74573eb6ce96fa595d`. The archive
supplied every selected authored input; no local-only source was required.

The same archive with a Newlib 1.10.0 tree whose ten pinned files matched
`external_files` compiled `floorf` (232 bytes) and `ceilf` (240 bytes). Those
are the only import changes: **869 → 867**, nothing added, zero COMMON, 411
function bodies. The optional inputs stay out of the default build. No startup
stage or kernel service has run. The [receipt](../experimental/startup-newlib-receipt.json)
records both outputs.

## InitSys service package

The nine services named by `_InitSys`, plus `CreateSema` and `DeleteSema`, are
now selected authored source. The two semaphore wrappers match the original
16-byte bodies. The other nine are complete call orchestration with host
contracts; they are not original-byte matches, and no EE service has run.
Defining the nine removes those imports and exposes their callees. The measured
default partial link is **911 unresolved imports** (869 − 9 + 51), zero COMMON,
420 function bodies. `_gp` was already defined, so `InitThread` did not add it.
`main` and `launchSRS` are still unresolved. The
[review](STARTUP_INITSYS.md) separates observed calls from unknown helper and
hardware effects. The [receipt](../experimental/startup-initsys-receipt.json)
lists the import delta.
