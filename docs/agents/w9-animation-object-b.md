# Animation, object lists and creature AI (b) (w9-animation-object-b)

Branch `agents/w9-animation-object-b`. New source is under `experimental/sources/w9-animation-object-b-*`, one
directory per function or data object. Reference executable: the private `SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`), read locally only. Addresses and
extents below are that file's symbol values and sizes. No game bytes, listings or decompiler output are committed.
Nothing here has run on the EE. No original-byte match is claimed.

## Checks performed

- Every source compiles with the reviewed EE `g++` and the `authored_cpp` flags (also checked one at a time while writing).
- Every function directory has a `host_check.cpp` (host compiler, not a plan input) that stubs the callees, records call
  order and arguments, and exits non-zero on a wrong result. Expected values are computed in the check from the
  rules in the source comment, not by calling the function under test. Mutation spot checks made the host check fail for
  each of these: wrong constants (24.0 versus 6.0, 1/60, 9.0, the 0.75 and 0.4/0.6 weights, the angle ranges and bases),
  wrong comparison strictness (`<` versus `<=` on the 3.0 margin, the 5.0 depth, the life-tap roll), wrong bit masks
  (`&0xf` versus `&0x7`, `&7` versus `&3`, `0x401` versus `0x400`), a dropped term in the push or velocity update, and
  a wrong index rule for the attacking player.
- Differential check against the original (private tooling, outside git): a small R5900 interpreter loaded the image and ran
  the original function and the freshly compiled authored function from identical memory snapshots, with every callee
  replaced by the same recording stub (the original `eRand`, `icos`/`isin`, `strcasecmp`, `atof`, `strlen` etc. ran as
  original code where noted). Call sequences with their arguments (including float registers and the memory the
  callees would read), the return value where meaningful, and every changed byte outside the stack had to match.
  Scenario counts: `newDamageHandler` about 22,000 over several seeds (1,445 of the 1,468 original instructions executed; the 23 not
  executed are alignment padding and one delay slot that is unreachable), `move2` about 10,000 (418 of 422; the four not
  executed are padding), `objectActiveProcessing` 600, `finishConstruction` 300, `setParameters` 500,
  `findTagFloat` 250, `initFile` 108, `lumpAdd` 12, the list routines, `eRand`, the two anim-event setters and the two line
  accessors 20 to 60 each. All matched. The checks found and fixed these mistakes the host contracts had missed: the
  splash perturbation strength in `move2` (24.0, not 6.0), the 0.75 scale of the experience weights, the table index of
  the life-tap skill record (`skills[59]`, not 27), and a constant in `setParameters` that was one unit in the last place
  high. The interpreter's copy of `sqrt.s` had to be fixed to read the `ft` operand, as the R5900 does (the shared copy
  of the interpreter reads `fs`; its owner may want to know). Float results are IEEE, not R5900 arithmetic.
- Private partial build (copy of `experimental/` with the 24 units below appended to `snapshot.json` and
  `build-plan.json`, regenerated outside git): `--check-plan` passed; one build: 2,340 compilations, one link, 1,903 function
  bodies (1,876 before), 1,493 unresolved imports (1,511 before), no duplicate definitions.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Functions and data

