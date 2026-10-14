# Core game loop, engine and scene switching (4/5)

Package `w8-core-loop-d`, branch `agents/w8-core-loop-d`. Addresses and sizes come from the pinned private
`SLUS_205.65` symbol table and instruction bytes. No game bytes, listings or decompiler output are committed.
Compiled bodies come from the reviewed EE g++ `authored_cpp` profile and are not claimed to equal the image bytes.
Nothing here has run on the EE or as gameplay.

## Evidence

- Every function has a host check beside its source (`host_check*.cpp`, not a plan input) that returns non-zero on a wrong
  branch, offset, constant or call order.
- `docs/agents/w8-core-loop-d-compare.py` (it imports the R5900 interpreter in `combat-reactions-compare.py`) runs the
  image function and the freshly linked authored object on identical random scenario memory with identical stubbed
  callees (libc string routines, `sprintf`, and every other callee are intercepted at their image addresses in both runs).
  It requires equal stub call logs (argument registers, string arguments by content, `f12` where it is an argument, the
  stack argument of the NPC constructor), equal return value, and equal bytes written outside the stack. The authored
  object is linked against the image's own symbol addresses, so data lives where the image keeps it. It needs the private
  executable and toolchain through `NORRATH_EXE` and `NORRATH_EE`. The interpreter was extended here with `sqrt.s` and the
  EE `rsqrt.s`. Floats in scenarios are small integers or dyadic values so the interpreter's rounding and the EE's agree.
  Last run: 500 seeds each for `init__Fv`, `initLump__FPv`, `pan__FG6Point3ffPfT3` and 300 for the others, zero mismatches
  (`python3 docs/agents/w8-core-loop-d-compare.py all 300`).
- Mutation spot checks that the comparison catches: `initLump` (skip test `<` to `<=`, 128 mask to 64, one copy fewer in the
  row slide, duplicate threshold, later-row duplicate test), `init__Fv` (90.0 to 91.0, flag constant, `hvecs` index, 60.0 to
  61.0, a light-matrix entry, `+ 1` in the texture name, yaw constant), `pan` (360 threshold, upper clamp),
  `initAnimation` (38 to 37), `gameSetObjectVis` (cut-mode test), `makeUnscrambleTable` (bit shift). Each fails on seeds.
- The comparison found one real divergence while this package ran: `getLongLevelName__FPc` read its lookup result
  without a null guard, so the compiler dropped the later `found != 0` test. A null result from `textGetString__FPc` is now
  documented as unsupported (the image reads address 4 and carries on) and is not generated.

## Functions

| symbol | image | note |
|---|---|---|
| `paramRunning__Fv` | `0x00194150`, 8 bytes | returns 0 |
| `rejectForeignModules__Fv` | `0x002d01d0`, 12 bytes | empty |
| `gameMiscTaskSync__Fv` | `0x001d0b60`, 36 bytes | `playerRecallSpell__Fv` then `playerCheckRecallHotKey__Fv` |
| `plotRouteTask__Fv` | `0x002d0dc0`, 36 bytes | `worldPlotRouteProcess__Fi(0)` when `cutMode` is zero |
| `resetTextBody__Fv` | `0x001d3940`, 44 bytes | `textBodyLen = 0`, `memset(textBody, 0, 2048)` |
| `jumpGateClear__Fv` | `0x00284680`, 52 bytes | when `jumping == 0` removes the run and draw jump-gate tasks |
| `gameSomeoneInShop__Fv` | `0x001d1158`, 116 bytes | scan of `playersAlive` for bit `0x800000` of the word at +12 of the pointer at record +21872; needs `netNetworkMode != 0` |
| `getLongLevelName__FPc` | `0x0025bf40`, 168 bytes | trims the key one character at a time while the looked-up string has `:` at unit 2 |
| `gamePushConfig__Fv` / `gamePopConfig__Fv` | `0x001cc920` / `0x001cc9c0`, 156 bytes each | eighteen settings globals to and from `pushedConfig` |
| `gameSetObjectVis__Fv` | `0x001cb4e8`, 268 bytes | visibility point list for the dialog creature or the players |
| `initAnimation__Fv` | `0x001d26e8`, 292 bytes | dialog "talk" and idle animation transitions |
| `makeUnscrambleTable__Fv` | `0x0016d410`, 268 bytes | fills `unscrambleTable` from three nested counters |
| `pan__FG6Point3ffPfT3` | `0x0024cd38`, 396 bytes | level and side balance from the camera position and matrix |
| `initLump__FPv` | `0x00164338`, 960 bytes (local) | converts lump rows, drops PAL-only entries, starts files, maps animation sounds |
| `init__Fv` | `0x0027d600`, 2016 bytes (local) | player-select screen set-up (objects, light matrices, NPCs) |

