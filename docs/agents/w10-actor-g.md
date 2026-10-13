# w10-actor-g: spell, skill and actor virtuals (Generator to Goblin)

Package `w10-actor-g`, branch `agents/w10-actor-g`. Private reference: the original executable
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses
and sizes are that file's symbol table. No game bytes or disassembly are committed. Everything was
compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here ran on a PS2 or in the game;
"paired" means the authored object was run next to the original function inside a private R5900
interpreter (kept outside git).

## How each function was checked

- Paired run (private): compile the unit, link it with every undefined symbol bound to the original's
  address, then run the original and the authored function from identical memory snapshots with
  identical Python stub callees. Compared: the ordered callee log (argument registers, float
  registers as bits, the bytes behind pointer arguments, stack argument words, and a hash of the
  object at every call, which catches store-order differences), the return value where the original
  defines one, and every changed byte outside the stack. Instruction coverage of the original was
  measured; uncovered words are listed per function in the commit messages.
- Mutations: constants, offsets, masks and boundaries were changed one at a time in copies of most
  units and the comparison was confirmed to fail. Where a first mutation was not detected, scenarios
  were added until it was (Goblin `setState`, Goblin `msg_run`, Ghoul `setState`, Ghoul `msg_run`).
- Host contract (`host_check.cpp` in each directory, not a build input): the expected behaviour as
  stub-callee sequences and field values. Units reading 32-bit pointer words use
  `W10_ACTOR_G_HOST` (see `w10-actor-g-common/words.hpp`), which turns words into offsets from a host
  base.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- One partial build of the unmodified plan (3474 inputs, 1332 unresolved imports) confirmed that all
  32 target names were unresolved before this work. My units were not added to a plan; they were each
  compiled alone and, via `ld -r`, have no duplicate definitions among themselves.
- Findings from the pairing worth knowing: a `jal` delay slot copies the previous call's result, so
  the frame flags in the Gnome, Ghoul and Goblin `msg_run` come from `creatureAdvanceAnimation`, not
  from `slideBack`; the Gnome and Goblin type tests call the class-table word at +4 with no
  arguments; floats such as 0.1 and 0.3 appear in the image as the truncated bits `0x3dcccccc` and
  `0x3e999999` and are written as hex floats; Goblin `setState` clears bit 9 of the 64-bit field at
  +160 before its jump table, so the branch that tests that bit in state 8 cannot run.

## Functions

