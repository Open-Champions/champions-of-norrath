# Actor virtual methods, destructors and type info (3/3)

Package `w8-actor-virtuals-c`, branch `agents/w8-actor-virtuals-c`, based on `d4fa081`. Addresses and sizes are from the private `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`), whose function symbols were used to find each body; `_gp` is `0x004879f0`. Compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a claim of byte equality with the image. Nothing here has run on the EE or as gameplay.

## What was done

All 29 imports in the brief now have an authored definition under `experimental/sources/w8-actor-virtuals-c-*`, one commit per function. Two of them (`setPassableState__8GothTrapb`, `setPassableState__10HateBridgeb`) give the symbol name used by the actor tables to bodies the vehicle-actors package had already authored under `norrath_*` names; the other 27 are new bodies. A small shared helper header (`w8-actor-virtuals-c-bytes/bytes.hpp`, a renamed copy of the combat-reactions one) and a shared Hate particle header (`w8-actor-virtuals-c-hate-run-common/hate_run.hpp`) are the only non-function files.

## How each body was checked

`docs/agents/w8-actor-virtuals-c-compare.py` loads the small R5900 interpreter of `combat-reactions-compare.py` and runs one function twice per seed: the image's instructions from the private executable, and the freshly compiled and linked authored object at a scratch address. Both runs get the same randomized scenario memory and the same seeded callee stubs; callees are matched by their original ELF names. The run fails unless the callee logs (selected argument registers, pointed-to Point3 and string bytes, float register bits, stack arguments, the bytes the caller initialises in item descriptions) and every compared byte of scenario memory afterwards are equal, and for functions that return a value, V0 as well. It needs `NORRATH_EXE` (the private executable) and `NORRATH_EE` (the reviewed toolchain). Extra opcodes the interpreter lacked were added in the script: `sqrt.s` (the R5900 form takes the `ft` operand), `rsqrt.s`, and the 64-bit variable shifts.

Final sweep after the last source change: all 29 functions at 400 seeds each, zero mismatches. At commit time the seed counts were 200 to 300 for the small bodies, 400 to 1000 for the mid-sized ones and 1500 to 2500 for `msg_run__7Trigger`, `msg_run__8FireBomb`, `msg_run__7Shooter`, `msg_run__9Container` and `msg_run__15PushPhysicsProp`. For every function more than a trivial one I changed constants, offsets, strings, branch directions and loop bounds in the source and required the comparison to fail; the ones it could not detect were one-ulp changes to thresholds that the scenario floats never straddle, and equal-value `<=`/`<` swaps whose results are identical (listed under Unknown). Seventeen bodies also have a `host_check.cpp` beside the source (not a plan input; compile with `c++ -std=c++17 host_check.cpp`), which fails on a wrong branch, offset, call order or constant.

