# launchSRS

`launchSRS` at 0x002d01e0 is 1588 bytes and returns. Fresh compilation is 1608 bytes and is not an original-byte match. The host contract covers the heap retry, the high-address assert, the load destination, both `srs_main` results, module stop, and the restore tail. The default partial link is 1040 unresolved imports, zero COMMON, and 422 function bodies. Defining `launchSRS` removes that one import and adds 48. `cdLoad__FPcPvii` is one of the additions. `engineAddTask__FPFv_viPci` stays unresolved. No EE startup stage has run.

It stores 1 in `coreBeenInSRS` and then calls `lumpClear__Fi(50)`. It then stores 0 in `coreLaunchSRS`.

The reservation starts at `0x400000` bytes. Each block whose end is not above `0x013590ac` is freed and the request grows by `0x40000`. The end compare is signed. An address above `0x01000000` calls `__assert__FPCcUiT0i` with the unnamed text at `0x004866a0`, line 2326, the unnamed text at `0x0047aed8`, and the current return address. The kept block is freed after SRS returns. `cdLoad__FPcPvii` is called with the unnamed text at `0x004867b0`, destination `0x01000000`, and two zero integers. That symbol is at `0x00165120` and is 276 bytes. `norrath_cd_load` does not publish that address, so the import stays separate.

`engineClear__Fv` runs, then three free-memory prints separated by `soundSleep__Fv` and by `keyboardFree__Fv` plus `textureFreeCacheBuffer__Fv`. The two query results are shifted right with `srl` by 10. `enableMallocWrapper` is set to 1, `memSetMark__Fi(0)` runs, and another print follows.

Every word from `_srsctorsstart` through `_srsctorsend` is called. Those symbols are 60 bytes apart. A call argument is not set in the delay slot. What the pointed-to functions receive is unknown.

`mallinfo` runs on a local buffer. `flag_word` is 1 when bit 0 of `netGameLaunchInfo` is set, otherwise 2. `count_word` starts at 4 and `zero_word` starts at 0. The two `char**` lists each address four stack slots 32 bytes apart. `textGetString__FPc` is called with `netGameLaunchInfo + 199`, and `ustrcpy__FPcPUs` copies that result into a local buffer. `srs_main` then receives `netGameLaunchInfo + 7`, the flag and count words, the first pointer list, the second word list, the zero word, the third pointer list, the fourth word list, 1 when `netInterfaceIsUp` is zero, `netGameLaunchInfo + 135`, the copied text, and byte 295. What `srs_main` writes is unknown.

`engineDisplayDisable` is set to 10000. When the result is 100, byte 4 of `netGameLaunchInfo` receives the low byte of the count word, an empty countdown of that word runs, and byte 6 receives the low byte of the zero word. The countdown body is five `nop` instructions. For each index below the count, `sscanf` parses the first and third pointer-list entries with the unnamed text at `0x0047afb0`. The first parse's low bytes are stored at record offsets -49 through -46. The second parse's low bytes are stored at -33 through -30. The word stored at the record is the fourth-list word when `sscanf` returns 4, otherwise the second-list word. The record starts at `netGameLaunchInfo + 312` and advances by 4. The original text is not copied here.

A result other than 100 skips that parse. Both results then print, search for a module by the unnamed text at `0x0047afc0`, and, when the id is nonnegative, call `sceSifStopModule` and `sceSifUnloadModule`. `enableMallocWrapper` is cleared. Marks are cleared and deleted, the reserved block is freed, `memScrambleFree__Fv` and `mallinfo` run, and the texture cache, keyboard, engine, and sound are restored. `soundRestore__Fb` is passed 1.

A result of 100 stores 1 in `MC_ProtectRamSlot`, `netInterfaceIsUp`, and `fromSRSButEnteringGame`. Any other result calls `frontEndInit__Fi(0)` and `rejectForeignModules__Fv`, then clears `netInterfaceIsUp`. Both paths store 1 in `fromSRS`, call `sceMcInit`, `lumpLoad` the unnamed text at `0x004867d0`, and resolve `haloTexture2` through `haloTexture7` from that lump. `lumpLock__Fi(50)` and `MC_Reset__Fv` follow. The word 120 is stored at `pad + 268`, `pad + 180`, `pad + 92`, and `pad + 4`.

These addresses have no ELF symbol. Each is its own unresolved object.

| Address | Use | Bytes before the NUL |
| --- | --- | --- |
| 0x0047aeb8 | allocation name | 11 |
| 0x0047aed8 | assert text | 8 |
| 0x004867b0 | `cdLoad` name | 7 |
| 0x0047af30 | first memory print | 26 |
| 0x0047af50 | second memory print | 27 |
| 0x0047af70 | third memory print | 19 |
| 0x0047af88 | mark print | 17 |
| 0x0047afa0 | print before `srs_main` | 11 |
| 0x0047afb0 | `sscanf` text | 11 |
| 0x0047afc0 | module name | 13 |
| 0x0047afd0 | last memory print | 20 |
| 0x004867b8 | print after `srs_main` | 3 |
| 0x004867c0 | print after the result branch | 3 |
| 0x004867c8 | print after the module | 3 |

`0x004866a0` and `0x004867d0`, and the six halo names, are the same unnamed objects `main` already imports.
