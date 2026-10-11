# Game boot

Package work is on `agents/game-boot`. It supplies the five calls `main` makes in this package, in that order: `DLG_Init__Fv`, `gameBoot__Fv`, `gameInit__Fv`, the registration of `debugCheckForPauseMenuStart__Fv`, and `simpleDrawLoop__FPFv_i`.

Host comparisons live beside the sources as `host_check.cpp`. They include host headers, and the pause-menu check defines `NORRATH_HOST_CONTRACT`, so they are not EE build inputs. Each implementation file compiles with the reviewed EE g++ `authored_cpp` profile. The camera guard `_$tmp_0.1090` is defined in `game_boot.cpp` as four BSS bytes. `c.1089` stays undefined because its size is unknown.

A private copy of the plan, kept outside git, appended these five units to the branch plan at `d5883aa` and compiled 349 authored inputs. That link reported 448 function bodies, 747 unresolved imports, and 0 COMMON. The same base without these units reported 443 function bodies and 647 unresolved imports. The five names below left that list, and 105 names appeared. The published tip is 60 commits ahead of this base, so the trial orders are not stable and the absolute unresolved count will not match a later tip. The link was not executed as gameplay, and it is not an original-byte match.

## Evidence

Observed, inferred, and unknown are separated in the source comments. Short form:

- `DLG_Init__Fv` at `0x001d37a0` (104 bytes) stores `lumpFindResource("dlgbox.lmp", "box.tex")` in `boxTex` before either task, registers `runDialog__Fv` at priority 110 with "Dialog Demon Run" and 0, registers `drawDialog__Fv` at priority 110 with "Dialog Demon Draw" and 1, then stores 0 in `curDlg`. The four strings are unnamed bytes at `0x0046cdd8`, `0x00485048`, `0x0046cde8`, and `0x0046ce00`.
- `gameBoot__Fv` at `0x001d0cc8` (84 bytes) calls `__6Camera` on `c.1089` only while `_$tmp_0.1090` at `0x00484eac` is zero, then sets that guard to 1. A nonzero guard, including 2, skips the constructor and leaves the guard unchanged. Both paths store `c.1089` in `gameCam`, call `gameInitSkillRamps__Fv`, and store 0 in `gameNetReplicateQuestItems`. `c.1089` is at `0x007d0988` with symbol size 0. `worldBoundingBox` is 384 bytes later. The constructor writes halfwords through offset 136. That gap is not a measured size.
- `gameInit__Fv` at `0x001ccf08` (724 bytes) registers `gameCameraTask__Fv` at 11 with "camera" and `gameDrawWorld__Fv` at 30 with "drawWorld". Signed `controlStyle` 1 writes one button set. Value 0 writes the other set and leaves `infoButton` and `infoButtonFontChar` unchanged. Any other value, including `0xff`, writes neither set. Four 36-byte records from `MC_GameConfig+52` copy nine words into the attack, spell, action, block, potion, automap, and weapon-toggle buttons. The remaining copies are raw four-byte loads, including the float slots. `soundUpdateMusicVolume__Fv` runs before `g_BloodLevel` is stored. `lumpLoad("globsnd.lmp")` follows that store, then `helpMessageInit__Fv`, `gameResetGameState__Fv`, `animSetSpecialSoundHandler__FPFiG6Point3_v(specialSoundHandler__FiG6Point3)`, and `worldSetLightHaloTexture__FP8_texture(haloTexture2)`.
- `main` registers `debugCheckForPauseMenuStart__Fv` (`0x0017f800`, 912 bytes) with `engineAddTask` at priority 110. It does not call it. The body returns while the signed display-list remainder is below 3000, `coreLoading` is nonzero, or `recallOn` is nonzero. Remainder is `GLISTSIZE` minus the arithmetic shift `(glistp - gstart) >> 3`. `gameIsPaused`, `cutMode`, `jumpGateJumping`, and `hudNetWaiting` short-circuit in that order. Offline, `gameCam+52` must be 1 and `DLG_Active__Fv` must return 0. A blocked path with `startMenuState == 0` returns. A zero word at the local player record `+21872` skips the `0x400000` test. The second `netGetLocalPlayerNm__Fi` result is the pointer used for that test. Opening stores `menuPlayerNm`, sets `startMenuState` before `msgHudSetPlayerBusy`, and sets `gamePause` and `particlesPaused` only while offline. `MC_Debug` with the base `pad+8` word equal to 2051 selects `superMainMenu`. `strcmp(levelName+81, "lava3")` selects `raceMainMenu`. Closing clears `startMenuState`, and while `debugPauseMenuOn` is set it also clears `exitToMenuMode`, the offline pause flags, the debug flag, the busy HUD from the sign-extended low byte, and the pause-menu task. Both close paths fade stream 0 when status bit `0x40` is set and call `gameMaybeSaveConfig(0, 0)`.
- `simpleDrawLoop__FPFv_i` at `0x002cfc68` (268 bytes) clears `frameNm`, calls `engineSetDisplayDisable(2)`, and stores 1 in `supersampleMode`. Each pass starts a frame, processes the pad, calls the supplied function when it is non-null, and processes the CD. The callback's result is the loop condition. While `gameControlPaused` is greater than zero, a nonzero `(pad+8 | pad+96)` masked with `0xff0fffff` keeps the loaded counter; otherwise that counter decreases by one. `frameNm` increases before `engineFrameEnd(0)` and `soundFrame`. A nonzero result clears the game, sets display disable to 1, resets streams 0, 1, and 2, and calls `lumpClear(1)`.
- `main` at `0x002d0aa8` calls `simpleDrawLoop` with `displaySOELogo__Fv` at `0x002cd7d8`. That call is unconditional and is `main`'s first call of this function. `displaySOELogo__Fv` draws its two textures and then returns 0 while `frameNm < 301` and the tested pad words are clear. The loop calls the callback before `engineFrameEnd`, and the first callback sees `frameNm == 0`. The first frame this loop ends, when `main` calls it, therefore follows one `displaySOELogo__Fv` call. `displaySOELogo__Fv` belongs to render-lights and is not defined here. `engineInit` runs earlier and is outside this package. Whether that call presents a frame was not established. The second `simpleDrawLoop` argument is `displaySBLogoScreen__Fv`, gated by `skipIntro`. `gameInit__Fv` is called again from the main loop at `0x002d0bb0`.

