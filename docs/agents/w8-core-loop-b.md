# Core game loop, engine and scene switching (2/5)

Package work is on `agents/w8-core-loop-b`. Addresses, sizes and initial values come from the pinned
`SLUS_205.65` symbol table and its bytes; every source file opens with its own observed / inferred / unknown
comment. Host checks sit beside each source as `host_check.cpp` and are not EE plan inputs. Every source compiles
with the reviewed `authored_cpp` flags. No compiled body is claimed to match the original bytes.

## Functions (one commit each)

- `coreInterfaceFadeUp__Fi` (`0x002d0cb0`, 20 bytes), `coreStartSwitchScene__Fi` (`0x002d0db0`, 16 bytes) and
  `drawListClear__Fv` were authored by an earlier run of this package and kept as they were.
- `cutClear__Fv` (`0x0018cda8`, 224 bytes): resets cutscene state, removes two engine tasks, restores clip,
  frustum and light scale, frees the two animation slots. A first version used the decimal 1.33f; the file word is
  `0x3faa3d70`, one unit below the nearest float, so a later commit writes it as a hexadecimal literal.
- `doNewWorld__FPc` (`0x0013a478`, 256 bytes): copies the level name and parses an `orientation=` argument of the
  running object into `levelName+80`, or stores -1.
- `earlyInit__3mapP12_worldHeader` (`0x0015c1d8`, 324 bytes): loads the map tile rows from the world header record.
  It contains an empty delay loop of five nop words per iteration.
- `drawDome__Fv` (`0x001cd998`, 412 bytes): draws the dome model and texture lump pairs around the camera.
- `cutRunFrame__Fb` (`0x0018c8f8`, 1200 bytes): dialog animation loading, cutscene camera, skip and finish logic.
- `drawAutoMap__Fv` (`0x001cabd0`, 2328 bytes): automap button handling, easing of the map values, and the map
  draw call with its object list.

## Data

One unit, `w8-core-loop-b-data`, defines 22 data symbols with their file values (all zero except
`coreInterfaceFadeUpVal`, four floats of 1.0f, and `currentLayerDir`, the address of `layer0Dir`). Sizes of the
uninitialised arrays are the distance to the next symbol. Placement in the original data areas is not reproduced.

## Verification run

- Each host check compiled with `c++ -std=c++17 -Wall` and ran with exit code 0.
- Mutation spot checks: `cutRunFrame` (pad mask, dialog entry stride, flag bit), `drawAutoMap` (half-size, filter
  bit, player count special case, easing factor, modulus, table row selection, filter copy, health comparison) each
  fail a numbered host check. The health comparison mutant survived once and a case was added that kills it.
- Each source compiled with the reviewed EE g++ and `authored_cpp` flags. Linking the ten objects' symbol tables
  shows all 31 package names defined. No full partial build was run; the unresolved-import delta below is computed
  from the object symbol tables, not from a link.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests ran, all OK.

## Observed, inferred, unknown (short form)

Observed: all addresses, sizes, string and float constants, branch structure, call order and argument registers
quoted in the source headers. Inferred: types of data from load and store forms; `advanceFrame__Ff` returns a
float; word12 of the pad record is a pressed set and word8 a held set; `dpcmp` etc. semantics for the soft double
helpers. Unknown: the meaning of 447, 16339, flag bits, the dialog packet and `dlgDir` record layouts, the world
header fields at +108, +112 and +128, the pad record beyond the fields used, the argument roles of `map::draw`,
and what the empty loop in `earlyInit` was before it was stripped.

## Imports closed

