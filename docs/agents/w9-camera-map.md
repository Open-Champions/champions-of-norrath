# Camera and automap (`agents/w9-camera-map`)

Package notes for `agents/w9-camera-map`. Addresses and sizes come from the
private `SLUS_205.65` symbol table and disassembly read locally; section bytes
were read locally for literal values. They are local evidence only. Nothing
derived from the executable besides short strings, small constants and
addresses is published. Each function is one commit; each has a `host_check.cpp`
(not a build-plan input) that I ran, and for most I ran deliberately wrong
variants of the source that the check rejected (a changed constant, offset,
comparison or call order). The unit-test suite
`python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).
Every unit also compiled alone with the reviewed `authored_cpp` profile. None of
the compiled bodies is claimed to match the original bytes, although several
small ones are visibly the same instruction sequence (`GetTargetNum__6Camera`,
`GetZoomDistance__6Camera`, `SetCutWasActive__6Camera`, `Pop__6Camera` apart
from a tail call, `__3Webii`, `isRevealed__3mapv` apart from address
formation). I did not run the shared partial build: `--check-plan` rejects a
private plan copy because the snapshot covers a fixed input set, so the plan
entries below are untested against the real driver.

## Observed

| Function | Address | Bytes | Unit directory |
|---|---|---|---|
| `SetCutWasActive__6Camera` | `0x0026fc30` | 16 | `set-cut-was-active` |
| `GetZoomDistance__6Camera` | `0x0026fc50` | 8 | `get-zoom-distance` |
| `GetTargetNum__6Camera` | `0x0026fc88` | 8 | `get-target-num` |
| `SetZoomDistance__6Cameraf` | `0x0026fb08` | 112 | `set-zoom-distance` |
| `Save__6CameraPc` | `0x0026c4d0` | 264 | `camera-save` |
| `Pop__6Camera` | `0x0026fa20` | 104 | `camera-pop` |
| `doInnorukCam__6Camera` | `0x0026f7a0` | 424 | `do-innoruk-cam` |
| `doDebugCam__6Camera` | `0x0026cf70` | 1376 | `do-debug-cam` |
| `doNormalCam__6Camera` | `0x0026d4d0` | 1496 | `do-normal-cam` |
| `isRevealed__3mapv` | `0x0015e138` | 8 | `map-is-revealed` |
| `revealEntireMap__3mapv` | `0x0015e078` | 120 | `map-reveal-entire` |
| `willDraw__3mapP12_worldHeaderf` | `0x0015e140` | 88 | `map-will-draw` |
| `worldToMap__3mapP6Point2PfT2` (local `t` symbol, needed by `revealMap`) | `0x0015e0f0` | 72 | `map-world-to-map` |
| `revealMap__3mapG6Point3` | `0x0015dd58` | 688 | `map-reveal-map` |
| `addIcon__3mapP6Point3iPciT3N33` (also `norrath_map_add_icon`) | `0x0015c320` | 776 | `map-add-icon` |
| `cutRunFrameTask__Fv` | `0x0018e428` | 32 | `cut-run-frame-task` |
| `cutStop__Fv` | `0x0018ce88` | 1224 | `cut-stop` |
| `cutDrawFrame__Fv` | `0x0018c378` | 1408 | `cut-draw-frame` |
| `cutInitScene__FP9_cutscene` | `0x0018d350` | 3696 | `cut-init-scene` |
| `__3Webii` (also `norrath_web_construct`) | `0x0020fac8` | 24 | `web-construct` |
| data: `mapButtonWasDown` (`0x0042a4b0`, 16 bytes .data), `mapObjList` (`0x00484f9c`, 4 bytes .sdata), `mapScale`, `mapX`, `mapY` (`0x00486b04`..`0x00486b0c`, .sbss) | | | `map-storage` |

Each directory name is prefixed `w9-camera-map-`. Every description comment in
the sources states the observed behaviour field by field; field names are
offsets, not guessed names.

## Inferred

- `norrath_map_add_icon` and `norrath_web_construct` are the names authored
  callers (`ctors-lava-wraith-*`) use for `addIcon__3mapP6Point3iPciT3N33` and
  the `Web` two-argument constructor `__3Webii`. Both are provided as GAS
  aliases inside the owning unit, following the existing `norrath_*` alias
  pattern (`.set`, with the Mach-O underscore variant for the host check). The
  call layouts match (eight arguments for `addIcon`; `this`, int, int for the
  constructor). `plan.symbol_aliases` stays empty.
- `worldToMap__3mapP6Point2PfT2` is a local (`t`) symbol in the original. It is
  authored with external linkage only because `revealMap` lives in its own unit.
- The Point3 argument of `revealMap__3mapG6Point3` and the position argument of
  `addIcon` are passed by address (copied by the caller); this follows the
  mangled `G6Point3` and the load pattern.
- In `Save__6CameraPc` the worldCameraMatrix is read as four groups of four floats
  with a stride of three (observed); I took it to be a quirk of the original, not
  a different layout. Whether the last read (float index 12) is intended is unknown.
- `doDebugCam__6Camera`, `doNormalCam__6Camera`, `doInnorukCam__6Camera`:
  the names "debug", "normal", "Innoruk" are only the symbol names. The
  reading of the pad record fields (+20/+24/+28/+32 floats, +8/+12 words)
  as stick and button inputs is inferred from use, not established.

## Imports closed (names)

`GetTargetNum__6Camera`, `GetZoomDistance__6Camera`, `Pop__6Camera`,
`Save__6CameraPc`, `SetCutWasActive__6Camera`, `SetZoomDistance__6Cameraf`,
`addIcon__3mapP6Point3iPciT3N33`, `cutDrawFrame__Fv`, `cutInitScene__FP9_cutscene`,
`cutRunFrameTask__Fv`, `cutStop__Fv`, `doDebugCam__6Camera`,
`doInnorukCam__6Camera`, `doNormalCam__6Camera`, `isRevealed__3mapv`,
`mapButtonWasDown`, `mapObjList`, `mapScale`, `mapX`, `mapY`,
`norrath_map_add_icon`, `norrath_web_construct`, `revealEntireMap__3mapv`,
`revealMap__3mapG6Point3`, `willDraw__3mapP12_worldHeaderf`. That is all 25
names of the package scope. Two helpers, `worldToMap__3mapP6Point2PfT2` and
`__3Webii`, are also defined; neither was in the scope list.

## Imports opened (referenced, left unresolved)

Classified against `work/plan9-build/unresolved-imports.json` and
`partial-inventory.json` of the lead's checkout, which are older than the current
shared branch; I did not re-link, so the third group may contain names that other
packages define or already reference.

- Already in that unresolved list (32): `_3map$mapOffset`, `_3map$mapheight`, `_3map$maptiles`, `_3map$mapwidth`, `__6PlayerG6Point3sii`, `animationLoadPos`, `animationLoading`, `blockAlloc__Fi`, `dialogPacket`, `displayDialogPos`, `dlgDir`, `flags`, `fontPrintWrapCenteredu__FiiPUsiiii`, `icos__Fi`, `isin__Fi`, `lightGlobalCharLightScale__Ff`, `memAlloc__FiPc`, `memFree__FPv`, `memmove`, `myFOV`, `myPitch`, `myRoll`, `myYaw`, `nmActions`, `nmDialogNodes`, `objectCancelAllAlwaysDraw__Fv`, `objectFindEmptySpot__FG6Point3iiPc`, `objectIterateRunLists__FPFP10GameObject_v`, `player`, `thereIsACutPlayer`, `world`, `worldFindHeight__FP12_worldHeaderG6Point3f`.
- Defined as functions in that baseline (33): `BuildMatrix__6CamerafffP6Point3`, `GetPitch__6Camera`, `GetYaw__6Camera`, `SetMode__6Camerai`, `SetPitch__6Camerai`, `SetYaw__6Camerai`, `cdWait__Fv`, `cutRunFrame__Fb`, `engineRemoveTask__FPFv_v`, `findTagString__10GameObjectPci`, `fontEnd__Fv`, `fontScissor__Fiiii`, `fontStart__FP4FontiUii`, `lumpFindResource__FPcT0`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectFindObject__FPc`, `objectFindObject__FPcPP10GameObjecti`, `objectUpdateInGrid__FP10GameObject`, `sleepMonsters__8creaturev`, `soundClearStreamQue__Fi`, `soundGetDialogDir__Fv`, `soundGetStreamPlayStatus__Fi`, `soundResetStream__Fi`, `soundStartDialogStream__FiPci`, `sprintf`, `sscanf`, `strcmp`, `strcpy`, `strncmp`, `textGetString__FPc`, `wakeMonsters__8creaturev`, `worldSetFarClip__Ff`, `worldSetFrustum__Fffff`.
- In neither (62); data symbols and functions that no earlier unit referenced or
  that other packages of this wave own: `GLISTSIZE`, `_3map$iconList`, `_3map$iconListCount`, `_3map$revealed`, `action`, `actor`, `anim`, `animRemoveNonpermanentAnimations__FP14AnimationState`, `checkYawReset__6Camera`, `coreLoadingScreenRunning`, `cutInit`, `cutLoadedDialogStream`, `cutMode`, `cutSceneName`, `cutSkipMode`, `cutStartMusic`, `determineBestController__6Camera`, `dialogActor`, `displayActionPos`, `displayFrame`, `displayMusicPos`, `displayTextDisable`, `doAutoPitch__6Camera`, `doUserYaw__6Camera`, `doUserZoom__6Camera`, `eRandState`, `enforceMaxSpeed__6CameraP6Point3`, `engineScissory1`, `engineScissory2`, `engineScreenSlide`, `findCutDialog__Fv`, `frameNm`, `frontEndRunning`, `gameCam`, `gamePlayersPresent`, `glistp`, `gstart`, `hideHiddenObjects__FP10GameObject`, `hudLetterBox__Fv`, `levelName`, `localNmPlayersAlive`, `localPlayersAlive`, `musicSize`, `netNetworkMode`, `nmActors`, `nmPlayers`, `nmPlayersAlive`, `objectCreateByName__FPcG6Point3sPPc`, `obtainTarget__6CameraP6Point3`, `pad`, `playerCountChanged__6Camera`, `playersAlive`, `rayHit__FP6Point3N30`, `removeMonster__FP10GameObject`, `resetScissor__Fv`, `showHiddenObjects__FP10GameObject`, `smallFont`, `soundStartMusic__Fii`, `subtitlesOn`, `unremoveMonster__FP10GameObject`, `worldCameraMatrix`, `worldCameraPos`.