| source directory (`experimental/sources/w9-animation-object-b-…`) | emitted name | image address | image size |
|---|---|---|---|
| `add-to-run-list` | `objectAddToRunList__FP10GameObject` | 0x143830 | 68 |
| `add-to-slow-run-list` | `objectAddToSlowRunList__FP10GameObject` | 0x1438a0 | 100 |
| `always-draw` | `objectAlwaysDraw__FP10GameObject` | 0x143498 | 60 |
| `cancel-all-always-draw` | `objectCancelAllAlwaysDraw__Fv` | 0x1434d8 | 124 |
| `active-processing` | `objectActiveProcessing__FP10GameObjectii` | 0x142158 | 436 |
| `e-rand` | `eRand__10GameObject` (+ `norrath_game_object_e_rand`) | 0x153fa8 | 40 |
| `find-tag-float` | `findTagFloat__10GameObjectPci` (+ `norrath_game_object_find_tag_float`) | 0x1410a0 | 332 |
| `game-line-get` | `gameLineGet__FPc` (+ `norrath_game_line_get`) | 0x279ad0 | 124 |
| `game-line-get-ptr` | `gameLineGetPtr__Fi` (+ `norrath_game_line_get_ptr`) | 0x279bf8 | 24 |
| `set-anim-event-flags` | `setAnimEventFlags__8Creatureififififi` (+ `norrath_creature_set_anim_event_flags`) | 0x1128a8 | 88 |
| `set-anim-event-flags-res-map` | `setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi` (+ `norrath_creature_set_anim_event_flags_res_map`) | 0x112900 | 60 |
| `finish-construction` | `finishConstruction__8Creature` (+ `norrath_creature_finish_construction`) | 0x1091b0 | 316 |
| `set-parameters` | `setParameters__8Creaturefffffi` (+ `norrath_creature_set_parameters`) | 0x1092f0 | 632 |
| `init-file` | `initFile__FPcPvi` | 0x1641a0 | 408 |
| `lump-add` | `lumpAdd__FPvPCc` | 0x1664b0 | 152 |
| `move2` | `move2__8CreatureG6Point3` | 0x10fc20 | 1688 |
| `new-damage-handler` | `newDamageHandler__8CreatureP12_modelHeader` | 0x10d428 | 5872 |
| `find-animation-bridge` | `norrath_object_find_animation` (naming bridge, not a reconstruction) | - | - |

Data units (no function bodies): `load-in-progress` defines `loadInProgress` (0x484110, 1 byte); `lump-state` defines
`myLumpBuffer` (0x484c88, 4) and `initializedLump` (0x484c90, 4); `load-name` defines `loadName` (0x8d14a0, 32 by
distance to the next symbol); `npc-lump-name` defines `npcLumpName` (0x486d84, 4); `never-save` defines `neverSave`
(0x333d98, nineteen words); `lumps` defines `lumps` (0x6cb280, 5120 bytes = 128 entries of 40) with its layout header
`lumps.hpp`, used by `lump-add`.

One commit per function or data object, plus: a correction commit for `setParameters` (a constant) and a comment-only
commit for four extents.

The `norrath_*` names in the table are the names the constructor sources import. A symbol alias cannot cross
translation units in this build, so each is a second symbol (`__attribute__((alias(...)))`, EE builds only) on
the same code as the mangled original. The mangled name is the original function; the second name adds no code.
The earlier authored `norrath_actor_finish_construction`, `norrath_actor_set_parameters` and
`norrath_player_find_tag_float` implement the same three originals under other names; this package did not touch them,
so those originals now exist twice in the link.

## Observed

- `objectActiveProcessing__FP10GameObjectii`: the radius is 0xabe0 when object flag 0x80000 is set, else the third argument;
  the squared radius is the 32-bit integer product converted to float. The reference point is `worldViewCenter` when the byte
  `objectVisUseCamCenter` (0x483ac9) is nonzero, otherwise any of the first `objectVisCount` (signed byte at 0x483afe)
  entries of `objectVisPoints` (0x333d28, 12-byte points). A changed answer moves the object between `runList` and
  `slowRunList[cycle]` with the cycle wrapped at 60 exactly as `objectAddToSlowRunList` does.
- `objectAddToRunList`, `objectAlwaysDraw`: the first passes object+4 as the second list head; `objectAlwaysDraw` passes the
  object address itself (as the delete-list routine does) and then sets flag bit 1 (0x2); `objectCancelAllAlwaysDraw` clears it
  for every node of `alwaysDrawList` (0x486888) through a safe iterator masked with `objectIgnoreFlags`, then calls `listDestroy`.