Data: `gameShowEnemiesInMap` `0x484f54`, `interfaceMode` `0x483aac`, `interfaceOn` `0x483f45`, `jumping` `0x486030`,
`lastWarpPos` `0x4842d8`, `layer0Dir` `0x6cc6a0`, `layer1Dir` `0x6d26a8` (24580 bytes each, the cd-archive extent),
`levelSeeds` `0x4349a8` (4096), `loadingScreenRequestExit` `0x4866d4`, `noMonsters` `0x486840`, `optTbl` `0x7d0ca0`
(40 bytes, the whole gap to the next symbol), and `pushedConfig` `0x7d0b70` (304 bytes, helper for the config pair). Every
file byte of these is zero. Virtual-table views: `norrath_ctors_ant_lantern_view_44e940` (`_vt$15AutoContainerLP`),
`..._44ea78` (`_vt$11AutoChestLP`), `..._453328` (`_vt$4Boat`) and `norrath_factory_prop_vtable` (`_vt$4Prop`, `0x44d018`),
each a table of {adjustment word, function pointer} slots whose targets are the sized image function symbols at the
slot addresses (the same convention as `vehicle-actors-views`).

## Observed / inferred / unknown

Each source opens with its own observed / inferred / unknown comment. Highlights:

- Observed in `init__Fv`: two local image symbols are both named `flareDelta` (`0x486cb4` and `0x486d44`); this routine
  writes the second. If the shared draft already defines `flareDelta` for the first, the two statics collide by name and
  the integrator must decide how to keep them apart. The routine's use of `rsqrt.s` is reproduced with inline assembly
  (the host check substitutes `1/sqrt`).
- Observed in `initLump__FPv`: a zero-size row prints through `scePrintf` and then stores zero through the zero size (a
  store to address 0), which is kept as a volatile store. The row slide for a dropped entry copies one row more than
  the table holds, as the image does.
- Inferred only from names: `gameSetObjectVis` outputs, `rejectForeignModules` (nothing observed beyond an empty body),
  the player-select screen label for `init__Fv`.
- Unknown: the meaning of `initFile__FPcPvi`'s result (it is accumulated and 128-aligned in `initLump`), the type of
  `lastWarpPos` beyond a 4-byte word, the real extents of `noMonsters` and `optTbl`, what the unscramble table is applied
  to, the NPC constructor's parameter meanings, and every virtual slot's dispatch.

## Imports closed (31)

- `gameMiscTaskSync__Fv`
- `gamePopConfig__Fv`
- `gamePushConfig__Fv`
- `gameSetObjectVis__Fv`
- `gameShowEnemiesInMap`
- `gameSomeoneInShop__Fv`
- `getLongLevelName__FPc`
- `initAnimation__Fv`
- `initLump__FPv`
- `init__Fv`
- `interfaceMode`
- `interfaceOn`
- `jumpGateClear__Fv`
- `jumping`
- `lastWarpPos`
- `layer0Dir`
- `layer1Dir`
- `levelSeeds`
- `loadingScreenRequestExit`
- `makeUnscrambleTable__Fv`
- `noMonsters`
- `norrath_ctors_ant_lantern_view_44e940`
- `norrath_ctors_ant_lantern_view_44ea78`
- `norrath_ctors_ant_lantern_view_453328`
- `norrath_factory_prop_vtable`
- `optTbl`
- `pan__FG6Point3ffPfT3`
- `paramRunning__Fv`
- `plotRouteTask__Fv`
- `rejectForeignModules__Fv`
- `resetTextBody__Fv`

Defined by this package but not on the original list: `pushedConfig` (the block `gamePushConfig__Fv` and `gamePopConfig__Fv` share;
a local image symbol, defined here so both units refer to one object).

## Imports opened (68 names new against the unresolved list of the integrator's earlier plan8 build)

