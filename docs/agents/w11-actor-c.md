# w11-actor-c: actor class methods (3 of 3)

Branch `agents/w11-actor-c`. Private executable `work/private/SLUS_205.65`; GP is `0x004879f0`. Addresses and sizes
below are the image's own symbol table. Nothing here ran on the EE or inside the game.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++ and compiled
  alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0, `-G0 -O2 ... -march=r5900 -std=c++17`). Every unit
  compiled. Authored bodies are not byte-identical to the image (sizes differ); no byte-match claim is made.
- Every unit has a `host_check.cpp` built with the host C++ compiler (`-fno-builtin` where the unit declares `strcpy`,
  `strstr` or `sprintf` itself). Units that store 32-bit pointers in game structures use the `W11_ACTOR_C_HOST` switch of
  `sources/w11-actor-c-common/c.hpp`, which keeps stored addresses as offsets in a test arena and routes the object's
  table calls (`call_slot`, `call_slot_int`, `call_slot_int_ptr`, `call_slot_int_int`) through function arrays the check
  defines; callees are recording stubs. All 36 checks exit 0 (re-run after the last source commit).
- Private paired runs (outside git): a small R5900 interpreter loaded the original function and my freshly compiled one
  from the same memory snapshot with identical Python stub callees (hooks keyed by the original symbol names, fake
  vtable targets keyed by raw address) and compared the ordered call log (argument registers, float registers, the bytes
  behind pointer arguments, stack arguments, an object hash and the generator word at each call) and every changed byte
  outside the stack. Scenario counts are in the table below and in the commit messages. Instruction coverage of the
  original was measured; the words never executed are nops and branch-delay padding, plus a few unreachable paths named
  in the commit messages (Sparks `msg_collision` description scan advance, the `dropTreasure` zero-count skip, the
  `msg_run__6Wraith` description scan advance).
- Where a rare path hides behind a random draw (the 1% combineable-item roll in `dropTreasure`, the `& 0x1ff == 0` foam roll
  in `doWavelet`, the `& 0xff == 0` alternate particle) I searched the 32-bit generator for seeds that reach it, or forced a
  chosen draw to a boundary value by running the generator backwards, so those paths are compared too. The boundary run
  found two wrong thresholds in my first `dropTreasure` draft (the image compares with `0xee47` and `0xea5f`, that is
  60999 and 59999, not 61000 and 60000); they are fixed and the comparison now passes.
- Mutated copies (changed constants, thresholds, offsets, counts) were run through the same comparisons for `addSmokelet`,
  Sparks `msg_run`, `doWavelet`, `dropTreasure`, WoodElfSoldier `msg_run` and Wraith `msg_run`; each was detected, after
  adding targeted seeds for the boundary mutants that first slipped through. One mutant (`msg_run__6Wraith` state 14 fade bound 71 to 72) is provably equivalent: at
  71 the two steps cancel.
- Interpreter notes (private copy `priv-w11-actor-c`, derived from the wave-10 copies): the image's soft-double helpers
  (`fptodp`, `dpcmp`, `dpsub`, `dpmul`, `dptofp`) use the MMI `mult1` and halt the interpreter, so they are hooked with exact
  Python doubles on both sides. Rejection-sampled unit-ball loops need 16-bit rolls in the hook (a 31-bit roll never
  accepts). A hook's printed stack addresses differ between original and authored code, so only contents are logged.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK, after the last source commit.
- I did not run `tools/build_partial.py` (the plan is not mine to edit); the plan entries below were produced by
  compiling each unit alone with the reviewed flags.

## Findings worth knowing (observed unless marked)

- `addImpulse__7VehicleP6Point3T1i` never reads its int argument. The torque is the cross product of the point and the
  impulse passed through `matUntransform` with the matrix at +1116 (inferred: body-frame torque).
- `doWavelet__9WaveMakerP6Point3P8Matrix33i`: the search box for nearby objects has the same x for both corners
  (`p.x - 12`), so it is a zero-width slab in x (observed; whether this is an authoring slip in the original is not known).
  It also computes `p - W` into a stack temporary that is never read; that is not reproduced.
