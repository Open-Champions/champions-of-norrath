# w14-projectile-b: projectile, spell and monster handlers (part b)

Branch `agents/w14-projectile-b`. Private reference executable `work/private/SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses and sizes below are that image's own symbol
values. Nothing from the image (bytes, disassembly, decompiler output) is committed. Nothing here ran on the EE or in the game.

## What was done

The package listed 26 unresolved imports. 25 are now authored (25 units, 28 function bodies counting three file-local helpers);
`msg_run__6Player` was not attempted (see "Blocked"). Every unit is its own commit with a host contract (`host_check.cpp`, not a
build input) that fails when a branch, offset, constant, call order or argument is wrong.

Two kinds of unit:

- **New work** (10): the five `terminate__*Projectilei` end-of-flight routines, `msg_run__13FireBombletJr`, `startFractalLine__FP4Boltfi`,
  `run__5SpellG6Point3s`, `msg_draw__16beetleProjectile` (two stubs with empty bodies) and `waterStep` (hand-written multimedia code,
  re-expressed as plain C++ with saturating 16-bit lanes).
- **Carried over and given a check** (15): the w13-spell-msg-a sources for `msg_run__10MummySnake`, `10PoisonRain`, `10SpawnMummy`,
  `11BoltGoRound`, `11DiseaseBolt`, `12ShockOfFrost`, `14HateProjectile`, `14poisonGasSpell`, `15FrostStormSpell`, `15ShockProjectile`,
  `16PoisonProjectile`, `16beetleProjectile`, `msg_draw__16PoisonProjectile`, `run__14SpellHarmTouch` and `run__15SpellConeOfFire`. The lead integrator did not select those
  w13-spell-msg-a units (no committed check), so their imports stayed open. The sources here are copies with the minimum changes needed
  for a host check (a division fallback for the `rsqrt.s` instruction under `#if defined(__mips__)`, a host route for three table calls in
  a helper header, and an include path), and each was re-run against the original in the private paired run before it was committed.
  **Select either the w13-spell-msg-a unit or the w14-projectile-b unit for a symbol, never both** (they define the same function).

## Method and checks

- Each function was read privately, rewritten as C++ in my own words and compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0).
  Every unit compiled. Sizes differ from the image; no byte match is claimed.
- **Host contracts**: every unit has `host_check.cpp`, built with the host C++ compiler. Stored 32-bit words are offsets inside an arena
  and table calls go through a logging hook; callees are recording stubs. All 25 exit 0 (re-run after the last source commit).
- **Paired runs** (private, outside git): a small R5900 interpreter loaded the original function and the freshly compiled authored one, bound
  every callee to a Python stub with identical behaviour, ran both from identical memory snapshots and required equal call logs (argument
  registers, float registers, bytes behind pointer arguments) and equal changed bytes outside the stack. The table gives scenarios,
  executed original words and hand mutations caught (a constant off by one, a flipped compare, a changed count, a wrong offset). Runs were
  checked for `HALT` lines. For `waterStep` the interpreter was extended with the 128-bit multimedia instructions the original uses; for
  the cone-of-fire routine with `mult1`.
- Floats: the interpreter rounds to nearest while the EE truncates, so scenarios use values where this does not matter; the survivors of
  the mutation runs are mostly one-ulp changes of constants that random data cannot separate.

## Units

