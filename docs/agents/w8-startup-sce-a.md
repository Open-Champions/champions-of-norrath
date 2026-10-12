# Startup SCE/IOP library and boot-support (1/4)

Package `w8-startup-sce-a`, branch `agents/w8-startup-sce-a`. Sources are under `experimental/sources/w8-startup-sce-a-*`, one directory per function, each its own commit. All addresses come from the pinned private executable `work/private/SLUS_205.65` (symbol table, bytes and instruction stream); nothing from it is committed.

## How each unit was checked

- Syscall stubs and the one assembly body (`.S`, `authored_entry_asm` flags): assembled with the reviewed toolchain and compared byte for byte against the executable with the `host_check.cpp` beside each (same checker as `sony-cd-poll-sema`). A wrong address makes it fail (tried once).
- `_ctype_`: `host_check_data.py` compiles nothing; it reads the reviewed-toolchain object and compares all 257 bytes with the executable (`python3 -I host_check_data.py <exe> 0x0047e1f0 <object> _ctype_`). A shifted address fails (tried).
- Everything else (`authored_cpp` flags): compiled with the reviewed toolchain, then a `host_contract.cpp` with the system C++ compiler runs it against mocks that log call order, arguments and stored values. These are semantic contracts. None of the compiled bodies is an original-byte match. No EE service ran.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- I did not run a full partial build (single units only); no private plan copy was linked. `unresolved-imports.json` from the integrator is authoritative for what is still open.

## Imports closed (names from the package list)

DPRINT, DeleteThread, DisableDmac, GetOsdConfigParam, Handler__7VITimeri, PowerOffCB, PowerOffHandler__FPv, PowerOffThread__FPv, RemoveDmacHandler, RotateThreadReadyQueue, SetArg, SetOsdConfigParam, SetTimerAlarm, SuspendThread, TerminateLibrary, TimerUSec2BusClock, VIDisplayScreen16__FPCcib, VIGetGameOptions__Fv, VIStartKeyboardDriver__Fv, VSync, VSync2, WakeupThread, _LoadExecPS2, __wrap_free, __wrap_memalign, _ctype_. That is 26 of the 30.

Not closed: `__EH_FRAME_BEGIN__`, `_args`, `_bssend`, `_end` (see "Unknown").

Also defined, because the closed functions call or take the address of them (none was in the list):
`_DisableDmac`, `SetVSyncFlag`, `iSetAlarm`, `Bounce__FiUsPv`, `_sceCd_Poff_Intr`, `kernel_setarg_copy`, `kernel_setarg_setup`, `kernel_setarg_pattern_kcopy`, `kernel_setarg_sysentry`. The last four are added names for the originally local `Copy` (0x002ddac0), `setup` (0x002ddb00), `kCopy` (0x002ddad0) and `SysEntry` (0x004451f8) of the object that holds `SetArg`; several other local objects carry the same original names, and the `kernel-patches` package used the same `kernel_<family>_...` convention for its own families. `_sceCd_Poff_Intr` is local in the original and is published global here, as `_lf_bind` was.

## Observed

