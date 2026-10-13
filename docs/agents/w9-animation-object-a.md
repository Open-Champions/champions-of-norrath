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
