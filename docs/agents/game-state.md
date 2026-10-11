# game-state

Package work is on `agents/game-state`, based on `origin/codex/norrath-first-reconstruction` at `26dc32c`. It covers pause, camera tasks, menu mode, and the `game` / `core` names opened by game-boot, except `coreClearColor`. `gameCam` is left to `origin/agents/natives-c`, which already defines that name.

The fresh partial link of the published plan, without these units, has 623 authored compilations, 531 function bodies, 779 unresolved imports, and 0 COMMON. Its `game` and `core` names are `gameBailFromGameLoop`, `gameBoot__Fv`, `gameCam`, `gameInit__Fv`, `gameLoop__Fv`, `gameSetDefaultGameConfig__Fi`, and `gameobjInit__Fv`. The 105 names opened by game-boot are not in that list because that branch is not merged. This package defines the assigned share of those 105, plus `gameBailFromGameLoop` and `gameSetDefaultGameConfig__Fi` from the fresh list. `gameBoot__Fv` and `gameInit__Fv` stay with game-boot. `gameobjInit__Fv` stays with gameobject-base. `gameLoop__Fv` is the 2812-byte loop `main` calls beside `gameInit`, and it is not defined here.

Each function below was compiled with the reviewed `authored_cpp` flags and has a host contract that fails when the observed stores or call order are wrong. The compiled bodies are not original-byte matches. No routine ran on the EE or as gameplay. Host checks are not plan inputs. `-G0` places the zero words in `.bss`.

## Functions

`gameIsPaused__Fv` at `0x001d0e70` (148 bytes, compiled 200). In network mode it returns the `gamePause` word. Otherwise a nonzero `cutMode` returns the `coreLoading` word, including zero. A signed `frameNm` below 60 returns 0. After that a nonzero `gamePause` or `coreLoading` returns 1. A zero `hudNoController` returns 0. A nonzero `DLG_Active__Fv` returns 0. A reloaded nonzero `cutMode` returns 0. Otherwise it returns 1.

`coreInterfaceFlare__Fi` at `0x002d0c90` (28 bytes, compiled 32). Stores the bits `0x40000000` (2.0f) at `coreInterfaceBrightness` plus the argument times 4. The only observed caller passes 0. There is no range check in the original body. This definition has four slots, so other indices are not given an object.

`gameMaybeSaveConfig__Fii` at `0x001cca60` (1192 bytes, compiled 2216). Returns the difference count. A nonzero second argument returns that count and does not save. A zero second argument with a nonzero count copies live scalars into `MC_GameConfig` and copies the config button words onto the live button arrays, then calls `MC_SaveConfig__Fv`. A zero count saves only when the first argument is nonzero. Float slots use `!=`, so NaN differs and `+0` equals `-0`. Button words are compared only while `netNetworkMode` is zero. Scalar words still compare in network mode.

`gameResetGameState__Fv` at `0x001cd1e0` (204 bytes, compiled 236). Copies the unnamed zero byte at `0x00484eb8` into `startEntranceName[0]`, then clears the level and cheat words and calls `MC_NewGame__Fi(0)`. `gameOver` is cleared before that call. The same unit defines that unnamed byte as `norrath_game_state_byte_00484eb8`. That label is not an ELF name. No direct store of the byte was found.

`gameClear__Fi` at `0x001cb5f8` (552 bytes, compiled 584). A nonzero argument, including a negative one, also calls `worldAllocateWaterPatches__Fi(0)` and `lumpClear__Fi(0)`. `gamePause` is cleared before `playerClearSpellEffect__Fv`. `world` is cleared before the integrity check at the 583 line. `coreClearColor` is stored as `0x80000000` and left undefined.

`jumpGateJumping__Fv` at `0x00284830` (8 bytes, compiled 12). Returns the local word `jumping` at `0x00486030`. That word stays an import.

