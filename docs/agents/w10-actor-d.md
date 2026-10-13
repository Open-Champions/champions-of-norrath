# w10-actor-d: spell, skill and actor virtuals (Chest to Cthulu)

Branch `agents/w10-actor-d`. Addresses and sizes are from the pinned private `SLUS_205.65` `.text`. Each function is its own commit under `experimental/sources/w10-actor-d-*`, has a host check beside it (`host_check.cpp`, not a plan input) and was compared against the image in a private R5900 interpreter harness kept outside git: the image's instructions and the freshly linked authored object run from identical memory snapshots with identical stubbed callees, and the stub call logs (arguments, stack arguments, an object checksum at each call where the order of stores matters) and every changed byte outside the stack must be equal. Nothing here has run on the EE or as gameplay. Compiled sizes come from the reviewed EE g++ `authored_cpp` objects; no byte-equality claim is made.

## Evidence

| Symbol | Address | Image bytes | Compiled bytes | Checks (paired = original vs authored runs) | Unit |
|---|---|---|---|---|---|
| `_$_10CloudGiant` | 0x00313f60 | 100 | 88 | host + 24 | `w10-actor-d-dtor-cloudgiant` |
| `_$_6Cthulu` | 0x003165f0 | 100 | 88 | host + 24 | `w10-actor-d-dtor-cthulu` |
| `_$_7Critter` | 0x0030ebe8 | 100 | 88 | host + 24 | `w10-actor-d-dtor-critter` |
| `__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc` | 0x00108e58 | 852 | 944 | host + 300 | `w10-actor-d-creature-ctor` |
| `__tf10CloudGiant` | 0x00313fc8 | 80 | 92 | host + 9 paired | `w10-actor-d-tf-cloudgiant` |
| `__tf6Cthulu` | 0x00316658 | 80 | 92 | host + 9 paired | `w10-actor-d-tf-cthulu` |
| `__tf7Critter` | 0x0030ec50 | 80 | 92 | host + 9 paired | `w10-actor-d-tf-critter` |
| `clone__7Critter` | 0x00118830 | 1604 | 780 | host + 10 | `w10-actor-d-clone-7critter` |
| `dropTreasure__8CreatureG6Point3iffffffPc` | 0x0010c338 | 3864 | 1060 | host + 240 | `w10-actor-d-creature-drop-treasure` |
| `getCurrentRoutePoint__8Creature` | 0x00113200 | 112 | 116 | host + 60 | `w10-actor-d-get-current-route-point` |
| `getModel__7Critter` | 0x0030eca0 | 8 | 8 | host + 12 | `w10-actor-d-critter-get-model` |
| `getNextRoutePoint__8CreatureP6Point3G6Point3ffi` | 0x00113270 | 216 | 188 | host + 120 | `w10-actor-d-get-next-route-point` |
| `msg_collision__7CritterP10GameObject` | 0x001f1910 | 172 | 184 | host + 100 | `w10-actor-d-critter-msg-collision` |
| `msg_draw__10CloudGiant` | 0x001e9990 | 112 | 128 | host + 40 | `w10-actor-d-cloudgiant-msg-draw` |
| `msg_draw__6Cthulu` | 0x00282010 | 40 | 20 | host + 12 | `w10-actor-d-cthulu-msg-draw` |
| `msg_draw__7Critter` | 0x001f1810 | 228 | 260 | host + 120 | `w10-actor-d-critter-msg-draw` |
| `msg_hurt__10CloudGiantfP10GameObjecti` | 0x001e9b60 | 60 | 64 | host + 40 | `w10-actor-d-cloudgiant-msg-hurt` |
| `msg_hurt__7CritterfP10GameObjecti` | 0x001f1908 | 8 | 8 | host + 12 | `w10-actor-d-critter-msg-hurt` |
| `msg_load__10CloudGiantPc` | 0x001e9a48 | 88 | 76 | host + 30 | `w10-actor-d-cloudgiant-msg-load` |
| `msg_load__5ChestPc` | 0x0021dab0 | 220 | 220 | host + 80 | `w10-actor-d-chest-msg-load` |
| `msg_load__7CritterPc` | 0x001f18f8 | 8 | 8 | host + 12 | `w10-actor-d-critter-msg-load` |
| `msg_moveCollision__6CthuluPP10GameObject` | 0x00282038 | 236 | 280 | host + 80 | `w10-actor-d-cthulu-msg-move-collision` |
| `msg_poke__5Chesti` | 0x00218eb8 | 1916 | 1696 | host + 160 | `w10-actor-d-chest-msg-poke` |
| `msg_run__10CloudGiant` | 0x001e70b0 | 7304 | 6620 | host + 460 | `w10-actor-d-cloudgiant-msg-run` |
| `msg_run__5Chest` | 0x002189e8 | 1228 | 1272 | host + 120 | `w10-actor-d-chest-msg-run` |
| `msg_run__6Cthulu` | 0x00280da8 | 3448 | 2732 | host + 580 | `w10-actor-d-cthulu-msg-run` |
| `msg_run__7Critter` | 0x001f0648 | 2300 | 2264 | host + 420 | `w10-actor-d-critter-msg-run` |
| `msg_save__10CloudGiantPc` | 0x001e9a00 | 72 | 56 | host + 30 | `w10-actor-d-cloudgiant-msg-save` |
| `msg_save__5ChestPc` | 0x0021da68 | 72 | 48 | host + 20 | `w10-actor-d-chest-msg-save` |
| `msg_save__7CritterPc` | 0x001f1900 | 8 | 8 | host + 12 | `w10-actor-d-critter-msg-save` |
| `msg_trigger__10CloudGianti` | 0x001e9aa0 | 192 | 160 | host + 40 | `w10-actor-d-cloudgiant-msg-trigger` |
| `setState__10CloudGianti` | 0x001e8d38 | 1448 | 1608 | host + 260 | `w10-actor-d-cloudgiant-set-state` |

