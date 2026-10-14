# Source-built entry bootstrap

`experimental/sources/runtime-entry/start.S` is independently authored startup
assembly, using register-list macros and explicit memory/kernel contracts. It
is not an assembly dump, embedded original bytes, or a bootable game. An ordinary
C++ function cannot enter with SP zero: kernel syscall 60 supplies the stack.

The pinned original `_start` is 576 bytes at 0x00100008. The new complete body is
480 bytes. Both reset the ordinary 128-bit GPRs (preserving k0/k1), both HI/LO
banks, SA, the 32 floating registers, accumulator and control/status; clear the
same BSS span with aligned 16-byte stores and byte tails; establish GP; request
kernel stack/heap setup; initialize system services and flush the cache; enable
interrupts; call `main(argc, argv)`; and pass its return value to `exit`.

The original zero-valued optional `_init`/`_fini` paths are omitted specifically
for this pinned target. No other edition's optional initialization is inferred.
The assembly uses symbolic storage/boundary/service references, never original
code to satisfy the actual partial link. `_root` and `_args` are deliberately
unresolved, as are system services and game `main`/`exit`.

## Executed verification and limits

`tools/check_entry.py` freshly reads the pinned original and complete linked
authored extent, and rejects unsupported instructions. It models branch delay
slots, register reset, memory-store order/width, kernel argument registers and
subsequent call order. It checks every clear store: 287,793 quadwords and twelve
bytes, exactly `[0x00486800, 0x008eab1c)`. SRS constructor/destructor tables and
COMMON remain outside this clear. `_end` remains 0x008ec91c for the heap request.

Three full original/authored traces agree, with different supplied kernel stack
results, argument counts and main return values. Incoming registers are poisoned;
intercepted external calls also clobber caller-saved registers. Negative tests
remove one full-width register reset, skip a quadword store, and remove interrupt
enable; each is rejected for its intended contract violation.

This is a bounded instruction model, not actual R5900, loader, kernel, cache,
interrupt or game execution. External calls/syscalls are intercepted test inputs;
their implementations/effects remain unverified. Both compared traces share the
model, so this does not eliminate model errors. No new exact-byte match is claimed.
The exact-match ledger and prior leaf receipts are unchanged.

## Reproduction

The portable plan includes a separate `authored_entry_asm` compiler-driver profile
using the same reviewed EE compiler and assembler. All source identities and the
emitted ABI are checked by the ordinary partial build. Generate that build first,
then use its `283/start.o` for this private model fixture (the input order is pinned
in the current snapshot):

```sh
/path/to/reviewed/ee/bin/mips64r5900el-ps2-elf-ld -m elf32lr5900 -T tools/checks/entry_model.ld -o work/entry-model.elf work/entry-partial/283/start.o
python3 tools/check_entry.py --original work/private/SLUS_205.65 --candidate work/entry-model.elf --output work/entry-model-check.json
```

The fixture script's absolute addresses identify intercepted model calls and
storage, not source implementations. Never use it as a game linker script or
count its resolved symbols as game dependencies closed. The public partial link
continues to leave those symbols unresolved and reports them explicitly.
