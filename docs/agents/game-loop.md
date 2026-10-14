# Game loop

Package work is on `agents/game-loop`. It closes the 15 imports below. Original names, addresses and sizes
come from the pinned `SLUS_205.65` symbol table and the instruction bytes. Host checks sit beside each source as
`host_check.cpp`, define `NORRATH_HOST_CONTRACT` where they need it, and are not EE plan inputs. Every source
compiles with the reviewed `authored_cpp` profile. No compiled body is claimed to match the original bytes.

## Observed, per symbol

Each source file opens with its own observed / inferred / unknown comment. Short form:

- `gameLoop__Fv` at `0x002cf048` (2812 bytes). Setup, the per-pass level start and frame work, and the exit
  sequence are described in `game-loop-game-loop/game_loop.cpp`. `main` is the only caller. The authored body is
  3876 bytes because the level start is folded into it. The host check drives every branch with call mocks and
  compares whole traces for the no-level and offline-level paths.
- `drawObjectsTask__Fv` at `0x0013f930` (1856 bytes) walks the 120 by 120 object grid by frustum subdivision and
  draws each object through the slot at vtable+40; see `game-loop-draw-objects/draw_objects.cpp`.
- `runObjectsTask__Fv` at `0x00140070` (924 bytes), `transformSceneLightTask__Fv` at `0x0013c918` (292 bytes),
  `drawParticlesTask__Fv` at `0x00180018` (32 bytes), `gameCheckPlayerAlive__Fv` at `0x001d0fa8` (76 bytes),
  `gameSetCameraShake__Ffb` at `0x001d0a88` (212 bytes), `gameNetKick__Fi` at `0x001d0900` (224 bytes) and
  `frontEndStartInNetMenu__Fv` at `0x001ca9b8` (68 bytes) are each described in their source header.
- Data: `gameNoDraw` (`0x00483f18`, 4 bytes, 0), `gameOver` (`0x00484f99`, 1 byte, 0),
  `gamePlayerLightAttenuate` (`0x00484f24`, 4 bytes, 0), `gameRecallInhibit` (`0x00486694`, 4 bytes, 0),
  `hudNoDraw` (`0x00484af8`, 1 byte, 0) and `objectDrawEnable` (`0x00483ac8`, 1 byte, 1). The values are the
  file bytes.

## Inferred

- The two `engineAddTask` flag words in `gameLoop` and their priorities are copied from the instructions; what the
  scheduler does with them is the scheduler package's concern.
- The `0x8000` start clip word and the 840.0 depth in `drawObjectsTask` are copied as constants. Their meaning is
  not established.
- In `gameLoop`, the float at player record `+172` being above zero is treated as the alive test, the same way
  `gameCheckPlayerAlive` uses it. No field name is claimed.

## Unknown

- The byte `loadingScreenRequestExit` (`0x004866d4`) is read once before the loading-thread wait and not used.
  The source stores 1 and waits for `ReferThreadStatus(loadThreadID, 0)` to return 16.
- `bossMeterCurrent`, `bossMeterAlpha` and `world` are only written with zero here. Their types are not known.
- The Player layout beyond offsets 172 and 21872, and the layout behind `lumpFindByType` results beyond the word
  at +4, are unknown.
- What the strings `a\n`, `b\n`, `c\n`, `d\n` mark, and what the line numbers 1638, 1649, 1700, 1741 and 1792
  stand for beyond being `_engineCheckStack` arguments.
- The meaning of the `-128, -128, -128, 0` screen color arguments and of the `0x10000000` timer 0 count writes
  and reads, beyond their instruction forms.
- Whether the draw order of `drawObjectsTask` (the second pushed half first) matters to the original renderer.

## Verification run

- Each host check compiled with `c++ -std=c++17 -Wall` and ran with exit code 0.
- Mutation spot checks on `game_loop.cpp` (AI step clamp, armor-cache counter floor, draw-list skip threshold,
  decompress threshold) each fail a numbered host check.
- Each source compiled with the reviewed EE g++ and `authored_cpp` flags.
- A private copy of the plan and snapshot, kept outside git, appended these 15 units to the branch plan. That
  build compiled 860 inputs, linked once with 736 function bodies, 963 unresolved imports and 0 COMMON. The same
  base without them gave 723 function bodies and 879 unresolved imports. All 15 package names left the
  unresolved list and 99 names were added. This link did not execute gameplay and is not an original-byte match.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests ran, all OK.

## Imports closed

