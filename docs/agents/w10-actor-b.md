# w10-actor-b: spell, skill and actor virtuals (AntLanternCtors to AutoObject)

Branch `agents/w10-actor-b`. Package: the 32 imports listed under "Imports closed" below — the six chest
resource maps behind `chestResourceList`, and the virtual methods, destructors, type-descriptor accessors
and the clone of AntQueen, Arenabeast, AutoChest, AutoContainer and AutoObject.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that file's symbol
values; no game bytes or listings are committed. The import list came from
`work/plan10-build/unresolved-imports.json` (the newest local build output); I did not run a new
partial build (it would take about as long as the rest of this package).

## Method (every function)

1. Observed: the original's instructions were read from the private image with the reviewed binutils and
   decoded by hand into the source; every constant, string and table is read from the image.
2. Each unit compiles alone with the reviewed EE `g++` `authored_cpp` flags. Compiled sizes differ from the
   image extents (column below); four match exactly (`msg_load__10AutoObjectPc` 8, `msg_trigger__8AntQueeni` 136,
   `msg_trigger__10Arenabeasti` 112, `msg_run__10Arenabeast` 3288). These are behaviour reconstructions, not byte matches.
3. Each unit directory has a `host_check.cpp` (host compiler, not an EE input) with mocked callees that fails
   if a field, argument, call order, boundary or return is wrong.
4. Each function was also run against the original in a private R5900 interpreter kept outside git: the
   original and the authored object start from identical memory snapshots with identical stand-in callees;
   call logs (with arguments, floats and key fields at call time) and every changed byte outside the stack
   are compared. Hundreds to thousands of scenarios per function (state × field × callee-result grids).
   Instruction coverage of the original was measured for the large functions; uncovered words were nops,
   annulled branch-likely slots, or unreachable code, as noted in the commit messages.
5. Mutants of every function (changed constants, offsets, flags, branch order) were run through the paired
   test and detected unless the commit message says the survivor was equivalent.
6. Nothing ran on an EE, an emulator or as gameplay. No claim of compatibility is made.

Shared helper: `sources/w10-actor-b-common/fields.hpp` (may-alias field access at observed byte offsets;
pointer-valued words go through the existing `rtti_c::pointer()` host mapping of
`w9-rtti-vtables-entities-c-bytes/bytes.hpp`).

## Observed, inferred, unknown (summary; per-function detail is in each source's header comment)

- Observed: all offsets, constants (including single-precision bit patterns that are one ulp below the nearest
  literal, such as 0.3 = 0x3e999999, 0.1 = 0x3dcccccc, 0.6 = 0x3f199999), string literals, jump-table
  contents (all states), call order and argument registers.
- Observed: `AutoChest`'s destructor stores the `10AutoObject` vtable, so its body equals `_$_10AutoObject`.
- Observed: the four boss-meter functions store the integer conversion of the health single into
  `bossMeterCurrent` with a float store instruction; the word is an int (the shared declaration is `int`).
- Inferred: that `+560` is current health, that `+572` is the state (virtual slot 184 enters a state), that
  slot 168 is the per-frame enemy update, and that the item-description fields written on the stack are the
  default-constructed part of `item::description`.
- Unknown: the names of the states and attack kinds, the roles of most counters (`+256`, `+716`, `+804`,
  `+808`, `+960`..`+976`), the owner of the file-local particle definitions, and what the 0x40008 /
  0x10004 attack-flag words select.

## Functions and data