Layout assumptions I made for data owned elsewhere: `player` is a table of four
23984-byte records (object pointer word at `+21872`, a float at `+172`); `pad` a table of
88-byte records; `actor` a pointer to 44-byte records and `action` a pointer to 20-byte
records; `anim` two 8-byte slots (`0x00790f60`); `worldCameraMatrix` and
`worldCameraPos` float arrays. The unit comments say where each is read.

## Build-plan entries needed

All 21 units use profile `authored_cpp`, an empty `header_closure`, and kind
`independently_authored_source`. `input_order` is for the integrator. Rows are
source, object name, sha256 of the source. `emitted_functions` are the
function names listed in the table above (plus the two `static` helpers
`transformPosition` and `writeCameraRecords` of `cut-init-scene`, which appear
as local symbols).

| Source | Object | sha256 |
|---|---|---|
| `sources/w9-camera-map-camera-pop/pop.cpp` | `w9_cam_camera_pop.o` | `461cc3416d6b91436a33d5103f253f50cb308e1163b906e490704922189b1a1e` |
| `sources/w9-camera-map-camera-save/save.cpp` | `w9_cam_camera_save.o` | `5610375e8933016e90dadf87d320d44ace9597cb3a114eaf5f920371f3ab2e4a` |
| `sources/w9-camera-map-cut-draw-frame/cut_draw_frame.cpp` | `w9_cam_cut_draw_frame.o` | `1d6453bfedd41a6b883fefa98a6e180c76bbe15ae2d74b8e523ec1aa2ca3e376` |
| `sources/w9-camera-map-cut-init-scene/cut_init_scene.cpp` | `w9_cam_cut_init_scene.o` | `722e0088687355b7254e8042a8406d4557b54b0a49c1830cf41ee0f374a65720` |
| `sources/w9-camera-map-cut-run-frame-task/cut_run_frame_task.cpp` | `w9_cam_cut_run_frame_task.o` | `1ebd3c5664e2de08b9b84a1f5e2f2a1b169ec1044823b6c7676272ec049b1cb4` |
| `sources/w9-camera-map-cut-stop/cut_stop.cpp` | `w9_cam_cut_stop.o` | `0509b499db526114396627a6c84dac32808511db621a093a1ec89f59385e05e2` |
| `sources/w9-camera-map-do-debug-cam/do_debug_cam.cpp` | `w9_cam_do_debug_cam.o` | `4e3adad8462c0ee86befc0e6bc8aa8bc4b09d7a2ed4ad6355f7af68db98922d9` |
| `sources/w9-camera-map-do-innoruk-cam/do_innoruk_cam.cpp` | `w9_cam_do_innoruk_cam.o` | `72dfc83b69dd6fe5683e87838379eab4af167132912aac08ff0517ad50670711` |
| `sources/w9-camera-map-do-normal-cam/do_normal_cam.cpp` | `w9_cam_do_normal_cam.o` | `7f5454479efb4dc9116721bf2d05e7b9efa31ea2df457396a03c6df7436ecf02` |
| `sources/w9-camera-map-get-target-num/get_target_num.cpp` | `w9_cam_get_target_num.o` | `d72e50a721a01cb8cd0c034bae818e4ba75f8eac195690a7e4870b7b045e7644` |
| `sources/w9-camera-map-get-zoom-distance/get_zoom_distance.cpp` | `w9_cam_get_zoom_distance.o` | `41655a8650c137e17612ae9eb129dee09ff358a0e954fb77c8938c4937dc5fd5` |
| `sources/w9-camera-map-map-add-icon/add_icon.cpp` | `w9_cam_map_add_icon.o` | `742bc448c200b541b615d5bf08cc8708faea880175ec3cdbc2392aec9e439243` |
| `sources/w9-camera-map-map-is-revealed/is_revealed.cpp` | `w9_cam_map_is_revealed.o` | `ec11ce4458a2f2c675968c7c9230c7e4608940443f653c63ecf2dcb73ecd90a5` |
| `sources/w9-camera-map-map-reveal-entire/reveal_entire.cpp` | `w9_cam_map_reveal_entire.o` | `ec3d2e9c6a3449670652d85de90609dd075131a790a2cf1a0d2e64c8762d0b80` |
| `sources/w9-camera-map-map-reveal-map/reveal_map.cpp` | `w9_cam_map_reveal_map.o` | `0524638d816c1280b19a7e8b36a403ee1a2591a6d6b929e3bdbc052a800a139d` |
| `sources/w9-camera-map-map-storage/storage.cpp` | `w9_cam_map_storage.o` | `cc965afbe65ce41394c9a8077aff4e73887203c17bc16959c0d386f6118f414c` |
| `sources/w9-camera-map-map-will-draw/will_draw.cpp` | `w9_cam_map_will_draw.o` | `7f2eb344ce8db01ee9040984269a7027a856b851ee57508eee9636703e72395e` |
| `sources/w9-camera-map-map-world-to-map/world_to_map.cpp` | `w9_cam_map_world_to_map.o` | `4a0cfa4750b911001da60380c1333a8054baf2e7474c5d5fc9c248b3ca388263` |
| `sources/w9-camera-map-set-cut-was-active/set_cut_was_active.cpp` | `w9_cam_set_cut_was_active.o` | `c03ff2f0fa3c7154222a459625e9af494d3753db57fcdc86a17f8dcb4b0e187b` |
| `sources/w9-camera-map-set-zoom-distance/set_zoom_distance.cpp` | `w9_cam_set_zoom_distance.o` | `20fbda10c54e6a05102d99f4196d3b000cc88e4d7e73188ca7e2985107bb8637` |
| `sources/w9-camera-map-web-construct/web_construct.cpp` | `w9_cam_web_construct.o` | `db986d1435b54f479b30a8b850ca7570fe9faac2c54324de6d63f5e9068299cf` |

