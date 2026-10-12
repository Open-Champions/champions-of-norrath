# Object grid and particle step (loop-grid-fx)

Branch `agents/loop-grid-fx`. This package took over from an earlier agent that was cut off
after one uncommitted file (`objectFindObject__FPcPP10GameObjecti`, reviewed against the
disassembly, its weak host assertions fixed, then committed). Private reference:
`work/private/SLUS_205.65`; addresses and extents below are that file's symbol table and
reviewed EE decoder, used only locally. No game bytes or listings are committed. Fresh EE
objects are not original-byte matches. Nothing here has run on the EE.

Every function compiles with the reviewed `authored_cpp` flags and has a `host_check.cpp`
beside it that returns non-zero when a store, call order, bound or result is wrong. Host
checks are not plan inputs. Sources that read pointer-valued words compile with
`-DNORRATH_HOST_CONTRACT` for the host check only: that adds an `norrath_host_pointer`
hook so 32-bit address words map into a static arena. The EE build never defines it.
A private plan (copy of the integrator plan with these 21 inputs, snapshot regenerated
outside git) linked: 785 function bodies (765 before), 940 unresolved (950 before).

## Functions

| source dir | exported name | image | compiled bytes |
|---|---|---|---|
| `loop-grid-fx-find-list` | `objectFindObject__FPcPP10GameObjecti` | `0x140678`, 352 | 444 |
| `loop-grid-fx-find-first` | `objectFindObject__FPc` | `0x143a58`, 36 | 36 |
| `loop-grid-fx-find-tag-string` | `findTagString__10GameObjectPci` | `0x143a80`, 304 | 360 |
| `loop-grid-fx-find-in-box` | `objectFindInBox__FG6Point3T0PP10GameObjecti` | `0x13ef30`, 548 | 820 |
| `loop-grid-fx-update-in-grid` | `objectUpdateInGrid__FP10GameObject` | `0x143630`, 240 | 468 |
| `loop-grid-fx-iterate-grid` | `objectIterateGrid__FPFP10GameObject_v` | `0x13ec40`, 424 | 212 (+124 helper) |
| `loop-grid-fx-remove-string` | `norrath_object_remove_string` | `objectRemoveString__FPcP10GameObject` `0x140580`, 244 | 332 |
| `loop-grid-fx-delete-list` | `norrath_object_add_to_delete_list` | `objectAddToDeleteList__FP10GameObject` `0x1437e8`, 68 | 84 |
| `loop-grid-fx-medium-run-list` | `norrath_object_add_to_medium_run_list` | `objectAddToMediumRunList__FP10GameObject` `0x143908`, 100 | 216 |
| `loop-grid-fx-list-head-copy` | `norrath_list_head_copy_construct` | `__8ListHeadRC8ListHead` `0x194498`, 8 | 8 |
| `loop-grid-fx-light-add` | `norrath_light_add` | `lightAdd__FP5Light` `0x13d230`, 200 | 456 |
| `loop-grid-fx-has-number` | `norrath_has_light_or_particle_number` | `hasLightOrParticleNumber__FPPci` `0x21df18`, 828 | 144 (+292 helper) |
| `loop-grid-fx-p-clear` | `P_Clear__Fv` | `0x180588`, 660 | 468 (+252 helpers) |
| `loop-grid-fx-add-particle` | `P_AddParticle__FP11ParticleDefP6Point3T1f` | `0x180da0`, 732 | 1156 |
| `loop-grid-fx-update-particle-type` | `P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v` | `0x180ac0`, 580 | 252 |
| `loop-grid-fx-p-update` | `P_Update__Fv` | `0x1812e0`, 336 | 360 |

Where an exported name starts with `norrath_`, the importers (actors-a, gameobject-base,
light-methods, render-lights, factory-tag-queries) use that name and the plan rejects
aliases, so the routine is published under the importer's name; the ELF name is in the
source comment.

## Data

