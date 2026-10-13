# Spell, skill and actor virtuals (SeaMonster to Soul)

Package `w10-actor-m`, branch `agents/w10-actor-m`, based on `cc78a45e`. Addresses, sizes and symbol names are from the private
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`; `_gp` is `0x004879f0`). No game bytes,
disassembly listings or tool output are committed. Compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a claim of
byte equality with the image. Nothing here has run on the EE or inside the game.

## What was done

The package brief lists 32 unresolved imports of the SeaMonster, Shooter, Skeleton, Skelight and Soul actor classes. 31 now have a
body under `experimental/sources/w10-actor-m-*` (one commit per function, plus a few comment, host-contract and cleanup commits); the
32nd, `msg_save__7ShooterPc`, was already authored by `w8-actor-virtuals-c-shooter-save` (`save.cpp`) but that unit is not in the shared
plan, so it stays an import of the shared build until the integrator selects it (the plan-entries script below includes it as an
extra unit). One more function, `demonNear__4Soul`, was authored because `msg_run__4Soul` calls it and nothing else defines it.

`setState__8Skeletoni` and `setState__8Skelighti` are thin definitions under the table symbol name that pass both arguments to the
already authored bodies `norrath_skeleton_set_state` and `norrath_skelight_set_state` (the same approach as the w8 `setPassableState`
units). Everything else is a new body.

## How each body was checked

Every unit has a `host_check.cpp` (not a plan input) that exits non-zero on a wrong branch, offset, constant, call order or
argument; callees are recorded stubs and words that hold addresses are offsets from a host arena (`W10_ACTOR_M_HOST`).

Every function was also compared against the image (for the `__tf` getters the type-info storage is bound to the image's own object) in a private paired-run harness (an R5900
interpreter outside the repository; it loads the private executable and the reviewed toolchain, nothing from it is stored). The
harness compiles the authored unit with the plan's `authored_cpp` flags, links it at a scratch address with every undefined symbol
bound to the image's address, then runs the original function and the authored function from identical memory snapshots with
identical Python stubs for every callee and table slot. Callee logs (integer and float argument registers, the bytes behind pointer
arguments and strings, stack arguments, a hash of the object memory at call time) and every byte written outside the stack must be
equal. The interpreter fixes noted in earlier packages (`sqrt.s`/`rsqrt.s`, halts counted as failures) are in my private copy; every
run was checked for `HALT` and none occurred. Scenario counts are in each commit message; the large state machines used 400 to 9000
scenarios with the states weighted toward the branchy ones, and instruction coverage of the original was measured: the words never
executed were alignment nops and the delay slots of branch-likely instructions (see the commit messages for the counts).

For every function more than trivial I changed constants, offsets, branch senses, bounds, strings and argument order in the source and
required the paired run or the host contract to fail. Mutations I could not detect: one-ulp changes to float constants that only
reach a truncating conversion (`0x43fa0000` in the Skeleton multi-shot spread), an exact-equality boundary (`<=` against `<` on a float
distance, and `z < ground`) that the random scenarios never hit exactly, and a few that only change an unreachable table slot.

## Method notes that mattered

- `V0` carried across a call: the image copies V0 into a saved register in the delay slot of the *following* call, so for
  `msg_run__8Skeleton`, `msg_run__8Skelight`, `msg_run__10SeaMonster` and `msg_run__4Soul` the value tested for bits 0 and 1 is the
  result of `creatureAdvanceAnimation__8Creaturef`, not of `slideBack__8Creaturef`. The paired runs fail if the wrong one is used.
- Type tests: `msg_hurt__8Skeleton` and `msg_hurt__4Soul` call slot 0 of the *source* object's table with the receiving object left
  in A0 and compare the result with a type-info getter (`__tf8Skeleton` / `__tf5Demon`). The Skeleton does nothing to a source of its own
  type; the Soul reacts only to a Demon.
- `partlist.1069` (0x7d7f70, 160 bytes) is a file-local scratch list filled by `dropItem__8Skeletoni`; it is a local static in
  that unit (the paired run binds it to the image symbol). `partlist.1103` (0x7d8010) is cleared by `dropItem__8Skelighti`; its
  definition is not authored and stays an import.
- The small-data float at `gp-11588` that the Skelight load borrows around `setParameters` is the image symbol `worldDifficulty`.
- Float expressions are written in the order of the image and the unit-ball velocity draws use the same rejection loop as the w9 Soul
  state code. `rsqrt.s` is spelled as inline assembly (like the w8 FireBomb unit); the host contracts fall back to `1/sqrt`.
- Host mode: a word that is zero must be tested as a word, not through a null check on the converted pointer (`from_word(0)` is the
  host arena base). Two host-contract failures traced to that were fixed in the sources (Soul run).

## Units

| symbol | image address | image bytes | compiled bytes | unit directory |
|---|---|---|---|---|
| `_$_4Soul` | 0x314278 | 116 | 120 | `w10-actor-m-dtor-4soul` |
| `_$_8Skeleton` | 0x3162e0 | 116 | 120 | `w10-actor-m-dtor-8skeleton` |
| `_$_8Skelight` | 0x3163a8 | 100 | 88 | `w10-actor-m-dtor-8skelight` |
| `__tf4Soul` | 0x3142f0 | 80 | 92 | `w10-actor-m-tf-4soul` |
| `__tf8Skeleton` | 0x316358 | 80 | 92 | `w10-actor-m-tf-8skeleton` |
| `__tf8Skelight` | 0x316410 | 80 | 92 | `w10-actor-m-tf-8skelight` |
| `clone__8Skeleton` | 0x11dab8 | 1612 | 992 | `w10-actor-m-clone-8skeleton` |
| `clone__8Skelight` | 0x11e108 | 1572 | 852 | `w10-actor-m-clone-8skelight` |
| `demonNear__4Soul` | 0x1f4d50 | 268 | 280 | `w10-actor-m-demon-near-4soul` |
| `dropItem__8Skeletoni` | 0x245938 | 984 | 1148 | `w10-actor-m-drop-item-8skeleton` |
| `dropItem__8Skelighti` | 0x2473f0 | 12 | 20 | `w10-actor-m-drop-item-8skelight` |
| `getModel__4Soul` | 0x1f5010 | 32 | 32 | `w10-actor-m-get-model-4soul` |
| `msg_alert__8SkeletonifG6Point3P10GameObject` | 0x247010 | 180 | 184 | `w10-actor-m-msg-alert-8skeleton` |
| `msg_draw__4Soul` | 0x1f3110 | 508 | 472 | `w10-actor-m-msg-draw-4soul` |
| `msg_draw__8Skeleton` | 0x2446f8 | 388 | 428 | `w10-actor-m-msg-draw-8skeleton` |
| `msg_draw__8Skelight` | 0x2468a0 | 304 | 328 | `w10-actor-m-msg-draw-8skelight` |
| `msg_hurt__4SoulfP10GameObjecti` | 0x1f4430 | 664 | 608 | `w10-actor-m-msg-hurt-4soul` |
| `msg_hurt__8SkeletonfP10GameObjecti` | 0x2470c8 | 148 | 148 | `w10-actor-m-msg-hurt-8skeleton` |
| `msg_load__4SoulPc` | 0x1f4fa8 | 104 | 100 | `w10-actor-m-msg-load-4soul` |
| `msg_load__8SkeletonPc` | 0x246f80 | 144 | 120 | `w10-actor-m-msg-load-8skeleton` |
| `msg_load__8SkelightPc` | 0x247310 | 220 | 200 | `w10-actor-m-msg-load-8skelight` |
| `msg_run__10SeaMonster` | 0x1f82a0 | 1232 | 1168 | `w10-actor-m-msg-run-10seamonster` |
| `msg_run__4Soul` | 0x1f3310 | 4384 | 4000 | `w10-actor-m-msg-run-4soul` |
| `msg_run__8Skeleton` | 0x244880 | 2924 | 3152 | `w10-actor-m-msg-run-8skeleton` |
| `msg_run__8Skelight` | 0x2469d0 | 800 | 784 | `w10-actor-m-msg-run-8skelight` |
| `msg_save__4SoulPc` | 0x1f4f40 | 104 | 100 | `w10-actor-m-msg-save-4soul` |
| `msg_save__7ShooterPc` | 0x1e5d80 | 24 | 36 | `w8-actor-virtuals-c-shooter-save` |
| `msg_save__8SkeletonPc` | 0x246f10 | 108 | 80 | `w10-actor-m-msg-save-8skeleton` |
| `msg_save__8SkelightPc` | 0x247288 | 132 | 100 | `w10-actor-m-msg-save-8skelight` |
| `msg_trigger__4Souli` | 0x1f4ee8 | 84 | 92 | `w10-actor-m-msg-trigger-4soul` |
| `setState__10SeaMonsteri` | 0x1f8770 | 1008 | 1240 | `w10-actor-m-set-state-10seamonster` |
| `setState__8Skeletoni` | 0x2453f0 | 1352 | 8 | `w10-actor-m-set-state-8skeleton` |
| `setState__8Skelighti` | 0x246cf0 | 544 | 8 | `w10-actor-m-set-state-8skelight` |

## Imports closed (names)

All 32 names of the brief resolve in a single partial build (see "Imports opened" for how it was measured):

- SeaMonster: `msg_run__10SeaMonster`, `setState__10SeaMonsteri`
- Shooter: `msg_save__7ShooterPc` (authored earlier as `w8-actor-virtuals-c-shooter-save/save.cpp`; it resolves only with that unit added to the plan)
- Skeleton: `_$_8Skeleton`, `__tf8Skeleton`, `clone__8Skeleton`, `dropItem__8Skeletoni`, `msg_alert__8SkeletonifG6Point3P10GameObject`,
  `msg_draw__8Skeleton`, `msg_hurt__8SkeletonfP10GameObjecti`, `msg_load__8SkeletonPc`, `msg_run__8Skeleton`, `msg_save__8SkeletonPc`,
  `setState__8Skeletoni`
- Skelight: `_$_8Skelight`, `__tf8Skelight`, `clone__8Skelight`, `dropItem__8Skelighti`, `msg_draw__8Skelight`, `msg_load__8SkelightPc`,
  `msg_run__8Skelight`, `msg_save__8SkelightPc`, `setState__8Skelighti`
- Soul: `_$_4Soul`, `__tf4Soul`, `getModel__4Soul`, `msg_draw__4Soul`, `msg_hurt__4SoulfP10GameObjecti`, `msg_load__4SoulPc`,
  `msg_run__4Soul`, `msg_save__4SoulPc`, `msg_trigger__4Souli`

Not in the brief but authored: `demonNear__4Soul` (called by `msg_run__4Soul`).

The three `__tf` units also define the 12-byte type-info objects `__ti8Skeleton`, `__ti8Skelight` and `__ti4Soul` (zero-initialised
storage, as the existing `__tf8SkulBoat` unit does). Nothing else defines them in the shared plan; if the integrator finds another
owner, drop the array line from the unit.

## Imports opened

Measured with a private plan copy (`docs/agents/w10-actor-m-plan-entries.py --private-root`) on top of the shared plan at `cc78a45e`
and one partial build (`tools/build_partial.py`, reviewed toolchain): 3509 inputs (33 added), 3507 compiled and linked once, 2517
function bodies, 1316 unresolved imports, COMMON zero. None of the 32 names is unresolved in that build. Sixteen unresolved names
are referenced only by this package's units, so the package opens them; the shared plan alone therefore has 1316 - 16 + 32 = 1332
unresolved (derived, not separately built).

Opened (16), all methods or constructors of other classes or library routines with no authored body, plus one data object:

- `__8ListHeadRC8ListHead` (ListHead copy constructor; the two clones)
- `enemyInZone__8Creaturefs`, `enemyInZone2__8Creatureffssfi`, `findEnemyLeadPosition__8CreaturefP6Point3`,
  `objectIsCrowded__8CreatureP10GameObjectffi`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`,
  `steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature` (Creature methods used by the run loops)