| unit | symbol | image address, size | compiled bytes | scenarios | original words executed | mutations caught | commit |
|---|---|---|---|---|---|---|---|
| `w14-projectile-b-start-fractal-line` | `startFractalLine__FP4Boltfi` | `0x001842b8`, 504 | 528 | 48 | 126/126 | 7/7 | `a3acec7b` |
| `w14-projectile-b-msg-run-poisongasspell` | `msg_run__14poisonGasSpell` | `0x001e0ce0`, 1196 | 1136 | 400 | 292/299 | 11/11 | `5553149e` |
| `w14-projectile-b-msg-run-froststormspell` | `msg_run__15FrostStormSpell` | `0x001e23f8`, 3596 | 3340 | 150 | 892/899 | 14/16 | `28dbd1f0` |
| `w14-projectile-b-msg-run-boltgoround` | `msg_run__11BoltGoRound` | `0x001e9528`, 1000 | 952 | 500 | 242/250 | 8/9 | `4537e16a` |
| `w14-projectile-b-msg-run-hateprojectile` | `msg_run__14HateProjectile` | `0x001fd9a8`, 3268 | 2888 | 120 | 812/817 | 16/17 | `13e3fcdc` |
| `w14-projectile-b-msg-run-poisonprojectile` | `msg_run__16PoisonProjectile` | `0x0020f390`, 908 | 956 | 300 | 227/227 | 8/9 | `55ccbe1e` |
| `w14-projectile-b-msg-draw-poisonprojectile` | `msg_draw__16PoisonProjectile` | `0x0020fd48`, 200 | 188 | 11 | 50/50 | 5/5 | `eb112042` |
| `w14-projectile-b-msg-run-beetleprojectile` | `msg_run__16beetleProjectile` | `0x00211f70`, 1204 | 1216 | 300 | 301/301 | 11/11 | `ce0922f4` |
| `w14-projectile-b-msg-draw-beetleprojectile` | `msg_draw__16beetleProjectile` | `0x00212618`, 8 | 8 | 4 | n/a (empty body) | n/a | `a852c278` |
| `w14-projectile-b-msg-run-spawnmummy` | `msg_run__10SpawnMummy` | `0x0023b588`, 312 | 384 | 200 | 78/78 | 8/8 | `60388cd3` |
| `w14-projectile-b-msg-run-mummysnake` | `msg_run__10MummySnake` | `0x0023b6c0`, 1448 | 1352 | 600 | 352/362 | 11/11 | `f1a83859` |
| `w14-projectile-b-msg-run-firebombletjr` | `msg_run__13FireBombletJr` | `0x0028b0b8`, 944 | 1096 | 2880 | 228/236 | 11/11 | `0f125187` |
| `w14-projectile-b-msg-run-shockprojectile` | `msg_run__15ShockProjectile` | `0x0028c910`, 3004 | 2412 | 120 | 740/751 | 19/21 | `6df0ed27` |
| `w14-projectile-b-msg-run-diseasebolt` | `msg_run__11DiseaseBolt` | `0x00292358`, 3476 | 904 | 200 | 868/869 | 9/10 | `d5fbd95c` |
| `w14-projectile-b-run-spellharmtouch` | `run__14SpellHarmTouchG6Point3s` | `0x0029b170`, 168 | 176 | 120 | 42/42 | 6/6 | `64fbf655` |
| `w14-projectile-b-terminate-fire-arrow` | `terminate__19FireArrowProjectilei` | `0x0029e5e0`, 196 | 208 | 16 | 49/49 | 4/4 | `da3e8027` |
| `w14-projectile-b-terminate-cold-arrow` | `terminate__19ColdArrowProjectilei` | `0x0029f368`, 1676 | 1352 | 92 | 418/419 | 7/8 | `dd565ecb` |
| `w14-projectile-b-terminate-poison-arrow` | `terminate__21PoisonArrowProjectilei` | `0x002a0888`, 424 | 396 | 72 | 106/106 | 6/6 | `ffa82aaf` |
| `w14-projectile-b-terminate-multifire` | `terminate__19MultiFireProjectilei` | `0x002a1a10`, 288 | 268 | 150 | 72/72 | 4/4 | `9d97ea8e` |
| `w14-projectile-b-terminate-explode-arrow` | `terminate__22ExplodeArrowProjectilei` | `0x002a2388`, 572 | 488 | 360 | 143/143 | 5/5 | `80cb131e` |
| `w14-projectile-b-run-spellconeoffire` | `run__15SpellConeOfFireG6Point3s` | `0x002b5ad8`, 3248 | 1148 | 150 | 805/812 | 17/17 | `03624fb2` |
| `w14-projectile-b-msg-run-shockoffrost` | `msg_run__12ShockOfFrost` | `0x002bca20`, 3548 | 1724 | 200 | 887/887 | 15/15 | `de736bd9` |
| `w14-projectile-b-msg-run-poisonrain` | `msg_run__10PoisonRain` | `0x002be6d0`, 1708 | 1804 | 300 | 420/427 | 12/14 | `56660076` |
| `w14-projectile-b-run-5spell` | `run__5SpellG6Point3s` | `0x003149f8`, 36 | 8 | 4 | n/a (empty body) | n/a | `543688d3` |
| `w14-projectile-b-water-step` | `waterStep` | `0x00187798`, 704 (to 0x00187a58) | 1528 | 30 | 175/176 | 11/11 | `be62caf8` |

