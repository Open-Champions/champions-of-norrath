# Vehicle, boat and tractor (w9-vehicle)

Branch `agents/w9-vehicle`. Package: the 27 unresolved imports of the partial build
that belong to Vehicle, Boat, SkulBoat and LavaTractor behaviour, plus the two
Vehicle globals. Private reference: `work/private/SLUS_205.65` of the main
checkout, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`
(the hash the vehicle-actors notes record for the same file). Addresses below are
that file's symbol values. No game bytes, disassembly listings or decompiler output
are committed; everything here is independently written C++ and prose.

## How each function was checked

- **Compile.** Every unit compiles with the reviewed EE g++ and the plan's
  `authored_cpp` flags (`-G0 -O2 -ffreestanding ... -msingle-float -std=c++17`),
  one unit at a time. No full partial build was run.
- **Host contract.** Each unit has a `host_check.cpp` (not an EE input) whose
  `main` returns non-zero when a store, a call argument, the call order or a
  result is wrong. Several were mutation-checked (a changed constant or branch
  makes them fail). Host pointers are 8 bytes wide while the EE's are 4, and many
  of these objects keep pointers in 4-byte-stride arrays; the host checks of those
  functions therefore use one entry, or (doAI, doDriveline) map their pointed-to
  objects at addresses whose halves line up (0x300000000 and 0x400000003,
  using `mmap`). That is a limit of the host check, not of the source.
- **Paired run against the original image.** In a private R5900 interpreter
  (outside git) each authored function was compiled, linked with every undefined
  symbol bound to the original's address, and run next to the original function
  from identical memory snapshots with identical Python stand-ins for the callees.
  A scenario passes only when the call logs (callee, argument words, words behind
  pointer arguments, float arguments), the return value where one exists, and
  every changed byte outside the stack are identical. Scenario counts and the
  fraction of the original's instructions executed are in the table; the only
  original instructions never executed are alignment nops, two clamps that cannot
  be reached with finite inputs (`doDriveline` keep > 1, `buildCollisionInfo` the
  unsigned-conversion adjustment for values at or above 2^31), and a NaN branch.
  The interpreter's float model is double arithmetic rounded to single, with
  NaNs made canonical; it is not the R5900 FPU, which has no NaN and rounds
  `rsqrt.s`/`div.s` differently in the low bits. Scenarios that produced infinity
  or NaN were therefore not treated as evidence.
- **Not byte matches.** Compiled sizes differ from the originals (compare the
  table). These are behaviour reconstructions; nothing claims instruction
  equivalence or a percentage.

## Toolchain facts found on the way

- Decimal float literals can be one unit in the last place off in this compiler
  (a literal for the word 0x3f8ccccc compiled to ...cb). Every float with a
  fraction is therefore written as a hexadecimal float literal.
- R5900 `sqrt.s` takes its source from the `ft` field in this assembler's
  encoding; `rsqrt.s`, `min.s` and `max.s` are written as inline assembly under
  `#if defined(__mips__)` with a portable fallback for the host contract, as the
  existing `cvt.w.s` helpers in other units do. `__builtin_sqrtf` compiles to a
  plain `sqrt.s`.
- `doDriveline` calls the original's soft double routines (`fptodp`, `dpcmp`,
  `dpsub`, `dpmul`, `dptofp`, `acos`, `sqrt`; doubles travel in one 64-bit
  register) by name rather than using `double` arithmetic, which would emit
  different library calls. `acos` and `sqrt` are declared with assembler labels
  so they do not collide with the compiler's built-in prototypes.

## Functions

