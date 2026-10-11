# Engine clear, restore, and front end

Package notes for the unresolved imports named by the fresh 344-input partial
link at `d5883aa`: 647 unresolved symbols, zero COMMON. Original names and
addresses were read from the pinned SLUS-20565 symbol table. Instruction
behavior was read from that image with EE binutils 2.45.1. No routine here
has run on the EE or as gameplay.

Host contracts are not plan inputs. They include the function source and fail
with a nonzero status when a checked call, argument, or stored value differs.

## frontEndInit

Observed `frontEndInit__Fi` at 0x001c96b0 is 372 bytes and returns. Four
callers reach it. Fresh compilation is 540 bytes and is not an original-byte
match.

`MC_GetReadyForMainMenu` runs first. `hudNoDraw` and `nmPlayers` become 1,
`netNetworkMode` and `musicPlaying` become 0. `musicPlaying` is cleared on
both sides of the `frontEndSuspend` test.

When `frontEndSuspend` is zero and `attemptLoadGame` is nonzero and
`MC_LastModeSuccessful` is zero, a nonzero `menuNm` becomes 15, then
`attemptLoadGame` is cleared and `MC_LoadGameBrowser(0, 1)` runs. Every other
combination of those two bytes stores `frontMenu` in `menu`, calls
`menuSetPos(frontMenu, 0)`, and then clears `menuNm`. The
`MC_LastModeSuccessful` load is the delay slot of the `attemptLoadGame` test.

`padInitPadIndexMap` follows. Six bytes from the unnamed text at 0x00484c38
are copied to `levelName`, then byte 80 becomes 0xFF. That text is `intro`
and its NUL. `frontEndStartInCredits` receives the low byte of the argument.
`frontendControllerConfigRunning`, `lineShift`, `expWarnAlpha`, `gamePause`,
and `gameNoDraw` become 0. `frontEndRunning`, `floorDrawEnable`, and
`objectDrawEnable` become 1. `frontEndSuspend` is cleared before
`engineAddTask(runFrontEnd, 25, frontend, 0)`.

A nonzero `frontEndStartInCredits` then sets `creditsPosY` to 448, `menu` to
`creditMenu`, and `menuNm` to 60. A zero byte sets `engineHoldMpeg` to 1 and
`emergencyMusicWaitAbort` to 0. `lumpLoad` then receives the unnamed `fx.lmp`
at 0x00484c20 and the unnamed `frontend.lmp` at 0x0046bf10. The task text at
0x0046c2c0 is `frontend`. These four addresses have no ELF symbol. They are
not the `fx.lmp` object `main` already imports.

What `MC_GetReadyForMainMenu`, `menuSetPos`, `runFrontEnd`, and `lumpLoad` do
after entry is unknown. The `Menu` object layout is unknown.

Host check:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/frontend-init-check \
  experimental/sources/engine-restore-front-end/host_contract.cpp
/tmp/frontend-init-check
```

### Build-plan entry

Append after the current last input. Profile `authored_cpp`. Header closure
empty. Object name `front_end_init.o`. Source
`sources/engine-restore-front-end/front_end_init.cpp`. SHA-256
`a2789c95500901d74970ec232974145da660201a54b3cb10acb8a77474c1fcab`.
Do not plan `host_contract.cpp`.
`snapshot.json` must record both files before `tools/check_source_snapshot.py`
will accept the tree. This note does not edit that snapshot.

## engineClear

Observed `engineClear__Fv` at 0x001911c8 is 48 bytes and returns. One caller
reaches it: `launchSRS`. Fresh compilation is 44 bytes and is not an
original-byte match.

It disables cause 2, removes the handler id currently in `vblankHID` for cause
2, then calls `drawRemoveInterrupts`. The handler word is loaded after
`DisableIntc` returns. `vblankHID` is the local four-byte symbol at
0x004843ec.

Host check:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/engine-clear-check \
  experimental/sources/engine-restore-engine-clear/host_contract.cpp
/tmp/engine-clear-check
```

### Build-plan entry

Profile `authored_cpp`. Header closure empty. Object name `engine_clear.o`.
Source `sources/engine-restore-engine-clear/engine_clear.cpp`. SHA-256
`aab20367270eaf329ebf56bddf583365d8d84636522a4fb3d0e439283cc43e41`.
Do not plan `host_contract.cpp`.

## engineRestore

