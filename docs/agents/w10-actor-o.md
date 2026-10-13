# w10-actor-o: spell, skill and actor virtuals (Torch to Vampire)

Branch `agents/w10-actor-o`. Private executable `work/private/SLUS_205.65`; GP is `0x004879f0`. Addresses and sizes
below are the image's own symbol table. Nothing here ran on the EE or inside the game.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++ and compiled
  alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0, `-G0 -O2 ... -march=r5900 -std=c++17`). Every unit
  compiled. Authored bodies are not byte-identical to the image (sizes differ); no byte-match claim is made.
- Every unit has a `host_check.cpp` built with the host C++ compiler (the UndeadKnightClone `msg_run` check needs
  `-fno-builtin` so that `strcmp` stays a real call). Units that store 32-bit pointers in game structures use the
  `W10_ACTOR_O_HOST` switch of `sources/w10-actor-o-common/access.hpp`, which keeps stored addresses as offsets in a test
  arena; collaborators are recorded stubs. All exited 0.
- Private paired runs (outside git): a small R5900 interpreter loaded the original function and my freshly compiled one
  from the same memory snapshot with identical Python stub callees (hooks keyed by the original symbol names, fake vtable
  and skill-table targets keyed by raw address) and compared the ordered call log (argument registers, float registers,
  the bytes behind pointer arguments, stack arguments, a hash of the object at each call) and every changed byte outside
  the stack. Scenario counts are in the commit messages and the table below. Instruction coverage of the original was
  measured; the few words never executed are nops and branch-delay padding, plus three unreachable defaults in Ulthork
  `msg_run`.
- Mutated copies were run through the same comparisons for every non-trivial function (changed constants, offsets, loop
  counts, masks, thresholds, registers); each was detected. Mutations that first slipped through (a loop count, a boundary,
  a rare roll) led to extra targeted scenarios, so the comparisons now cover them.
- The interpreter copy used here (`priv-w10-actor-o`, derived from the wave-10 copies) has the `sqrt.s`/`rsqrt.s`/`cvt.w.s`
  fixes and counts a halted run as a failure. No run in this package halted after the fixes; hooks were added for
  `memset`, `strcmp` and `sprintf` where the image's MMI versions would have halted both sides.
- One more defect of the shared interpreter, found here: `div`/`divu` by zero left HI and LO unchanged instead of giving
  LO = -1 (1 for a negative dividend, 0xffffffff unsigned) and HI = the dividend as the R5900 does. A divide by a zero
  shadow-point count (UndeadKnight `msg_run` state 19, UndeadKnightClone `msg_run`) therefore compared stale registers and
  produced one false mismatch; it is fixed in my private copy and the affected comparisons were re-run with zero mismatches.
  Any older comparison that divides by a possibly-zero value on that interpreter should be re-run.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK, after the last source commit.
- I did not run `tools/build_partial.py` (the plan and snapshot are not mine to edit); the plan entries below were produced
  by compiling each unit alone with the reviewed flags.

## Findings worth knowing (observed unless marked)

- Several image functions capture a value in a branch delay slot before the call that precedes it returns: in the Vampire
  and UndeadKnightClone `msg_run` the flag word tested later is the return value of `creatureAdvanceAnimation`, not of
  `slideBack` (the `move` sits in the delay slot of the `jal` to `slideBack`). Ulthork `msg_run` state 13 uses the same
  trick for `creatureFindLeadYaw` (its result is the `short` yaw passed to the ShockOfFrost constructor).
- UndeadKnight `msg_draw` copies 12 bytes of its own never-written stack slot into the position at +16 when the word at
  +960 is non-zero, then subtracts an attachment point. I reproduced the read as an uninitialised local behind an empty asm;
  it is as indeterminate as the image's. Whether this is a defect in the original (it looks like a saved position that was
  never saved) is inferred, not known.
