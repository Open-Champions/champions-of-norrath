# Animation, object lists and creature AI (c) (w9-animation-object-c)

Branch `agents/w9-animation-object-c`. New source is under `experimental/sources/w9-animation-object-c-*`; shared helpers are in `w9-animation-object-c-common`. Reference executable: the private `SLUS_205.65` named in the docs, read locally only. No game bytes, listings or decompiler output are committed; addresses and extents are that file's symbol table.

Every function compiles with the reviewed `authored_cpp` profile and has a `host_check.cpp` (host compiler, exits non-zero on a wrong store, result or branch; not a plan input). Each was also run against the original on a private R5900 interpreter harness kept outside git: original and authored function ran from identical memory snapshots and the return value, call log and every changed byte outside the stack were compared. That harness run is not part of the repository and no byte match of compiled code is claimed.

Records are addressed by byte offsets through `mem.hpp` because no original class declaration is claimed; field roles are inferred from use and are not recovered names.

## Already closed before this package

Checked against the tree at the start: `objectFindAnimation__FP10GameObjectPc`, `objectFindObjectByClass__FPcPP10GameObjecti`, `objectInside__7TriggerP10GameObject`, `objectRemoveFromGrid__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `objectSaveAddToStringPool__FPCc`, `objectSaveGetFromStringPool__Fi`, `objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`, `objectSetFilterFunction__FPFPc_i` are in the build plan (w8-object-list-math); `runList` and `slowRunList` are defined by `w8-object-list-math-run-list-heads`; `saveActivate__9Savepoint` has `w8-actor-virtuals-c-save-activate`. Those are not repeated here.

## Functions

| source directory (`experimental/sources/w9-animation-object-c-…`) | emitted symbol | image address | image extent |
|---|---|---|---|
| `reset-routing` | `resetRouting__8Creature` | 0x113460 | 40 |
| `set-anim-event-flags` | `setAnimEventFlags__8Creatureififififi` | 0x1128a8 | 88 |
| `set-state-processing` | `setStateProcessing__8CreaturePi` | 0x1129f0 | 108 |
| `object-is-off-grid` | `objectIsOffGrid__FP10GameObject` | 0x1435a8 | 132 |
| `iterate-run-lists` | `objectIterateRunLists__FPFP10GameObject_v` | 0x13ede8 | 324 |
| `smart-wander` | `smartWander__8Creature` | 0x113128 | 212 |
| `object-vis-data` | data: `objectVisPoints` (48 bytes), `objectVisCount` (1), `objectVisUseCamCenter` (1, initial 1) | 0x333d28, 0x483afe, 0x483ac9 | 48, 1, 1 |
| `out-msg-data` | data: `outMsg` | 0x8d88b8 | 128 |
| `w-lump-header-data` | data: `wLumpHeader` | 0x4845a4 | 4 |
| `radial-damage` | `objectRadialDamage__FP10GameObjectG6Point3iffiii` | 0x13f708 | 548 |
| `find-empty-spot` | `objectFindEmptySpot__FG6Point3iiPc` | 0x13f2d0 | 1076 |
| `slide-back` | `slideBack__8Creaturef` | 0x111490 | 924 |
| `steer-and-route` | `steerAndRouteToPosition__8CreatureG6Point3` | 0x1102b8 | 1572 |
| `spawn` | `spawn__FG6Point3iisPcbPPci` | 0x26a738 | 840 |
| `update-animation` | `updateAnimation__FP13animComponent` | 0x161528 | 2248 |

Data units (no function bodies) have a `host_check.cpp` that asserts the extent and initial value instead of a paired run: `object-vis-data` defines `objectVisPoints` (0x333d28, 48 bytes, four 12-byte points), `objectVisCount` (0x483afe, signed byte, 0) and `objectVisUseCamCenter` (0x483ac9, byte, initial 1); `out-msg-data` defines `outMsg` (0x8d88b8, 128 bytes to the next symbol `inMsg`); `w-lump-header-data` defines `wLumpHeader` (0x4845a4, one zero word). The image places the first group in initialised data; the zero-valued ones compile to BSS in the reviewed profile, the flag byte stays in `.data`.

## What was checked

- Every source compiles with the reviewed `authored_cpp` flags (`mips64r5900el-ps2-elf-g++`, GCC 15.2.0) one unit at a time. No full partial build was run (the lead integrates plans); the undefined symbols of the fifteen objects were listed with `nm -u` and compared with the current plan and with the older unresolved list (see "Imports opened").
- Each function's `host_check.cpp` builds with the host compiler (`c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp`, also clean under address and undefined-behaviour sanitizers for `update-animation`) and returns non-zero on a wrong store, result, call order or branch. Callees are stand-ins that record their arguments.
- Each function was run against the original on the private R5900 interpreter harness (kept outside git, described in the working notes of earlier packages): original and authored code ran from identical memory snapshots with identical stand-ins for the callees, and the return value (where the function has one), the call log with arguments, and every changed byte outside the stack were compared. Mismatches seen on the way and fixed: `slideBack` copied a 12-byte vector where the original copies only the 8-byte pair at +180/+184, and the halfword at +188 is saved before it is cleared; the harness's `sqrt.s` took its operand from the wrong register field (R5900 `sqrt.s fd, ft`), fixed in the private copy. Counts: `steer-and-route` 700 scenarios (391 of 393 original instructions executed; the other two are padding), `update-animation` 200 scenarios (561 of 562, the other is padding), `spawn` 64 scenarios (210 of 210, with the libc string calls replaced by identical stand-ins on both sides), `find-empty-spot` 36, `slide-back` 160, `radial-damage` 40, `smart-wander` 15, `iterate-run-lists` 5, `object-is-off-grid` 17, `set-state-processing` 67, `set-anim-event-flags` 8, `reset-routing` 4.
- Mutation spot checks (paired run must fail): `< 16001` to `<= 16001` in `steer-and-route` (74 of 700 scenarios failed), the 2^-17 scale to 2^-16 in `update-animation` (12 of 40), and `depth < 0` to `depth <= 0` in `spawn` (6 of 64).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Not run: `python3 tools/build_partial.py` (no private plan was built), `--check-plan` (no plan edit made).

## Observed

- `resetRouting__8Creature` stores -1.0 at +260 and +264, -1 at +580 and +584, 0 at +708 and +716, and reads nothing.
- `setStateProcessing__8CreaturePi`: with the signed byte at +570 nonzero and *state == 1 it returns 1; with *state == 3 it calls `bExitStatePersueIdle__8Creature` (which tests whether the halfword at +794 is zero) and stores 5 when that returns zero.
- `smartWander__8Creature` calls the steering routine on a copy of the point at +272, calls virtual slot 184 when it reports a result, returns 1 while the word at +716 is below 4 and otherwise resets the routing and picks a new goal with `getSmartRandomDestination` (range 300).
- `objectIsOffGrid__FP10GameObject` and `objectIterateRunLists__FPFP10GameObject_v` use the list safe iterator with the default off-grid head (0x486890) and `runList`, the fifteen `mediumRunList` heads and the sixty `slowRunList` heads respectively, each head with its own iterator and the mask word `objectIgnoreFlags`.
- `objectFindEmptySpot__FG6Point3iiPc` draws points in the unit sphere with the generator on `syncRandState` (multiplier 0x19660d, increment 0x3c6ef35f, three steps per draw), offsets x and y by the spread, tests with `worldTestForbidden` and `worldFindHeight` and a box query, and gives up after 50 attempts. The result is written even on failure (the centre, or the last candidate that passed the height test).
- `objectRadialDamage__FP10GameObjectG6Point3iffiii` uses an octagon-style distance (|dx| + |dy| - 0.333 * min), an object radius at +28, linear falloff, and calls virtual slot 16 with the damage converted through an integer.
- `slideBack__8Creaturef`, `steerAndRouteToPosition__8CreatureG6Point3`, `spawn__FG6Point3iisPcbPPci` and `updateAnimation__FP13animComponent`: field offsets, constants, call order and branch structure are in each source header. In `spawn` the five `strcasecmp` calls ("scorpion", "firebeetle", "bbeetle", "ant", "lbeetle") have unused results. `spawn` and `updateAnimation` read past a missing terminator or end of stream exactly as the original does (not tested).
- `updateAnimation` reads a packed bit stream through a 32-bit window made of two halfwords; the frame state is a list node (+12 older, +16 newer, +20 owner) followed by 60-byte track records at +28.

## Inferred

- Field roles (`B+4..B+12` position, `B+16..B+24` its velocity, `B+28..B+40` a four-float rotation with a rate, the yaw at state+24, "once-only"/"always" style flags) are read from arithmetic and use and are not recovered names. `steerAndRouteToPosition` returning 1 means "arrived"; `spawn` is "place one monster of this class". The octagon distance constant, the 100000.0 vertical box extent and the particle/effect choice (bit 0x100 of `levelFlags`) are observed values; their purpose is inferred.
- The `strcasecmp` chain in `spawn` is what is left of a check whose reaction was compiled out; the stack word in the original `slideBack` was resolved by direct observation (the pair at +180/+184 is eight bytes), so nothing there is inferred.

## Imports closed

Names newly defined by this package (functions and data): `objectFindEmptySpot__FG6Point3iiPc`, `objectIsOffGrid__FP10GameObject`, `objectIterateRunLists__FPFP10GameObject_v`, `objectRadialDamage__FP10GameObjectG6Point3iffiii`, `objectVisCount`, `objectVisPoints`, `objectVisUseCamCenter`, `outMsg`, `resetRouting__8Creature`, `setAnimEventFlags__8Creatureififififi`, `setStateProcessing__8CreaturePi`, `slideBack__8Creaturef`, `smartWander__8Creature`, `spawn__FG6Point3iisPcbPPci`, `steerAndRouteToPosition__8CreatureG6Point3`, `updateAnimation__FP13animComponent`, `wLumpHeader`.

Names in the package list that were already closed by earlier work (nothing authored here): `objectFindAnimation__FP10GameObjectPc`, `objectFindObjectByClass__FPcPP10GameObjecti`, `objectInside__7TriggerP10GameObject`, `objectRemoveFromGrid__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `objectSaveAddToStringPool__FPCc`, `objectSaveGetFromStringPool__Fi`, `objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`, `objectSetFilterFunction__FPFPc_i` (all in the current build plan from `w8-object-list-math`), `runList` and `slowRunList` (defined by `w8-object-list-math-run-list-heads`, which the plan already selects) and `saveActivate__9Savepoint` (`w8-actor-virtuals-c-save-activate`, not yet selected in the plan; add that unit to close it).