Host-check notes for the integrator: `cut-init-scene/host_check.cpp` must be
built for x86_64 with `-Wl,-pagezero_size,0x10000` (it keeps fake objects below
4 GiB, like `w8-net-play-c-start-packet`). The others build with a plain host `c++ -std=c++17`.

## Authored but never executed

All of it. Every unit was compared only by its host check against stubbed callees and
an independently written model or table. No unit ran on the EE, in the private
paired-run harness, or in the game. In particular `doNormalCam__6Camera`,
`cutInitScene__FP9_cutscene` and `cutDrawFrame__Fv` are large and branchy; the host
checks cover their branches but are written from the same reading of the disassembly
as the source. Float literal constants that the file holds one unit below the nearest
float of a decimal are written as hexadecimal floats.

## Unknown

- What the camera fields mean beyond the offsets used (all unit comments say so).
- The meaning of the action kinds `-100` and `-99`, the action types 0 to 11,
  the scene flag bits 1, 2, 4, 8, 0x10, and the actor record byte +42 and +43.
- `cutDrawFrame__Fv` reads `*(float*)prev` through a null pointer in the original
  when no previous marker record is found; the authored source uses 0.0 for that
  case instead and the case is not exercised. Whether low memory holds zero on
  the EE is not established.
- `cutInitScene__FP9_cutscene` contains an unreachable null test that calls
  `__throw_bad_typeid` before a virtual type-record call; it is not reproduced.
- The original passes two incidental registers around some calls; they are not
  modelled (no claim on incidental A2/A3 or low64 state).
- `SetZoomDistance__6Cameraf`: the original uses the EE `min.s` for the blend limit;
  the authored `if` form compiles to a compare and branch instead. They are the same
  for non-NaN values.
- Indexing of `mapButtonWasDown` by player and the ownership of `mapScissorX` and
  `mapScissorY` (`0x00486b10`, `0x00486b14`), which sit next to my three floats and
  are not defined by any unit of mine.
