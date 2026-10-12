# Camera and vehicle methods (1/2)

Package notes for `agents/w8-camera-vehicle-a`. Addresses and extents come from
the private `SLUS_205.65` symbol table and disassembly read locally. They are
local evidence only. Host contracts (`host_check.cpp`) are not build-plan inputs.
Each function compiled alone with the reviewed `authored_cpp` profile; none of
the compiled bodies is claimed to match the original bytes. I did not run the
shared partial build, so which imports below were already unresolved on the
shared branch was not rechecked.

Every function is its own commit. Each host check was run, and for most I ran
one or more deliberately wrong variants of the source that the check rejected
(mutations of a constant, offset, comparison or call order). The unit-test suite
`python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).

## Observed

| Function | Address | Bytes | Unit directory |
|---|---|---|---|
| `GetYaw__6Camera` | `0x0026fc40` | 8 | `camera-yaw` |
| `GetPitch__6Camera` | `0x0026fc48` | 8 | `camera-pitch` |
| `SetYaw__6Camerai` | `0x0026faf8` | 8 | `set-yaw` |
| `SetPitch__6Camerai` | `0x0026fb00` | 8 | `set-pitch` |
| `SetMode__6Camerai` | `0x0026fa88` | 112 | `camera-set-mode` |
| `Push__6Camera` | `0x0026f9b0` | 108 | `camera-push` |
| `Update__6Camera` | `0x0026bfe0` | 284 | `camera-update` |
| `Reset__6Cameraii` | `0x0026be00` | 480 | `camera-reset` |
| `BuildMatrix__6CamerafffP6Point3` | `0x0026c190` | 828 | `camera-build-matrix` |
| `SetHold__7Vehiclei` | `0x00277b80` | 8 | `set-hold` |
| `Reset__5Wheel` | `0x002783a8` | 20 | `wheel-reset` |
| `doMyUniqueForces__7Vehicle` | base class | | `vehicle-unique-forces` |
| `AddNPCDriver__7VehicleP3NPC` | `0x00277ad8` | 40 | `add-npc-driver` |
| `GetNPCPosition__7VehicleP6Point3Ps` | `0x002779d0` | 84 | `get-npc-position` |
| `GetPositionAndYaw__7VehicleP6Point3T1Ps` | `0x00277a28` | 176 | `get-position-and-yaw` |
| `GetPlayerPosition__7VehicleiP6Point3Ps` | `0x00271168` | 336 | `get-player-position` |
| `Kick__7Vehicle` | `0x00277b88` | 108 | `kick` |
| `_$_7Vehicle` | `0x00270598` | 212 | `vehicle-destructor` |
| `_$_11LavaTractor` | `0x003166a8` | 100 | `lavatractor-destructor` |
| `_$_8SkulBoat` | `0x0031a1f0` | 100 | `skulboat-destructor` |
| `__tf7Vehicle` | `0x00316558` | 76 | `vehicle-typeinfo` |
| `__tf11LavaTractor` | `0x00316710` | 80 | `lavatractor-typeinfo` |
| `__tf8SkulBoat` | `0x0031a258` | 80 | `skulboat-typeinfo` |
| `doMyUniqueForces__8SkulBoat` | `0x002bf788` | 660 | `skulboat-unique-forces` |
| `_GLOBAL_$I$__7VehicleG6Point3sPPci` | `0x00278460` | 32 (+652 helper) | `vehicle-global-init` |

Directory names above are under `experimental/sources/w8-camera-vehicle-a-`.
The first eight rows' addresses for the first-session commits (yaw, pitch
accessors, SetHold, Reset__5Wheel, the base `doMyUniqueForces`) are recorded in
the comment at the top of each source.

Observed findings worth stating once:

- `Kick__7Vehicle` compares `levelName+81` (the data symbol `levelName` at
  `0x0034b660`) with "lava2". `game-boot` records the same offset compared with
  "lava3".
- `_$_11LavaTractor` stores the `_vt$11LavaVehicle` vtable, not a LavaTractor
  vtable, before calling the Vehicle destructor.
- `BuildMatrix` uses `gameCameraShake` (`0x00484f64`, float) and `eRandState`
  (`0x00483e58`): the shake advances `eRandState` with the multiplier `0x19660d`
  and increment `0x3c6ef35f`, and decays `gameCameraShake` by 0.5 per use.
- `rsqrt.s` and `cvt.w.s` are inline assembly on the EE and a portable model on
  the host, as in the existing `floor-world-view-set-frustum` and
  `light-methods-cosmetic-save` sources. The host contracts for those two
  functions use a tolerance (rsqrt) and round-to-nearest (cvt).
- In `BuildMatrix` the observed code multiplies by a stored zero and omits the
  other cross-product terms; the source keeps exactly those terms.
- `_GLOBAL_$I$__7Vehicle...` calls a file-local helper that constructs six
  `ParticleDef` objects. Their storage has no symbol; it is file-local BSS at
  an 80-byte stride in the source, and a 12-byte point is zeroed.

## Inferred

- The camera mode word at `+52` and the state word at `+72` select among
  `doNormalCam`, `doDebugCam` and `doInnorukCam`; the names of the modes and the
  meaning of the other camera fields are not established.
- `Push__6Camera` saves state in a slot at `+354..+376`; no matching restore
  was examined.
- The table at `vehicle+960` with count at `+976` is searched for a player
  value, and `vehicle+2088` holds a pointer table indexed the same way.
  `BuildMatrix`'s `sqrt` replacement in `doMyUniqueForces__8SkulBoat` (square sum above zero in place
  of the observed `sqrt.s` above zero) is an equivalence argument, not a
  comparison on the EE.

## Build-plan entries needed

All 25 units use profile `authored_cpp`, an empty `header_closure`, and kind
`independently_authored_source`. `input_order` is for the integrator. Rows are
source, object name, sha256 of the source.

| Source | Object | sha256 |
|---|---|---|
| `sources/w8-camera-vehicle-a-add-npc-driver/add_npc_driver.cpp` | `w8_cv_a_add_npc_driver.o` | `67e5c9a22693ef9ca950e17017200d65c1baebae0c16c44978f2a1a1e0341747` |
| `sources/w8-camera-vehicle-a-camera-build-matrix/build_matrix.cpp` | `w8_cv_a_camera_build_matrix.o` | `821c6abcd61064883c9a544fd9b7b644c5764d25be849fa0f71735da221b5c8c` |
| `sources/w8-camera-vehicle-a-camera-pitch/get_pitch.cpp` | `w8_cv_a_camera_pitch.o` | `fd89c793ab9735e14fac26fbe7b772294d0f48eb6ebf337dc868960a0a814736` |
| `sources/w8-camera-vehicle-a-camera-push/push.cpp` | `w8_cv_a_camera_push.o` | `98f1215d09ca0cd1cf7112e8fe8ff6bf2e0c97cac58b40c25b8e82c8673258c6` |
| `sources/w8-camera-vehicle-a-camera-reset/reset.cpp` | `w8_cv_a_camera_reset.o` | `6b693ca6b1fed405a9a1b113cbf13132deb7c651cd57362214c517ca40bcf2ed` |
| `sources/w8-camera-vehicle-a-camera-set-mode/set_mode.cpp` | `w8_cv_a_camera_set_mode.o` | `0dfda0b438aa3c9ef1dd02d5c531753e933c9006f74e4e91cf383ae3aee89693` |
| `sources/w8-camera-vehicle-a-camera-update/update.cpp` | `w8_cv_a_camera_update.o` | `5213fc1b22a2814ccb218dd56d8f34608843f414a31b2c5a4dad23ca9dbf16f2` |
| `sources/w8-camera-vehicle-a-camera-yaw/get_yaw.cpp` | `w8_cv_a_camera_yaw.o` | `ea04843e161df4af0592c3bdc63dbc4981b8b3dd16b268528bc5571324ecb14c` |
| `sources/w8-camera-vehicle-a-get-npc-position/get_npc_position.cpp` | `w8_cv_a_get_npc_position.o` | `68172be1e1f9b857ffe4ecfb0ace631a073e8a715db97099324c6d01f05d1de5` |
| `sources/w8-camera-vehicle-a-get-player-position/get_player_position.cpp` | `w8_cv_a_get_player_position.o` | `c0c39447dda297ea4f7e30993e71c86a231fc065a4eee796020f704724dc06af` |
| `sources/w8-camera-vehicle-a-get-position-and-yaw/get_position_and_yaw.cpp` | `w8_cv_a_get_position_and_yaw.o` | `409c278941e852674cff26c8373fa7fa218b0fe1966cce50808f146576399405` |
| `sources/w8-camera-vehicle-a-kick/kick.cpp` | `w8_cv_a_kick.o` | `b5e3b7f5b383a2342c52d0e6bd8a62d3b5ea117569d3781c6b3bbd33259a90ce` |
| `sources/w8-camera-vehicle-a-lavatractor-destructor/destructor.cpp` | `w8_cv_a_lavatractor_destructor.o` | `c9735a6c221822fc51dc773757edf626f6bb9d4df267a50769fe8501ae798bfe` |
| `sources/w8-camera-vehicle-a-lavatractor-typeinfo/typeinfo.cpp` | `w8_cv_a_lavatractor_typeinfo.o` | `eacef6831b07cc9c8e2dfcc13893eeef466db0ba9d060700c34cd6c0a04e6ecf` |
| `sources/w8-camera-vehicle-a-set-hold/set_hold.cpp` | `w8_cv_a_set_hold.o` | `8fa43eda3e9e47586529ab10430e53af65e668d26fc50473650509bb18fe3b96` |
| `sources/w8-camera-vehicle-a-set-pitch/set_pitch.cpp` | `w8_cv_a_set_pitch.o` | `2700137af450bddb31ac82d059849509033b8dfa182c0f8458f2d9465314f07d` |
| `sources/w8-camera-vehicle-a-set-yaw/set_yaw.cpp` | `w8_cv_a_set_yaw.o` | `8fe736b74e0d0c4d632947a0086f18ef1ebae6dd3dc59047ece297e2385d227b` |
| `sources/w8-camera-vehicle-a-skulboat-destructor/destructor.cpp` | `w8_cv_a_skulboat_destructor.o` | `fdebf98e3ecaaaf5deade4d4702d19ca1bea478211dcbe358d8ca7c5c0ac67ea` |
| `sources/w8-camera-vehicle-a-skulboat-typeinfo/typeinfo.cpp` | `w8_cv_a_skulboat_typeinfo.o` | `37213cb8f91776621b0783b5b6bd3765cd96433c97a645febc3d2b68d8564d34` |
| `sources/w8-camera-vehicle-a-skulboat-unique-forces/unique_forces.cpp` | `w8_cv_a_skulboat_unique_forces.o` | `3d208ee9ccb54da1323f2628e56dcefc786e188b0897e51971a2dbb04fa30b17` |
| `sources/w8-camera-vehicle-a-vehicle-destructor/destructor.cpp` | `w8_cv_a_vehicle_destructor.o` | `66e7b7e67f8f8a070ed2dd33cf845d21136863ff0c6a05f8df44457b2dd48e60` |
| `sources/w8-camera-vehicle-a-vehicle-global-init/global_init.cpp` | `w8_cv_a_vehicle_global_init.o` | `091db2b11bcd1d06bf2b4d505c53b09d329c7a8b88a38f2c54ae919ad7cc4244` |
| `sources/w8-camera-vehicle-a-vehicle-typeinfo/typeinfo.cpp` | `w8_cv_a_vehicle_typeinfo.o` | `a5b9d3e4dfaf235f91f30aaffb755fee89d1654d8e69560e0ff387cbac01d593` |
| `sources/w8-camera-vehicle-a-vehicle-unique-forces/unique_forces.cpp` | `w8_cv_a_vehicle_unique_forces.o` | `bd715202d919b3f8ccb627593e110855fa7bd05987463b2a2576f77f935d44d2` |
| `sources/w8-camera-vehicle-a-wheel-reset/reset_wheel.cpp` | `w8_cv_a_wheel_reset.o` | `dab70e61977498058a54e7b7df656b81997b0df3bd4fd0b84be30e0dc4e360d9` |

## Imports closed

`AddNPCDriver__7VehicleP3NPC`, `BuildMatrix__6CamerafffP6Point3`,
`GetNPCPosition__7VehicleP6Point3Ps`, `GetPitch__6Camera`,
`GetPlayerPosition__7VehicleiP6Point3Ps`, `GetPositionAndYaw__7VehicleP6Point3T1Ps`,
`GetYaw__6Camera`, `Kick__7Vehicle`, `Push__6Camera`, `Reset__5Wheel`,
`Reset__6Cameraii`, `SetHold__7Vehiclei`, `SetMode__6Camerai`,
`SetPitch__6Camerai`, `SetYaw__6Camerai`, `Update__6Camera`, `_$_11LavaTractor`,
`_$_7Vehicle`, `_$_8SkulBoat`, `_GLOBAL_$I$__7VehicleG6Point3sPPci`,
`__tf11LavaTractor`, `__tf7Vehicle`, `__tf8SkulBoat`,
`doMyUniqueForces__7Vehicle`, `doMyUniqueForces__8SkulBoat`. That is all 25
names of the package scope.

## Imports opened (referenced, left unresolved)

Not checked against the shared baseline, so some may already have been open.

- Functions: `DLG_Active__Fv`, `gameIsPaused__Fv`, `playerLocalPlayerBusy__Fv`,
  `matIdent__FP8Matrix34`, `matRot__FP8Matrix34is`, `matRot__FP8Matrix33is`,
  `matTransform__FP6Point3P8Matrix33T0`, `worldSetViewMatricies__FP6Point3P8Matrix34`,
  `worldPerturbWater__FP12_worldHeaderG6Point3ff`, `eRand__10GameObject`,
  `getSeatPosition__7VehicleiP6Point3`, `removePassenger__7Vehiclei`,
  `accumForceAndTorque__7VehicleP6Point3T1i`, `iatan2__Fff`,
  `revealEntireMap__3mapv`, `doNormalCam__6Camera`, `doDebugCam__6Camera`,
  `doInnorukCam__6Camera`, `SetZoomDistance__6Cameraf`, `GetZoomDistance__6Camera`,
  `listDestroy__FP8ListHead`, `_$_14AnimationState`, `_$_10GameObject`,
  `blockFree__FPv`, `__rtti_si`, `__tf10GameObject`, `__tf11LavaVehicle`,
  `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `strcmp`, `memset`.
