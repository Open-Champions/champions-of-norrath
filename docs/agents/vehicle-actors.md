# Vehicle constructor and actor views

Package: the 27 imports the actors-b merge opened, plus the Vehicle virtual
table the constructor stores. Branch `agents/vehicle-actors`. Shared base is
`999b562`. World door height and the water check stay with world-state.

The actors-b section of `docs/agents/INTEGRATION.md` is the import list for
this tip (796 unresolved after that merge). No fresh `unresolved-imports.json`
existed in this worktree. Private reference: `work/private/SLUS_205.65`,
SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Addresses below are that file's symbol values. No game bytes or disassembly
listings were committed.

Each function was compiled with the reviewed EE g++ `authored_cpp` flags and
has a host contract under `host/` that returns non-zero when the stores, call
order, or result are wrong. Each view has a host check that returns non-zero
when a slot, string, or float word differs. Host files are not EE inputs.
`snapshot.json` still rejects unlisted files under `experimental/sources/`.
This package does not edit that file or `experimental/build-plan.json`.

Compiled bodies are larger than the image extents. These are behavior
reconstructions. Nothing here has run on the EE or as gameplay.

## Functions

| source | symbol | image | compiled bytes | sha256 |
|---|---|---|---|---|
| `vehicle-actors-medium-run-list/add_medium_run_list.cpp` | `norrath_actor_add_medium_run_list` | `objectAddToMediumRunList__FP10GameObject` `0x143908`, 100 bytes | 240 | `91e11a386598b4f7585447bfa56b2bae985519132800d17bc971ba92a063e8ce` |
| `vehicle-actors-goth-trap-passable/set_passable_state.cpp` | `norrath_goth_trap_set_passable_state` | `setPassableState__8GothTrapb` `0x225cc0`, 300 bytes | 504 | `976f6fea7f18fe6271d40e86c3524a3a73be741a46036bf332a6a993c0609d81` |
| `vehicle-actors-hate-bridge-passable/set_passable_state.cpp` | `norrath_hate_bridge_set_passable_state` | `setPassableState__10HateBridgeb` `0x225df0`, 252 bytes | 448 | `a460f7393f60ea6ce2efaf0af85596144efa2381a8d0badc59eedf4086c7ca4c` |
| `vehicle-actors-vehicle-construct/construct_vehicle.cpp` | `norrath_construct_Vehicle` | `__7VehicleG6Point3sPPci` `0x2702d8`, 700 bytes | 1172 | `32ffb8eceddc9a3eaad95f232c817c98cf8a2243e6e99058147ffa907d27b549` |
| `vehicle-actors-vehicle-reset/reset_vehicle.cpp` | `norrath_vehicle_reset` | `Reset__7Vehiclei` `0x270f28`, 576 bytes | 1340 | `e9ec063ee27d22e1518f0c26fefa715de90edc4174424641ce1c8a8c0be48447` |

Object names for the plan: `medium_run_list.o`, `goth_trap_passable.o`,
`hate_bridge_passable.o`, `construct_vehicle.o`, `reset_vehicle.o`.

### Medium run list

Observed: fifteen heads at `mediumRunList` (`0x4fbf90`). The next symbol,
`slowRunList`, is at `0x4fbfd0`, so the array is 64 bytes and the code indexes
60 of them. `mediumListCycle` is the 4 zero bytes at `0x483ad4`. The cycle
store happens in the `listOnewayLink` delay slot. The compare is signed: a
count of `0x7fffffff + 1` stays `0x80000000`. The return is the low 32 bits of
the link pointer. `norrath_list_remove` and `norrath_list_oneway_link` are
already defined.

Defined here, not previous imports: `norrath_medium_list_cycle_word` (4 BSS
bytes) and `norrath_medium_run_list_heads` (60 BSS bytes). The 4 bytes at
`0x4fbfcc` are not indexed.

### Passable state

Observed for both GothTrap and HateBridge: the bool argument is not read.
Every call reloads `world` (`0x4844a0`) and the door index at object+396.
`worldSetDoorCollision` always receives flag 1. Word 376 selects the rest.
State 0 moves the door to `{0,0,0}` and sets the height from object+400, then
reloads word 376. State 1 moves to `{0,0,-350}` (`0xc3af0000`) and subtracts
350 (`0x43af0000`). State 2 on GothTrap also moves to `{0,0,-350}` and adds
256 (`0x43800000`). State 2 on HateBridge does not move; it only adds 256.
Any other state stops after the collision call. The return is the last
callee's low 32 bits.