Helper symbols the compiler keeps as local functions (not imports): `burst` (DiseaseBolt, ShockOfFrost) and `spray` (SpellConeOfFire).
`waterStep` has no size in the symbol table; its code runs from 0x00187798 to 0x00187a58 (the labels `waterLoop` and `bailo` sit inside it).

Shared helper headers (new): `sources/w14-projectile-b-common/spell.hpp` (the w13-spell-msg-a helpers, with the three three-argument table calls
routed through a host hook only under `W12_SPELL_C_HOST`) and `sources/w14-projectile-b-common/access.hpp` (the w12-actor-spell-c accessors; in host
mode the zero word maps to null, so list walks that end on a zero word end in a host contract). The terminate units and the FireBombletJr unit
include the existing `sources/w13-spell-msg-b-common/access.hpp`.

## Findings that shaped the sources (observed)

- The second argument of the terminate routines is tested only against zero (and, for the fire and cold arrows, against 1); the poison arrow
  ignores it. All five check the done word at +96 first and set it to 1 last.
- The cold arrow's second particle loop passes the object's own position (`O+16`) rather than the position it computes; the computed
  position is stored and never used. Authored as observed.
- `msg_run__13FireBombletJr` is a smaller sibling of `msg_run__11FireBomblet`: attack amounts 1.0 and 1.0, no sound id, no spawned children;
  the halfword at +92 is halved only on a bounce.
- `startFractalLine` takes six generator steps in one block and stores the final state once; an earlier version that kept the six draws in
  an array compiled to code that did not match the original in the paired run and was rewritten with explicit variables (the cause of the
  miscompile was not investigated).
- In `msg_run__11DiseaseBolt` and `msg_run__12ShockOfFrost` the constant bits 0x43af0000 are 350.0, not the 351.0 the w13 comments said; the
  copies here carry the corrected comments (code unchanged).

## Imports closed

`msg_draw__16PoisonProjectile`, `msg_draw__16beetleProjectile`, `msg_run__10MummySnake`, `msg_run__10PoisonRain`, `msg_run__10SpawnMummy`, `msg_run__11BoltGoRound`, `msg_run__11DiseaseBolt`, `msg_run__12ShockOfFrost`, `msg_run__13FireBombletJr`, `msg_run__14HateProjectile`, `msg_run__14poisonGasSpell`, `msg_run__15FrostStormSpell`, `msg_run__15ShockProjectile`, `msg_run__16PoisonProjectile`, `msg_run__16beetleProjectile`, `run__14SpellHarmTouchG6Point3s`, `run__15SpellConeOfFireG6Point3s`, `run__5SpellG6Point3s`, `startFractalLine__FP4Boltfi`, `terminate__19ColdArrowProjectilei`, `terminate__19FireArrowProjectilei`, `terminate__19MultiFireProjectilei`, `terminate__21PoisonArrowProjectilei`, `terminate__22ExplodeArrowProjectilei`, `waterStep`

(Measured on a private copy of the shared plan with these 25 entries appended: 957 unresolved before, 963 after; closed 25, opened 31.)

## Imports opened

`__5MummyG6Point3sPPcPc`, `__9LooseFireG6Point3P10GameObject`, `_vt$10FrostShard`, `_vt$18FrostStormIceShard`, `bounce__13FireBombletJr`, `buildShockChainList__14HateProjectileP6Point3`, `buildShockChainList__15ShockProjectileP6Point3`, `norrath_w13_spell_msg_a_view_7d6970`, `norrath_w13_spell_msg_a_view_7d6a10`, `norrath_w13_spell_msg_a_view_7d6a60`, `norrath_w13_spell_msg_a_view_7d6b00`, `norrath_w13_spell_msg_a_view_7d6fc0`, `norrath_w13_spell_msg_a_view_7d7010`, `norrath_w13_spell_msg_a_view_7d7260`, `norrath_w13_spell_msg_a_view_7d72b0`, `norrath_w13_spell_msg_a_view_7d7300`, `norrath_w13_spell_msg_a_view_8d6320`, `norrath_w13_spell_msg_a_view_8d6370`, `norrath_w13_spell_msg_a_view_8d67d0`, `norrath_w13_spell_msg_a_view_8d6820`, `norrath_w13_spell_msg_a_view_8d6870`, `norrath_w13_spell_msg_a_view_8d7b30`, `norrath_w13_spell_msg_a_view_8d7b80`, `norrath_w13_spell_msg_a_view_8d7bd0`, `norrath_w13_spell_msg_a_view_8d80a0`, `norrath_w13_spell_msg_a_view_8d80f0`, `norrath_w13_spell_msg_a_view_8d8140`, `norrath_w13_spell_msg_a_view_8d8190`, `norrath_w13_spell_msg_a_view_8d81e0`, `pack`, `poisonPoof`

