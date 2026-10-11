# Experimental source snapshot

This directory publishes the newer independently authored reconstruction work.
It contains real implementations, including unresolved dependencies and explicitly
held drafts. It does not add entries to the exact-match ledger in `reconstructed/`.
The public verified total remains eleven functions and 160 matching code bytes.

## Partial integration

The private V43 relocatable build combined 264 input objects and emitted 388
function definitions, including compiler helpers, totaling 231,604 body bytes.
The unchanged repeat link produced the same object bytes. This is a partial
relocatable object, not a rebuilt executable or a playable game.

The snapshot publishes the exact source and included headers for 262 authored
inputs. The remaining two inputs, `floorf` and `ceilf`, reference Newlib 1.10.0
sources with their upstream hashes in [snapshot.json](snapshot.json). They are
not bundled or relicensed. The historical Newlib files retain their Sun/Cygnus
notices and `COPYING.NEWLIB`; see
[Newlib source releases](https://sourceware.org/pub/newlib/).

The input index preserves link order and maps each object to its source and
emitted function names. Components cover actor/player behavior, camera and
collision math, allocation and archive access, scripting support, formatting,
network operations, and sound/kernel interfaces. Interfaces and layouts retain
their reviewed local scope; the names do not establish complete game systems.

There are 833 pending dependencies in the reviewed prospective binding plan.
The raw object has 834 undefined imports; one specific reviewed `memset` alias
accounts for that difference. General dependency closure, startup, load layout,
SRS/VU code, and runtime behavior remain unfinished. No whole-game completion
percentage follows from the emitted function count.

## Additional implementations and held drafts

- `game-list-safe-iterator-source` now supplies all three complete iterator
  functions needed by entity restoration: construction, advancement, and
  destruction. The implementations retain mutation-sensitive link handling and
  use the existing genuine deletion provider. All three compiled and integrated
  into private V44. Sixteen bounded fixtures passed 51 original/generated paired
  calls; one additional pair closed a specific constructor branch gap without
  repeating earlier cases. Persistent list/marker memory and saved low64
  registers matched. Fixture removals between calls are explicit environment
  changes, not executed game callbacks. Delete flag one, physical access/frame
  behavior, interrupts, and game runtime remain unverified.
- `game-megatile-name-pool-source` supplies the complete name-pool routine used
  six times by megatile restoration. It preserves interior suffix matching,
  mismatch advancement, live counter reads, and genuine string helper calls.
  Its first compilation, output review, and private V44 integration passed.
  The pool owner, capacity, behavior execution, and game runtime remain unfinished.
- `game-megatile-pm-pointer-cap-source` supplies pointer-list sentinel insertion
  and reuse of earlier runs. Its first compilation and private V45 integration
  passed. Twelve focused comparisons matched persistent memory, returned
  pointers, count updates, and saved low64 registers. Coverage reached 97/99
  original words and 256/270 generated words. Sparse alias probes do not establish
  pool capacity or valid gameplay inputs; hardware and game runtime remain open.
- `game-script-set-script-source` publishes the current script pointer and calls
  the genuine AMX initialization and native-registration providers. Its first
  compilation and private V45 integration passed. Interpreter transport, storage
  ownership, physical access/frame behavior, and game runtime remain open.
- `game-object-process-delete-list-source` supplies the deletion-list traversal
  using the genuine safe iterator and adjusted-object callback. Its first
  compilation and private V46 integration passed. The callback scope is only
  the declared two-argument interface; incidental A2/A3, arbitrary native
  signatures, physical access/frame behavior, object lifetime, interrupts,
  faults, and gameplay remain unverified.
- `game-software-arithmetic-source-v2` reconstructs multiplication, division,
  and remainder algorithms with explicit accumulator capture and restoration.
  All 1,596 bounded normal/correction comparisons matched return values and
  four final accumulator values; all 16 bounded zero probes matched their
  selector and incoming banks. The unchanged repeat compilation produced the
  same object. Coverage reaches 472/513 generated words, with gaps recorded.
  Original frame/table access, fault/interrupt/exception/resumption behavior and
  gameplay remain unverified. It is not bound into V43.
- `game-amx-exec-native-transport-source-v4` contains the interpreter, context,
  and independently authored architecture transport draft. All three original
  `#error` holds remain in place. Physical-frame, register, lifetime, nesting,
  interrupt, and exception behavior remain unresolved. Do not remove the holds
  to infer a runnable interpreter. A separate private diagnostic compilation
  of the worker does not validate this transport or its integration.

The current private V46 partial object integrates seven new definitions with
the unchanged V43 inputs: 271 inputs, 395 emitted function bodies, 234,600 body
bytes, 5,384 relocation fields, and 827 unresolved imports. All prior code/storage
is preserved; the pool views remain unowned and unbound. This first link has not been reproduced and is
distinct from the 262-input public build recipe and its 386-body output.

## Checking this publication

From the repository root:

```sh
python3 tools/check_source_snapshot.py
```

This checks published source hashes, include closure, input counts, and retained
draft holds. It does not compile code, rerun private comparisons, verify the
original executable, or execute gameplay. The private object and review hashes
are provenance references, not evidence independently reproduced by this check.

The [partial source-build recipe](../docs/PORTABLE_BUILD.md) has now compiled all
262 authored inputs from a clean checkout and produced a genuine partial object.
It excludes the two external math files and the held drafts, leaves imports
unresolved, and supplies no game executable or runtime result.

Files preserve the exact reviewed source bytes and their component directory
structure, including repeated local interface headers. This is a source delivery
snapshot, not a complete game build recipe. The existing
[toolchain documentation](../docs/TOOLCHAINS.md) describes compiler setup;
per-component build integration remains work in progress.

The current build plan has the same 262 input sources, include closure, profiles,
and tool identities as the executed recipe. Its snapshot digest was updated to
cover the new excluded implementations and arithmetic status metadata. The
[first build receipt](build-receipt.json) retains the actual earlier snapshot and
plan hashes; [commit 8bf270c](https://github.com/Open-Champions/champions-of-norrath/commit/8bf270c162f2aef0717ee8a025dbf16688052424)
preserves that exact recipe. The added implementations are not silently included
in its measured output counts.

Original executables, disc/BIOS images, assets, disassembly, generated decompiler
output, compiler outputs, private fixtures, and tool binaries are excluded.