- Data: `levelName`, `world`, `gameNoDraw`, `coreLoading`, `gameControlPaused`,
  `hudNetWaiting`, `hudDisplayNetCamToggle`, `gameCameraShake`, `eRandState`,
  `gameShowEnemiesInMap`, `worldCameraPos`, `nmPlayers`, `nmPlayersAlive`,
  `vehicleList`, `vehicleCount`, `__ti10GameObject`, `__ti11LavaVehicle`,
  `_vt$7Vehicle`, `_vt$8SkulBoat`, `_vt$11LavaVehicle`.

`__ti7Vehicle`, `__ti8SkulBoat` and `__ti11LavaTractor` are now defined here (12
zero bytes of BSS each; the unit that defines one imports it where a derived
unit needs it: `skulboat-typeinfo` imports `__ti7Vehicle`).

## Authored but never executed

All of it. Nothing ran on the EE or in an emulator. Host contracts execute the
authored C++ against stubs on the developer machine, and compare with expected
values written from the reading above. They prove that the source does what the
notes say, not that the notes match the original program.

## Unknown

- What the camera modes, the camera field at `+80`..`+240`, and most vehicle
  fields mean.
- The cvt.w.s rounding mode in effect at run time on the EE (the host models
  round to nearest).
- The numeric result of `rsqrt.s` on the EE against the host's `1/sqrt`.
- Whether the six `ParticleDef` objects built by `_GLOBAL_$I$` are read by other
  Vehicle code; their symbols are not in the table, so they stay file-local here
  and any other reader needs a named definition from the owner.
- The `ParticleDef` object size (80 bytes is only the stride) and the meaning of
  its constructor arguments.
- Unresolved ownership: `__tf11LavaVehicle`, `__ti11LavaVehicle` and the
  `_vt$...` vtables belong to the vehicle-actors package or the integrator.