## Build-plan entries needed

Profile `authored_cpp`. The lead integrator assigns `input_order`, adds the object hashes to the snapshot, and lists every file below in the
snapshot file list (`host_check.cpp` files are not plan inputs). `docs/agents/w14-projectile-b-plan-entries.py --toolchain <ee> [--private-root DIR]`
prints the entries below and writes a private plan/snapshot copy (nothing is written inside the repository).

```json
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_draw__16beetleProjectile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pb_msg_draw_beetleprojectile.o", "profile": "authored_cpp", "sha256": "47fb5f9c6290a1b7f4ba581604c0317266ecf78e37c7d7c3116092ca02c09645", "source": "sources/w14-projectile-b-msg-draw-beetleprojectile/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 188, "name": "msg_draw__16PoisonProjectile"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_draw_poisonprojectile.o", "profile": "authored_cpp", "sha256": "9ca4ba73c0355f96908a478da2496905ea3550723c2dd739aa4697e6ca8df1f4", "source": "sources/w14-projectile-b-msg-draw-poisonprojectile/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1216, "name": "msg_run__16beetleProjectile"}], "header_closure": ["sources/w13-spell-msg-a-common/spell.hpp", "sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_beetleprojectile.o", "profile": "authored_cpp", "sha256": "edb33900fa41dc2b561190004d7444d156e2d482c2c7d6ebd87bfca553a46fab", "source": "sources/w14-projectile-b-msg-run-beetleprojectile/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 952, "name": "msg_run__11BoltGoRound"}], "header_closure": ["sources/w13-spell-msg-a-common/spell.hpp", "sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_boltgoround.o", "profile": "authored_cpp", "sha256": "4f8f91bd6b2ae6427c4a1d6c34517a8e69d9c80466f2678579141828d01717ae", "source": "sources/w14-projectile-b-msg-run-boltgoround/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 472, "name": "_ZL5burstPhPvff"}, {"compiled_body_bytes": 904, "name": "msg_run__11DiseaseBolt"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_diseasebolt.o", "profile": "authored_cpp", "sha256": "a4f701eb5d9e0c167a34a99857a2a215caf19c40c41aaed74f8e694b9138a5e9", "source": "sources/w14-projectile-b-msg-run-diseasebolt/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1096, "name": "msg_run__13FireBombletJr"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_firebombletjr.o", "profile": "authored_cpp", "sha256": "aa6908def9dc5e3daed87f49d367f136473a57f291a7becaabfe0a18e763a494", "source": "sources/w14-projectile-b-msg-run-firebombletjr/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3340, "name": "msg_run__15FrostStormSpell"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_froststormspell.o", "profile": "authored_cpp", "sha256": "a38880100ba9f1db20de870fe04b14622fc7e8aadbbbce96e236b760c89fd815", "source": "sources/w14-projectile-b-msg-run-froststormspell/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2888, "name": "msg_run__14HateProjectile"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_hateprojectile.o", "profile": "authored_cpp", "sha256": "af14e413f183266a62d6a2ea2f4eb54e73b30bd5dd27d8b6a18c47bdd0083a5a", "source": "sources/w14-projectile-b-msg-run-hateprojectile/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1352, "name": "msg_run__10MummySnake"}], "header_closure": ["sources/w13-spell-msg-a-common/spell.hpp", "sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_mummysnake.o", "profile": "authored_cpp", "sha256": "83de642f3dbb49aae0978fc66593783409de4667b8d2d3a2199376e87724eafa", "source": "sources/w14-projectile-b-msg-run-mummysnake/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1136, "name": "msg_run__14poisonGasSpell"}], "header_closure": ["sources/w13-spell-msg-a-common/spell.hpp", "sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_poisongasspell.o", "profile": "authored_cpp", "sha256": "3fb43fb33bbefffbed86e0e93a8163a7a2fc65690e44c07a7ef2df3002e573ab", "source": "sources/w14-projectile-b-msg-run-poisongasspell/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 956, "name": "msg_run__16PoisonProjectile"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_poisonprojectile.o", "profile": "authored_cpp", "sha256": "a0ea19b9cfb8b1378de99864ad79d2c71ddfdbab597b8264506bdda0029d6657", "source": "sources/w14-projectile-b-msg-run-poisonprojectile/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1804, "name": "msg_run__10PoisonRain"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_poisonrain.o", "profile": "authored_cpp", "sha256": "8d97f0d3c95446d2d91cd0c560043aa533c1aa42e119584608118e7e60ec7693", "source": "sources/w14-projectile-b-msg-run-poisonrain/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 584, "name": "_ZL5burstPhPvfbPKff"}, {"compiled_body_bytes": 1724, "name": "msg_run__12ShockOfFrost"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_shockoffrost.o", "profile": "authored_cpp", "sha256": "d80fb46ca66d7f4a5f1a5fa5cbbabba986eaf7e67d10b866a959dedb60ae94fe", "source": "sources/w14-projectile-b-msg-run-shockoffrost/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2412, "name": "msg_run__15ShockProjectile"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_shockprojectile.o", "profile": "authored_cpp", "sha256": "6997f800c092730cb7c0e0ebc6d6e7a53ab397f745622c52763bc3cf835f9040", "source": "sources/w14-projectile-b-msg-run-shockprojectile/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 384, "name": "msg_run__10SpawnMummy"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_msg_run_spawnmummy.o", "profile": "authored_cpp", "sha256": "ba31f74ff15637200d819eee1bf277331ea90426faa8e102401b22980cbb3452", "source": "sources/w14-projectile-b-msg-run-spawnmummy/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "run__5SpellG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pb_run_5spell.o", "profile": "authored_cpp", "sha256": "37826e9bd9cfef8ecde0e59243071073e57be8f11db8efedf9c898e555b4331f", "source": "sources/w14-projectile-b-run-5spell/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 644, "name": "_ZL5sprayPhRK6Point3Pvffiii"}, {"compiled_body_bytes": 1148, "name": "run__15SpellConeOfFireG6Point3s"}], "header_closure": ["sources/w14-projectile-b-common/spell.hpp", "sources/w14-projectile-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_run_spellconeoffire.o", "profile": "authored_cpp", "sha256": "e9289e8ca0bc1d13a9a61cf685229d1177650dafe72bce16ed7040bb32baa5fd", "source": "sources/w14-projectile-b-run-spellconeoffire/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "run__14SpellHarmTouchG6Point3s"}], "header_closure": ["sources/w12-actor-spell-c-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_run_spellharmtouch.o", "profile": "authored_cpp", "sha256": "33aedfc706fa54d3cb0275e9b1025a34111c35e5f806464f727687b504d7b969", "source": "sources/w14-projectile-b-run-spellharmtouch/run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 528, "name": "startFractalLine__FP4Boltfi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pb_start_fractal_line.o", "profile": "authored_cpp", "sha256": "13ccfeed5eb0d5fe1e4e8824ac0c7401e858d7eb713ed19e9af661d4b090c769", "source": "sources/w14-projectile-b-start-fractal-line/start_fractal_line.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1352, "name": "terminate__19ColdArrowProjectilei"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_terminate_cold_arrow.o", "profile": "authored_cpp", "sha256": "d83e942769a3b37941a00d5cc332294dc7a9ea04bef40744d24f8d3d28acbc1e", "source": "sources/w14-projectile-b-terminate-cold-arrow/terminate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 488, "name": "terminate__22ExplodeArrowProjectilei"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_terminate_explode_arrow.o", "profile": "authored_cpp", "sha256": "23bf54636c031c8d6e08744e95b87e17d7315c6c076254c7781f3236ca38985d", "source": "sources/w14-projectile-b-terminate-explode-arrow/terminate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 208, "name": "terminate__19FireArrowProjectilei"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_terminate_fire_arrow.o", "profile": "authored_cpp", "sha256": "9267bae6aa75795a1e28a20082b2d89222f4fdbd39d51ed5be19e63d183df62f", "source": "sources/w14-projectile-b-terminate-fire-arrow/terminate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 268, "name": "terminate__19MultiFireProjectilei"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_terminate_multifire.o", "profile": "authored_cpp", "sha256": "c9f939a19d0449470a41745a780e2eb802f88e5990103208d41b7df3eb5c03d3", "source": "sources/w14-projectile-b-terminate-multifire/terminate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 396, "name": "terminate__21PoisonArrowProjectilei"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w14pb_terminate_poison_arrow.o", "profile": "authored_cpp", "sha256": "119f361c0765c1b2a0bbd5ec3ac6f4e4a925b664257fd0af63c6d4a262c2ca84", "source": "sources/w14-projectile-b-terminate-poison-arrow/terminate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1528, "name": "waterStep"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14pb_water_step.o", "profile": "authored_cpp", "sha256": "ac47f612b1ca9416e94b80f9727e8f9b351d55f27e49aa546dc36245bed34755", "source": "sources/w14-projectile-b-water-step/water_step.cpp"}
```

