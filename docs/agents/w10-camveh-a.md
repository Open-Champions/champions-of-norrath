# w10-camveh-a: camera and vehicle (Camera to Wheel)

This package reconstructs the remaining Camera and Vehicle routines and the small storage objects that the authored w9-vehicle, w9-frontend and w9-camera sources import. Every unit under `experimental/sources/w10-camveh-a-*` compiles with the reviewed EE `authored_cpp` flags and has a host check (`host_check.cpp`, not a plan input). Every function was also run side by side with the original in a private R5900 interpreter (outside git, loads the private image; nothing derived from it is committed). A paired run starts the original and the authored function from identical memory with identical stubbed callees and compares the return value, every byte written outside the stack, and the ordered call log with argument values. All runs ended with zero mismatches and no halted runs. None of this was run on a real EE, and none of it has been linked into the partial object: `experimental/build-plan.json` and `experimental/snapshot.json` were not touched, so the units below are not in any build until the lead adds the entries at the end of this note.

## Functions

Byte-exact output is not claimed; the sizes are the original body size and the size of the reviewed compile, in bytes.

| Function | Original at | Original / authored bytes | Notes |
|---|---|---|---|
| `checkYawReset__6Camera` | 0x0026ed68 | 296 / 328 | gates on levelFlags bit 0x2000000, camera words 56/100 and byte 352, a pad flag, and a stick magnitude below a limit; then picks a yaw from the player target and asks `fastestRotDirection__FssPi` for a direction. |
| `determineBestController__6Camera` | 0x0026f528 | 628 / 796 | prefers the current controller (word at +68) when its stick moves or its flag 0x400 is set, else the listed pad with the largest stick sum (soft doubles). |
| `Load__6CameraPc` | 0x0026c5d8 | 408 / 332 | inverse of `Save__6CameraPc`; returns 196 and derives the floats at +216 and +252. |
| `doUserZoom__6Camera` | 0x0026e290 | 1280 / 1264 | zoom blend, HUD fade, step zoom in/out from the stick. |
| `doUserYaw__6Camera` | 0x0026e790 | 1492 / 1604 | turn rate from the stick, local player or combat follow, with clamp and damping. |
| `doAutoPitch__6Camera` | 0x0026daa8 | 2020 / 1992 | casts five rays per listed player, finds the first blocked index, and eases the pitch halfword. |
| `obtainTarget__6CameraP6Point3` | 0x0026ee98 | 1672 / 1748 | camera target for one player, the spectated player, a vehicle, or a framing of all listed players. |
| `resolveCreatureCollisions__7Vehicle` | 0x00276590 | 1476 / 1280 | carries attached creatures, pushes overlapping ones and reacts with a virtual call. |
| `worldTransformFrustum__FP8Matrix34` | 0x00177a48 | 1156 / 1008 | transforms the view and clip frustum normals by a matrix. |

The 17 units authored earlier in this package (`GetSaveFootPrint`, `enforceMaxSpeed`, `setMassAndIBody`, `rayHit`, `changeOrientation`, `getEngineTorque`, `scalePatchLimit`, `doDifferential`, `getParticleVelocity`, `addWheel`, `backupJustForces`, `getBlockingVehicle`, `__17VehicleBodySphereP7Vehicleff`, `FinalizePosition__5Wheel`, `buildCollisionBox__7Vehicle`, `impulse__7VehicleP6Point3N21fii`, `dualImpulse__7VehicleP7VehicleT1P6Point3T3`) are described in their own source headers.

## Storage and tables

| Symbol | Address | Bytes | Section | Evidence |
|---|---|---|---|---|
| `gameMinCamHeight`, `gameMaxCamHeight` | 0x00484cc8, 0x00484ccc | 4 + 4 | .sdata | observed: image bytes are -100000.0f and +100000.0f; min precedes max |
| `originalPos` | 0x0042e2d0 | 48 | .data | observed size and zero bytes; element type unknown (byte array) |
| `originalYaw` | 0x00485bf0 | 8 | .sdata | observed size and zero bytes; element type unknown (byte array) |
| `worldClipFrustum` | 0x006eb9f8 | 96 | .bss | observed: exact distance to the next symbol; 24 floats |
| `pitchTab` | 0x006cb078 | 512 | .bss | inferred: the only reader clamps its index to 0..127; the distance to the next symbol is 520 |
| `clipFrustumNormalSign` | 0x006ec7a8 | 18 | .bss | inferred from 6 planes x 3 axes; the distance to the next named symbol is 88 |
| `_vt$4Boat` | 0x00453328 | 248 | .data (weak) | observed: 30 slots of (adjustment 0, function address) then a zero slot; slot names are the ELF function symbols at the stored addresses, checked against the compiled relocations |

