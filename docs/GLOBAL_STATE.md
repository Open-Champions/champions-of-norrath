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

## Two independent owners and a bounded update

`ee-gprel-scalar-v2` explicitly opts a ledger into one or two separate source
owners. It retains the v1 read, write and set-to-one shapes and adds only one
read-modify-write operation: add ten to an unsigned 32-bit word modulo 2^32.
The complete 16-byte body must load and store the same owner, with two ordinary
zero-addend GP-relative relocations at offsets zero and twelve. The addition,
registers, return and delay-slot store are fixed by the reviewed instruction
contract. Arbitrary arithmetic, cross-owner copies and combined call/data
functions remain outside this mode.

Each owner has a distinct source file, emitted name, original identity, address
range and review record. Each consumer selects one owner explicitly. Adjacent
owners are permitted; aliases, overlaps, interchangeable zero initializers and
proof borrowed from the other owner are rejected. Every owner must have at least
one consumer, and both owners compile afresh before their consumers can match.
Source, review, GP or compiler drift invalidates the entire scalar group.

V2 uses ledger schema 3, receipt schema 6 and per-owner review schema 2. Reviews
bind the exact operation map in addition to each owner's own consumers and source
hashes. Receipts contain an ordered relocation list and the selected owner's
result digest. V1 remains a separate one-owner policy with ledger schema 2 and
receipt schema 5; it cannot claim the new operation or a second owner. A scalar
collection cannot mix the two policy versions. Migration requires explicit
reviews and a fresh comparison, even when an existing source file is unchanged.

The `interfaceMode` state at `0x00483aac` illustrates why the update contract is
narrow. Its observed users apply signed comparisons: push changes the state only
when the old signed value is nonpositive, pop only when it is positive, and reset
subtracts ten once only when it is at least ten. An unsigned storage/update adapter
can preserve the complete add-ten instruction effect for every bit pattern; it
does not change those other users into unsigned operations or prove the original
C++ declaration. The operation requires exclusive access to live, aligned, writable
storage and makes no atomicity or synchronization claim. The void comparison
adapter does not infer an original return type from an incidental result register.

## Source-owned floating-point store

`ee-gprel-float-store-v1` is a separate policy for one complete eight-byte
function: return through RA and store F12 to a source-owned float in the return
delay slot. It admits no floating-point arithmetic, conversions, calls or other
accesses. One zero-addend GP-relative relocation at offset four must identify the
same owner as the original instruction. The owner is a separate mutable,
four-byte `.sdata` definition initialized to `1.0f`, with exact file bytes
`0000803f`. Integer policies still require their original zero initializer.

Ledger schema 4 requires exactly one float owner and permits up to two unchanged
scalar-v2 integer owners. Every owner explicitly names its policy. Identities,
canonical source and review paths, and address ranges remain distinct across
both types. Cross-type bindings and interchangeable owner proofs are rejected.
The float review uses schema 3; integer reviews remain schema 2. Receipt schema 7
records both policies when needed, the selected owner's fresh result, and the
complete source, relocation, compiler-model and ABI evidence. Older ledgers and
receipts retain their existing rules.

The generated float owner and consumer must both contain the reviewed natural
R5900 single-precision ABI metadata: 64-bit general registers, 32-bit FPU
registers, and the exact ABI and GNU attribute records. The compiler's float
predefines are checked independently of its integer model. In particular, its
maximum exponent is 129 and it declares no infinity or quiet-NaN support;
ordinary desktop IEEE assumptions must not replace this target evidence.

Source-level value checks cover finite values representable on both the host
and target. The matched target store separately establishes the instruction's
four-byte F12 bit transfer, without arithmetic. Host observations of exponent-255
patterns do not establish portable C++ exceptional-value behavior or PS2 hardware
semantics. Original translation-unit ownership, general calling conventions,
runtime GP preservation and initialization execution remain unverified.