| source | symbol | image | image bytes | compiled bytes |
|---|---|---|---|---|
| `w10-actor-b-clone-antqueen/clone.cpp` | `clone__8AntQueen` | `0x1171f0` | 1412 | 1260 |
| `w10-actor-b-dtor-10arenabeast/dtor.cpp` | `_$_10Arenabeast` | `0x315bb8` | 100 | 88 |
| `w10-actor-b-dtor-10autoobject/dtor.cpp` | `_$_10AutoObject` | `0x314d20` | 116 | 120 |
| `w10-actor-b-dtor-13autocontainer/dtor.cpp` | `_$_13AutoContainer` | `0x221870` | 204 | 264 |
| `w10-actor-b-dtor-8antqueen/dtor.cpp` | `_$_8AntQueen` | `0x30e900` | 100 | 88 |
| `w10-actor-b-dtor-9autochest/dtor.cpp` | `_$_9AutoChest` | `0x314df0` | 116 | 120 |
| `w10-actor-b-msg-draw-antqueen/msg_draw.cpp` | `msg_draw__8AntQueen` | `0x208938` | 104 | 100 |
| `w10-actor-b-msg-draw-arenabeast/msg_draw.cpp` | `msg_draw__10Arenabeast` | `0x236950` | 68 | 56 |
| `w10-actor-b-msg-hurt-antqueen/msg_hurt.cpp` | `msg_hurt__8AntQueenfP10GameObjecti` | `0x208a28` | 88 | 84 |
| `w10-actor-b-msg-hurt-arenabeast/msg_hurt.cpp` | `msg_hurt__10ArenabeastfP10GameObjecti` | `0x236a08` | 104 | 100 |
| `w10-actor-b-msg-load-autoobject/msg_load.cpp` | `msg_load__10AutoObjectPc` | `0x224ab0` | 8 | 8 |
| `w10-actor-b-msg-move-collision-arenabeast/msg_move_collision.cpp` | `msg_moveCollision__10ArenabeastPP10GameObject` | `0x236a70` | 244 | 268 |
| `w10-actor-b-msg-poke-autochest/msg_poke.cpp` | `msg_poke__9AutoChesti` | `0x21f6d8` | 448 | 420 |
| `w10-actor-b-msg-run-antqueen/msg_run.cpp` | `msg_run__8AntQueen` | `0x206ea8` | 2540 | 2616 |
| `w10-actor-b-msg-run-arenabeast/msg_run.cpp` | `msg_run__10Arenabeast` | `0x2357f0` | 3288 | 3288 |
| `w10-actor-b-msg-run-autochest/msg_run.cpp` | `msg_run__9AutoChest` | `0x21f1a8` | 1328 | 1476 |
| `w10-actor-b-msg-run-autocontainer/msg_run.cpp` | `msg_run__13AutoContainer` | `0x221a20` | 4124 | 4036 |
| `w10-actor-b-msg-trigger-antqueen/msg_trigger.cpp` | `msg_trigger__8AntQueeni` | `0x2089a0` | 136 | 136 |
| `w10-actor-b-msg-trigger-arenabeast/msg_trigger.cpp` | `msg_trigger__10Arenabeasti` | `0x236998` | 112 | 112 |
| `w10-actor-b-set-state-antqueen/set_state.cpp` | `setState__8AntQueeni` | `0x207898` | 2068 | 2608 |
| `w10-actor-b-set-state-arenabeast/set_state.cpp` | `setState__10Arenabeasti` | `0x2364c8` | 1160 | 1208 |
| `w10-actor-b-tf-10arenabeast/typeinfo.cpp` | `__tf10Arenabeast` | `0x315c20` | 80 | 92 |
| `w10-actor-b-tf-10autoobject/typeinfo.cpp` | `__tf10AutoObject` | `0x314d98` | 76 | 92 |
| `w10-actor-b-tf-13autocontainer/typeinfo.cpp` | `__tf13AutoContainer` | `0x314f78` | 132 | 184 |
| `w10-actor-b-tf-8antqueen/typeinfo.cpp` | `__tf8AntQueen` | `0x30e968` | 80 | 92 |
| `w10-actor-b-tf-9autochest/typeinfo.cpp` | `__tf9AutoChest` | `0x314e68` | 132 | 184 |
| `w10-actor-b-views/norrath_ctors_ant_lantern_view_42c7b0.cpp` | `norrath_ctors_ant_lantern_view_42c7b0` | `0x42c7b0` | 40 | 40 |
| `w10-actor-b-views/norrath_ctors_ant_lantern_view_42c7d8.cpp` | `norrath_ctors_ant_lantern_view_42c7d8` | `0x42c7d8` | 40 | 40 |
| `w10-actor-b-views/norrath_ctors_ant_lantern_view_42c800.cpp` | `norrath_ctors_ant_lantern_view_42c800` | `0x42c800` | 40 | 40 |
| `w10-actor-b-views/norrath_ctors_ant_lantern_view_42c828.cpp` | `norrath_ctors_ant_lantern_view_42c828` | `0x42c828` | 40 | 40 |
| `w10-actor-b-views/norrath_ctors_ant_lantern_view_42c850.cpp` | `norrath_ctors_ant_lantern_view_42c850` | `0x42c850` | 40 | 40 |
| `w10-actor-b-views/norrath_ctors_ant_lantern_view_42c878.cpp` | `norrath_ctors_ant_lantern_view_42c878` | `0x42c878` | 40 | 40 |

