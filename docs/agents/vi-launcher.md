# VI launcher, timer, language, VRAM clear, product id, ezNet exits

Package notes for the 22 unresolved imports assigned to `vi-launcher`. Addresses
and bytes were read from the pinned SLUS-20565 image with EE binutils 2.45.1.
Each source file opens with an `Observed` comment giving address and size. No
routine here has run on the EE or as gameplay. Fresh compilations are not
original-byte matches. Every unit has a `host_contract.cpp` that includes the
function source and returns nonzero when a checked call, argument or stored
value differs. Host contracts are not plan inputs.

The work was taken over from an agent that was cut off before committing anything.
All 22 uncommitted units were re-checked here against the image disassembly
(addresses, string literals, call order, offsets, argument registers), compiled
with the plan's `authored_cpp` profile, host-tested, and committed one per commit.
The 22 objects also link together with `ld -r` without duplicate definitions.

## Imports closed (22)

`CheckNetwork__15VINetworkStatus`, `ClearVRAM__FUcUcUcUc`, `Close__7VITimer`,
`Free__10VILauncher`, `GetDefault__10VILanguage`, `Init__10VILauncherb`,
`Init__7VITimer`, `Initialize__15VINetworkStatus`, `LoadExecPS2`,
`SetDefault__10VILanguage12VILanguageID`, `Setup__10VILauncher`,
`Station__10VILauncheriPCcN22i`, `VIDisplayLoadingScreen__Fb`,
`VIMemCardSetProductId__FPCc`, `VIMemCardSetProductTitle__FPCc`,
`_$_10VILauncher`, `_$_15VINetworkStatus`, `__10VILauncher`,
`__15VINetworkStatus`, `exitEzNetCnf`, `exitEzNetCtl`, `ezNetCtlDownInterface`.

Data also defined by these units: `_10VILanguage$Default` (GetDefault),
`_7VITimer$HandlerID/$SystemTime/$TimerMode/$Ready` (Init__7VITimer),
`m_rpcRunning` at 0x01192c6c (exitEzNetCtl), `gPatchPresent`, `gPowerOffTid`,
`gPowerOffSid`, `gPowerOffTerminate`, `sModuleId[16]` (Init__10VILauncherb).
`sProductId` and `sProductTitle` stay local with their observed initial bytes.

Observed: `m_rpcRunning`, `sModuleId` and the `gPowerOff*` words are local in the
original. `exitEzNetCtl` and `Init__10VILauncherb` publish them so other units can
share them; the original `exitEzNetCnf` word at 0x01192c68 is a different local.
`gPowerOffTerminate`, `gPatchPresent` are published only for the same reason.

## Imports opened

Referenced but not defined here: `AddIntcHandler`, `DisableIntc`, `EnableIntc`,
`RemoveIntcHandler`, `Handler__7VITimeri`, `ChangeThreadPriority`, `GetThreadId`,
`DeleteSema`, `DeleteThread`, `ReferThreadStatus`, `RotateThreadReadyQueue`,
`SignalSema`, `FlushCache`, `dmaGIF`, `exit`, `printf`, `strcpy`, `memset`, `rand`,
`srand`, `__wrap_free`, `__wrap_memalign`, `__builtin_delete`, `sceCdReadClock`,
`sceDmaGetChan`, `sceDmaSendN`, `sceGifPkAddGsData`, `sceGifPkInit`,
`sceGifPkReset`, `sceGifPkSize`, `sceGifPkTerminate`, `sceGsSyncPath`,
`sceGsSyncV`, `sceGszbufaddr`, `sceInetCtlGetState`, `sceInetGetInterfaceList`,
`sceLibnetInitialize`, `sceLibnetTerminate`, `sceMcEnd`, `sceMcGetInfo`,
`sceMcSync`, `scePadEnd`, `scePadInit`, `sceSifCallRpc`, `sceSifStopModule`,
`sceSifUnloadModule`, and the VI-family symbols `VIDisplayScreen16__FPCcib`,
`VIGetGameOptions__Fv`, `Username__C13VIGameOptions`, `Password__C13VIGameOptions`,
`initEzNetCtl`, `setup_main__Fv`, `station_main__FiPCcPcT2T1T1i`, `SetArg`,
`TerminateLibrary`, `_LoadExecPS2` and `_kExecArg`. Many of these are SDK or
other-package symbols that may already be defined in the shared link; compare
against `unresolved-imports.json` after integration. `printf`, `strcpy`, `exit`
and `sceSifInitIopHeap` stay with their owners.

## Build-plan entries needed

Profile `authored_cpp`, empty header closure, kind
`independently_authored_source`, appended in this order. `snapshot.json` must also
record each source and its `host_contract.cpp`. Neither shared file is edited here.