coreInterfaceFadeUpVal, coreInterfaceFadeUp__Fi, coreLevelCommentTimer, coreLoadingStart, coreStartSwitchScene__Fi,
coreSwitchScene, coreSwitchSceneStart, correction, curFocus, curPlaySeconds, currentLayerDir, currentRunningObject,
cutClear__Fv, cutRunFrame__Fb, delayCount, disableVolumetricLighting, doNewWorld__FPc, drawAutoMap__Fv, drawBustCur,
drawBustMax, drawBustOn, drawBustTexName, drawBustTitle, drawCompassOn, drawCompassPoint, drawCompassTitle,
drawDome__Fv, drawListClear__Fv, dummyTex, earlyInit__3mapP12_worldHeader, endInit.

## Imports opened (new or still unresolved after this package)

Functions: GetTargetNum__6Camera, GetYaw__6Camera, MC_GetAsyncMode__Fv, SetCutWasActive__6Camera,
SetMode__6Camerai, advanceFrame__Ff, cdIsBusy__Fv, cdLoadAsync__FPcPvT0ii, cdWait__Fv, coreStartLoading__Fv,
cutDrawFrame__Fv, cutInitScene__FP9_cutscene, cutRunFrameTask__Fv, cutStop__Fv,
drawCharacterCallBack__FiP6Point3PsPiN23, draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is,
engineRemoveTask__FPFv_v, engineSetDisplayDisable__Fi, engineSyncVBlank__Fv, engineTaskActive__FPc,
gameIsPaused__Fv, isRevealed__3mapv, lightDisableLocalLights__Fv, lightGlobalCharLightScale__Ff,
lightResetOptionalParams__Fv, lightSetAmbientColor__FPs, lightSetAmbientOnly__Fv, lumpFindByType__FPvPci,
matIdent__FP8Matrix34, matMakeTrans__FP8Matrix34G6Point3, matRot__FP8Matrix34is, memFree__FPv, modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi,
modelResetOptionalParams__Fv, modelSetNoZWrite__Fv, netGetLocalObject__FP8Creature, objectFindInBox__FG6Point3T0PP10GameObjecti,
revealMap__3mapG6Point3, scriptActivatePostAction__Fv, setCamera__Ff, soundGetDialogFilename__Fv,
soundGetStreamPlayStatus__Fi, soundResetStream__Fi, willDraw__3mapP12_worldHeaderf, worldSetFarClip__Ff,
worldSetFrustum__Fffff, worldSetViewCenter__Fv, worldSetViewMatricies__FP6Point3P8Matrix34, and the libc names
atoi, memcpy, sprintf, strcat, strcmp, strcpy, strlen, strncmp (these may already be provided by other packages).

Soft double helpers, named by address like the other packages do: `norrath_softdouble_compare_2fba48` (original
dpcmp), `norrath_softdouble_multiply_2fb680` (dpmul, same name the ctors-lava-wraith package uses),
`norrath_softdouble_to_float_2fbc20` (dptofp, same name as ctors-lava-wraith) and the math package's
`norrath_entity_fptodp`. The integrator has to map these to the original routines or to authored ones.

Data owned by other packages and only referenced here: anim, animationLoadPos, animationLoading, automapButton,
coreLoading, coreLoadingScreenRunning, cutCameraPos, cutCurrentScene, cutInit, cutMode, cutSkipMode, cutTime,
dialogPacket, displayActionPos, displayDialogPos, displayFrame, dlgDir, domeLump, eRandState, engineDisplayDisable,
engineScissory1, engineScissory2, flags, frameNm, frontEndRunning, gameAutoMapMode, gameCam, gameControlPaused,
hudNoDraw, layer0Dir (used by the `currentLayerDir` initialiser), levelName, mapButtonWasDown, mapObjList, mapScale,
mapScissorX, mapScissorY, mapX, mapY, modelClippingOn, myFOV, myPitch, myRoll, myYaw, netLocalPad, netNetworkMode,
nmActions, nmDialogNodes, nmPlayers, pad, player, scaleScale, thereIsACutPlayer, validLocalPlayer, world,
worldCameraPos, drawFrameNm, drawHead, drawRecords, nmDrawUsed, writeBuffer.

