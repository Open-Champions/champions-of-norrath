# memcard-calls

Package work is on `agents/memcard-calls`. The branch was merged with
twenty-five units and no summary note, so this file records the
integrator's selection. All twenty-five units were selected. Each unit
has a host contract beside its source that returns failure on wrong
behavior. Twenty-four passed on this machine with the harness's default
flags; the `MCA_Init` contract passes at `-O0` (at `-O1` this host's
compiler expands the constant-size `memset` inline and bypasses the
call-counting wrapper, a host-toolchain artifact, not a reconstruction
defect). No game binary or disassembly listing was committed.

## Observed

`MC_Busy__Fv` at `0x0025acd8`, 44 bytes. A nonzero `disableMC` returns
0; otherwise an `asyncMode` other than -1 returns 1; otherwise nonzero
`errorCondition` returns 1 and zero returns 0.

`MC_CheckPointAge` at `0x00485be4`: 4 `.sdata` bytes, initial 0.
`mcHeartBeatRun` adds one; `MC_SaveCheckPoint` stores 0.

`mcClientID` at `0x008e6b40`: local `.bss` symbol, size 0 in the ELF;
the next symbol `typeAddr` is 40 bytes later. `sceMcInit` reads and
writes the word at offset 36. The view is 40 zero bytes.

`MC_Debug` at `0x00485aac`: 1 `.sdata` byte, initial 0.

`MC_GameConfig` at `0x0042e300`: 252 zero bytes in `.data` (63 words).
`MC_SaveConfigSilent` ORs its first argument into the first word;
`gameInit` reads fixed offsets inside.

`MC_GetAsyncMode__Fv` at `0x0025ad08`, 8 bytes. Returns the `asyncMode`
word.

`mcHeartBeat__Fv` at `0x00251370`, 224 bytes. Returns without calls when
`disableMC` is nonzero and `asyncMode` is neither 0 nor 7; otherwise
calls `MCA_Process` with `errorCondition`.

`mcHeartBeatRun__Fv` at `0x002511f0`, 384 bytes. Advances `progress`
toward 402 with the two observed float steps while `asyncMode` is 3.

`MC_KickProtect` at `0x00485be2`: 1 `.sdata` byte, initial 0.

`MCA_LastDirCount` at `0x00485c0c`: 4 `.sdata` bytes, initial 0. It is
not the local `lastDirCount` word cleared by `MCA_Init`.

`MC_LastModeSuccessful` at `0x00485bdf`: 1 `.sdata` byte, initial 0.

`MC_LoadGameBrowser__Fii` at `0x0025ad38`, 108 bytes.

`MCA_ClearInfo__Fv` at `0x0025c688`, 92 bytes. Writes eight words;
`MCA_Present` receives 0 and `MCA_Formatted` receives -1.

`MCA_Cmd__Fiiii` at `0x0025c790`, 116 bytes. Writes the four arguments
into `commandQueue[queueEnd]` with wraparound at 256.

`MCA_Init__Fv` at `0x0025c638`, 80 bytes. Stores -1 in `clearBytes` and
0 in `queueEnd`, `queueStart`, `checkResults`, `lastDirCount`,
`MCA_Port`, `MCA_Slot`, and `MCA_FileOpen`, calls `MCA_ClearInfo`, then
clears 16384 bytes at `nullBuff`. The five queue words were local in the
image and are global here so `MCA_Cmd` can use the same words.

`MCA_Sync__FPi` at `0x0025c748`, 72 bytes. Calls `MCA_Process` with the
same pointer; nonzero pointee returns 1.

`MC_NewGame__Fi` at `0x0025ac68`, 80 bytes. Stores -1 in `delayCount`
and 0 in `errorCondition`, `actionCoolOff`, and `curPlaySeconds`, then
calls `resetQuickLoad`; a zero argument then calls `MC_SetNewAct(0)`.

`MCA_Port` at `0x00485c00`: 4 `.sdata` bytes, initial 0.

`MC_GetReadyForMainMenu__Fv` at `0x0025acb8`, 28 bytes. Calls
`resetQuickLoad` and returns.

`MC_SaveGameBrowser__Fi` at `0x002506b8`, 244 bytes. Scans `nmPlayers`
player records for qualifying words.

`MC_SaveCheckPoint__Fiii` at `0x00250d48`, 780 bytes. The third argument
is not read. Scans players like the browser; `curAct` of -1 skips naming.

`MC_SaveConfig__Fv` at `0x0025ada8`, 52 bytes. A nonzero `disableMC`
returns that word; otherwise clears `ranConfig` and `delayCount`, calls
`pushPause`, and returns `newMode(4, 0)`.

`MC_SaveConfigSilent__Fii` at `0x0025ade0`, 96 bytes. ORs the first
argument into `MC_GameConfig` before testing `disableMC`.

`MC_SavingOrLoading` at `0x00485bde`: 1 `.sdata` byte, initial 0.

`MCA_Slot` at `0x00485c04`: 4 `.sdata` bytes, initial 0.

## Inferred

The defined names are reconstructions of the observed originals. The
`nullBuff` extent (16384 bytes to `zeroTile`), the 63-word reading of
`MC_GameConfig`, and the global promotion of the five queue words are
inferences recorded in the sources. Consumers of several words remain
unknown.

## Imports closed

The twenty-five defined names, to the extent each was an unresolved
import on the integration tip.

## Imports opened

Whatever the units reference that the tip does not define; the measured
link delta decides the list.

## Build-plan entries needed

Twenty-five inputs with fresh `input_order` values, profile
`authored_cpp`, empty `header_closure`, and the measured
emitted-function lists. Host files stay out of the plan. Snapshot
`files` records every new source and host file.

## Authored but never executed

All twenty-five units above. Host checks ran on this machine (see the
`-O0` note for `MCA_Init`). The reviewed EE compiler produced
relocatable objects and the partial link succeeded. No routine has run
on the EE, under the original process, or as gameplay.

## Unknown

Card hardware behavior, `MCA_Process` internals, and the consumers of
several words remain unverified.
