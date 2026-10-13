# w10-actor-e: spell, skill and actor virtuals (Cthulu to FireBeetle)

Package `w10-actor-e`, branch `agents/w10-actor-e`. Private reference: the original executable
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`).
Addresses are that file's symbol values. No game bytes or disassembly listings are committed.
Everything below was compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here has
run on a PS2 or in a game; "paired" means the authored object was run next to the original
function in a private R5900 interpreter (kept outside git).

## How each function was checked

- Paired run (private): compile the unit, link it with every undefined symbol bound to the
  original's address (the vtable view names and `norrath_block_free` are bound to the original's
  table and `blockFree__FPv`), then run the original and the authored function from identical memory
  snapshots with identical stub callees. Compared: the ordered callee log (argument registers,
  float arguments, pointed-to vectors, stack arguments), the return value where the original
  defines one, and every changed byte outside the stack. Callee results are scripted per
  scenario and replayed identically for both runs. Scenarios use fixed random seeds plus
  targeted boundary cases. Instruction coverage of the original was measured for the large
  functions; the words never reached are alignment padding, annulled delay slots and the
  branches listed under "Unknown". Single-constant mutations were confirmed to fail for the
  units marked "mutation caught".
- Host contract (`host_check.cpp` in each directory, not a build input): restates the expected
  behaviour as stub-callee sequences and field values. Units that read 32-bit pointer words use
  the existing `NORRATH_*_HOST` conventions of the shared byte helpers.
- Interpreter note: a first host build of the three clone contracts failed to link and an
  earlier commit message wrongly said it passed; the follow-up commits fix the stubs. All 32
  host contracts and all 32 compilations were rerun from scratch at the end and pass.

## Functions

| directory | symbol | original extent | compiled bytes | check |
|---|---|---|---|---|
| `w10-actor-e-clone-10firebeetle` | `clone__10FireBeetle` | `0x1199f0`, 1472 bytes | 816 | 10 random sources, object snapshot at every constructor call; host contract |
| `w10-actor-e-clone-13femaledarkelf` | `clone__13FemaleDarkElf` | `0x1193c8`, 1572 bytes | 816 | 10 random sources, object snapshot at every constructor call; host contract |
| `w10-actor-e-clone-7cyclops` | `clone__7Cyclops` | `0x118e78`, 1356 bytes | 776 | 10 random sources, object snapshot at every constructor call; host contract |
| `w10-actor-e-cthulu-set-state` | `setState__6Cthului` | `0x281b20`, 1260 bytes | 2056 | 610 paired scenarios, 308 of 315 words; host contract |
| `w10-actor-e-cyclops-get-miniboss-name` | `getMinibossName__7Cyclops` | `0x203c98`, 60 bytes | 68 | 7 paired byte values with sprintf hooked; host contract |
| `w10-actor-e-cyclops-init-miniboss` | `initMiniboss__7Cyclops` | `0x203c60`, 56 bytes | 104 | 12 paired roll cases; host contract |
| `w10-actor-e-cyclops-msg-draw` | `msg_draw__7Cyclops` | `0x203bc8`, 68 bytes | 68 | 1 paired scenario; host contract |
| `w10-actor-e-cyclops-msg-load` | `msg_load__7CyclopsPc` | `0x203c10`, 80 bytes | 72 | 7 paired cases; host contract |
| `w10-actor-e-cyclops-msg-run` | `msg_run__7Cyclops` | `0x202a08`, 2520 bytes | 4104 | 1155 paired scenarios, 623 of 630 words; mutations caught; host contract |
| `w10-actor-e-cyclops-set-state` | `setState__7Cyclopsi` | `0x2033e0`, 1052 bytes | 1680 | 347 paired scenarios, 257 of 263 original words reached (rest padding); mutation caught; host contract |
| `w10-actor-e-demon-msg-draw` | `msg_draw__5Demon` | `0x1eb348`, 108 bytes | 152 | 6 paired cases, every register argument; host contract |
| `w10-actor-e-demon-msg-load` | `msg_load__5DemonPc` | `0x1eb400`, 88 bytes | 96 | 5 paired cases; host contract |
| `w10-actor-e-demon-msg-run` | `msg_run__5Demon` | `0x1ea028`, 3592 bytes | 5496 | 1719 paired scenarios, 877 of 898 words; mutations caught; host contract |
| `w10-actor-e-demon-msg-save` | `msg_save__5DemonPc` | `0x1eb3b8`, 72 bytes | 84 | 5 paired cases; host contract |
| `w10-actor-e-demon-set-state` | `setState__5Demoni` | `0x1eae30`, 1064 bytes | 1896 | 379 paired scenarios, 261 of 266 words; host contract |
| `w10-actor-e-dtor-10firebeetle` | `_$_10FireBeetle` | `0x30ed60`, 100 bytes | 112 | 24 paired scenarios; host contract |
| `w10-actor-e-dtor-13femaledarkelf` | `_$_13FemaleDarkElf` | `0x316818`, 100 bytes | 112 | 24 paired scenarios; host contract |
| `w10-actor-e-dtor-5demon` | `_$_5Demon` | `0x314018`, 100 bytes | 112 | 24 paired scenarios; host contract |
| `w10-actor-e-dtor-7cyclops` | `_$_7Cyclops` | `0x30eca8`, 100 bytes | 112 | 24 paired scenarios; host contract |
| `w10-actor-e-fdelf-msg-alert` | `msg_alert__13FemaleDarkElfifG6Point3P10GameObject` | `0x2895c8`, 180 bytes | 380 | 216 paired scenarios; a boundary mutation was caught; host contract |
| `w10-actor-e-fdelf-msg-draw` | `msg_draw__13FemaleDarkElf` | `0x285708`, 296 bytes | 440 | 36 paired cases; host contract |
| `w10-actor-e-fdelf-msg-load` | `msg_load__13FemaleDarkElfPc` | `0x289548`, 128 bytes | 188 | 5 paired cases; host contract |
| `w10-actor-e-fdelf-msg-run` | `msg_run__13FemaleDarkElf` | `0x285830`, 2504 bytes | 3840 | 911 paired scenarios, 612 of 626 words; mutations caught; host contract |
| `w10-actor-e-fdelf-msg-save` | `msg_save__13FemaleDarkElfPc` | `0x2894d8`, 108 bytes | 224 | 24 paired cases; host contract |
| `w10-actor-e-fdelf-set-state` | `setState__13FemaleDarkElfi` | `0x2861f8`, 1832 bytes | 3008 | 1086 paired scenarios in both blockAlloc outcomes, 445 of 458 words; mutations caught; host contract |
| `w10-actor-e-static-init-disease-corpse` | `_GLOBAL_$I$msg_run__13DiseaseCorpse` | `0x297638`, 32 bytes | 280 | paired: 3 constructor calls, every argument and record identity; host contract |
| `w10-actor-e-static-init-disease-trail` | `_GLOBAL_$I$msg_run__12DiseaseTrail` | `0x2955a0`, 32 bytes | 292 | paired: 3 constructor calls; host contract |
| `w10-actor-e-static-init-explode-arrow-projectile` | `_GLOBAL_$I$msg_run__22ExplodeArrowProjectile` | `0x2a2b18`, 32 bytes | 116 | paired: 1 constructor call; host contract; a one-off mutation was caught |
| `w10-actor-e-tf-10firebeetle` | `__tf10FireBeetle` | `0x30edc8`, 80 bytes | 92 | 5 paired gate values; host contract |
| `w10-actor-e-tf-13femaledarkelf` | `__tf13FemaleDarkElf` | `0x316880`, 80 bytes | 92 | 5 paired gate values; host contract |
| `w10-actor-e-tf-5demon` | `__tf5Demon` | `0x314080`, 80 bytes | 92 | 5 paired gate values; host contract |
| `w10-actor-e-tf-7cyclops` | `__tf7Cyclops` | `0x30ed10`, 80 bytes | 92 | 5 paired gate values; host contract |

## Observed

- Static initialisers: each `_GLOBAL_$I$msg_run__*` wrapper calls a unit-local initialiser with
  (1, 0xffff); the particle-definition constructor is called with the tuples in the sources, in
  that order, with a null callback. The records are unit-local zero-initialised storage in the
  image (DiseaseCorpse 3 records from 0x008d6a50, DiseaseTrail 3 from 0x008d68c0,
  ExplodeArrowProjectile 1 at 0x008d7090). The ExplodeArrow floats come from two small-data
  constants.
- `__tf` and destructors follow the sibling pattern: type-info registration against the Creature
  type-info object; destructors store the class vtable at +72, call the Creature destructor with
  flag 0, then free when bit 0 of the incoming flag is set.
- The `setState` routines are jump-table switches; Cyclops (17 states), Cthulu (17), FemaleDarkElf
  (16) call `setStateProcessing__8CreaturePi` first, Demon (18) does not and instead clears bit 9 of
  the 64-bit value at +160 on every call. Each state's animation header offset, rate and flags are
  in the source comments.
- The `msg_run` routines share a prologue (damage handler, `everyFrame`, activation check or, in
  Demon, a zero test of +236, `creatureAdvanceAnimation`, `slideBack`, the virtual slot at byte 168)
  and then switch on the signed halfword at +572.
- Demon's type tests are `typeid(*E) == typeid(Soul)` through the function at vtable byte 4.
- The clone copy ranges were measured by running the original on pattern-filled sources.

## Inferred

- The names used for the three vtable pointers (`norrath_ctors_ant_lantern_view_44bf90`, `44dd68`,
  `450530`, `44bea0`) are the existing view objects of the constructor packages.
- State names, flag meanings and field meanings are not claimed anywhere. Comments call fields by
  offset only.

## Unknown

- What the state numbers, the flag word returned by `creatureAdvanceAnimation`, the words at +208,
  +236, +256, +744, +804, +808, +960, +964, +972 and +1104, and the particle tuples mean.
- FemaleDarkElf `msg_run` state 13 takes three rolls and scales them into a local that is never
  read afterwards; the rolls are made (they consume the random sequence) but the values are not
  reproduced. Likewise the dead stores of a lift vector in the FemaleDarkElf DiseaseBolt spawn and the Demon Sparks
  spawn are not reproduced (the vector is built but never passed).
- Cyclops `msg_run` state 15's loop runs exactly once in the original (its counter starts at 0 and
  the test is "not positive"); the source has no loop.
- Demon `msg_run` with a null E in states 3 and 13 calls `__throw_bad_typeid`; the original never
  returns from it, so that path is not compared. The FemaleDarkElf `setState` state 8 has a bit 9
  test that cannot be true after the clear above it; it is kept in the source, not reachable in a
  paired run.
- The `DiseaseBolt` constructor's first parameter is a short in its mangled name; the original
  passes the full result register unmodified, so it is declared `int` in
  `w10-actor-e-fdelf-msg-run`.

## Imports closed (names)

`setState__6Cthului`, `_$_7Cyclops`, `__tf7Cyclops`, `clone__7Cyclops`, `getMinibossName__7Cyclops`,
`initMiniboss__7Cyclops`, `msg_draw__7Cyclops`, `msg_load__7CyclopsPc`, `msg_run__7Cyclops`,
`setState__7Cyclopsi`, `_$_5Demon`, `__tf5Demon`, `msg_draw__5Demon`, `msg_load__5DemonPc`,
`msg_run__5Demon`, `msg_save__5DemonPc`, `setState__5Demoni`,
`_GLOBAL_$I$msg_run__13DiseaseCorpse`, `_GLOBAL_$I$msg_run__12DiseaseTrail`,
`_GLOBAL_$I$msg_run__22ExplodeArrowProjectile`, `_$_13FemaleDarkElf`, `__tf13FemaleDarkElf`,
`clone__13FemaleDarkElf`, `msg_alert__13FemaleDarkElfifG6Point3P10GameObject`,
`msg_draw__13FemaleDarkElf`, `msg_load__13FemaleDarkElfPc`, `msg_run__13FemaleDarkElf`,
`msg_save__13FemaleDarkElfPc`, `setState__13FemaleDarkElfi`, `_$_10FireBeetle`,
`__tf10FireBeetle`, `clone__10FireBeetle` (all 31 names of the task list).

The units also define the 12-byte type-info objects `__ti7Cyclops`, `__ti5Demon`,
`__ti13FemaleDarkElf` and `__ti10FireBeetle` (zero-initialised storage, as the sibling `__tf`
units do for their classes) and four file-local particle record groups.

## Imports opened

Names these units call or read that no authored unit in the current plan defines. Other w10
packages may close some of them; none was supplied here.
`ISeeDeadPeople__8Creaturef`, `_$_8Creature`, `__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`, `__11DiseaseBoltsiG6Point3P10GameObjectP5Skill`, `__14SpellHarmTouchiP8CreatureP5Skill`, `__14poisonGasSpellG6Point3iiiP10GameObjecti`, `__16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature`, `__8ListHeadRC8ListHead`, `__9FireStormiP10GameObjectG6Point3P5Skill`, `__9dustCloudG6Point3iiii`, `__tf14SpellHarmTouch`, `__tf8Creature`, `__ti8Creature`, `alternateEnemyInZone__8Creatureffss`, `attack__14SpellHarmTouchP10GameObjectPiT2`, `checkIfSpellIsRunning__8CreatureRC9type_info`, `creatureFindLeadYaw__8CreatureP10GameObjectf`, `creatureSlamEffect__8CreatureP8Creaturei`, `dropWeapon__5Demon`, `enemyInZone2__8Creatureffssfi`, `enemyInZone__8Creaturefs`, `findEnemyLeadPosition__8CreaturefP6Point3`, `flee2__8CreatureP10GameObject`, `frameNm`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `generateDust__7CyclopsG6Point3`, `levelName`, `lumpFindResource__FPcT0`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `norrath_actor_view_4488f0`, `norrath_block_free`, `norrath_ctors_ant_lantern_view_44bea0`, `norrath_ctors_ant_lantern_view_44bf90`, `norrath_ctors_ant_lantern_view_44dd68`, `norrath_ctors_ant_lantern_view_450530`, `norrath_game_object_vtable_view`, `objectIsCrowded__8CreatureP10GameObjectffi`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `sprintf`, `steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature`.

Of the other names the units reference, 6 were already in the unresolved list
(`__15AnimationState8RC15AnimationState8`, `__tf4Soul`, `dropTreasure__8CreatureG6Point3iffffffPc`, `floorAdd__FG6Point3sfii`, `fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff`, `skills`) and 39 are defined by authored units already in the
plan (`__11ParticleDefiiiiiiiifffiiiPFPv_v`, `__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `__6SparksG6Point3T1P8CreatureP10_vagHeaderT4iiii`, `__eq__C9type_infoRC9type_info`, `__rtti_si`, `__throw_bad_typeid`, `addSpell__8CreatureP5Spell`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddToEnd__FP14AnimationStateP15AnimationHeaderi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animGlobalSync__FP14AnimationState`, `bExitStatePersueIdle__8Creature`, `blockAlloc__Fi`, `creatureAdvanceAnimation__8Creaturef`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `eRand__10GameObject`, `everyFrame__8CreatureP12_modelHeader`, `getSmartRandomDestination__8Creaturei`, `iatan2__Fff`, `icos__Fi`, `initAttackVariant__5Demon`, `initType__13FemaleDarkElfb`, `isin__Fi`, `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`, `modelSetShadowScale__Ff`, `move2__8CreatureG6Point3`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `newDamageHandler__8CreatureP12_modelHeader`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`, `objectAddToRunList__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `resetRouting__8Creature`, `setStateProcessing__8CreaturePi`, `slideBack__8Creaturef`, `smartWander__8Creature`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `strticmp__FPCcT0`).

## Build-plan entries needed

All 32 units use profile `authored_cpp`. The header closures name existing shared headers
(`sources/vslots-b-bytes/bytes.hpp`, `sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp`,
`sources/w8-spell-static-init-a-common/particle_def.hpp`); `host_check.cpp` files and
`host_record.hpp` are host-only and are not plan inputs. `input_order` is the integrator's. Each
line is one entry; `emitted_functions` and `compiled_body_bytes` come from the object the reviewed
compiler produced for the unit.

```json
{"emitted_functions": [{"compiled_body_bytes": 816, "name": "clone__10FireBeetle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "clone.o", "profile": "authored_cpp", "sha256": "29037247a18e65600e2c73f2acb2b6f36620c8c23ff6be306638f765c2884c11", "source": "sources/w10-actor-e-clone-10firebeetle/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 816, "name": "clone__13FemaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "clone.o", "profile": "authored_cpp", "sha256": "7e22695c98d11023bf84ad27153b7a5b23d625de118cd14bc348e8778a4e10d1", "source": "sources/w10-actor-e-clone-13femaledarkelf/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 776, "name": "clone__7Cyclops"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "clone.o", "profile": "authored_cpp", "sha256": "ae75d4562af91963833c54a3a80e2cebf65ff9429b6e7fd7c0f7aca45244c014", "source": "sources/w10-actor-e-clone-7cyclops/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2056, "name": "setState__6Cthului"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "8242658d21df6a5c696b1fd91a408b1852f9677a12a1f45a1bd5413a27fd9acf", "source": "sources/w10-actor-e-cthulu-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "getMinibossName__7Cyclops"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "get_miniboss_name.o", "profile": "authored_cpp", "sha256": "f0028f1faeec1a10686a2abc43fd0a13bdec726435be0e462d9e86cad86872f4", "source": "sources/w10-actor-e-cyclops-get-miniboss-name/get_miniboss_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "initMiniboss__7Cyclops"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "init_miniboss.o", "profile": "authored_cpp", "sha256": "ed431385786fd8edcf64878d5b0d86ad5989ad45097ad8090a131489eacd4081", "source": "sources/w10-actor-e-cyclops-init-miniboss/init_miniboss.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "msg_draw__7Cyclops"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_draw.o", "profile": "authored_cpp", "sha256": "cce025250a6d296b342ba1f343b2328e38caa6cbc42b34f43a32aa6704686b65", "source": "sources/w10-actor-e-cyclops-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "msg_load__7CyclopsPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_load.o", "profile": "authored_cpp", "sha256": "ec1aaa54d432a6b79b2e08e84ab027a7a930a82849444f7008f462695210a6fa", "source": "sources/w10-actor-e-cyclops-msg-load/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 4104, "name": "msg_run__7Cyclops"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_run.o", "profile": "authored_cpp", "sha256": "9bcbac8cc6772de3b3efd4a6ea4f3854016726f7222c3b2a8439ec9f860ca6f5", "source": "sources/w10-actor-e-cyclops-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1680, "name": "setState__7Cyclopsi"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "ce8f82485b3a95e1e1c1667da52f6d964bdc4f37aba6013046fde6987deba3b1", "source": "sources/w10-actor-e-cyclops-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "msg_draw__5Demon"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_draw.o", "profile": "authored_cpp", "sha256": "ce9be11cfe7755da7019b3c9ce151c62268cf102fee064df4d546becc727630e", "source": "sources/w10-actor-e-demon-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "msg_load__5DemonPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_load.o", "profile": "authored_cpp", "sha256": "02c30a09f6f6f6fc7ece960f93b04431f3fabef1b697e177349c68d90df15ceb", "source": "sources/w10-actor-e-demon-msg-load/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 5496, "name": "msg_run__5Demon"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_run.o", "profile": "authored_cpp", "sha256": "750efa1b48a37bf2e80a140c5fa766fc3ea048c3d4c5dcb4b5709e2770baad1f", "source": "sources/w10-actor-e-demon-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "msg_save__5DemonPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_save.o", "profile": "authored_cpp", "sha256": "a58b57c8c91d78c65c4a2634fcff4719cccf5dbe3dc62a4e8a0504fa7d3c9b6a", "source": "sources/w10-actor-e-demon-msg-save/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1896, "name": "setState__5Demoni"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "2cfd2b9884ca6d82c45cf5555540d6b5d542ca62a6ebf83a18638cd0b58a318b", "source": "sources/w10-actor-e-demon-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_10FireBeetle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "7e99d77496287563333e245bba3552d9e4423b4856f7c7037490514363da3c5d", "source": "sources/w10-actor-e-dtor-10firebeetle/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_13FemaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "c97b2a3ed80ec2d80fd09ba4ce5cef44fbf408cbf989b9ed27672f62703b141a", "source": "sources/w10-actor-e-dtor-13femaledarkelf/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_5Demon"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "e58241b8916ef57f2771f382fea77b4f7e39a85b90fe60ea0f2069e50d326a12", "source": "sources/w10-actor-e-dtor-5demon/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_7Cyclops"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "f1eafad4e8bbc918226b6f9273fb5a11804cfc0db11ccfbc832135fad9a83008", "source": "sources/w10-actor-e-dtor-7cyclops/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 380, "name": "msg_alert__13FemaleDarkElfifG6Point3P10GameObject"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_alert.o", "profile": "authored_cpp", "sha256": "5a47090986848624fabac6f138e4424de7b9bcd30e0b1199642d76ba9d349645", "source": "sources/w10-actor-e-fdelf-msg-alert/msg_alert.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 440, "name": "msg_draw__13FemaleDarkElf"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_draw.o", "profile": "authored_cpp", "sha256": "5700974559546b6d3c771ace4b2f9f0255114623b7ab3c6571b267111287b91f", "source": "sources/w10-actor-e-fdelf-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 188, "name": "msg_load__13FemaleDarkElfPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_load.o", "profile": "authored_cpp", "sha256": "99272aca840b276f2bb750e83c2a667045cdc91a6050143f5a8cda69fb98bd94", "source": "sources/w10-actor-e-fdelf-msg-load/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3840, "name": "msg_run__13FemaleDarkElf"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_run.o", "profile": "authored_cpp", "sha256": "19dcf2ceafe69832236c95f94589b4930b48151b47f7a565ec98181b3efa7448", "source": "sources/w10-actor-e-fdelf-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 224, "name": "msg_save__13FemaleDarkElfPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "msg_save.o", "profile": "authored_cpp", "sha256": "98c03ebd730b675380e8b707c5db1e359b02417222182d655b9b1462c959a081", "source": "sources/w10-actor-e-fdelf-msg-save/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3008, "name": "setState__13FemaleDarkElfi"}], "header_closure": ["sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "e8ec026bb56d0cca61302a2213fb742d4698b388cd39ec2a13b4a30f1b14725b", "source": "sources/w10-actor-e-fdelf-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 280, "name": "_GLOBAL_$I$msg_run__13DiseaseCorpse"}], "header_closure": ["sources/w8-spell-static-init-a-common/particle_def.hpp"], "kind": "independently_authored_source", "object_name": "disease_corpse_static_init.o", "profile": "authored_cpp", "sha256": "5b5db230dfd2c0cd24374fd6a27ea4f1b0bcfd2c0ab1308b089dadd33a65f310", "source": "sources/w10-actor-e-static-init-disease-corpse/disease_corpse_static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 292, "name": "_GLOBAL_$I$msg_run__12DiseaseTrail"}], "header_closure": ["sources/w8-spell-static-init-a-common/particle_def.hpp"], "kind": "independently_authored_source", "object_name": "disease_trail_static_init.o", "profile": "authored_cpp", "sha256": "d95f83dc1754a91110e1e6de9f5ee3677fcd090ad8e874574ec3c358dc21bddb", "source": "sources/w10-actor-e-static-init-disease-trail/disease_trail_static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "_GLOBAL_$I$msg_run__22ExplodeArrowProjectile"}], "header_closure": ["sources/w8-spell-static-init-a-common/particle_def.hpp"], "kind": "independently_authored_source", "object_name": "explode_arrow_projectile_static_init.o", "profile": "authored_cpp", "sha256": "03d0174258bd838fb9d2439bdd896f2844e43eecf0dea3b374705f817c9dd061", "source": "sources/w10-actor-e-static-init-explode-arrow-projectile/explode_arrow_projectile_static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf10FireBeetle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "typeinfo.o", "profile": "authored_cpp", "sha256": "94cec23a396fc737ccf72e06ab2330b40d3a93bada84531111e46fd98a8d61fb", "source": "sources/w10-actor-e-tf-10firebeetle/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf13FemaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "typeinfo.o", "profile": "authored_cpp", "sha256": "29cabe46043068d23e83c33e1f03074ab13ea94787efb6b6f54d73a6b63da593", "source": "sources/w10-actor-e-tf-13femaledarkelf/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf5Demon"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "typeinfo.o", "profile": "authored_cpp", "sha256": "6258852c1093210214a5f8b5fb3453813c488230fb218a498cd1b181e9197ea5", "source": "sources/w10-actor-e-tf-5demon/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf7Cyclops"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "typeinfo.o", "profile": "authored_cpp", "sha256": "c490bdc98ca734d609e54cd253312859f578ac6ea09a3e3a109dae6a3c9ee3f2", "source": "sources/w10-actor-e-tf-7cyclops/typeinfo.cpp"}
```

## Authored but never executed

All 32 units. They were compiled, run on the host against recording stubs, and compared with the
original in a private interpreter, but none ran on an EE or in the game. Specifically unexercised:
the `blockAlloc` and constructor side effects of every spawned object (stubs only), the virtual
slots reached through vtables (fake tables only), the `skills` table contents (written by the
paired scenarios), and every path that depends on live game data.

## Blockers and things for the owner

- Several callees are imports that no package of this wave appears to cover yet (for example
  `enemyInZone__8Creaturefs`, `objectIsCrowded__8CreatureP10GameObjectffi`,
  `findEnemyLeadPosition__8CreaturefP6Point3`, `ISeeDeadPeople__8Creaturef`, the `SpellHarmTouch`,
  `Sparks`, `FireStorm`, `DiseaseBolt`, `PoisonRain` and `PoisonProjectile` constructors,
  `generateDust__7CyclopsG6Point3`, `dropWeapon__5Demon`). They are listed above.
- `norrath_block_free` (dtors) and the Creature vtable view `norrath_actor_view_4488f0`
  (clones) are used as the sibling units use them.