- UndeadKnightClone `msg_run` state 10 divides the random number by a constant zero (the `div` has no zero check; HI gets
  the dividend). The authored code spells the divide as `div`/`mfhi` inline asm so it does not gain a trap.
- Ulthork, UndeadKnight, UndeadKnightClone and Vampire share one `msg_run`/`setState` skeleton with the Torch-era
  creatures (`newDamageHandler`, `everyFrame`, table entries +168/+184 and the jump tables). The differences per class are
  in the notes inside each source.

## Functions

| function | image address | image bytes | authored object bytes | paired scenarios |
|---|---|---|---|---|
| `clone__12UndeadKnight` | 0x120358 | 1588 | 880 | 7 |
| `clone__5Torch` | 0x122ea8 | 440 | 340 | 13 |
| `clone__7Vampire` | 0x120990 | 1348 | 776 | 7 |
| `msg_collision__5TorchP10GameObject` | 0x22fee8 | 328 | 244 | 48 |
| `msg_draw__12UndeadKnight` | 0x23c638 | 352 | 352 | 64 |
| `msg_draw__17UndeadKnightClone` | 0x23db88 | 1380 | 928 | 150 |
| `msg_draw__5Torch` | 0x230030 | 372 | 384 | 48 |
| `msg_draw__7Ulthork` | 0x1f5358 | 272 | 280 | 60 |
| `msg_draw__7Vampire` | 0x243600 | 40 | 20 | 12 |
| `_$_12UndeadKnight` | 0x316090 | 116 | 120 | 10 |
| `_$_17UndeadKnightClone` | 0x316158 | 116 | 120 | 10 |
| `_$_7Ulthork` | 0x314340 | 100 | 88 | 10 |
| `_$_7Vampire` | 0x30f8b0 | 100 | 88 | 10 |
| `msg_hurt__12UndeadKnightfP10GameObjecti` | 0x23f990 | 104 | 116 | 27 |
| `msg_load__5TorchPc` | 0x231020 | 72 | 64 | 16 |
| `msg_load__7UlthorkPc` | 0x1f6240 | 168 | 168 | 48 |
| `msg_run__12UndeadKnight` | 0x23c798 | 2448 | 2292 | 1500 |
| `msg_run__17UndeadKnightClone` | 0x23e0f0 | 4992 | 2184 | 960 |
| `msg_run__5Torch` | 0x22f700 | 2024 | 1828 | 240 |
| `msg_run__7Ulthork` | 0x1f5468 | 2088 | 2148 | 1800 |
| `msg_run__7Vampire` | 0x23fca0 | 1392 | 1392 | 2400 |
| `msg_save__5TorchPc` | 0x231008 | 24 | 16 | 16 |
| `msg_save__7UlthorkPc` | 0x1f61e8 | 84 | 64 | 24 |
| `setState__12UndeadKnighti` | 0x23d128 | 1648 | 1756 | 1200 |
| `setState__17UndeadKnightClonei` | 0x23f470 | 824 | 972 | 600 |
| `setState__7Ulthorki` | 0x1f5c90 | 1368 | 1632 | 900 |
| `setState__7Vampirei` | 0x240210 | 1196 | 1400 | 690 |
| `__tf12UndeadKnight` | 0x316108 | 80 | 92 | 8 |
| `__tf17UndeadKnightClone` | 0x3161d0 | 80 | 92 | 8 |
| `__tf7Ulthork` | 0x3143a8 | 80 | 92 | 8 |
| `__tf7Vampire` | 0x30f918 | 80 | 92 | 8 |
| `msg_trigger__12UndeadKnighti` | 0x23f8f8 | 152 | 168 | 24 |

## Imports closed

