# C-runtime byte fill

`memset.cpp` is independently authored from the ordinary byte-fill contract.
The complete pinned original at `memset`, 0x003021b0, spans 192 bytes. Inspection
with the reviewed EE decoder establishes: return the original destination,
write the low eight bits of the value across the requested unsigned byte count,
use an aligned wide-store path when eligible, and finish with byte stores.

This implementation deliberately uses byte stores throughout. It preserves the
final-memory/return contract for valid ordinary spans, not access width, timing,
concurrent observation, MMIO behavior, or original instruction bytes. It is an
experimental complete implementation, not a new exact-match ledger admission.
The existing `norrath_block_memory_set` and its users remain unchanged.

The reviewed EE profile emits a complete 40-byte function with no imports and
no recursive `memset` call. Its 32-bit pointer/size ABI matches the reviewed
compiler callers. A host-renamed build passed 196,992 guarded cases under
ASan/UBSan: 32 alignments, lengths 0 through 512, and twelve values including
negative values and signed integer limits. Guards and returned pointers were
checked in every case. Host execution is not EE/game runtime verification.

Reproduce the host check from the repository root (generated files stay private):

```sh
mkdir -p work/memory-check
clang++ -std=c++17 -O2 -ffreestanding -fno-builtin -fsanitize=address,undefined -Dmemset=norrath_test_memset -c experimental/sources/runtime-memory/memset.cpp -o work/memory-check/memset.o
clang++ -std=c++17 -O2 -fsanitize=address,undefined tools/checks/memset_contract.cpp work/memory-check/memset.o -o work/memory-check/check
work/memory-check/check
```

Commit `33bc826`'s alias is defensible for its three frozen compiler consumers:
its provider has the same ordinary byte-fill contract and target calling ABI.
It is not merged because a public source definition under the standard name
avoids a special linker alias and frozen caller-object allowlist. This adds one
complete provider, no stub or original executable bytes. The old commit remains
available on its original branch.