| symbol | address | image bytes | check | source dir (`w8-startup-sce-a-...`) |
|---|---|---|---|---|
| DeleteThread | 0x002d4ef0 | 16 | byte match, service 33 | delete-thread |
| RemoveDmacHandler | 0x002d4e10 | 16 | byte match, service 19 | remove-dmac-handler |
| RotateThreadReadyQueue | 0x002d4f90 | 16 | byte match, service 43 | rotate-thread-ready-queue |
| SetOsdConfigParam | 0x002d5180 | 16 | byte match, service 74 | set-osd-config-param |
| GetOsdConfigParam | 0x002d5190 | 16 | byte match, service 75 | get-osd-config-param |
| SuspendThread | 0x002d5050 | 16 | byte match, service 55 | suspend-thread |
| WakeupThread | 0x002d5010 | 16 | byte match, service 51 | wakeup-thread |
| _LoadExecPS2 | 0x002d4d20 | 16 | byte match, service 6 | load-exec-ps2 |
| _DisableDmac | 0x002d4e50 | 16 | byte match, service 23 | disable-dmac-service |
| SetVSyncFlag | 0x002d5450 | 16 | byte match, service 115 | set-vsync-flag |
| iSetAlarm | 0x002d4ec0 | 16 | byte match, service -254 | i-set-alarm |
| kernel_setarg_copy (local Copy) | 0x002ddac0 | 16 | byte match, service 90 | setarg-copy |
| kernel_setarg_setup (local setup) | 0x002ddb00 | 16 | byte match, service 116 | setarg-setup |
| kernel_setarg_pattern_kcopy (local kCopy) | 0x002ddad0 | 48 | byte match | setarg-pattern-kcopy |
| _ctype_ | 0x0047e1f0 | 257 | data match | ctype-table |
| kernel_setarg_sysentry (local SysEntry) | 0x004451f8 | 8 | contract + object layout (one relocation at offset 4) | setarg-sysentry |
| DisableDmac | 0x002d5c90 | 104 | contract | disable-dmac |
| VSync | 0x002d5570 | 144 | contract | vsync |
| VSync2 | 0x002d5600 | 164 | contract | vsync2 |
| TimerUSec2BusClock | 0x002df108 | 112 | contract | timer-usec-to-bus-clock |
| SetTimerAlarm (+ local handler 0x002df258) | 0x002df2b8 | 300 | contract | set-timer-alarm |
| DPRINT | 0x002d4818 | 40 | contract (trivial) | dprint |
| TerminateLibrary | 0x002ddc28 | 8 | contract; compiled body is also `j InitTLB` | terminate-library |
| VIStartKeyboardDriver__Fv | 0x002cbe98 | 52 | contract | vi-start-keyboard-driver |
| __wrap_free | 0x00144ac0 | 44 | contract | wrap-free |
| __wrap_memalign | 0x001449e0 | 56 | contract | wrap-memalign |
| PowerOffThread__FPv | 0x002cc128 | 180 | contract | power-off-thread |
| Bounce__FiUsPv | 0x002cc1e0 | 32 | contract | bounce |
| PowerOffHandler__FPv | 0x002cc200 | 48 | contract | power-off-handler |
| _sceCd_Poff_Intr | 0x002e28c8 | 84 | contract | cd-poff-intr |
| PowerOffCB | 0x002e2920 | 120 | contract | power-off-cb |
| SetArg | 0x002ddb10 | 280 | contract | set-arg |
| Handler__7VITimeri | 0x011345b8 | 56 | contract | vi-timer-handler |
| VIGetGameOptions__Fv | 0x01019968 | 12 | contract | vi-get-game-options |
| VIDisplayScreen16__FPCcib | 0x0106bee8 | 704 | contract | vi-display-screen16 |

Behavior read from the instruction stream, stated in each source header: the syscall stubs load a service number into `$v1`, issue `syscall` and return, leaving arguments and result in their registers. `DisableDmac` samples Status bit 0x10000, runs DIntr when set, calls `_DisableDmac`, `sync`, then EIntr on the sampled bit. `VSync`/`VSync2` write 4 to the word at 0x1000f000 under DIntr/EIntr, then poll bit 4 (`VSync2` also stops when a stack word, handed to `SetVSyncFlag` with a 64-bit slot, becomes nonzero, and returns the slot). `TimerUSec2BusClock(a, b)` returns `a*147456000 + b*147456000/1000000` as 64 bits. `SetTimerAlarm` takes a 16-byte node from the `g_pFreeAlarm` list, allocates a counter, registers the local handler and returns `(node<<4)|(counter&0xfe)|1`; its error codes are 0x80000016 (null handler), 0x80008005 (empty list) or the negative counter result. `PowerOffThread__FPv`, `PowerOffHandler__FPv`, `PowerOffCB`: strings, call order and stores are in the source headers; the `powerOffMode`/`Init_seq` stores sit in delay slots and land before the neighbouring call starts, which the contracts check. `SetArg` writes a pointer table at `_kExecArg` buffer offset 0.. and the strings from offset 64, limiting the argument count to 15. `VIDisplayScreen16__FPCcib` reads `\DATA2\<text><index>.16;1` with retries and draws the 640x448 16-bit image by GIF packet; `Handler__7VITimeri` adds 2 to `_7VITimer$SystemTime`, stores `_7VITimer$TimerMode` to 0x10000010, `sync`, `ei`.