Observed `engineRestore__Fv` at 0x001901e0 is 204 bytes and returns. One caller
reaches it: `launchSRS`. Fresh compilation is 192 bytes and is not an
original-byte match. The reviewed compiler emitted `sra`, `sh`, and `sync`.

`AddIntcHandler(2, vblankHandler, 0)` stores its result in `vblankHID` before
`EnableIntc(2)`. `GetThreadId` and `ChangeThreadPriority` are already defined;
this file calls them and does not redefine them. The priority is 10.
`drawRestoreInterrupts` follows.

Three words are then stored: 130 at 0x10000010, 0 at 0x10000000, and 159 at
0x10000810. `lumpFindResource` is called with the unnamed text `engine.lmp`
at 0x00464238 and `shadowblob.tex` at 0x00464248. Those addresses have no ELF
symbol and are not the `engine.lmp` object `main` already imports.

The word at offset 16 of that result is combined with 0x20000000 and the
halfword at offset 20 of that address receives bits 8..23 of
`engineShadowTexAddress`. The instruction is `sra` by 8; the filled bits are
above the stored halfword. `sync` runs, then `sceDmaSendN` receives `dmaGif`,
the unmodified word from offset 16, and the unsigned halfword at offset 6.
`initSupersample`, `initVIF`, and `padInit` follow, in that order.

Calling 0x20000000 the uncached segment is inferred from the public EE map.
What the three absolute stores program, and what the resource record means,
is unknown.

Host check:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/engine-restore-check \
  experimental/sources/engine-restore-engine-restore/host_contract.cpp
/tmp/engine-restore-check
```

The EE profile does not define `NORRATH_HOST_CONTRACT`. That macro is how the
host contract observes the absolute stores.

### Build-plan entry

Profile `authored_cpp`. Header closure empty. Object name `engine_restore.o`.
Source `sources/engine-restore-engine-restore/engine_restore.cpp`. SHA-256
`158155411558a2cfcf35300e58eeb384aaf0ae86f4b24fe2d979c5054537cdc4`.
Do not plan `host_contract.cpp`.

## srs_main

Observed `srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i` at 0x0116ca68 is 1172 bytes
and returns an int. It is the `.srs` symbol `launchSRS` calls. Fresh
compilation is 1200 bytes and is not an original-byte match.

The ninth argument is the word `launchSRS` stores at 0(sp). This function
does not read it. The jump table at 0x011ddd10 selects on `Setup`'s return
plus one, for values whose unsigned sum is below 7:

| Setup result | Continuation |
| --- | --- |
| 0, 1, 3, and any result outside 0..5 | `Station` |
| 2 | language exec, then `Station` if `LoadExecPS2` returns |
| 4, 5 | tear down and return 0 |

`Station` returns 1 to make this function return 0. Any other `Station`
result returns 100. `exit(-1)` is called when `Init` is negative, and the
instructions after that call still execute if `exit` returns.

`g_mode` is initially 3 and is copied into the flag word after `Station`.
`g_numClients` is copied into the count word and bounds the copy loop.
`g_myIndex` is copied into the zero word. `first.3069` is initially 1. While
it is nonzero the function calls `sceDmaReset(1)`, sets bit 1 of the word at
0x1000e020, and calls the two SIF init routines, then clears the flag. The
copy loop writes `g_ips` and `g_localClientIps` in 24-byte steps and the port
words one index at a time.

The text at the printf and memcard sites has no ELF symbol. `g_mode`,
`g_ips`, `g_ports`, `g_localClientIps`, `g_localClientPorts`, `g_numClients`,
`g_localIp`, `g_myIndex`, `first.3069`, `s_argv`, and `s_buf` are defined
here with the observed sizes and initial bytes. What those VI calls do after
entry is unknown. The `VILauncher` and `VINetworkStatus` object sizes are
unknown; the source only passes their addresses.

Host check:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/srs-main-check \
  experimental/sources/engine-restore-srs-main/host_contract.cpp
/tmp/srs-main-check
```

`NORRATH_HOST_CONTRACT` is not defined by the EE profile.

### Build-plan entry

Profile `authored_cpp`. Header closure empty. Object name `srs_main.o`.
Source `sources/engine-restore-srs-main/srs_main.cpp`. SHA-256
`415c71f5721962b7a7f9ec17e99fc4f5335d62ffab51bbbb63a39c4784669d2d`.
Do not plan `host_contract.cpp`.

## Imports closed

`frontEndInit__Fi`, `engineClear__Fv`, `engineRestore__Fv`,
`srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i`.

## Imports opened