| symbol | image | image bytes | compiled bytes | source directory |
|---|---|---|---|---|
| `wakeMonsters__8creaturev` | 0x113790 | 32 | 12 (+48 local callback) | `w8-actor-virtuals-c-wake-monsters` |
| `sleepMonsters__8creaturev` | 0x113770 | 32 | 12 (+176 local callback) | `w8-actor-virtuals-c-sleep-monsters` |
| `saveActivate__9Savepoint` | 0x21c318 | 12 | 24 | `w8-actor-virtuals-c-save-activate` |
| `setState__10Teleporteri` | 0x21ce80 | 8 | 8 | `w8-actor-virtuals-c-teleporter-set-state` |
| `setActiveChunks__10Teleporter` | 0x312880 (weak) | 8 | 8 | `w8-actor-virtuals-c-teleporter-set-active-chunks` |
| `msg_run__8GothTrap` | 0x226290 | 8 | 8 | `w8-actor-virtuals-c-goth-trap-run` |
| `msg_run__8HateHole` | 0x226318 | 28 | 8 | `w8-actor-virtuals-c-hate-hole-run` |
| `msg_save__7ShooterPc` | 0x1e5d80 | 24 | 36 | `w8-actor-virtuals-c-shooter-save` |
| `msg_save__7TriggerPc` | 0x1d1ab8 | 36 | 52 | `w8-actor-virtuals-c-trigger-save` |
| `msg_save__8AutoPropPc` | 0x224d00 | 24 | 44 | `w8-actor-virtuals-c-auto-prop-save` |
| `msg_trigger__11MissileTrapi` | 0x1e5c78 | 44 | 24 | `w8-actor-virtuals-c-missile-trap-trigger` |
| `setPassableState__8HackDoorb` | 0x226548 | 48 | 72 | `w8-actor-virtuals-c-hack-door-passable` |
| `setPassableState__8AutoPropb` | 0x221040 | 212 | 364 | `w8-actor-virtuals-c-auto-prop-passable` |
| `setState__8AutoPropi` | 0x220f30 | 272 | 464 | `w8-actor-virtuals-c-auto-prop-set-state` |
| `msg_trigger__8AutoPropi` | 0x224ec8 | 256 | 272 | `w8-actor-virtuals-c-auto-prop-trigger` |
| `msg_run__8AutoProp` | 0x2207a8 | 652 | 1056 | `w8-actor-virtuals-c-auto-prop-run` |
| `setPassableState__8GothTrapb` | 0x225cc0 | 300 | 8 (calls the vehicle-actors body) | `w8-actor-virtuals-c-goth-trap-passable` |
| `setPassableState__10HateBridgeb` | 0x225df0 | 252 | 8 (calls the vehicle-actors body) | `w8-actor-virtuals-c-hate-bridge-passable` |
| `setPassableState__8HateHoleb` | 0x226338 | 172 | 288 | `w8-actor-virtuals-c-hate-hole-passable` |
| `msg_save__9ContainerPc` | 0x21d630 | 192 | 288 | `w8-actor-virtuals-c-container-save` |
| `msg_run__8HateInto` | 0x225910 | 472 | 536 | `w8-actor-virtuals-c-hate-into-run` |
| `msg_run__8HateGate` | 0x225ae8 | 472 | 536 | `w8-actor-virtuals-c-hate-gate-run` |
| `msg_save__15PushPhysicsPropPc` | 0x155bd0 | 276 | 228 | `w8-actor-virtuals-c-push-physics-prop-save` |
| `msg_run__7Trigger` | 0x1d1570 | 736 | 1284 | `w8-actor-virtuals-c-trigger-run` |
| `msg_run__11MissileTrap` | 0x1d9650 | 616 | 1088 | `w8-actor-virtuals-c-missile-trap-run` |
| `msg_run__8FireBomb` | 0x289a38 | 808 | 1160 | `w8-actor-virtuals-c-fire-bomb-run` |
| `msg_run__7Shooter` | 0x1da5d0 | 1808 | 3016 | `w8-actor-virtuals-c-shooter-run` |
| `msg_run__9Container` | 0x217a38 | 3416 | 5032 | `w8-actor-virtuals-c-container-run` |
| `msg_run__15PushPhysicsProp` | 0x1505d0 | 2732 | 6556 | `w8-actor-virtuals-c-push-physics-prop-run` |

Floats in the scenarios are small integers, quarter values or a few exact threshold values, because the interpreter rounds to nearest and the EE truncates.

## Observed