`gameDrawWorld__Fv` at `0x001d0c50` (116 bytes, compiled 172). A nonzero `gameNoDraw` returns before any call. `drawAutoMap__Fv` runs only when the loading-screen byte, `hudNoDraw`, and `world` are clear. `worldDrawWorld` then sees `world` again, so the automap call can change it. `drawDome__Fv` checks the loading-screen byte after that draw.

`gameInitSkillRamps__Fv` at `0x001d0d20` (96 bytes, compiled 156). Walks 32-bit pointers at `skills+4`. Word 0 is never loaded. Each nonzero pointer calls the function word at object+84 with `object` plus the signed halfword at the pointer-at-offset-96 plus 80. `skills` stays an import.

`__6Camera` at `0x0026f948` (100 bytes, compiled 136). Writes halfwords 100, 4000, 6000, 8000, 9000, and 10000 at offsets 126 through 136, calls `SetMode__6Camerai(this, 1)`, then tail-calls `Reset__6Cameraii(this, 1, 1)`.

`gameCameraTask__Fv` at `0x001d0b88` (48 bytes, compiled 48). Returns when `gameCam` is null or `netNetworkMode` is nonzero. Otherwise it calls `Update__6Camera` with the pointer captured before those tests.

`gameSetDefaultGameConfig__Fi` at `0x001cc778` (420 bytes, compiled 1332). `MC_Init__Fi` calls it with -1. That argument stores the scalar defaults into the live words and `MC_GameConfig`, and `soundUpdateMusicVolume__Fv` runs after the music word is 0.5f and before `commRecord` receives the `.lit4` bits `0x3f4ccccc` at `0x00480660`. Any other argument skips the scalars. Button record N is written only for argument -1 or N. Signed `controlStyle` 1 writes 64, 32, 16 in the first three words. Value 0 writes 32, 16, 64. Any other byte, including `0xff`, writes no button words. The shared tail is 128, 8, 2, 1, 1024, 4.

## Data

These image bytes are zero. `exitToMenuMode` and `mainMenu` are local in the ELF. They are global here because the pause menu imports the names. `jumping` is also local and is not defined.

`mainMenu` at `0x0037f1a0` is 128 bytes: eight 16-byte records. The first word of each of the first seven records points at `menu_returntogame`, `menu_recall`, `menu_loadgame`, `menu_option`, `menu_changeplayers`, `menu_kickplayers`, and `menu_exittomenu`. The other words, and the eighth record, are zero. The EE object is 128 bytes.

`cutMode` and `coreLoading` are the ELF names. World-state already publishes the same bytes as `norrath_game_cut_mode_byte_view` and `norrath_game_core_loading_word_view`, and data-views publishes `norrath_actor_view_484318` for the `cutMode` address. Those are different symbol names. Selecting both this definition and those views gives two objects for one address.

| Name | Address | Size |
| --- | --- | --- |
| `gamePause` | `0x00483f14` | 4 |
| `gameControlPaused` | `0x00483f1c` | 4 |
| `gameBailFromGameLoop` | `0x00483f28` | 4 |
| `gameCombatCamera` | `0x00484f5c` | 4 |
| `gameMultiPlayerMarkersOn` | `0x00484f58` | 4 |
| `gameNetReplicateQuestItems` | `0x00484f94` | 4 |
| `gameReverseCameraYaw` | `0x00484f60` | 4 |
| `coreLoading` | `0x00486680` | 4 |
| `exitToMenuMode` | `0x004842ce` | 1 |
| `cutMode` | `0x00484318` | 1 |
| `mainMenu` | `0x0037f1a0` | 128 |

## Imports closed