| source | symbol | original address, bytes | compiled bytes | paired scenarios / original instructions covered | sha256 |
|---|---|---|---|---|---|
| `w9-vehicle-accum-force-and-torque/accum_force_and_torque.cpp` | `accumForceAndTorque__7VehicleP6Point3T1i` | `0x00271c90`, 468 | 284 | 480 / 117 of 117 | `5548726b6d235e76119b6a67c323ff718223d65ef29fec70f5188c048a6f0d75` |
| `w9-vehicle-add-passenger/add_passenger.cpp` | `addPassenger__7Vehiclei` | `0x00271488`, 544 | 636 | 360 / 134 of 136 | `346a3b25637d754ddfd97fce0d2ee0ed9f903aa4d193742a8106c0091e0ad1c8` |
| `w9-vehicle-backup/backup.cpp` | `backup__7Vehicle` | `0x00272cc0`, 352 | 300 | 150 / 88 of 88 | `d17ab32ec7516f7bc47a172c22d6c391d21c49abb03977adc377cfd3da7cfd3e` |
| `w9-vehicle-boat-destructor/destructor.cpp` | `_$_4Boat` | `0x0031a138`, 100 | 88 | 7 / 24 of 25 | `aaa0bf2f72fd952869835bc785a5e29e81af4b27ae372060bcbc9b8193a0f9b6` |
| `w9-vehicle-boat-set/boat_set.cpp` | `set__4Boat` | `0x002bf290`, 328 | 360 | 6 / 82 of 82 | `f5fbaa66e7116c3bb413f6842e5a78d22ec1a73b7d4cee9e9a4a7051c4ec1885` |
| `w9-vehicle-boat-unique-forces/unique_forces.cpp` | `doMyUniqueForces__4Boat` | `0x002bf3d8`, 616 | 540 | 140 / 154 of 154 | `86d2f926a5b9e2501dc7f6372ff4e2a5a6efa4555e5ce13eef81015301fe830e` |
| `w9-vehicle-build-collision-info/build_collision_info.cpp` | `buildCollisionInfo__7Vehicle` | `0x00276b58`, 1136 | 960 | 500 / 278 of 284 | `9bbd636975b4b57c1637746cbf4baed4121a20e6d225c71987025e3a68840081` |
| `w9-vehicle-check-vehicle-collision/check_vehicle_collision.cpp` | `checkVehicleCollision__7Vehicle` | `0x002738a8`, 556 | 552 | 500 / 139 of 139 | `919792fc015919e2c7253091cc0e031a1f134bc5dd751dd36c7f6d5127cbb672` |
| `w9-vehicle-check-wall-collision/check_wall_collision.cpp` | `checkWallCollision__7Vehicle` | `0x002733d0`, 1236 | 1020 | 1500 / 307 of 309 | `826d4aadbec1c6837e02775078bb8d936c56eab01698e4e291e0cd4ab90e2be9` |
| `w9-vehicle-do-ai/do_ai.cpp` | `doAI__7Vehicle` | `0x002749e0`, 560 | 612 | 800 / 140 of 140 | `d923ac5a97c9134ddb6d59529a8323c4def91352a1bd905cf9fc9b07e19d385e` |
| `w9-vehicle-do-brakes/do_brakes.cpp` | `doBrakes__7Vehicle` | `0x002748a0`, 316 | 364 | 600 / 78 of 79 | `749c9dfef0d2411d8914c6cb49cc4084539e9536bd06ff52f6d7bf47e29e5ece` |
| `w9-vehicle-do-driveline/do_driveline.cpp` | `doDriveline__7Vehicle` | `0x00275298`, 3712 | 3200 | 1500 / 924 of 928 | `ba4080659dae6ab72fbc9ff5d8211b0d49b263b23acf2c0b01b071aaef3a1ef0` |
| `w9-vehicle-do-misc/do_misc.cpp` | `doMisc__7Vehicle` | `0x00273ad8`, 1288 | 1348 | 1500 / 321 of 322 | `2ba513ca9062addeaf0eeaba7a1630d668de24635ea68573451bcf8ec8036ecc` |
| `w9-vehicle-do-steering/do_steering.cpp` | `doSteering__7Vehicle` | `0x00274dd0`, 1220 | 1516 | 1500 / 301 of 305 | `7feb0fd15271e9c8af8ff98597db7d091f6f40d0d1eb4d548a29ebb16f8f55b8` |
| `w9-vehicle-do-suspension/do_suspension.cpp` | `doSuspension__7Vehicle` | `0x00273fe0`, 1448 | 1244 | 800 / 362 of 362 | `31947982cea618860cb22fd4db87c5096692ff55203af01b28a62b5a7daac76f` |
| `w9-vehicle-do-throttle/do_throttle.cpp` | `doThrottle__7Vehicle` | `0x00274588`, 788 | 888 | 1500 / 196 of 197 | `a247bca27736e8b4a263ab18890c6c587b2a00816a3af3e9ab817005dd89713d` |
| `w9-vehicle-finalize-position/finalize_position.cpp` | `finalizePosition__7Vehicle` | `0x00272f70`, 1116 | 912 | 400 / 278 of 279 | `8210d4864f4556948fa2d9b7863943c4526643ad84e26447f38f0a0b7887530a` |
| `w9-vehicle-get-new-rot-and-pos/get_new_rot_and_pos.cpp` | `getNewRotAndPos__7VehicleP6Point3T1fii` | `0x00271e68`, 1104 | 908 | 300 / 276 of 276 | `4b2c06c40a9ad382d03457dd74dc09518e05bf9c2b2eb12438e5b78e475e366f` |
| `w9-vehicle-get-seat-position/get_seat_position.cpp` | `getSeatPosition__7VehicleiP6Point3` | `0x002712b8`, 460 | 528 | 600 / 114 of 115 | `033f25d7f3e87d88718e3cbe78ad416522b731e49f084f343bfe46aa2282fe92` |
| `w9-vehicle-globals/vehicle_globals.cpp` | `vehicleCount` | `0x00485e40`, 4 | 4 | data only | `a277cd748a40e79368fdeedc7636a9dc102d1c1c19bc93ab9ea1e9696ab58650` |
| `w9-vehicle-globals/vehicle_globals.cpp` | `vehicleList` | `0x00436fa8`, 128 | 128 | data only | `a277cd748a40e79368fdeedc7636a9dc102d1c1c19bc93ab9ea1e9696ab58650` |
| `w9-vehicle-have-room/have_room.cpp` | `haveRoom__7Vehicle` | `0x00277ce0`, 52 | 72 | 200 / 13 of 13 | `3c2dabc14a429c63837eeead15145ec564135837abed5013a3188781fcc7fb7d` |
| `w9-vehicle-is-passenger/is_passenger.cpp` | `isPassenger__7Vehiclei` | `0x00277d18`, 72 | 64 | 360 / 18 of 18 | `595b15f9a80473cb5c5e83a100bbb861b8879f88419df1e1f4491b5db7b3ff0c` |
| `w9-vehicle-lavatractor-set/lavatractor_set.cpp` | `set__11LavaTractor` | `0x00284918`, 620 | 648 | 6 / 155 of 155 | `65971286e4ad7e8ad39a08d4c4fe2a5f68fca5801e519af1134a211b1b20d4ea` |
| `w9-vehicle-remove-passenger/remove_passenger.cpp` | `removePassenger__7Vehiclei` | `0x002716a8`, 456 | 416 | 480 / 114 of 114 | `80dd1a7cfe4516b39ebb89db6755ddb2cb3cc1f78c909614a0f7203cace8c1d7` |
| `w9-vehicle-skulboat-set/skulboat_set.cpp` | `set__8SkulBoat` | `0x002bf640`, 328 | 360 | 6 / 82 of 82 | `3cb8307fc3e882a0c6ed1c4aa312a8b2f4fc6d45fb43a62629cd332d98407daa` |
| `w9-vehicle-start-forces/start_forces.cpp` | `startForces__7Vehicle` | `0x00277e10`, 32 | 32 | 5 / 8 of 8 | `e91885cdcda43c3f4b83f154cfe771971aa0ac0d2a6953d6d9a2c983bd411a67` |

