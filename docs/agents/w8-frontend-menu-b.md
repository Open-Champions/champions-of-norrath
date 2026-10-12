# Front end, menus and option screens (2/2)

Package notes for `w8-frontend-menu-b`. Addresses come from the pinned SLUS-20565
image, read with the EE binutils into scratch space outside git. No routine here
has run on an EE or in the game; nothing here is an original-byte match.
Every function compiled with the reviewed `authored_cpp` flags and has a host
check that fails when its branches, stores or call order change (several were
mutated to confirm). No game bytes, disassembly or decompiler output is
committed. `tools/build_partial.py` was not run; see the build section.

## Functions

| Symbol | Address | Original bytes | Fresh compile | Unit |
|---|---|---|---|---|
| `pushPause__Fv` | 0x0025b970 | 72 | 96 | `w8-frontend-menu-b-push-pause` |
| `stopCredits__Fv` | 0x001ca878 | 84 | 116 | `w8-frontend-menu-b-stop-credits` |
| `pauseProcess__Fv` | 0x0024b570 | 412 | 460 | `w8-frontend-menu-b-pause-process` |
| `menuRun__FP4MenuP3padPi` | 0x00192f38 | 1392 | 1276 | `w8-frontend-menu-b-menu-run` |
| `menuDraw__FiiP4MenuUiUiP4FontP8_texturei` | 0x001934a8 | 2432 | 2748 | `w8-frontend-menu-b-menu-draw` |

