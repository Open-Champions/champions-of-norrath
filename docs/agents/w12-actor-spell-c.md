# w12-actor-spell-c: spell, projectile, prop and misc actor classes (C)

Branch `agents/w12-actor-spell-c`. Every function below is its own commit (four follow-up commits only make helper
functions file-local, see "Checks performed"). Sources are under `experimental/sources/w12-actor-spell-c-*`; each source
starts with a comment stating what was observed in the private image, what is inferred and what is unknown. Nothing from
the image (bytes, disassembly) is committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and sizes below are that file's symbol
values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the `authored_cpp`
flags; it differs from the image size because these are behaviour reconstructions, not byte matches.

## What was done

27 of the 28 assigned imports are closed (27 plan inputs). The remaining one, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`
(9,184 bytes), was surveyed and not authored; see "Not done".

- 5 clone functions (`clone__...`): allocate, copy the observed byte ranges, call the ListHead copy constructor, store the
  class tables in the observed order. The OniBall clone also clears and registers an embedded light (`lightAdd__FP5Light`).
- 4 leaf stubs (`draw__5Spell`, `msg_draw__7OniBall`, `msg_run__7Weather`, `fxRunBallLightning__...`): the image returns at once.
- 18 behaviour functions: `eRand__5Spell`, `joinCommander__14WoodElfSoldier`, `start__3WebG6Point3T1i`, `moveArrow__14WoodElfSoldier`,
  `msg_collision__17VehicleBodySphereP10GameObject`, `msg_collision__7OniBallP10GameObject`, `msg_run__15GlowingCylinder`,
  `msg_draw__15GlowingCylinder`, `msg_run__5PhIce`, `move__10ProjectileG6Point3`, `manaAbsorbOnHit__15SpellHolyShield`,
  `processParams__7VehiclePPc`, `msg_run__7OniBall`, `initType__5Mummyb`, `hitSomething__11SpellChargeP10GameObject`,
  `run__19SpellHornOmenEffectG6Point3s`, `run__22SpellPlanarSteelEffectG6Point3s`, `fxFlyingFire__FP6Point3T0ffb`.

All callees use their real image symbol names. A shared accessor header `sources/w12-actor-spell-c-common/access.hpp` (a
copy of the w10-actor-n accessors under the `w12c` namespace) holds the word/float accessors and the slot-table call helpers
(`call_entry`, `call_entry_from`); in host contracts it keeps stored addresses as offsets from a test base.

## Units

| unit | symbol | image | compiled bytes | source sha256 |
|---|---|---|---|---|
| `w12-actor-spell-c-clone-glowingcylinder` | `clone__15GlowingCylinder` | `0x00124818`, 304 bytes | 316 | `f2710c078310c0b20f0cbbdd0729985ce950b7b2b0df43b89f2451c99d0599b1` |
| `w12-actor-spell-c-clone-oniball` | `clone__7OniBall` | `0x00125390`, 528 bytes | 420 | `502bd37c4a97d688376765e1e5300535c62b7973b9bcbfba4c9bdb82aa781165` |
| `w12-actor-spell-c-clone-phice` | `clone__5PhIce` | `0x00114f70`, 1012 bytes | 436 | `241e6fc3f7749bd4a4f4227aa23f209f408af3e88c2b678476a40a17e1f5639c` |
| `w12-actor-spell-c-clone-vehiclebodysphere` | `clone__17VehicleBodySphere` | `0x00115fd0`, 272 bytes | 276 | `87d365a3773b7d1368317671a55b97ee36d26cf9ca18c4cbb5d53224b834331d` |
| `w12-actor-spell-c-clone-weather` | `clone__7Weather` | `0x00129280`, 288 bytes | 276 | `b079ea3aa3e663519fa87cf053a6ff38317b23c538fd769fbfcd067405211d9a` |
| `w12-actor-spell-c-draw-5spell` | `draw__5Spell` | `0x00314628`, 8 bytes | 8 | `f0b6279a020b07b10cb7015e93cd0e1ccca62a5ac22cab0dda548d95ae78b93a` |
| `w12-actor-spell-c-erand-5spell` | `eRand__5Spell` | `0x0015e948`, 40 bytes | 56 | `4bde6f550a367a9e74224bca205510adf866e2db42ad6f76332251020158bee6` |
| `w12-actor-spell-c-fx-flying-fire` | `fxFlyingFire__FP6Point3T0ffb` | `0x001dfe98`, 2516 bytes | 1920 | `264946036224359f08a2f62562c52a78e58da2d040f6a34a2c7e2c7f68f144ae` |
| `w12-actor-spell-c-fx-run-ball-lightning` | `fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii` | `0x001df560`, 36 bytes | 8 | `e92e5dc799e50643535556de0d089636c3ea412a95fa0b71936daafac033cc77` |
| `w12-actor-spell-c-hit-something-11spellcharge` | `hitSomething__11SpellChargeP10GameObject` | `0x00291738`, 1596 bytes | 1704 | `6e27c402946f8c7f91cfb6d3a55646d6b50a27a51cfd10a761814abd5b256740` |
| `w12-actor-spell-c-init-type-5mummy` | `initType__5Mummyb` | `0x002371a8`, 1196 bytes | 708 | `16f590663c71b41f491fd0722c73909ea11aec2fe69054444ec646658712c2f5` |
| `w12-actor-spell-c-join-commander-14woodelfsoldier` | `joinCommander__14WoodElfSoldier` | `0x00249db0`, 92 bytes | 88 | `b8cf9ba4211b288773e4a1cd335b161a696d9590cd19b914de48ddcca2247689` |
| `w12-actor-spell-c-mana-absorb-on-hit-15spellholyshield` | `manaAbsorbOnHit__15SpellHolyShield` | `0x002ae060`, 864 bytes | 740 | `8a51154d696418c9df86282da4131058cd0a6996d48ba057bd00566cec89f99d` |
| `w12-actor-spell-c-move-10projectile` | `move__10ProjectileG6Point3` | `0x00144cb8`, 1072 bytes | 1052 | `b8e37428b8e5ecc20ed385151980d635627c3708b6bc0dca7082a2dba3aac706` |
| `w12-actor-spell-c-move-arrow-14woodelfsoldier` | `moveArrow__14WoodElfSoldier` | `0x00249a98`, 220 bytes | 196 | `c5fbb02d4f5bb3c2b3aa38f379471cb975fb9648fa172de4172fbc8b2b5cad61` |
| `w12-actor-spell-c-msg-collision-17vehiclebodysphere` | `msg_collision__17VehicleBodySphereP10GameObject` | `0x0021cf88`, 48 bytes | 24 | `0352445c3ebd7b9a9fedeced8b1bec58a30402392d2703c3f456f92783a7eb7d` |
| `w12-actor-spell-c-msg-collision-7oniball` | `msg_collision__7OniBallP10GameObject` | `0x0021aaf0`, 364 bytes | 456 | `fe470c234a3d607e704a8d668f63b4c91715c227ae80e2ae84847e6a3caa0a65` |
| `w12-actor-spell-c-msg-draw-15glowingcylinder` | `msg_draw__15GlowingCylinder` | `0x001d8c08`, 840 bytes | 716 | `067bfddf38487e9c112a5bc9acc28aefa6334e8237616596eaf963758dd352af` |
| `w12-actor-spell-c-msg-draw-7oniball` | `msg_draw__7OniBall` | `0x0021dd40`, 8 bytes | 8 | `286e7e1b4d677c95a38af420ef853b627118164ee34d1208e8b69b55ef5dcb28` |
| `w12-actor-spell-c-msg-run-15glowingcylinder` | `msg_run__15GlowingCylinder` | `0x001d8f50`, 460 bytes | 488 | `14cd90ddd3b467a85eba10ed2d702204b86416afdaa04222491b605dd174700b` |
| `w12-actor-spell-c-msg-run-5phice` | `msg_run__5PhIce` | `0x001df588`, 632 bytes | 596 | `c09eb2f316a1da7d3e6b288138c674818a65e41bfd48939666ad46df3a400b71` |
| `w12-actor-spell-c-msg-run-7oniball` | `msg_run__7OniBall` | `0x0021a5e0`, 1292 bytes | 1752 | `69d0ae939fa1ef31a448b2bbbe55e2545393220617e964fc729fdf116ca25add` |
| `w12-actor-spell-c-msg-run-7weather` | `msg_run__7Weather` | `0x0021cdf8`, 8 bytes | 8 | `fb04c25e63b82c008efbd3fc4227d21afba0dc1b9a4cd35764f18ffe0c3d56c6` |
| `w12-actor-spell-c-process-params-7vehicle` | `processParams__7VehiclePPc` | `0x00271870`, 688 bytes | 672 | `60fafb712f52c846d421d42fdb8507099a397da8c1779879f556e3cd0a663ee1` |
| `w12-actor-spell-c-run-19spellhornomeneffect` | `run__19SpellHornOmenEffectG6Point3s` | `0x002c9a38`, 2052 bytes | 616 | `122ec3e9a5e7871b73d326183baefce01367a923f9cc449e10f1cfca37a330b4` |
| `w12-actor-spell-c-run-22spellplanarsteeleffect` | `run__22SpellPlanarSteelEffectG6Point3s` | `0x002c8da8`, 2616 bytes | 1964 | `bb088ff4ce2312172dfc060f36b4b09ac7728a0da6766b0d38f1341131af34d5` |
| `w12-actor-spell-c-start-3web` | `start__3WebG6Point3T1i` | `0x0020faf0`, 148 bytes | 88 | `9af189365575c51049e68161ef5557f90098445f62099a43443a8d27dc9071a2` |

## Checks performed

Every unit has a `host_check.cpp` (not an EE input; it includes the source and replaces callees with recording stubs; built
with `clang++ -std=c++17` and exits non-zero on a wrong call order, argument, store or return). The five clones, `eRand`,
`start__3Web`, the four stubs' paired runs and the two collision functions are checked through their paired run (their host
contracts are small).

Every function was also compared with the original in a private paired-run harness (an R5900 interpreter that runs the original
function from the image and the freshly compiled authored function from identical memory snapshots, with every callee replaced by
the same Python stub). Calls with their arguments (registers, stack slots, float registers), object snapshots, the return where
meaningful (the float result for `manaAbsorbOnHit`, the owner's colour halfwords for the two spell run functions, the global random
state for `fxFlyingFire`) and every changed byte outside the stack were compared; `HALT` never occurred in a final run. The
harness lives outside git (`priv-w12-actor-spell-c/`).

| function | scenarios | original words covered |
|---|---|---|
| the five clones | 6 random sources + null allocation (OniBall: + 4 light-word cases) | 72/72, 68/68, 76/76, 227/253 (PhIce: the unaligned 128-byte copy path is not reached by aligned test buffers), 132/132 |
| the four stubs | 4 random-memory scenarios each | all |
| `eRand__5Spell` | 8 random states | all |
| `msg_collision__17VehicleBodySphere` | 4 table adjustments | all |
| `joinCommander`, `start__3Web`, `moveArrow` | 3, 8, 16 | all reachable |
| `msg_collision__7OniBall` | 116 | 91/91 |
| `msg_run__15GlowingCylinder` | 26 | 115/115 |
| `msg_draw__15GlowingCylinder` | 39 | 209/210 (padding) |
| `msg_run__5PhIce` | 34 | 158/158 |
| `move__10Projectile` | 38 | 268/268 |
| `manaAbsorbOnHit__15SpellHolyShield` | 13 | 216/216 |
| `processParams__7Vehicle` | 13 | 170/172 (padding) |
| `msg_run__7OniBall` | 49 | 321/323 (padding) |
| `initType__5Mummyb` | 72 | 270/299 (padding and the record-search branches, dead because the four records were cleared a few steps earlier) |
| `hitSomething__11SpellCharge` | 87 | 399/399 |
| `run__19SpellHornOmenEffect` | 37 | 513/513 |
| `run__22SpellPlanarSteelEffect` | 42 | 653/654 (padding) |
| `fxFlyingFire` | 56 | 625/629 (padding) |

Each paired script was also run against deliberate mutations of the source (constants, offsets, comparison operators, loop
counts, argument orders, roll arithmetic); every non-equivalent mutation was caught, and equivalent ones are named in the commit
messages or below. The mutated files were restored before every commit. The scripts are not committed (private tooling).

A private partial build (shared selection plus these 27 units, plan copy kept outside git, built with the reviewed toolchain)
produced 5,140 compiled inputs, 3,940 function bodies, no COMMON symbols and **822 unresolved imports** (the shared tip was recorded at
827: 827 - 27 closed + 22 opened = 822). A first attempt failed to link on a duplicate `draw_direction`: helper functions in an
anonymous namespace inside an `extern "C"` block are still exported. Four follow-up commits made them `static`
(`hitSomething__11SpellChargeP10GameObject`, `initType__5Mummyb`, `run__19SpellHornOmenEffectG6Point3s`,
`run__22SpellPlanarSteelEffectG6Point3s`); the paired runs were repeated with the same results. Of the 28 assigned names only
`gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` is still unresolved.
`python3 -m unittest discover -s tools -p 'test_*.py'` was run on the final tree (result in the final message).

Plan helper: `python3 docs/agents/w12-actor-spell-c-plan-entries.py --toolchain /Users/case/norrath-agents/repo/work/toolchains/ee [--private-root DIR]`
prints the entries below (and optionally writes a private plan/snapshot copy outside git).

## Observed behaviour worth knowing (details are in the source comments)

- `move__10ProjectileG6Point3`: the three pool floats it reads are 1/60 (displacement scale), 0.1 (search step) and 0.2 (sound
  volume). The step-back search loop adds the step to itself while it is below 2.0; the reviewed compiler would turn the loop into an integer
  trip count folded with its own float rounding, so the add is kept at run time with an empty asm barrier.
- `msg_collision__7OniBall` tests skill 59 (the same skill id as `msg_run__7OniBall`) and calls `SkillLifeTap::getAmount` through
  the table whose pointer is the word at +96 of a skill object (the slot tables of skill objects are at +96, those of game objects at +72;
  `manaAbsorbOnHit` uses +72 for the owner and +96 for the target).
- `initType__5Mummyb`: the four effect records are cleared with -1 just before the search that looks for a free record, so the search
  always uses the first record.
- `processParams__7VehiclePPc` parses "seatoffset", "turret" (with a model file name whose ".vif" is replaced by ".tex"), "disable" and
  "stack" parameters; the strstr result for ".vif" and the allocation result are not checked.
- `msg_run__5PhIce` and the spell run functions draw directions inside the unit ball by rejection from a 16-bit generator;
  `run__22SpellPlanarSteelEffect` passes the unused-looking short as a second argument and reads its position argument only in the last two loops.
- `run__19SpellHornOmenEffect` and `run__22SpellPlanarSteelEffect` end the effect by calling slot byte 8 of the table at word +20 with 3.
- `fxRunBallLightning__...` reserves an 816-byte frame, copies its position argument onto the stack and returns without using it.

## Imports closed

- `clone__15GlowingCylinder`
- `clone__7OniBall`
- `clone__5PhIce`
- `clone__17VehicleBodySphere`
- `clone__7Weather`
- `draw__5Spell`
- `eRand__5Spell`
- `fxFlyingFire__FP6Point3T0ffb`
- `fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii`
- `hitSomething__11SpellChargeP10GameObject`
- `initType__5Mummyb`
- `joinCommander__14WoodElfSoldier`
- `manaAbsorbOnHit__15SpellHolyShield`
- `move__10ProjectileG6Point3`
- `moveArrow__14WoodElfSoldier`
- `msg_collision__17VehicleBodySphereP10GameObject`
- `msg_collision__7OniBallP10GameObject`
- `msg_draw__15GlowingCylinder`
- `msg_draw__7OniBall`
- `msg_run__15GlowingCylinder`
- `msg_run__5PhIce`
- `msg_run__7OniBall`
- `msg_run__7Weather`
- `processParams__7VehiclePPc`
- `run__19SpellHornOmenEffectG6Point3s`
- `run__22SpellPlanarSteelEffectG6Point3s`
- `start__3WebG6Point3T1i`

## Imports opened

New callee/data names introduced by these sources that no other source in the tree already uses and that stay unresolved in the check build.

Real image symbols (their definitions belong to other packages): `P_CloseParticleSystem__Fi`, `P_OpenParticleSystem__Fi`, `P_SetParticleSystemMatrix__FiP8Matrix34`, `P_SetTrailColor__Fiiiii`, `findCommander__14WoodElfSoldier`, `getAmount__12SkillLifeTapi`, `modelDrawEffect__FP12_modelHeaderP8_textureiP8Matrix34fP14AnimationStateUl`.

File-local particle records of the original units (80-byte objects defined `static` by the particle static-initialiser sources, for example
`w8-fx-particles-init-fx`, `w8-spell-static-init-a-*`, `w10-actor-a-static-init-*`). They are imported here under address-based names, the
convention already used by `ant_msg_run_particle_7d7170`; the integrator has to export those objects under these names (or define the names as aliases)
before the callers link: `fxflyingfire_particle_7d6830`, `fxflyingfire_particle_7d6880`, `fxflyingfire_particle_7d68d0`, `fxflyingfire_particle_7d6920`, `holyshield_particle_8d7720`, `hornomen_run_particle_8d8650`, `oniball_msg_run_particle_7d7810`, `oniball_msg_run_particle_7d78b0`, `phice_msg_run_particle_7d6740`, `phice_msg_run_particle_7d6790`, `planarsteel_run_particle_8d8510`, `planarsteel_run_particle_8d8560`, `planarsteel_run_particle_8d85b0`, `planarsteel_run_particle_8d8600`, `spellcharge_hit_something_particle_8d6780`.

Already imports of other packages and still unresolved in the check build, now also used here: `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`,
`addEffect__FiG6Point3`, `matScale__FP8Matrix34fff`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`.

