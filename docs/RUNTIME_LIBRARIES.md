# Runtime-library evidence

This document records static dependency evidence for the hash-pinned Norrath
input. It does not identify an original library release or count library code as
reconstructed.

## Error state and reentrancy

The retained `__errno` function loads `_impure_ptr`, a four-byte initialized
pointer at `0x00448480`. It does not reference the separate `errno` symbol in
zero-initialized storage. Its initial target is the 752-byte `impure_data` object
at `0x00448190`. The observed `unlink` helper calls `__errno`, writes error value
5 through the result, and returns -1. Neither function has been banked.

The public [newlib reentrancy documentation](https://sourceware.org/newlib/libc.html#Reentrancy)
describes the shared pointer and per-context state model. A bounded comparison
of official [newlib source releases](https://sourceware.org/pub/newlib/) found:

| Public source | Measured state size with the reviewed EE data model | Result |
| --- | --- | --- |
| 1.8.2 | 748 bytes | Does not fit the original extent |
| 1.9.0 | 752 bytes | Complete initial state image matches in a private source probe |

The 1.9.0 aggregate initializer supplies three internal standard-stream pointers,
a locale string, an initial random-number seed and zero-initialized fields. A
private build from those public headers, independently linked at the observed
symbol locations, reproduces all 752 state bytes and all four bytes of the
separately source-initialized pointer. No original data was inserted into a
source initializer or patched into the output. An independent rerun reproduced
the result; the probe records header trees, authored inputs and build-component
hashes.

This establishes a compatible public layout and initializer, not a unique
historical version. Later releases or vendor changes may preserve them. Public
header reuse is also distinct from independently authored source; any eventual
import must retain the applicable upstream notices and provenance.

## Remaining integration requirements

The state has mutable fields and pointers. Static initial-image equality does
not establish loader execution, pointer lifetime, runtime writers, scheduler
behavior or reconstructed callers. A same-address zero-sized marker and an
unsymbolized locale literal also require explicit ownership rules.

The verifier currently rejects runtime-data relocations. Adding this dependency
requires reviewed source-backed data definitions, absolute high/low address
references, and typed static pointer initializers. Code-body matches, initial
data matches, reconstructed state users and complete runtime behavior must remain
separate claims. The first capability experiment uses an entirely authored scalar
reader/writer fixture; it supplies no game-source credit.