`clone__5Torch`, `msg_collision__5TorchP10GameObject`, `msg_draw__5Torch`, `msg_load__5TorchPc`, `msg_run__5Torch`, `msg_save__5TorchPc`, `_$_7Ulthork`, `__tf7Ulthork`, `msg_draw__7Ulthork`, `msg_load__7UlthorkPc`, `msg_run__7Ulthork`, `msg_save__7UlthorkPc`, `setState__7Ulthorki`, `_$_12UndeadKnight`, `__tf12UndeadKnight`, `clone__12UndeadKnight`, `msg_draw__12UndeadKnight`, `msg_hurt__12UndeadKnightfP10GameObjecti`, `msg_run__12UndeadKnight`, `msg_trigger__12UndeadKnighti`, `setState__12UndeadKnighti`, `_$_17UndeadKnightClone`, `__tf17UndeadKnightClone`, `msg_draw__17UndeadKnightClone`, `msg_run__17UndeadKnightClone`, `setState__17UndeadKnightClonei`, `_$_7Vampire`, `__tf7Vampire`, `clone__7Vampire`, `msg_draw__7Vampire`, `msg_run__7Vampire`, `setState__7Vampirei`.

All 32 names were in `repo/work/plan10-build/unresolved-imports.json` (the wave start list). The callers are the authored w9-ctors sources.

## Imports opened

Symbols my sources reference but do not define (from `ld -r`-style `nm` of only my objects). Three groups.

Already unresolved before my work (not newly opened): `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `_$_5Light`, `__15AnimationState8RC15AnimationState8`, `asyncRandState`, `blockFree__FPv`, `bossMeterName`, `dropTreasure__8CreatureG6Point3iffffffPc`, `ef1Noise__Fi`, `fxRunCreatureFire__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStateffff`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `lightAdd__FP5Light`, `skills`.

Defined by an authored unit of the integration branch (`wt/00-integrator`, 2026-10-13) and so resolved there: `P_AddParticle__FP11ParticleDefP6Point3T1f`, `__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `__rtti_si`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animGlobalSync__FP14AnimationState`, `bExitStatePersueIdle__8Creature`, `blockAlloc__Fi`, `creatureAdvanceAnimation__8Creaturef`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `eRand__10GameObject`, `everyFrame__8CreatureP12_modelHeader`, `getSmartRandomDestination__8Creaturei`, `icos__Fi`, `isin__Fi`, `listOnewayLink__FP8ListHeadT0Pv`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`, `modelGetActiveChunks__FP12_modelHeaderi`, `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `modelResetOptionalParams__Fv`, `modelSetShadowScale__Ff`, `move2__8CreatureG6Point3`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `newDamageHandler__8CreatureP12_modelHeader`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`, `objectAddToRunList__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `objectUpdateInGrid__FP10GameObject`, `resetRouting__8Creature`, `setParameters__8Creaturefffffi`, `setStateProcessing__8CreaturePi`, `slideBack__8Creaturef`, `smartWander__8Creature`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `soundResetStream__Fi`, `soundStartMusic__FPci`.

Not defined in that plan and not in the wave-start unresolved list, so opened or only reachable from the private object: `ISeeDeadPeople__8Creaturef`, `_$_8Creature`, `__12ShockOfFrostG6Point3siP10GameObjectP5Skill`, `__15FrostStormSpellG6Point3ifiP10GameObjectT1`, `__17UndeadKnightCloneG6Point3sPPc`, `__6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf`, `__8ListHeadRC8ListHead`, `__tf8Creature`, `__ti8Creature`, `_vt$10GameObject`, `_vt$12UndeadKnight`, `_vt$17UndeadKnightClone`, `_vt$5Torch`, `_vt$7Ulthork`, `_vt$7Vampire`, `_vt$8Creature`, `bossMeterCurrent`, `creatureFindLeadYaw__8CreatureP10GameObjectf`, `enemyInZone2__8Creatureffssfi`, `enemyInZone__8Creaturefs`, `findClearPath__8CreatureG6Point3scf`, `findEnemyLeadPosition__8CreaturefP6Point3`, `frameNm`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `haloTexture2`, `haloTexture5`, `levelName`, `lightUpdatePosition__FP5Light`, `listDestroy__FP8ListHead`, `lumpFindResource__FPcT0`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f`, `msg_hurt__8CreaturefP10GameObjecti`, `objectIsCrowded__8CreatureP10GameObjectffi`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `soundFadeStream__Fi`, `sprintf`, `steerAndRouteToEnemy__8Creature`, `strcmp`, `torch_msg_run_particle_7d7ba0`, `torch_msg_run_particle_7d7c40`, `torch_msg_run_particle_7d7c90`, `turnToFaceEnemy__8Creature`, `udkResourceMap`, `undead_knight_clone_msg_run_particle_7d7e80`, `undead_knight_clone_msg_run_particle_7d7ed0`, `undead_knight_msg_run_particle_7d7e30`, `worldDifficulty`.