`__6Camera`, `coreInterfaceFlare__Fi`, `coreLoading`, `cutMode`, `exitToMenuMode`, `gameBailFromGameLoop`, `gameCameraTask__Fv`, `gameClear__Fi`, `gameCombatCamera`, `gameControlPaused`, `gameDrawWorld__Fv`, `gameInitSkillRamps__Fv`, `gameIsPaused__Fv`, `gameMaybeSaveConfig__Fii`, `gameMultiPlayerMarkersOn`, `gameNetReplicateQuestItems`, `gamePause`, `gameResetGameState__Fv`, `gameReverseCameraYaw`, `gameSetDefaultGameConfig__Fi`, `jumpGateJumping__Fv`, `mainMenu`.

## Imports opened

These names are referenced by this package, are not defined here, and are absent from the 779 unresolved imports of the published plan at `26dc32c`. Symbols that plan already defines or already leaves unresolved are not listed. `gameCam` is one of those already-unresolved names and is not defined here.

`DLG_Active__Fv`, `DLG_Clear__Fv`, `DLG_ShowDialogText`, `MC_GameConfig`, `MC_SaveConfig__Fv`, `P_Clear__Fv`, `Reset__6Cameraii`, `SetMode__6Camerai`, `Update__6Camera`, `_listCheckAllIntegrity__FPCci`, `actionButton`, `attackButton`, `automapButton`, `blockButton`, `bossMeterAlpha`, `bossMeterCurrent`, `bossMeterMax`, `cdClear__Fv`, `chatOn`, `cheatOn`, `commEcho`, `commEnable`, `commInit__Fv`, `commPlayback`, `commRecord`, `commThresh`, `controlStyle`, `coreClearColor`, `coreInterfaceBrightness`, `coreLoadingScreenRunning`, `cutClear__Fv`, `damageDisplayOn`, `drawAutoMap__Fv`, `drawDome__Fv`, `drawListClear__Fv`, `energyPotionButton`, `engineBrightnessCorrection`, `floorClear__Fv`, `floorDrawEnable`, `fxClear__Fv`, `g_BloodLevel`, `gameAutoMapMode`, `gameCameraShake`, `gameDeathTimer`, `gameDeathTimerSingle`, `gameHidePlayers`, `gameLineClear__Fv`, `gameLineDebugDraw__Fv`, `gameNoDraw`, `gameOver`, `gamePlayerLightAttenuate`, `gameRecallInhibit`, `gameShowEnemiesInMap`, `healthPotionButton`, `helpMessageOn`, `hudClearInfoMessage__Fv`, `hudFinishGold__Fv`, `hudNoController`, `hudNoDraw`, `inventoryReset__Fv`, `jumpGateClear__Fv`, `jumping`, `lastNmLocalPlayersAlive`, `levelSeeds`, `lightClear__Fv`, `localNmPlayersAlive`, `memCheckHeapReal__FPCci`, `multiHelpMessageOn`, `objectClear__Fv`, `objectDrawEnable`, `padClear__Fv`, `padVibrationEnable`, `particlesPaused`, `playerClearSpellEffect__Fv`, `playerGlobalInit__Fv`, `recallOn`, `removeAllQuestsFromLog__Fv`, `scriptClearAllGameState__Fv`, `scriptClear__Fv`, `scriptSetGameState__FPci`, `scriptSuperClear__Fv`, `shopActiveFrame`, `skills`, `soundMusicVolume`, `soundSfxVolume`, `soundStereoEnable`, `soundUpdateMusicVolume__Fv`, `spellButton1`, `spellButton2`, `startEntranceName`, `subtitlesOn`, `textInit__Fv`, `textureClear__Fv`, `weaponToggleButton`, `world`, `worldAllocateWaterPatches__Fi`, `worldClear__Fv`, `worldDrawWorld__FP12_worldHeader`, `worldReset__FP12_worldHeader`, `worldSetFarClip__Ff`, `worldSetViewCenter__Fv`.