`drawObjectsTask__Fv`, `drawParticlesTask__Fv`, `frontEndStartInNetMenu__Fv`, `gameCheckPlayerAlive__Fv`,
`gameLoop__Fv`, `gameNetKick__Fi`, `gameNoDraw`, `gameOver`, `gamePlayerLightAttenuate`, `gameRecallInhibit`,
`gameSetCameraShake__Ffb`, `hudNoDraw`, `objectDrawEnable`, `runObjectsTask__Fv`, `transformSceneLightTask__Fv`.

## Imports opened

These 99 names were absent from the baseline unresolved list and present after the private link:

`Kick__7Vehicle`, `MC_FrontEndImport`, `MC_Kick__Fv`, `P_DrawParticles__FP8Matrix34`, `Reset__6Cameraii`,
`Update__6Camera`, `_$_16ListSafeIterator`, `__16ListSafeIteratorP8ListHeadi`, `alwaysDrawList`, `animFrame__Fv`,
`blockGetNmFree__Fv`, `bossMeterAlpha`, `bossMeterCurrent`, `charDirectDir`, `charDirectDir2`, `cheatOn`,
`commProcess__Fv`, `coreGameStep__Fii`, `coreHeadsetResetTimer`, `coreInterfaceBrightness`,
`coreInterfaceFadeUpVal`, `coreLevelCommentTimer`, `coreLoadingStart`, `coreSwitchScene`,
`coreSwitchSceneStart`, `cutRunFrame__Fb`, `debugClearStartMenu__Fv`, `deleteList`, `drawListClear__Fv`,
`engineFullFrameMode`, `engineGetAIStepCount__Fv`, `engineLastAITime`, `engineLoadEnvTex__FP8_texture`,
`engineSetBlur__Fi`, `enumerateAlivePlayers__Fv`, `gameCameraShake`, `gameCreatureThinkSpinner`,
`gameMiscTaskSync__Fv`, `gamePlayersPresent`, `gameSetObjectVis__Fv`, `hudKick__Fv`, `interfaceOn`,
`invShutDown__Fi`, `inventoryInit__Fv`, `isNetGame`, `lastNmBlocksFree`, `loadThreadID`,
`loadingScreenRequestExit`, `lumpCheck__Fv`, `lumpFindByType__FPvPci`, `lumpQuery__FPc`,
`matTransform34Vec__FP6Point3P8Matrix34T0`, `mcProcessKick__Fv`, `mediumRunList`, `menuNetClientServer`,
`modelClippingOn`, `musicTimer`, `netBroadCastStart`, `netClearContact__Fv`, `netGameStep__FbT0`,
`netInContact__Fv`, `netInit__Fv`, `netStartLevel__Fv`, `netStartNetworkGameFrame__Fv`,
`netStartNetworkGameInit__Fv`, `netStart__Fv`, `netTrouble`, `netWaitForPeersToCatchUp__Fv`,
`next__16ListSafeIterator`, `objectIgnoreFlags`, `padBigRumble__Fiff`, `picProcess__4itemv`,
`playerDeallocateArmorCaches__Fv`, `playerPlaySound__Fii`, `playerRecalled`, `plotRouteTask__Fv`,
`preCacheArmor__6Player`, `recallEnter`, `runList`, `runMusic__Fv`, `scriptRun__Fv`, `scriptSetScript__FPv`,
`shopKick__Fv`, `slowRunList`, `startNewWorld__Fv`, `subDrawLoop__FPFv_i`, `superGamestateInit`,
`texIsDecompressing__Fv`, `texIsLoading__Fv`, `texProcessDecompress__Fv`, `texProcessLoad__Fi`,
`textMessageDraw__Fv`, `textMessageProcess__Fv`, `world`, `worldCameraMatrix`,
`worldClipCubeToClipFrustum__FPA1_fi`, `worldDrawEnable`, `worldRunWaterTask__Fv`, `worldViewCenter`.

## Build-plan entries needed

Append in this order after the current last input (profile `authored_cpp`, header closure empty, kind
`independently_authored_source`). `snapshot.json` must record each source file and its
`emitted_functions` before the checker accepts the tree; this note does not edit the snapshot. Do not add any
`host_check.cpp` as a plan input.