- `clear_vram.o`, `sources/vi-launcher-clear-vram/clear_vram.cpp`, SHA-256 `60afe6cad548152d278e92384c23b730dad7494db574e792c9b1c6d72c9e06bd`
- `launcher_ctor.o`, `sources/vi-launcher-ctor/launcher_ctor.cpp`, SHA-256 `dd3834493226ed130e89c24083ddffd943858cd1625266708e7fcd5c3d827d89`
- `launcher_dtor.o`, `sources/vi-launcher-dtor/launcher_dtor.cpp`, SHA-256 `6b1ded2dc9ef67b3d679e2ecdaa7ba8d8ff8ce3e4a184f622c7d9f3f14278efd`
- `exit_cnf.o`, `sources/vi-launcher-eznet-cnf/exit_cnf.cpp`, SHA-256 `5f0214453a3884990bfbb6a2821d6863b9352ce74fc9493f75f3864910bd1a26`
- `exit_ctl.o`, `sources/vi-launcher-eznet-ctl/exit_ctl.cpp`, SHA-256 `04a71dfbb480cae4ecee04e6e55c3218ee846a44b27446a6046d25d7d87889f6`
- `down_interface.o`, `sources/vi-launcher-eznet-down/down_interface.cpp`, SHA-256 `e4ea5f69ac3b5b6a98775586b123cdb6f6eb7987bc9975c18c19eddc5e622d6e`
- `launcher_free.o`, `sources/vi-launcher-free/launcher_free.cpp`, SHA-256 `1a4ac5c14da404bd93bbebbdc3f5d2316a9e6c1c3f17c32f6e0320a08f0d20d3`
- `launcher_init.o`, `sources/vi-launcher-init/launcher_init.cpp`, SHA-256 `5d95fb54ca4200b499db267b733b60b23a813b4402c02bf6f54a115ed4b7ebbe`
- `language_get.o`, `sources/vi-launcher-language-get/language_get.cpp`, SHA-256 `93aa6b43b260786a7e260aa89ae4ed358f781ea3326da59413ece7fe8cf0f5c9`
- `language_set.o`, `sources/vi-launcher-language-set/language_set.cpp`, SHA-256 `bf896a62a4c67714f72c5ee174ef8de1f60105f2cc09698236fe16c8d26d72ef`
- `load_exec.o`, `sources/vi-launcher-load-exec/load_exec.cpp`, SHA-256 `f5283a80eeacd5d409bde777577176695017e5f5954beca6c3cfb2dc9d0d1058`
- `loading.o`, `sources/vi-launcher-loading/loading.cpp`, SHA-256 `9e5527548d6f1c730a5ca97baea506e138f926b39965421b09a9e208a8302ab4`
- `net_check.o`, `sources/vi-launcher-net-check/net_check.cpp`, SHA-256 `a1537bba04207ad4512a6f415db539b3a5a9d6fed78164a41df708a0e3a08fea`
- `net_ctor.o`, `sources/vi-launcher-net-ctor/net_ctor.cpp`, SHA-256 `34bde2ada00542fbb7c08027817ad70b964b9debdf6a97bd5af41eec918968a5`
- `net_dtor.o`, `sources/vi-launcher-net-dtor/net_dtor.cpp`, SHA-256 `efe89cd5c9100f20e407d75b87f04fb81eb1db931ca6611348f9e0ad99222a49`
- `net_init.o`, `sources/vi-launcher-net-init/net_init.cpp`, SHA-256 `5a26b2aaa805cf56435b18191030cc5601861c28653cc8ae4a3098eb09c99074`
- `product_id.o`, `sources/vi-launcher-product-id/product_id.cpp`, SHA-256 `03a525420cf95301565eb273e5b0dd8b35a79de1b47eab226e54d1d305cebae9`
- `product_title.o`, `sources/vi-launcher-product-title/product_title.cpp`, SHA-256 `79cd03e6007c76eac5e175a5ca06347aa588367ec2ea987bc8615a252fa5d254`
- `setup.o`, `sources/vi-launcher-setup/setup.cpp`, SHA-256 `32df6ee732c9feccaddb549459a18472be1fb0326d1d0f7109ca98f05cc609ac`
- `station.o`, `sources/vi-launcher-station/station.cpp`, SHA-256 `ca4af5ec603ac6c8a7dc77a16c894f3b500db8079172c611fcb9751cc2825cff`
- `timer_close.o`, `sources/vi-launcher-timer-close/timer_close.cpp`, SHA-256 `8ecc7bd6eb83ba0cc837da508b55419a34e6d7c96d7587eb78ad5bcabdfad9bc`
- `timer_init.o`, `sources/vi-launcher-timer-init/timer_init.cpp`, SHA-256 `988d97365b44eafe216444806df2e2afd9d3c65dbe0fd9c124f1e507db1ded0b`

## Authored but never executed

All 22 functions. Only host contracts with substituted callees have run them; none
has run on the EE, so no hardware behaviour is established.

## Unknown

- The sizes and field layouts of `VILauncher` (fields seen at offsets 0, 4, 8, 264)
  and `VINetworkStatus` (offsets 0..59, 60, 64..103, 104) beyond the offsets used.
- What the two absolute stores in `Init__7VITimer` (0x10000010, 0x10000020) program,
  and the GS packet contents in `ClearVRAM`.
- What `__builtin_delete` releases and what `sceLibnetTerminate` frees.
- The password and username length limits in `Station`.
- Whether the original `sModuleId`, `m_rpcRunning` and `gPowerOff*` words are
  file-local in separate units, as the symbol table suggests.