| directory | symbol | original (address, bytes) | compiled bytes |
|---|---|---|---|
| `w10-actor-g-clone-ghoul` | `clone__5Ghoul` | 0x11b1d0, 1372 | 820 |
| `w10-actor-g-clone-gnome` | `clone__5Gnome` | 0x11b730, 1384 | 828 |
| `w10-actor-g-clone-goblin` | `clone__6Goblin` | 0x11bc98, 1652 | 864 |
| `w10-actor-g-ctor-glowing-cylinder` | `__15GlowingCylinderG6Point3iiii` | 0x1e5828, 352 | 272 |
| `w10-actor-g-dtor-ghoul` | `_$_5Ghoul` | 0x30f0a8, 100 | 88 |
| `w10-actor-g-dtor-gnome` | `_$_5Gnome` | 0x30f160, 108 | 112 |
| `w10-actor-g-dtor-goblin` | `_$_6Goblin` | 0x30f220, 108 | 112 |
| `w10-actor-g-get-miniboss-name-6goblin` | `getMinibossName__6Goblin` | 0x22f530, 60 | 56 |
| `w10-actor-g-init-miniboss-6goblin` | `initMiniboss__6Goblin` | 0x22f4f8, 56 | 96 |
| `w10-actor-g-msg-alert-5ghoul` | `msg_alert__5GhoulifG6Point3P10GameObject` | 0x1f75e0, 188 | 188 |
| `w10-actor-g-msg-alert-6goblin` | `msg_alert__6GoblinifG6Point3P10GameObject` | 0x22ef80, 340 | 392 |
| `w10-actor-g-msg-collision-5gnome` | `msg_collision__5GnomeP10GameObject` | 0x202488, 256 | 352 |
| `w10-actor-g-msg-collision-world-6goblin` | `msg_collisionWorld__6GoblinG6Point3` | 0x22f1d8, 100 | 72 |
| `w10-actor-g-msg-draw-5ghoul` | `msg_draw__5Ghoul` | 0x1f75b8, 40 | 20 |
| `w10-actor-g-msg-draw-5gnome` | `msg_draw__5Gnome` | 0x2013d0, 472 | 560 |
| `w10-actor-g-msg-draw-6goblin` | `msg_draw__6Goblin` | 0x22c808, 400 | 448 |
| `w10-actor-g-msg-load-6goblin` | `msg_load__6GoblinPc` | 0x22f148, 144 | 120 |
| `w10-actor-g-msg-move-collision-5gnome` | `msg_moveCollision__5GnomePP10GameObject` | 0x202588, 236 | 264 |
| `w10-actor-g-msg-move-collision-6goblin` | `msg_moveCollision__6GoblinPP10GameObject` | 0x22f240, 268 | 244 |
| `w10-actor-g-msg-run-5ghoul` | `msg_run__5Ghoul` | 0x1f6640, 2012 | 2360 |
| `w10-actor-g-msg-run-5gnome` | `msg_run__5Gnome` | 0x2015a8, 1016 | 1080 |
| `w10-actor-g-msg-run-6goblin` | `msg_run__6Goblin` | 0x22c998, 5264 | 5040 |
| `w10-actor-g-msg-save-6goblin` | `msg_save__6GoblinPc` | 0x22f0d8, 108 | 80 |
| `w10-actor-g-msg-save-9generator` | `msg_save__9GeneratorPc` | 0x1f2088, 36 | 24 |
| `w10-actor-g-msg-trigger-9generator` | `msg_trigger__9Generatori` | 0x1f1ff8, 140 | 136 |
| `w10-actor-g-set-state-5ghoul` | `setState__5Ghouli` | 0x1f6e20, 1624 | 1836 |
| `w10-actor-g-set-state-5gnome` | `setState__5Gnomei` | 0x2019a0, 764 | 952 |
| `w10-actor-g-set-state-6goblin` | `setState__6Goblini` | 0x22de28, 2412 | 2648 |
| `w10-actor-g-set-state-9generator` | `setState__9Generatori` | 0x1f20d8, 112 | 96 |
| `w10-actor-g-tf-ghoul` | `__tf5Ghoul` | 0x30f110, 80 | 92 |
| `w10-actor-g-tf-gnome` | `__tf5Gnome` | 0x30f1d0, 80 | 92 |
| `w10-actor-g-tf-goblin` | `__tf6Goblin` | 0x30f290, 80 | 92 |

## Observed, inferred, unknown

Observed: every offset, constant, call order, branch and jump-table slot used in the sources comes
from the original function bodies. Jump tables: Generator `setState` 11 entries, Ghoul `setState` 18
and `msg_run` 18, Gnome `setState` 14 and `msg_run` 14, Goblin `setState` 17 and `msg_run` 16. Text
literals used (all short identifiers read from the image): `MBGoblin%d`, `exclamation`,
`exclamationShort`, `icons.lmp`, `exclaim.tex`, `ghoulflesh`, `dead`, `arrows`, `fx.lmp`,
`swsh_arrow%d.anm`, `spel_he_heal.anm`.

Inferred (from symbol names and shapes only): that `_$_` functions are destructors with bit 0 of the
flag meaning "free", that `__tf` functions return a runtime type node whose single base is Creature,
that the word at class table +4 is a type-node function, that the +184 table entry is a `setState`
setter, that the +72 word is the class table, that Goblin's `getMinibossName` formats a miniboss
number kept at +76, and that the weapon map's second words are filled with loaded resources.

Unknown: the meaning of nearly every object field (+256, +576, +960..+988, +972, +992, +996, +1136,
+1140 and the state numbers), the units of the angle tests, what the +168/+176 table entries do, and
the real names of the data globals used by position only. Whether `cutMode`, `gameCam`, `frameNm`,
`worldViewMatrix` are the intended types (they are read as signed byte, word, int and byte array).