- `__5WheelPcifP7Vehiclei` overwrites the five bytes at the `strstr(copy, ".vif")` hit with `".tex"` and a NUL, which
  also truncates any text that followed; the `strstr` result is not tested, so a name without `.vif` stores through a null
  pointer. The constructor returns whatever is in the result register, so it is declared `void`.
- `msg_collision__6Sparks` fills an item description with four 8-byte entries and picks the first entry whose first
  halfword is -1 or 31; because the scan starts with every entry at -1 the first entry is always chosen and the advance
  path is unreachable (kept, since the image has it).
- `dropTreasure__4itemG6Point3iffffffPci`: the generator state is advanced in memory and in a register; the image stores
  it only before calls that might read it. Plain C++ updates of `syncRandState` reproduce every observable store (the
  paired runs hash the global at each call). With two or more players the third and fourth floats are doubled and the
  gold chance is multiplied by 1.5 through the soft-double helpers. `tab.1068` (12 rows of three floats, 144 bytes at
  0x31f480) is local data of the image; its words are reproduced in the unit.
- Several image functions keep file-local data of their translation unit (particle definitions, `skelname.1044`, the
  construct-once flag `_$tmp_0.1046`, `splashInfoBits`). They are data imports here, not authored data (see Imports opened).
- The eight units that the earlier wave left without checks (`WoodElfSoldier` clone, dropItem, msg_run, msg_trigger,
  setState and `Wraith` clone, msg_run, setState; their sources are in `w10-actor-q-*`) are re-authored here as
  `w11-actor-c-*` units with checks. The clones were regenerated from the copy ranges; the five behavioural units are
  copies of the earlier text with the virtual-slot calls routed through the shared host-aware helpers, and
  `msg_run__6Wraith` takes its file-local data as imports. The paired runs found no behavioural difference.
  `w10-actor-q-wraith-get-miniboss-name` was excluded by the integrator because its host check used an undeclared
  `seen_dest`; `getMinibossName__6Wraith` here is a fresh unit with a working check.

## Functions

Image address, size in the image, size of the authored object body, and paired scenarios (about for the large random runs).

| function | image address | image bytes | authored object bytes | paired scenarios |
|---|---|---|---|---|
| `dropTreasure__4itemG6Point3iffffffPci` | 0x100e20 | 2484 | 2092 | 340 |
| `clone__14WoodElfSoldier` | 0x121578 | 1420 | 892 | 7 |
| `clone__6Wraith` | 0x121b08 | 1376 | 840 | 7 |
| `clone__6Sparks` | 0x124f60 | 528 | 464 | 11 |
| `doWavelet__9WaveMakerP6Point3P8Matrix33i` | 0x1521e8 | 1872 | 1740 | 150 |
| `doWaveletReflection__9WaveMakerP6Point3` | 0x152938 | 300 | 288 | 60 |
| `doWaveletCollision__9WaveMakerP6Point3i` | 0x152a68 | 608 | 528 | 60 |
| `getWaveletMat__9WaveMakerifP8Matrix33` | 0x155e48 | 192 | 188 | 336 |
| `msg_run__6Wraith` | 0x203f08 | 3400 | 3492 | ~55000 |
| `setState__6Wraithi` | 0x204c50 | 932 | 1120 | 273 |
| `getMinibossName__6Wraith` | 0x2052e8 | 60 | 56 | 57 |
| `start__3WebG6Point3P8Creaturefi` | 0x20dc68 | 380 | 300 | 54 |
| `draw__3WebG6Point3` | 0x20dde8 | 588 | 424 | 84 |
| `run__3WebG6Point3i` | 0x20e038 | 1112 | 1008 | 900 |
| `canGrab__11SpiderQueen` | 0x20fa78 | 80 | 104 | 192 |
| `reset__3Web` | 0x20fae0 | 12 | 12 | 6 |
| `breakWeb__3Web` | 0x20fb88 | 52 | 56 | 40 |
| `bActive__3Web` | 0x20fbd0 | 12 | 12 | 8 |
| `msg_run__6Sparks` | 0x216970 | 1716 | 1468 | 144 |
| `msg_collision__6SparksP10GameObject` | 0x217028 | 544 | 512 | 64 |
| `msg_draw__6Sparks` | 0x21d450 | 144 | 164 | 14 |
| `convertToBat__11VampireLord` | 0x2436d8 | 108 | 108 | 6 |
| `convertToLord__11VampireLord` | 0x243748 | 180 | 176 | 6 |
| `msg_run__14WoodElfSoldier` | 0x2477b0 | 3360 | 3500 | ~27000 |
| `msg_trigger__14WoodElfSoldieri` | 0x2484d0 | 444 | 428 | 24 |
| `setState__14WoodElfSoldieri` | 0x248690 | 2472 | 3204 | ~1000 |
| `dropItem__14WoodElfSoldieri` | 0x249038 | 976 | 1088 | 96 |
| `drawArrow__14WoodElfSoldier` | 0x249eb0 | 192 | 172 | 30 |
| `addImpulse__7VehicleP6Point3T1i` | 0x272e20 | 336 | 272 | 80 |
| `__5WheelPcifP7Vehiclei` | 0x276fc8 | 436 | 416 | 90 |
| `SetNaturalRPM__5Wheel` | 0x2783c0 | 148 | 136 | 8 |
| `ModifyRPM__5Wheelf` | 0x278458 | 8 | 8 | 6 |
| `addSmokelet__10StaticFire` | 0x28f118 | 560 | 512 | 300 |
| `updateSmokelets__10StaticFire` | 0x28f518 | 208 | 212 | 300 |
| `__tf6Sparks` | 0x310b80 | 132 | 184 | 6 |
| `__tf14SpellHarmTouch` | 0x3178a8 | 76 | 92 | 6 |

