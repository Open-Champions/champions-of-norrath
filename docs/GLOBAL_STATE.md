# Source-backed lighting state

A complete function comparison can depend on writable data as well as other
functions. A matching instruction address alone does not reconstruct that data.
The GP-relative scalar workflow therefore requires a fresh authored definition
and compares its complete initial storage before accepting any consumer.

## Restricted comparison contract

The `ee-gprel-scalar-v1` mode is limited to an explicitly initialized unsigned
32-bit zero value and three complete accessors: read the word, write the argument,
or set it to one. The owner and consumers use the reviewed EE compiler with
`-G4` and `-fno-zero-initialized-in-bss`, replacing the ordinary `-G0` profile.
These options belong to this mode only. Existing leaf, historical and direct-call
profiles retain their own requirements.

The generated owner must be the sole four-byte object in file-backed `.sdata`,
with no imports, helper functions, dynamic initialization or runtime relocations.
The pure-data object naturally carries flags `0x20924000`; its consumers carry
`0x20924001`, adding the instruction no-reordering bit. Each shape is checked
separately, with no flag rewriting or relaxation of the existing function profiles.
Its original counterpart must have an unambiguous complete symbol, the same
storage and initial value, and a unique writable load mapping. File-backed zero
is distinct from uninitialized or zero-fill storage; the verifier must not read
bytes at a NOBITS section's nominal file offset as an initializer.

Each consumer imports the same freshly compiled source owner. Only one ordinary
GP-relative relocation is permitted, with a zero input addend, the exact access
width/registers and a signed displacement that fits without wrapping. The full
relocated body must equal the complete original function; no bytes are masked.
Additional or changed owner source, an absent or failed owner, malformed mappings,
wrong GP evidence or stale review bindings invalidate the comparison.

## Global-pointer evidence

The [fixed-GP context](../targets/SLUS-20565/gp-context.json) pins the retained
`_gp` symbol and original `.reginfo` declaration to `0x004879f0`. A selected
44-byte startup context establishes the same value before syscall 60. These
original metadata and instruction checks supply a static address interpretation;
they do not prove a running loader, kernel, thread or caller preserves GP.
Runtime preservation and initialization remain explicitly unverified.

## Original local ownership

The lighting flag at `0x00483aa8`, retained name
`disableVolumetricLighting`, is a local four-byte object whose initial file bytes
are zero. Observed instructions set it to one, reset it to zero, and branch on a
loaded nonzero value before a light-map query. This supports a numeric flag
comparison adapter, while the original C++ spelling and complete user set remain
unknown.

Two observed callers also limit the state-lifetime inference. The compass path
reaches a light reset after its setter and drawing calls if those calls return.
The world-object path uses different conditions for setting and resetting the
flag, with descriptor flags reloaded between them. It does not establish that
every set is followed by a reset. Neither complete caller directly writes GP,
but incoming state and called functions still prevent a universal preservation
claim. These are static control-flow observations, not a reported runtime bug.

A separately authored external definition can supply an isolated source
comparison for that local original only through an explicit recorded adaptation.
This does not reproduce the original translation-unit linkage or establish a
whole-program symbol layout. The source owner and each consumer must have
reviewed matching unsigned declarations and type-width assertions. The review
record binds their exact source hashes, consumer identities and GP context;
changing source requires a renewed review record.

Code-function counts and data-object counts are separate. Same-run owner and
consumer comparison establishes only their declared dependencies. Other lighting
users, reset paths, object lifetimes, initialization execution and full subsystem
behavior still require reconstruction and runtime evidence. The original
executable and raw analysis stay private.