Left unresolved under the original ELF names:
`worldSetDoorCollision__FP12_worldHeaderii`,
`worldMoveDoor__FP12_worldHeaderiG6Point3s` (the point is passed as a pointer,
and the short extra is 0), and
`worldSetDoorHeight__FP12_worldHeaderif`.

### Vehicle constructor

Observed control flow of `__7VehicleG6Point3sPPci`:

- Snapshot the 12-byte point with byte loads, then call
  `norrath_game_object_construct`.
- Byte 125 becomes 8. The virtual-table address is stored at object+72.
  Word 2136 becomes 0.
- Find the tag `lump`, copy it, and format `%s.lmp` at object+76, `%s.vif`
  into a 128-byte local, and `%s.tex` into a 144-byte local. Those five
  strings live in this unit's private rodata. They are not the actor views.
- object+12 gains `0x00100001`.
- Flag 0 reloads the point at object+16, calls `norrath_world_find_height`
  with radius 32 (`0x42000000`), and stores that height plus 19.5
  (`0x419c0000`) at object+24. A nonzero flag, including -1, skips the store.
- `lumpFindResource__FPcT0` fills object+112 and object+116.
- `modelGetActiveChunks__FP12_modelHeaderi` is called on the reloaded lump
  with 0, and all 8 returned bytes are stored at object+392 before
  `norrath_actor_add_run_list`.
- Word 976 becomes 4, then `addPassenger__7Vehiclei(object, -1)`.
- `memset` clears object+984 (16), object+2104 (32), object+1052 (32), and
  object+2088 (16). Between the first and second clear, the body stores 0 at
  1024, 2148, 820, 1000, and 2168, and -1 at 956. The store at 2168 is in the
  delay slot of the next `memset`.
- Four iterations clear 12 floats from offset 1932 through 1976 inclusive.
- The current `vehicleCount` is stored at object+2156, the object's low bits
  go into `vehicleList[count]`, and the count increments before
  `processParams__7VehiclePPc`. The pointer passed to `processParams` is the
  word reloaded from object+44, not the original arguments pointer.
- Twelve bytes are copied from object+16 to object+1632 after the height
  store, so object+1640 is the raised height when the flag is 0.
- The type halfword is stored at object+56 and object+1048.
- The return is `gameLineInitPathState__FP17gameLinePathStatePcP6Point3`
  on object+2272, the text `path`, and object+16.

There is no bounds check. A count of 32 or more writes past the 128-byte
`vehicleList`. `vehicleCount` is the 4 zero bytes at `0x485e40`
(`norrath_vehicle_count_word`). `vehicleList` is the 128 zero bytes at
`0x436fa8` (`norrath_vehicle_list_bytes`), immediately after `allVehicleSpecs`.

### Vehicle reset

Observed `Reset__7Vehiclei`:

- `refreshSettings__7Vehicle` runs first. Object+2052 is still the pre-call
  word during that call. The spec address is then stored at object+2052.
  The address is `allVehicleSpecs` plus `value << 6`. The ELF object
  `allVehicleSpecs` is 64 bytes, so value 0 stays inside it and value 1 is
  the first byte of `vehicleList`. The shift is still in the source. The host
  contract only uses value 0. This unit defines those 64 bytes as
  `norrath_vehicle_specs_bytes`.
- Twelve bytes move from object+1632 to object+16. Floats at 1188, 1192,
  1196, 1200, 1204, and 1208 become 0.
- Word 2160 is loaded from `norrath_actor_view_4844a4` (`frameNm`), which is
  already defined. This unit does not redefine it.
- Words 2152, 1028, and 1040 become 0. Floats 848 and 752 become 0.
- Yaw is the sign-extended low 16 bits of the halfword at object+1048 plus
  16384. Halfword `0xC000` plus 16384 wraps to yaw 0.
- `matMakeTransYaw__FP8Matrix34G6Point3s` receives the updated point at
  object+16. `matToQuaternion__FP4QuatP8Matrix34` writes object+1084.
  `quatToMat__FP8Matrix33P4Quat` writes object+1116.