- `DLG_GetDialogCreature__Fv`
- `_$_11AutoChestLP`
- `_$_15AutoContainerLP`
- `_$_4Boat`
- `__3NPCG6Point3sPcN33ifPPc`
- `__tf11AutoChestLP`
- `__tf15AutoContainerLP`
- `__tf4Boat`
- `animAddToEnd__FP14AnimationStateP15AnimationHeaderi`
- `animMapAnimationSounds__FP15AnimationHeader`
- `animUseBuffer`
- `bodyAlpha`
- `changeBeard__Fii`
- `changeHairColor__Fii`
- `changeHairStyle__Fii`
- `changeSkin__Fii`
- `curPlayer`
- `doMyUniqueForces__4Boat`
- `dummyList`
- `getHairAndSkin__Fi`
- `headerAlpha`
- `hermDeltaScale`
- `hermiteTime`
- `hpos`
- `hvecs`
- `initFile__FPcPvi`
- `initLightInfo__Fv`
- `initNamePad__Fv`
- `initPreSelect__Fv`
- `jumpGateTaskDraw__Fv`
- `jumpGateTaskRun__Fv`
- `lightMats`
- `memmove`
- `msg_collision__13AutoContainerP10GameObject`
- `msg_draw__11AutoChestLP`
- `msg_draw__15AutoContainerLP`
- `msg_getPokeName__13AutoContainerPUsi`
- `msg_getPokeName__9AutoChestPUsi`
- `msg_hurt__13AutoContainerfP10GameObjecti`
- `msg_load__13AutoContainerPc`
- `msg_load__9AutoChestPc`
- `msg_poke__11AutoChestLPi`
- `msg_poke__13AutoContaineri`
- `msg_run__11AutoChestLP`
- `msg_run__15AutoContainerLP`
- `msg_save__13AutoContainerPc`
- `msg_save__9AutoChestPc`
- `nameTags`
- `needEnforce`
- `npcLumpName`
- `objectAddToRunList__FP10GameObject`
- `objectVisCount`
- `objectVisPoints`
- `objectVisUseCamCenter`
- `playerCheckRecallHotKey__Fv`
- `playerRecallSpell__Fv`
- `playerSelectInfo`
- `pnums`
- `requestToExit`
- `set__4Boat`
- `startExitFade`
- `strrchr`
- `targetObjs`
- `targets`
- `textBodyLen`
- `unscrambleTable`
- `worldPlotRouteProcess__Fi`
- `yaws`

Notes on these: `unscrambleTable` (`0x6e9080`, 256 bytes) is written by `makeUnscrambleTable__Fv` and is nobody's definition yet.
`objectVisCount` (`0x483afe`, signed byte), `objectVisUseCamCenter` (`0x483ac9`, byte) and
`objectVisPoints` (`0x333d28`, 48 bytes) are the outputs of `gameSetObjectVis__Fv`. `textBodyLen` is the word at `0x486b68`.
`initFile__FPcPvi`, `memCheckHeapReal__FPCci`, `scePrintf` and the libc routines are single-address callees. The virtual-table
views open the slot functions named in the image (`msg_*`, `__tf*`, `_$_*`, `eventActivate__7Vehicle` and friends) because
every slot is referenced by name. `init__Fv` opens the player-select statics (`targets`, `targetObjs`, `yaws`, `hpos`, `hvecs`,
`lightMats`, `nameTags`, `pnums`, `dummyList`, `playerSelectInfo`, `npcLumpName`, `curPlayer`, `requestToExit`, `startExitFade`,
`needEnforce`, `hermiteTime`, `headerAlpha`, `hermDeltaScale`, `bodyAlpha`, and `flareDelta` which another package defines) and
the screen helpers (`getHairAndSkin__Fi`, `changeHair*`, `changeBeard__Fii`, `changeSkin__Fii`, `initPreSelect__Fv`,
`initNamePad__Fv`, `initLightInfo__Fv`) plus `__3NPCG6Point3sPcN33ifPPc`. Names already pending from other packages (for example
`player`, `playersAlive`, `nmPlayersAlive`, `textBody`, `dialogCreature`, `anim`, `strlen`, `strncmp`) were not counted as opened.

## Build-plan entries needed