`currentLayerDir` is initialised to `layer0Dir`. The cd-archive package describes the same pointer through its
`norrath_archive_directory_pointer_view`; if that package defines `currentLayerDir` itself, drop the definition here
to avoid a duplicate.

## Build-plan entries needed

Append in this order (10 authored inputs, empty `header_closure`, profile `authored_cpp`). The integrator assigns
`input_order` and regenerates the snapshot; `emitted_functions` is for the integrator to fill from its own build.
Three earlier sources (fade-up, start-switch-scene, draw-list-clear) are included.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "cut_clear.o",
  "profile": "authored_cpp",
  "sha256": "4345997fd2afbef54bd9d0977753cdeabf0bee2e09adaf77c8d7a988762dc5df",
  "source": "sources/w8-core-loop-b-cut-clear/cut_clear.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "cut_run_frame.o",
  "profile": "authored_cpp",
  "sha256": "663140a922d8f8d99d0c05ffaffd09bc09797aa45b5388ad67670a11f48ed745",
  "source": "sources/w8-core-loop-b-cut-run-frame/cut_run_frame.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "core_loop_data.o",
  "profile": "authored_cpp",
  "sha256": "827c6d465788dcc6e2b4e73fef59f6fb871e7e428fe9d1dc7e64892e96ef2779",
  "source": "sources/w8-core-loop-b-data/core_loop_data.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "do_new_world.o",
  "profile": "authored_cpp",
  "sha256": "b202cce884f547467516862122b70c46ca5b8736ba332673a848c55f4dba5833",
  "source": "sources/w8-core-loop-b-do-new-world/do_new_world.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "draw_auto_map.o",
  "profile": "authored_cpp",
  "sha256": "5127970140d40ea8547c4d085dfd132bf7aeff3002a372a78393477f85fef28d",
  "source": "sources/w8-core-loop-b-draw-auto-map/draw_auto_map.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "draw_dome.o",
  "profile": "authored_cpp",
  "sha256": "38ffca9e314d9c2b779e64fd99db904b1114073605cb72a96e74cff48627a9d0",
  "source": "sources/w8-core-loop-b-draw-dome/draw_dome.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "draw_list_clear.o",
  "profile": "authored_cpp",
  "sha256": "ce2e43d41b14f41059c3624e0acfb4d56c2bdc27d85ce2eef955782fbe527610",
  "source": "sources/w8-core-loop-b-draw-list-clear/draw_list_clear.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "fade_up.o",
  "profile": "authored_cpp",
  "sha256": "fe2741c7ddced140b86c5a3293adb189113421e4a236896d9001376c6e47c50d",
  "source": "sources/w8-core-loop-b-fade-up/fade_up.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "early_init.o",
  "profile": "authored_cpp",
  "sha256": "9c25058150bb3663565b890519a678329b5660feff6a5b440ce5c412a7636618",
  "source": "sources/w8-core-loop-b-map-early-init/early_init.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "start_switch_scene.o",
  "profile": "authored_cpp",
  "sha256": "98f82888c70399820d8700f5720026d6c5d7d4095351ba10cc652548d5061c7c",
  "source": "sources/w8-core-loop-b-start-switch-scene/start_switch_scene.cpp"
 }
]
```

## Authored but never executed

Everything here ran only under host mocks. Nothing was linked into an executable and nothing ran on the target
or an emulator: the soft double helpers, the map and camera calls, the cd and sound calls, and the lump and model
calls are all mocks. The decimal-to-bits claims for the truncated floats (0.1, 1/60, 1.6, 1.33 and the easing
thresholds) were checked against the file words by the host checks and the notes above, not by running the original.

## Unknown and not attempted

- The decoded semantics of the pad record, the dialog packet and `dlgDir` rows, and several flag bits.
- Whether the `drawBust`, `drawCompass` title and point arrays have element types other than bytes.
- `drawBust__Fv` and `drawCompass__Fv` use several of the data symbols above; they belong to another package.