- `eRand__10GameObject`: `seed = seed * 0x19660d + 0x3c6ef35f` with the seed at object+68; the result is the new seed shifted
  right 16 (0 to 65535).
- `findTagFloat__10GameObjectPci`: the same tag search as `findTagString` (the query is lowercased in place), then `atof` and
  `dptofp` on the text after `=`; 0.0 when absent.
- `lumpAdd__FPvPCc`: a null lump makes the routine store zero through the null pointer (reproduced with a volatile store);
  otherwise `initLump`, then `lumps[nmLumps++]` receives the name (strcpy at +0), the lump pointer at +16 and zeros at
  +20, +24, +28, +32, +36. `initFile__FPcPvi` picks the initializer from the text after the last `.`:
  anm, vif, tex, fnt, world, vag (result size - 64), uni; `etex` is compared and ignored.
- `setParameters__8Creaturefffffi` and `finishConstruction__8Creature` match the already authored `norrath_actor_*`
  versions in behavior; their constants are written as exact bit patterns.
- `move2__8CreatureG6Point3`: the result bits are 1 (world move hit), 2 (objects touched) and 4 (landed). The water, splash,
  platform (flag 0x10), push-apart and ground steps are described in the source comment with every constant.
  The word 0x47fc3c, which is the gravity-like 9.0, is 8.999999 in the image (0x410fffff).