- **wakeMonsters / sleepMonsters** pass the address of a file-local callback (`wakeMonster__FP10GameObject` at 0x113c90, 48 bytes; `sleepMonster__FP10GameObject` at 0x113c10, 128 bytes) to `objectIterateRunLists`. Both callbacks are authored as unnamed-namespace functions in the same unit and appear in the unit's emitted-function inventory under their mangled local names. The callbacks test bit 0x20 set and bit 0x40 clear of the word at +12; wake clears bit 0x10 of the byte at +570; sleep returns when that byte is nonzero, sets 0x10, calls virtual slot 184 with the adjusted object and 0 unless the halfword at +572 is 10, and destroys the list at +576.
- **saveActivate__9Savepoint** stores 1 at +204 of its first argument. The HUD message source declares it with an `unsigned` argument; that does not matter at link level, but the image's body treats the first argument as the object address.
- **Teleporter setState, Teleporter setActiveChunks and GothTrap run** return at once. **HateHole run** passes the object to `msg_run__8AutoProp` unchanged.
- **Save routines.** Shooter stores the halfword at +82 and returns 2; Trigger stores the word at +100 and the byte at +96 and returns 5; AutoProp stores the word at +376 and returns 4; Container stores five string-pool ids (for the pointers at +76..+92), two truncated halfword floats (+28, +32) and the word at +104 and returns 28; PushPhysicsProp stores the float at +256, three string-pool ids (+268..+276) and thirteen float words (+96..+144) and returns 68.
- **AutoProp.** `setPassableState` moves the door at index +396 by (0,0,-256) with collision off when passable and back with collision on otherwise, only when bit 1 of +284 is set, and sets or clears bit 1 of +12; `setState` stores the state, builds the 64-bit active-chunk mask in +384 from 32-bit variable shifts (so state 31 and above give all 64 bits set), calls `modelGetActiveChunks` and the animation add and set-to-end routines and finally virtual slot 152; `msg_trigger` plays the lump/animation pairs for the names `gothtrap` and `fayactuator`; `msg_run` is the pipe-piston behaviour (name `pipepistona`) that damages and turns players inside 18 units.
- **HackDoor, GothTrap and HateBridge, HateHole `setPassableState`.** HackDoor calls the AutoProp body and sets bit 1 of +12. The GothTrap and HateBridge bodies the vehicle-actors package authored match the image over 400 seeds each. That package's notes say the return value is the last callee's V0; in the image the exit V0 is not the callee's result (the comparison fails if it is returned), so both definitions here return nothing.
- **Hate gate and Hate into** share one shape that differs in three constants (frame mask 7 or 1, step 8 or 2, particle definition at 0x7d7a90 or 0x7d7a40, which are the file-local `HateGateParticle` and `FireParticle`).
- **Run bodies using the shared random generator and trig**: FireBomb draws from the global `syncRandState` (0x483e5c) with the multiplier 1664525 and increment 1013904223, and uses `rsqrt.s`; MissileTrap, Shooter and Container divide without a zero check, so the divide is spelled as the image's `div`/`mfhi` pair; the image's constant tables are read through `gp` and were located by exact match against the image (the 0x4815c8 table for Container, 0x482570 for FireBomb, 0x47fdc4 for PushPhysicsProp).
- The Container body also uses file-local particle definitions at 0x7d7720 and 0x7d7770 (`fuseParticle`, `smokeParticle`); the Shooter and MissileTrap bodies read the words at +8 and +12 of `_4item$globalItem` (0x31f540) as model and texture handles, which the image data leaves zero.

## Inferred

- The callee identities come from the mangled symbols in the image; where an authored body exists under a `norrath_*` name (`norrath_actor_random` is `eRand`, `norrath_block_alloc` is `blockAlloc`, `norrath_lump_find_resource` is `lumpFindResource`, `norrath_sound_play_sequence`, `norrath_list_destroy`, the run-list routines, `norrath_game_object_find_tag_int`, `norrath_actor_find_tag_string`) it is called under that name and the comparison maps it back to the image name.
- The type descriptor tests in Trigger and PushPhysicsProp call the first function pointer of the other object's virtual table with no meaningful argument (the image leaves an index in A0) and compare with `__eq__C9type_infoRC9type_info` against `__tf13TriggerParams` or `__tf15PushPhysicsProp`.
- Dead stack stores are not reproduced: the third component raised by 24 in AutoProp's point, a scaled direction vector in FireBomb, and two scratch vectors in Container's item-drop loop, all of which the image writes and never reads.

## Imports closed

