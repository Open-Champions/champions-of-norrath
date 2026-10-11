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