## Things that went wrong, and what to know

- Decimal float literals are not safe with this compiler. It rounds a decimal such as `0.03199999779462814f` one bit low (0x3d03126d instead of 0x3d03126e). Constants that come from the image are written as hexadecimal floats with the bits in a comment. A first version of `checkYawReset__6Camera` had the wrong limit; the fix is its own commit, and the paired run now sweeps inputs around the limit so that it would fail.
- The EE compiler assumes no NaN: it turns `!(a < b)` into `a <= b`. The original (and the real EE) have no NaN either. The private interpreter does, so fixtures avoid divisions by zero that would make one.
- The soft double routines `dpmul`, `litodp` and others use MMI that the interpreter does not implement, so in the `doAutoPitch__6Camera` run they are replaced by Python equivalents. Their calls and arguments are still compared.
- Integer division by a byte field in `resolveCreatureCollisions__7Vehicle` compiles with a zero check that the original does not have; the results are identical for nonzero divisors and the original's divisor-zero result is unspecified.
- A host check cannot model 32-bit pointers inside game structures on a 64-bit host. `resolveCreatureCollisions__7Vehicle` therefore reads its structure pointers through `LOAD_PTR`/`LOAD_CALL` macros that default to the plain 32-bit reads and are replaced by the host check.

## Observed, inferred, unknown

Observed: every behaviour listed in the source header comment of each unit, from the original instructions and the paired runs. Offsets stay numeric because the meanings are not established.

Inferred: names only where the original symbol names say so (a camera target, a pad entry, a frustum plane). The sizes of `pitchTab` and `clipFrustumNormalSign`. The `Boat` slot 15 being `clone__10GameObject` (same address as the Vehicle table).