## Imports opened

Symbols the new objects reference that no unit in the current plan or in this package defines (checked against the plan's emitted function names, the older partial object's symbol table and the older unresolved list): `addpmlist__FPc`, `addpmlistptrscap__Fv`, `fsListBack` (0x486910), `getClearradius__FPCc`, `getCurrentRoutePoint__8Creature`, `getFreeFrameState__Fv`, `getNextRoutePoint__8CreatureP6Point3G6Point3ffi`, `objectCreateByName__FPcG6Point3sPPc`, `objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc`, `pmlistptrs` (0x435b68, 1024 bytes), `pmlistptrsused` (0x485e1c), `userFilterFunction` (0x483b00), `wallAndObjectSteering__FP8CreatureffP10GameObject` (a local function, 0x108618), `worldTestForbidden__FP12_worldHeaderG6Point3f`.

Already-unresolved names that the new objects also use (not opened here): `addEffect__FiG6Point3`, `bExitStatePersueIdle__8Creature`, `eRand__10GameObject`, `getSmartRandomDestination__8Creaturei`, `mediumRunList`, `move2__8CreatureG6Point3`, `runList`, `slowRunList`, `syncRandState`, `world`, `worldFindHeight__FP12_worldHeaderG6Point3f`, `worldHitscan__FP12_worldHeaderG6Point3T1fi`. Several of these have authored sources elsewhere in the tree that the plan has not selected yet (for example `eRand__10GameObject`, `bExitStatePersueIdle__8Creature`, `getSmartRandomDestination__8Creaturei`, `runList`); the integrator should check each against the final plan. `fsList` and the libc calls `sscanf`, `strncpy`, `strstr`, `strcasecmp` are taken to be provided by the plan's existing owners (they were not in the older unresolved list).