Callee pointers/vectors: in the original, Point3 values wider than a register are
passed by hidden reference; the authored declarations pass the address of a
caller-owned copy and return Point3 results through an explicit first pointer
(`getParticleVelocity`, `objectFindEmptySpot`).

## Observed

Field meanings are not established anywhere in this package; offsets stay numeric.
What the code shows:

- `vehicleCount` is a 4-byte word (0x00485e40) and `vehicleList` is 128 bytes
  (0x00436fa8), both zero in the image. The vehicle at index `vehicleCount - 1`
  (its own index is the word at +2156) is the one that runs the pairwise vehicle
  test.
- Four seat words at +960 (−1 = free), occupied count at +980, seat flags at +984,
  driver index at +956 (−1 = none), passenger capacity at +976, and player records
  (`player + 23984*i`, pointer at +21872) are used by `addPassenger`,
  `removePassenger`, `getSeatPosition` and `haveRoom`. The Vehicle virtual table
  slots used are at offsets 160 (the eventActivate entry), 168 (eventDeActivate),
  176 (eventGasOn), 184 (eventGasOff), 208 (eventCollisionWall) and 216
  (eventSteeringChange); every virtual call in these functions loads a halfword
  delta and a pointer from the entry.
- Boat, SkulBoat and LavaTractor `set` functions are plain parameter loads (float
  bit patterns from the image) around `setMassAndIBody__7Vehicle`; LavaTractor
  also calls `addWheel__7VehiclePciffff` four times with the model names
  `wheelfrnt.vif` and `wheelbk.vif`.