| source dir | name | address | extent |
|---|---|---|---|
| `loop-grid-fx-emitter-tag` | `norrath_factory_particle_emitter_tag_bytes` | `0x470cb8` (.rodata) | 16, the text `ParticleEmitter` and NUL, writable |
| `loop-grid-fx-script-particles` | `scriptParticles`, `scriptParticle0..2` | `0x333920`, `0x4f3f20`, `0x4f3f70`, `0x4f3fc0` | 16 (three pointers, zero word), 80 each |
| `loop-grid-fx-fx-dust` | `fxDust` | `0x42ad70` | 80, file bytes zero |
| `loop-grid-fx-task-list` | `taskList` | `0x793e60` | 512 (distance to `colorStack`, symbol size 0) |
| `loop-grid-fx-particle-storage` | `pushLevel`, `currentPS`, `freeBlocks`, `globalSystem`, `blockList` | `0x484304`, `0x48430c`, `0x4869f0`, `0x37f730`, `0x780080` | 4, 4 (initial value `&globalSystem`), 4, 256, 512 |

## Observed

- Grid cell lists: heads are words; a node has next at 0, previous-link at 4, partner at 8
  and value at 12 (from `listOnewayLink` at `0x194390`). An object's word at +8 is its own
  node, whose +8 is the cell node. `objectGrid`/`ghostObjectGrid` are 120 x 120 head words,
  cells 256 wide with origin offset 15360 (`objectUpdateInGrid`) and 15264/15456 for box
  corners (`objectFindInBox`). `lightGrid` (`0x4f40f0`) is 90 x 90, offset 11520, and a
  light that falls outside goes to the head at `0x486848`, a different object from the
  constructor default head at `0x486890`; both are named `offTheGrid` in the executable.
  `loop-grid-fx-light-add` defines the first as `norrath_light_off_grid_head_view`.
- Name hash (objectFindObject, objectRemoveString): ctype bit 0 folds by adding 32, a
  one-at-a-time mix, 512 slots. The object word table is the one cleared on removal; the
  name pointer table is left alone.
- `objectUpdateInGrid` does not look at flag bit 2; bit `0x800` selects the ghost grid.
- Particle blocks: an entry's word +12 is the next record index in its last block, where
  257 means full. Indexes skip from 30 to 33 (and 254 to 257).
- `P_Update` calls `updatePS` on `globalSystem` and on each used `PS` record; a record that
  reports no live particles for 32 consecutive updates is passed to `P_DeleteParticleSystem`.

## Inferred

- Field roles in the particle definition: +0 type index, +4 descriptor slot, +8 use count,
  +24 a fall term `g`. These come from how `P_AddParticle` and `P_UpdateParticleType` use
  them; no names were guessed beyond that.
- The 512-byte `taskList` extent is the gap to the next label, an upper bound, not a count.
- `P_UpdateParticleType`'s alignment-safe doubleword copy follows the original's `ldl/ldr`
  forms (definitions may be only word aligned).

## Imports closed

