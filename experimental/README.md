# Experimental source snapshot

This directory publishes the newer independently authored reconstruction work.
It contains real implementations, including unresolved dependencies and explicitly
held drafts. It does not add entries to the exact-match ledger in `reconstructed/`.
The public verified total remains eleven functions and 160 matching code bytes.

## Selected source and partial integration

The current selection contains 279 inputs: 277 authored source units and two
optional external Newlib inputs. All fifteen additions after the original 264
input prefix are selected in their reviewed integration order. The accepted
private V47 partial object contains 403 function definitions, including compiler
helpers, totaling 241,992 body bytes, with 5,511 relocation fields and 845
unresolved imports. It retains the same eight mutable storage owners, 32 data
bytes and 9,344 BSS bytes. Its first link has not been reproduced. This is a
relocatable partial object, not a game executable or a playable game.

The two external inputs, `floorf` and `ceilf`, reference Newlib 1.10.0 sources
with upstream hashes in [snapshot.json](snapshot.json). They are not bundled,
fetched implicitly, or relicensed. Their Sun/Cygnus notices and `COPYING.NEWLIB`
remain applicable; see [Newlib source releases](https://sourceware.org/pub/newlib/).

The input index maps exact sources and headers to emitted function names.
Components cover actor/player behavior, camera and collision math, allocation
and archive access, scripting support, formatting, networking, sound/kernel
interfaces, entity restoration, and floor lighting. Interfaces and layouts
retain their reviewed scope. Storage ownership, dependency closure, startup,
load layout, SRS/VU code, and game runtime remain unfinished.

The historical `partial_integration` record describes only the original V43
264-input prefix: 388 definitions and 231,604 body bytes, with an identical
repeat link. The [public build receipt](build-receipt.json) describes the earlier
262 authored-input clean-source build and its 386 definitions. Neither record
is evidence of a clean-source build of the expanded 277 authored-input selection.

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
- `game-floor-clear-source` supplies floor-grid reset, free-list construction,
  and per-record program commands. Three bounded original/generated comparisons
  passed using both genuine memory-set providers, including all persistent
  pool/grid/global memory and saved low64 registers. This does not establish
  hardware timing, physical access equivalence, or game runtime.
- `game-floor-add-source-v2` supplies allocation, floor-grid insertion, lighting,
  rotation, count/minimum/maximum updates, and delayed record handling. Its
  corrected source uses the real architectural float-comparison primitive.
- `game-floor-lighting-kernel-source` groups the accepted literal lighting,
  light lookup, world light-map query, and float-comparison sources with their
  canonical headers. Original source versions are recorded in snapshot
  provenance. No superseded implementation is selected.
- `game-integer-trig-code-only-source` supplies the genuine integer-angle cosine
  and sine polynomial routines. All eight coefficient variables remain real
  unresolved imports; no coefficient initializer or storage owner is included.
  The eight new floor/lighting/trig definitions passed first-compilation review
  and V47 integration. Their forty bytes of generated source constants do not
  establish coefficient ownership. Floating-point environment, storage lifetime,
  physical frame/access behavior, and native/game runtime remain held.
- `game-software-arithmetic-source-v2` reconstructs multiplication, division,
  and remainder algorithms with explicit accumulator capture and restoration.
  All 1,596 bounded normal/correction comparisons matched return values and
  four final accumulator values; all 16 bounded zero probes matched their
  selector and incoming banks. The unchanged repeat compilation produced the
  same object. Coverage reaches 472/513 generated words, with gaps recorded.
  Original frame/table access, fault/interrupt/exception/resumption behavior and
  gameplay remain unverified. It is excluded from the selected source build and V47 integration.
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

The [partial source-build recipe](../docs/PORTABLE_BUILD.md) selects all 277
authored units by default and accepts the two optional external sources only
through explicit paths and hash checks. Held arithmetic/interpreter drafts are
excluded. The expanded recipe has not yet been built from a clean checkout;
its selection must not be confused with the historical 262-input success.

Files preserve exact reviewed source bytes, including repeated canonical local
headers. The coherent lighting folder aggregates literal files from their
accepted source versions. See [toolchain documentation](../docs/TOOLCHAINS.md)
for compiler setup. The original [first build receipt](build-receipt.json)
retains its earlier snapshot and plan hashes;
[commit 8bf270c](https://github.com/Open-Champions/champions-of-norrath/commit/8bf270c162f2aef0717ee8a025dbf16688052424)
preserves that exact historical recipe. No new build or runtime result follows
from updating the source selection and plan.

Original executables, disc/BIOS images, assets, disassembly, generated decompiler
output, compiler outputs, private fixtures, and tool binaries are excluded.