| Source | Object name | SHA-256 |
| --- | --- | --- |
| `sources/game-loop-camera-shake/camera_shake.cpp` | `camera_shake.o` | `4aa0461b39d7d865d37070fb3426a43c5a378a5b12aa1c42b483d8f718bc2f52` |
| `sources/game-loop-check-player-alive/check_player_alive.cpp` | `check_player_alive.o` | `10943687073c56b7da1db73661cd9bb7517b9576ce59a2807ecd88e64f40fd7a` |
| `sources/game-loop-draw-objects/draw_objects.cpp` | `draw_objects.o` | `26e86e74c356e3d8e4f293962c0bea53a0ceaff31750ff6b12d9f4940654ce89` |
| `sources/game-loop-draw-particles/draw_particles.cpp` | `draw_particles.o` | `2d1ae2baa9069d4517c0b4a433e990784e57b4e04ee944a011d86006e710fe2d` |
| `sources/game-loop-frontend-net-menu/front_end_net_menu.cpp` | `front_end_net_menu.o` | `f036ebb94489e181afc72b10893debb71c7fdc541db52ec6cd4879a66810e4ee` |
| `sources/game-loop-game-loop/game_loop.cpp` | `game_loop.o` | `9d677437073f3f06c554946e38a93d7a27f1c491f2aef465a7e81bdad356f0fa` |
| `sources/game-loop-game-no-draw/game_no_draw.cpp` | `game_no_draw.o` | `23aa68b5a3277318b5de7b9a90e2e2df22219a964502b91f3b038dab557bdea7` |
| `sources/game-loop-game-over/game_over.cpp` | `game_over.o` | `fccf5890e355a7b5b9078e8f25766dba7c6743503edc1809e7d27de47d9e5c19` |
| `sources/game-loop-hud-no-draw/hud_no_draw.cpp` | `hud_no_draw.o` | `c6f679b6b1b40242f58b82dbffd07df8cf487b18dee536b8f7af94412cbed2dc` |
| `sources/game-loop-net-kick/net_kick.cpp` | `net_kick.o` | `f2d47c59494493fad2e0996c7440758d48a9728890313df16bdbb5e476e6ad62` |
| `sources/game-loop-object-draw-enable/object_draw_enable.cpp` | `object_draw_enable.o` | `8d2ea358de45b0b4b8e86da44cd8d105976a5c64e4f884f38f7b3d5802a2f4d0` |
| `sources/game-loop-player-light-attenuate/player_light_attenuate.cpp` | `player_light_attenuate.o` | `64bb3764c444695b01c0111d6c1fb71111e8c1015834474fa7daeaa5842ddba4` |
| `sources/game-loop-recall-inhibit/recall_inhibit.cpp` | `recall_inhibit.o` | `96446d66b9421320e778e852b8e15e30e99fa3acad8bf9bd3b09af4b2849b912` |
| `sources/game-loop-run-objects/run_objects.cpp` | `run_objects.o` | `9f4e4f4b6cf6c2d774582a2da40b41ad97471552519f858212fbdc4142dacbfe` |
| `sources/game-loop-transform-scene-light/transform_scene_light.cpp` | `transform_scene_light.o` | `c4da33e3bfcd72ba87a119e3daf50c44640cb0e4f1ac23e0ba8d18e8eb5aa444` |

Function inventories from the private build, for the snapshot: `camera_shake.o` `gameSetCameraShake__Ffb` 272;
`check_player_alive.o` `gameCheckPlayerAlive__Fv` 104; `draw_objects.o` `drawObjectsTask__Fv` 1068 plus two
anonymous-namespace helpers (`draw_object` 48, `walk_list` 544); `draw_particles.o` `drawParticlesTask__Fv` 12;
`front_end_net_menu.o` `frontEndStartInNetMenu__Fv` 60; `game_loop.o` `gameLoop__Fv` 3876; `net_kick.o`
`gameNetKick__Fi` 276; `run_objects.o` `runObjectsTask__Fv` 1024 plus two helpers (`call_slot` 72,
`walk_run_list` 172); `transform_scene_light.o` `transformSceneLightTask__Fv` 632. `game_no_draw.o`,
`game_over.o`, `hud_no_draw.o`, `object_draw_enable.o`, `player_light_attenuate.o` and `recall_inhibit.o` define
data only.

## Authored but never executed

Everything. The host checks run the reconstructed bodies against mocks for every callee and global. No EE,
scheduler, renderer, memory-card, network or gameplay path ran, and no authored body was compared byte for byte
with the original.

## Integrator note (2026-10-12)

On the shared tip only `game-loop-game-loop` was added to the plan. The
draw-objects unit defines `drawObjectsTask__Fv`, which `particles-draw-objects-task`
already defines; the link rejects two definitions. `drawParticlesTask__Fv` and
`objectDrawEnable` were already defined by particles. Those three units stay in
the snapshot, unselected. The 15-import count above does not hold on this tip;
`docs/agents/INTEGRATION.md` records the measured one.