## Imports closed

`addSmokelet__10StaticFire`, `canGrab__11SpiderQueen`, `clone__14WoodElfSoldier`, `clone__6Wraith`, `convertToBat__11VampireLord`, `convertToLord__11VampireLord`, `getMinibossName__6Wraith`, `dropTreasure__4itemG6Point3iffffffPci`, `msg_draw__6Sparks`, `clone__6Sparks`, `msg_collision__6SparksP10GameObject`, `msg_run__6Sparks`, `__tf14SpellHarmTouch`, `__tf6Sparks`, `updateSmokelets__10StaticFire`, `addImpulse__7VehicleP6Point3T1i`, `doWavelet__9WaveMakerP6Point3P8Matrix33i`, `doWaveletCollision__9WaveMakerP6Point3i`, `doWaveletReflection__9WaveMakerP6Point3`, `getWaveletMat__9WaveMakerifP8Matrix33`, `bActive__3Web`, `breakWeb__3Web`, `draw__3WebG6Point3`, `reset__3Web`, `run__3WebG6Point3i`, `start__3WebG6Point3P8Creaturefi`, `drawArrow__14WoodElfSoldier`, `dropItem__14WoodElfSoldieri`, `msg_run__14WoodElfSoldier`, `msg_trigger__14WoodElfSoldieri`, `setState__14WoodElfSoldieri`, `__5WheelPcifP7Vehiclei`, `ModifyRPM__5Wheelf`, `SetNaturalRPM__5Wheel`, `msg_run__6Wraith`, `setState__6Wraithi`.

All 36 names were in the wave-start unresolved list. Callers are the game-object dispatch (msg_run/msg_draw/clone) and the
authored actor constructors.

## Imports opened

Symbols my sources reference but do not define (from `nm` of only my objects, 133 names). Three groups.