## Files to record in the snapshot

| file (under `experimental/`) | sha256 |
|---|---|
| `sources/w14-projectile-b-common/access.hpp` | `74aca7d7a6bb4b43eacc8b1ce81153cc139b4b3bd52627aeb86057734567e4f3` |
| `sources/w14-projectile-b-common/spell.hpp` | `b1c4f643192730e392001db499050f6aef9ac5ee515f4840dbb55b7d99fc4905` |
| `sources/w14-projectile-b-msg-draw-beetleprojectile/host_check.cpp` | `a1a7821c2e8882d5ff512de6c4b23666e592fe10d45d80233662cab91c9fec7f` |
| `sources/w14-projectile-b-msg-draw-beetleprojectile/msg_draw.cpp` | `47fb5f9c6290a1b7f4ba581604c0317266ecf78e37c7d7c3116092ca02c09645` |
| `sources/w14-projectile-b-msg-draw-poisonprojectile/host_check.cpp` | `f1fb9871ff536eb448a93790a14c9105e50005a9d6ec8d8393f1629c672a46e1` |
| `sources/w14-projectile-b-msg-draw-poisonprojectile/msg_draw.cpp` | `9ca4ba73c0355f96908a478da2496905ea3550723c2dd739aa4697e6ca8df1f4` |
| `sources/w14-projectile-b-msg-run-beetleprojectile/host_check.cpp` | `fad6d87cf0c1a60a4b74dd71cf232ae8612566b809a2e9b9f6011f92a2689691` |
| `sources/w14-projectile-b-msg-run-beetleprojectile/msg_run.cpp` | `edb33900fa41dc2b561190004d7444d156e2d482c2c7d6ebd87bfca553a46fab` |
| `sources/w14-projectile-b-msg-run-boltgoround/host_check.cpp` | `494234fdfec3b66e701d249cecb84dd1b5215abad46eb8cf5051833abd62c3b6` |
| `sources/w14-projectile-b-msg-run-boltgoround/msg_run.cpp` | `4f8f91bd6b2ae6427c4a1d6c34517a8e69d9c80466f2678579141828d01717ae` |
| `sources/w14-projectile-b-msg-run-diseasebolt/host_check.cpp` | `dc096c2bdaa1bd2e294667b049760e38cc6b469ef589467322b4acad0e6663a9` |
| `sources/w14-projectile-b-msg-run-diseasebolt/msg_run.cpp` | `a4f701eb5d9e0c167a34a99857a2a215caf19c40c41aaed74f8e694b9138a5e9` |
| `sources/w14-projectile-b-msg-run-firebombletjr/host_check.cpp` | `b391c00f9ac775794ffe0b0070e70d0ff7ebeb72bf4f64a4dc5d58a31f3b4a97` |
| `sources/w14-projectile-b-msg-run-firebombletjr/msg_run.cpp` | `aa6908def9dc5e3daed87f49d367f136473a57f291a7becaabfe0a18e763a494` |
| `sources/w14-projectile-b-msg-run-froststormspell/host_check.cpp` | `04fbd1fdba3c334f85173b42241b052302ef1df4268df70cd677687112f6b881` |
| `sources/w14-projectile-b-msg-run-froststormspell/msg_run.cpp` | `a38880100ba9f1db20de870fe04b14622fc7e8aadbbbce96e236b760c89fd815` |
| `sources/w14-projectile-b-msg-run-hateprojectile/host_check.cpp` | `a21f9e83de3bf73db1a02ab0700eafd9433132f01a06c082bff6d261fe2fbeb2` |
| `sources/w14-projectile-b-msg-run-hateprojectile/msg_run.cpp` | `af14e413f183266a62d6a2ea2f4eb54e73b30bd5dd27d8b6a18c47bdd0083a5a` |
| `sources/w14-projectile-b-msg-run-mummysnake/host_check.cpp` | `8d8ee1742c9bf88069ace1618d1d4015cc6f48ccd3658a20e880ce2e4c975531` |
| `sources/w14-projectile-b-msg-run-mummysnake/msg_run.cpp` | `83de642f3dbb49aae0978fc66593783409de4667b8d2d3a2199376e87724eafa` |
| `sources/w14-projectile-b-msg-run-poisongasspell/host_check.cpp` | `5c815081e2797162b9466155a8d8cc53a464578707a8eab6c4ddb1d9fe6c8eb7` |
| `sources/w14-projectile-b-msg-run-poisongasspell/msg_run.cpp` | `3fb43fb33bbefffbed86e0e93a8163a7a2fc65690e44c07a7ef2df3002e573ab` |
| `sources/w14-projectile-b-msg-run-poisonprojectile/host_check.cpp` | `04fe8ea976b20c12aaa095d02758c83574a2347a99bd7ce39dcf318c75cb5a8b` |
| `sources/w14-projectile-b-msg-run-poisonprojectile/msg_run.cpp` | `a0ea19b9cfb8b1378de99864ad79d2c71ddfdbab597b8264506bdda0029d6657` |
| `sources/w14-projectile-b-msg-run-poisonrain/host_check.cpp` | `a76d55721d279e3588b67eaa42ebe90e58905e2dac0360ce7898b3f2c15ccccd` |
| `sources/w14-projectile-b-msg-run-poisonrain/msg_run.cpp` | `8d97f0d3c95446d2d91cd0c560043aa533c1aa42e119584608118e7e60ec7693` |
| `sources/w14-projectile-b-msg-run-shockoffrost/host_check.cpp` | `339f31a691c9ece97c39570d8b13eeefb9659ad5e423c4c0549e3db8053bcdcb` |
| `sources/w14-projectile-b-msg-run-shockoffrost/msg_run.cpp` | `d80fb46ca66d7f4a5f1a5fa5cbbabba986eaf7e67d10b866a959dedb60ae94fe` |
| `sources/w14-projectile-b-msg-run-shockprojectile/host_check.cpp` | `36f114b634c2bdf9231d900864f23148262435171881d6a41201535f3169c1e2` |
| `sources/w14-projectile-b-msg-run-shockprojectile/msg_run.cpp` | `6997f800c092730cb7c0e0ebc6d6e7a53ab397f745622c52763bc3cf835f9040` |
| `sources/w14-projectile-b-msg-run-spawnmummy/host_check.cpp` | `b7a8efd52a367f56e440ba77368298f0f26aabf22646232b5d641556275a3960` |
| `sources/w14-projectile-b-msg-run-spawnmummy/msg_run.cpp` | `ba31f74ff15637200d819eee1bf277331ea90426faa8e102401b22980cbb3452` |
| `sources/w14-projectile-b-run-5spell/host_check.cpp` | `c60ff900eafb007269cd569adca0757b5e5a2b347d3ff253491fac29c9b8337a` |
| `sources/w14-projectile-b-run-5spell/run.cpp` | `37826e9bd9cfef8ecde0e59243071073e57be8f11db8efedf9c898e555b4331f` |
| `sources/w14-projectile-b-run-spellconeoffire/host_check.cpp` | `d83f72c35be74991aa2ef76b54f9d4928c08a430762a0f0ec01512f5dd891497` |
| `sources/w14-projectile-b-run-spellconeoffire/run.cpp` | `e9289e8ca0bc1d13a9a61cf685229d1177650dafe72bce16ed7040bb32baa5fd` |
| `sources/w14-projectile-b-run-spellharmtouch/host_check.cpp` | `9e6cf7ca04717c7fc6e5692fd44e6289b477c62660e519b2a31e2f2c209c601e` |
| `sources/w14-projectile-b-run-spellharmtouch/run.cpp` | `33aedfc706fa54d3cb0275e9b1025a34111c35e5f806464f727687b504d7b969` |
| `sources/w14-projectile-b-start-fractal-line/host_check.cpp` | `13a38b7da146e6cd2f8789fe7830acced3355ae97fecd0afe847c697eb1cd3cd` |
| `sources/w14-projectile-b-start-fractal-line/start_fractal_line.cpp` | `13ccfeed5eb0d5fe1e4e8824ac0c7401e858d7eb713ed19e9af661d4b090c769` |
| `sources/w14-projectile-b-terminate-cold-arrow/host_check.cpp` | `cd8e29dc8a0863625dcadc2761a73aa6782fdb5d1e24532c29978c70c65d5b97` |
| `sources/w14-projectile-b-terminate-cold-arrow/terminate.cpp` | `d83e942769a3b37941a00d5cc332294dc7a9ea04bef40744d24f8d3d28acbc1e` |
| `sources/w14-projectile-b-terminate-explode-arrow/host_check.cpp` | `994fd9f498af365941947c23d4e3cac2ab55f9ea5977aaae644342e4256fd550` |
| `sources/w14-projectile-b-terminate-explode-arrow/terminate.cpp` | `23bf54636c031c8d6e08744e95b87e17d7315c6c076254c7781f3236ca38985d` |
| `sources/w14-projectile-b-terminate-fire-arrow/host_check.cpp` | `dc210aaa012877dfe42df38fc7b79e5221543c8e99c2a5c526a7909dc8267302` |
| `sources/w14-projectile-b-terminate-fire-arrow/terminate.cpp` | `9267bae6aa75795a1e28a20082b2d89222f4fdbd39d51ed5be19e63d183df62f` |
| `sources/w14-projectile-b-terminate-multifire/host_check.cpp` | `54e8abf73f79afbc452a14e5e036b8c0a93f113007fd0112790c6b49749d8d04` |
| `sources/w14-projectile-b-terminate-multifire/terminate.cpp` | `c9f939a19d0449470a41745a780e2eb802f88e5990103208d41b7df3eb5c03d3` |
| `sources/w14-projectile-b-terminate-poison-arrow/host_check.cpp` | `327dd4ea3d0b59e8fdb8565bb87bdfa196cd6a229f7a9c0af3de6d7212d18e24` |
| `sources/w14-projectile-b-terminate-poison-arrow/terminate.cpp` | `119f361c0765c1b2a0bbd5ec3ac6f4e4a925b664257fd0af63c6d4a262c2ca84` |
| `sources/w14-projectile-b-water-step/host_check.cpp` | `6d5b6f8343b19f2686e5d4540300a4d32dcab7ba3165f04f47aac70d8d1eb08f` |
| `sources/w14-projectile-b-water-step/water_step.cpp` | `ac47f612b1ca9416e94b80f9727e8f9b351d55f27e49aa546dc36245bed34755` |

## Authored but never executed

All 25 units. They compiled with the reviewed toolchain and passed host contracts and paired interpreter runs only. Nothing ran on the EE or in
the game. Specifically unexecuted: all float rounding on the EE (truncation versus the interpreter's round-to-nearest), the scratchpad register
save area `waterStep` uses (the authored version needs none), real table slots (the host contracts stand in for them), and every call into
the callee symbols that remain unresolved.

## Unknown

- The meaning of most object words and constants named in the source headers (they say observed, inferred and unknown separately).
- `pack` (the particle definition the cold arrow's first ring uses) and the other bss particle definitions are imported as image-named or
  address-named views; their layouts are not established here.
- Why the cold arrow's second loop passes the object's own position.
- Whether `linePos` is reset elsewhere before `startFractalLine` runs (the lightning starter sets it to 0 before the call).
- The cause of the miscompile noted for `startFractalLine`'s first version.

## Blocked

- `msg_run__6Player` (0x00197878, 27,496 bytes, about 6,900 instructions, about 650 branches, 17 indirect calls and 94 distinct direct callees
  across combat, animation, spells, HUD and net code). Not attempted beyond an inventory of its callees; it needs its own owner and a
  scenario plan far larger than this package's budget. Left as an open import.