- Three particle behaviours use the static particle definitions `slideDustParticle`,
  `slideDustParticleDark`, `slideDustParticleBig`, `slideDustParticleBigDark`,
  `lavaGoopParticle` and `lavaSteamParticle` (0x008d55b8 ... 0x008d5748) and the
  static point `nullp` (0x008d5798).

## Inferred

- The two accumulator pairs at +1992/+2004 and +2016/+2028 are separate
  force/torque sums (`accumForceAndTorque` flag 0 and 1); `doSuspension` uses the
  second pair for a trial integration (`getNewRotAndPos` with flag 1) whose result
  is then discarded by restoring the saved state. The structure strongly suggests a
  predictor step; no name is claimed.
- `doAI`, `doSteering` and `doThrottle` have an automatic mode selected by the word
  at +2148 and a manual mode driven by the pad; the pad words are read at
  `pad + 88*driver` (+8 buttons, +20 a stick axis).

## Unknown

- What every vehicle field means; the contents of `allVehicleSpecs` beyond what the
  code reads (spec float +0, +4, +56 are used by `doDriveline`).
- Whether `cutMode` (a signed byte at 0x00484318) means a camera cut; only its
  use as a flag is shown.
- The exact sign-of-zero and overflow behaviour on the EE FPU for the places
  where the interpreter model differs (see above).
- The reason `doDriveline` reads `player[-1]`-style data is not relevant here; but
  `doSteering` and `doThrottle` index `pad` and `player` with the driver index
  even when it is −1, reading the words just before those arrays; the sources
  keep that behaviour.

## Imports closed (names)

`_$_4Boat`, `accumForceAndTorque__7VehicleP6Point3T1i`, `addPassenger__7Vehiclei`,
`backup__7Vehicle`, `buildCollisionInfo__7Vehicle`, `checkVehicleCollision__7Vehicle`,
`checkWallCollision__7Vehicle`, `doAI__7Vehicle`, `doBrakes__7Vehicle`,
`doDriveline__7Vehicle`, `doMisc__7Vehicle`, `doMyUniqueForces__4Boat`,
`doSteering__7Vehicle`, `doSuspension__7Vehicle`, `doThrottle__7Vehicle`,
`finalizePosition__7Vehicle`, `getNewRotAndPos__7VehicleP6Point3T1fii`,
`getSeatPosition__7VehicleiP6Point3`, `haveRoom__7Vehicle`, `isPassenger__7Vehiclei`,
`removePassenger__7Vehiclei`, `set__11LavaTractor`, `set__4Boat`, `set__8SkulBoat`,
`startForces__7Vehicle`, `vehicleCount`, `vehicleList`.

Not closed here: `norrath_construct_Vehicle`. It is already authored in
`vehicle-actors-vehicle-construct/construct_vehicle.cpp`
(`norrath_construct_Vehicle`); it shows up as an import only because that unit is
not in the build plan. Adding that unit to the plan closes it.

## Imports opened

Names these units call that are not defined by a unit in this package and are not
in the base unresolved list of `work/plan9-build/unresolved-imports.json` (some
may already be defined by units merged after that list was produced; the integrator
should re-run the import list):

Functions: `FinalizePosition__5Wheel`, `P_AddParticle__FP11ParticleDefP6Point3T1f`,
`__17VehicleBodySphereP7Vehicleff`, `acos`, `acosf`, `addWheel__7VehiclePciffff`,
`atan2f`, `backupJustForces__7Vehicle`, `buildCollisionBox__7Vehicle`,
`changeOrientation__7VehicleG6Point3`, `dismount__6Player`,
`doDifferential__7Vehicle`, `dpmul`, `dualImpulse__7VehicleP7VehicleT1P6Point3T3`,
`gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi`,
`getBlockingVehicle__7VehiclePf`, `getEngineTorque__7Vehicle`,
`getParticleVelocity__7VehicleP6Point3`, `impulse__7VehicleP6Point3N21fii`, `memset`,
`objectUpdateInGrid__FP10GameObject`, `powf`, `quatMul__FP4QuatN20`,
`rayHit__7VehicleP6Point3T1fT1`, `resolveCreatureCollisions__7Vehicle`,
`scalePatchLimit__7VehicleiP6Point3`, `segmentLineHit__FP6Point3N50`,
`setMassAndIBody__7Vehicle`, `sqrt`, `_$_7Vehicle`, `_vt$4Boat`.