- `newDamageHandler__8CreatureP12_modelHeader`: structure and constants are in the source comment. The per-damage
  tables (hit speeds 50/75/100/156.25 when nothing is pending, 100/150/200/312.5 on death, 100/150/225/375 when alive),
  the gut-chunk and effect numbers, the 50-particle shower for flag 0x20000, the debris spray driven by `asyncRandState`
  and the rewards (experience split 0.5 / 0.6 / 0.4, life tap capped at the player's maximum) are all observed.
- Float constants in these sources are written as hex floats where the executable's word differs from the nearest
  decimal float (1/60 = 0x3c888888, 0.001 = 0x3a83126e, 1.3 = 0x3fa66666, 0.8 = 0x3f4ccccc, 1.1 = 0x3f8ccccc).

## Inferred

- The `Creature` offsets used (+176 water depth, +180/+184 pending displacement, +192 pending damage, +560 health, +744
  damage-type bits, +752 attacker) are named from use only. Fields of other objects touched by `move2` are assumed to share the
  creature's leading layout (+12 flags, +16 position, +28 radius, +32 height, +72 virtual table).
- The virtual table entries are eight bytes: a halfword delta and a function pointer at +4. Entry +184 is called with an
  integer (10 for death, 8 for a hurt reaction); entry +48 of a touched object, +80 and +88 of a mover are contact callbacks.
- `skills[59]` is the life-tap record (skill 59 is passed to `playerGetSkillRank` and the record's `getChance` is called).
- The record at `lumps` is 40 bytes by the `lumpAdd` stride and 128 entries by the extent to the next symbol.

## Unknown

- What `loadName` holds beyond a string compared with `strcmp`, and its declared capacity (32 is the distance to the next symbol).
- The hit record `move2` passes to `objectMoveWithWorldCollision` (16 bytes assumed from the stack gap), the meaning of
  almost every flag bit, what the `+48` (9.0) constant is, and the unnamed callee behaviors.
- The type of `finishConstruction`'s result (it returns the end-of-blend call's result or 0) and of `setParameters`' result
  (the tag count).
- Whether `objectAddToSlowRunList`/`objectAddToRunList` return anything (the originals do not set a result after the last call).

## Imports closed (names that left the unresolved list)

`eRand__10GameObject`, `findTagFloat__10GameObjectPci`, `initFile__FPcPvi`, `initializedLump`, `loadInProgress`, `loadName`,
`lumpAdd__FPvPCc`, `lumps`, `move2__8CreatureG6Point3`, `myLumpBuffer`, `neverSave`,
`newDamageHandler__8CreatureP12_modelHeader`, `norrath_creature_finish_construction`,
`norrath_creature_set_anim_event_flags`, `norrath_creature_set_anim_event_flags_res_map`, `norrath_creature_set_parameters`,
`norrath_game_line_get`, `norrath_game_line_get_ptr`, `norrath_game_object_e_rand`, `norrath_game_object_find_tag_float`,
`norrath_object_find_animation`, `npcLumpName`, `objectActiveProcessing__FP10GameObjectii`, `objectAddToRunList__FP10GameObject`,
`objectAddToSlowRunList__FP10GameObject`, `objectAlwaysDraw__FP10GameObject`, `objectCancelAllAlwaysDraw__Fv`,
`setAnimEventFlags__8Creatureififififi`. (28 names; the list given in the task also named `listOnewayLink__FP8ListHeadT0Pv`,
`mediumRunList`, `next__16ListSafeIterator`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`,
`objectClear__Fv` and `objectDoMyActions__FP10GameObjecti`, which the w8-object-list-math package had already closed.)

## Imports opened

`__7OniBallG6Point3iP10GameObjectP10_vagHeader`, `animSetToEndFullyBlended__FP14AnimationState`, `asyncRandState`,
`findTagInt__10GameObjectPci`, `fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff`,
`getChance__12SkillLifeTapi`, `modelGetBoundingBox__FP12_modelHeaderlPiN52`, `objectMoveWithWorldCollision__FP6Point3G6Point3fiT0`,
`sendAlertMessage__FifG6Point3P10GameObject`, `trailParticleSpark`. They are the original names of callees and data in
other packages' areas. Existing authored providers with other names exist for some (`norrath_animation_set_to_end_fully_blended`,
`norrath_game_object_find_tag_int`, `norrath_model_bounding_box`); the originals were imported by their original names instead.
Imports that were already unresolved and are now also used here: `blockAlloc__Fi`, `addEffect__FiG6Point3`,
`__8GutChunkG6Point3iif`, `floorAdd__FG6Point3sfii`, `initStuff__6PhPropi`, `disintegrationEffect…`,
`modelGetCharAttachmentPos…`, `player`, `playerGetSkillRank__Fii`, `skills`, `gameLines`, `gameLineCount`, `objectVisCount`,
`objectVisPoints`, `objectVisUseCamCenter`, `worldCheckForWater…`, `worldFindStandHeight…`, `worldPerturbWater…`.
Possible duplicate: `trailParticleSpark` has four symbols of that name in the image (0x4e2080 is the one used here).

## Build-plan entries needed

Append these 24 units to `experimental/snapshot.json` (inputs, `files`) and `experimental/build-plan.json` (inputs, `files`,
`default_authored_inputs` +24), profile `authored_cpp`, `kind` `independently_authored_source`. `header_closure` lists the
quoted-include closure; `object_name` is the source stem plus `.o`. The order below is only the order of one private run.
`lump-add` includes `../w9-animation-object-b-lumps/lumps.hpp`; the other headers are from `w8-object-list-math-common`.
The `emitted_functions` sizes are from the reviewed toolchain and must be re-measured by whoever regenerates the snapshot.
Host checks and `host_check.cpp` files are not plan inputs.

| order | source (under `experimental/`) | sha256 | emitted functions (bytes) | header closure |
|---|---|---|---|---|
| 2318 | `sources/w9-animation-object-b-active-processing/active_processing.cpp` | `0145767a4e7f5ba0e4371bec07a9b56e82bc3bce37d62f3d29d7f0ddadbfa1b2` | `objectActiveProcessing__FP10GameObjectii` (480) | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp` |
| 2319 | `sources/w9-animation-object-b-add-to-run-list/add_to_run_list.cpp` | `88b7afdbb03db5e47cc606145bb7beadbc990efb8f2a08cca1548bd16f5bbd98` | `objectAddToRunList__FP10GameObject` (80) | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp` |
| 2320 | `sources/w9-animation-object-b-add-to-slow-run-list/add_to_slow_run_list.cpp` | `7e4a7f2ebbbddb1f3fd912b39e53cfd93908bc7348f5f4e5b864ca828699f956` | `objectAddToSlowRunList__FP10GameObject` (216) | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp` |
| 2321 | `sources/w9-animation-object-b-always-draw/always_draw.cpp` | `09c3d88ab30574382a769b72456e26a6f658f7865de2ecffd071ba9e1c7ca15d` | `objectAlwaysDraw__FP10GameObject` (60) | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp` |
| 2322 | `sources/w9-animation-object-b-cancel-all-always-draw/cancel_all_always_draw.cpp` | `f8dfb96a63769fce6946c8a7c6a1b2effbb4ef2fc2d40fdd7a3987db42e1a82d` | `objectCancelAllAlwaysDraw__Fv` (132) | `sources/w8-object-list-math-common/list_types.hpp`, `sources/w8-object-list-math-common/object_types.hpp`, `sources/w8-object-list-math-common/safe_iterator.hpp` |
| 2323 | `sources/w9-animation-object-b-e-rand/e_rand.cpp` | `23a2eecea5aba264176d77ffb6f1560b84b15759d25b853f3d789e130f537d17` | `eRand__10GameObject` (56), `norrath_game_object_e_rand` (56) | - |
| 2324 | `sources/w9-animation-object-b-find-animation-bridge/find_animation_bridge.cpp` | `02091042ef3b2c01de9583fd410b1a69bedbab327e92965a1338d566d57b46d5` | `norrath_object_find_animation` (8) | - |
| 2325 | `sources/w9-animation-object-b-find-tag-float/find_tag_float.cpp` | `d1cd922239cc25b9d8ef7fc3bb63b6108c3a43d628bdfa79f1cbf665fae0ef3e` | `findTagFloat__10GameObjectPci` (376), `norrath_game_object_find_tag_float` (376) | - |
| 2326 | `sources/w9-animation-object-b-finish-construction/finish_construction.cpp` | `014349c0d1f7cb15b57a65efd761a1ee2c7c8a260655ddf3e4caa59c10eed5f7` | `finishConstruction__8Creature` (320), `norrath_creature_finish_construction` (320) | - |
| 2327 | `sources/w9-animation-object-b-game-line-get/game_line_get.cpp` | `48fb814748b6bb4a1a1a6dc54124c41f2da5c165cf645547ba48fda68b8b195d` | `gameLineGet__FPc` (172), `norrath_game_line_get` (172) | - |
| 2328 | `sources/w9-animation-object-b-game-line-get-ptr/game_line_get_ptr.cpp` | `17e39fac4003568043412f78d4b0e2297543dcb3c3832ae86eb48ac3bdcdfd48` | `gameLineGetPtr__Fi` (24), `norrath_game_line_get_ptr` (24) | - |
| 2329 | `sources/w9-animation-object-b-init-file/init_file.cpp` | `c2454d19e622b6c122658580af37e9fb838b14442f568581bc4e670e5ff3320f` | `initFile__FPcPvi` (404) | - |
| 2330 | `sources/w9-animation-object-b-load-in-progress/load_in_progress.cpp` | `c5756974a33ac3b9276cafe880aebcfc6980603eb0ec39be26c661a9fcc800ce` | data only | - |
| 2331 | `sources/w9-animation-object-b-load-name/load_name.cpp` | `9ad62efe9cb1cb594be90aec3096227b648cda6f61549a7c96c99d75ad34e42a` | data only | - |
| 2332 | `sources/w9-animation-object-b-lump-add/lump_add.cpp` | `9e30970eace8059ff9326f23539e71c5ed1afbf5e3bf665f8184104904a2006c` | `lumpAdd__FPvPCc` (152) | `sources/w9-animation-object-b-lumps/lumps.hpp` |
| 2333 | `sources/w9-animation-object-b-lump-state/lump_state.cpp` | `0a80b29fa24c3e9f9ce53b27fb9e2a347c1e22d0204be60d52cbe3b31b6bbb61` | data only | - |
| 2334 | `sources/w9-animation-object-b-lumps/lumps.cpp` | `3984e9ccfe990a856437d518f0e4867977f267f0ae4b4bae42191e9bcf374798` | data only | `sources/w9-animation-object-b-lumps/lumps.hpp` |
| 2335 | `sources/w9-animation-object-b-move2/move2.cpp` | `6f65fff3770232ec91aba8c9e83786a5d8ed3134ba1b679625af3adc8e5e5d4e` | `move2__8CreatureG6Point3` (1716) | - |
| 2336 | `sources/w9-animation-object-b-never-save/never_save.cpp` | `88284bcaec8a2614faad3c12b3f6f33fef834baef711614d197bf9e34e6c5ed9` | data only | - |
| 2337 | `sources/w9-animation-object-b-new-damage-handler/new_damage_handler.cpp` | `cd4d306295fbc9fb63fe0d280252ad87a96a3a5343d4ffd3908e7a8306f4d221` | `_ZN12_GLOBAL__N_1L10push_chunkEP14DamageCreaturePvfffjj.constprop.0` (240), `newDamageHandler__8CreatureP12_modelHeader` (5596) | - |
| 2338 | `sources/w9-animation-object-b-npc-lump-name/npc_lump_name.cpp` | `2e9cb40283917cad00d9b1f3c7550f427cf5677f026dc5fa67e148a947a441b4` | data only | - |
| 2339 | `sources/w9-animation-object-b-set-anim-event-flags/set_anim_event_flags.cpp` | `537be6c11aba77f7f48f2de538f339cf7dc8ed2d5e2ad391bec063a7fd299ae9` | `norrath_creature_set_anim_event_flags` (100), `setAnimEventFlags__8Creatureififififi` (100) | - |
| 2340 | `sources/w9-animation-object-b-set-anim-event-flags-res-map/set_anim_event_flags_res_map.cpp` | `5b2e4dfbd37a70216e985b8a67495f644c6fa94974c26195bb0b27f5590b0625` | `norrath_creature_set_anim_event_flags_res_map` (64), `setAnimEventFlagsResMap__8CreaturePQ210GameObject11resourceMapififififi` (64) | - |
| 2341 | `sources/w9-animation-object-b-set-parameters/set_parameters.cpp` | `bdf0bcd6d98c36a1a1cb1126ad8e0536a398bcdc4c26856f47a396746337d214` | `norrath_creature_set_parameters` (752), `setParameters__8Creaturefffffi` (752) | - |

## Authored but never executed

Everything above. None of it has run on the EE or in a game process. The host contracts run on the host compiler with stub callees.
The differential runs used an interpreter and stub callees; they cover the authored function's own instructions,
not the callees' code, hardware timing, or R5900 float rounding. The following are not exercised by any check at all:
the null-lump store in `lumpAdd` (a write to address 0, not performed on the host and not hit by the interpreter scenarios),
the `norrath_*` second symbols (EE-only aliases; the importers' calls were not run), and the data units beyond their size and initial value.
`move2`'s hit record and the `newDamageHandler` callees see stub memory only.

## Unknown / blocked

- Nothing was blocked. `newDamageHandler` and `move2` are large; both were completed and matched in the differential runs.
- Not attempted (outside the list): the other creature behavior (`everyFrame__8CreatureP12_modelHeader`, `steerAndRouteToPosition`,
  `getClosestLivingPlayer`), the second copies of the three `norrath_actor_*` / `norrath_player_*` routines noted above,
  and the callees opened above.