- The signed word at object+1000 is captured before later stores. If it is
  greater than 0, that many float pairs at 676+4*i and 708+4*i are cleared.
  The captured count is not reloaded in that loop.
- Object+748 becomes 0, then the inverse is `1.0 / float(object+820)`, then
  the cleared float at object+748 is copied to 740, 744, and 800. Words 1036
  and 1032 become 0. The inverse is stored at object+760. A zero divisor is
  untested.
- The spec pointer is reloaded from object+2052. `float(spec+48) * 0x3da3d70a`
  (the unnamed word at `0x48217c`) is stored at object+756 even when the
  captured count is not positive. Floats at 952 and 808 are cleared. Bytes
  109, 110, and 108 become 0.
- If the captured count is positive, wheels at object+1052+4*i are passed to
  `Reset__5Wheel` (20 bytes at `0x2783a8`). The first wheel whose word at
  wheel+388 is nonzero is selected. The loop limit is the signed word at
  object+1000 reloaded after each call. If none is selected, the return is
  `0xffffffff`. If one is selected, the spec is reloaded again and object+832
  becomes `(spec[4] / (spec[0] / float(object+912))) * 0x3d888888 * float(wheel+380)`.
  The return is the unsigned word in the selected wheel slot.

## Views

Each virtual table is 8-byte slots: an adjustment word, then a function
pointer. Every adjustment in these tables is 0. A zero function word is a
null slot, and each of these tables ends with one. The function name is the
sized ELF function at that address. `__gnu_compiled_cplusplus` and
`gcc2_compiled.` share some of those addresses and are not the slot. Where
`clone__10GameObject` is the sized symbol, that is the slot.

| symbol | ELF object | address | bytes | sha256 |
|---|---|---|---|---|
| `norrath_actors_b_view_448a70` | `_vt$15PushPhysicsProp` | `0x448a70` | 144 | `e3dbbe94c807f0fdd9d95540644ea59dcdb46b7ae7ff5600043412943d8b52c8` |
| `norrath_actors_b_view_448e30` | `_vt$10Teleporter` | `0x448e30` | 240 | `f1a494ea674e1afbe280ab8e156126b69103dca58dcbda98e3e314efc7c5cb29` |
| `norrath_actors_b_view_44a420` | `_vt$7Shooter` | `0x44a420` | 144 | `3d89667aef084127b98fec35cefa9b32165af2290b80b68e5ff1ab88be38c186` |
| `norrath_actors_b_view_44a4b0` | `_vt$11MissileTrap` | `0x44a4b0` | 144 | `ac6c39858d67c38a5ce328acff5d3622ad9b4a6272a702ac8d53d3cc424db62b` |
| `norrath_actors_b_view_44c8c0` | `_vt$7Trigger` | `0x44c8c0` | 152 | `a0f4ca797a878a1b88af0924593a8ef49c0ee11a631af51296e74d3722f22dea` |
| `norrath_actors_b_view_44ee08` | `_vt$8HackDoor` | `0x44ee08` | 168 | `b17806913776f7ce514c7fa64040fdc9cdcda6d8a953bb0f4a50eafbc765b1df` |
| `norrath_actors_b_view_44eeb0` | `_vt$10HateBridge` | `0x44eeb0` | 168 | `68781e49d0d2cc19860725389c59da467f095ea7f0319b380510f3e579362e0a` |
| `norrath_actors_b_view_44ef58` | `_vt$8HateHole` | `0x44ef58` | 168 | `46ad5f9c5058bc882d146f1376cecb41bcfe7d8db0edb9f5e335260e6eb71aa2` |
| `norrath_actors_b_view_44f000` | `_vt$8GothTrap` | `0x44f000` | 168 | `1ad27b63a14d87ba016db71f0477a93bf71f741e272c3d8aeaef763f293a9760` |
| `norrath_actors_b_view_44f0a8` | `_vt$8HateGate` | `0x44f0a8` | 168 | `5e0efff25cd04362a242d5ce985527611e1ab2656f9f237535cc77f735d8e576` |
| `norrath_actors_b_view_44f150` | `_vt$8HateInto` | `0x44f150` | 168 | `64a0fa834fe85457e8faf42d2dc33c1a3554572175cae389bde612cb9f3f6fbd` |
| `norrath_actors_b_view_450250` | `_vt$11LavaTractor` | `0x450250` | 248 | `6369491c0326d065f2fa922bdfcb3f03610257f2c99dea90fbf669dfb602d705` |
| `norrath_actors_b_view_453230` | `_vt$8SkulBoat` | `0x453230` | 248 | `1603d32aa453d3c9fd2f80b13d7c3cff6f11ff5b5aa4b644940a2cf841fbd18a` |
| `norrath_vehicle_view_44ffd8` | `_vt$7Vehicle` | `0x44ffd8` | 248 | `207842972edac51a126ca22b37a2b9954e13b3b46148029f990ae4be7f2dccc1` |
| `norrath_actors_b_view_4816d4` | unnamed `.lit4` | `0x4816d4` | 4 | `6ee3ece77a21da3d5df58038cf5bd614bebb439ae8bc4a7ae78438243597ac86` |
| `norrath_actors_b_view_4844a0` | `world` | `0x4844a0` | 4 | `ce2e1a6d60ee53c04a36dd3136ce6930f8d0a1cee3d8b525e1bfb2c5df790473` |
| `norrath_actors_b_view_484fd8` | unnamed C string `w` | `0x484fd8` | 2 | `db0b752c26bafd38cb76d5cc7839e5bc09ca29f29e6411a5828e79302c2ce235` |
| `norrath_actors_b_view_484fe0` | unnamed C string `h` | `0x484fe0` | 2 | `500f80f7b7d547159c4cacfdda8a58a2d51b6203cc83fff46c772655290617ce` |
| `norrath_actors_b_view_484fe8` | unnamed C string `t` | `0x484fe8` | 2 | `f2c7a2ffb8469b42c2e1ee8d4ff156d459091290926dc1626c3f3a552c4a36f2` |
| `norrath_actors_b_view_4850f0` | unnamed C string `%d,%d` | `0x4850f0` | 6 | `4a45c2edd9c0fab4301c496ea921331f7b307d2338de2133590de9d5ec51dff3` |
| `norrath_actors_b_view_4850f8` | unnamed C string `%d` | `0x4850f8` | 3 | `3b7a05b61df65d13b7a74d457309b93ad4f55896fc0f027479d9be8d0ffda14e` |

