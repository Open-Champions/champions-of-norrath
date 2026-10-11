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

## Imports closed

`frontEndInit__Fi`, `engineClear__Fv`.

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

## Build-plan entries needed

| Source | Object | SHA-256 |
| --- | --- | --- |
| `sources/engine-restore-front-end/front_end_init.cpp` | `front_end_init.o` | `a2789c95500901d74970ec232974145da660201a54b3cb10acb8a77474c1fcab` |
| `sources/engine-restore-engine-clear/engine_clear.cpp` | `engine_clear.o` | `aab20367270eaf329ebf56bddf583365d8d84636522a4fb3d0e439283cc43e41` |

Both use `authored_cpp` and an empty header closure. Host contracts are not
inputs.

## Authored but never executed

`frontEndInit__Fi` and `engineClear__Fv`. Host contracts ran on this machine.
The reviewed EE compiler produced relocatable objects. No EE, interrupt, menu,
or game path ran.

## Unknown

`Menu` layout and the effects of the functions these two routines call.
`main` and `launchSRS` pass 0 to `frontEndInit`. Whether a suspended front end
is reachable from those callers is unknown. What `vblankHID` contains before
`engineClear` is unknown.