- `msg_run__11MissileTrap`
- `msg_run__15PushPhysicsProp`
- `msg_run__7Shooter`
- `msg_run__7Trigger`
- `msg_run__8AutoProp`
- `msg_run__8FireBomb`
- `msg_run__8GothTrap`
- `msg_run__8HateGate`
- `msg_run__8HateHole`
- `msg_run__8HateInto`
- `msg_run__9Container`
- `msg_save__15PushPhysicsPropPc`
- `msg_save__7ShooterPc`
- `msg_save__7TriggerPc`
- `msg_save__8AutoPropPc`
- `msg_save__9ContainerPc`
- `msg_trigger__11MissileTrapi`
- `msg_trigger__8AutoPropi`
- `saveActivate__9Savepoint`
- `setActiveChunks__10Teleporter`
- `setPassableState__10HateBridgeb`
- `setPassableState__8AutoPropb`
- `setPassableState__8GothTrapb`
- `setPassableState__8HackDoorb`
- `setPassableState__8HateHoleb`
- `setState__10Teleporteri`
- `setState__8AutoPropi`
- `sleepMonsters__8creaturev`
- `wakeMonsters__8creaturev`

Measured with a private plan copy (`docs/agents/w8-actor-virtuals-c-plan-entries.py --private-root`) on top of the shared plan at `d4fa081`: 1195 unresolved before, 1204 after, 29 closed, 38 opened, COMMON zero. The private build compiled 1436 units and linked once.

## Imports opened

New unresolved names (38). None has an owner on this branch, except the four particle storage names whose owner is noted.