Already unresolved in the integration branch's list (`work/int-final-0345/unresolved-imports.json`, not newly opened): `DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf`, `__8ListHeadRC8ListHead`, `__ti5Spell`, `_vt$10GameObject`, `_vt$14WoodElfSoldier`, `_vt$6Wraith`, `_vt$8Creature`, `creatureFindLeadYaw__8CreatureP10GameObjectf`, `enemyInZone__8Creaturefs`, `flee2__8CreatureP10GameObject`, `floorAdd__FG6Point3sfii`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `lightAdd__FP5Light`, `lightUpdatePosition__FP5Light`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `splashInfoBits`, `steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature`, `waveMainAlternate`, `waveMainParticle`, `waveRandomFoam`, `waveSplashBig`, `waveSplashSmall`, `waveTrail`.

Not in that list and not defined by the current `experimental/build-plan.json` (opened by my sources, or defined by a unit that
is not in this branch's plan; the integrator's full build decides): `P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf`, `TurnToFaceEdge__14WoodElfSoldier`, `_4item$globalItem`, `__7WebTrapG6Point3T1`, `__8SkeletonG6Point3sPPcPci`, `__tf5Spell`, `__ti10GameObject`, `__ti10Projectile`, `_vt$10Projectile`, `_vt$5Wheel`, `_vt$6Sparks`, `createNamedRandom__4itemPQ24item11descriptioniii`, `createRandomCombineableItem__4itemPQ24item11descriptionsPc`, `dropGold__4itemG6Point3i`, `frameNm`, `haloTexture1`, `joinCommander__14WoodElfSoldier`, `listDestroy__FP8ListHead`, `lumpFindResource__FPcT0`, `moveArrow__14WoodElfSoldier`, `move__10ProjectileG6Point3`, `msg_trigger__8Creaturei`, `nmPlayers`, `skills`, `soundPan2D__FiP6Point3f`, `sprintf`, `start__3WebG6Point3T1i`, `syncRandState`, `world`, `worldDifficulty`.

New data imports named by address (same convention as the earlier waves), all file-local data of the original
translation units that the authored code uses but does not own: `norrath_w11_actor_c_view_7d7540` (`trailParticle`, Sparks),
`norrath_w11_actor_c_view_7d7590` (`fireSmokeParticle`), `norrath_w11_actor_c_view_7d7630` (`fireHitParticle`),
`norrath_w11_actor_c_view_7d7680` (`fireSmoke`), `norrath_w11_actor_c_view_7d7110` (`wraithParticle.1045`),
`norrath_w11_actor_c_view_4852b0` (`skelname.1044`, a char pointer to the text "Skeleton"), `norrath_w11_actor_c_view_4852ac`
(`_$tmp_0.1046`, the construct-once flag of the Wraith particle definition). The paired runs bound them to those image
addresses. The other file-local objects (`waveTrail`, `waveSplashBig`, `waveSplashSmall`, `waveMainParticle`,
`waveMainAlternate`, `waveRandomFoam`, `splashInfoBits`) keep their image names like the existing WaveMaker unit.