## Imports closed (32)

`norrath_ctors_ant_lantern_view_42c7b0`, `norrath_ctors_ant_lantern_view_42c7d8`, `norrath_ctors_ant_lantern_view_42c800`, `norrath_ctors_ant_lantern_view_42c828`, `norrath_ctors_ant_lantern_view_42c850`, `norrath_ctors_ant_lantern_view_42c878`, `_$_8AntQueen`, `__tf8AntQueen`, `clone__8AntQueen`, `msg_draw__8AntQueen`, `msg_hurt__8AntQueenfP10GameObjecti`, `msg_run__8AntQueen`, `msg_trigger__8AntQueeni`, `setState__8AntQueeni`, `_$_10Arenabeast`, `__tf10Arenabeast`, `msg_draw__10Arenabeast`, `msg_hurt__10ArenabeastfP10GameObjecti`, `msg_moveCollision__10ArenabeastPP10GameObject`, `msg_run__10Arenabeast`, `msg_trigger__10Arenabeasti`, `setState__10Arenabeasti`, `_$_9AutoChest`, `__tf9AutoChest`, `msg_poke__9AutoChesti`, `msg_run__9AutoChest`, `_$_13AutoContainer`, `__tf13AutoContainer`, `msg_run__13AutoContainer`, `_$_10AutoObject`, `__tf10AutoObject`, `msg_load__10AutoObjectPc`

Also defined (nodes initialised by these accessors): `__ti8AntQueen`, `__ti10Arenabeast` (12-byte zero nodes inside `__tf8AntQueen` / `__tf10Arenabeast`).

## Imports opened

Symbols my units reference that neither the plan-10 partial object defines nor its unresolved list names (a partial build against the shared branch may find some of them supplied by other packages; it was not run here):

`__7AntWaveP10GameObjectG6Point3si`, `addTurnAnimation__8AntQueen`, `alternateEnemyInZone__8Creatureffss`, `dropTreasure__4itemG6Point3iffffffPci`, `enemyInZone2__8Creatureffssfi`, `enemyInZone__8Creaturefs`, `findEvadeDestination__8AntQueen`, `norrath_auto_chest_msg_run_fuseParticle_bytes`, `norrath_auto_chest_msg_run_smokeParticle_bytes`, `produceGeneralDamage__8CreatureffssiiiiPQ24item11description`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `setOnFire__6PhPropi`, `spawnItems__9AutoChest`, `steerAndRouteToEnemy__8Creature`, `subTurnAnimation__8AntQueen`, `turnToFaceEnemy__8Creature`

Symbols my units reference that were already unresolved in the plan-10 baseline (not newly opened): `__15AnimationState8RC15AnimationState8`, `__ti10GameObject`, `addEffect__FiG6Point3`, `animGetAnimComponent__FP14AnimationStateP15AnimationHeader`, `bossMeterName`, `create__4itemPcPQ24item11descriptionii`, `dropTreasure__8CreatureG6Point3iffffffPc`, `findTagInt__10GameObjectPci`, `floorAdd__FG6Point3sfii`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `skills`.