32 units in `authored_cpp`, appended in this order (the integrator assigns `input_order`; `emitted_functions` below are the
compiled sizes from the reviewed toolchain). Do not add any `host_check*.cpp`. `pushed_config.hpp` is the only header.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "misc_task_sync.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-misc-task-sync/misc_task_sync.cpp",
  "sha256": "f46041f94bd1d43f69b9edad09a27304b66a06846fc3bd09a52056e9f582324c",
  "emitted_functions": [
   {
    "name": "gameMiscTaskSync__Fv",
    "compiled_body_bytes": 28
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "param_running.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-param-running/param_running.cpp",
  "sha256": "2b23267803052e4fbee1ed3cab9b108418b58485fdf8258b496ec64088698682",
  "emitted_functions": [
   {
    "name": "paramRunning__Fv",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "plot_route_task.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-plot-route-task/plot_route_task.cpp",
  "sha256": "695b9b7bffb534e3b54969b151a925593701a446df4515adff92b41e5c2ad75f",
  "emitted_functions": [
   {
    "name": "plotRouteTask__Fv",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "reject_foreign_modules.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-reject-foreign-modules/reject_foreign_modules.cpp",
  "sha256": "d75c31473fb86dcecd4527b64b37539ed209155965376ca70458673de28d1579",
  "emitted_functions": [
   {
    "name": "rejectForeignModules__Fv",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "reset_text_body.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-reset-text-body/reset_text_body.cpp",
  "sha256": "e49c14a607a7e4cf8a07d5eaae7de9b489730f88707fca8d3c3529eb83316e93",
  "emitted_functions": [
   {
    "name": "resetTextBody__Fv",
    "compiled_body_bytes": 28
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "jump_gate_clear.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-jump-gate-clear/jump_gate_clear.cpp",
  "sha256": "03ee72203217ce3599f12c706cc787c51d09acf6e6f583d4acf1c30d3359ed0d",
  "emitted_functions": [
   {
    "name": "jumpGateClear__Fv",
    "compiled_body_bytes": 64
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "someone_in_shop.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-someone-in-shop/someone_in_shop.cpp",
  "sha256": "4c130176bdc3b8f0a39e269216f22885f3c38a30933679a5128d31d2251a5ba1",
  "emitted_functions": [
   {
    "name": "gameSomeoneInShop__Fv",
    "compiled_body_bytes": 136
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "long_level_name.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-long-level-name/long_level_name.cpp",
  "sha256": "dc18e217ee53935e31b9d95e59418c5ab0b15c6d59f46edae80c488df5813dfe",
  "emitted_functions": [
   {
    "name": "getLongLevelName__FPc",
    "compiled_body_bytes": 172
   }
  ]
 },
 {
  "header_closure": [
   "sources/w8-core-loop-d-config-stack/pushed_config.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "pushed_config.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-config-stack/pushed_config.cpp",
  "sha256": "9ad3aa86b6f2f3a64ddea9fb335981802817ecbe987ad816782723aa181febdd",
  "emitted_functions": []
 },
 {
  "header_closure": [
   "sources/w8-core-loop-d-config-stack/pushed_config.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "push_config.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-config-stack/push_config.cpp",
  "sha256": "c4638e6b5cd9b65a264b8f4ecbfcd365e2b58b1de4faf6f405d79b91b66722a5",
  "emitted_functions": [
   {
    "name": "gamePushConfig__Fv",
    "compiled_body_bytes": 228
   }
  ]
 },
 {
  "header_closure": [
   "sources/w8-core-loop-d-config-stack/pushed_config.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "pop_config.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-config-stack/pop_config.cpp",
  "sha256": "16ffdf90bd5feb37448fc6f3ab247114a6ff4df9c8327686813557a9d4a4b7fc",
  "emitted_functions": [
   {
    "name": "gamePopConfig__Fv",
    "compiled_body_bytes": 228
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "set_object_vis.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-set-object-vis/set_object_vis.cpp",
  "sha256": "0d69c049aaf2ded956f0c3f12fed1968af97d1e900f56f5285dca1eecf583d96",
  "emitted_functions": [
   {
    "name": "gameSetObjectVis__Fv",
    "compiled_body_bytes": 296
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "init_animation.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-init-animation/init_animation.cpp",
  "sha256": "9cae9cf31fc8e9e2255b82a3c2aef6e51ff1c3496f219f062f8b15952b46fd1c",
  "emitted_functions": [
   {
    "name": "initAnimation__Fv",
    "compiled_body_bytes": 332
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "unscramble_table.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-unscramble-table/unscramble_table.cpp",
  "sha256": "3c514b7e9c3b7c4a056121e8801868d172701992790867c2d70ecdb2eb26ef81",
  "emitted_functions": [
   {
    "name": "makeUnscrambleTable__Fv",
    "compiled_body_bytes": 260
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "pan.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-pan/pan.cpp",
  "sha256": "63bdacc5763308aef292ad5ebd79f6978fd1c17419783c7418c70205985ce65a",
  "emitted_functions": [
   {
    "name": "pan__FG6Point3ffPfT3",
    "compiled_body_bytes": 328
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "game_show_enemies_in_map.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/game_show_enemies_in_map.cpp",
  "sha256": "49f122d3c68e2bdbb532915f9b2d2f0d728a85ae6d5eb2a7b947d9cbe4794090",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "interface_mode.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/interface_mode.cpp",
  "sha256": "28c637757490f4a0dc96c63ee565956dc9980dadd8c250b0f3081665ea6c92ed",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "interface_on.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/interface_on.cpp",
  "sha256": "40d1ac870875116bf2d16841fe8ea375cbea408a9b6655c4ae907d8918894b1a",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "jumping.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/jumping.cpp",
  "sha256": "c7bb63294b6805a4bdeea8a20643e5b6539d60cbf02e4597cfb6470494fc8242",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "last_warp_pos.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/last_warp_pos.cpp",
  "sha256": "9307817c6c3c57597af936652b8f0f6e35e548644fe0fab6061864c097cd078c",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "layer0_dir.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/layer0_dir.cpp",
  "sha256": "a5b810f8a63d4e7cabf6f36e5e5deaba2a4d44cbc3cf9b41e84b1f8689db410b",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "layer1_dir.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/layer1_dir.cpp",
  "sha256": "f5ab6995e05d9830238266866511c380eca2383c20ca8db24ee2c744f53f7c79",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "level_seeds.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/level_seeds.cpp",
  "sha256": "87e802c3125c8bb6d14da134dd252438dacb5e67dcf0cb13e1fcc7fbf8c2b58a",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "loading_screen_request_exit.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/loading_screen_request_exit.cpp",
  "sha256": "8a85fc20b8a3708749e5ad0910addad605a6807a09fe64ab3258977e675e2964",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "no_monsters.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/no_monsters.cpp",
  "sha256": "abcca08a6c442156a5dfc5151f3dd3e4869a4444979ce5577b45bb0987722e0b",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "opt_tbl.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-data/opt_tbl.cpp",
  "sha256": "6ed51a14d66932eb8050264dd93eb4953c54e56d85776b560e0694d3d73c02a9",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "view_44e940.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-vtables/view_44e940.cpp",
  "sha256": "735bf28f4671512ff1eea94b4a7f94c28b9465c8f8031e5b10f75bd0533da656",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "view_44ea78.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-vtables/view_44ea78.cpp",
  "sha256": "0c42d62db86778a4915cb7291e1d56a30030893c6b1c7ceed059184b600107f3",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "view_453328.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-vtables/view_453328.cpp",
  "sha256": "80f87abb07321d2381e7219267cc308629e387282a7cf40f6673a2b1ba4130bc",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "factory_prop_vtable.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-vtables/factory_prop_vtable.cpp",
  "sha256": "f39639ba9ede5d85577ebf0ef5c39951d83afdb51ef3f9239e0c627292aa8ac9",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "init_lump.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-init-lump/init_lump.cpp",
  "sha256": "b722e2c05b5e2d79594ac813aba18705b534c729aff36dc1e5aa66ca4df586c0",
  "emitted_functions": [
   {
    "name": "initLump__FPv",
    "compiled_body_bytes": 1012
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "select_init.o",
  "profile": "authored_cpp",
  "source": "sources/w8-core-loop-d-select-init/select_init.cpp",
  "sha256": "475cb5f26a50ac979189808c8a3cb90b2f8a3955055b6d441bc339336f143534",
  "emitted_functions": [
   {
    "name": "init__Fv",
    "compiled_body_bytes": 1812
   }
  ]
 }
]
```

## Verification run

- Each source compiled with the reviewed EE g++ and the `authored_cpp` flags, and each host check built with the host
  C++ compiler and exited 0.
- `python3 docs/agents/w8-core-loop-d-compare.py all 300`: every function above, zero mismatches (see Evidence).
- A private copy of the plan and snapshot, kept outside git, appended these 32 units to the branch plan
  (`--check-plan` passed). The partial build compiled 1439 inputs with implicit assembly, linked once with 1123 function
  bodies, 1232 unresolved imports and 0 COMMON symbols, with none of the 31 closed names left unresolved. This link did
  not execute gameplay and is not an original-byte match.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests ran, all OK.

## Authored but never executed

All of the above. The functions ran only in host checks and in the R5900 interpreter comparison against the image's
instructions with stubbed callees; none ran on the EE, none in a running game. `init__Fv`, `initLump__FPv` and the
virtual-table views have never been linked into an executable. The virtual-table slot targets are referenced by name only.

## Unknown

- What `initFile__FPcPvi` returns and why `initLump__FPv` accumulates it as an alignment shift; why only `pal_` rows are dropped.
- Which screen `init__Fv` belongs to beyond the helper names, the layout of `playerSelectInfo` and `targets` beyond the
  fields used, the NPC constructor's parameter meanings, and the buffer sizes of its temporaries.
- The consumers of `unscrambleTable`, `objectVisPoints`, `lastWarpPos`, `levelSeeds`, `optTbl` (only the 40-byte gap is known) and
  `noMonsters` (extent unrecorded).
- What a nonzero `paramRunning__Fv` result would mean; what `rejectForeignModules__Fv` was for before it was emptied.
- The two image symbols named `flareDelta` and which of them another package's definition corresponds to.