- `__11FireBombletG6Point3sPcT1P6Point3i`
- `__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`
- `__4GoldG6Point3i`
- `__6PhPropG6Point3PcN22liPQ24item11descriptionf`
- `__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii`
- `addEffect__FiG6Point3`
- `animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`
- `bodyAddForce__FP5_bodyP6Point3N31`
- `bodyApplyForce__FP5_bodyG6Point3T1f`
- `bodyApplyImpulse__FP5_bodyG6Point3T1f`
- `bodyGetVelocity__FP5_bodyP6Point3T1`
- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`
- `gameDifficultyScale__Fv`
- `gamePositionVisible__FP6Point3f`
- `gameTreasureRamp__Fv`
- `genericMove__10GameObjectG6Point3P10GameObject`
- `getProxyMeshId__4itemPQ24item11description`
- `initStuff__6PhPropi`
- `makeExplosion__FG6Point3f`
- `makeRandomPotion__4itemPQ24item11description`
- `matUntransform__FP6Point3P8Matrix33T0`
- `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`
- `modelGetNmShadowPoints__FP12_modelHeader`
- `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`
- `norrath_anttrap_msg_run_FireParticle_bytes`
- `norrath_anttrap_msg_run_HateGateParticle_bytes`
- `norrath_container_msg_run_fuseParticle_bytes`
- `norrath_container_msg_run_smokeParticle_bytes`
- `objectActiveProcessing__FP10GameObjectii`
- `objectIterateRunLists__FPFP10GameObject_v`
- `objectRadialDamage__FP10GameObjectG6Point3iffiii`
- `soundStopSound__Fi`
- `strticmp__FPCcT0`
- `worldCheckForWater__FP12_worldHeaderG6Point3fPf`
- `worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3`
- `worldFindStandHeight__FP12_worldHeaderG6Point3f`
- `worldHitscan__FP12_worldHeaderG6Point3T1fi`
- `worldPerturbWater__FP12_worldHeaderG6Point3ff`

- `norrath_anttrap_msg_run_FireParticle_bytes` (0x7d7a40) and `norrath_anttrap_msg_run_HateGateParticle_bytes` (0x7d7a90) are the names the `w8-spell-static-init-b` branch gives the two 80-byte objects it constructs in `_GLOBAL_$I$msg_run__7AntTrap`; the Hate gate and Hate into bodies use those names and close when that branch is merged.
- `norrath_container_msg_run_fuseParticle_bytes` (0x7d7720) and `norrath_container_msg_run_smokeParticle_bytes` (0x7d7770) follow the same naming convention for two more file-local objects of the Container translation unit. No branch defines them yet; whoever authors that unit's static initialiser should use these names or tell this package the names to use.
- Already-unresolved names the new sources also call: `__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__eq__C9type_infoRC9type_info`, `__tf15PushPhysicsProp`, `activePlayer`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `create__4itemPcPQ24item11descriptionii`, `currentRunningObject`, `floorAdd__FG6Point3sfii`, `iatan2__Fff`, `icos__Fi`, `isin__Fi`, `modelGetActiveChunks__FP12_modelHeaderi`, `objectDoMyActions__FP10GameObjecti`, `objectSaveAddToStringPool__FPCc`, `player`, `scriptStart__FPc`, `strcasecmp`, `strstr`, `syncRandState`, `worldDifficulty`, `worldMoveDoor__FP12_worldHeaderiG6Point3s`, `worldSetDoorCollision__FP12_worldHeaderii`, `worldSetDoorHeight__FP12_worldHeaderif`.

## Build-plan entries needed

Do not add `host_check.cpp` or `host_hate_check.hpp`. The integrator assigns `input_order`, appends each entry's source and header closure to the plan `files` map and the snapshot, and raises `default_authored_inputs` by 29. The two shared headers (`sources/w8-actor-virtuals-c-bytes/bytes.hpp` and `sources/w8-actor-virtuals-c-hate-run-common/hate_run.hpp`) go in the `files` map once. Regenerate these lines with `python3 docs/agents/w8-actor-virtuals-c-plan-entries.py --toolchain <ee>` if a source changes. Profile is `authored_cpp`. The inventory lists local helper functions as well as the target (the two monster callbacks); the driver compares that inventory to the plan.

```json
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "_ZN12_GLOBAL__N_1L12wake_monsterEPv"}, {"compiled_body_bytes": 12, "name": "wakeMonsters__8creaturev"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_wake_monsters.o", "profile": "authored_cpp", "sha256": "a42cefd52d2bdaadea728f31c35c0bd5d654eac56c6018c0bafb3e9ee24e2bfd", "source": "sources/w8-actor-virtuals-c-wake-monsters/wake_monsters.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "_ZN12_GLOBAL__N_1L13sleep_monsterEPv"}, {"compiled_body_bytes": 12, "name": "sleepMonsters__8creaturev"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_sleep_monsters.o", "profile": "authored_cpp", "sha256": "26c12186f86af4c8766fd1ed2f2dc1c036e05838296e3979feb4c5e31c3f0fa8", "source": "sources/w8-actor-virtuals-c-sleep-monsters/sleep_monsters.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "saveActivate__9Savepoint"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_save_activate.o", "profile": "authored_cpp", "sha256": "5b93c665d61b7cf9cede5506c390ef6c6b1ec2ae8426a7701aab6096814cb395", "source": "sources/w8-actor-virtuals-c-save-activate/save_activate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setState__10Teleporteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_teleporter_set_state.o", "profile": "authored_cpp", "sha256": "102a106b18544f6ea8766f09c5d4408320d734b956bf3a407ac1e55e8ae98e1f", "source": "sources/w8-actor-virtuals-c-teleporter-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setActiveChunks__10Teleporter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_teleporter_set_active_chunks.o", "profile": "authored_cpp", "sha256": "c2ee29c8bfb45d428df671c4abcba5f6dc50e55a3add11763f2627bd53f36ca8", "source": "sources/w8-actor-virtuals-c-teleporter-set-active-chunks/set_active_chunks.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_run__8GothTrap"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_goth_trap_run.o", "profile": "authored_cpp", "sha256": "d98953d34a8591c1c5a24134ff53192a8922845718be497cd0d935abc1f8c557", "source": "sources/w8-actor-virtuals-c-goth-trap-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_run__8HateHole"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_hate_hole_run.o", "profile": "authored_cpp", "sha256": "22e13b51cfb992512ab876e1f063a77016cbf77b905de82b46fad8419973b64c", "source": "sources/w8-actor-virtuals-c-hate-hole-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "msg_save__7ShooterPc"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_shooter_save.o", "profile": "authored_cpp", "sha256": "0fb439ebd88d54429f015a43fd93a57898ef4833f2a525f109a384f60e0c0da4", "source": "sources/w8-actor-virtuals-c-shooter-save/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "msg_save__7TriggerPc"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_trigger_save.o", "profile": "authored_cpp", "sha256": "d6eb0babdb3ad6b39a10ab994c6e1120cc4c87ce208933f04b033c040cbb8bf8", "source": "sources/w8-actor-virtuals-c-trigger-save/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "msg_save__8AutoPropPc"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_auto_prop_save.o", "profile": "authored_cpp", "sha256": "0bc709b07e417fc3d317408eece73e5d635fca8159104f0ed3569d0d769bb2f9", "source": "sources/w8-actor-virtuals-c-auto-prop-save/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "msg_trigger__11MissileTrapi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_missile_trap_trigger.o", "profile": "authored_cpp", "sha256": "7180a7b63621df2687fe3262a854e1d7b3254a3d24c4efc93cab078b2bae14c2", "source": "sources/w8-actor-virtuals-c-missile-trap-trigger/trigger.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "setPassableState__8HackDoorb"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_hack_door_passable.o", "profile": "authored_cpp", "sha256": "bef8417e3b67d649c146f3126adbd2e36261e922278acbe61fc859fa4b290757", "source": "sources/w8-actor-virtuals-c-hack-door-passable/set_passable_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 364, "name": "setPassableState__8AutoPropb"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_auto_prop_passable.o", "profile": "authored_cpp", "sha256": "3fa68153308ce8fca8e928d354ab22bd3ea7c5f02e1e0752c422d8b88faa3b0f", "source": "sources/w8-actor-virtuals-c-auto-prop-passable/set_passable_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 464, "name": "setState__8AutoPropi"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_auto_prop_set_state.o", "profile": "authored_cpp", "sha256": "41a0a6a8213fab0a378c0f82061696fc72221fade5d7ae3e97ef8999ca212184", "source": "sources/w8-actor-virtuals-c-auto-prop-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 272, "name": "msg_trigger__8AutoPropi"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_auto_prop_trigger.o", "profile": "authored_cpp", "sha256": "e7ac2e4ff49ee7c394c6d522b2d040bc63266a2a593fb027baf167db2f56ded6", "source": "sources/w8-actor-virtuals-c-auto-prop-trigger/trigger.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1056, "name": "msg_run__8AutoProp"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_auto_prop_run.o", "profile": "authored_cpp", "sha256": "a122c4c030f6e751e518c6e13233307849dfafeffdf25f2324d3dbd8f76ba068", "source": "sources/w8-actor-virtuals-c-auto-prop-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setPassableState__8GothTrapb"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_goth_trap_passable.o", "profile": "authored_cpp", "sha256": "65e02833f97ccaf9a4da20ebc6e02a8a762b0d081777b9a06dad2d42c938ca94", "source": "sources/w8-actor-virtuals-c-goth-trap-passable/set_passable_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setPassableState__10HateBridgeb"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w8c_hate_bridge_passable.o", "profile": "authored_cpp", "sha256": "beba2bb9a202554e5462c17104791f9b0628a1e3b626f3185726c154c17deb59", "source": "sources/w8-actor-virtuals-c-hate-bridge-passable/set_passable_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 288, "name": "setPassableState__8HateHoleb"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_hate_hole_passable.o", "profile": "authored_cpp", "sha256": "7f3c40ac35bae8f912102f9d15ce6eb2da478da339b07d4446afd253193414f2", "source": "sources/w8-actor-virtuals-c-hate-hole-passable/set_passable_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 288, "name": "msg_save__9ContainerPc"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_container_save.o", "profile": "authored_cpp", "sha256": "b20e03f8ec244564321d8e74aca5c28f8d8124f57af80dc93e0d3da9636c8d80", "source": "sources/w8-actor-virtuals-c-container-save/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 536, "name": "msg_run__8HateInto"}], "header_closure": ["sources/w8-actor-virtuals-c-hate-run-common/hate_run.hpp", "sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_hate_into_run.o", "profile": "authored_cpp", "sha256": "2db51610a9f6d4d087d5cef57049c06980892063b8d7989a2f3b9827b31851da", "source": "sources/w8-actor-virtuals-c-hate-into-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 536, "name": "msg_run__8HateGate"}], "header_closure": ["sources/w8-actor-virtuals-c-hate-run-common/hate_run.hpp", "sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_hate_gate_run.o", "profile": "authored_cpp", "sha256": "6feafa56d4df6a4221f4e1ad7e961909bec5970ea2f05303bb04e6c1c5e95d18", "source": "sources/w8-actor-virtuals-c-hate-gate-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 228, "name": "msg_save__15PushPhysicsPropPc"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_push_physics_prop_save.o", "profile": "authored_cpp", "sha256": "281c54c9613cd35313f5368eb9e890deeb832182e4ec525f53b22ea15bf7151c", "source": "sources/w8-actor-virtuals-c-push-physics-prop-save/save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1284, "name": "msg_run__7Trigger"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_trigger_run.o", "profile": "authored_cpp", "sha256": "7cf286451d6cced9b1507df831b87133ee84dd1c52ed711429fcbec1e1595c47", "source": "sources/w8-actor-virtuals-c-trigger-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1088, "name": "msg_run__11MissileTrap"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_missile_trap_run.o", "profile": "authored_cpp", "sha256": "df833bd81b4ef61f56abbec5876dbc67b5c9e07732f5ebe6b869639be50a3850", "source": "sources/w8-actor-virtuals-c-missile-trap-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1160, "name": "msg_run__8FireBomb"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_fire_bomb_run.o", "profile": "authored_cpp", "sha256": "95f1e74ceb8399cd793220f6d5e6ef69616f40bf60b0fb716fe9809a70381138", "source": "sources/w8-actor-virtuals-c-fire-bomb-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3016, "name": "msg_run__7Shooter"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_shooter_run.o", "profile": "authored_cpp", "sha256": "670619c50af39ea252ae34f65277ada57d2f38eff417db7219d533bc25a39ea2", "source": "sources/w8-actor-virtuals-c-shooter-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 5032, "name": "msg_run__9Container"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_container_run.o", "profile": "authored_cpp", "sha256": "ae06d8184fc19e68ee420fbc38565b4f2ff6e78fd36ce6ce94212c9f9be572c1", "source": "sources/w8-actor-virtuals-c-container-run/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 6556, "name": "msg_run__15PushPhysicsProp"}], "header_closure": ["sources/w8-actor-virtuals-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w8c_push_physics_prop_run.o", "profile": "authored_cpp", "sha256": "fb6d6f587757287f4afe4b79e8658df0f26dbb2f16b6804152fe166a05426ac1", "source": "sources/w8-actor-virtuals-c-push-physics-prop-run/run.cpp"}
```

## Authored but never executed

Everything. The bodies ran only in the R5900 interpreter of the comparison script against the image with stubbed callees, and in native host checks with stubbed callees. No callee behind a stub (movement, water, hitscan, particles, items, sound, scripts, the body physics routines, the sparks, missile, bomblet and prop constructors) is authored here or was run with these bodies. Paths that need real callee state were exercised only with randomized stub answers. Not exercised at all: a zero divisor in the three unchecked divisions (MissileTrap period, Shooter period, Container debris extents; the image's result is undefined there), the Trigger list scan beyond 128 entries (the source stops at 128, the image has no bound), and the `__throw_bad_typeid` call in PushPhysicsProp (unreachable, kept for fidelity).

## Unknown

- What any of the objects' fields, flag bits, state numbers and tags mean beyond what the callee names show; the names above are the image's own symbols, not behaviours.
- Which class owns each flag at +12, +104 and +284 and whether "AutoProp", "Container" and "PushPhysicsProp" bodies share a base beyond what the vtable slots show.
- What `item::globalItem` words +8 and +12 hold at run time (zero in the file image).
- The uninitialised stack bytes of the item description in the Container drop path; only the bytes the image writes are written, and the comparison replaces the whole struct at each callee.
- Boundary behaviour the comparison cannot see: one-ulp changes to the thresholds 518400.0, 0.001, 0.01, 0.094 and 0.149 where the scenario floats never straddle them, and `<=` versus `<` where both sides are the same float.
- The retail edition of the executable; only the one file above was used.