## Build-plan entries needed

None of the 27 inputs is in `experimental/build-plan.json` (not edited by this package). Profile `authored_cpp`; the header closure of most
units is `sources/w12-actor-spell-c-common/access.hpp`. Host checks are not plan inputs. Entries (also printed by the plan helper; the helper units
also list the local helper symbols the compiler emits, `register_effect`, `draw_direction` and `emit_one.constprop.0`):

```json
[
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 316,
        "name": "clone__15GlowingCylinder"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_clone_glowingcylinder.o",
    "profile": "authored_cpp",
    "sha256": "f2710c078310c0b20f0cbbdd0729985ce950b7b2b0df43b89f2451c99d0599b1",
    "source": "sources/w12-actor-spell-c-clone-glowingcylinder/clone.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 420,
        "name": "clone__7OniBall"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_clone_oniball.o",
    "profile": "authored_cpp",
    "sha256": "502bd37c4a97d688376765e1e5300535c62b7973b9bcbfba4c9bdb82aa781165",
    "source": "sources/w12-actor-spell-c-clone-oniball/clone.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 436,
        "name": "clone__5PhIce"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_clone_phice.o",
    "profile": "authored_cpp",
    "sha256": "241e6fc3f7749bd4a4f4227aa23f209f408af3e88c2b678476a40a17e1f5639c",
    "source": "sources/w12-actor-spell-c-clone-phice/clone.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 276,
        "name": "clone__17VehicleBodySphere"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_clone_vehiclebodysphere.o",
    "profile": "authored_cpp",
    "sha256": "87d365a3773b7d1368317671a55b97ee36d26cf9ca18c4cbb5d53224b834331d",
    "source": "sources/w12-actor-spell-c-clone-vehiclebodysphere/clone.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 276,
        "name": "clone__7Weather"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_clone_weather.o",
    "profile": "authored_cpp",
    "sha256": "b079ea3aa3e663519fa87cf053a6ff38317b23c538fd769fbfcd067405211d9a",
    "source": "sources/w12-actor-spell-c-clone-weather/clone.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "draw__5Spell"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_draw_5spell.o",
    "profile": "authored_cpp",
    "sha256": "f0b6279a020b07b10cb7015e93cd0e1ccca62a5ac22cab0dda548d95ae78b93a",
    "source": "sources/w12-actor-spell-c-draw-5spell/draw.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 56,
        "name": "eRand__5Spell"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_erand_5spell.o",
    "profile": "authored_cpp",
    "sha256": "4bde6f550a367a9e74224bca205510adf866e2db42ad6f76332251020158bee6",
    "source": "sources/w12-actor-spell-c-erand-5spell/erand.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1920,
        "name": "fxFlyingFire__FP6Point3T0ffb"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_fx_flying_fire.o",
    "profile": "authored_cpp",
    "sha256": "264946036224359f08a2f62562c52a78e58da2d040f6a34a2c7e2c7f68f144ae",
    "source": "sources/w12-actor-spell-c-fx-flying-fire/fx_flying_fire.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_fx_run_ball_lightning.o",
    "profile": "authored_cpp",
    "sha256": "e92e5dc799e50643535556de0d089636c3ea412a95fa0b71936daafac033cc77",
    "source": "sources/w12-actor-spell-c-fx-run-ball-lightning/fx_run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1704,
        "name": "hitSomething__11SpellChargeP10GameObject"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_hit_something_11spellcharge.o",
    "profile": "authored_cpp",
    "sha256": "6e27c402946f8c7f91cfb6d3a55646d6b50a27a51cfd10a761814abd5b256740",
    "source": "sources/w12-actor-spell-c-hit-something-11spellcharge/hit_something.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 268,
        "name": "register_effect"
      },
      {
        "compiled_body_bytes": 708,
        "name": "initType__5Mummyb"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_init_type_5mummy.o",
    "profile": "authored_cpp",
    "sha256": "16f590663c71b41f491fd0722c73909ea11aec2fe69054444ec646658712c2f5",
    "source": "sources/w12-actor-spell-c-init-type-5mummy/init_type.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 88,
        "name": "joinCommander__14WoodElfSoldier"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_join_commander_14woodelfsoldier.o",
    "profile": "authored_cpp",
    "sha256": "b8cf9ba4211b288773e4a1cd335b161a696d9590cd19b914de48ddcca2247689",
    "source": "sources/w12-actor-spell-c-join-commander-14woodelfsoldier/join_commander.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 740,
        "name": "manaAbsorbOnHit__15SpellHolyShield"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_mana_absorb_on_hit_15spellholyshield.o",
    "profile": "authored_cpp",
    "sha256": "8a51154d696418c9df86282da4131058cd0a6996d48ba057bd00566cec89f99d",
    "source": "sources/w12-actor-spell-c-mana-absorb-on-hit-15spellholyshield/mana_absorb.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1052,
        "name": "move__10ProjectileG6Point3"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_move_10projectile.o",
    "profile": "authored_cpp",
    "sha256": "b8e37428b8e5ecc20ed385151980d635627c3708b6bc0dca7082a2dba3aac706",
    "source": "sources/w12-actor-spell-c-move-10projectile/move.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 196,
        "name": "moveArrow__14WoodElfSoldier"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_move_arrow_14woodelfsoldier.o",
    "profile": "authored_cpp",
    "sha256": "c5fbb02d4f5bb3c2b3aa38f379471cb975fb9648fa172de4172fbc8b2b5cad61",
    "source": "sources/w12-actor-spell-c-move-arrow-14woodelfsoldier/move_arrow.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 24,
        "name": "msg_collision__17VehicleBodySphereP10GameObject"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_collision_17vehiclebodysphere.o",
    "profile": "authored_cpp",
    "sha256": "0352445c3ebd7b9a9fedeced8b1bec58a30402392d2703c3f456f92783a7eb7d",
    "source": "sources/w12-actor-spell-c-msg-collision-17vehiclebodysphere/msg_collision.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 456,
        "name": "msg_collision__7OniBallP10GameObject"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_collision_7oniball.o",
    "profile": "authored_cpp",
    "sha256": "fe470c234a3d607e704a8d668f63b4c91715c227ae80e2ae84847e6a3caa0a65",
    "source": "sources/w12-actor-spell-c-msg-collision-7oniball/msg_collision.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 716,
        "name": "msg_draw__15GlowingCylinder"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_draw_15glowingcylinder.o",
    "profile": "authored_cpp",
    "sha256": "067bfddf38487e9c112a5bc9acc28aefa6334e8237616596eaf963758dd352af",
    "source": "sources/w12-actor-spell-c-msg-draw-15glowingcylinder/msg_draw.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "msg_draw__7OniBall"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_draw_7oniball.o",
    "profile": "authored_cpp",
    "sha256": "286e7e1b4d677c95a38af420ef853b627118164ee34d1208e8b69b55ef5dcb28",
    "source": "sources/w12-actor-spell-c-msg-draw-7oniball/msg_draw.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 488,
        "name": "msg_run__15GlowingCylinder"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_run_15glowingcylinder.o",
    "profile": "authored_cpp",
    "sha256": "14cd90ddd3b467a85eba10ed2d702204b86416afdaa04222491b605dd174700b",
    "source": "sources/w12-actor-spell-c-msg-run-15glowingcylinder/msg_run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 596,
        "name": "msg_run__5PhIce"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_run_5phice.o",
    "profile": "authored_cpp",
    "sha256": "c09eb2f316a1da7d3e6b288138c674818a65e41bfd48939666ad46df3a400b71",
    "source": "sources/w12-actor-spell-c-msg-run-5phice/msg_run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1752,
        "name": "msg_run__7OniBall"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_run_7oniball.o",
    "profile": "authored_cpp",
    "sha256": "69d0ae939fa1ef31a448b2bbbe55e2545393220617e964fc729fdf116ca25add",
    "source": "sources/w12-actor-spell-c-msg-run-7oniball/msg_run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "msg_run__7Weather"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_msg_run_7weather.o",
    "profile": "authored_cpp",
    "sha256": "fb04c25e63b82c008efbd3fc4227d21afba0dc1b9a4cd35764f18ffe0c3d56c6",
    "source": "sources/w12-actor-spell-c-msg-run-7weather/msg_run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 672,
        "name": "processParams__7VehiclePPc"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_process_params_7vehicle.o",
    "profile": "authored_cpp",
    "sha256": "60fafb712f52c846d421d42fdb8507099a397da8c1779879f556e3cd0a663ee1",
    "source": "sources/w12-actor-spell-c-process-params-7vehicle/process_params.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 224,
        "name": "draw_direction"
      },
      {
        "compiled_body_bytes": 312,
        "name": "emit_one.constprop.0"
      },
      {
        "compiled_body_bytes": 616,
        "name": "run__19SpellHornOmenEffectG6Point3s"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_run_19spellhornomeneffect.o",
    "profile": "authored_cpp",
    "sha256": "122ec3e9a5e7871b73d326183baefce01367a923f9cc449e10f1cfca37a330b4",
    "source": "sources/w12-actor-spell-c-run-19spellhornomeneffect/run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 224,
        "name": "draw_direction"
      },
      {
        "compiled_body_bytes": 1964,
        "name": "run__22SpellPlanarSteelEffectG6Point3s"
      }
    ],
    "header_closure": [
      "sources/w12-actor-spell-c-common/access.hpp"
    ],
    "kind": "independently_authored_source",
    "object_name": "w12asc_run_22spellplanarsteeleffect.o",
    "profile": "authored_cpp",
    "sha256": "bb088ff4ce2312172dfc060f36b4b09ac7728a0da6766b0d38f1341131af34d5",
    "source": "sources/w12-actor-spell-c-run-22spellplanarsteeleffect/run.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 88,
        "name": "start__3WebG6Point3T1i"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w12asc_start_3web.o",
    "profile": "authored_cpp",
    "sha256": "9af189365575c51049e68161ef5557f90098445f62099a43443a8d27dc9071a2",
    "source": "sources/w12-actor-spell-c-start-3web/start.cpp"
  }
]
```