## Build-plan entries needed

Append, in any order, with fresh `input_order` values; profile `authored_cpp`, kind `independently_authored_source`. `files` additions are the sha256 of every source and header listed.

```json
[
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "clone.o",
  "profile": "authored_cpp",
  "sha256": "fe6905cd4feed3bc5025244d664723c1cf6b840a5c595a8448c48d68d0f5de1a",
  "source": "sources/w10-actor-b-clone-antqueen/clone.cpp",
  "emitted_functions": [
   {
    "name": "clone__8AntQueen",
    "compiled_body_bytes": 1260
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "dtor.o",
  "profile": "authored_cpp",
  "sha256": "3dd7fc48cf8317a7bfcae033261cc5612bdf4768b33ecde65726be7673e61638",
  "source": "sources/w10-actor-b-dtor-10arenabeast/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_10Arenabeast",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "dtor.o",
  "profile": "authored_cpp",
  "sha256": "f9e95ce457ef847dc18e84be27432cb147cab937c5e3cb56473e75adc1a0d799",
  "source": "sources/w10-actor-b-dtor-10autoobject/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_10AutoObject",
    "compiled_body_bytes": 120
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "dtor.o",
  "profile": "authored_cpp",
  "sha256": "a32bf226770ad7e915242c572cc8bb00d15661b6df0329caa4827fdbe7cb6e04",
  "source": "sources/w10-actor-b-dtor-13autocontainer/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13AutoContainer",
    "compiled_body_bytes": 264
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "dtor.o",
  "profile": "authored_cpp",
  "sha256": "dd542f5b9799e42c5b087d012a3cc3f0f004b26ccd6be822c316350c2d1594b7",
  "source": "sources/w10-actor-b-dtor-8antqueen/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_8AntQueen",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "dtor.o",
  "profile": "authored_cpp",
  "sha256": "0399c90b07d962fb6801f32e88106daaed9faf6b3ef5b4148614e9213096517b",
  "source": "sources/w10-actor-b-dtor-9autochest/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_9AutoChest",
    "compiled_body_bytes": 120
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_draw.o",
  "profile": "authored_cpp",
  "sha256": "2a0f167fa2adc7782e57314da277f562c8c7cff5a654db5f37db6788984d7881",
  "source": "sources/w10-actor-b-msg-draw-antqueen/msg_draw.cpp",
  "emitted_functions": [
   {
    "name": "msg_draw__8AntQueen",
    "compiled_body_bytes": 100
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_draw.o",
  "profile": "authored_cpp",
  "sha256": "2bf3366ea7cec891bc9719c195df18ac9cce281fb38a6528ee5643f3d4098f65",
  "source": "sources/w10-actor-b-msg-draw-arenabeast/msg_draw.cpp",
  "emitted_functions": [
   {
    "name": "msg_draw__10Arenabeast",
    "compiled_body_bytes": 56
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_hurt.o",
  "profile": "authored_cpp",
  "sha256": "e215074b9f02e9de55bc44ad864598e5a6d757c206e262d48287dce3bc1d2328",
  "source": "sources/w10-actor-b-msg-hurt-antqueen/msg_hurt.cpp",
  "emitted_functions": [
   {
    "name": "msg_hurt__8AntQueenfP10GameObjecti",
    "compiled_body_bytes": 84
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_hurt.o",
  "profile": "authored_cpp",
  "sha256": "981de52d22de3b27786ce80afc35c813448b950495e7865c7cc8015b10c2ffc9",
  "source": "sources/w10-actor-b-msg-hurt-arenabeast/msg_hurt.cpp",
  "emitted_functions": [
   {
    "name": "msg_hurt__10ArenabeastfP10GameObjecti",
    "compiled_body_bytes": 100
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "msg_load.o",
  "profile": "authored_cpp",
  "sha256": "976406fa160e5077e4d7e9c929a0d985b3b21233716e923cfd3f34ef079f0386",
  "source": "sources/w10-actor-b-msg-load-autoobject/msg_load.cpp",
  "emitted_functions": [
   {
    "name": "msg_load__10AutoObjectPc",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_move_collision.o",
  "profile": "authored_cpp",
  "sha256": "77ffc922fdb0489e8033604abcbabdc4c86999dfd82d3ebd4802213a2492d652",
  "source": "sources/w10-actor-b-msg-move-collision-arenabeast/msg_move_collision.cpp",
  "emitted_functions": [
   {
    "name": "msg_moveCollision__10ArenabeastPP10GameObject",
    "compiled_body_bytes": 268
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_poke.o",
  "profile": "authored_cpp",
  "sha256": "9d6378629290010f7e88ba06df5ce3c5c40f02a33fa65ca4486c18d1c49f7ed9",
  "source": "sources/w10-actor-b-msg-poke-autochest/msg_poke.cpp",
  "emitted_functions": [
   {
    "name": "msg_poke__9AutoChesti",
    "compiled_body_bytes": 420
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_run.o",
  "profile": "authored_cpp",
  "sha256": "eaa93530f94f032fe4c9cfec322f6269e034325b718b10922f840d43739331c3",
  "source": "sources/w10-actor-b-msg-run-antqueen/msg_run.cpp",
  "emitted_functions": [
   {
    "name": "msg_run__8AntQueen",
    "compiled_body_bytes": 2616
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_run.o",
  "profile": "authored_cpp",
  "sha256": "3a497d9610cc4cbfb6d5239e485ca28044eea8348ab29443792e262185d93520",
  "source": "sources/w10-actor-b-msg-run-arenabeast/msg_run.cpp",
  "emitted_functions": [
   {
    "name": "msg_run__10Arenabeast",
    "compiled_body_bytes": 3288
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_run.o",
  "profile": "authored_cpp",
  "sha256": "e0b65a8535e259ae71bf8a01f487c4e980b0b633f1d11f65f1d2da13f434eaf5",
  "source": "sources/w10-actor-b-msg-run-autochest/msg_run.cpp",
  "emitted_functions": [
   {
    "name": "msg_run__9AutoChest",
    "compiled_body_bytes": 1476
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_run.o",
  "profile": "authored_cpp",
  "sha256": "06c393b023117284269de3e20ee71103c414556a89fd90dd9fd50ba854293340",
  "source": "sources/w10-actor-b-msg-run-autocontainer/msg_run.cpp",
  "emitted_functions": [
   {
    "name": "msg_run__13AutoContainer",
    "compiled_body_bytes": 4036
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_trigger.o",
  "profile": "authored_cpp",
  "sha256": "d862f590a949ee1a535e8c9e5626d9352fbf9c5f13893140896e539216f0fd2c",
  "source": "sources/w10-actor-b-msg-trigger-antqueen/msg_trigger.cpp",
  "emitted_functions": [
   {
    "name": "msg_trigger__8AntQueeni",
    "compiled_body_bytes": 136
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "msg_trigger.o",
  "profile": "authored_cpp",
  "sha256": "52fe05c87bb61ea05f66fcedb5c6d6a954358c1a7749fccf95a9ea02fb22ea32",
  "source": "sources/w10-actor-b-msg-trigger-arenabeast/msg_trigger.cpp",
  "emitted_functions": [
   {
    "name": "msg_trigger__10Arenabeasti",
    "compiled_body_bytes": 112
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "set_state.o",
  "profile": "authored_cpp",
  "sha256": "10e7f7396dc0daec74a8e063b7d940e5b659957498c1631a6c0433796c818dd3",
  "source": "sources/w10-actor-b-set-state-antqueen/set_state.cpp",
  "emitted_functions": [
   {
    "name": "setState__8AntQueeni",
    "compiled_body_bytes": 2608
   }
  ]
 },
 {
  "header_closure": [
   "sources/w10-actor-b-common/fields.hpp",
   "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "set_state.o",
  "profile": "authored_cpp",
  "sha256": "eaaad55fe322fe0b71a39fb30a4ce58debcd2a583b902316814552b01268ca15",
  "source": "sources/w10-actor-b-set-state-arenabeast/set_state.cpp",
  "emitted_functions": [
   {
    "name": "setState__10Arenabeasti",
    "compiled_body_bytes": 1208
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "typeinfo.o",
  "profile": "authored_cpp",
  "sha256": "edd32981e88e16710806eba3bf0d2b5ef0022f5dc0d9146929d07da582189177",
  "source": "sources/w10-actor-b-tf-10arenabeast/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10Arenabeast",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "typeinfo.o",
  "profile": "authored_cpp",
  "sha256": "83bc9be83f7bd0e2514bf99ec633d033d7e3de2b2ef1da7f60545b26399243c0",
  "source": "sources/w10-actor-b-tf-10autoobject/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10AutoObject",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "typeinfo.o",
  "profile": "authored_cpp",
  "sha256": "3180e0da2bf3b2a2ab69b2935179fbc977920ce4cb4695d02064df8494963b97",
  "source": "sources/w10-actor-b-tf-13autocontainer/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13AutoContainer",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "typeinfo.o",
  "profile": "authored_cpp",
  "sha256": "a4a841f723e837faaad46ba4b22790f242546924d042ff0370d10a7f9874f8cb",
  "source": "sources/w10-actor-b-tf-8antqueen/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf8AntQueen",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "typeinfo.o",
  "profile": "authored_cpp",
  "sha256": "86d0a63d83cb448224d3a22d6f7e2a3b006d62af9cbbac14c7e8ce64856431e2",
  "source": "sources/w10-actor-b-tf-9autochest/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9AutoChest",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_42c7b0.o",
  "profile": "authored_cpp",
  "sha256": "250caf2854c7748aca81ed2e517d6961ce4d5255f8b1eec6b0f8e4915339cd75",
  "source": "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c7b0.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_42c7d8.o",
  "profile": "authored_cpp",
  "sha256": "a200adbc62b932679804ed76416963bc266414dd4c63a7b5a133d9b0bccb8442",
  "source": "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c7d8.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_42c800.o",
  "profile": "authored_cpp",
  "sha256": "7a9260abaf8b6bedc0ca5ad90bdf12c1a73aa2a4b2868fec9fe5f1bc2235eba1",
  "source": "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c800.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_42c828.o",
  "profile": "authored_cpp",
  "sha256": "876702947577435cf40b3f8b76f7814f8555fa38b487e9e9eec872f7e3f9d8a8",
  "source": "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c828.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_42c850.o",
  "profile": "authored_cpp",
  "sha256": "0d1e424c22748c4631aec03d2c58d329d0d296dbe65d2d2ded7b0677cf0082ce",
  "source": "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c850.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "norrath_ctors_ant_lantern_view_42c878.o",
  "profile": "authored_cpp",
  "sha256": "4fb6f439b6b15904d96e080cb5969525c2a62964b064029633157d3da3276829",
  "source": "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c878.cpp",
  "emitted_functions": []
 }
]
```