## Imports closed

- `DLG_Init__Fv`
- `gameBoot__Fv`
- `gameInit__Fv`
- `debugCheckForPauseMenuStart__Fv`
- `simpleDrawLoop__FPFv_i`

## Imports opened

These 105 names were absent from the baseline unresolved list and present after the private link:

- `DLG_Active__Fv`
- `DLG_ShowDialogText`
- `GLISTSIZE`
- `MC_Busy__Fv`
- `MC_Debug`
- `MC_GameConfig`
- `__6Camera`
- `actionButton`
- `animSetSpecialSoundHandler__FPFiG6Point3_v`
- `attackButton`
- `automapButton`
- `blockButton`
- `boxTex`
- `c.1089`
- `cdProcess__Fv`
- `chatOn`
- `commEcho`
- `commEnable`
- `commPlayback`
- `commRecord`
- `commThresh`
- `controlStyle`
- `coreClearColor`
- `coreInterfaceFlare__Fi`
- `coreLoading`
- `curDlg`
- `cutMode`
- `damageDisplayOn`
- `debugPauseMenuOn`
- `drawDialog__Fv`
- `dropButton`
- `dropButtonFontChar`
- `energyPotionButton`
- `engineBrightnessCorrection`
- `engineFrameEnd__Fb`
- `engineFrameStart__FUib`
- `engineRemoveTask__FPFv_v`
- `engineSetDisplayDisable__Fi`
- `equipButton`
- `equipButtonFontChar`
- `exitToMenuMode`
- `frameNm`
- `g_BloodLevel`
- `gameCam`
- `gameCameraTask__Fv`
- `gameClear__Fi`
- `gameCombatCamera`
- `gameControlPaused`
- `gameDrawWorld__Fv`
- `gameInitSkillRamps__Fv`
- `gameIsPaused__Fv`
- `gameMaybeSaveConfig__Fii`
- `gameMultiPlayerMarkersOn`
- `gameNetReplicateQuestItems`
- `gamePause`
- `gameResetGameState__Fv`
- `gameReverseCameraYaw`
- `glistp`
- `gstart`
- `healthPotionButton`
- `helpMessageInit__Fv`
- `hudNetWaiting`
- `infoButton`
- `infoButtonFontChar`
- `jumpGateJumping__Fv`
- `mainMenu`
- `menu`
- `menuNm`
- `menuPlayerNm`
- `menuSetPos__FP4Menui`
- `msgHudSetPlayerBusy__Fcc`
- `netGetLocalPlayerNm__Fi`
- `netPlayerIsLocal__Fi`
- `nmPlayers`
- `padAcceptButton`
- `padAcceptButtonFontChar`
- `padBackButton`
- `padBackButtonFontChar`
- `padProcess__Fv`
- `padVibrationEnable`
- `particlesPaused`
- `pauseMenuFrameRun__Fv`
- `player`
- `raceMainMenu`
- `recallOn`
- `runDialog__Fv`
- `soundFadeStream__Fi`
- `soundFrame__Fv`
- `soundGetStreamPlayStatus__Fi`
- `soundMusicVolume`
- `soundResetStream__Fi`
- `soundSfxVolume`
- `soundStereoEnable`
- `soundUpdateMusicVolume__Fv`
- `specialSoundHandler__FiG6Point3`
- `spellButton1`
- `spellButton2`
- `startMenuState`
- `subtitlesOn`
- `superMainMenu`
- `supersampleMode`
- `useRaceMenu`
- `waitNetExitCount`
- `weaponToggleButton`
- `worldSetLightHaloTexture__FP8_texture`