Per-function observed, inferred and unknown statements are in the header comment of each source file. Short form of what the comparisons established:

- Where a mutant of the source (one constant, comparison or offset changed) was tried, the paired run flagged it. Mutants that were not flagged are equivalent ones and are listed in the commit message: the unreachable `== 23` test in the Cthulu melee slot search (all four slots start at -1) and the clamp edge in the heading clamp (the clamp value equals the unclamped one at the edge).
- Float singles that are not exact binary fractions are written as hexadecimal float literals where a decimal would be truncated to a different single by the reviewed compiler (it truncates decimal literals instead of rounding to nearest). The decimals 0.1, 0.2, 0.3, 0.6, 2.4, 0.01, 0.0025, 0.95 are safe because their truncated value is the image's value.
- `creatureAdvanceAnimation__8Creaturef`'s integer result is the flag word S that Cthulu and CloudGiant `msg_run` test (`bit 0`, `bit 1`, `bit 2`); the image moves it into a saved register in the delay slot of the next call, so `slideBack__8Creaturef`'s result is not used. The type-info slot of the virtual table is read at table byte 4 and called without arguments (the typeid idiom: slot, then `__tf<Class>`, then `__eq__C9type_infoRC9type_info`).
- `dropTreasure__8Creature` special-cases eleven boss types; SeaMonster only when bit 0 of the 64-bit word at +160 is set and CloudGiant only when the word at +976 is nonzero, otherwise the chain continues to the default drop.
- `__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc` is the Creature constructor; it reads these tag names from the placed object's list: `initScript`, `wait`, `hide`, `ShowName`, `Dead`, `HomeRange`, `Scale`, `MiniBoss`, `light1`, `light2`, `ambient`.

## Imports closed

All 32 names of the package scope:

`msg_load__5ChestPc`, `msg_poke__5Chesti`, `msg_run__5Chest`, `msg_save__5ChestPc`, `_$_10CloudGiant`, `__tf10CloudGiant`, `msg_draw__10CloudGiant`, `msg_hurt__10CloudGiantfP10GameObjecti`, `msg_load__10CloudGiantPc`, `msg_run__10CloudGiant`, `msg_save__10CloudGiantPc`, `msg_trigger__10CloudGianti`, `setState__10CloudGianti`, `__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc`, `dropTreasure__8CreatureG6Point3iffffffPc`, `getCurrentRoutePoint__8Creature`, `getNextRoutePoint__8CreatureP6Point3G6Point3ffi`, `_$_7Critter`, `__tf7Critter`, `clone__7Critter`, `getModel__7Critter`, `msg_collision__7CritterP10GameObject`, `msg_draw__7Critter`, `msg_hurt__7CritterfP10GameObjecti`, `msg_load__7CritterPc`, `msg_run__7Critter`, `msg_save__7CritterPc`, `_$_6Cthulu`, `__tf6Cthulu`, `msg_draw__6Cthulu`, `msg_moveCollision__6CthuluPP10GameObject`, `msg_run__6Cthulu`.