`0x4816d4` is the bytes `00 40 1c 46`, which is 10000. The neighboring literals
are separate words. `world` is four zero `.sdata` bytes; `frameNm` starts at
`0x4844a4`. The tag strings are the characters plus a NUL. Padding before the
next tag is not part of the string. Virtual-table object names are the symbol
name plus `.o`. Data units emit no functions. The Vehicle table was not one of
the original 20 views; the constructor stores it, so this package defines it.

Slot 28 of `_vt$7Vehicle` is `__pure_virtual`. LavaTractor's slot 28 is
`set__11LavaTractor`. SkulBoat's slot 28 is `set__8SkulBoat`. GothTrap slot 19
is `setPassableState__8GothTrapb` and HateBridge slot 19 is
`setPassableState__10HateBridgeb`. Those ELF names are not the `norrath_*`
entry points authored above. The tables keep the ELF names.

## Build-plan entries needed

Append these as `independently_authored_source` inputs. Profile `authored_cpp`.
`header_closure` is empty. Put the same sha256 under `files` in both the plan
and the snapshot. Snapshot `emitted_functions` is the one function and byte
count in the function table above, or `[]` for every view. Do not add the
`host/` contracts. A private copy of the plan and snapshot that appends these
26 units passed `--check-plan` (`829` authored inputs, `1488` source and
header files). The same private plan then linked. That link has 684 function
bodies, 914 unresolved symbols, and zero COMMON. The 26 names in "Imports
closed" are absent from that unresolved list. The two world-state names and
the 12 callees below are present. The integrator assigns `input_order`.
That link was not executed.

## Imports closed

`norrath_actor_add_medium_run_list`,
`norrath_goth_trap_set_passable_state`,
`norrath_hate_bridge_set_passable_state`,
`norrath_construct_Vehicle`,
`norrath_vehicle_reset`,
`norrath_actors_b_view_448a70`,
`norrath_actors_b_view_448e30`,
`norrath_actors_b_view_44a420`,
`norrath_actors_b_view_44a4b0`,
`norrath_actors_b_view_44c8c0`,
`norrath_actors_b_view_44ee08`,
`norrath_actors_b_view_44eeb0`,
`norrath_actors_b_view_44ef58`,
`norrath_actors_b_view_44f000`,
`norrath_actors_b_view_44f0a8`,
`norrath_actors_b_view_44f150`,
`norrath_actors_b_view_450250`,
`norrath_actors_b_view_453230`,
`norrath_actors_b_view_4816d4`,
`norrath_actors_b_view_4844a0`,
`norrath_actors_b_view_484fd8`,
`norrath_actors_b_view_484fe0`,
`norrath_actors_b_view_484fe8`,
`norrath_actors_b_view_4850f0`,
`norrath_actors_b_view_4850f8`,
`norrath_vehicle_view_44ffd8`.