Of these, the data names `torch_msg_run_particle_7d7ba0`, `torch_msg_run_particle_7d7c40`, `torch_msg_run_particle_7d7c90`, `undead_knight_msg_run_particle_7d7e30`, `undead_knight_clone_msg_run_particle_7d7e80` and `undead_knight_clone_msg_run_particle_7d7ed0` are file-local particle definitions of the original units, named here by address (same convention as `ant_msg_run_particle_7d7170` in w10-actor-a). The authored static initializers (`w8-spell-static-init-a-torch` for the Torch definitions, `w8-fx-particles-init-udk` for the UndeadKnight ones at 0x7d7e30, 0x7d7e80 and 0x7d7ed0) keep their objects file-local (anonymous namespace or `static`), so the integrator has to export those objects under these names (or define these names as aliases) before the msg_run objects can link.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values after the current tip. `header_closure` is the shared `sources/w10-actor-o-common/access.hpp` where listed, `profile` is `authored_cpp`, and no `host_check.cpp` is an input (each includes host headers). Body sizes are from the reviewed EE g++ `authored_cpp` profile, not from the original executable. No `private_object_sha256` is claimed. Each unit emits the symbol it replaces (the destructors and type-info functions are weak in the image; here they are strong) plus, in two units, the local helper symbols the compiler emits, which are listed so the plan matches the object.