`files` additions:

```json
{
 "sources/w10-actor-b-clone-antqueen/clone.cpp": "fe6905cd4feed3bc5025244d664723c1cf6b840a5c595a8448c48d68d0f5de1a",
 "sources/w10-actor-b-common/fields.hpp": "5b1369edbb6490dac486ad72a1f729c82118327fcdaacdef4d0c917cc10276a3",
 "sources/w10-actor-b-dtor-10arenabeast/dtor.cpp": "3dd7fc48cf8317a7bfcae033261cc5612bdf4768b33ecde65726be7673e61638",
 "sources/w10-actor-b-dtor-10autoobject/dtor.cpp": "f9e95ce457ef847dc18e84be27432cb147cab937c5e3cb56473e75adc1a0d799",
 "sources/w10-actor-b-dtor-13autocontainer/dtor.cpp": "a32bf226770ad7e915242c572cc8bb00d15661b6df0329caa4827fdbe7cb6e04",
 "sources/w10-actor-b-dtor-8antqueen/dtor.cpp": "dd542f5b9799e42c5b087d012a3cc3f0f004b26ccd6be822c316350c2d1594b7",
 "sources/w10-actor-b-dtor-9autochest/dtor.cpp": "0399c90b07d962fb6801f32e88106daaed9faf6b3ef5b4148614e9213096517b",
 "sources/w10-actor-b-msg-draw-antqueen/msg_draw.cpp": "2a0f167fa2adc7782e57314da277f562c8c7cff5a654db5f37db6788984d7881",
 "sources/w10-actor-b-msg-draw-arenabeast/msg_draw.cpp": "2bf3366ea7cec891bc9719c195df18ac9cce281fb38a6528ee5643f3d4098f65",
 "sources/w10-actor-b-msg-hurt-antqueen/msg_hurt.cpp": "e215074b9f02e9de55bc44ad864598e5a6d757c206e262d48287dce3bc1d2328",
 "sources/w10-actor-b-msg-hurt-arenabeast/msg_hurt.cpp": "981de52d22de3b27786ce80afc35c813448b950495e7865c7cc8015b10c2ffc9",
 "sources/w10-actor-b-msg-load-autoobject/msg_load.cpp": "976406fa160e5077e4d7e9c929a0d985b3b21233716e923cfd3f34ef079f0386",
 "sources/w10-actor-b-msg-move-collision-arenabeast/msg_move_collision.cpp": "77ffc922fdb0489e8033604abcbabdc4c86999dfd82d3ebd4802213a2492d652",
 "sources/w10-actor-b-msg-poke-autochest/msg_poke.cpp": "9d6378629290010f7e88ba06df5ce3c5c40f02a33fa65ca4486c18d1c49f7ed9",
 "sources/w10-actor-b-msg-run-antqueen/msg_run.cpp": "eaa93530f94f032fe4c9cfec322f6269e034325b718b10922f840d43739331c3",
 "sources/w10-actor-b-msg-run-arenabeast/msg_run.cpp": "3a497d9610cc4cbfb6d5239e485ca28044eea8348ab29443792e262185d93520",
 "sources/w10-actor-b-msg-run-autochest/msg_run.cpp": "e0b65a8535e259ae71bf8a01f487c4e980b0b633f1d11f65f1d2da13f434eaf5",
 "sources/w10-actor-b-msg-run-autocontainer/msg_run.cpp": "06c393b023117284269de3e20ee71103c414556a89fd90dd9fd50ba854293340",
 "sources/w10-actor-b-msg-trigger-antqueen/msg_trigger.cpp": "d862f590a949ee1a535e8c9e5626d9352fbf9c5f13893140896e539216f0fd2c",
 "sources/w10-actor-b-msg-trigger-arenabeast/msg_trigger.cpp": "52fe05c87bb61ea05f66fcedb5c6d6a954358c1a7749fccf95a9ea02fb22ea32",
 "sources/w10-actor-b-set-state-antqueen/set_state.cpp": "10e7f7396dc0daec74a8e063b7d940e5b659957498c1631a6c0433796c818dd3",
 "sources/w10-actor-b-set-state-arenabeast/set_state.cpp": "eaaad55fe322fe0b71a39fb30a4ce58debcd2a583b902316814552b01268ca15",
 "sources/w10-actor-b-tf-10arenabeast/typeinfo.cpp": "edd32981e88e16710806eba3bf0d2b5ef0022f5dc0d9146929d07da582189177",
 "sources/w10-actor-b-tf-10autoobject/typeinfo.cpp": "83bc9be83f7bd0e2514bf99ec633d033d7e3de2b2ef1da7f60545b26399243c0",
 "sources/w10-actor-b-tf-13autocontainer/typeinfo.cpp": "3180e0da2bf3b2a2ab69b2935179fbc977920ce4cb4695d02064df8494963b97",
 "sources/w10-actor-b-tf-8antqueen/typeinfo.cpp": "a4a841f723e837faaad46ba4b22790f242546924d042ff0370d10a7f9874f8cb",
 "sources/w10-actor-b-tf-9autochest/typeinfo.cpp": "86d0a63d83cb448224d3a22d6f7e2a3b006d62af9cbbac14c7e8ce64856431e2",
 "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c7b0.cpp": "250caf2854c7748aca81ed2e517d6961ce4d5255f8b1eec6b0f8e4915339cd75",
 "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c7d8.cpp": "a200adbc62b932679804ed76416963bc266414dd4c63a7b5a133d9b0bccb8442",
 "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c800.cpp": "7a9260abaf8b6bedc0ca5ad90bdf12c1a73aa2a4b2868fec9fe5f1bc2235eba1",
 "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c828.cpp": "876702947577435cf40b3f8b76f7814f8555fa38b487e9e9eec872f7e3f9d8a8",
 "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c850.cpp": "0d1e424c22748c4631aec03d2c58d329d0d296dbe65d2d2ded7b0677cf0082ce",
 "sources/w10-actor-b-views/norrath_ctors_ant_lantern_view_42c878.cpp": "4fb6f439b6b15904d96e080cb5969525c2a62964b064029633157d3da3276829",
 "sources/w9-rtti-vtables-entities-c-bytes/bytes.hpp": "7ab77f72720c6a549323b5e1a46692c42d4a946022c8ac4eea044874481ded7d"
}
```

