# Animation, object lists and creature AI (c) (w9-animation-object-c)

Branch `agents/w9-animation-object-c`. New source is under `experimental/sources/w9-animation-object-c-*`; shared helpers are in `w9-animation-object-c-common`. Reference executable: the private `SLUS_205.65` named in the docs, read locally only. No game bytes, listings or decompiler output are committed; addresses and extents are that file's symbol table.

Every function compiles with the reviewed `authored_cpp` profile and has a `host_check.cpp` (host compiler, exits non-zero on a wrong store, result or branch; not a plan input). Each was also run against the original on a private R5900 interpreter harness kept outside git: original and authored function ran from identical memory snapshots and the return value, call log and every changed byte outside the stack were compared. That harness run is not part of the repository and no byte match of compiled code is claimed.

Records are addressed by byte offsets through `mem.hpp` because no original class declaration is claimed; field roles are inferred from use and are not recovered names.

## Already closed before this package

Checked against the tree at the start: `objectFindAnimation__FP10GameObjectPc`, `objectFindObjectByClass__FPcPP10GameObjecti`, `objectInside__7TriggerP10GameObject`, `objectRemoveFromGrid__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `objectSaveAddToStringPool__FPCc`, `objectSaveGetFromStringPool__Fi`, `objectSetCreateByNameFunction__FPFPcG6Point3sPPc_P10GameObject`, `objectSetFilterFunction__FPFPc_i` are in the build plan (w8-object-list-math); `runList` and `slowRunList` are defined by `w8-object-list-math-run-list-heads`; `saveActivate__9Savepoint` has `w8-actor-virtuals-c-save-activate`. Those are not repeated here.

## Functions

| source directory (`experimental/sources/w9-animation-object-c-…`) | emitted symbol | image address | image extent |
|---|---|---|---|
| `reset-routing` | `resetRouting__8Creature` | 0x113460 | 40 |
| `set-anim-event-flags` | `setAnimEventFlags__8Creatureififififi` | 0x1128a8 | 88 |
| `set-state-processing` | `setStateProcessing__8CreaturePi` | 0x1129f0 | 108 |
| `object-is-off-grid` | `objectIsOffGrid__FP10GameObject` | 0x1435a8 | 132 |
| `iterate-run-lists` | `objectIterateRunLists__FPFP10GameObject_v` | 0x13ede8 | 324 |
| `smart-wander` | `smartWander__8Creature` | 0x113128 | 212 |
| `object-vis-data` | data: `objectVisPoints` (48 bytes), `objectVisCount` (1), `objectVisUseCamCenter` (1, initial 1) | 0x333d28, 0x483afe, 0x483ac9 | 48, 1, 1 |
| `out-msg-data` | data: `outMsg` | 0x8d88b8 | 128 |
| `w-lump-header-data` | data: `wLumpHeader` | 0x4845a4 | 4 |
| `radial-damage` | `objectRadialDamage__FP10GameObjectG6Point3iffiii` | 0x13f708 | 548 |
| `find-empty-spot` | `objectFindEmptySpot__FG6Point3iiPc` | 0x13f2d0 | 1076 |
| `slide-back` | `slideBack__8Creaturef` | 0x111490 | 920 |
| `steer-and-route` | `steerAndRouteToPosition__8CreatureG6Point3` | 0x1102b8 | 1572 |
| `spawn` | `spawn__FG6Point3iisPcbPPci` | 0x26a738 | 840 |
| `update-animation` | `updateAnimation__FP13animComponent` | 0x161528 | 2248 |