```json
{"emitted_functions":[{"compiled_body_bytes":880,"name":"clone__12UndeadKnight"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"bfbdb2d0dd3643e9a24ac346637635234afdf209fde300c97825beb5c6b4950b","source":"sources/w10-actor-o-clone-12undeadknight/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":340,"name":"clone__5Torch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"a27f37fac0c8763977c86810a732b7d2ffcdb4e2e1397748c7d5dc58211d58d9","source":"sources/w10-actor-o-clone-5torch/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":776,"name":"clone__7Vampire"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"8a6ead87b6c507eabd13c3f3f9c9e5ef096c5160b0cf38ced3a3ef05021b97ff","source":"sources/w10-actor-o-clone-7vampire/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":244,"name":"msg_collision__5TorchP10GameObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_collision.o","profile":"authored_cpp","sha256":"87bee1006d70c4dd8c7db87fb82635b12021260715f700470b4dcc8d80b3732f","source":"sources/w10-actor-o-collision-5torch/msg_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":352,"name":"msg_draw__12UndeadKnight"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"07c2c72e3c178036de6bf83826ddbfa470396b1abf44f8398df58f87af4aa8f2","source":"sources/w10-actor-o-draw-12undeadknight/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":928,"name":"msg_draw__17UndeadKnightClone"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"84b1c508c4987c5a8f5ddc8108c3c4b63e11cdb36a4c06a963ed57908cb8f933","source":"sources/w10-actor-o-draw-17undeadknightclone/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":384,"name":"msg_draw__5Torch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"203901ce9e8e718d330a61c908e7d169ef025bc933f0d6e569497ff6dbf90057","source":"sources/w10-actor-o-draw-5torch/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":280,"name":"msg_draw__7Ulthork"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"ec7a8226c2b93a54e7e13b523aee2a2ecd4f7cfb46ea41a001f4c186ca2fdc2e","source":"sources/w10-actor-o-draw-7ulthork/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"msg_draw__7Vampire"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"050f1ab2ae337a580dccb7caa010928193ae4f560650c90fb6519fdf09e115ee","source":"sources/w10-actor-o-draw-7vampire/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_$_12UndeadKnight"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"6b4002b79ce30d85011a43b62bca89daf0d1b35e517f4da67fdd4cbfa3d41d8b","source":"sources/w10-actor-o-dtor-12undeadknight/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_$_17UndeadKnightClone"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"5fac11e5aa1d065c21bf67e55a5d2ed6c04d44e7f1152b74e5182464285e16c1","source":"sources/w10-actor-o-dtor-17undeadknightclone/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_7Ulthork"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"b50d919f8cd70423c61a930dfde837edb25381feab930328e2824ea2f2893120","source":"sources/w10-actor-o-dtor-7ulthork/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_7Vampire"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"945c1f27eaa26184cba6b84e990dc127f224556377866e3af468060fdf5f7a51","source":"sources/w10-actor-o-dtor-7vampire/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":116,"name":"msg_hurt__12UndeadKnightfP10GameObjecti"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_hurt.o","profile":"authored_cpp","sha256":"46e93f27a15943ee3d33e41cb4f9e9dbc781daa95663b3ec35e358510b416d1e","source":"sources/w10-actor-o-hurt-12undeadknight/msg_hurt.cpp"}
{"emitted_functions":[{"compiled_body_bytes":64,"name":"msg_load__5TorchPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"a5c9668b7640e8d0126fa0422d690f6aba7e9c815714d0b33645ae7321ebd12d","source":"sources/w10-actor-o-load-5torch/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":168,"name":"msg_load__7UlthorkPc"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"25a67980da96db73fb1fc8f88098a1ee861abef90e5148d6a471cdd5fe652b5a","source":"sources/w10-actor-o-load-7ulthork/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2292,"name":"msg_run__12UndeadKnight"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"57101223bab352888f9a41e4e45c5d7d7c61eb07c9c3c59aef298686dc6ba442","source":"sources/w10-actor-o-run-12undeadknight/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":860,"name":"_ZN12_GLOBAL__N_1L5burstEPhPvfjjj.constprop.0"},{"compiled_body_bytes":656,"name":"_ZN12_GLOBAL__N_1L7scatterEPhiPvfb"},{"compiled_body_bytes":2184,"name":"msg_run__17UndeadKnightClone"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"23296e755eaf045fe185de74228381c9d23bb4b63049ab66af1de492ed33e7ef","source":"sources/w10-actor-o-run-17undeadknightclone/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1828,"name":"msg_run__5Torch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"bc990c4c5d3ab72f75e2834fab26fdd5dfbbe41dcd5e7e26fc1188f5278e3b89","source":"sources/w10-actor-o-run-5torch/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":28,"name":"_ZN4w10o10call_entryEPhjm.isra.0"},{"compiled_body_bytes":2148,"name":"msg_run__7Ulthork"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"10da832c75ed4ea8cfc6800c11bab2cfcc4dcd555e0b5a3d81dc2ed86a670558","source":"sources/w10-actor-o-run-7ulthork/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1392,"name":"msg_run__7Vampire"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"10be9ef2cc153ec6afb837bf553880d02b86ef67662142bcdf4203e2ec154614","source":"sources/w10-actor-o-run-7vampire/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"msg_save__5TorchPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_save.o","profile":"authored_cpp","sha256":"a487501405b6d5523625f7a894c0334267165ec88acdc28100e23e442fb99fe7","source":"sources/w10-actor-o-save-5torch/msg_save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":64,"name":"msg_save__7UlthorkPc"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_save.o","profile":"authored_cpp","sha256":"265874a27168cc1ad33e118295f335109528d34086fa65bfa8b1cd05b17ee36e","source":"sources/w10-actor-o-save-7ulthork/msg_save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1756,"name":"setState__12UndeadKnighti"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"2f85ef2b77722b8596aedddb1321edbc2f16f2bd1489ce8a7fa11fe25c72243e","source":"sources/w10-actor-o-set-state-12undeadknight/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":972,"name":"setState__17UndeadKnightClonei"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"0db387e02ea1d5d657aff505b01698a01fca5125e84a704666b9540b9f4111c1","source":"sources/w10-actor-o-set-state-17undeadknightclone/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1632,"name":"setState__7Ulthorki"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"56e2fb404fa9f7c99a7d9aee631ff206356bed2f728ed634ec08af53627ac39f","source":"sources/w10-actor-o-set-state-7ulthork/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1400,"name":"setState__7Vampirei"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"2966b7f2b1a98996ee285f33be5a3a427a03811a5ef1be839e577a7b7df9bd87","source":"sources/w10-actor-o-set-state-7vampire/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf12UndeadKnight"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"09d7348715c67b5cb86c5fa872053467291a53c428b09fb383ef31c319a3ebd3","source":"sources/w10-actor-o-tf-12undeadknight/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf17UndeadKnightClone"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"06432a0ed31eae8ae2c3eb0d61cfa8fd59b8df2a2ed22733f4f37cdd3199db49","source":"sources/w10-actor-o-tf-17undeadknightclone/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf7Ulthork"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"18ed1526221e9685ebef7a511bc18a5a4a614953c78c1b70b319e0020c0b6e52","source":"sources/w10-actor-o-tf-7ulthork/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf7Vampire"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"f651130f25a7985f9a7a6f21ec15c78548bb6f79a48a0efcda2646033cee3512","source":"sources/w10-actor-o-tf-7vampire/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":168,"name":"msg_trigger__12UndeadKnighti"}],"header_closure":["sources/w10-actor-o-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_trigger.o","profile":"authored_cpp","sha256":"ce23b3c7e34d5d36125c780de31c52802715dfeeb3da130aabc703700e014a98","source":"sources/w10-actor-o-trigger-12undeadknight/msg_trigger.cpp"}
```

