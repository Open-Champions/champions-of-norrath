# Front end, menus and option screens (1/2)

Package `w8-frontend-menu-a`, branch `agents/w8-frontend-menu-a`. Addresses are
from the pinned SLUS-20565 image read with the EE binutils; nothing from it is
committed. Each function was compiled alone with the reviewed EE `g++` and the
`authored_cpp` flags, and has a host contract that replaces every callee with a
recording mock. No full partial build was run (see Unknown), and no routine has
run on the EE.

## Functions (one commit each)

| Symbol | Address, size | What was observed |
|---|---|---|
| `Username__C13VIGameOptions` | 0x01019ac8, 8 | returns object + 16 |
| `Password__C13VIGameOptions` | 0x01019b00, 8 | returns object + 80 |
| `frontEndCleanUp__Fi` | 0x001ca8d0, 76 | stops the cut, removes the `runFrontEnd__Fv` task, camera mode 1, clears four flags, stores the argument byte in `frontEndSuspend` |
| `debugClearStartMenu__Fv` | 0x0017ff18, 92 | leaves the debug pause menu: clears state, un-busies the local player, removes the `pauseMenuFrameRun__Fv` task |
| `colorLevelNames__FP4Menu` | 0x0017ff78, 156 | writes 7, 6 or 0 at offset 4 of each 16-byte record by comparing act numbers against the string at `levelName+81` |
| `hudRunNoControllers__Fi` | 0x001c3838, 456 | the "controller out" message box |
| `_GLOBAL_$I$optionMenu`, `_GLOBAL_$D$optionMenu` | 0x001caa00 / 0x001caa20, 32 each | call the local static-init helper at 0x001ca7c8 (construct branch, then destruct branch; two commits) |
| `debugRecall__Fi` | 0x0017ddb0, 804 | recall-potion/hub recall for one player |
| `hudRunYesNo__FPUsiiii` | 0x001c32f0, 1348 | the yes/no box; returns 0, 1 or 2 |

Data: `option_flags` (`gameControlDontResetPause`, `cheatOn`, `helpMessageOn`,
`language`, `controlPaused`, `hudNoController`), `menu_state_words` (`body`,
`cursorTween`, `fullLaunchOK`, `kickMenuList`, `letterAtATime`), `menu_tables`
(`gameTypeMenu`, `difficultyMenu`, `kickMenu`) and `yes_no_state` (`yesNoPos`,
`activeDelay`). Widths come from load/store opcodes; initial values from the
image bytes. Under `-G0` placement follows the build profile, not the
original `.sdata`/`.sbss` split.

## Observed, inferred, unknown

Observed: all addresses, sizes, call sequences, constants and string keys
above. The 1.4 scale in `hudRunYesNo` is the float stored twice at
0x00480614 and 0x00480618. `debugRecall` reads `player+36` (not the indexed
record) for the "None" test.

Inferred: record layouts that are only visible through offsets (`pad` is
88-byte elements, four of them; its word at offset 12 is the button mask in
`hudRunYesNo` and the word at offset 4 is tested for zero in
`hudRunNoControllers`; the 72-byte inventory entries in `debugRecall`; the
text buffer of 400 UTF-16 units). The three-element 40-byte array in the
option-menu static init is `AnimationState` objects only because the
destructor callee is `_$_14AnimationState`.

Unknown: the meaning of `pad` offset 4, `kickMenu[0]`'s second word (5),
`language` values, `body`, `cursorTween` units, what `player+36` holds, the
original source name of the local function at 0x001ca7c8 (a placeholder name
is used because GCC reserves the original), the true size of `kickMenuList`
(8 is the distance to its neighbour) and the border/print numeric arguments.

## Imports closed (24 of the 25 named in the package)

