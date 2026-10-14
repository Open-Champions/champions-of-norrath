# Front-end shell

Package notes for `runFrontEnd` and the front-end data words. Original names
and addresses were read from the pinned SLUS-20565 image with EE binutils.
No routine here has run on the EE or as gameplay. This package was resumed
after an earlier worker stopped; its uncommitted `runFrontEnd` source was
reviewed before it was kept (see Review).

## runFrontEnd

Observed `runFrontEnd__Fv` at 0x001c8098 is 5656 bytes and returns. One
caller registers it: `frontEndInit`. Fresh compilation is 7060 bytes and is
not an original-byte match.

It plays the title music once, draws the title plate and the button hint,
runs the controller-config screen, the reboot question, the network launch
poll and the save-config question, then draws and runs the current menu and
handles accept and back for each menu number. Menu numbers 0, 1, 2, 10, 11,
12, 15 to 18, 20, 21, 50, 51, 52, 55 and 60 are distinguished. Credits scroll
state uses `creditsPosY`, `endTimer`, `endInit` and `creditsText`.

Inferred: the 16-byte record stride of the menu objects, from the index
scaling; the meaning of menu numbers, from the labels compared on accept.
Unknown: the record fields after the label word, the `Menu` type, what the
unnamed byte at 0x00484ba8 means, and whether the two color constants are
anything but packed ARGB.

`menu` and `menuNm` here are the locals at 0x00484ba4 and 0x00484ba0.
`dialog-hud-controls` also defines `menu` and `menuNm`, so the link binds
this code to those objects. That aliasing is not resolved here.

### Review

The earlier source was kept after these checks: every string literal exists
in the image; the callee sets match the original except for call-site counts
that the compiler merges or duplicates; the color and float constants were
found in the image. One defect was fixed: the store after the `kelsel` copy
goes to 0x004389f8, which is `playerSelectPrevName[0]`, not a byte inside
`playerSelectNextName`. The host contract now covers it (codes 43 to 45).

### Host checks

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/frontend-run-check \
  experimental/sources/frontend-shell-run/host_contract.cpp
/tmp/frontend-run-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/frontend-words-check \
  experimental/sources/frontend-shell-words/host_check.cpp
/tmp/frontend-words-check
```

## Data words

`frontMenu` (80 bytes: four labelled records, one zero record), `creditMenu`
(16 zero bytes), `frontEndRunning`, `engineHoldMpeg`, `frontEndSuspend`,
`frontEndStartInCredits`, `creditsPosY` (448), `autodemoTimer`, `lineShift`,
`frontendControllerConfigRunning`, `attemptLoadGame`, `expWarnAlpha`,
`frontEndWasRunning`. Initial values are from the image. Section placement
follows the build profile (`-G0`), not the original `.sdata`/`.sbss` split.
`run_locals.cpp` defines `endTimer.1104` and a placeholder-named object for
the symbol-less byte at 0x00484ba8.

## Build-plan entries needed

Append in this order, profile `authored_cpp`, empty header closure. Both
files must also be recorded in `snapshot.json`. Do not plan the host files.

| Object | Source | SHA-256 |
|---|---|---|
| `front_end_words.o` | `sources/frontend-shell-words/front_end_words.cpp` | `9b4d9ff91e2afeba955256b53be42cae13814abb84632d406a572c449039a690` |
| `run_locals.o` | `sources/frontend-shell-run/run_locals.cpp` | `307fa4d9464111fb4d24b50bc28001d7fd50ea3e4f5c691bd4bcbbfbe405797b` |
| `run_front_end.o` | `sources/frontend-shell-run/run_front_end.cpp` | `32b88e1aafa0fb48365223291d9c632b8d7b29d71fa0b47181523d444fd22373` |

Emitted functions: `runFrontEnd__Fv` 7060 bytes (0x1b94); the other two
inputs emit none. Checked with a private plan and snapshot kept outside git.

## Summary

Imports closed (14): `attemptLoadGame`, `autodemoTimer`, `creditMenu`,
`creditsPosY`, `engineHoldMpeg`, `expWarnAlpha`, `frontEndRunning`,
`frontEndStartInCredits`, `frontEndSuspend`, `frontEndWasRunning`,
`frontMenu`, `frontendControllerConfigRunning`, `lineShift`,
`runFrontEnd__Fv`. Unresolved count went from 879 to 928 in the private
build.

Imports opened (63): `MC_ChangePlayers__FiPUsiii`, `commMenu`,
`coreClearNetLogFuck`, `creditsText`, `curFocus`, `difficultyMenu`,
`endInit`, `endTimer`, `engineReboot__Fv`, `fontColor__FUiii`, `fontEnd__Fv`,
`fontPrintCentered__FiiPci`, `fontPrintCenteredu__FiiPUsi`,
`fontStart__FP4FontiUii`, `fontStringSizeu__FP4FontPUsi`,
`frontEndCleanUp__Fi`, `frontendInitControllerConfig__Fi`,
`frontendRunControllerConfig__Fv`, `gameDeathStart__Fi`, `gameHidePlayers`,
`gamePopConfig__Fv`, `gamePushConfig__Fv`, `gameTypeMenu`, `hudFooterUni`,
`hudRunNoControllers__Fi`, `hudRunYesNo__FPUsiiii`, `hudShowMsg__FPUsN20iii`,
`isNetGame`, `launchPlayerSelect__Fv`,
`menuDraw__FiiP4MenuUiUiP4FontP8_texturei`, `menuModeRebootQuestion`,
`menuNetClientServer`, `menuNetImportOrNewGame`, `menuNetImportOrNewPlayer`,
`menuRun__FP4MenuP3padPi`, `musicStart`, `netAttemptLaunch`,
`netBroadCastStart`, `netDoNewLaunch__Fv`, `netInit__Fv`, `netStart__Fv`,
`optionMenu`, `paramRunning__Fv`, `playerMenu`, `playerSelectCount`,
`playerSelectEnforce`, `playerSelectExitMessage`, `playerSelectNextName`,
`playerSelectPrevName`, `playerSelectStartPlayer`, `playerSelectSuccessful`,
`preConfigure`, `rectangleDraw__FiiiiiUi`, `setSRSInfo__Fv`,
`soundClearStreamQue__Fi`, `soundGetStreamAmountPlayed__Fi`,
`soundStartGlobalMusic__FPci`, `soundUpdateAdjustments__Fv`,
`startNetLaunch__Fv`, `statusPtr`, `stopCredits__Fv`,
`textureSmoothBorder__FiiiiiUiiii`, `usprintf__FPUsT0ffffffff`.
Several are already named in other packages' scopes; the list is what the
link reports as undefined.

Authored but never executed: everything. Only host contracts with stubbed
callees have run (45 checked outcomes for `runFrontEnd`, one value check for
the words).

Unknown: the `Menu` object layout past the label word; the meaning of the
byte at 0x00484ba8; the `menu`/`menuNm` aliasing with `dialog-hud-controls`;
whether the 7060-byte compile reflects the original control flow beyond the
call sets and constants compared here (no instruction-level comparison was
done); the stack-argument words of the ten-argument
`textureSmoothBorder` call, which are passed as zero and not observed.

Not in scope and left as imports: the VI launcher classes, the memory-card
functions, `padInitPadIndexMap`, `gameLoop`, `gameNoDraw`, `hudNoDraw`,
`objectDrawEnable`.
