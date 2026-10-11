# InitSys service requests

These experimental sources are the nine services called by `_InitSys`, plus the
two semaphore syscall wrappers those services request. Original addresses and
full extents were checked against the pinned baseline. No service is executed
on the EE, and none is admitted to the exact-match ledger unless a fresh
body comparison says so.

Local helpers that share one spelling in the original (`setup`, `Copy`,
`GetEntryAddress`, and `kCopy`) are separate functions. The imports below use
distinct names so those copies are not merged. Their original local names and
addresses are stated with each caller. What those helpers do to kernel memory
is unknown.

## Semaphore service wrappers

`CreateSema` at 0x002d50e0 and `DeleteSema` at 0x002d50f0 are 16-byte service
requests, numbers 64 and 65. Fresh compilation reproduces both bodies. They do
not define semaphore objects or deletion effects. `SignalSema` (service 66) is
not on this path.

`DeleteSema` uses the same shape for service 65. Both wrappers were compared as complete `.text` bodies against the pinned executable.

## CRT0 supplement

`supplement_crt0` at 0x002dd6b0 stores `CreateSema` results in `__sce_sema_id` and `__sce_eh_sema_id`. Each request has word 1 and word 2 set to 1. The other parameter words are not written; how many words service 64 reads is unknown. The host contract checks the two stored identifiers and those two words. The 72-byte body is not an original-byte match.

## Reboot notification

`sceSifInitRebootNotify` at 0x002df5f8 stores zero to `g_pUserRebootBuf` and `g_iUserRebootSize`, then calls `memset(g_SysRebootBuf, 0, 512)`. The 512-byte length is the symbol span. Whether a reboot consumer observes that buffer is unknown. The host contract checks both stores and the fill arguments.

## Timer system time

`StartTimerSystemTime` at 0x002de180 calls `DIntr`, then loads the word at 0x10001010. If bit 0x80 is set it returns 1, calling `EIntr` only when `DIntr` returned nonzero. Otherwise it calls `SetT2_MODE` with bits 10 and 11 cleared and bit 7 set, then `SetNextComp(iGetTimerSystemTime())`, restores through `EIntr` on the same condition, and returns 0. Naming 0x10001010 as timer-2 mode is inferred from the public EE timer map. The host contract supplies that word through `NORRATH_HOST_CONTRACT`, which the reviewed EE profile does not define.

## Thread initializer

`InitThread` at 0x002d5eb8 returns -1 when `topId` is already greater than zero. Otherwise it requests a semaphore with word 1 set to 255 and word 2 set to 0, stores that result in `topSema`, and returns -1 when the result is negative. The thread parameter stores `topThread`, `stack.15`, 1024, `_gp`, and 0. `CreateThread`'s result is stored in `topId` even when negative; a negative result deletes `topSema` and returns -1. A nonnegative result clears the first two `topArg` words, calls `StartThread` with that argument, then `ChangeThreadPriority(GetThreadId(), 1)`, and returns `topId`. Other parameter words are not written. The host contract covers the existing-thread, semaphore-failure, thread-failure, and success paths.

## ExecPS2 patch

`InitExecPS2` at 0x002dda10 returns without further calls when `PatchIsNeeded` is zero. Otherwise it calls the local `setup` at 0x002dd940 for the first two `SysExecPS2Entry` pairs, `Copy` at 0x002dd950 from `osdsrc` to 0x80074000 for 1960 bytes, `FlushCache(0)` and `FlushCache(2)`, then one `GetEntryAddress`/`setup` pair starting at word 4. The gate starts at 2 and stops once it is no longer below 3, so this image walks one pair. The imported names are `kernel_execps2_setup`, `kernel_execps2_copy`, and `kernel_execps2_get_entry` because other copies share the original local spellings. The copied bytes are not reproduced here.

## Alarm patch

`InitAlarm` at 0x002dddc8 returns immediately when bit 0x100 of the word at 0x10001810 is set. Otherwise it uses the local `setup` at 0x002ddd60, `Copy` at 0x002ddd70, and `GetEntryAddress` at 0x002dddb8. The copies are 1856 bytes from `srcfile` to 0x80076000 and 40 bytes from `eenull` to 0x00082000, followed by `FlushCache(0)` and `FlushCache(2)`. Six entry pairs are then installed, starting at word 4 of the local `SysEntry` at 0x00445968. That table is imported as `kernel_alarm_sysentry`. Naming 0x10001810 as timer-3 mode is inferred. The host contract uses `NORRATH_HOST_CONTRACT` for that load.

## TLB handlers

`InitTLBFunctions` at 0x002dfbe0 calls the local `setup` at 0x002dfbd0 three times for the first six words of the local `SysEntry` at 0x00445d50, imported as `kernel_tlb_sysentry`. It copies 816 bytes from `tlbsrc` to 0x80075000 through the local `Copy` at 0x002dfb78, then `FlushCache(0)` and `FlushCache(2)`. Five `GetEntryAddress`/`setup` pairs follow, using the local `GetEntryAddress` at 0x002dfbc0, starting at word 6. It then stores `GetEntryAddress(3)` in `_kExecArg`. The gate starts at 3 and stops once it is no longer below 8.

## Syscall table address

`InitSystemCallTableAddress` at 0x002dd7d8 calls the local `setup` at 0x002dd8d8 for both pairs in the 16-byte local `SysEntry` at 0x00444a20, imported as `kernel_syscall_sysentry`. It then calls local `FindAddress` at 0x002dd790 over `[0x80000000, 0x80080000)` with `kFindAddress` and with the local `kCopy` at 0x002dd718. That second pattern is imported as `kernel_syscall_pattern_kcopy` because `kCopy` is not a unique symbol. While the hits still disagree, the lower adjusted address is advanced by four bytes and searched again. The adjustments are 524 and 360. The agreeing adjusted `kFindAddress` hit is stored in `_SyscallEntry`. The host contract covers an immediate match and one repair step. What the patterns recognize inside kernel memory is unknown.

## Timer initialization

`InitTimer` at 0x002ddf40 returns `0x80008001` when `g_Timer` offset 8 is nonnegative, before any other store. Otherwise it clears the first 8 bytes and offset 16, zero-fills the 8192-byte `g_CounterBuf`, and writes 128 pointer words at a 64-byte stride from offset 8128 down to 0. The stored values are the low 32 bits of addresses from `g_CounterBuf + 8192` downward. Offset 8128 is then cleared and `_InitAlarm` is called. `AddIntcHandler2(11, cbTimerHandler, 0, 0)` follows; a negative result returns `0x80009021`. A nonnegative handler id is stored at `g_Timer` offset 8. After `DIntr`, the word at 0x10001010 has its low two bits replaced by the caller's selector and bits 8 and 9 set. If bit 7 is still clear, bits 7, 10, and 11 are set and `SetT2_COUNT(0)` and `SetT2_COMP(0xffff)` run. `SetT2_MODE` and `EnableIntc(11)` always run on this path, and `EIntr` runs when `DIntr` returned nonzero. The function then returns 0. The host contract covers the already-installed, handler-failure, and success paths. Hardware timer behavior is unknown.