Host checks (not build inputs); build from the unit directory, for example:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/check host_check.cpp && /tmp/check
```

### Observed

- `pushPause__Fv`: returns when `netNetworkMode` is nonzero. Otherwise it saves
  the low byte of `gamePause` at `pauseStack[pauseStackPos]`, increments the
  byte `pauseStackPos`, and, if `frontEndRunning` is zero, sets
  `particlesPaused` and `gamePause` to 1.
- `stopCredits__Fv`: fades stream 0, resets the credits scroll words,
  starts `mx_mainmenu_l.vag` with mode 1, and points `menu` at `frontMenu` with
  `menuNm` 0.
- `pauseProcess__Fv`: follows `gameIsPaused`. On pause start it queues one batch
  command per held sound channel and sets flag bit 2; on resume it queues the
  saved volume word and clears the bit. The byte `paused` tracks which edge it is on.
- `menuRun`: menu records are 16 bytes (label word, kind word, value pointer,
  flags word), ended by a zero label. The terminator's word +4 holds the cursor
  (bits 0 to 6) and the previous cursor (shifted by 8). Kinds seen: 1 int
  counter, 2 float step, 3 on/off, 4 slider, 5 separator; flag bit 0 hides and
  skips an entry. Input masks and the two sounds are in the source header.
- `menuDraw`: draws each visible row, the kind 4 and kind 8 bars, a smooth plate
  eased between the previous and current row, and an outer border unless the
  front end is running. It returns first if less than 3000 display-list units
  remain. The `texture` argument is never read.
- `optionMenu` has thirteen records plus a terminator; the three network menus
  have two records plus a terminator each. Label words are text keys.

### Inferred

- Kind 8 is a threshold meter because `menuDraw` scales a marker by `commThresh`.
- `pauseStack` is a saved-pause-state stack (paired use with `popPause__Fi`).
- The bit-2 flag on a sound channel means "silenced by pause".

### Unknown

- What the label word means beyond selecting a text key; original field names.
- The command encoding on the IOP side for batch type 1 with flags 0x200.
- Why `menuRun` suppresses the cursor sound when `gameBailFromGameLoop` is 101.
- The identity of the words at 0x004844e8 (1.0f) and 0x00484514 (116) and whether
  they were function-local statics; they have no image symbol, so they are defined
  as `norrath_menu_local_t_004844e8` and `norrath_menu_word_00484514`. The global
  `t` at 0x00484c80 is a different object.
- What `optionsShowing`, `ranBrowser` and `ranConfig` record once set; the largest
  option count the dialog allows (`optionLinePos` has 12 slots only because
  `optTbl` follows it).

## Data

`w8-frontend-menu-b-menu-words`: the two local words above.
`w8-frontend-menu-b-net-menus`: `menuNetClientServer`, `menuNetImportOrNewPlayer`,
`menuNetImportOrNewGame` (48 bytes each, confirmed by a fresh compile).
`w8-frontend-menu-b-option-menu`: `optionMenu` (224 bytes, confirmed by a fresh
compile); its value pointers reference externs owned by other packages.
`w8-frontend-menu-b-dialog-state`: `showOptions`, `showMore`, `optionIndex`,
`scrollTarget`, `scrollY`, `scrollEnd`, `nameChars`, `optionCount`, `scrollSpan`
(float), `optionsShowing` and `optionLinePos` (48 bytes).
`w8-frontend-menu-b-misc-words`: `menuModeRebootQuestion` (byte), `title`
(pointer), `ranBrowser`, `ranConfig`. Section placement follows the build
profile (`-G0`), not the original `.sdata`/`.sbss` split.

## Build-plan entries needed

Append in this order, profile `authored_cpp`, empty header closure. Record each
file in `snapshot.json` too. Do not plan the `host_check.cpp` files.

| Source | SHA-256 |
|---|---|
| `sources/w8-frontend-menu-b-push-pause/push_pause.cpp` | `957e63e9aa0b08f69ba36d988230f1554fd7068fdbc73ba66180511ee4768749` |
| `sources/w8-frontend-menu-b-stop-credits/stop_credits.cpp` | `57d7547a3f6add64e572c31575e2bc2a33361cf7085353507261dd950e61751d` |
| `sources/w8-frontend-menu-b-pause-process/pause_process.cpp` | `a1723698d7913c2e46aed1445dc0bc4af7e55b1ffc5ae60c09d47d9fa359cfa6` |
| `sources/w8-frontend-menu-b-menu-words/menu_words.cpp` | `bce1c0114b60ce67cdc1ad339e684f4422ba76349013d0b6313a258109ab3488` |
| `sources/w8-frontend-menu-b-menu-run/menu_run.cpp` | `33d3cc451dff97834b0974b5666862fc74f5321b55a87583c33c6bb06f2e76c1` |
| `sources/w8-frontend-menu-b-menu-draw/menu_draw.cpp` | `1b8e520a732e178a48d37c269f1589b74be663204989c2fba50a5ab8b5145aa5` |
| `sources/w8-frontend-menu-b-net-menus/net_menus.cpp` | `b6b2a7873d09d7557edb4c1e443b5a66d1ed5fb72de6aec5aa99d66ae196a6ad` |
| `sources/w8-frontend-menu-b-option-menu/option_menu.cpp` | `52eaab1186ce263fe5d13b561d597e417401434c4af55f4f998fef0e3ab400cd` |
| `sources/w8-frontend-menu-b-dialog-state/dialog_state.cpp` | `179538a61b618febb99f412236c51264973523b7d70d6eb656b2458af1c2eb73` |
| `sources/w8-frontend-menu-b-misc-words/misc_words.cpp` | `65880cf2eaae722a753f1529bff9824e1cb73429ff4374abdf8ced834eaf732f` |

(Hashes are of the files at the last commit of this branch; recompute if edited.)

## Build check performed

Each of the ten sources was compiled alone with the plan's `authored_cpp` flags
and the reviewed toolchain, then all ten linked with `ld -m elf32lr5900 -r
--warn-common`: no duplicate definitions, no COMMON symbols, five function
definitions. The partial build driver and the unit test suite were not run (the
full partial build is out of scope while others build; this package edits no
shared plan, snapshot or tool).

## Summary

Imports closed (24): `menuDraw__FiiP4MenuUiUiP4FontP8_texturei`,
`menuModeRebootQuestion`, `menuNetClientServer`, `menuNetImportOrNewGame`,
`menuNetImportOrNewPlayer`, `menuRun__FP4MenuP3padPi`, `nameChars`,
`optionCount`, `optionIndex`, `optionLinePos`, `optionMenu`, `optionsShowing`,
`pauseProcess__Fv`, `pushPause__Fv`, `ranBrowser`, `ranConfig`, `scrollEnd`,
`scrollSpan`, `scrollTarget`, `scrollY`, `showMore`, `showOptions`,
`stopCredits__Fv`, `title`.

Imports opened (owned elsewhere or not yet checked against the current plan):
`pauseStack`, `paused`, `lerpColor__FUiUif`, `evalHermite__Ffffff`,
`colorScale__FUif`, `rectangleDraw__FiiiiiUi`,
`textureSmoothBorder__FiiiiiUiiii`, `textureDrawBorder__FiiiiiUiPcii`,
`fontStart__FP4FontiUii`, `fontEnd__Fv`, `fontColor__FUiii`,
`fontStringSizeu__FP4FontPUsi`, `fontPrintCenteredu__FiiPUsi`,
`ustrcpy__FPUsT0`, `ustrcpy__FPUsPc`, `ustrcat__FPUsT0`, `ustrcat__FPUsPc`,
`usprintf__FPUsT0ffffffff`, `textGetString__FPc`, `queryInventoryItem__FPci`,
`strstr`, `strcmp`, `lumpFindResource__FPcT0`,
`soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `soundFadeStream__Fi`,
`soundStartGlobalMusic__FPci`, `gameIsPaused__Fv`, `batchBuffer`,
`nmBatchCommands`, `norrath_sound_channel_records_view`,
`norrath_sound_channel_voice_map_view`, `gstart`, `glistp`, `GLISTSIZE`,
`coreInterfaceBrightness`, `commThresh`, `gameBailFromGameLoop`,
`padAcceptButton`, `pad`, `menu`, `menuNm`, `endTimer`, `endInit`, `creditsPosY`,
`lineShift`, `frontEndStartInCredits`, `frontEndRunning`, `frontMenu`,
`gamePause`, `particlesPaused`, `pauseStackPos`, `netNetworkMode`, and the
option value objects `engineBrightnessCorrection`, `soundMusicVolume`,
`soundSfxVolume`, `gameCombatCamera`, `gameReverseCameraYaw`,
`padVibrationEnable`, `soundStereoEnable`, `g_BloodLevel`, `subtitlesOn`,
`damageDisplayOn`, `chatOn`, `gameMultiPlayerMarkersOn`. Most are defined by
other packages; `pauseStack` (5 bytes at 0x00486cc0) and `paused` (byte at
0x00485964) had no definition that I found.

Authored but never executed: all five functions and all data, apart from the
host checks, which run only the isolated source with fakes.

Not done: no blocker needed the owner. The `_GLOBAL_$I$optionMenu` and
`_GLOBAL_$D$optionMenu` symbols are in the other front-end package's list; their
bodies call `__static_initialization_and_destruction_0` for an array of three
`AnimationState` records at 0x007d0910, not for the menus.
