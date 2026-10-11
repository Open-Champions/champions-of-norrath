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
