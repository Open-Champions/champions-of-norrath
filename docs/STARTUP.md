# Observed Norrath startup requirements

These observations are tied to the exact [baseline hash](../targets/SLUS-20565/baseline.json).
They come from complete symbol extents inspected with EE-aware binutils 2.45.1,
plus bounded interpretation of address arithmetic and zero stores. No startup
source has been reconstructed, and no kernel calls or game runtime were executed.
Raw instruction output and detailed evidence remain private.

## Entry, memory and global pointer

`_start` occupies 576 bytes at `0x00100008`. Its explicit zero loop covers the
half-open range `[0x00486800, 0x008eab1c)`: all `.sbss`, its 12-byte alignment gap,
and `.bss`. The loop performs 287,793 quadword stores and 12 byte stores, totaling
4,604,700 contiguous bytes. Independent replay of the fixed address arithmetic
confirmed the range without executing syscalls.

This ends at `_bssend`, **not** `_end=0x008ec91c`. It excludes `.srsctors`,
`.srsdtors`, and the 7,588-byte `COMMON` section at `[0x008eab78, 0x008ec91c)`.
The inspected Creature type-info initializer reads a guard in COMMON before
deciding whether to initialize it. Its initial state therefore matters, but
this analysis does not establish who clears it. Absence from this loop is not
proof that runtime memory is uninitialized.

The entry explicitly establishes `GP=0x004879f0`, matching `_gp`, before syscall
60 at `0x001001c8`. Its arguments include stack address `0x01fc0000`, stack size
`0x40000`, `_args=0x005192c0` and `_root=0x00100250`. SP receives the syscall result;
the actual resulting value is unknown. The following syscall 61 receives `_end`
and `0xffffffff`. It is unsafe to replace these observations with inferred kernel
effects or an incoming `.reginfo`-derived GP.

The direct startup path calls `_InitSys`, then `FlushCache(0)`, enables interrupts,
and calls `main` at `0x002d0818` with arguments from `_args`. If `main` returns,
control transfers to `exit`. Optional `_init`/`_fini` branches are skipped because
their values are zero in this pinned image.

## Distinct initialization tables

| Table | Address | Callback slots | Observed dispatcher |
|---|---|---:|---|
| `.ctors` | `0x0031f250` | 101 | Reverse order through `__do_global_ctors`, guarded by `__main` |
| `.dtors` | `0x0031f3ec` | 11 | Forward order through `__do_global_dtors`, registered with `atexit` |
| `.srsctors` | `0x008eab1c` | 15 | Forward order in `launchSRS`, separate from ordinary constructors |
| `.srsdtors` | `0x008eab58` | 7 | Dispatcher not yet established |

Ordinary tables have leading `0xffffffff` and trailing zero markers; the separate
SRS tables do not. Every callback slot was checked against an exact retained
function entry in the corresponding `.text` or `.srs` section. This establishes
table structure, not the callback bodies' behavior.

`main` first calls `__main`; its one-time guard lies in explicitly cleared `.bss`.
The ordinary constructor dispatcher also registers frame information. Separately,
`launchSRS` calls the retained `cdLoad` function with destination `0x01000000`
before traversing `.srsctors`, then later calls the retained `srs_main` entry.
The load's source, detailed behavior, launch frequency and destructor path remain
unresolved. The `.srs` region must not be assumed immutable from entry onward.

## Remaining requirements

Establish actual loader/kernel initialization of COMMON, the SRS load and
destructor paths, all constructor effects and DVP overlay use. Preserve distinct
`_bssend` and `_end` boundaries and both constructor families in any future linker
design. Generic LLVM disassembly misdecoded some R5900 instructions in this entry;
the pinned EE-aware decoder was necessary. These findings provide build
requirements, with no new function-match or runtime credit.