`norrath_world_get_door_height` and `norrath_world_check_for_water` were on
the actors-b list and are left open. They are world-state symbols
(`worldGetDoorHeight__FP12_worldHeaderi` at `0x17d3d0`, 64 bytes, and
`worldCheckForWater__FP12_worldHeaderG6Point3fPf` at `0x17db78`, 216 bytes).

## Imports opened

Callees this package calls and does not define:

`worldSetDoorCollision__FP12_worldHeaderii`,
`worldMoveDoor__FP12_worldHeaderiG6Point3s`,
`worldSetDoorHeight__FP12_worldHeaderif`,
`modelGetActiveChunks__FP12_modelHeaderi`,
`addPassenger__7Vehiclei`,
`processParams__7VehiclePPc`,
`gameLineInitPathState__FP17gameLinePathStatePcP6Point3`,
`refreshSettings__7Vehicle`,
`matMakeTransYaw__FP8Matrix34G6Point3s`,
`matToQuaternion__FP4QuatP8Matrix34`,
`quatToMat__FP8Matrix33P4Quat`,
`Reset__5Wheel`.

Already defined, so not opened: `norrath_list_remove`,
`norrath_list_oneway_link`, `norrath_game_object_construct`,
`norrath_actor_find_tag_string`, `norrath_actor_add_run_list`,
`norrath_world_find_height`, `norrath_actor_view_4844a4`, `lumpFindResource__FPcT0`,
`memset`, `sprintf`, `strcpy`.

The 14 virtual tables also reference these ELF function symbols. Names already
pending stay pending. Names that were not pending are new imports. This
package does not define any of them:

