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

## Drafts outside the partial integration

- `game-list-safe-iterator-source` now supplies all three complete iterator
  functions needed by entity restoration: construction, advancement, and
  destruction. The implementations retain mutation-sensitive link handling and
  use the existing genuine deletion provider. All three compiled successfully;
  behavior execution and integration remain pending.
- `game-megatile-name-pool-source` supplies the complete name-pool routine used
  six times by megatile restoration. It preserves interior suffix matching,
  mismatch advancement, live counter reads, and genuine string helper calls.
  Its first compilation and focused output review passed. The pool owner,
  capacity, behavior execution, and integration remain unfinished.
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
