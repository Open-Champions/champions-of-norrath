# w10-actor-f: spell, skill and actor virtuals (FireBeetle to Generator)

Package `w10-actor-f`, branch `agents/w10-actor-f`. Private reference: the original executable
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`).
Addresses are that file's symbol values. No game bytes or disassembly listings are committed.
Everything below was compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here has
run on a PS2 or in a game; "paired" means the authored object was run next to the original
function in a private R5900 interpreter (kept outside git).

## How each function was checked

- Compile: every unit was compiled on its own with the plan's `authored_cpp` flags, and all 32 were
  then built together with the shared selection by the real driver (`tools/build_partial.py`) from a
  private copy of the plan (kept outside git, produced by `docs/agents/w10-actor-f-plan-entries.py`):
  3,506 sources compiled and linked with no retry.
- Paired run (private): the authored unit is linked with every undefined symbol bound to the
  original's address and run next to the original function from identical memory snapshots, with
  identical Python stand-ins for the callees. Compared: the ordered callee log (argument registers,
  float arguments, pointed-to vectors, stack arguments), and every changed byte outside the stack.
  Return values are not compared for functions that return nothing. Per-scenario callee answers
  come from a seeded random stream that is reset for each of the two runs. Instruction coverage of
  the original was measured for the large functions; the words never reached are listed under
  "Unknown". Single-constant mutations were confirmed to fail for the units marked in the commit
  messages. The interpreter is the pure-Python one described in the existing notes; its float model is
  double arithmetic rounded to single, with `cvt.w.s` saturating and `sqrt.s` taking its operand from `ft`.
- Host contract (`host_check.cpp` in each directory, not a build input): restates the expected
  behaviour as stub-callee sequences and field values, with the 32-bit object pointers mapped through
  the shared `vslots-b-bytes` helper. Every contract was rerun from scratch at the end and passes;
  several were mutation-checked.

## Functions

| directory | symbol | original address, bytes | compiled bytes (sum of the unit's emitted functions) | paired scenarios |
|---|---|---|---|---|
| `w10-actor-f-clone-7firefly` | `clone__7FireFly` | `0x00119fb0`, 1648 | 892 | 12 |
| `w10-actor-f-clone-8froglock` | `clone__8Froglock` | `0x0011a620`, 1580 | 820 | 12 |
| `w10-actor-f-clone-9generator` | `clone__9Generator` | `0x0011ac50`, 1408 | 788 | 12 |
| `w10-actor-f-dtor-7firefly` | `_$_7FireFly` | `0x0030ee18`, 124 | 176 | 24 |
| `w10-actor-f-dtor-8froglock` | `_$_8Froglock` | `0x0030ef28`, 100 | 112 | 24 |
| `w10-actor-f-dtor-9generator` | `_$_9Generator` | `0x0030efe0`, 108 | 136 | 24 |
| `w10-actor-f-firebeetle-msg-draw` | `msg_draw__10FireBeetle` | `0x00210338`, 744 | 1164 | 60 |
| `w10-actor-f-firebeetle-msg-move-collision` | `msg_moveCollision__10FireBeetlePP10GameObject` | `0x00211618`, 372 | 496 | 96 |
| `w10-actor-f-firebeetle-msg-run` | `msg_run__10FireBeetle` | `0x00210620`, 2668 | 3948 | 8000 |
| `w10-actor-f-firebeetle-set-state` | `setState__10FireBeetlei` | `0x00211090`, 1416 | 2416 | 212 |
| `w10-actor-f-firefly-move-collision` | `msg_moveCollision__7FireFlyPP10GameObject` | `0x0030ef20`, 8 | 8 | 2 |
| `w10-actor-f-firefly-msg-draw` | `msg_draw__7FireFly` | `0x00211ad0`, 564 | 1524 | 40 |
| `w10-actor-f-firefly-msg-run` | `msg_run__7FireFly` | `0x00211d08`, 612 | 1084 | 188 |
| `w10-actor-f-firefly-set-state` | `setState__7FireFlyi` | `0x00212518`, 8 | 8 | 3 |
| `w10-actor-f-froglock-msg-draw` | `msg_draw__8Froglock` | `0x001ffe10`, 228 | 340 | 16 |
| `w10-actor-f-froglock-msg-load` | `msg_load__8FroglockPc` | `0x00201140`, 116 | 180 | 20 |
| `w10-actor-f-froglock-msg-run` | `msg_run__8Froglock` | `0x001ffef8`, 3040 | 4012 | 8000 |
| `w10-actor-f-froglock-msg-save` | `msg_save__8FroglockPc` | `0x002010e8`, 84 | 120 | 4 |
| `w10-actor-f-froglock-set-state` | `setState__8Froglocki` | `0x00200ad8`, 1552 | 1856 | 237 |
| `w10-actor-f-game-object-ctor` | `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc` | `0x001411f0`, 1124 | 1624 | 6000 |
| `w10-actor-f-generator-get-model` | `getModel__9Generator` | `0x0030f0a0`, 8 | 8 | 2 |
| `w10-actor-f-generator-msg-draw` | `msg_draw__9Generator` | `0x001f1ff0`, 8 | 8 | 1 |
| `w10-actor-f-generator-msg-load` | `msg_load__9GeneratorPc` | `0x001f20b0`, 36 | 68 | 2 |
| `w10-actor-f-generator-msg-run` | `msg_run__9Generator` | `0x001f1b50`, 1180 | 1476 | 112 |
| `w10-actor-f-static-init-fireflare` | `_GLOBAL_$I$msg_run__9FireFlare` | `0x002a49d8`, 32 | 296 | 1 |
| `w10-actor-f-static-init-firestorm-flare` | `_GLOBAL_$I$msg_run__14FireStormFlare` | `0x0029d268`, 32 | 296 | 1 |
| `w10-actor-f-static-init-frost-shard` | `_GLOBAL_$I$msg_draw__10FrostShard` | `0x002be198`, 32 | 296 | 1 |
| `w10-actor-f-tf-7firefly` | `__tf7FireFly` | `0x0030ee98`, 136 | 192 | 10 |
| `w10-actor-f-tf-8froglock` | `__tf8Froglock` | `0x0030ef90`, 80 | 92 | 10 |
| `w10-actor-f-tf-9generator` | `__tf9Generator` | `0x0030f050`, 80 | 92 | 10 |
| `w10-actor-f-ti-10gameobject` | `__ti10GameObject` (data) | `0x00486dc8`, 8 | 8 | - |
| `w10-actor-f-vt-11firebomblet` | `_vt$11FireBomblet` (data) | `0x00450620`, 144 | 144 | relocation check |

Compiled bytes are the sum of the unit's emitted functions, including local helper functions that the
compiler did not inline. The two data rows are storage only. Where a function's compiled size is far
from the original's (the byte-loop clones, the large state machines), no match is claimed: these are
behaviour reconstructions.

## Findings

Observed (address evidence, private image):
- The word at `gp-11588` (`0x00484cac`) is the global float `worldDifficulty` (initial file value 1.0, written by
  `gameLoadWorld`). `msg_run__8Froglock` passes its truncated value as the power argument of the four arrow
  projectile constructors. An older note in `w9-rtti-vtables-entities-c-skelight-init-weapon` calls this word a
  small-data constant that truncates to 1; that is the file value before the game writes it.
- `_vt$11FireBomblet` (`0x00450620`) has 18 slots, all with zero adjustments; the last is null. Its own slots are
  `__tf`, `_$_`, `msg_run`, `msg_draw` and `clone` of FireBomblet; the rest are the GameObject defaults.
- FireFly sits above FireBeetle: its destructor stores the FireBeetle vtable (not calling the FireBeetle
  destructor) before the Creature destructor, and its type-info function registers the FireBeetle type-info
  node first. Froglock and Generator register directly on Creature.
- Resource-name strings in the sources (`"fire_beetle_eye"`, `"fx.lmp"`, `"swsh_arrow%d.anm"`) are the literal
  bytes at the cited positions of the image; no dialogue or asset data is committed.
- `msg_run__9Generator` calls `worldHitscanWithObjects` with a start point 10 above the object and a direction
  500 times the unit heading vector; the two 1.1 values it passes come from two separate words with the same bits.

Inferred (from names of the callees and the vtables in sibling units): virtual slot byte 168 is
`updateEnemy`, 176 `turnToFace`, 184 `setState`, 192 `death`, and byte 16 `msg_hurt`; the halfword at `+572`
is the creature state that `setState` stores. The word at `+576` of Froglock and FireBeetle behaves as a list
head whose first node's word at `+12` is an object (the flee, melee and aim code reads it that way). These names
are used only in comments.

Unknown: what the numbered states mean, what the fields at `+256`, `+804`, `+960`, `+964`, `+968`, `+972`,
`+976..987`, `+1104..1106` and `+1112..1123` hold beyond the observed use, the meaning of the small integer arguments
(`2051`, `2064`, `-2000`, `16385`, `0x40001`, `18000`, ...), and the animation header offsets (`at(n)`) used by the
two `setState` functions.

## Imports closed

All 32 names of the package scope are gone from the unresolved list of a full private partial build (3,506 sources,
one link, no retry; unresolved count 1,332 -> 1,326 because the new code also opened 26 names):

`msg_draw__10FireBeetle`, `msg_moveCollision__10FireBeetlePP10GameObject`, `msg_run__10FireBeetle`, `setState__10FireBeetlei`,
`_vt$11FireBomblet`, `_GLOBAL_$I$msg_run__9FireFlare`, `_$_7FireFly`, `__tf7FireFly`, `clone__7FireFly`, `msg_draw__7FireFly`,
`msg_moveCollision__7FireFlyPP10GameObject`, `msg_run__7FireFly`, `setState__7FireFlyi`, `_GLOBAL_$I$msg_run__14FireStormFlare`,
`_$_8Froglock`, `__tf8Froglock`, `clone__8Froglock`, `msg_draw__8Froglock`, `msg_load__8FroglockPc`, `msg_run__8Froglock`,
`msg_save__8FroglockPc`, `setState__8Froglocki`, `_GLOBAL_$I$msg_draw__10FrostShard`,
`__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`, `__ti10GameObject`, `_$_9Generator`, `__tf9Generator`, `clone__9Generator`,
`getModel__9Generator`, `msg_draw__9Generator`, `msg_load__9GeneratorPc`, `msg_run__9Generator`.

`__10GameObjectG6Point3PQ210GameObject11resourceMapPPc` is the same routine that the tree already has as
`norrath_game_object_construct` (`game-object-constructor-build-v3`); this unit writes it under the symbol that the
derived constructors import. Both can stay in the plan; nothing calls both. `__ti10GameObject` is storage only (eight
word-aligned zero bytes); the type-info function `__tf10GameObject` of `w9-startup-crt-b` reads and writes it.

## Imports opened

`_$_11FireBomblet`, `__tf11FireBomblet`, `msg_run__11FireBomblet`, `msg_draw__11FireBomblet`, `clone__11FireBomblet` (the
FireBomblet vtable's own slots); `__ti10FireBeetle` (the FireFly type-info function reads it; the FireBeetle type-info unit of
`w10-actor-e` defines it); `_vt$10GameObject`, `_vt$8Creature` (clones); `__8ListHeadRC8ListHead` (clones);
`__16beetleProjectileG6Point3T1iP8CreatureiiiiiP15AnimationHeader`, `__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`,
`__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill`,
`__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill` (projectile constructors); `bCrossingWater__8Froglock`,
`loseBodyPart__10FireBeetlei`; `enemyInZone__8Creaturefs`, `findEnemyLeadPosition__8CreaturefP6Point3`,
`flee2__8CreatureP10GameObject`, `steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature`,
`produceMeleeDamage__8CreaturefiiiiPQ24item11description`; `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`,
`modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`; `objectAddString__FPcP10GameObject` (GameObject constructor);
`worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii` (Generator run).

## Build-plan entries needed

The 32 entries below (JSON lines, same shape as the other packages; `input_order` is assigned by the integrator) are
reproduced by `python3 docs/agents/w10-actor-f-plan-entries.py --toolchain <ee> [--private-root DIR]`, which also writes
a private plan/snapshot copy for `tools/build_partial.py`. Header closures name only files that already exist in
the plan (`vslots-b-bytes/bytes.hpp`, `w8-spell-static-init-a-common/particle_def.hpp`).

```
{"emitted_functions": [{"compiled_body_bytes": 892, "name": "clone__7FireFly"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_clone_7firefly.o", "profile": "authored_cpp", "sha256": "aa47f5bf6422ece99013b0bed61e0fb82457e9b47e17e72fa3c0d643f1aae51a", "source": "sources/w10-actor-f-clone-7firefly/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 820, "name": "clone__8Froglock"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_clone_8froglock.o", "profile": "authored_cpp", "sha256": "10c01245b8d0882f8847ce18148013f13184ca7a984143dfea37bece1ec7998b", "source": "sources/w10-actor-f-clone-8froglock/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 788, "name": "clone__9Generator"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_clone_9generator.o", "profile": "authored_cpp", "sha256": "ad512f9f364d32032bcce7376badd2f8b66f48893b439937fb13e887f6b8688a", "source": "sources/w10-actor-f-clone-9generator/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "_$_7FireFly"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_dtor_7firefly.o", "profile": "authored_cpp", "sha256": "ca6ad03e71472b861fe1eee7ba60d4bd72a9161ca34f4e2bd16155281ec9c11e", "source": "sources/w10-actor-f-dtor-7firefly/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_8Froglock"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_dtor_8froglock.o", "profile": "authored_cpp", "sha256": "5fcd6cb99e16e9100dae80914b3fd799cc44138fb7585e51cb8efcf8ca65c0d8", "source": "sources/w10-actor-f-dtor-8froglock/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "_$_9Generator"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_dtor_9generator.o", "profile": "authored_cpp", "sha256": "733f2ffd332fc07cdf90cda770a208d568a83c1961dcbd45cd768b07f50a34b0", "source": "sources/w10-actor-f-dtor-9generator/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1164, "name": "msg_draw__10FireBeetle"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_firebeetle_msg_draw.o", "profile": "authored_cpp", "sha256": "aa98c09d5b4cfb892e7712f78088045e1706eaf77b78ac208eb027ae76acad1a", "source": "sources/w10-actor-f-firebeetle-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 496, "name": "msg_moveCollision__10FireBeetlePP10GameObject"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_firebeetle_msg_move_collision.o", "profile": "authored_cpp", "sha256": "502f990abfda413dd38cbec0246b724ac987d544175039ee535ef789deaa95e3", "source": "sources/w10-actor-f-firebeetle-msg-move-collision/msg_move_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "_ZN12_GLOBAL__N_1L9set_stateEPvi"}, {"compiled_body_bytes": 96, "name": "_ZN12_GLOBAL__N_1L13decrement_endEPvjb"}, {"compiled_body_bytes": 3804, "name": "msg_run__10FireBeetle"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_firebeetle_msg_run.o", "profile": "authored_cpp", "sha256": "afeec8f8b58ffc8fc8d6ba93114b8209b4576584e3eb739ef41995f6a400d200", "source": "sources/w10-actor-f-firebeetle-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "_ZN12_GLOBAL__N_1L10transitionEjjji.constprop.0"}, {"compiled_body_bytes": 44, "name": "_ZN12_GLOBAL__N_1L8one_shotEjjjji.constprop.0"}, {"compiled_body_bytes": 2336, "name": "setState__10FireBeetlei"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_firebeetle_set_state.o", "profile": "authored_cpp", "sha256": "f7c004553db326fc2166066085150489e189d1cf5f36bcb570901880d2b655ca", "source": "sources/w10-actor-f-firebeetle-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_moveCollision__7FireFlyPP10GameObject"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_firefly_move_collision.o", "profile": "authored_cpp", "sha256": "6a0ebc80b92238209800801358de45f6ffbbca9b2082102488060eaebe9827d6", "source": "sources/w10-actor-f-firefly-move-collision/msg_move_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1524, "name": "msg_draw__7FireFly"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_firefly_msg_draw.o", "profile": "authored_cpp", "sha256": "65d3bd07924ebd23766f00dc12e9555fc252c3162777ad2ea8d4a95828fbe3a6", "source": "sources/w10-actor-f-firefly-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1084, "name": "msg_run__7FireFly"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_firefly_msg_run.o", "profile": "authored_cpp", "sha256": "bcd155d5313e661964fc41dc2fcb03ee5d1e08dd9a03b482701c6d4153a4cb62", "source": "sources/w10-actor-f-firefly-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setState__7FireFlyi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_firefly_set_state.o", "profile": "authored_cpp", "sha256": "bd776fea2fee84a8523fac5dc7d9a7146f727b1ec190bbad392071a22da3e7f3", "source": "sources/w10-actor-f-firefly-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 340, "name": "msg_draw__8Froglock"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_froglock_msg_draw.o", "profile": "authored_cpp", "sha256": "483df1395c635b73be3f862107af7d711f8830b48aa7f6d8986e2fd700d6cc47", "source": "sources/w10-actor-f-froglock-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 180, "name": "msg_load__8FroglockPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_froglock_msg_load.o", "profile": "authored_cpp", "sha256": "51b3f113022e19e5b8f5c98d5a97a455de3b0c4f02b707416b46822c53fa8e9a", "source": "sources/w10-actor-f-froglock-msg-load/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "_ZN12_GLOBAL__N_1L9set_stateEPvi"}, {"compiled_body_bytes": 12, "name": "_ZN8vslots_b9read_wordEj"}, {"compiled_body_bytes": 3952, "name": "msg_run__8Froglock"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_froglock_msg_run.o", "profile": "authored_cpp", "sha256": "c79f8919b06753bc71294e0aacc362a7e704c9f7a73454fee438d1f5e3d68bf6", "source": "sources/w10-actor-f-froglock-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "msg_save__8FroglockPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_froglock_msg_save.o", "profile": "authored_cpp", "sha256": "1e26f4b3189c94574a76f9ab648ca5d132b18b8c08864d05c83d5528d31ce3a1", "source": "sources/w10-actor-f-froglock-msg-save/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "_ZN12_GLOBAL__N_1L10transitionEjjji"}, {"compiled_body_bytes": 40, "name": "_ZN12_GLOBAL__N_1L8one_shotEjji"}, {"compiled_body_bytes": 1780, "name": "setState__8Froglocki"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_froglock_set_state.o", "profile": "authored_cpp", "sha256": "5755f5dffee45fa8e5a1a39ab1c612c047e76de538350edf8988f94d771e2fbc", "source": "sources/w10-actor-f-froglock-set-state/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "_ZN12_GLOBAL__N_1L9grid_cellEj"}, {"compiled_body_bytes": 332, "name": "_ZN12_GLOBAL__N_1L10find_namedEjj"}, {"compiled_body_bytes": 1208, "name": "__10GameObjectG6Point3PQ210GameObject11resourceMapPPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_game_object_ctor.o", "profile": "authored_cpp", "sha256": "2dbef60d7a125a103c2ba326cb016492692d8ebdeac86c916bf2a7b3a6270ed4", "source": "sources/w10-actor-f-game-object-ctor/game_object_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getModel__9Generator"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_generator_get_model.o", "profile": "authored_cpp", "sha256": "aa0b074b3002f335f3670efb72cf825cf0db3df33dde61c67702f7afd5993d6e", "source": "sources/w10-actor-f-generator-get-model/get_model.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_draw__9Generator"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_generator_msg_draw.o", "profile": "authored_cpp", "sha256": "895e01c88937cc99ee869fe95a1dc9805595aedfa100f49db6f9c9760222b403", "source": "sources/w10-actor-f-generator-msg-draw/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "msg_load__9GeneratorPc"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_generator_msg_load.o", "profile": "authored_cpp", "sha256": "8ef810a3e92c18cd9aae9a72e967080f0208f533b65556116592596f217cd026", "source": "sources/w10-actor-f-generator-msg-load/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "_ZN12_GLOBAL__N_1L11set_state_3EPv"}, {"compiled_body_bytes": 400, "name": "_ZN12_GLOBAL__N_1L15hitscan_blockedEPvf.constprop.0"}, {"compiled_body_bytes": 1024, "name": "msg_run__9Generator"}], "header_closure": ["sources/vslots-b-bytes/bytes.hpp"], "kind": "independently_authored_source", "object_name": "w10af_generator_msg_run.o", "profile": "authored_cpp", "sha256": "fd862514a278240b8cd18e06a72eb5240635f0bc4af3a0cf9e6a0398869b4be8", "source": "sources/w10-actor-f-generator-msg-run/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 296, "name": "_GLOBAL_$I$msg_run__9FireFlare"}], "header_closure": ["sources/w8-spell-static-init-a-common/particle_def.hpp"], "kind": "independently_authored_source", "object_name": "w10af_static_init_fireflare.o", "profile": "authored_cpp", "sha256": "e7c47225f5b87c94a54b69302a81c3d0d1510714c4754d8b4bcb6de0ec033bab", "source": "sources/w10-actor-f-static-init-fireflare/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 296, "name": "_GLOBAL_$I$msg_run__14FireStormFlare"}], "header_closure": ["sources/w8-spell-static-init-a-common/particle_def.hpp"], "kind": "independently_authored_source", "object_name": "w10af_static_init_firestorm_flare.o", "profile": "authored_cpp", "sha256": "9f4b373fd4b63c2b8389ea9e6dd8808fac95a32489256c0df331e21ba9110249", "source": "sources/w10-actor-f-static-init-firestorm-flare/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 296, "name": "_GLOBAL_$I$msg_draw__10FrostShard"}], "header_closure": ["sources/w8-spell-static-init-a-common/particle_def.hpp"], "kind": "independently_authored_source", "object_name": "w10af_static_init_frost_shard.o", "profile": "authored_cpp", "sha256": "1ff756c92d1da1b3a87976e38d6c9da7b6bc9814974dbf5dbe39a9a1f9e87a9b", "source": "sources/w10-actor-f-static-init-frost-shard/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "__tf7FireFly"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_tf_7firefly.o", "profile": "authored_cpp", "sha256": "b873c41c677b0e07b4f1d1758efe099ace3aafa202a9a0dc63299f86f8156453", "source": "sources/w10-actor-f-tf-7firefly/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf8Froglock"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_tf_8froglock.o", "profile": "authored_cpp", "sha256": "ff4699cdd225b75093e18a36db7272520f06afd1fcf4974b75ad229921253f32", "source": "sources/w10-actor-f-tf-8froglock/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf9Generator"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_tf_9generator.o", "profile": "authored_cpp", "sha256": "f8d98683842af625f9f114cf37e90e4bbf9c58a78d3e6c225bc4fa317e96eda2", "source": "sources/w10-actor-f-tf-9generator/typeinfo.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_ti_10gameobject.o", "profile": "authored_cpp", "sha256": "cd4dd0633d9cc8fa5bee9e16cac719828507a145131182f8dc88eae6db312728", "source": "sources/w10-actor-f-ti-10gameobject/ti.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10af_vt_11firebomblet.o", "profile": "authored_cpp", "sha256": "6113047d5c66720e878f63442ffc489b9981e9cd85e0dcf8c141a7a7a720b928", "source": "sources/w10-actor-f-vt-11firebomblet/vtable.cpp"}
```

## Authored but never executed

All 32 units. Each compiled, ran on the host against recording stubs, and (except `__ti10GameObject`, which is
storage, and the vtable, which was compared by its relocation targets) ran next to the original in the private
interpreter, but none ran on an EE or in the game. Specifically unexercised: everything the callees do (the stand-ins
only record), the virtual slots reached through fake tables, allocation behaviour, and every path that depends on
live game data.

## Unknown, not reached or open

- Original instruction words never executed by the paired runs: alignment nops and delay-slot duplicates in all
  large functions; in `msg_moveCollision__10FireBeetlePP10GameObject` the null-object `__throw_bad_typeid` call;
  in the GameObject constructor the `ghostObjectGrid` branch (it tests bit 11 of a word the constructor has just
  zeroed); in `msg_run__8Froglock` one reload that only runs if a zone call changes `+960`.
- Float inputs in the paired runs were finite (including very large `worldDifficulty` and position values for the
  saturating truncation); infinities and NaN in the interpreter are not evidence, because its float model is
  not the R5900 FPU.
- Edition of the retail image is not established beyond the SHA above.