Unknown: what the camera fields at +56, +60, +100, +144, +152, +156, +204..+216 and +252 mean; what the virtual method called from `resolveCreatureCollisions__7Vehicle` (vtable slot at +16 of the creature's table) is; the element types of `originalPos` and `originalYaw`; the real sizes of `pitchTab` and `clipFrustumNormalSign`; what `playersAlive` (0x0042a3e8, indexed 0 in `obtainTarget`) holds in relation to `localPlayersAlive`.

## Imports

Imports closed (defined by units here): clipFrustumNormalSign, gameMaxCamHeight, gameMinCamHeight, originalPos, originalYaw, pitchTab, worldClipFrustum, worldTransformFrustum__FP8Matrix34, GetSaveFootPrint__6Camera, Load__6CameraPc, checkYawReset__6Camera, determineBestController__6Camera, doAutoPitch__6Camera, doUserYaw__6Camera, doUserZoom__6Camera, enforceMaxSpeed__6CameraP6Point3, obtainTarget__6CameraP6Point3, _vt$4Boat, addWheel__7VehiclePciffff, backupJustForces__7Vehicle, buildCollisionBox__7Vehicle, changeOrientation__7VehicleG6Point3, doDifferential__7Vehicle, dualImpulse__7VehicleP7VehicleT1P6Point3T3, getBlockingVehicle__7VehiclePf, getEngineTorque__7Vehicle, getParticleVelocity__7VehicleP6Point3, impulse__7VehicleP6Point3N21fii, rayHit__7VehicleP6Point3T1fT1, resolveCreatureCollisions__7Vehicle, scalePatchLimit__7VehicleiP6Point3, setMassAndIBody__7Vehicle, __17VehicleBodySphereP7Vehicleff, FinalizePosition__5Wheel. That is every name in the package scope.

Imports these units open (names that other packages must define): GetNPCPosition__7VehicleP6Point3Ps, GetPlayerPosition__7VehicleiP6Point3Ps, GetPositionAndYaw__7VehicleP6Point3T1Ps, MC_SavingOrLoading, ModifyRPM__5Wheelf, Reset__6Cameraii, Save__6CameraPc, SetNaturalRPM__5Wheel, _$_4Boat, __10GameObjectG6Point3PQ210GameObject11resourceMapPPc, __5WheelPcifP7Vehiclei, __tf4Boat, _vt$17VehicleBodySphere, addImpulse__7VehicleP6Point3T1i, automapButton, backup__7Vehicle, blockAlloc__Fi, clampZoomMag__6Camera, clipViewFrustum, clone__10GameObject, distanceToPlane__FP6Point3N20, doMyUniqueForces__4Boat, dpadd, dpcmp, dpmul, dpsub, dptofp, engineSetBlur__Fi, evalHermite__Ffffff, event* (eventActivate, eventDeActivate, eventGasOn, eventGasOff, eventCurRPM, eventCollisionGround, eventCollisionWall, eventSteeringChange; all `__7Vehicle`), fadeIconY, fadeTimer, farClip, fastestRotDirection__FssPi, fptodp, frameNm, frustumNormalSign, gameCombatCamera, gameControlPaused, gameDeathTimerSingle, gameReverseCameraYaw, getModel__10GameObject, halfFrustumMode, hudDisplayNetCamToggle, hudGetFadeIconPos__Fii, iatan2__Fff, levelFlags, listDestroy__FP8ListHead, listOnewayLink__FP8ListHeadT0Pv, litodp, localNmPlayersAlive, localPlayersAlive, matIdent__FP8Matrix33, matRot__FP8Matrix33is, matTransform__FP6Point3P8Matrix33T0, matTransform__FP6Point3P8Matrix34T0, matUntransform34Vec__FP6Point3P8Matrix34T0, matUntransform__FP6Point3P8Matrix33T0, the msg_* routines named in the Boat table, netGetLocalPlayerNm__Fi, netNetworkMode, netPlayerIsLocal__Fi, nmPlayers, nmPlayersAlive, objectFindInBox__FG6Point3T0PP10GameObjecti, objectIgnoreFlags, objectMoveWithWorldCollision__FP6Point3G6Point3fiT0, objectUpdateInGrid__FP10GameObject, pad, player, playersAlive, powf, quatMul__FP4QuatN20, quatNormalize__FP4Quat, quatToMat__FP8Matrix33P4Quat, set__4Boat, shite, vehicleCount, vehicleList, viewFrustum, world, worldCameraMatrix, worldCameraPos, worldFindHeight__FP12_worldHeaderG6Point3f, worldFrustum, worldHitscan__FP12_worldHeaderG6Point3T1fi, worldTestForbidden__FP12_worldHeaderG6Point3f. (`viewFrustum`, `clipViewFrustum` and `frustumNormalSign` are 72, 72 and 18..24 bytes of storage whose owner is another package.)

## Build-plan entries needed

None of these sources is in `experimental/build-plan.json`. Each needs one input entry (the `input_order` is for the integrator to assign, `files` hashes for each source and the `snapshot_sha256` change with it). The `sha256` values are of the files at the last commit of this branch; `emitted_functions` and `emitted_data` are the symbols the unit's compiled object defines.

```json
[
 {
  "source": "sources/w10-camveh-a-add-wheel/add_wheel.cpp",
  "sha256": "33f2baaedffaad7f50c829e524a6d085c8676ee398e4f012df370acec9d4046f",
  "object_name": "add_wheel.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "addWheel__7VehiclePciffff"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-backup-just-forces/backup_just_forces.cpp",
  "sha256": "7571f7fa9aebbea17c044439538e65c166595e2ba6cddcd56a6b537f9d7a0f71",
  "object_name": "backup_just_forces.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "backupJustForces__7Vehicle"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-build-collision-box/build_collision_box.cpp",
  "sha256": "2df0ee18da98adec6658f403301b4a1ef0f9f1d777766d0a5cd250f1d7dedd1b",
  "object_name": "build_collision_box.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "buildCollisionBox__7Vehicle"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-camera-load/camera_load.cpp",
  "sha256": "803653dac5a75499dc7d2c36581a955ca6300f7150ddc9b881716009567149bf",
  "object_name": "camera_load.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "Load__6CameraPc"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-change-orientation/change_orientation.cpp",
  "sha256": "e01b6fe4d3c47499d5433b88e2ea444e21439957b8a41b75a04875604f9b6e77",
  "object_name": "change_orientation.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "changeOrientation__7VehicleG6Point3"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-check-yaw-reset/check_yaw_reset.cpp",
  "sha256": "7eab8585e85cb2bf0adbe10654410cbb2233889c0f49cb5a26c71707112f1f57",
  "object_name": "check_yaw_reset.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "checkYawReset__6Camera"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-determine-best-controller/determine_best_controller.cpp",
  "sha256": "6e4a79cf0285ddcf94be2f3a6642ff7d3ab806633da1381b0ce3a57197ce07d3",
  "object_name": "determine_best_controller.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "determineBestController__6Camera"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-do-auto-pitch/do_auto_pitch.cpp",
  "sha256": "49f199d6c62229105ce8df8986e0bbea9985b53cc6220b6db0710e6981d4888c",
  "object_name": "do_auto_pitch.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "doAutoPitch__6Camera"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-do-differential/do_differential.cpp",
  "sha256": "54735434d35bb4f3918002dadf8234f87bdc4f8185fca495983ef8c584211a6b",
  "object_name": "do_differential.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "doDifferential__7Vehicle"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-do-user-yaw/do_user_yaw.cpp",
  "sha256": "c144140d0586983fb1f2158a02ab638cba47625e4e481f7be33a83f3e768f94e",
  "object_name": "do_user_yaw.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "doUserYaw__6Camera"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-do-user-zoom/do_user_zoom.cpp",
  "sha256": "e30bf896fd8708e4df808b9b1dceb40bd44c0ef10129530c5b28d39c73b07f9b",
  "object_name": "do_user_zoom.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "doUserZoom__6Camera"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-dual-impulse/dual_impulse.cpp",
  "sha256": "46d5a555488122b98a98c5ba71741a5f34235af5a95fcd115b4375bb5fac92e3",
  "object_name": "dual_impulse.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "dualImpulse__7VehicleP7VehicleT1P6Point3T3"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-enforce-max-speed/enforce_max_speed.cpp",
  "sha256": "5706950ce949005a8503ed3387279c4b85d66e08527ce9b4432c1eec95816642",
  "object_name": "enforce_max_speed.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "enforceMaxSpeed__6CameraP6Point3"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-get-blocking-vehicle/get_blocking_vehicle.cpp",
  "sha256": "f6af66e69f91006b0320fce45ecb5241ae61469674ae1daaa58b320d89c89de9",
  "object_name": "get_blocking_vehicle.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "getBlockingVehicle__7VehiclePf"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-get-engine-torque/get_engine_torque.cpp",
  "sha256": "d2047dbb887fa04210864325e8449298493bb2bc71363707d444164063da22bf",
  "object_name": "get_engine_torque.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "getEngineTorque__7Vehicle"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-get-particle-velocity/get_particle_velocity.cpp",
  "sha256": "788da19d0cb7d8b36278be88f01256959b5789bbcdc490a7f56027cfa22bc106",
  "object_name": "get_particle_velocity.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "getParticleVelocity__7VehicleP6Point3"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-get-save-foot-print/get_save_foot_print.cpp",
  "sha256": "c27c5463df7dc68e03350ea1a2a269c0b139f6878c4c005d2a7ff87e0e05512d",
  "object_name": "get_save_foot_print.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "GetSaveFootPrint__6Camera"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-impulse/impulse.cpp",
  "sha256": "3d5b769bd95d348ebc78f3ee537a35ac0ba631cfc839bda42a168fded4ec32e5",
  "object_name": "impulse.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "impulse__7VehicleP6Point3N21fii"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-obtain-target/obtain_target.cpp",
  "sha256": "ba3f818aa7e4246419509c4a381a8e1efed8bea91918bd5f1d6d39121616444a",
  "object_name": "obtain_target.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "obtainTarget__6CameraP6Point3"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-ray-hit/ray_hit.cpp",
  "sha256": "4b13d92b551e99ddd4dea51ff2424adf0a1245f2ebfa85728629c453b59c1a36",
  "object_name": "ray_hit.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "rayHit__7VehicleP6Point3T1fT1"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-resolve-creature-collisions/resolve_creature_collisions.cpp",
  "sha256": "8fee82378f14e2c036b1282a573840183cd455d2ca40e995483123f758434ae5",
  "object_name": "resolve_creature_collisions.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "resolveCreatureCollisions__7Vehicle"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-scale-patch-limit/scale_patch_limit.cpp",
  "sha256": "8b85d80204223318c97885db911f10f07e899eacdbadda76869e418e07c2b778",
  "object_name": "scale_patch_limit.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "scalePatchLimit__7VehicleiP6Point3"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-set-mass-and-ibody/set_mass_and_ibody.cpp",
  "sha256": "4cd1db030b98da53424fb3824e4ff8411aa68abe91eee9a89656c3860085d31e",
  "object_name": "set_mass_and_ibody.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "setMassAndIBody__7Vehicle"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-storage-cam-height/storage.cpp",
  "sha256": "57ea782f351f1bc762519a4723a78ad763303800af3f6afba7a977e1e3f122b2",
  "object_name": "storage.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [],
  "emitted_data": [
   "gameMaxCamHeight",
   "gameMinCamHeight"
  ]
 },
 {
  "source": "sources/w10-camveh-a-storage-clip-normal-sign/storage.cpp",
  "sha256": "bb3fc8dd22e5f6b1534975ce82b9edf802d4569c8dfaf4d6d5f46323f0416812",
  "object_name": "storage.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [],
  "emitted_data": [
   "clipFrustumNormalSign"
  ]
 },
 {
  "source": "sources/w10-camveh-a-storage-original-net/storage.cpp",
  "sha256": "614d14f538cf651ce71e2dfe57cdcb5750809df5b9ec92e94353cb3cd3fc68f1",
  "object_name": "storage.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [],
  "emitted_data": [
   "originalPos",
   "originalYaw"
  ]
 },
 {
  "source": "sources/w10-camveh-a-storage-pitch-tab/storage.cpp",
  "sha256": "a0412434ec608172c3e8df8fd8ffd1c259906640202e505bdf69b092b81c9b3f",
  "object_name": "storage.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [],
  "emitted_data": [
   "pitchTab"
  ]
 },
 {
  "source": "sources/w10-camveh-a-storage-world-clip-frustum/storage.cpp",
  "sha256": "0a3b8075dcc0e0ec607a7b3ac017caf1ae1df3a29b46a22de218aa5641db8ed4",
  "object_name": "storage.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [],
  "emitted_data": [
   "worldClipFrustum"
  ]
 },
 {
  "source": "sources/w10-camveh-a-vehicle-body-sphere/vehicle_body_sphere.cpp",
  "sha256": "b38f1ee3e96dcbb40fd5425b6a9cdf8bc7ae9410e55a7699fb29e3f587838485",
  "object_name": "vehicle_body_sphere.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "__17VehicleBodySphereP7Vehicleff"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-vt-boat/vt_boat.cpp",
  "sha256": "6c8956661d5ff2a60cf33507ea71e5507714871f30a8d9026532019a829eaab9",
  "object_name": "vt_boat.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [],
  "emitted_data": [
   "_vt$4Boat"
  ]
 },
 {
  "source": "sources/w10-camveh-a-wheel-finalize-position/finalize_position.cpp",
  "sha256": "28b63cec31e8e4053b47287abfc3438a9b1b6082df6f5a8d2ae0fe1ca3082ee9",
  "object_name": "finalize_position.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "FinalizePosition__5Wheel"
  ],
  "emitted_data": []
 },
 {
  "source": "sources/w10-camveh-a-world-transform-frustum/world_transform_frustum.cpp",
  "sha256": "bb55f95fc32a301c764c4c5b1c77e62a6568179a4c6e1f62d61df850b731ad3e",
  "object_name": "world_transform_frustum.o",
  "kind": "independently_authored_source",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   "worldTransformFrustum__FP8Matrix34"
  ],
  "emitted_data": []
 }
]
```

## Authored but never executed

All of it. Nothing ran on an EE or in the game. The functions were compared with the original function in a private interpreter under stubbed callees and one random memory image per scenario (between 800 and 6000 scenarios each, plus targeted ones for rarely reached branches). Callees (`worldHitscan`, `objectFindInBox`, the matrix helpers, `powf`, the soft double routines, `iatan2`, `listOnewayLink` and others) were replaced by deterministic stubs, so their real behaviour is not covered. The remaining uncovered original words in the runs were delay-slot nops, annulled slots, and in `obtainTarget__6CameraP6Point3` the branch for a negative `localNmPlayersAlive`, which cannot happen with a count. The storage objects and `_vt$4Boat` have no behaviour to run; they were checked against the image bytes and, for the vtable, against the relocations of the compiled object.

## Blockers

None that needs the owner. `pitchTab` and `clipFrustumNormalSign` have inferred sizes, and the two RAM-slot arrays have unknown element types; the owner of the net-save code may want to refine them.