Already defined by a unit in the current plan (70 names, so resolved there): `P_AddParticle__FP11ParticleDefP6Point3T1f`, `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`, `__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`, `__15AnimationState8RC15AnimationState8`, `__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `__rtti_si`, `__tf10GameObject`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animGlobalSync__FP14AnimationState`, `bExitStatePersueIdle__8Creature`, `bFarFromHome__8Creature`, `bNextToEdge__14WoodElfSoldier`, `blockAlloc__Fi`, `commanderValid__14WoodElfSoldier`, `createRandom__4itemPQ24item11descriptioniiii`, `create__4itemPcPQ24item11descriptionii`, `creatureAdvanceAnimation__8Creaturef`, `dpcmp`, `dpmul`, `dpsub`, `dptofp`, `dropTreasure__8CreatureG6Point3iffffffPc`, `eRand__10GameObject`, `everyFrame__8CreatureP12_modelHeader`, `findTagString__10GameObjectPci`, `fptodp`, `gameItemTreasureRamp__Fv`, `gameTreasureRamp__Fv`, `getProxyMeshId__4itemPQ24item11description`, `getSmartRandomDestination__8Creaturei`, `iatan2__Fff`, `icos__Fi`, `initStuff__6PhPropi`, `isin__Fi`, `leaveCommander__14WoodElfSoldier`, `listOnewayLink__FP8ListHeadT0Pv`, `makeExplosion__FG6Point3f`, `makeRandomPotion__4itemPQ24item11description`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `matRot__FP8Matrix33is`, `matRot__FP8Matrix34is`, `matTransform__FP6Point3P8Matrix33T0`, `matUntransform__FP6Point3P8Matrix33T0`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`, `modelGetActiveChunks__FP12_modelHeaderi`, `modelGetBoundingBox__FP12_modelHeaderlPiN52`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `newDamageHandler__8CreatureP12_modelHeader`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`, `objectAddToRunList__FP10GameObject`, `objectAddToSlowRunList__FP10GameObject`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectRadialDamage__FP10GameObjectG6Point3iffiii`, `objectUpdateInGrid__FP10GameObject`, `resetRouting__8Creature`, `setStateProcessing__8CreaturePi`, `slideBack__8Creaturef`, `smartWander__8Creature`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `steerAndRouteToPosition__8CreatureG6Point3`, `strcpy`, `strstr`, `worldFindHeight__FP12_worldHeaderG6Point3f`, `worldPerturbWater__FP12_worldHeaderG6Point3ff`.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values after the current tip. `header_closure` is the shared
`sources/w11-actor-c-common/c.hpp` where listed, `profile` is `authored_cpp`, and no `host_check.cpp` is an input
(each includes host headers). Body sizes are from the reviewed EE g++ `authored_cpp` profile, not from the original
executable. No `private_object_sha256` is claimed. Each unit emits the symbol it replaces plus, in a few units, local helper
symbols the compiler emits, listed so the plan matches the object.

Duplicate definitions: do **not** also select `w10-actor-q-wes-clone`, `w10-actor-q-wes-drop-item`, `w10-actor-q-wes-msg-run`,
`w10-actor-q-wes-msg-trigger`, `w10-actor-q-wes-set-state`, `w10-actor-q-wraith-clone`, `w10-actor-q-wraith-msg-run`,
`w10-actor-q-wraith-set-state` or `w10-actor-q-wraith-get-miniboss-name`; they define the same symbols as the units below
(`w11-actor-c-wes-*`, `w11-actor-c-clone-wes`, `w11-actor-c-wraith-*`, `w11-actor-c-clone-wraith`,
`w11-actor-c-get-miniboss-name-6wraith`), which carry the checks.

```json
{"emitted_functions":[{"compiled_body_bytes":512,"name":"addSmokelet__10StaticFire"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_add_smokelet_10staticfire.o","profile":"authored_cpp","sha256":"d7a2cda44159838945fdc545128b6517fd14521f5f6c6a73c986d487285a0488","source":"sources/w11-actor-c-add-smokelet-10staticfire/add_smokelet.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"canGrab__11SpiderQueen"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_can_grab_11spiderqueen.o","profile":"authored_cpp","sha256":"61f92b3905a73db1ed897a65e0facdf0d4ea562faba9403fd7b7cfac7e169e58","source":"sources/w11-actor-c-can-grab-11spiderqueen/can_grab.cpp"}
{"emitted_functions":[{"compiled_body_bytes":892,"name":"clone__14WoodElfSoldier"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_clone_wes.o","profile":"authored_cpp","sha256":"819db6affe912f8a6eb5cda74e4389feaf976f994fd2bb4d212d7d462aa3acbd","source":"sources/w11-actor-c-clone-wes/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":840,"name":"clone__6Wraith"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_clone_wraith.o","profile":"authored_cpp","sha256":"8e30f04bfeec35963c15cc49d5ef68c7296fd2ccaba527c222b39d5d603ff019","source":"sources/w11-actor-c-clone-wraith/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":108,"name":"convertToBat__11VampireLord"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_convert_to_bat_11vampirelord.o","profile":"authored_cpp","sha256":"5c5a7575ef1361cbce494c4a4abf947bc0b40442ff44c6129d36c319514a7a0d","source":"sources/w11-actor-c-convert-to-bat-11vampirelord/convert_to_bat.cpp"}
{"emitted_functions":[{"compiled_body_bytes":176,"name":"convertToLord__11VampireLord"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_convert_to_lord_11vampirelord.o","profile":"authored_cpp","sha256":"5c4150e090902ad7aee5760fde1cd6d2d57e8284aab0313f8285121aa4542b13","source":"sources/w11-actor-c-convert-to-lord-11vampirelord/convert_to_lord.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"getMinibossName__6Wraith"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_get_miniboss_name_6wraith.o","profile":"authored_cpp","sha256":"0dca2e7feadf34f3419827bf1ad13fb599b6313dd3245f2fdf3a99a8d21733ea","source":"sources/w11-actor-c-get-miniboss-name-6wraith/get_miniboss_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"build_prop"},{"compiled_body_bytes":2092,"name":"dropTreasure__4itemG6Point3iffffffPci"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_item_drop_treasure.o","profile":"authored_cpp","sha256":"50645522b601c27f55b62bc11584d8cc21ecc4a156c20d70cf11c48231af214a","source":"sources/w11-actor-c-item-drop-treasure/drop_treasure.cpp"}
{"emitted_functions":[{"compiled_body_bytes":164,"name":"msg_draw__6Sparks"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_msg_draw_6sparks.o","profile":"authored_cpp","sha256":"8eaf6bf177689dd12d562061702cb44abfc8976b03ccc8a608d1f4a69211e560","source":"sources/w11-actor-c-msg-draw-6sparks/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":464,"name":"clone__6Sparks"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_sparks_clone.o","profile":"authored_cpp","sha256":"710b1ef48c1f778efc5379211c77a77d992ead616114dea240949dda8c64816a","source":"sources/w11-actor-c-sparks-clone/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":512,"name":"msg_collision__6SparksP10GameObject"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_sparks_msg_collision.o","profile":"authored_cpp","sha256":"c692320cd32fa954f27ca00712a9e8506bc157e3db2aba2aa789ef92bbcb86ec","source":"sources/w11-actor-c-sparks-msg-collision/msg_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1468,"name":"msg_run__6Sparks"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_sparks_msg_run.o","profile":"authored_cpp","sha256":"263ad91980311c7ed6c69248a0556cccbce6b488ede8cdd15872c2a675713677","source":"sources/w11-actor-c-sparks-msg-run/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf14SpellHarmTouch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_tf_14spellharmtouch.o","profile":"authored_cpp","sha256":"4c7f441af6e52f18c29a0a3f5c2836689d2601146872efb198c3f6c3be81c820","source":"sources/w11-actor-c-tf-14spellharmtouch/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":184,"name":"__tf6Sparks"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_tf_6sparks.o","profile":"authored_cpp","sha256":"2a833e2291dd9fea76e1fe082d72df3c955f31d451a497365644634574828cc9","source":"sources/w11-actor-c-tf-6sparks/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":212,"name":"updateSmokelets__10StaticFire"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_update_smokelets_10staticfire.o","profile":"authored_cpp","sha256":"63357c8cd1dad89b708f9cc70833bb01eb4fb7bd21e91e3ec67f4a1933a5d8a3","source":"sources/w11-actor-c-update-smokelets-10staticfire/update_smokelets.cpp"}
{"emitted_functions":[{"compiled_body_bytes":272,"name":"addImpulse__7VehicleP6Point3T1i"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_vehicle_add_impulse.o","profile":"authored_cpp","sha256":"518b77a1646e317ebae1c20f9cf24e5bef61aaa5b5179b48a4c39323baf9af9d","source":"sources/w11-actor-c-vehicle-add-impulse/add_impulse.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1740,"name":"doWavelet__9WaveMakerP6Point3P8Matrix33i"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wavemaker_do_wavelet.o","profile":"authored_cpp","sha256":"aca5656e8bc015bf496c90871175f2539ec1368a8c5a0bab8b6ea628d8cf266f","source":"sources/w11-actor-c-wavemaker-do-wavelet/do_wavelet.cpp"}
{"emitted_functions":[{"compiled_body_bytes":528,"name":"doWaveletCollision__9WaveMakerP6Point3i"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_wavemaker_do_wavelet_collision.o","profile":"authored_cpp","sha256":"0fd15100f69c829953572931156b8c40776e483063361be577bb14fa6b5e8ac1","source":"sources/w11-actor-c-wavemaker-do-wavelet-collision/do_wavelet_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":288,"name":"doWaveletReflection__9WaveMakerP6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_wavemaker_do_wavelet_reflection.o","profile":"authored_cpp","sha256":"6e2ee9c6c987364d0d5e4db51290409f3e77f3fda0b3d20a92eea850b2de2f6b","source":"sources/w11-actor-c-wavemaker-do-wavelet-reflection/do_wavelet_reflection.cpp"}
{"emitted_functions":[{"compiled_body_bytes":188,"name":"getWaveletMat__9WaveMakerifP8Matrix33"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wavemaker_get_wavelet_mat.o","profile":"authored_cpp","sha256":"b5e4cf9cb2acf711a98ad5da24952539cea5950cd26b2501c33356afe5699cca","source":"sources/w11-actor-c-wavemaker-get-wavelet-mat/get_wavelet_mat.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"bActive__3Web"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_web_b_active.o","profile":"authored_cpp","sha256":"c9cceed3a8f7dcd60bb61aab7c25817a47182306b1b0b2d1966763f03f01f671","source":"sources/w11-actor-c-web-b-active/b_active.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"breakWeb__3Web"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_web_break.o","profile":"authored_cpp","sha256":"9259ba480bad0ddc3e2fd71a5361d166204d91471af5550dd810b6db3cee73ec","source":"sources/w11-actor-c-web-break/break_web.cpp"}
{"emitted_functions":[{"compiled_body_bytes":424,"name":"draw__3WebG6Point3"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_web_draw_point.o","profile":"authored_cpp","sha256":"77e61fdc447e82c11e519b620721096973db3089bcb987baf5688eed10970673","source":"sources/w11-actor-c-web-draw-point/draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"reset__3Web"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_web_reset.o","profile":"authored_cpp","sha256":"4b91fa972b0274bf62cef3a1be77188e4a80d8b945bb7557a4784a17e9cb026b","source":"sources/w11-actor-c-web-reset/reset.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1008,"name":"run__3WebG6Point3i"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_web_run_point_int.o","profile":"authored_cpp","sha256":"60975fcf5dc4aaf31a450aec314c0d7e3aa010c0a8de2a2bc5bbdc24fd844466","source":"sources/w11-actor-c-web-run-point-int/run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":300,"name":"start__3WebG6Point3P8Creaturefi"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_web_start_point_creature.o","profile":"authored_cpp","sha256":"5264b58688d9d9219660ca216403b0241ac74b9317ae1c679ffb3c1adf1ed312","source":"sources/w11-actor-c-web-start-point-creature/start.cpp"}
{"emitted_functions":[{"compiled_body_bytes":172,"name":"drawArrow__14WoodElfSoldier"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wes_draw_arrow.o","profile":"authored_cpp","sha256":"aaa5a580b302d3f9b7ceaeeb7449ec53e2f5b556131e3a536345b72b19a3b98b","source":"sources/w11-actor-c-wes-draw-arrow/draw_arrow.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1088,"name":"dropItem__14WoodElfSoldieri"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wes_drop_item.o","profile":"authored_cpp","sha256":"97eb12dbaaff5ea991635468e2a0a60553e5b7ab8d9ad3d400c431ba1097151f","source":"sources/w11-actor-c-wes-drop-item/drop_item.cpp"}
{"emitted_functions":[{"compiled_body_bytes":28,"name":"_ZN4w11c13call_slot_intEPvji.isra.0"},{"compiled_body_bytes":3500,"name":"msg_run__14WoodElfSoldier"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wes_msg_run.o","profile":"authored_cpp","sha256":"8f84b1160185a69bc476cc64ec1865eef3728ec2e8924d298c68b7b3cc3cdad7","source":"sources/w11-actor-c-wes-msg-run/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":428,"name":"msg_trigger__14WoodElfSoldieri"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wes_msg_trigger.o","profile":"authored_cpp","sha256":"98cab1912c010a9fba168997dac2f9e09be854d3803b5260aa411f85ec1a0d80","source":"sources/w11-actor-c-wes-msg-trigger/msg_trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":3204,"name":"setState__14WoodElfSoldieri"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wes_set_state.o","profile":"authored_cpp","sha256":"98d187a970d0b61985d13932389c0fffe3e4513621405f2ea0c4d940ebb4b98b","source":"sources/w11-actor-c-wes-set-state/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":416,"name":"__5WheelPcifP7Vehiclei"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wheel_ctor.o","profile":"authored_cpp","sha256":"80c0f02ed2f9651039851b005070193933532e46657a3a8046e8131677e97b97","source":"sources/w11-actor-c-wheel-ctor/wheel_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"ModifyRPM__5Wheelf"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_actor_c_wheel_modify_rpm.o","profile":"authored_cpp","sha256":"c8d44d3cd94cb1b838e972da82f88d2af79ae5196a5ee205aa94004364697e1a","source":"sources/w11-actor-c-wheel-modify-rpm/modify_rpm.cpp"}
{"emitted_functions":[{"compiled_body_bytes":136,"name":"SetNaturalRPM__5Wheel"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wheel_set_natural_rpm.o","profile":"authored_cpp","sha256":"84e931c25b662bf8451da4fda851458017b967a55cd9efead5c2e6969338b72d","source":"sources/w11-actor-c-wheel-set-natural-rpm/set_natural_rpm.cpp"}
{"emitted_functions":[{"compiled_body_bytes":3492,"name":"msg_run__6Wraith"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wraith_msg_run.o","profile":"authored_cpp","sha256":"dacad4aa6851cad7f65ada337bfa6234e81e690563d4597c43f92bc616baca85","source":"sources/w11-actor-c-wraith-msg-run/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1120,"name":"setState__6Wraithi"}],"header_closure":["sources/w11-actor-c-common/c.hpp"],"kind":"independently_authored_source","object_name":"w11_actor_c_wraith_set_state.o","profile":"authored_cpp","sha256":"edde1a7b8951e9246edb378dded557ad563f49ecb70895639fcce0ea6a31aeac","source":"sources/w11-actor-c-wraith-set-state/set_state.cpp"}
```

## Authored but never executed

All of it. Host contracts ran on this Mac, and the paired comparisons ran inside a private interpreter with stubbed
callees, not on the EE and not in the game. None of the callees (the Creature base class methods, the model, particle and
sound calls, the item creators, the skill table entries, the soft-double helpers) ran together with these functions.
Float behaviour was compared under the interpreter's model (round to nearest); the EE's truncating arithmetic,
denormals and NaN corners were sampled for control flow, not systematically for results. `getWaveletMat` was compared with NaN and
extreme blend factors and `addImpulse` with random floats. The seven data views above are not defined anywhere yet.

## Unknown

- What the states and the variant word at +964 mean for `WoodElfSoldier` and `Wraith` (only their effects are recorded),
  and most object fields (`+28`, `+256`, `+294`, `+576`, `+804`, `+808`, `+960`, `+968`, `+972..+988`, `+1888`).
- What the six floats of `dropTreasure` are called (the arithmetic is recorded: lift, speed scale, two gold factors, a
  quality value that indexes a table, and an item chance), what the table `tab.1068` expresses, and what the values 1089
  and 16385 passed to the `PhProp` constructor select.
- Which axis the bounding box in the `Wheel` constructor measures (the third and sixth ints of the six returned), and what the
  two ints and the float arguments of that constructor are.
- What the words at `Sparks` +84, +164, +176, +180, the `Web` fields and the `WaveMaker` halfwords at +1876/+1878 hold.
- Whether the zero-width x slab in the `doWavelet` object search is intended.
- The types behind `item::description` (its size is at most 144 bytes; the units reserve 72..144) and of the objects
  the particle and effect calls take.