## Authored but never executed (outside the private paired runs)

- All of it: no unit ran on a PS2, an emulator, or in-game. The paired runs execute the compiled authored code
  in a private interpreter against the original with stand-in callees only.
- Not reached even in the paired runs: nop/annulled words listed in the commit messages; the second and
  third iterations of the item-effect slot search in `msg_run__8AntQueen` (unreachable because the
  description is initialised with all slots free).
- The six resource maps and the two type-info nodes were checked by host tests and by size only (data
  objects are not executed).
- Callee behaviour (everyFrame, animation, particles, light effects, item creation, world queries) is mocked in
  every test; their real behaviour is outside this package.

## Unknown / blocked

- The two particle definitions at `0x7d7950` and `0x7d79a0` are file-local in the original unit
  (`w8-fx-particles-init-has-light-or-particle-number` defines them as file-local statics). The AutoChest and
  AutoContainer runs import them under the placeholder names
  `norrath_auto_chest_msg_run_fuseParticle_bytes` / `norrath_auto_chest_msg_run_smokeParticle_bytes`; the
  owner of that unit must export them under these names (or the callers be renamed) for the link to close.
- `spawnItems__9AutoChest` (0x21f898), `addTurnAnimation__8AntQueen` (0x2082d8),
  `subTurnAnimation__8AntQueen` (0x208b68), `findEvadeDestination__8AntQueen` (0x2080b0) and the AntWave
  constructor are called but not mine; they were left as imports.
- `__ti10AutoObject`, `__ti9AutoChest` and `__ti13AutoContainer` are defined by
  `w9-startup-crt-b-ti-inline-bases`; that unit's comment says no accessor exists for them, but
  `__tf10AutoObject` (0x314d98, now authored here) is one.
- Block free in the destructors is called as `norrath_block_free` (the name the existing authored
  destructors use and `block-allocator-lifecycle-build` defines); the image calls `blockFree__FPv`, which is
  still unresolved in the plan-10 list. Block allocation is called as `blockAlloc__Fi`.
- Not closed because they were outside the task list: `msg_run__10AutoObject`, `msg_save__*`, the other
  AntQueen methods named above.