Data: `cutMode`, `frameNm`, `gameControlPaused`, `pad`, `nullp`,
`lavaGoopParticle`, `lavaSteamParticle`, `slideDustParticle`,
`slideDustParticleBig`, `slideDustParticleBigDark`, `slideDustParticleDark`.

`world`, `player` and the other imports already in the base list stay as they were.

Integration hazard: `vehicle-actors-vehicle-construct` keeps its own private
storage `norrath_vehicle_count_word` and `norrath_vehicle_list_bytes` instead of
using `vehicleCount` and `vehicleList`. With both in the link, construct and the
`Vehicle` destructor (which reads `vehicleCount`/`vehicleList`) would disagree. The
construct unit needs to reference `vehicleCount`/`vehicleList` (or the plan needs
symbol aliases from the `norrath_` names); that unit belongs to the vehicle-actors
package, so it was not edited here.

## Build-plan entries needed

All 26 units use profile `authored_cpp`, an empty `header_closure`, and kind
`independently_authored_source`. Host contracts (`host_check.cpp`) are not plan
inputs. Rows are source, object name, sha256 of the source. `input_order` and the
`emitted_functions` list (symbol and `compiled_body_bytes`, both in the table above)
are for the integrator.

| Source | Object | sha256 |
|---|---|---|
| `sources/w9-vehicle-accum-force-and-torque/accum_force_and_torque.cpp` | `w9_vehicle_accum_force_and_torque.o` | `5548726b6d235e76119b6a67c323ff718223d65ef29fec70f5188c048a6f0d75` |
| `sources/w9-vehicle-add-passenger/add_passenger.cpp` | `w9_vehicle_add_passenger.o` | `346a3b25637d754ddfd97fce0d2ee0ed9f903aa4d193742a8106c0091e0ad1c8` |
| `sources/w9-vehicle-backup/backup.cpp` | `w9_vehicle_backup.o` | `d17ab32ec7516f7bc47a172c22d6c391d21c49abb03977adc377cfd3da7cfd3e` |
| `sources/w9-vehicle-boat-destructor/destructor.cpp` | `w9_vehicle_boat_destructor.o` | `aaa0bf2f72fd952869835bc785a5e29e81af4b27ae372060bcbc9b8193a0f9b6` |
| `sources/w9-vehicle-boat-set/boat_set.cpp` | `w9_vehicle_boat_set.o` | `f5fbaa66e7116c3bb413f6842e5a78d22ec1a73b7d4cee9e9a4a7051c4ec1885` |
| `sources/w9-vehicle-boat-unique-forces/unique_forces.cpp` | `w9_vehicle_boat_unique_forces.o` | `86d2f926a5b9e2501dc7f6372ff4e2a5a6efa4555e5ce13eef81015301fe830e` |
| `sources/w9-vehicle-build-collision-info/build_collision_info.cpp` | `w9_vehicle_build_collision_info.o` | `9bbd636975b4b57c1637746cbf4baed4121a20e6d225c71987025e3a68840081` |
| `sources/w9-vehicle-check-vehicle-collision/check_vehicle_collision.cpp` | `w9_vehicle_check_vehicle_collision.o` | `919792fc015919e2c7253091cc0e031a1f134bc5dd751dd36c7f6d5127cbb672` |
| `sources/w9-vehicle-check-wall-collision/check_wall_collision.cpp` | `w9_vehicle_check_wall_collision.o` | `826d4aadbec1c6837e02775078bb8d936c56eab01698e4e291e0cd4ab90e2be9` |
| `sources/w9-vehicle-do-ai/do_ai.cpp` | `w9_vehicle_do_ai.o` | `d923ac5a97c9134ddb6d59529a8323c4def91352a1bd905cf9fc9b07e19d385e` |
| `sources/w9-vehicle-do-brakes/do_brakes.cpp` | `w9_vehicle_do_brakes.o` | `749c9dfef0d2411d8914c6cb49cc4084539e9536bd06ff52f6d7bf47e29e5ece` |
| `sources/w9-vehicle-do-driveline/do_driveline.cpp` | `w9_vehicle_do_driveline.o` | `ba4080659dae6ab72fbc9ff5d8211b0d49b263b23acf2c0b01b071aaef3a1ef0` |
| `sources/w9-vehicle-do-misc/do_misc.cpp` | `w9_vehicle_do_misc.o` | `2ba513ca9062addeaf0eeaba7a1630d668de24635ea68573451bcf8ec8036ecc` |
| `sources/w9-vehicle-do-steering/do_steering.cpp` | `w9_vehicle_do_steering.o` | `7feb0fd15271e9c8af8ff98597db7d091f6f40d0d1eb4d548a29ebb16f8f55b8` |
| `sources/w9-vehicle-do-suspension/do_suspension.cpp` | `w9_vehicle_do_suspension.o` | `31947982cea618860cb22fd4db87c5096692ff55203af01b28a62b5a7daac76f` |
| `sources/w9-vehicle-do-throttle/do_throttle.cpp` | `w9_vehicle_do_throttle.o` | `a247bca27736e8b4a263ab18890c6c587b2a00816a3af3e9ab817005dd89713d` |
| `sources/w9-vehicle-finalize-position/finalize_position.cpp` | `w9_vehicle_finalize_position.o` | `8210d4864f4556948fa2d9b7863943c4526643ad84e26447f38f0a0b7887530a` |
| `sources/w9-vehicle-get-new-rot-and-pos/get_new_rot_and_pos.cpp` | `w9_vehicle_get_new_rot_and_pos.o` | `4b2c06c40a9ad382d03457dd74dc09518e05bf9c2b2eb12438e5b78e475e366f` |
| `sources/w9-vehicle-get-seat-position/get_seat_position.cpp` | `w9_vehicle_get_seat_position.o` | `033f25d7f3e87d88718e3cbe78ad416522b731e49f084f343bfe46aa2282fe92` |
| `sources/w9-vehicle-globals/vehicle_globals.cpp` | `w9_vehicle_globals.o` | `a277cd748a40e79368fdeedc7636a9dc102d1c1c19bc93ab9ea1e9696ab58650` |
| `sources/w9-vehicle-have-room/have_room.cpp` | `w9_vehicle_have_room.o` | `3c2dabc14a429c63837eeead15145ec564135837abed5013a3188781fcc7fb7d` |
| `sources/w9-vehicle-is-passenger/is_passenger.cpp` | `w9_vehicle_is_passenger.o` | `595b15f9a80473cb5c5e83a100bbb861b8879f88419df1e1f4491b5db7b3ff0c` |
| `sources/w9-vehicle-lavatractor-set/lavatractor_set.cpp` | `w9_vehicle_lavatractor_set.o` | `65971286e4ad7e8ad39a08d4c4fe2a5f68fca5801e519af1134a211b1b20d4ea` |
| `sources/w9-vehicle-remove-passenger/remove_passenger.cpp` | `w9_vehicle_remove_passenger.o` | `80dd1a7cfe4516b39ebb89db6755ddb2cb3cc1f78c909614a0f7203cace8c1d7` |
| `sources/w9-vehicle-skulboat-set/skulboat_set.cpp` | `w9_vehicle_skulboat_set.o` | `3cb8307fc3e882a0c6ed1c4aa312a8b2f4fc6d45fb43a62629cd332d98407daa` |
| `sources/w9-vehicle-start-forces/start_forces.cpp` | `w9_vehicle_start_forces.o` | `e91885cdcda43c3f4b83f154cfe771971aa0ac0d2a6953d6d9a2c983bd411a67` |

## Authored but never executed

All 26 units. None has run on the EE, in the original program, or as gameplay. The
paired runs execute the original's machine code and the authored code in a private
interpreter that models the R5900 integer unit and a single-precision FPU
approximately (see above). Specifically not exercised by any scenario: the
`doDriveline` clamp where `keep > 1`; `buildCollisionInfo`'s unsigned-conversion
adjustment (values at or above 2^31); the NaN branch of `finalizePosition`; and
`buildCollisionInfo` with exactly one sphere (a divide by zero whose result the EE
and the host treat differently).

## What is left

Nothing in the assigned import list is skipped. Open items for others: the plan
entries above; the construct-storage hazard; re-running the import list after
merging.