## Inferred

- `TimerUSec2BusClock` first parameter is "seconds" because `DelayThread` passes 0 and the second is microseconds; the 147,456,000 constant is the observed multiplier.
- `0x1000f000` as an interrupt status register, `0x12000000`-range stores as GS privileged registers, and `0x10000010` as a timer register are inferred from address ranges, not documented in the repo.
- The return type of `Bounce__FiUsPv` and `Handler__7VITimeri` is `int` because the original leaves the callee's/zero result in `$v0`; the mangled names say nothing about the return.
- `VSync` returns EIntr's result when interrupts were enabled and 0 otherwise (what the register holds on return); whether the original intends a value is unknown.

## Unknown

- `__EH_FRAME_BEGIN__`: first byte of `.eh_frame` at 0x00453970. `link-image-bounds` already documents why it cannot be supplied under the `-fno-unwind-tables` profile; I agree and did not add a sentinel.
- `_bssend` (0x008eab1c) and `_end` (0x008ec91c) are absolute section-layout values; `_args` (0x005192c0) is a zero-size local in bss whose syscall-buffer size is unverified (`link-image-bounds` notes a 384-byte upper bound). None is ordinary C++ storage, so none was invented.
- Meaning of the operands of `DisableDmac` (channel mask?), the 0x4402 devctl command, `sceCdPowerOff`'s result word, the 1000 passed to `iSetAlarm`, the arguments of `lgKbMInit` (1, 0, 15, 0), the 2-tick step in `Handler__7VITimeri`, the meaning of the three observed stack-argument positions of `sceGifPkRefLoadImage`, and the third buffer in `VIDisplayScreen16__FPCcib` (cleared, never read).
- Size and layout of `sGameOptions` (0x011e2dd0), `sCdRmode` (0x011e7d60; at least 4 bytes used) and `gCdFile` (0x011e7d68; first word is the sector number). They are imports here because their definitions belong to the launcher module owner. The three words behind `sceCdPOffCallback` (`sceCdPoffCbfunc` 0x008e8148, `sceCdCbPoff_Gp` 0x008e814c, `sceCdPoffCbdata` 0x008e8150) are imported the same way.
- Where each contract mock stands in for kernel, CD, GS or DMA behavior, that behavior is unverified.

## Authored but never executed

All of it. The syscall stubs have never made a kernel call, no function ran on EE hardware or an emulator, and none ran as part of the startup chain. `DPRINT`'s contract cannot fail (the function is empty); its check is the observed absence of any call or load in the original. `TerminateLibrary`'s contract checks only that `InitTLB` is called.

## Imports opened

Referenced but not defined by this package. Names marked * were found defined in another source in this tree; the rest I could not find and the integrator should check `unresolved-imports.json`:
`AllocTimerCounter`, `SetTimerHandler`, `StartTimerCounter` (timer counter services used by `SetTimerAlarm`), `InitTLB`, `__udivdi3` (compiler helper from `TimerUSec2BusClock`), `lgKbMInit`, `netFree__Fb`, `sceDevctl`, `sceCdPowerOff`, `sceCdPOffCallback`'s three words (above), `sceCdDiskReady`, `sceCdSearchFile`, `sceCdRead`, `sceCdSync`, `sceSifAddCmdHandler`, `sceGsResetGraph`, `sceGsSyncPath`, `sceDmaGetChan`, `sceGifPkInit`, `sceGifPkReset`, `sceGifPkRefLoadImage`, `sceGifPkEnd`, `sceGifPkTerminate`, `sGameOptions`, `sCdRmode`, `gCdFile`, `printf`, `scePrintf`, `sprintf`, `strlen`, `memalign`, `memset`, `memAlloc__FiPc`, `memFree__FPv`, `free`*, `DIntr`*, `EIntr`*, `FlushCache`*, `WaitSema`*, `iSignalSema`*, `sceDmaSend`*, `sceGsSyncV`*, `Init_seq`*, `_icmd_bind`*, `sid`*, `powerOffMode`, `enableMallocWrapper`*, `_kExecArg`*, `g_pFreeAlarm`*, `_7VITimer$SystemTime`*, `_7VITimer$TimerMode`*.