`GetNPCPosition__7VehicleP6Point3Ps`,
`GetPlayerPosition__7VehicleiP6Point3Ps`,
`GetPositionAndYaw__7VehicleP6Point3T1Ps`,
`_$_10HateBridge`, `_$_10Teleporter`, `_$_11LavaTractor`, `_$_11MissileTrap`,
`_$_15PushPhysicsProp`, `_$_7Shooter`, `_$_7Trigger`, `_$_7Vehicle`,
`_$_8GothTrap`, `_$_8HackDoor`, `_$_8HateGate`, `_$_8HateHole`, `_$_8HateInto`,
`_$_8SkulBoat`, `__pure_virtual`, `__tf10HateBridge`, `__tf10Teleporter`,
`__tf11LavaTractor`, `__tf11MissileTrap`, `__tf15PushPhysicsProp`,
`__tf7Shooter`, `__tf7Trigger`, `__tf7Vehicle`, `__tf8GothTrap`,
`__tf8HackDoor`, `__tf8HateGate`, `__tf8HateHole`, `__tf8HateInto`,
`__tf8SkulBoat`, `advanceState__8AutoProp`, `clone__10GameObject`,
`clone__10Teleporter`, `clone__11MissileTrap`, `clone__15PushPhysicsProp`,
`clone__7Shooter`, `clone__7Trigger`, `confuse__8Creaturei`, `death__8Creature`,
`doMyUniqueForces__7Vehicle`, `doMyUniqueForces__8SkulBoat`,
`eventActivate__11LavaTractor`, `eventActivate__7Vehicle`,
`eventCollisionGround__11LavaTractor`, `eventCollisionGround__7Vehicle`,
`eventCollisionWall__11LavaTractorP6Point3T1`,
`eventCollisionWall__7VehicleP6Point3T1`, `eventCurRPM__11LavaTractor`,
`eventCurRPM__7Vehicle`, `eventDeActivate__11LavaTractor`,
`eventDeActivate__7Vehicle`, `eventGasOff__11LavaTractor`,
`eventGasOff__7Vehicle`, `eventGasOn__11LavaTractor`, `eventGasOn__7Vehicle`,
`eventSteeringChange__11LavaTractor`, `eventSteeringChange__7Vehicle`,
`getMinibossName__8Creature`, `getModel__10AutoObject`, `getModel__10GameObject`,
`getModel__8Creature`, `getSkillRank__8Creaturei`, `initMiniboss__8Creature`,
`isEnemy__8CreatureP10GameObject`,
`msg_alert__10GameObjectifG6Point3P10GameObject`,
`msg_alert__8CreatureifG6Point3P10GameObject`,
`msg_collisionWorld__10GameObjectG6Point3`,
`msg_collisionWorld__8CreatureG6Point3`,
`msg_collision__10GameObjectP10GameObject`,
`msg_collision__15PushPhysicsPropP10GameObject`,
`msg_collision__7VehicleP10GameObject`, `msg_collision__8CreatureP10GameObject`,
`msg_draw__10AutoObject`, `msg_draw__10GameObject`, `msg_draw__10Teleporter`,
`msg_draw__15PushPhysicsProp`, `msg_draw__7Vehicle`,
`msg_getPokeName__10GameObjectPUsi`, `msg_getPokeName__7VehiclePUsi`,
`msg_getPokeName__8AutoPropPUsi`, `msg_getPokeName__8CreaturePUsi`,
`msg_hurt__10GameObjectfP10GameObjecti`, `msg_hurt__8AutoPropfP10GameObjecti`,
`msg_hurt__8CreaturefP10GameObjecti`, `msg_load__10GameObjectPc`,
`msg_load__15PushPhysicsPropPc`, `msg_load__7ShooterPc`, `msg_load__7TriggerPc`,
`msg_load__8AutoPropPc`, `msg_load__8CreaturePc`,
`msg_moveCollision__10GameObjectPP10GameObject`,
`msg_moveCollision__8CreaturePP10GameObject`, `msg_poke__10GameObjecti`,
`msg_poke__7Vehiclei`, `msg_poke__8AutoPropi`, `msg_poke__8Creaturei`,
`msg_route__10GameObjecti`, `msg_route__8Creaturei`, `msg_run__10HateBridge`,
`msg_run__10Teleporter`, `msg_run__11MissileTrap`, `msg_run__15PushPhysicsProp`,
`msg_run__7Shooter`, `msg_run__7Trigger`, `msg_run__7Vehicle`,
`msg_run__8AutoProp`, `msg_run__8GothTrap`, `msg_run__8HateGate`,
`msg_run__8HateHole`, `msg_run__8HateInto`, `msg_save__10GameObjectPc`,
`msg_save__15PushPhysicsPropPc`, `msg_save__7ShooterPc`, `msg_save__7TriggerPc`,
`msg_save__8AutoPropPc`, `msg_save__8CreaturePc`, `msg_trigger__10GameObjecti`,
`msg_trigger__11MissileTrapi`, `msg_trigger__8AutoPropi`,
`msg_trigger__8Creaturei`, `objectInside__7TriggerP10GameObject`,
`runSpells__8CreatureG6Point3`, `setActiveChunks__10Teleporter`,
`setPassableState__10HateBridgeb`, `setPassableState__8AutoPropb`,
`setPassableState__8GothTrapb`, `setPassableState__8HackDoorb`,
`setPassableState__8HateHoleb`, `setState__10Teleporteri`,
`setState__8AutoPropi`, `set__11LavaTractor`, `set__8SkulBoat`,
`stun__8Creaturei`, `turnToFace__8CreatureiG6Point3`, `updateEnemy__8Creature`.

## Authored but never executed

All five functions and all 21 views. Host contracts ran on the host compiler.
The EE objects were compiled and not linked into an executed program. No
startup frame, door, or vehicle path has called them.

## Unknown

- Whether a vehicle count of 32 or more is rejected before the list store.
  The image body does not check.
- The contents of `allVehicleSpecs` past its 64-byte ELF object. Reset still
  shifts the index by 6.
- The result of dividing by the float at object+820 when that float is 0.
- What `modelGetActiveChunks` returns when its second argument is not 0. The
  constructor always passes 0 and stores the full 64-bit return.
- Which virtual call actually dispatches through these tables, and which
  overrides are live. The slot words are recorded. Their behavior is not.
- The door-height and water-check bodies. They belong to world-state.