From `frontEndInit`: `MC_GetReadyForMainMenu__Fv`, `MC_LoadGameBrowser__Fii`,
`menuSetPos__FP4Menui`, `padInitPadIndexMap__Fv`, `runFrontEnd__Fv`,
`frontEndSuspend`, `hudNoDraw`, `nmPlayers`, `musicPlaying`, `attemptLoadGame`,
`MC_LastModeSuccessful`, `menuNm`, `menu`, `frontMenu`, `creditMenu`,
`autodemoTimer`, `frontEndStartInCredits`, `frontendControllerConfigRunning`,
`frontEndRunning`, `lineShift`, `expWarnAlpha`, `gamePause`, `gameNoDraw`,
`floorDrawEnable`, `objectDrawEnable`, `creditsPosY`, `engineHoldMpeg`,
`emergencyMusicWaitAbort`.

`engineAddTask__FPFv_viPci`, `lumpLoad__FPc`, `netNetworkMode`, and `levelName`
were already unresolved.

From `engineClear`: `DisableIntc`, `RemoveIntcHandler`,
`drawRemoveInterrupts__Fv`, `vblankHID`.

From `engineRestore`: `AddIntcHandler`, `drawRestoreInterrupts__Fv`,
`sceDmaSendN`, `initSupersample__Fv`, `initVIF__Fv`, `padInit__Fv`,
`vblankHandler__Fi`, `engineShadowTexAddress`, `dmaGif`. `EnableIntc` was
already unresolved. `GetThreadId` and `ChangeThreadPriority` are already
defined.

From `srs_main`: `__10VILauncher`, `__15VINetworkStatus`, `_$_10VILauncher`,
`_$_15VINetworkStatus`, `CheckNetwork__15VINetworkStatus`,
`ClearVRAM__FUcUcUcUc`, `Close__7VITimer`, `exitEzNetCnf`, `exitEzNetCtl`,
`ezNetCtlDownInterface`, `Free__10VILauncher`, `GetDefault__10VILanguage`,
`Init__10VILauncherb`, `Init__7VITimer`, `Initialize__15VINetworkStatus`,
`LoadExecPS2`, `printf`, `sceDmaReset`, `sceGsResetPath`, `sceSifInitIopHeap`,
`sceSifInitRpc`, `SetDefault__10VILanguage12VILanguageID`, `Setup__10VILauncher`,
`sprintf`, `Station__10VILauncheriPCcN22i`, `strcpy`,
`VIDisplayLoadingScreen__Fb`, `VIMemCardSetProductId__FPCc`,
`VIMemCardSetProductTitle__FPCc`. `exit` and `memset` were already unresolved
or defined: `memset` is defined, `exit` was already unresolved.

## Build-plan entries needed

| Source | Object | SHA-256 |
| --- | --- | --- |
| `sources/engine-restore-front-end/front_end_init.cpp` | `front_end_init.o` | `a2789c95500901d74970ec232974145da660201a54b3cb10acb8a77474c1fcab` |
| `sources/engine-restore-engine-clear/engine_clear.cpp` | `engine_clear.o` | `aab20367270eaf329ebf56bddf583365d8d84636522a4fb3d0e439283cc43e41` |
| `sources/engine-restore-engine-restore/engine_restore.cpp` | `engine_restore.o` | `158155411558a2cfcf35300e58eeb384aaf0ae86f4b24fe2d979c5054537cdc4` |
| `sources/engine-restore-srs-main/srs_main.cpp` | `srs_main.o` | `415c71f5721962b7a7f9ec17e99fc4f5335d62ffab51bbbb63a39c4784669d2d` |

All four use `authored_cpp` and an empty header closure. Host contracts are
not inputs.

## Authored but never executed

`frontEndInit__Fi`, `engineClear__Fv`, `engineRestore__Fv`, and
`srs_main__FPCcPiT1PPcT1T1T3T1bT0T0i`. Host contracts ran on this machine.
The reviewed EE compiler produced relocatable objects. No EE, interrupt, DMA,
SIF, menu, or game path ran.

## Unknown

`Menu` layout and the effects of the functions these routines call.
`main` and `launchSRS` pass 0 to `frontEndInit`. Whether a suspended front end
is reachable from those callers is unknown. What `vblankHID` contains before
`engineClear` is unknown. The resource record passed to `sceDmaSendN`, and the
effect of the three absolute stores, are unknown. `gameLoop`, `srs_main`,
`pad`, `skipIntro`, and `engineDisplayDisable` are still open.