These references were already unresolved on this base, so they are not opened here: `engineAddTask__FPFv_viPci`, `lumpFindResource__FPcT0`, `lumpLoad__FPc`, `lumpClear__Fi`, `strcmp`, `pad`, `levelName`, `haloTexture2`, and `netNetworkMode`.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. The private trial used orders 346 through 350 against revision `d5883aa`, whose plan had 344 authored inputs and 2 optional external inputs. The published tip already moved, so those trial orders are not stable. `header_closure` is empty for every unit. `profile` is `authored_cpp`. Do not add any `host_check.cpp` file. `snapshot.json` was left unchanged; the integrator records the file hashes. Body sizes below are the reviewed EE g++ `authored_cpp` symbol sizes, which differ from the original extents of 104, 84, 724, 912, and 268.

```json
{"source":"sources/game-boot-dlg-init/dlg_init.cpp","object_name":"dlg_init.o","sha256":"e0c37e5c4b9521e4fa8361f7878a65998cb98dcca42faf6fabedb4c793578153","emitted_functions":[{"name":"DLG_Init__Fv","compiled_body_bytes":112}]}
{"source":"sources/game-boot-game-boot/game_boot.cpp","object_name":"game_boot.o","sha256":"6de79f5720a339e640d2ad0e086d06b16233bdd786521bf28fc751b0d669a368","emitted_functions":[{"name":"gameBoot__Fv","compiled_body_bytes":136}]}
{"source":"sources/game-boot-game-init/game_init.cpp","object_name":"game_init.o","sha256":"e6b668524220ea55600b8caa6fec1c11e0cb56b5de98ca9a090356b616dcc387","emitted_functions":[{"name":"gameInit__Fv","compiled_body_bytes":888}]}
{"source":"sources/game-boot-pause-menu/debug_check.cpp","object_name":"debug_check.o","sha256":"dbef5b51d52b65d68168d3b9e018e2692e34131df23549f5c010cfea15bb4d35","emitted_functions":[{"name":"debugCheckForPauseMenuStart__Fv","compiled_body_bytes":1184}]}
{"source":"sources/game-boot-draw-loop/simple_draw.cpp","object_name":"simple_draw.o","sha256":"8c4f99c1c4cae0065450faf3198f6d581b3de63012a95df2ec0cf9723ae30833","emitted_functions":[{"name":"simpleDrawLoop__FPFv_i","compiled_body_bytes":464}]}
```

## Authored but never executed

All five functions. Host comparisons ran on this Mac, and the reviewed EE compiler produced relocatable objects. The private 349-input link checked symbol closure. No routine ran on the EE, under the original process, or as gameplay. `displaySOELogo__Fv` was not called. The first-frame conclusion is the call order in `main` and in `simpleDrawLoop__FPFv_i`.

## Unknown

- The size of `c.1089`. The 384-byte gap to `worldBoundingBox` and the constructor writes through offset 136 do not establish it.
- What `curDlg` means past the one word stored here. Its ELF symbol size is 0.
- Whether `engineInit`, which runs before this package, presents a frame.
- The original owner of every opened global. They stay unresolved imports.
- These definitions close the five `main` imports. They do not place the objects at the original addresses, and the compiled bodies are larger than the original extents.
- Exception flags, interrupts, and game runtime remain untested.