Already unresolved on that plan, and still referenced: `MC_NewGame__Fi`, `blockAllocInit__Fv`, `engineCurrentLevelName`, `frameNm`, `gameCam`, `lumpClear__Fi`. Already defined there: `_engineCheckStack__FPCci`, `levelName`, `memset`, `netNetworkMode`, `scePrintf`.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. `header_closure` is empty. `profile` is `authored_cpp`. Do not add any `host_check.cpp`. `snapshot.json` was left unchanged. The published driver rejects a plan that adds inputs without a matching snapshot successor, so these units were compiled and host-tested one by one rather than linked into a private successor plan. Body sizes are the reviewed EE g++ `authored_cpp` sizes.

```json
{"header_closure":[],"kind":"independently_authored_source","object_name":"is_paused.o","profile":"authored_cpp","sha256":"cd98ce9e5bdb0352cc0517fe424213a080c3cc89bc9252247fa04ae8a8f03c3b","source":"sources/game-state-is-paused/is_paused.cpp","emitted_functions":[{"name":"gameIsPaused__Fv","compiled_body_bytes":200}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"interface_flare.o","profile":"authored_cpp","sha256":"10c92badac59ee46d12a978be49463f80a14fd0f588734c504dd1f6c8551cb06","source":"sources/game-state-interface-flare/interface_flare.cpp","emitted_functions":[{"name":"coreInterfaceFlare__Fi","compiled_body_bytes":32}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"maybe_save.o","profile":"authored_cpp","sha256":"98797751d2c59da159ba223d1f65d9341ae9f3ecac6cca26ae014b065bfc8810","source":"sources/game-state-maybe-save-config/maybe_save.cpp","emitted_functions":[{"name":"gameMaybeSaveConfig__Fii","compiled_body_bytes":2216}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"reset.o","profile":"authored_cpp","sha256":"822af2f142bae43e2130b1718d0f35def774cd2dd1cb47cc1f716228d3bac579","source":"sources/game-state-reset/reset.cpp","emitted_functions":[{"name":"gameResetGameState__Fv","compiled_body_bytes":236}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"clear.o","profile":"authored_cpp","sha256":"3413e3a1b48328223df72ed3127a472a8212d79e9666b8521a568ae926996e95","source":"sources/game-state-clear/clear.cpp","emitted_functions":[{"name":"gameClear__Fi","compiled_body_bytes":584}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"jump_gate.o","profile":"authored_cpp","sha256":"c3584ced7775235a58c9fce8153ed11ad560d821fbb2fe9ba472aff8b6599fc0","source":"sources/game-state-jump-gate/jump_gate.cpp","emitted_functions":[{"name":"jumpGateJumping__Fv","compiled_body_bytes":12}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"draw_world.o","profile":"authored_cpp","sha256":"82d07cd6ef0767ecfc52542710ee29d1e25e4f7ea82aa99cbf799219c6f1fabb","source":"sources/game-state-draw-world/draw_world.cpp","emitted_functions":[{"name":"gameDrawWorld__Fv","compiled_body_bytes":172}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"skill_ramps.o","profile":"authored_cpp","sha256":"516a140a4b20d4d4cdfb58a9bf053802b327d593e0d47387f2f63f8f0a5eadf3","source":"sources/game-state-skill-ramps/skill_ramps.cpp","emitted_functions":[{"name":"gameInitSkillRamps__Fv","compiled_body_bytes":156}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"camera_ctor.o","profile":"authored_cpp","sha256":"d29e507645ab267eafedf0912bb51fb70a0c61854733db23e9b9f7b8e5377a07","source":"sources/game-state-camera-ctor/camera_ctor.cpp","emitted_functions":[{"name":"__6Camera","compiled_body_bytes":136}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"camera_task.o","profile":"authored_cpp","sha256":"a47e41ad208ee867274d744ce5cef353bd5e9e415b3d738bb562294d6adf0121","source":"sources/game-state-camera-task/camera_task.cpp","emitted_functions":[{"name":"gameCameraTask__Fv","compiled_body_bytes":48}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"default_config.o","profile":"authored_cpp","sha256":"2ed9fec83b9d45f994fe5b4e99f74375eecd8dc671ab97bcc3b002e17bf4703a","source":"sources/game-state-default-config/default_config.cpp","emitted_functions":[{"name":"gameSetDefaultGameConfig__Fi","compiled_body_bytes":1332}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"pause_word.o","profile":"authored_cpp","sha256":"60f6bd34c66aca050ed72b6f873254f6af3e2f4ceaa2e54b50804c48557b09b9","source":"sources/game-state-pause-word/pause_word.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"control_paused.o","profile":"authored_cpp","sha256":"d103c8475d68449ed3a1b090cef34b32d87619183fc9c1b940f5bac65c456f51","source":"sources/game-state-control-paused/control_paused.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"combat_camera.o","profile":"authored_cpp","sha256":"d809f0e63698b1a00dfdb9a402f60f26928300802e1abcbe573d89b7f0f2d3ba","source":"sources/game-state-combat-camera/combat_camera.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"multiplayer_markers.o","profile":"authored_cpp","sha256":"f5d9e390fbb9b9d0a9f5ef19ed25bffa527679fb81d74f4c4b2d54dddd62848a","source":"sources/game-state-multiplayer-markers/multiplayer_markers.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"replicate_quest_items.o","profile":"authored_cpp","sha256":"5fea5459d8664d449f91a8d12ca82ce4685712efb2efa16dea60b87106dffaa7","source":"sources/game-state-replicate-quest-items/replicate_quest_items.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"reverse_camera_yaw.o","profile":"authored_cpp","sha256":"2f391a6ad913d82e1e7bd50a75643cb08ce7b6fbb0eccd0283ef496f8b7f7bce","source":"sources/game-state-reverse-camera-yaw/reverse_camera_yaw.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"core_loading.o","profile":"authored_cpp","sha256":"41600616cb5ac32d0ba747b847c5c97eb2e3db10af233d05833f9a377b334fe0","source":"sources/game-state-core-loading/core_loading.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"exit_to_menu.o","profile":"authored_cpp","sha256":"ad10736aaa58f9ee242bb3416d672ec5e51dc460ecf2165b8c82ee7db7503d32","source":"sources/game-state-exit-to-menu/exit_to_menu.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"cut_mode.o","profile":"authored_cpp","sha256":"18f2647a78c735b21e19047fd3f52c242586ca5c1c2ebfdc976356fd7021e129","source":"sources/game-state-cut-mode/cut_mode.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"main_menu.o","profile":"authored_cpp","sha256":"a792c4b861df2b19a64f4e2517a29cfcd97fae4d5092ec3ef04b37a4010288dc","source":"sources/game-state-main-menu/main_menu.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"bail.o","profile":"authored_cpp","sha256":"06c57fb3be0a6c2e2e78b08886c30f7051beb61f520e963d7b6c48261b5ae9a3","source":"sources/game-state-bail/bail.cpp"}
```

## Authored but never executed

All eleven functions and the eleven data objects above. Host contracts ran on this Mac. The reviewed EE compiler produced one relocatable object per unit, each with one global function or one data symbol. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests. Nothing ran on the EE, under the original process, or as gameplay. `gameCameraTask__Fv` is registered by `gameInit` and is not called directly. `gameLoop__Fv` was not authored.

## Unknown

The Camera object size is unknown. `c.1089` has symbol size 0, and `__6Camera` writes through offset 136. Whether anything stores `0x00484eb8` through a pointer is unknown; no direct store was found. `exitToMenuMode`, `mainMenu`, and `jumping` are local bindings in the ELF, while the first two are global definitions here. Out-of-range `coreInterfaceFlare` indices are not represented. The `.lit4` word at `0x00480660` has no ELF symbol. Which duplicate global a final link binds, including `cutMode` beside `norrath_actor_view_484318` and `coreLoading` beside `norrath_game_core_loading_word_view`, is for the integrator. `gameCam` remains the natives-c definition. `gameLoop__Fv` remains unauthored.