## Build-plan entries needed

Append these to `experimental/build-plan.json` inputs (host contracts, `host_check.cpp` and `host_check_data.py` are not inputs). `input_order` is left to the integrator, as is the `snapshot.json` record of each source. `_ctype_` and `kernel_setarg_sysentry` are data-only units (empty `emitted_functions`). `_ctype_` may collide with a future libc-format/Newlib `_ctype_` definition; `libc-format-ctype` only publishes a view, not the symbol. One JSON object per line:

```
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "Bounce__FiUsPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "bounce.o", "profile": "authored_cpp", "sha256": "76b3a5922307471921111e9f521c5ed00c4a3592d896d83c1b187778e354b7d9", "source": "sources/w8-startup-sce-a-bounce/bounce.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "_sceCd_Poff_Intr"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "cd_poff_intr.o", "profile": "authored_cpp", "sha256": "18f73f43e1384dfe83783aaa0edd24e03014fe1d87833f34092cbd9a1149fa6e", "source": "sources/w8-startup-sce-a-cd-poff-intr/cd_poff_intr.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "ctype_table.o", "profile": "authored_cpp", "sha256": "aed03768f7aa65e8cceced2fbc7a715b55a1c38536b1d10b39a971db2bffe096", "source": "sources/w8-startup-sce-a-ctype-table/ctype_table.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "DeleteThread"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "delete_thread.o", "profile": "authored_entry_asm", "sha256": "e0f18d0523190aa15d848db920e7e43729fbfb39149de5e1596973fe5f3efa92", "source": "sources/w8-startup-sce-a-delete-thread/delete_thread.S"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "DisableDmac"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "disable_dmac.o", "profile": "authored_cpp", "sha256": "39a3d8303fb7ff1f9c2827b6c9ca1695dba45709b40fe79d485ae11c035378d8", "source": "sources/w8-startup-sce-a-disable-dmac/disable_dmac.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "_DisableDmac"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "disable_dmac_service.o", "profile": "authored_entry_asm", "sha256": "a510128c0cad78b00e62647644f58729b41973d6e44effc9dfb4be93a882b2a6", "source": "sources/w8-startup-sce-a-disable-dmac-service/disable_dmac_service.S"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "DPRINT"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dprint.o", "profile": "authored_cpp", "sha256": "34c4360825b0aefa1943233e4718822a4527f650c73327f9361bdf3fb1e92616", "source": "sources/w8-startup-sce-a-dprint/dprint.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "GetOsdConfigParam"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_osd_config_param.o", "profile": "authored_entry_asm", "sha256": "64b3492cf4f01ab1c2f528696733492be59a8e97bd9c605073c46e57c208cd93", "source": "sources/w8-startup-sce-a-get-osd-config-param/get_osd_config_param.S"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "iSetAlarm"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "i_set_alarm.o", "profile": "authored_entry_asm", "sha256": "f58a1a565e5a1e33fd71cf43fe9672f03ba9712c9374936854dbcf3a9d3e7357", "source": "sources/w8-startup-sce-a-i-set-alarm/i_set_alarm.S"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "_LoadExecPS2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "load_exec_ps2.o", "profile": "authored_entry_asm", "sha256": "dd9beeb5f4dd4372fe208db83bf36eea1c2af9a9c3ffb7dfc8b41355dc45fe41", "source": "sources/w8-startup-sce-a-load-exec-ps2/load_exec_ps2.S"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "PowerOffCB"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "power_off_cb.o", "profile": "authored_cpp", "sha256": "90aa9008aac09381c784cf14b10bf5ecc4de24256cd6816b3efd9a8c22b601eb", "source": "sources/w8-startup-sce-a-power-off-cb/power_off_cb.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "PowerOffHandler__FPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "power_off_handler.o", "profile": "authored_cpp", "sha256": "1609b31345033dafa56c9ee96125002d78604ed3a39e5afa2097e2482fd5818f", "source": "sources/w8-startup-sce-a-power-off-handler/power_off_handler.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "PowerOffThread__FPv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "power_off_thread.o", "profile": "authored_cpp", "sha256": "3494fdbed6b10f0fd44680db7aae9127687e538bba465744395f7ef3537cec99", "source": "sources/w8-startup-sce-a-power-off-thread/power_off_thread.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "RemoveDmacHandler"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "remove_dmac_handler.o", "profile": "authored_entry_asm", "sha256": "2120b58d9a82771adf6f37af6f4424176ae9ec0ad20dc0cdad5f2b0b8f0e8601", "source": "sources/w8-startup-sce-a-remove-dmac-handler/remove_dmac_handler.S"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "RotateThreadReadyQueue"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "rotate_thread_ready_queue.o", "profile": "authored_entry_asm", "sha256": "7b49c68ada71f1b904800172e98fe3689887511632b37227b652a24acbd1463f", "source": "sources/w8-startup-sce-a-rotate-thread-ready-queue/rotate_thread_ready_queue.S"}
{"emitted_functions": [{"compiled_body_bytes": 280, "name": "SetArg"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "set_arg.o", "profile": "authored_cpp", "sha256": "fa358bd3d3c847a29399addd874bce36d3f2579e8ef7931a77d44fc9c36c19d3", "source": "sources/w8-startup-sce-a-set-arg/set_arg.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "SetOsdConfigParam"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "set_osd_config_param.o", "profile": "authored_entry_asm", "sha256": "e75dfb47104f52719caa30493c2ac95172feee8a63292c2447af9a4e2dcb78a2", "source": "sources/w8-startup-sce-a-set-osd-config-param/set_osd_config_param.S"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "_ZL13alarm_handleriiiPv"}, {"compiled_body_bytes": 344, "name": "SetTimerAlarm"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "set_timer_alarm.o", "profile": "authored_cpp", "sha256": "abff744cefa0c10b559d968b3e336535311198470341cc0447ff3371a905870a", "source": "sources/w8-startup-sce-a-set-timer-alarm/set_timer_alarm.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "SetVSyncFlag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "set_vsync_flag.o", "profile": "authored_entry_asm", "sha256": "2b54fd1382f5d3a784c3cc0d711f2776426de110c99fd295220f8b6f0a3ad02d", "source": "sources/w8-startup-sce-a-set-vsync-flag/set_vsync_flag.S"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "kernel_setarg_copy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "setarg_copy.o", "profile": "authored_entry_asm", "sha256": "08190c07516052882ff2f6519085485674f89e14cb3379e6fceaa239fdde5583", "source": "sources/w8-startup-sce-a-setarg-copy/setarg_copy.S"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "kernel_setarg_pattern_kcopy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "setarg_pattern_kcopy.o", "profile": "authored_entry_asm", "sha256": "66ea0ff04fed92c07c8c607557b23d82aed85ad8032162439a3c4954de767b37", "source": "sources/w8-startup-sce-a-setarg-pattern-kcopy/setarg_pattern_kcopy.S"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "kernel_setarg_setup"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "setarg_setup.o", "profile": "authored_entry_asm", "sha256": "a4378d7dcc66c5ababfa15d25b7c3312b9621016a1d471fb879cf526b7b40ef6", "source": "sources/w8-startup-sce-a-setarg-setup/setarg_setup.S"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "setarg_sysentry.o", "profile": "authored_cpp", "sha256": "a51327f85a8b27bec4acd87a9244cc41658b21e216b438b676ee109edf697fc2", "source": "sources/w8-startup-sce-a-setarg-sysentry/setarg_sysentry.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "SuspendThread"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "suspend_thread.o", "profile": "authored_entry_asm", "sha256": "0f8aebcca4680eefae7e98f219ef402cf2fbe60a50c742edb71e5744be542639", "source": "sources/w8-startup-sce-a-suspend-thread/suspend_thread.S"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "TerminateLibrary"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "terminate_library.o", "profile": "authored_cpp", "sha256": "291a98f2d7d6fe25ba24765d6564502e6b6a907ff915ad20291c2c8b2b7d419e", "source": "sources/w8-startup-sce-a-terminate-library/terminate_library.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "TimerUSec2BusClock"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "timer_usec_to_bus_clock.o", "profile": "authored_cpp", "sha256": "a9f6eb66ec6d3917ce33f2f556ee2ed84f8baf9f3cd21f482805fa347522597a", "source": "sources/w8-startup-sce-a-timer-usec-to-bus-clock/timer_usec_to_bus_clock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 644, "name": "VIDisplayScreen16__FPCcib"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "display_screen16.o", "profile": "authored_cpp", "sha256": "915a2cfd763872d34ffe4931a79e929dcdf8b3a919d9563e36b7916edf675474", "source": "sources/w8-startup-sce-a-vi-display-screen16/display_screen16.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "VIGetGameOptions__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_game_options.o", "profile": "authored_cpp", "sha256": "196cae127cb6f91a2392c84214c2b84d90e6764d14669939bfb4a682c3765e19", "source": "sources/w8-startup-sce-a-vi-get-game-options/get_game_options.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "VIStartKeyboardDriver__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "start_keyboard_driver.o", "profile": "authored_cpp", "sha256": "14786d15c76abb579b0bebad315eba46ece6223c09f3518dae415d23e2bbc309", "source": "sources/w8-startup-sce-a-vi-start-keyboard-driver/start_keyboard_driver.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "Handler__7VITimeri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "timer_handler.o", "profile": "authored_cpp", "sha256": "213d591a41d38e061fde17c430355c2665e87d540821b2179967ce373c4bfe4e", "source": "sources/w8-startup-sce-a-vi-timer-handler/timer_handler.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "VSync"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "vsync.o", "profile": "authored_cpp", "sha256": "eff603ed212d6a07593bedfd971707b21532d501bc49d0cd7a2d68a9546d3bf6", "source": "sources/w8-startup-sce-a-vsync/vsync.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 224, "name": "VSync2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "vsync2.o", "profile": "authored_cpp", "sha256": "b3179e6e651c2d4032b69f7c78f112a3b9d94b161d5654df66515e2cf74c1e0f", "source": "sources/w8-startup-sce-a-vsync2/vsync2.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "WakeupThread"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "wakeup_thread.o", "profile": "authored_entry_asm", "sha256": "14fc41c77de0e34baa0a83d0d0d84b33ebc70a34199519343be821219ea600d5", "source": "sources/w8-startup-sce-a-wakeup-thread/wakeup_thread.S"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "__wrap_free"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "wrap_free.o", "profile": "authored_cpp", "sha256": "25706f94e6a852549b81cac06a3cb4e4bb93ffa137a6f6f6c041ca57fcf5581f", "source": "sources/w8-startup-sce-a-wrap-free/wrap_free.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "__wrap_memalign"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "wrap_memalign.o", "profile": "authored_cpp", "sha256": "e2674ab75737c1fdd87c22012282b38d73c59ab05a915aca416ce44e77b30618", "source": "sources/w8-startup-sce-a-wrap-memalign/wrap_memalign.cpp"}
```