## Build-plan entries needed

Append these inputs after the current last input and assign fresh `input_order` values. `default_authored_inputs` increases by 15. Add the `files` list below to `files` in the plan and snapshot, and add a snapshot input record per unit (component = the source directory name). No `private_object_sha256` is claimed. `profile` is `authored_cpp`; body sizes are from the reviewed EE `g++` profile, not from the original executable. Host check files are not inputs. The data units have no emitted functions.

```json
{"emitted_functions":[{"compiled_body_bytes":844,"name":"objectFindEmptySpot__FG6Point3iiPc"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"find_empty_spot.o","profile":"authored_cpp","sha256":"3ccf2f9fb0d20b96bed1eb6f3d8c4baba5e919bddcdd66773e76971368a39c5a","source":"sources/w9-animation-object-c-find-empty-spot/find_empty_spot.cpp"}
{"emitted_functions":[{"compiled_body_bytes":328,"name":"objectIterateRunLists__FPFP10GameObject_v"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"iterate_run_lists.o","profile":"authored_cpp","sha256":"1fe180910f0746ae6cdef2cda4acd0349369f38c0bf794fee3c2a4b16c562c2e","source":"sources/w9-animation-object-c-iterate-run-lists/iterate_run_lists.cpp"}
{"emitted_functions":[{"compiled_body_bytes":152,"name":"objectIsOffGrid__FP10GameObject"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"object_is_off_grid.o","profile":"authored_cpp","sha256":"4ffea20fc916e907653a79ba9813e50cb663472718bec261abbf9833041648b2","source":"sources/w9-animation-object-c-object-is-off-grid/object_is_off_grid.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"object_vis_data.o","profile":"authored_cpp","sha256":"7a498e769a10daa1ad313f9e8ebf8cb542553238f4b821e8522abcfa0c8cef3c","source":"sources/w9-animation-object-c-object-vis-data/object_vis_data.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"out_msg_data.o","profile":"authored_cpp","sha256":"37dc166f37d37a3ca85c35ff0e2a069ad8113a4fc7aeac548a85bb9fd8540050","source":"sources/w9-animation-object-c-out-msg-data/out_msg_data.cpp"}
{"emitted_functions":[{"compiled_body_bytes":500,"name":"objectRadialDamage__FP10GameObjectG6Point3iffiii"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"radial_damage.o","profile":"authored_cpp","sha256":"ab738a81581138d1697141879c8f702c6bbb42d044d504a2695dcfc337eeb9b8","source":"sources/w9-animation-object-c-radial-damage/radial_damage.cpp"}
{"emitted_functions":[{"compiled_body_bytes":40,"name":"resetRouting__8Creature"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"reset_routing.o","profile":"authored_cpp","sha256":"82356106da53a3c4a8219f742b28feb572badf65e678c345d64d50dcecdfdc13","source":"sources/w9-animation-object-c-reset-routing/reset_routing.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"setAnimEventFlags__8Creatureififififi"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"set_anim_event_flags.o","profile":"authored_cpp","sha256":"7f4038684237a4738efd9b8edbd47f2a2f543bc38227c107ce105ed81881d79b","source":"sources/w9-animation-object-c-set-anim-event-flags/set_anim_event_flags.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"setStateProcessing__8CreaturePi"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"set_state_processing.o","profile":"authored_cpp","sha256":"a423fdbb59906eab23362e7b37adcb51103e0391ec8202dc3269cf8ed081d9ae","source":"sources/w9-animation-object-c-set-state-processing/set_state_processing.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1468,"name":"slideBack__8Creaturef"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"slide_back.o","profile":"authored_cpp","sha256":"0d9f4c195be1fc974bff0acef134d9acb805393afe029f3fc15792930fc67588","source":"sources/w9-animation-object-c-slide-back/slide_back.cpp"}
{"emitted_functions":[{"compiled_body_bytes":188,"name":"smartWander__8Creature"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"smart_wander.o","profile":"authored_cpp","sha256":"9c3b3eaa062196e5e2743fc560b2d8a23ea65f55f72f580fc7026f90e9dcfba6","source":"sources/w9-animation-object-c-smart-wander/smart_wander.cpp"}
{"emitted_functions":[{"compiled_body_bytes":932,"name":"spawn__FG6Point3iisPcbPPci"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"spawn.o","profile":"authored_cpp","sha256":"77cfb4b46456c77d252eaf5185c5d18d24ddf9e6351401e95ae80a991834dbbf","source":"sources/w9-animation-object-c-spawn/spawn.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1580,"name":"steerAndRouteToPosition__8CreatureG6Point3"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"steer_and_route.o","profile":"authored_cpp","sha256":"bb0a311d438d36b794b99f5109f4680eced6b4a2eeab0c1d6720dbcee3eeac8e","source":"sources/w9-animation-object-c-steer-and-route/steer_and_route.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1788,"name":"updateAnimation__FP13animComponent"}],"header_closure":["sources/w9-animation-object-c-common/mem.hpp"],"kind":"independently_authored_source","object_name":"update_animation.o","profile":"authored_cpp","sha256":"dfed32bb444cddaac1ac7d7145c5a3541ba2d83e153e5c64cff8e214883b528d","source":"sources/w9-animation-object-c-update-animation/update_animation.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w_lump_header_data.o","profile":"authored_cpp","sha256":"a8bb946c4185e0eb30145f3d5eb45681f3d4dac5d851c1abe29ec837abd9cb53","source":"sources/w9-animation-object-c-w-lump-header-data/w_lump_header_data.cpp"}
```