## Authored but never executed

All of it. Host contracts ran on this Mac, and the paired comparisons ran inside a private interpreter with stubbed
callees, not on the EE and not in the game. None of the callees (the Creature base class methods, the model and
attachment queries, the particle and sound calls, the spell constructors, the skill table entries) ran together with
these functions. Float behaviour was compared under the interpreter's model (round to nearest); the EE's truncating
arithmetic, denormals and NaN corners were not sampled for results, only for control flow. The three data views the
particle code needs are not defined (see Imports opened).

## Unknown

- What the states of each class mean (the numbers are the jump-table indexes; only their effects are recorded), and what
  most object fields at +28, +256, +294, +576, +804, +808, +960, +964, +1104 and the spell blocks beyond +2496 hold.
- Whether the `Light` at +76 (Torch) and +2496 (UndeadKnightClone) is the engine's `Light` (inferred from the destructor
  call and `lightAdd` / `lightUpdatePosition`).
- Whether the stack-residue copy in UndeadKnight `msg_draw` is a defect of the original, and what value it carries in game.
- The meaning of the description halfwords written in Ulthork state 13, of the constants `0x40008`, `0x80008`, `0x100c040`,
  `16641`, and of the .lit4 float constants (they are named by value only).
- The return types of virtuals the image ignores (the Vampire `msg_draw` leaves the callee's return value in `v0`).
- Torch `msg_collision`: the image builds an item description on the stack, writes only the bytes listed in the source
  and leaves the rest uninitialised; the authored object reserves 72 bytes and writes the same bytes.