- `__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill`,
  `__19MultiFireProjectileG6Point3sP10GameObjectiP5Skill`, `__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill`
  (projectile constructors, `msg_run__8Skeleton`)
- `__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf` (PhProp constructor, `dropItem__8Skeletoni`)
- `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`,
  `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii` (both draw routines)
- `partlist.1103` (data at 0x7d8010 cleared by `dropItem__8Skelighti`; its owner is not authored)

Also used and already unresolved from other units, so not new (13): `__15AnimationState8RC15AnimationState8`, `__tf5Demon`,
`addEffect__FiG6Point3`, `blockFree__FPv`, `dropTreasure__8CreatureG6Point3iffffffPc`,
`gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `getSoulAttachMatrix__6MinionP8Matrix34`,
`matInvert__FP8Matrix34T0`, `matTransform__FP6Point3P8Matrix34T0`,
`modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f`, and the data objects `skeletonResMap`, `skills` and
`soulParticle` (named as the image's symbols; no data view of them is in the plan yet). The other callees and data the units name
(`msg_*__8Creature`, `setStateProcessing__8CreaturePi`, `eRand__10GameObject`, `listDestroy__FP8ListHead`, `_$_8Creature`,
`sprintf`, `lumpFindResource__FPcT0`, the matrix, model, particle and sound routines, `frameNm`, `nmPlayers`, `player`, `world`,
`worldDifficulty`, `_4item$globalItem`, and the Creature, GameObject and class table views) resolve in that build.

## Build-plan entries needed

Thirty-three entries (the 32 `w10-actor-m-*` units plus the `w8-actor-virtuals-c-shooter-save` unit), printed in full with their
SHA-256 and emitted functions by `docs/agents/w10-actor-m-plan-entries.py --toolchain /path/to/ee` (the script compiles each unit with
the reviewed toolchain; `--private-root DIR` also writes a private plan/snapshot copy outside the repository). Profile
`authored_cpp` for every unit. Header closures: `["sources/w10-actor-m-common/access.hpp"]` for every unit whose source includes
it (21 units; the eleven that do not are the three `__tf`, three `_$_` destructor, two `clone`, `dropItem__8Skelighti` and the two
`setState` wrapper units, which include nothing), and `["sources/w8-actor-virtuals-c-bytes/bytes.hpp"]` for the Shooter unit. `host_check.cpp` files are not inputs.
`snapshot.json` still rejects the unlisted source files until the integrator merges the entries.

## Authored but never executed

Every body was only run in the private interpreter against the image (the paired runs above) and as host contracts. None has run on the
EE, in a game process or with real callees. The two table-wrapper definitions (`setState__8Skeletoni`, `setState__8Skelighti`) were
checked together with the w9 bodies they call. The table slots, strings, data views and float constants are checked by address and
value only. `msg_save__7ShooterPc` is the w8 unit's body, reviewed here only by confirming it is the missing definition.

## Unknown

- The meaning of nearly every field and state number: the words at +208, +256, +272, +576, +804, +808, +960, +964, +968, +972, +1120
  and +1124, the halfwords at +200..+204, +294 and +572, the byte at +569/+570, and the states of the three classes; no names are
  proposed.
- What `skeletonResMap` holds beyond the 8-byte word pairs and the 104-byte records used by the w9 state code; what the Skill object's
  table entry at byte 96 computes (it returns the multi-shot count); what the two particle suppressions do.
- The small-data float constants (0.2, 0.3, 0.4, 0.7 and 1/240 among them) were taken by value from the image; no unit names them.
- `partlist.1103` size and layout; whether `partlist.1069` is also used elsewhere in the image (the references found are in
  `dropItem__8Skeletoni`).
- Every callee with no authored body stays an unresolved import; their argument types come from the mangled names and the callers'
  register use only. In particular the three-vector `Point3` parameters are passed by reference exactly as the image does.
- Exact bit patterns for out-of-range float-to-int conversions and the floating-point environment: scenario floats are small and exactly
  representable.
- `msg_run__4Soul` loop control that the image encodes redundantly (a second zero test of the shadow count inside the loops) is not
  reproduced; it cannot change behaviour.