`files` to add (sorted):

```json
["sources/w9-animation-object-c-common/mem.hpp", "sources/w9-animation-object-c-find-empty-spot/find_empty_spot.cpp", "sources/w9-animation-object-c-iterate-run-lists/iterate_run_lists.cpp", "sources/w9-animation-object-c-object-is-off-grid/object_is_off_grid.cpp", "sources/w9-animation-object-c-object-vis-data/object_vis_data.cpp", "sources/w9-animation-object-c-out-msg-data/out_msg_data.cpp", "sources/w9-animation-object-c-radial-damage/radial_damage.cpp", "sources/w9-animation-object-c-reset-routing/reset_routing.cpp", "sources/w9-animation-object-c-set-anim-event-flags/set_anim_event_flags.cpp", "sources/w9-animation-object-c-set-state-processing/set_state_processing.cpp", "sources/w9-animation-object-c-slide-back/slide_back.cpp", "sources/w9-animation-object-c-smart-wander/smart_wander.cpp", "sources/w9-animation-object-c-spawn/spawn.cpp", "sources/w9-animation-object-c-steer-and-route/steer_and_route.cpp", "sources/w9-animation-object-c-update-animation/update_animation.cpp", "sources/w9-animation-object-c-w-lump-header-data/w_lump_header_data.cpp"]
```

