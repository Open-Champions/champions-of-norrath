# RTTI, vtables and entity class methods (c)

Package `agents/w9-rtti-vtables-entities-c`. Addresses and sizes are from the pinned private `SLUS_205.65` (`.text`/`.data`/`.bss`). Nothing here has run on the EE or as gameplay. Compiled sizes are the reviewed EE g++ `authored_cpp` objects; they are larger than the image bodies because field access goes through byte helpers, and they are not a claim of byte equality with the image.

## Evidence

Every function has a `host_check.cpp` beside its source (not a plan input). Each host check defines `NORRATH_RTTI_C_HOST`, stubs every callee, and fails on a wrong offset, constant, branch, call order, argument or early return.

Every function except the data object was also compared against the image in a private paired-run harness (an R5900 interpreter outside the repository; it loads the private executable and the reviewed toolchain, nothing from it is stored). The harness compiles the authored unit with the plan's `authored_cpp` flags, links it at a scratch address with every undefined symbol bound to the image's address, then runs the original function and the authored function from identical memory snapshots with identical Python stubs for every callee, virtual slot and (where the image's own copy needs unsupported MMI opcodes) `strcpy`/`strcat`. A scenario passes only when the stub call logs (argument registers, the bytes behind pointer arguments, float argument registers, and selected object fields at call time) and every changed byte outside the stack are equal; `norrath_construct_AutoObject` also compares the returned V0. The last run of each function had zero mismatches; scenario counts ranged from 5 (Teleporter `setState`) to 1584 (MaleDarkElf `initType`), with instruction coverage of the original recorded for the larger bodies. All uncovered original words were nops, branch-likely delay slots that are annulled when the branch is taken, or the unreachable paths listed below. Changing the argument -10 in the DoorSwing trigger body made its paired run fail with the argument logged; mutation was not tried for the other functions.

The sources were derived from the image's instructions read locally, then written in my own words and structure. They carry no assembly. Names are the sized ELF symbols of the image where one exists; the `norrath_*` names are the ones the authored callers import (see "Emitted names").

## Functions (image symbol, address, bytes; authored compiled bytes)

| Emitted name | Image symbol | Address | Image bytes | Authored bytes |
|---|---|---|---|---|
| `msg_save__7TriggerPc` | `msg_save__7TriggerPc` | 0x1d1ab8 | 36 | 52 |
| `msg_save__8AutoPropPc` | `msg_save__8AutoPropPc` | 0x224d00 | 24 | 44 |
| `msg_save__9AutoChestPc` | `msg_save__9AutoChestPc` | 0x224ab8 | 72 | 124 |
| `msg_save__9DoorSwingPc` | `msg_save__9DoorSwingPc` | 0x21bf90 | 56 | 112 |
| `msg_trigger__10DoorSecreti` | `msg_trigger__10DoorSecreti` | 0x21c008 | 68 | 84 |
| `msg_trigger__11MissileTrapi` | `msg_trigger__11MissileTrapi` | 0x1e5c78 | 44 | 24 |
| `msg_trigger__9DoorSwingi` | `msg_trigger__9DoorSwingi` | 0x21bf50 | 52 | 52 |
| `norrath_construct_AutoObject` | `__10AutoObjectPcG6Point3sPPc` | 0x21e548 | 1732 | 2192 |
| `norrath_lava_monster_get_model` | `getModel__11LavaMonster` | 0x1ee280 | 32 | 44 |
| `norrath_male_dark_elf_init_type` | `initType__11MaleDarkElfb` | 0x2870d8 | 1336 | 1920 |
| `norrath_male_dark_elf_set_state` | `setState__11MaleDarkElfi` | 0x288818 | 3264 | 4892 |
| `norrath_mummy_init_type` | `initType__5Mummyb` | 0x2371a8 | 1196 | 1512 |
| `norrath_mummy_set_state` | `setState__5Mummyi` | 0x239008 | 2196 | 3480 |
| `norrath_mummy_set_weapon` | `setWeapon__5Mummy` | 0x23bcf0 | 104 | 140 |
| `norrath_orc_init_type` | `initType__3Orcb` | 0x2270b8 | 1408 | 2200 |
| `norrath_orc_set_weapon` | `setWeapon__3Orc` | 0x226fb8 | 256 | 452 |
| `norrath_rtti_ptr` | `__rtti_ptr` | 0x2fe658 | 32 | 36 |
| `norrath_skeleton_init_type` | `initType__8Skeletonb` | 0x2442b8 | 1088 | 1488 |
| `norrath_skeleton_set_state` | `setState__8Skeletoni` | 0x2453f0 | 1352 | 1900 |
| `norrath_skelight_init_weapon` | `initWeapon__8Skelight` | 0x246598 | 776 | 476 |
| `norrath_skelight_set_state` | `setState__8Skelighti` | 0x246cf0 | 544 | 780 |
| `norrath_soul_set_state` | `setState__4Souli` | 0x1f46c8 | 1672 | 2256 |
| `norrath_static_fire_init` | `init__10StaticFire` | 0x28df70 | 276 | 640 |
| `norrath_ti_6PhProp` (data) | `__ti6PhProp` | 0x8eb248 | 12 (.bss) | 12 (.bss) |
| `norrath_wood_elf_soldier_init_type` | `initType__14WoodElfSoldier` | 0x249c28 | 96 | 184 |
| `norrath_wood_elf_soldier_set_state` | `setState__14WoodElfSoldieri` | 0x248690 | 2472 | 4276 |
| `setPassableState__10HateBridgeb` | `setPassableState__10HateBridgeb` | 0x225df0 | 252 | 456 |
| `setPassableState__8GothTrapb` | `setPassableState__8GothTrapb` | 0x225cc0 | 300 | 504 |
| `setPassableState__8HackDoorb` | `setPassableState__8HackDoorb` | 0x226548 | 48 | 72 |
| `setState__10Teleporteri` | `setState__10Teleporteri` | 0x21ce80 | 8 | 8 |
| `setState__7Critteri` | `setState__7Critteri` | 0x1f0f48 | 756 | 1636 |
| `setWeapons__6Goblin` | `setWeapons__6Goblin` | 0x22c180 | 204 | 224 |
| `specificInit__7CritterPcT1` | `specificInit__7CritterPcT1` | 0x1f1240 | 1488 | 2984 |
| `subTurnAnimation__3Cat` | `subTurnAnimation__3Cat` | 0x1eef20 | 16 | 48 |
| `switchToWalk__3Cat` | `switchToWalk__3Cat` | 0x1eea70 | 396 | 488 |

## Observed, inferred, unknown

Observed (per function, in each source header comment): offsets and widths of every object field read or written, the immediate constants and the single-float bit patterns, jump-table contents (read from the image), branch order, call order and arguments, and which returns skip the final halfword store at `object+572`. The common frame: `object+36` points to a table of resource headers, `object+72` is the virtual table (a signed halfword adjustment followed by the function word per 8-byte slot), `object+288` is the animation state, and the halfword at `object+572` is the stored state. The `setState` bodies for MaleDarkElf, Skeleton, Skelight, Soul, Mummy and WoodElfSoldier call `setStateProcessing__8CreaturePi` first with the address of a stack word that holds the requested state; a nonzero result returns without storing.

Inferred: that the `norrath_*` thin names correspond to the image symbols in the table above (each was matched by the constructor that imports it plus the paired run); that the 4-slot, 8-byte weapon records at the offsets named in the sources are weapon effects (shared by Skeleton, Orc, Mummy and MaleDarkElf initType through `weapon_slot.hpp`); that `skills` is a table of object pointers; that `__rtti_ptr` fills a legacy pointer type_info (name, table, target).

Unknown: the meaning of the state numbers, kind/type numbers, flag bits and most field offsets; the layouts of the header tables behind `object+36` and of `skeletonResMap`; what the called world, model, matrix, particle and spell routines compute; whether the per-class tables hold animation headers in every slot used.

## Emitted names

The eighteen `norrath_*` function names are emitted under that name only, because that is what the authored constructors import: `norrath_rtti_ptr` (`__rtti_ptr`), `norrath_construct_AutoObject` (`__10AutoObjectPcG6Point3sPPc`), `norrath_lava_monster_get_model`, `norrath_male_dark_elf_{init_type,set_state}`, `norrath_mummy_{init_type,set_state,set_weapon}`, `norrath_orc_{init_type,set_weapon}`, `norrath_skeleton_{init_type,set_state}`, `norrath_skelight_{init_weapon,set_state}`, `norrath_soul_set_state`, `norrath_static_fire_init` and `norrath_wood_elf_soldier_{init_type,set_state}`. The other sixteen functions use the image symbol. Anything else that still imports the original names (`initType__5Mummyb`, `setState__14WoodElfSoldieri` and so on, for example from a vtable view) stays unresolved until a second symbol or an alias is agreed with the integrator. `norrath_ti_6PhProp` is the 12-byte zero-initialised object `__ti6PhProp` (0x8eb248, `.bss`).

## Imports closed

- `msg_save__7TriggerPc`
- `msg_save__8AutoPropPc`
- `msg_save__9AutoChestPc`
- `msg_save__9DoorSwingPc`
- `msg_trigger__10DoorSecreti`
- `msg_trigger__11MissileTrapi`
- `msg_trigger__9DoorSwingi`
- `norrath_construct_AutoObject`
- `norrath_lava_monster_get_model`
- `norrath_male_dark_elf_init_type`
- `norrath_male_dark_elf_set_state`
- `norrath_mummy_init_type`
- `norrath_mummy_set_state`
- `norrath_mummy_set_weapon`
- `norrath_orc_init_type`
- `norrath_orc_set_weapon`
- `norrath_rtti_ptr`
- `norrath_skeleton_init_type`
- `norrath_skeleton_set_state`
- `norrath_skelight_init_weapon`
- `norrath_skelight_set_state`
- `norrath_soul_set_state`
- `norrath_static_fire_init`
- `norrath_ti_6PhProp`
- `norrath_wood_elf_soldier_init_type`
- `norrath_wood_elf_soldier_set_state`
- `setPassableState__10HateBridgeb`
- `setPassableState__8GothTrapb`
- `setPassableState__8HackDoorb`
- `setState__10Teleporteri`
- `setState__7Critteri`
- `setWeapons__6Goblin`
- `specificInit__7CritterPcT1`
- `subTurnAnimation__3Cat`
- `switchToWalk__3Cat`

All 35 names of the brief. Measured with a private copy of the plan (outside git) added to a private copy of the snapshot on this base (`ad8cd7c`): 2351 inputs (2316 authored on the base plus these 35), 1910 function bodies, 1500 unresolved imports, COMMON symbols zero, none of the 35 names unresolved. I did not rebuild the base without them, so the 24 opened and 35 closed names imply 1511 unresolved before, if all 35 were unresolved on this base.

## Imports opened

Names referenced only by these units that are unresolved in that measured build (original names; Point3 arguments are passed by address, as the callers of the other authored units do):

- `__tf15SpellConeOfFire` (0x319918, 112 bytes)
- `addCommander__14WoodElfSoldier` (0x249ce8)
- `bNextToEdge__14WoodElfSoldier` (0x2494f0)
- `commanderValid__14WoodElfSoldier` (0x249e60)
- `dropItem__14WoodElfSoldieri` (0x249038), `dropItem__8Skeletoni` (0x245938), `dropItem__8Skelighti` (0x2473f0)
- `dropTreasure__8CreatureG6Point3iffffffPc` (0x10c338; Point3 by address, six floats in the float argument registers, name pointer last)
- `dropWeapon__5Mummy` (0x2398a0)
- `findTagInt__10GameObjectPci` (0x140f60)
- `fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff` (0x1e5f98)
- `getSoulAttachMatrix__6MinionP8Matrix34` (0x235420)
- `leaveCommander__14WoodElfSoldier` (0x249e10)
- `matInvert__FP8Matrix34T0` (0x157d78), `matTransform__FP6Point3P8Matrix34T0` (0x159ae0)
- `modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f` (0x16ca18)
- `modelGetBoundingBox__FP12_modelHeaderlPiN52` (0x16c820)
- `newWind__10StaticFirei` (0x28eff8)
- `setParameters__8Creaturefffffi` (0x1092f0)
- `terminateSpell__8CreatureRC9type_info` (0x113890)
- data: `skeletonResMap` (0x42d8d0, 800 bytes, read as 104-byte records indexed by the word at +968), `soulParticle` (0x7d6d80, passed by address to `P_AddParticle__FP11ParticleDefP6Point3T1f`)
- `norrath_legacy_pointer_type_info_vtable_view`: the address of `_vt$19__pointer_type_info` (0x4538f8, 24 bytes) that `norrath_rtti_ptr` stores into a type_info object. The package that owns the other type_info table views should supply it.
- `norrath_rtti_entities_a_view_44ed78`: the `_vt$10AutoObject` view (0x44ed78, 144 bytes) authored on the `agents/w9-rtti-vtables-entities-a` branch; `norrath_construct_AutoObject` stores its address at `object+72`. It is unresolved here only until that branch is integrated.

Already-unresolved names these units also call: `__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `_ctype_`, `animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animGlobalSync__FP14AnimationState`, `blockAlloc__Fi`, `eRand__10GameObject`, `getSmartRandomDestination__8Creaturei`, `matMul__FP8Matrix34N20`, `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `objectAddToRunList__FP10GameObject`, `objectAlwaysDraw__FP10GameObject`, `resetRouting__8Creature`, `setStateProcessing__8CreaturePi`, `skills` (252 bytes at 0x34b440, read as a table of object pointers), `strcat`, `strticmp__FPCcT0`. Defined elsewhere on this base and used here: `norrath_game_object_construct`, `norrath_factory_world_pointer_word`, `levelName`, `cutMode`, `lumpFindResource__FPcT0`, `findTagString__10GameObjectPci`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animInitState__FP14AnimationState`, `animResetFramestates__FP14AnimationState`, `listDestroy__FP8ListHead`, `memAlloc__FiPc`, `objectAddToDeleteList__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `scriptStart__FPc`, `setPassableState__8AutoPropb`, `sprintf`, `strcasecmp`, `strcpy`, `P_AddParticle__FP11ParticleDefP6Point3T1f` and the three `world*Door*` routines.

## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order`. Do not add any `host_check.cpp`. Add these two headers to the plan and snapshot `files` lists: `sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp` and `sources/w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp` (`host_vcall.hpp` there is for host checks only). Each unit emits exactly one function (the byte helpers are `always_inline`); `w9-rtti-vtables-entities-c-ti-ph-prop` emits none (it defines one 12-byte `.bss` object, `norrath_ti_6PhProp`). The 35 entries validated with `tools/build_partial.py --check-plan` and built, in a private copy of the plan, together with the 2316 existing inputs. Lines are `sources/<name>/...`, listed in alphabetical order:

```json
{"emitted_functions":[{"compiled_body_bytes":48,"name":"subTurnAnimation__3Cat"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"sub_turn.o","profile":"authored_cpp","sha256":"46e14340cc6757d758ff763c3c623b668caf490540ca25b7694286ac170d423f","source":"sources/w9-rtti-vtables-entities-c-cat-sub-turn-animation/sub_turn.cpp"}
{"emitted_functions":[{"compiled_body_bytes":488,"name":"switchToWalk__3Cat"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"switch_to_walk.o","profile":"authored_cpp","sha256":"c3615679d8c5640577484bb0c7b721180a0c3ccb48d966a6bf7b5fd177042c01","source":"sources/w9-rtti-vtables-entities-c-cat-switch-to-walk/switch_to_walk.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2192,"name":"norrath_construct_AutoObject"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"construct.o","profile":"authored_cpp","sha256":"b6a9d5b88e38267bb7eb01b6a4efb46e26e2b7b6b9896aa08574e05a22f1fa0d","source":"sources/w9-rtti-vtables-entities-c-construct-auto-object/construct.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1636,"name":"setState__7Critteri"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"9267ec5197740f1ca60e6cb0efe158ecd69310f06a6bb76b9d3f9ec68aaa8f92","source":"sources/w9-rtti-vtables-entities-c-critter-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":3020,"name":"specificInit__7CritterPcT1"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"specific_init.o","profile":"authored_cpp","sha256":"4352e5375a517cf5866e5c856980ffb073c675760d0259833c8ce65d2aaea138","source":"sources/w9-rtti-vtables-entities-c-critter-specific-init/specific_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"setWeapons__6Goblin"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_weapons.o","profile":"authored_cpp","sha256":"76bac7113c39e44267d6acd9bfd1aa19ffaca58852e61c6bc5ea443b091afb5d","source":"sources/w9-rtti-vtables-entities-c-goblin-set-weapons/set_weapons.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"norrath_lava_monster_get_model"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"get_model.o","profile":"authored_cpp","sha256":"38cff7b735bb3869f261e5fc3ab7408b6d530c38b613a90516a8fd9d4a1453cd","source":"sources/w9-rtti-vtables-entities-c-lava-monster-get-model/get_model.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1920,"name":"norrath_male_dark_elf_init_type"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp","sources/w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"],"kind":"independently_authored_source","object_name":"init_type.o","profile":"authored_cpp","sha256":"07702c497b460196baf8b30ac53830ecc59d8e27856b48caa9cea62306477e9a","source":"sources/w9-rtti-vtables-entities-c-male-dark-elf-init-type/init_type.cpp"}
{"emitted_functions":[{"compiled_body_bytes":4892,"name":"norrath_male_dark_elf_set_state"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"3fb5653b3595a87f788c6c03300404a429fc5d286d0f797338fcd14546d5a4cd","source":"sources/w9-rtti-vtables-entities-c-male-dark-elf-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1512,"name":"norrath_mummy_init_type"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp","sources/w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"],"kind":"independently_authored_source","object_name":"init_type.o","profile":"authored_cpp","sha256":"b1b6c6a481d125b51469e6df9db85e8b67b85a27184d4c283d3143e736a70953","source":"sources/w9-rtti-vtables-entities-c-mummy-init-type/init_type.cpp"}
{"emitted_functions":[{"compiled_body_bytes":3480,"name":"norrath_mummy_set_state"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"ba3debbff189535e1ed5848439e572ba0bc4cdbcec1eadcc5df146be1409051d","source":"sources/w9-rtti-vtables-entities-c-mummy-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":140,"name":"norrath_mummy_set_weapon"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_weapon.o","profile":"authored_cpp","sha256":"f2db4d8d2f1301d9554ec5ad6256079daa49955c41ebc4f388a53c3a6e768fdf","source":"sources/w9-rtti-vtables-entities-c-mummy-set-weapon/set_weapon.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2200,"name":"norrath_orc_init_type"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp","sources/w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"],"kind":"independently_authored_source","object_name":"init_type.o","profile":"authored_cpp","sha256":"8001c7849c19590a6da05453d3a0e8194f5b44fa4227846c5b6c8df4cc7774b0","source":"sources/w9-rtti-vtables-entities-c-orc-init-type/init_type.cpp"}
{"emitted_functions":[{"compiled_body_bytes":452,"name":"norrath_orc_set_weapon"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_weapon.o","profile":"authored_cpp","sha256":"a17eb68b78897ac0ff48a479db9561d832900d4ffdadc14d6bfda3565e3eb329","source":"sources/w9-rtti-vtables-entities-c-orc-set-weapon/set_weapon.cpp"}
{"emitted_functions":[{"compiled_body_bytes":504,"name":"setPassableState__8GothTrapb"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"passable.o","profile":"authored_cpp","sha256":"b7e8787dc44351c5e8b4eaecf982a6d6da21eee88f76af2d9dca143933533d71","source":"sources/w9-rtti-vtables-entities-c-passable-goth-trap/passable.cpp"}
{"emitted_functions":[{"compiled_body_bytes":72,"name":"setPassableState__8HackDoorb"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"passable.o","profile":"authored_cpp","sha256":"e5f8c0eba89a3ee8637c74718c9c78fe26dfcd10a1357ac2c0106177adfff022","source":"sources/w9-rtti-vtables-entities-c-passable-hack-door/passable.cpp"}
{"emitted_functions":[{"compiled_body_bytes":456,"name":"setPassableState__10HateBridgeb"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"passable.o","profile":"authored_cpp","sha256":"e92c9aa9a116743ce16cbfc798ca7bc8a15dbae7a220dfe5522f80d2069e0d72","source":"sources/w9-rtti-vtables-entities-c-passable-hate-bridge/passable.cpp"}
{"emitted_functions":[{"compiled_body_bytes":36,"name":"norrath_rtti_ptr"}],"header_closure":[],"kind":"independently_authored_source","object_name":"rtti_ptr.o","profile":"authored_cpp","sha256":"df8c45c1161c83dd3a98eafed7c6a57410113121fd6d810d7bfd979511aa1129","source":"sources/w9-rtti-vtables-entities-c-rtti-ptr/rtti_ptr.cpp"}
{"emitted_functions":[{"compiled_body_bytes":124,"name":"msg_save__9AutoChestPc"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"save.o","profile":"authored_cpp","sha256":"cc652f65f6891c972caf20559a6019285a19db6f1393f0f14986aa39614a29c1","source":"sources/w9-rtti-vtables-entities-c-save-auto-chest/save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"msg_save__8AutoPropPc"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"save.o","profile":"authored_cpp","sha256":"ef0e4f635432e1d64598411d01d09c86ab6c30387eec4b2d83dd18485585c705","source":"sources/w9-rtti-vtables-entities-c-save-auto-prop/save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"msg_save__9DoorSwingPc"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"save.o","profile":"authored_cpp","sha256":"c403465c4712d54a44a0f31c7b50589dde3457a0bc7b27d1e9f20d09d207f2c5","source":"sources/w9-rtti-vtables-entities-c-save-door-swing/save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"msg_save__7TriggerPc"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"save.o","profile":"authored_cpp","sha256":"6034462f436483d890a36d33e797e416e0eac30642c99ba7b7406831d3c789a5","source":"sources/w9-rtti-vtables-entities-c-save-trigger/save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1488,"name":"norrath_skeleton_init_type"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp","sources/w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"],"kind":"independently_authored_source","object_name":"init_type.o","profile":"authored_cpp","sha256":"866a07c993eedb0c1f2d008465ae710d53a1916824f20c8beafc1cbb7fe366b8","source":"sources/w9-rtti-vtables-entities-c-skeleton-init-type/init_type.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1900,"name":"norrath_skeleton_set_state"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"a6d4722fb31f9fa74749fea66607c2484fd34597b8014268725c0188fb875a1b","source":"sources/w9-rtti-vtables-entities-c-skeleton-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":476,"name":"norrath_skelight_init_weapon"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"init_weapon.o","profile":"authored_cpp","sha256":"1741cedd0998c9ab61a6a6a3e90c86ecfd7873585884f7504ae3610c45e7a6ad","source":"sources/w9-rtti-vtables-entities-c-skelight-init-weapon/init_weapon.cpp"}
{"emitted_functions":[{"compiled_body_bytes":780,"name":"norrath_skelight_set_state"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"461a50aa661db9f5723d2c82b0b42eec08e6ad292860f0433b7efaa6339ebc85","source":"sources/w9-rtti-vtables-entities-c-skelight-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2256,"name":"norrath_soul_set_state"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"23c0298685ccbfe9aff74060ff8c4db4021b30c758e1786aedfcf42a56ccad7e","source":"sources/w9-rtti-vtables-entities-c-soul-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":640,"name":"norrath_static_fire_init"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"init.o","profile":"authored_cpp","sha256":"488c7c5ff82ab8b2740753045c50657ba097992e8849554053b7ad34f2ad99f8","source":"sources/w9-rtti-vtables-entities-c-static-fire-init/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"setState__10Teleporteri"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"5652b0545d94423f79f08837682f0dffd91cc804e6faef320b3218825afbfbc3","source":"sources/w9-rtti-vtables-entities-c-teleporter-set-state/set_state.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"storage.o","profile":"authored_cpp","sha256":"a5d881ca9b98ce7c7e3fc3dc026fdc623336140c9dd1fe283c0ba2f68e50a0c0","source":"sources/w9-rtti-vtables-entities-c-ti-ph-prop/storage.cpp"}
{"emitted_functions":[{"compiled_body_bytes":84,"name":"msg_trigger__10DoorSecreti"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"trigger.o","profile":"authored_cpp","sha256":"10d0812d2dc581d8444aad1f4b9eda47aa63a9eb230517307c74e16f3fb10606","source":"sources/w9-rtti-vtables-entities-c-trigger-door-secret/trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"msg_trigger__9DoorSwingi"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"trigger.o","profile":"authored_cpp","sha256":"53314848ffcfb71b75d8f79ce082463ba75aa4fcaae72dcd1bdaa0a38fb67114","source":"sources/w9-rtti-vtables-entities-c-trigger-door-swing/trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":24,"name":"msg_trigger__11MissileTrapi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"trigger.o","profile":"authored_cpp","sha256":"5260c2a4600bee0b9e4b15230d3ae4c9708da5da3adf9bd549549efeaf19bb6e","source":"sources/w9-rtti-vtables-entities-c-trigger-missile-trap/trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":184,"name":"norrath_wood_elf_soldier_init_type"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"init_type.o","profile":"authored_cpp","sha256":"020b5f7e7ab375bf26e68eff3fbba1a73d6918d21f59059129d72adeeb174fb8","source":"sources/w9-rtti-vtables-entities-c-wood-elf-init-type/init_type.cpp"}
{"emitted_functions":[{"compiled_body_bytes":4276,"name":"norrath_wood_elf_soldier_set_state"}],"header_closure":["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"e355de427403f9ea6ea196326853c64d0c408bb665e1c4a9eb63c5ef8d92e669","source":"sources/w9-rtti-vtables-entities-c-wood-elf-set-state/set_state.cpp"}
```

## Authored but never executed

All of it on the EE or in the game. Everything was executed only inside the private interpreter with every callee stubbed (including virtual slots, `strcpy` and `strcat`), against single-object scenarios, plus host checks. Specifically:

- `norrath_ti_6PhProp` has only a host check (size, zero fill, alignment). `norrath_rtti_ptr` ran against the image with the table address bound to 0x4538f8.
- Skeleton `setState` state 8, bit 0x200 half, and MaleDarkElf `setState` state 8, bit 0x200 half: the 64-bit word at `object+160` has bit 0x200 cleared a few instructions earlier in each function, so these halves cannot run unless something else sets the bit in between. They were authored from the instructions and are covered by neither the paired runs nor the host checks.
- Soul `setState` state 23 with a minion present (attachment-matrix chain): the early filter returns for that combination, so the paired run reached it only through a hook that makes the minion word nonzero during the shadow-position call. That is an environment change, not a game path.
- Floating-point conversions use the interpreter's behaviour on both sides; scenario values were chosen so truncation and rounding agree where they could differ on the EE. The two float-to-unsigned branches for values at or above 2^31 in `norrath_construct_AutoObject` were exercised with such values.
- Four-byte and halfword stores are done with byte helpers, so generated code differs from the image's instruction mix; no hazard, alignment or physical-access claim is made.

## Unknown

The names and numbers of states, kinds and types; the meaning of most flag bits (for example bit 0x200, 0x100000, 0x4002 and 0x2 in the object flag words); the layout of the per-class header tables behind `object+36` and of `skeletonResMap`; what `skills[12..14]`, `skills[25]` and `skills[26]` are and what virtual slot 96 of the objects they point to computes; the weapon record format shared by the four initType bodies; whether `setStateProcessing__8CreaturePi` may legally return a nonzero value that is not a plain boolean; the return values of most called routines (declared `void` or `int` according to whether the caller reads V0); and whether any other package expects the original names (`initType__5Mummyb` and the like) to stay resolvable.