## Imports closed (32)

`msg_save__9GeneratorPc`, `msg_trigger__9Generatori`, `setState__9Generatori`, `_$_5Ghoul`,
`__tf5Ghoul`, `clone__5Ghoul`, `msg_alert__5GhoulifG6Point3P10GameObject`, `msg_draw__5Ghoul`,
`msg_run__5Ghoul`, `setState__5Ghouli`, `__15GlowingCylinderG6Point3iiii`, `_$_5Gnome`, `__tf5Gnome`,
`clone__5Gnome`, `msg_collision__5GnomeP10GameObject`, `msg_draw__5Gnome`,
`msg_moveCollision__5GnomePP10GameObject`, `msg_run__5Gnome`, `setState__5Gnomei`, `_$_6Goblin`,
`__tf6Goblin`, `clone__6Goblin`, `getMinibossName__6Goblin`, `initMiniboss__6Goblin`,
`msg_alert__6GoblinifG6Point3P10GameObject`, `msg_collisionWorld__6GoblinG6Point3`,
`msg_draw__6Goblin`, `msg_load__6GoblinPc`, `msg_moveCollision__6GoblinPP10GameObject`,
`msg_run__6Goblin`, `msg_save__6GoblinPc`, `setState__6Goblini`.

## Definitions opened

The three `__tf` units also define the 12-byte type nodes `__ti5Ghoul`, `__ti5Gnome` and `__ti6Goblin`
(the same pattern as the Ant node). `getMinibossName__6Goblin` has a file-local 16-byte buffer. If
another package defines those type nodes there will be a duplicate to resolve.

## Imports opened or still open

Symbols my sources reference that are still unresolved in the unmodified plan (not mine to close):

- `P_ExtendTrail__FiG6Point3`
- `P_StartTrail__Fiifiiiii`
- `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`
- `__15AnimationState8RC15AnimationState8`
- `blockFree__FPv`
- `dropTreasure__8CreatureG6Point3iffffffPc`
- `findTagInt__10GameObjectPci`
- `floorAdd__FG6Point3sfii`
- `objectCreateByName__FPcG6Point3sPPc`
- `skills`

All other symbols my sources reference are already defined in the unmodified plan (checked against
`unresolved-imports.json`). Notes on them: `cutMode`, `gameCam`, `frameNm`, `worldDifficulty`,
`skills` and `worldViewMatrix` are read as data; the Goblin weapon map is linked through the authored
name `norrath_ctors_ant_lantern_view_42cfc8` (an asm label in `msg_draw.cpp`), and `_4item$globalItem`
through its asm label. The shared header `w10-actor-g-common/words.hpp` is required by 15 units.

## Build-plan entries needed

