# Animation, object lists and creature AI (a) (w9-animation-object-a)

Branch `agents/w9-animation-object-a`. New source is under `experimental/sources/w9-animation-object-a-*`; shared helpers are in `w9-animation-object-a-common`. Reference executable: the private `SLUS_205.65` named in the docs, read locally only. No game bytes, listings or decompiler output are committed; addresses and extents are that file's symbol table.

Every function compiles with the reviewed `authored_cpp` profile and has a `host_check.cpp` (host compiler, exits non-zero on a wrong store, result or branch; not a plan input). Each was also run against the original on a private R5900 interpreter harness kept outside git: original and authored function ran from identical memory snapshots and the return value, call log and every changed byte outside the stack were compared. That harness run is not part of the repository and no byte match of compiled code is claimed.

## Functions

| source directory (`experimental/sources/w9-animation-object-a-…`) | emitted function | image address | image extent |
|---|---|---|---|
| `erand` | `eRand__10GameObject` | 0x153fa8 | 40 |
| `b-exit-state-persue-idle` | `bExitStatePersueIdle__8Creature` | 0x112e08 | 12 |
| `b-far-from-home` | `bFarFromHome__8Creature` | 0x112da8 | 88 |
| `anim-find-component-with-flags` | `animFindComponentWithFlags__FP14AnimationStatei` | 0x164130 | 76 |
| `anim-add-to-end` | `animAddToEnd__FP14AnimationStateP15AnimationHeaderi` | 0x163930 | 148 |
| `anim-add-hairy` | `animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi` | 0x1638b8 | 120 |
| `anim-add-one-shot` | `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi` | 0x160010 | 280 |
| `anim-global-sync` | `animGlobalSync__FP14AnimationState` | 0x1639e8 | 248 |
| `anim-advance-animation` | `animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps` | 0x1639c8 | 28 |
| `game-line-init-path-state` | `gameLineInitPathState__FP17gameLinePathStatePcP6Point3` | 0x279b58 | 88 |
| `find-tag-float` | `findTagFloat__10GameObjectPci` | 0x1410a0 | 328 |
| `get-smart-random-destination` | `getSmartRandomDestination__8Creaturei` | 0x10d250 | 464 |
| `anim-map-animation-sounds` | `animMapAnimationSounds__FP15AnimationHeader` | 0x15fd98 | 624 |
| `anim-state-data` | data: `animState` (264 bytes), `animStatePtr`, `animUseBuffer`, `animationLoading`, `animationLoadPos` | 0x7ccad8, 0x486aa4, 0x48501f, 0x484358, 0x486a08 | 264, 4, 1, 1, 4 |
| `lump-data` | data: `domeLump`, `dialogLumpName`, `eqLumpHeader` | 0x486b00, 0x48500c, 0x4845a0 | 4 each |
| `game-line-data` | data: `gameLineCount`, `gameLines` (100 pointers) | 0x485ea8, 0x8d57a8 | 4, 400 |
| `in-msg-data` | data: `inMsg` | 0x8d8938 | 1008 |
| `has-light-or-particle-number` | `hasLightOrParticleNumber__FPPci` | 0x21df18 | 828 |