## Authored but never executed

Nothing here has run on an EE or as part of the game. All 27 functions were run only (a) in the private R5900 interpreter next to the original
image function with hooked callees, and (b) in the host contracts. The interpreter rounds floating-point results to nearest while the EE truncates, so
exact float equality at rounding edges (for example the 0.1-step search count in `move__10Projectile`) is established under nearest rounding only;
that loop uses a run-time add on purpose. Callee behaviour (the particle, sound, light, world and model calls) is stubbed identically on both sides.
`modelGetShadowPos` outputs, `lumpFindResource` results and the table functions are stand-ins, not the real routines.

## Unknown

- What most fields mean: the halfword colour group at +140..+154 of the owner in the two spell run functions, the word at +72 of
  `SpellCharge`, the counters at +584/+588 of `PhIce`, the record at +1068 of `Mummy`, the three floats near gp-18600.
- Which virtual functions the slot bytes 16, 48, 88, 96 and 128 are (they are called with consistent argument shapes but no name is known).
- Whether the pool floats in `msg_run__7OniBall` (1e8, 1/72, 0.6), `move__10Projectile` and `manaAbsorbOnHit` (0.263, 0.01) are named globals.
- The type of the 304-byte turret block in `processParams__7Vehicle` and the true size of `Matrix34` (64 bytes here, from stack frames).
- Why the original clones leave halfword gaps uncopied (for example 42..43, 58..59, 86..87 in GlowingCylinder).

## Not done

`gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` at `0x001ce218`, 9,184 bytes (2,297 instruction words): surveyed only.
It is the damage resolver: it calls itself, tests object types through RTTI (`__eq__C9type_infoRC9type_info` with `__tf8Innoruuk`, `__tf17SpellUndeadShield`,
`__tf26SpellSecondaryUndeadShield`, `__tf18SpellDiseaseEffect`), uses soft-double helpers (`litodp`, `dpadd`, `fmod`), two generators (`eRandState`,
`syncRandState`), and makes about 100 calls (sounds, hit text, shields, riposte, critical hit, poison gas, disease, floating text, `playHitSound`). A
faithful reconstruction with a paired run and a mutation check is a package of its own; it was not attempted beyond reading the first ~500
words, so no source for it is committed.