## Authored but never executed

- No function has run on an EE or in an emulator. The paired runs use a private interpreter that implements only the instructions the originals use; its floating-point behaviour was compared against the original code, not against hardware.
- Every callee in the paired runs was either the original image's own code (icos, isin, iatan2, eRand, the list iterator, bExitStatePersueIdle) or a recording stand-in with scripted results (world queries, steering, route points, move2, effects, particles, the virtual slots, libc string functions in `spawn`). The authored functions have therefore not been run against authored callees.
- Paths no scenario reached: the padding no-ops in `steerAndRouteToPosition` (0x1105fc, 0x110814) and `updateAnimation` (0x161bd4). Division by zero in `slideBack` (a radius below 1.0) was not run on either side; the EE `g++` build inserts divide-by-zero checks that the original does not have.
- The data units were checked only for extent and initial value on the host.
- `slideBack`, `steerAndRouteToPosition` and `spawn` are larger than the original (1468, 1580 and 932 bytes against 924, 1572 and 840) because the compiler differs; sizes are not a match claim.

## Unknown

- What the fields used by these routines are for beyond their arithmetic: creature +176, +180/+184, +568..+570, +580..+584, +712..+740, +760, +800, +808 and the vtable slots 16, 176 and 184; what `line`, `radius` and `type` mean to the spot search in `spawn`; why exactly those five class names are compared; what bit 0x100 of `levelFlags` selects; what the low two bits of `move2`'s result mean.
- The declared types of `outMsg`, `objectVisPoints` and the record layouts; `outMsg` is given its distance to the next symbol (128 bytes), and a 126-byte record is only a guess from `inMsg`'s size.
- Whether `wLumpHeader` is meant to be file-local (it is a local symbol in the image).
- The original's exact return types: `spawn`, `updateAnimation`, `slideBack`, `setAnimEventFlags` and `resetRouting` leave an incidental register value; `smartWander` leaves 1 or 0 and `steerAndRouteToPosition` returns 1 or 0.