`P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_Clear__Fv`,
`P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `P_Update__Fv`,
`findTagString__10GameObjectPci`, `fxDust`, `norrath_factory_particle_emitter_tag_bytes`,
`norrath_has_light_or_particle_number`, `norrath_light_add`,
`norrath_list_head_copy_construct`, `norrath_object_add_to_delete_list`,
`norrath_object_add_to_medium_run_list`, `norrath_object_remove_string`,
`objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectFindObject__FPc`,
`objectFindObject__FPcPP10GameObjecti`, `objectIterateGrid__FPFP10GameObject_v`,
`objectUpdateInGrid__FP10GameObject`, `scriptParticles`, `taskList`.

## Duplicates for the integrator to resolve

- `objectUpdateInGrid__FP10GameObject` (this package) and `norrath_object_update_in_grid`
  (`game-player-grid-build-v2/update_grid.cpp`) are the same routine; natives and others call
  the first name. Select one source. Same for `objectFindInBox__FG6Point3T0PP10GameObjecti`
  and `norrath_object_find_in_box` (`find_box.cpp`), and for
  `norrath_object_add_to_medium_run_list` here and `norrath_actor_add_medium_run_list`
  (vehicle-actors). The medium run list source here references vehicle-actors'
  `norrath_medium_run_list_heads` and `norrath_medium_list_cycle_word`.
- Names still unresolved after this package, all defined by `origin/agents/particles`
  and not present in this branch's base: `P_FrameNum`, `block`, `freeNodes`, `nodes`,
  `objectGrid`, `ghostObjectGrid`, `objectHash`, `stringHash`, `objectStrings`,
  `offTheGrid`. They were not defined again here. My sources reference them by those names,
  plus `PS`, `descList`, `descListMark`, `nmSingle`, `nmLines`, `objectDrawEnable`,
  `particlesPaused`, `cutMode`. The object-infra views for the grid, hash, string-count and
  free-node words are separate objects at different addresses until the pair decision in
  INTEGRATION.md is made.
- `norrath_grid_ignore_flags_view` (world-state) is used for the ignore mask at `0x483ac0`
  instead of the unresolved `norrath_restore_broadcast_mask_word`.

## Imports opened

`PS`, `descList` (particles branch), `P_DeleteParticleSystem__Fi`, `P_Pop__Fv`,
`checkCanAllocDescriptor__Fv`, `frameNumWrapped__Fv`, `setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v`,
`updatePS__FP14ParticleSystem`, `updateTrails__Fv`. Not authored here: they are outside the
listed 30 names. Images: `P_Pop` `0x180580` (4-byte stub), `setParticleDef` `0x180820`,
`updatePS` `0x181080`, `frameNumWrapped` `0x181550` (calls local `frameNumWrappedPS` `0x181438`),
`updateTrails` `0x1840c0`, `P_DeleteParticleSystem` `0x180468`, `checkCanAllocDescriptor` `0x187268`.
Also used and already defined elsewhere: `norrath_list_remove`, `norrath_list_oneway_link`,
`norrath_list_safe_iterator_*`, `norrath_memory_compare`, `norrath_factory_strlen`,
`norrath_factory_strncmp`, `norrath_factory_strcasecmp`, `norrath_factory_atoi`,
`norrath_factory_sprintf`, `norrath_factory_ctype_minimum_view`,
`norrath_floor_light_grid_view`, `norrath_restore_broadcast_head_view`,
`allocDescriptor__FP11ParticleDef`, `gameIsPaused__Fv`.

## Build-plan entries needed

Append in this order (21 authored inputs, empty `header_closure`, profile `authored_cpp`).
The integrator assigns `input_order` and regenerates the snapshot.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "find_list.o",
  "profile": "authored_cpp",
  "sha256": "730618cd0a530c44d2d1233d7fab52d430e4b3808babb513ec042fa894045f51",
  "source": "sources/loop-grid-fx-find-list/find_list.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "add_to_delete_list.o",
  "profile": "authored_cpp",
  "sha256": "2039e5ec93efba03eae4fa7e5a2367ca0ec4ca8709f08678ae3e6a764a8edea7",
  "source": "sources/loop-grid-fx-delete-list/add_to_delete_list.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "list_head_copy.o",
  "profile": "authored_cpp",
  "sha256": "17f1e1b6583f9d295e159fe8ff92a9ffe702516c2e2ad5c62e4154875c523123",
  "source": "sources/loop-grid-fx-list-head-copy/list_head_copy.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "find_first.o",
  "profile": "authored_cpp",
  "sha256": "d8b27ab520e8925b9619685aa658bdc8fbd822bf02667de46354cfaf254ee0f8",
  "source": "sources/loop-grid-fx-find-first/find_first.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "find_tag_string.o",
  "profile": "authored_cpp",
  "sha256": "add70ed93bd34635df3b4481ad274b87333bc95a4cd1c45033e012ca0fbfa048",
  "source": "sources/loop-grid-fx-find-tag-string/find_tag_string.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "remove_string.o",
  "profile": "authored_cpp",
  "sha256": "4607f6628ad2a2e6219878ff096663da4e08e7e359155d4ed3c5855f9524e4a7",
  "source": "sources/loop-grid-fx-remove-string/remove_string.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "light_add.o",
  "profile": "authored_cpp",
  "sha256": "868ee6d645bde3d33afd6b86aa018710f45bb3299ff525bdd44c2cac0f2eda58",
  "source": "sources/loop-grid-fx-light-add/light_add.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "update_in_grid.o",
  "profile": "authored_cpp",
  "sha256": "d1596ba32b1c5fae3098fe55b02ff398a49b87f9417fb9182ccb41a5602a940e",
  "source": "sources/loop-grid-fx-update-in-grid/update_in_grid.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "iterate_grid.o",
  "profile": "authored_cpp",
  "sha256": "e995ed3cf2d19718f812d40abfda86dbd0a8a2dd0e8112fffca019cd419afb88",
  "source": "sources/loop-grid-fx-iterate-grid/iterate_grid.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "p_clear.o",
  "profile": "authored_cpp",
  "sha256": "439338a5c4145652dc941cd943c27f5dd88d6bc5c1cd0efd034edcba6eed5999",
  "source": "sources/loop-grid-fx-p-clear/p_clear.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "add_particle.o",
  "profile": "authored_cpp",
  "sha256": "f4b04eeecfaf3781bb5ae8afdcd40b3cf052e17bfe335a721845cc316bf04e24",
  "source": "sources/loop-grid-fx-add-particle/add_particle.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "update_particle_type.o",
  "profile": "authored_cpp",
  "sha256": "9e4b677ed880dcce69f46995c4c9136080a4ea0e80d3be0ae4ef651056bac6ce",
  "source": "sources/loop-grid-fx-update-particle-type/update_particle_type.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "has_number.o",
  "profile": "authored_cpp",
  "sha256": "87f9041ecce098aec520e7fbdc0e99bc1b28e8b872bb3f2738a2ebf65f148c31",
  "source": "sources/loop-grid-fx-has-number/has_number.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "emitter_tag.o",
  "profile": "authored_cpp",
  "sha256": "8c4959de7bcda474dfe8e0d55be4aa0cbffd17af9bc8cab2a27b935811f0fb95",
  "source": "sources/loop-grid-fx-emitter-tag/emitter_tag.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "script_particles.o",
  "profile": "authored_cpp",
  "sha256": "650fbdd7c5cb03072df90b8b7fd03bb109e8b1110646aa02f78ff5074eb679de",
  "source": "sources/loop-grid-fx-script-particles/script_particles.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "fx_dust.o",
  "profile": "authored_cpp",
  "sha256": "9c87d2f531dfd1ece9364b14c4ceca352180e7ce8d75c54d5e8ef686750493b6",
  "source": "sources/loop-grid-fx-fx-dust/fx_dust.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "task_list.o",
  "profile": "authored_cpp",
  "sha256": "1ea5a3e554ad11cd67c3030b3514f7ab08c2d0a0c1e342ae945c969ebea89c88",
  "source": "sources/loop-grid-fx-task-list/task_list.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "particle_storage.o",
  "profile": "authored_cpp",
  "sha256": "a361efd49bc8be4d7a3c1c74b067c1ac54a1a84a66f71b0a6aedb547baff47f6",
  "source": "sources/loop-grid-fx-particle-storage/particle_storage.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "p_update.o",
  "profile": "authored_cpp",
  "sha256": "cddc326de47725ebffadaa9b26c3db0942af0770006af7e1b1117a9789f3319b",
  "source": "sources/loop-grid-fx-p-update/p_update.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "find_in_box.o",
  "profile": "authored_cpp",
  "sha256": "ca2ba19e6b460784c40aa2557edf3c8d23e2659aa551ea1c7b7da4947f089889",
  "source": "sources/loop-grid-fx-find-in-box/find_in_box.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "add_to_medium_run_list.o",
  "profile": "authored_cpp",
  "sha256": "73a4b0f95d7a0f930ca99ba0b621d8b2eda6d86663e684e868a2b8a0ddc28bea",
  "source": "sources/loop-grid-fx-medium-run-list/add_to_medium_run_list.cpp"
 }
]
```

## Authored but never executed

All of it. The host checks run the functions on the host with stand-ins for their callees
(list routines, iterators, `setParticleDef`, `updatePS`, string primitives). Host float
checks use exactly representable values; the EE conversion `cvt.w.s` truncates on this CPU and
the host cast truncates, so negative fractions agree. No grid, light, particle system, trail
or frame update has run on the EE. The disassembly was matched by reading, not by byte
comparison: compiled bodies are larger than the image extents.

## Unknown

- The meaning of definition bytes 0..15 and 28..79 beyond the words listed above, and the
  role of `P_Pop__Fv` (a 4-byte stub in the image).
- Why `updatePS` returns "live" for the whole 32-update idle window before deletion
  (observed count 31, purpose not inferred).
- Whether any caller depends on `objectFindObject__FPc` returning a stack word read after
  the call (it reads `found[0]` only).
- The real capacity of `taskList`, and whether `scriptParticles[3]` is a terminator or only
  padding to the next symbol.
- Light cell origin offset 11520 against object offset 15360: both observed, reason unknown.