Three type-info data nodes, `__ti7Critter`, `__ti10CloudGiant` and `__ti6Cthulu`, are also defined, by the `__tf` units.

## Imports opened

Referenced by these sources and not defined by any source in the base object `game-components-partial.o` of the integration build I measured against (so each is a new unresolved name unless another wave-10 package defines it; some, such as `enemyInZone__8Creaturefs`, `pointInZone__8CreatureG6Point3ffss`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature`, `flee2__8CreatureP10GameObject`, `findEnemyLeadPosition__8CreaturefP6Point3` and `fxRunWeapon__...`, are plausible members of sibling actor packages):

- `P_MoveLightning__FiG6Point3T1ii`
- `P_StartLightning__FG6Point3T0iiffUi`
- `__11BoltGoRoundG6Point3siP15AnimationHeader`
- `__16PoisonProjectilePQ210GameObject11resourceMapiiiiG6Point3T6ifP8Creature`
- `__8ListHeadRC8ListHead`
- `_vt$10CloudGiant`
- `_vt$10GameObject`
- `_vt$6Cthulu`
- `_vt$7Critter`
- `_vt$8Creature`
- `chestResourceList`
- `dropTreasure__4itemG6Point3iffffffPci`
- `enemyInZone2__8Creatureffssfi`
- `enemyInZone__8Creaturefs`
- `findEnemyLeadPosition__8CreaturefP6Point3`
- `flee2__8CreatureP10GameObject`
- `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`
- `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`
- `modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf`
- `modelSetFade__Fc`
- `norrath_w10_actor_d_view_7d6c40`
- `norrath_w10_actor_d_view_7d7720`
- `norrath_w10_actor_d_view_7d7770`
- `plotRoute__8CreatureG6Point3ffi`
- `pointInZone__8CreatureG6Point3ffss`
- `produceMeleeDamage__8CreaturefiiiiPQ24item11description`
- `setResource__10GameObjectPQ210GameObject11resourceMap`
- `steerAndRouteToEnemy__8Creature`
- `steerToPosition__8CreatureG6Point3`
- `turnToFaceEnemy__8Creature`
- `worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii`

Notes on these:

- `norrath_w10_actor_d_view_7d7720` and `norrath_w10_actor_d_view_7d7770` are my names for two 80-byte `ParticleDef` statics of the Chest translation unit (the image's `fuseParticle` at 0x7d7720 and `smokeParticle` at 0x7d7770; the image has several identically named file-local statics, so the address-suffixed name avoids picking the wrong one). `norrath_w10_actor_d_view_7d6c40` is the 80-byte `breathParticle1` static at 0x7d6c40. Their storage owner is the respective class's static initializer, which is not part of this package.
- `_vt$8Creature`, `_vt$10GameObject`, `_vt$7Critter`, `_vt$10CloudGiant` and `_vt$6Cthulu` are the vtable data; the vtable views other packages author carry other names, so the integrator has to alias or rename them.
- `chestResourceList` is the 32-byte file-local table at 0x42c8a0 that `msg_load__5ChestPc` indexes.
- `_4item$globalItem`, `catagry` (the table {{0, 1, 2}} is defined locally in `msg_poke__5Chesti`'s unit, not imported), `skills`, `bossMeterName`, `bossMeterMax`, `bossMeterCurrent`, `cutMode`, `world`, `worldDifficulty`, `levelFlags`, `nmPlayers`, `player`, `frameNm`, `gameCreatureThinkSpinner` are existing data names.
- The soft-double entry points are used under their original names: `fptodp`, `dpadd`, `dpsub`, `dpmul`, `dpcmp`, `dptofp`.

## Build-plan entries needed

Profile `authored_cpp`. Do not add `host_check.cpp`. The integrator assigns `input_order` and adds `sources/w10-actor-d-common/words.hpp` (shared by the units whose `header_closure` names it) to the file list. Object names are unique per unit. `emitted_functions` lists the functions each object defines; the type-info units additionally define a 12-byte `__ti<Class>` node in `.bss`.

```json
{"source":"sources/w10-actor-d-chest-msg-load/msg_load.cpp","object_name":"w10_actor_d_chest_msg_load.o","sha256":"6d34f5d74bbd4341a082a8d6773f43d719feffe01401aacbbb95165456da2dea","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_load__5ChestPc","compiled_body_bytes":220}]}
{"source":"sources/w10-actor-d-chest-msg-poke/msg_poke.cpp","object_name":"w10_actor_d_chest_msg_poke.o","sha256":"ca76a6df0c34551226f26ad29fd90e7cd4b448522689c3c3d734ffeac5275f09","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__5Chesti","compiled_body_bytes":1696}]}
{"source":"sources/w10-actor-d-chest-msg-run/msg_run.cpp","object_name":"w10_actor_d_chest_msg_run.o","sha256":"63fd991e840d0c5543ed257661dad841d2d206080e2c3c0588e38e0684b80ab7","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__5Chest","compiled_body_bytes":1272}]}
{"source":"sources/w10-actor-d-chest-msg-save/msg_save.cpp","object_name":"w10_actor_d_chest_msg_save.o","sha256":"10ed81bb5e8be861f8f560ddbec860e032fc086b1149508acd50cd34c0def2bf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_save__5ChestPc","compiled_body_bytes":48}]}
{"source":"sources/w10-actor-d-clone-7critter/clone.cpp","object_name":"w10_actor_d_clone_7critter.o","sha256":"c0f0055088e47dd37bf74f5406fbf3d67f37389b1d2ba47d33fc2f6bc40cecfb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__7Critter","compiled_body_bytes":780}]}
{"source":"sources/w10-actor-d-cloudgiant-msg-draw/msg_draw.cpp","object_name":"w10_actor_d_cloudgiant_msg_draw.o","sha256":"e6fafc7eba77148e9e35351503758cb330772377c5177f674807641b164d0110","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__10CloudGiant","compiled_body_bytes":128}]}
{"source":"sources/w10-actor-d-cloudgiant-msg-hurt/msg_hurt.cpp","object_name":"w10_actor_d_cloudgiant_msg_hurt.o","sha256":"ab2d201e283dee88eadae07493db1a307ce34a33c8f1dec37f9e7bdd75dee3fe","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__10CloudGiantfP10GameObjecti","compiled_body_bytes":64}]}
{"source":"sources/w10-actor-d-cloudgiant-msg-load/msg_load.cpp","object_name":"w10_actor_d_cloudgiant_msg_load.o","sha256":"eccc4e1c15b11532910c938728a2112ac061c5fbe7311c68ea7fcdcf20cfc9ee","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_load__10CloudGiantPc","compiled_body_bytes":76}]}
{"source":"sources/w10-actor-d-cloudgiant-msg-run/msg_run.cpp","object_name":"w10_actor_d_cloudgiant_msg_run.o","sha256":"129ce5fadb9773ee4fec3851e946b3f11547fd429f50dc12b662b2c319002c07","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__10CloudGiant","compiled_body_bytes":6620}]}
{"source":"sources/w10-actor-d-cloudgiant-msg-save/msg_save.cpp","object_name":"w10_actor_d_cloudgiant_msg_save.o","sha256":"8dd99446edf801ba57bb8e9e8ee4f3b068414a4da026fe7ecef21a821587e545","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_save__10CloudGiantPc","compiled_body_bytes":56}]}
{"source":"sources/w10-actor-d-cloudgiant-msg-trigger/msg_trigger.cpp","object_name":"w10_actor_d_cloudgiant_msg_trigger.o","sha256":"a85a5dfeb29cae3774c01d236dbaf0ccbb380864b6cbfaf02764785e5af1398d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_trigger__10CloudGianti","compiled_body_bytes":160}]}
{"source":"sources/w10-actor-d-cloudgiant-set-state/set_state.cpp","object_name":"w10_actor_d_cloudgiant_set_state.o","sha256":"682a4260ced0824553dc692cc59a8facd21d659d9d357917d93f51a8ba8b9581","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__10CloudGianti","compiled_body_bytes":1608}]}
{"source":"sources/w10-actor-d-creature-ctor/creature_ctor.cpp","object_name":"w10_actor_d_creature_ctor.o","sha256":"19b8dde196f082a763416db3069515f1205a21f131e479f5e48336eb72a55cfd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__8CreatureG6Point3sPQ210GameObject11resourceMapfPPc","compiled_body_bytes":944}]}
{"source":"sources/w10-actor-d-creature-drop-treasure/drop_treasure.cpp","object_name":"w10_actor_d_creature_drop_treasure.o","sha256":"32c11e258b858bde5f82045530841466d3d24135da02663ce9554217fa7e2d4f","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"dropTreasure__8CreatureG6Point3iffffffPc","compiled_body_bytes":1060}]}
{"source":"sources/w10-actor-d-critter-get-model/get_model.cpp","object_name":"w10_actor_d_critter_get_model.o","sha256":"3999f03dffccb212299415557323fe7772ef135399968ee88333ec27f144fbb1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getModel__7Critter","compiled_body_bytes":8}]}
{"source":"sources/w10-actor-d-critter-msg-collision/msg_collision.cpp","object_name":"w10_actor_d_critter_msg_collision.o","sha256":"17a4331da6f74b4167d50c86edce2acac74c994386ec937ac7ae681c06b5e40b","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collision__7CritterP10GameObject","compiled_body_bytes":184}]}
{"source":"sources/w10-actor-d-critter-msg-draw/msg_draw.cpp","object_name":"w10_actor_d_critter_msg_draw.o","sha256":"f05680467504abc5fdafafea0dde514d7a007cf3913bf11e9733ff4f346c744d","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__7Critter","compiled_body_bytes":260}]}
{"source":"sources/w10-actor-d-critter-msg-hurt/msg_hurt.cpp","object_name":"w10_actor_d_critter_msg_hurt.o","sha256":"e7c3996b958b649389f6c5cd0119cb0270824f7655a437fef3050d775e7dfc6b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__7CritterfP10GameObjecti","compiled_body_bytes":8}]}
{"source":"sources/w10-actor-d-critter-msg-load/msg_load.cpp","object_name":"w10_actor_d_critter_msg_load.o","sha256":"d0579409e65b0e233ad18762c29ce4217fcbee6c1e55dbbf2a8228e72051b35c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_load__7CritterPc","compiled_body_bytes":8}]}
{"source":"sources/w10-actor-d-critter-msg-run/msg_run.cpp","object_name":"w10_actor_d_critter_msg_run.o","sha256":"3c7451c7069cbe60e23c7ea97beefa4cf48a152721f652b5031d77e02a240959","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__7Critter","compiled_body_bytes":2264}]}
{"source":"sources/w10-actor-d-critter-msg-save/msg_save.cpp","object_name":"w10_actor_d_critter_msg_save.o","sha256":"b1487dfbf4f1533905bb4cc389d95148b0c5df59d5b0483324cf20a00257b8c8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_save__7CritterPc","compiled_body_bytes":8}]}
{"source":"sources/w10-actor-d-cthulu-msg-draw/msg_draw.cpp","object_name":"w10_actor_d_cthulu_msg_draw.o","sha256":"13d1433e73fd044f88806e12b1e61936c29bdac092bce6a8da21e0df3afb083a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__6Cthulu","compiled_body_bytes":20}]}
{"source":"sources/w10-actor-d-cthulu-msg-move-collision/move_collision.cpp","object_name":"w10_actor_d_cthulu_msg_move_collision.o","sha256":"fc18b94d8cfa1a0e97a5662ca48ca82aae9a73469ffd8bc6ede4f0d75ef57bb9","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_moveCollision__6CthuluPP10GameObject","compiled_body_bytes":280}]}
{"source":"sources/w10-actor-d-cthulu-msg-run/msg_run.cpp","object_name":"w10_actor_d_cthulu_msg_run.o","sha256":"d4cfe967a3bcbb4f9eb5a437f2f67024cfe356b471733f935422a678fa08c714","header_closure":["sources/w10-actor-d-common/words.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__6Cthulu","compiled_body_bytes":2732}]}
{"source":"sources/w10-actor-d-dtor-cloudgiant/dtor.cpp","object_name":"w10_actor_d_dtor_cloudgiant.o","sha256":"a1695893a18f73c662caeef8828eba09693cf90597f867d4aeeff7cf394e29f6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_10CloudGiant","compiled_body_bytes":88}]}
{"source":"sources/w10-actor-d-dtor-critter/dtor.cpp","object_name":"w10_actor_d_dtor_critter.o","sha256":"ca58c09024c916dbd4af083753a4515649d7f20db36d28540d954eb09a385545","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_7Critter","compiled_body_bytes":88}]}
{"source":"sources/w10-actor-d-dtor-cthulu/dtor.cpp","object_name":"w10_actor_d_dtor_cthulu.o","sha256":"77d8702adb2392318bcd47954146c674eeaa1b168c2bc9a756aa909d2db41910","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_6Cthulu","compiled_body_bytes":88}]}
{"source":"sources/w10-actor-d-get-current-route-point/route_point.cpp","object_name":"w10_actor_d_get_current_route_point.o","sha256":"6a34d59d435497c9bcbc4e56a6731624bc53394b3a3a039ab923ea174ab91b27","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getCurrentRoutePoint__8Creature","compiled_body_bytes":116}]}
{"source":"sources/w10-actor-d-get-next-route-point/route_point.cpp","object_name":"w10_actor_d_get_next_route_point.o","sha256":"9b0184499031e5edc565fe99a55cdb434f06b33189188ef2e6f5de320f947384","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getNextRoutePoint__8CreatureP6Point3G6Point3ffi","compiled_body_bytes":188}]}
{"source":"sources/w10-actor-d-tf-cloudgiant/typeinfo.cpp","object_name":"w10_actor_d_tf_cloudgiant.o","sha256":"16aebc5e77bf10ef30244191747a63ddd01d828d96665c762da43f93f3e96bbe","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf10CloudGiant","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-d-tf-critter/typeinfo.cpp","object_name":"w10_actor_d_tf_critter.o","sha256":"385bdb9c99a5244ed1db314701df08d162283668abbedd1c9fd670f67b5441a3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf7Critter","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-d-tf-cthulu/typeinfo.cpp","object_name":"w10_actor_d_tf_cthulu.o","sha256":"ff1cc8663845fd8ade2b1b232b188f7b3fd9da6a15fdce4d085fccc315a186a6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf6Cthulu","compiled_body_bytes":92}]}
```

## Authored but never executed

Everything above: no function has run on the EE or in a game. Each was run only in the private interpreter against the image and in the host checks, with stubbed callees (including the soft-double routines, the random source, the sound and particle routines and all virtual calls). Specifically not exercised: real callee behaviour, memory hardware timing, the real virtual tables (host checks use scripted tables), floating-point rounding on the EE (the interpreter rounds to nearest where the EE truncates, so scenario inputs were chosen away from rounding edges and both sides use the same interpreter), and every use of the particle statics.

## Unknown

- The names and types of the fields behind almost every offset (Creature +160 flags, +208, +256, +576 list, +808 clock, +960..+1308 boss fields), of the states (Chest, Critter 0..16, Cthulu 0..16, CloudGiant 0..18) and of most tag names' use. No name in the sources is claimed to be the original's.
- The size of `item::description` (at most 80 bytes; only the bytes the image initialises are reproduced) and of `Matrix34` (48 bytes, from stack spacing).
- Whether `dropTreasure__4item...`'s trailing int and `fxRunWeapon`'s integer arguments are flags or counts.
- The table {0, 1, 2} that `msg_poke__5Chesti` indexes is the image's file-local `catagry` at 0x42c8c0; the source defines its own copy of those three words rather than importing it.
- Retail edition of the image (only the pinned private binary was used).
- No blockers requiring the owner. The only open item for the integrator is the vtable and static naming above.

## Process note

Commit `d2327b6f` (message "x") contains only `experimental/sources/w10-actor-d-common/words.hpp`. It was pushed by mistake from a helper script before I noticed; the header is used by later commits and its content is correct. History was not rewritten.