Append these 32 authored inputs with fresh `input_order` values; `default_authored_inputs` grows by 32.
Every unit uses profile `authored_cpp`. `host_check.cpp` files are not inputs. Units that include
`words.hpp` list it in `header_closure` (the plan's file table also needs that header). `sha256` is
of the source file as committed; body sizes are from the reviewed build and not compared with the
original; `emitted_functions` includes file-local helpers.

```json
{"emitted_functions":[{"compiled_body_bytes":820,"name":"clone__5Ghoul"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"907f0d4514c6bd78ac31c24054144e0b3a1c07e98b0906fd33d77d5f61cbdd92","source":"sources/w10-actor-g-clone-ghoul/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":828,"name":"clone__5Gnome"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"846d548632bd6fb4d8e5cad6af142109f9ae665bf3a43d637cbf69da804e9a90","source":"sources/w10-actor-g-clone-gnome/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":864,"name":"clone__6Goblin"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"a459e2ae81efad82affe9effa4ebe869ee34d972d0eb5d9b386dbdc5d2ed233c","source":"sources/w10-actor-g-clone-goblin/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":272,"name":"__15GlowingCylinderG6Point3iiii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"glowing_cylinder_ctor.o","profile":"authored_cpp","sha256":"054da26fbb28a3a17dc77f2cd541ac4b18a7070fbdeabf2118ccf2e48425be7a","source":"sources/w10-actor-g-ctor-glowing-cylinder/glowing_cylinder_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_5Ghoul"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"b933de886d135f1b2a2ed7e64f51f4f5d0e7a451987cdcac9d462bb67bc408f9","source":"sources/w10-actor-g-dtor-ghoul/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_$_5Gnome"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"9c9104cf5a888eba174f86faec11fc9f8b8750a33748386b01a1ac4a8d62aa53","source":"sources/w10-actor-g-dtor-gnome/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_$_6Goblin"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"4f764d3ead46cc84e65386710884ddb191609d38b21d919474e01435f3a0f59b","source":"sources/w10-actor-g-dtor-goblin/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"getMinibossName__6Goblin"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_miniboss_name.o","profile":"authored_cpp","sha256":"2c441542e819279a55574de55cf60c8e39ea1f9e3c32aef10edfc1d5492a535e","source":"sources/w10-actor-g-get-miniboss-name-6goblin/get_miniboss_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"initMiniboss__6Goblin"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_miniboss.o","profile":"authored_cpp","sha256":"03c009c104de4254aa38a6d40ce553bdcdc09844c917471677d3e1238fecf1a6","source":"sources/w10-actor-g-init-miniboss-6goblin/init_miniboss.cpp"}
{"emitted_functions":[{"compiled_body_bytes":188,"name":"msg_alert__5GhoulifG6Point3P10GameObject"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_alert.o","profile":"authored_cpp","sha256":"a3d57ca132019ac02480aec2a1afda51735f836e2747ba377ed354badff0ae41","source":"sources/w10-actor-g-msg-alert-5ghoul/msg_alert.cpp"}
{"emitted_functions":[{"compiled_body_bytes":392,"name":"msg_alert__6GoblinifG6Point3P10GameObject"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_alert.o","profile":"authored_cpp","sha256":"daaaa082e1577ffc0eff2dfd5225c3faef7e8574b30045c7e32e3ce6ece95aff","source":"sources/w10-actor-g-msg-alert-6goblin/msg_alert.cpp"}
{"emitted_functions":[{"compiled_body_bytes":352,"name":"msg_collision__5GnomeP10GameObject"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_collision.o","profile":"authored_cpp","sha256":"d4f6977d944735e869560acf1693866f701eea0ceed7af058464d7fa7c3a096b","source":"sources/w10-actor-g-msg-collision-5gnome/msg_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":72,"name":"msg_collisionWorld__6GoblinG6Point3"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_collision_world.o","profile":"authored_cpp","sha256":"7c01ea5384db30b813cff4b32b4a3dc64fa3ab008c2459182381307b8af138fc","source":"sources/w10-actor-g-msg-collision-world-6goblin/msg_collision_world.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"msg_draw__5Ghoul"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"1014d4f764e95cb3966ec6080e9f7e415950b581138ccd17c00a1e68a8d43323","source":"sources/w10-actor-g-msg-draw-5ghoul/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":560,"name":"msg_draw__5Gnome"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"1e0a64cdfb4bc3d4d8a7113a32d9d9fc1216144179d5b2607bf544a989f2a4f4","source":"sources/w10-actor-g-msg-draw-5gnome/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":448,"name":"msg_draw__6Goblin"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"feb17ea3edcc37be36780f398345599c15b4c0acb2b1c53b696c8cce19feefa2","source":"sources/w10-actor-g-msg-draw-6goblin/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"msg_load__6GoblinPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"f73bffcb49c9bfc3aa30fc68934a2edbba7b5c663e47a1a426b97a096ad03b34","source":"sources/w10-actor-g-msg-load-6goblin/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":264,"name":"msg_moveCollision__5GnomePP10GameObject"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_move_collision.o","profile":"authored_cpp","sha256":"4831ea4cc8eaa8c924dd19aab0ccb847b5508fbfd1c0e3bd8b2a07b4d4665df2","source":"sources/w10-actor-g-msg-move-collision-5gnome/msg_move_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":244,"name":"msg_moveCollision__6GoblinPP10GameObject"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_move_collision.o","profile":"authored_cpp","sha256":"d41d9980226b93dd8978b89053164b0eecc090adb9bbbf5f9eb036031fb93303","source":"sources/w10-actor-g-msg-move-collision-6goblin/msg_move_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2360,"name":"msg_run__5Ghoul"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"55031638c55251142ea726bd8333e7455e887c50757a5afcb6bf7359d053f9d1","source":"sources/w10-actor-g-msg-run-5ghoul/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1080,"name":"msg_run__5Gnome"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"2aa910d2dad612ad21775d3aaf77adfebfc0a95d8cff65474b39b8c91dd72b11","source":"sources/w10-actor-g-msg-run-5gnome/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":280,"name":"_ZN12_GLOBAL__N_1L12wander_testsEPhi"},{"compiled_body_bytes":5040,"name":"msg_run__6Goblin"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"afd7b05d1524bc23e7e1f42dd9e1cdc4bf5ffd2b75e6563ac7e5c41ad27207f2","source":"sources/w10-actor-g-msg-run-6goblin/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":80,"name":"msg_save__6GoblinPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_save.o","profile":"authored_cpp","sha256":"7674db42295509822389693c2de94185893138f9a713cdb1339385e7b026b107","source":"sources/w10-actor-g-msg-save-6goblin/msg_save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":24,"name":"msg_save__9GeneratorPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_save.o","profile":"authored_cpp","sha256":"66f4ea4a610f10ab5ee688678f3389c4d296aacb7bfd48dfba6f66620bae0288","source":"sources/w10-actor-g-msg-save-9generator/msg_save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":136,"name":"msg_trigger__9Generatori"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_trigger.o","profile":"authored_cpp","sha256":"355cc905031a25c6ff26debb249ecd74b250acf1e6256f5dab62eab05842580a","source":"sources/w10-actor-g-msg-trigger-9generator/msg_trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1836,"name":"setState__5Ghouli"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"5c39bdb4f6cb721a358a03d66b55cf1c19f7925cc1db2edc40b4e06d69770665","source":"sources/w10-actor-g-set-state-5ghoul/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":952,"name":"setState__5Gnomei"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"87692efd59c095ec443981e65d5a42443699e6b8ce94fb14f802ecb0cdc22bf4","source":"sources/w10-actor-g-set-state-5gnome/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2648,"name":"setState__6Goblini"}],"header_closure":["sources/w10-actor-g-common/words.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"a09becf0abcfd661cd92e8e56555d48faac023466775f7a7936f97b6f2c7c725","source":"sources/w10-actor-g-set-state-6goblin/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"setState__9Generatori"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"95baafecfc63b6eaa17faf1def6124dc511302f054c3a11df00d773c57ac313d","source":"sources/w10-actor-g-set-state-9generator/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf5Ghoul"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"ddd70b4735086ef44dfd4efb0e7c435c66bcec4a6eddfe62ce3402cf00ec0481","source":"sources/w10-actor-g-tf-ghoul/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf5Gnome"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"f046b282fdc9004816ed335a3f2fd2df60da0d1dab02c1654381c14915775d7b","source":"sources/w10-actor-g-tf-gnome/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf6Goblin"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"667d33de4df8cf8fc64db2ed48abba7637d0fa1354add1d70e6b6a25c7985d82","source":"sources/w10-actor-g-tf-goblin/typeinfo.cpp"}
```

## Authored but never executed

All of it was executed only in the private interpreter and in host contracts. Never run on hardware or
inside the game. Specifically unreachable in the paired runs: the Goblin `setState` state-8 branch that
tests bit 9 of +160 (the function clears that bit first), four words of impossible switch fallbacks in
Goblin `msg_run`, and padding nops.

## Unknown / blocked

Nothing was blocked. Not done: no byte-for-byte match is claimed for any function; compiled sizes
differ from the originals. Struct and field names are deliberately not proposed.