`Password__C13VIGameOptions`, `Username__C13VIGameOptions`,
`_GLOBAL_$D$optionMenu`, `_GLOBAL_$I$optionMenu`, `body`, `cheatOn`,
`colorLevelNames__FP4Menu`, `controlPaused`, `cursorTween`,
`debugClearStartMenu__Fv`, `debugRecall__Fi`, `difficultyMenu`,
`frontEndCleanUp__Fi`, `fullLaunchOK`, `gameControlDontResetPause`,
`gameTypeMenu`, `helpMessageOn`, `hudNoController`,
`hudRunNoControllers__Fi`, `hudRunYesNo__FPUsiiii`, `kickMenu`,
`kickMenuList`, `language`, `letterAtATime`. Not closed: `creditsText`.

`creditsText` (0x00428a20, 2408 bytes) is 301 records of 8 bytes, an int then a
string pointer, whose strings are the credits names and titles. That is game
text, so it is not reproduced here. It stays an unresolved import.

## Imports opened

Only symbols that these sources call or read and that belong to other packages;
none was checked against a fresh full build, so some may already be defined:
`cutStop__Fv`, `runFrontEnd__Fv`, `engineRemoveTask__FPFv_v`,
`SetMode__6Camerai`, `gameCam`, `frontEndRunning`, `frontEndSuspend`,
`hudNoDraw`, `attemptLoadGame`, `netGetLocalPlayerNm__Fi`,
`msgHudSetPlayerBusy__Fcc`, `pauseMenuFrameRun__Fv`, `debugPauseMenuOn`,
`netNetworkMode`, `startMenuState` (the object at 0x004841c8; a second local of
the same name is at 0x00484bdc), `exitToMenuMode`, `gamePause`,
`particlesPaused`, `MC_NameToAct__FPc`, `levelName`, `powerOffMode`,
`playerSelectCount`, `mediumFont`, `pad`, `player`, `textGetString__FPc`,
`usprintf__FPUsT0ffffffff`, `lumpFindResource__FPcT0`, `fontStart__FP4FontiUii`,
`fontPrintWrapCenteredu__FiiPUsiiii`, `fontEnd__Fv`,
`textureDrawBorder__FiiiiiUiPcii`, `strcmp`, `_$_14AnimationState`,
`gameCheckNoSaveZone__Fv`, `hudSetInfoMessage__FPUsiic`,
`removeItemFromInventory__Fiiii`, `blockAlloc__Fi`, `__12RecallEffectG6Point3`,
`soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`soundGetStreamPlayStatus__Fi`, `soundFadeStream__Fi`, `nmPlayersAlive`,
`playersAlive`, `recallOn`, `gameControlPaused`, `colorScale__FUif`,
`fontColor__FUiii`, `fontPrintCenteredu__FiiPUsi`,
`textureDrawColor__FP8_textureiiiUi`, `coreInterfaceBrightness`,
`padAcceptButton`, `nmPlayers`, `netPlayerIsLocal__Fi`,
`coreInterfaceFlare__Fi`.

`hudNoController` is also viewed by `save-net-anim-views`
(`norrath_game_hud_no_controller_byte_view`); if the integrator keeps that
view, one of the two definitions must go. `yesNoPos` and `activeDelay` were
local objects in the original unit; they are globals here.

## Build-plan entries needed

Profile `authored_cpp`, empty `header_closure`, all under `sources/`, in this
order. Each file must also be recorded in `snapshot.json`. Host checks are not
planned.

| Object | Source | SHA-256 |
|---|---|---|
| `w8fma_username.o` | `w8-frontend-menu-a-game-options-username/username.cpp` | `89747cc2432aa91e5ec8778f67eed65c76d9f617eeba5bde56a4cea8c95d1223` |
| `w8fma_password.o` | `w8-frontend-menu-a-game-options-password/password.cpp` | `e984a602e121b85e09a035aa9918bebecdd761d80945d3a2c5dbd4cdf0af1eb9` |
| `w8fma_clean_up.o` | `w8-frontend-menu-a-front-end-clean-up/clean_up.cpp` | `dc105faf71576c8abe24603fedc014cbf517b065c6ac35081f2efc558cc7191c` |
| `w8fma_clear_start_menu.o` | `w8-frontend-menu-a-debug-clear-start-menu/clear_start_menu.cpp` | `82567785b6b6b3bf812c64986c02fc9bab4472ee7e52993ca70f0ff6153b7086` |
| `w8fma_color_level_names.o` | `w8-frontend-menu-a-color-level-names/color_level_names.cpp` | `5cc03883563807f991404c28de5003f66852c62df1aa9ecc51a46593eb368f1f` |
| `w8fma_no_controllers.o` | `w8-frontend-menu-a-hud-run-no-controllers/no_controllers.cpp` | `91a9a68869f1836c2b4b402134509de6d943f2fae99887aea75af40c16d1f6f9` |
| `w8fma_static_init.o` | `w8-frontend-menu-a-option-menu-static/static_init.cpp` | `b083271ef926d219f4f40ccf7e24d67b17a11b573441ff117e2de92d6c0622b8` |
| `w8fma_option_flags.o` | `w8-frontend-menu-a-option-flags/option_flags.cpp` | `6c8641f2924b7bc01822eeebf58ae26c2be3de7c822409c549886f41b3ce6723` |
| `w8fma_menu_state_words.o` | `w8-frontend-menu-a-menu-state-words/menu_state_words.cpp` | `799df5300c81cb361955b97db360f966bf3a78c79441bd6df0e019eba083a495` |
| `w8fma_menu_tables.o` | `w8-frontend-menu-a-menu-tables/menu_tables.cpp` | `ca082e84918793308dad6dfffd464faf8a24b74cd61308207fd7e0cc550885d0` |
| `w8fma_debug_recall.o` | `w8-frontend-menu-a-debug-recall/debug_recall.cpp` | `6eda19225df3bc02fcd01df99443fbdc5cd27432d8b06d2976173a0c1dba3e21` |
| `w8fma_yes_no.o` | `w8-frontend-menu-a-hud-run-yes-no/yes_no.cpp` | `d50837b02de681a2d140e856974f582a3fd4bc5e511619118b5adf056bd926a0` |
| `w8fma_yes_no_state.o` | `w8-frontend-menu-a-yes-no-state/yes_no_state.cpp` | `f954997097d441e54bf6569bb2617904068a024724b8816b383b1c9e586f4055` |

Compiled sizes under the profile: `hudRunNoControllers__Fi` 0x1fc,
`hudRunYesNo__FPUsiiii` 0x574, `debugRecall__Fi` 0x43c,
`frontEndCleanUp__Fi` 0x60, `debugClearStartMenu__Fv` 0x78,
`colorLevelNames__FP4Menu` 0xa8, the two accessors 8 each. They differ from
the original sizes and none is claimed as a byte match.

## Host checks

Each directory has a `host_check.cpp`:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/check experimental/sources/<dir>/host_check.cpp && /tmp/check
```

`debugRecall` was mutation-checked by changing six constants or branches in
turn; each broke the check. `hudRunYesNo` the same for five changes; changing
the extra low bit of the 0x408000 mask is not caught because the check uses
exact button bits.

## Authored but never executed

Every function and data object above. Host checks ran on this Mac against
mocks; the EE compiler only produced relocatable objects. Nothing ran on the
EE, in the original process, or as gameplay. The static-init pair was checked
only on its array effects and the destructor call order.

## Unknown

- No full partial build was run, so the unresolved-import count after these
  units and any duplicate-definition clashes (see `hudNoController`) are not
  measured.
- `creditsText` is not authored (game text).
- The unrelated large neighbours (`pauseMenuFrameRun__Fv`, `runFrontEnd__Fv`
  and the player-select and option screens) were not in this package's list.
- Semantics listed under Unknown above.
