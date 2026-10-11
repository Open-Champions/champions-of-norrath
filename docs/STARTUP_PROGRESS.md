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
