# w11-creature-a: Creature AI helpers

Branch `agents/w11-creature-a`. Private executable `work/private/SLUS_205.65`, gp `0x004879f0`. Addresses, sizes
and names are the image's own symbol table; the first line of each source says which. Nothing here has run on
hardware or inside the game.

## Method and checks performed

- Each function was read from a private disassembly (nothing derived is committed), rewritten as readable C++
  and compiled alone with the reviewed `authored_cpp` flags. All 20 units compile.
- Every function was compared against the original in a private R5900 interpreter (outside git): original and
  authored code ran from identical memory snapshots with identical Python stand-ins for every callee, and the
  call log (argument registers, float registers, bytes behind pointer arguments), the return value (where the
  return is meaningful) and every changed byte outside the stack had to agree. Scenario counts are in the commit
  messages. A halted run counts as a failure. The `sqrt.s`/`rsqrt.s`/`mult1` defects noted by earlier packages were
  already fixed in the private copy used.
- Each comparison was re-run against mutated copies (changed constants, offsets, tie boundaries, loop bounds,
  argument order); every mutant listed in the commit messages was detected. Where a first scenario set did not
  catch a tie boundary (`<` against `<=`) a geometry family with exact ties was added and the mutant then failed.
- The functions that use the soft-double helpers (`alternateEnemyInZone`) were compared with `fptodp`, `dpmul` and
  `dptofp` replaced by Python arithmetic on both sides, so the double rounding itself is not compared.
- Not done: `tools/build_partial.py` (the plan is not mine to edit). `python3 -m unittest discover -s tools -p
  'test_*.py'` was run and passes.

## Observed, inferred, unknown

Each source starts with a comment that separates what was observed from what is inferred and unknown. In short:

- Observed: the control flow, offsets, constants, comparison directions and call arguments of all 20 functions.
  Examples worth knowing: `pointInZone` and `alternateEnemyInZone` use `16384 - iatan2(x, y)` while
  `enemyInZone2`/`steerToPosition` use `iatan2(y, x)` with no 16384; the zone tests disagree on whether the
  heading width is inclusive (`alternateEnemyInZone`'s object branch accepts a difference equal to the width; its player
  branch, `pointInZone` and `enemyInZone2` do not);
  `steerToPosition` and `flee2` share a 1/60 speed ramp on the byte at +800 and a frame cadence on the byte at +569.
- Inferred (named as such in the headers): the roles of the +576 enemy list, the +752 list, and the +792 mode
  halfword; that `findClearPath` returns a heading with a clear lane; that the slam effects are the impact flash and
  spray.
- Unknown: the meaning of most object fields (+12 flag bits, +294 bit 0x80, +569, +570, +788, +800, +816/+820), the
  virtual slots at table byte 128 (a model getter), 160 (a candidate judge), 176 (a turn error), and the last two
  ints of the two hitscan functions.

## Imports closed

- `ISeeDeadPeople__8Creaturef`
- `alternateEnemyInZone__8Creatureffss`
- `checkIfSpellIsRunning__8CreatureRC9type_info`
- `creatureFindLeadYaw__8CreatureP10GameObjectf`
- `creatureSlamEffect__8CreatureP8Creaturei`
- `enemyInZone2__8Creatureffssfi`
- `enemyInZone__8Creaturefs`
- `findClearPath__8CreatureG6Point3scf`
- `findEnemyLeadPosition__8CreaturefP6Point3`
- `flee2__8CreatureP10GameObject`
- `numInRegion__8creatureG6Point3f`
- `objectIsCrowded__8CreatureP10GameObjectffi`
- `plotRoute__8CreatureG6Point3ffi`
- `pointInZone__8CreatureG6Point3ffss`
- `produceGeneralDamage__8CreatureffssiiiiPQ24item11description`
- `produceMeleeDamage__8CreaturefiiiiPQ24item11description`
- `steerAndRouteToEnemy__8Creature`
- `steerToPosition__8CreatureG6Point3`
- `superorcSlamEffect__8SuperOrcP8Creaturei`
- `turnToFaceEnemy__8Creature`

## Imports opened

Names these sources reference but do not define. The first group is already defined elsewhere in the shared plan
(verified by name against `experimental/build-plan.json` on the branch base); the second group is not and is now an
open import. In every case the calling form follows what the original call sites pass (a returned `Point3` comes
back through a hidden first pointer; `Point3` arguments are passed by address).

- Already defined in the plan: `P_AddParticle__FP11ParticleDefP6Point3T1f`,
  `__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__eq__C9type_infoRC9type_info`, `blockAlloc__Fi`, `dpmul`,
  `dptofp`, `eRand__10GameObject`, `fptodp`, `getCurrentRoutePoint__8Creature`,
  `getNextRoutePoint__8CreatureP6Point3G6Point3ffi`, `getSmartRandomDestination__8Creaturei`, `iatan2__Fff`, `icos__Fi`,
  `isin__Fi`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`,
  `move2__8CreatureG6Point3`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `steerAndRouteToPosition__8CreatureG6Point3`,
  `wallAndObjectSteering__FP8CreatureffP10GameObject`, `worldHitscan__FP12_worldHeaderG6Point3T1fi`.
- Not defined in the plan (open imports): `frameNm`, `nmPlayers`, `player` (data), `world` (data),
  `objectInZone__8CreatureP10GameObjectfs`, `worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii`,
  `worldPlotRouteCanShoot__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi`,
  `worldPlotRouteCantShoot__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi`,
  `worldPlotRouteCloseTo__FP12_worldHeaderG6Point3T1fP10GameObjectP11RouteBufferi`,
  `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, and `slamParticle`.
- `slamParticle` is a file-local `.bss` symbol with three separate instances in the image (0x004e20d0, 0x007cb6e0,
  0x007d7af0). `creatureSlamEffect` uses 0x004e20d0 and `superorcSlamEffect` uses 0x007d7af0. Both sources import the
  plain name `slamParticle`; the integrator must give each a distinct definition (or a distinct alias) of the right
  instance, or the two effects will share one particle definition. The comparisons bound each to its own address.

## Build-plan entries needed

Profile `authored_cpp`, no host check. Every unit includes `sources/w11-creature-a-common/words.hpp`, which
the integrator adds to the file list; `header_closure` names it. `input_order` is left for the integrator. Body sizes are
from the reviewed EE g++ `authored_cpp` profile, not from the original executable. No `private_object_sha256` is claimed.

```json
{"emitted_functions":[{"compiled_body_bytes":1436,"name":"alternateEnemyInZone__8Creatureffss"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"alternate_enemy_in_zone.o","profile":"authored_cpp","sha256":"f34ddeca8f3c2b7249a71bdda88bfb0176f6801f93b75dffdf7302f0480aa6d3","source":"sources/w11-creature-a-alternate-enemy-in-zone/alternate_enemy_in_zone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":132,"name":"checkIfSpellIsRunning__8CreatureRC9type_info"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"check_if_spell_is_running.o","profile":"authored_cpp","sha256":"1dce07c84081a369a215c5b28d2a06e0c407b17eb6af14b5cdaed17cbbf625fe","source":"sources/w11-creature-a-check-if-spell-is-running/check_if_spell_is_running.cpp"}
{"emitted_functions":[{"compiled_body_bytes":272,"name":"creatureFindLeadYaw__8CreatureP10GameObjectf"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"creature_find_lead_yaw.o","profile":"authored_cpp","sha256":"78956347c9d944a12080aa02f19c7d580ded624e2bc8fdd5346c8fd78c912ba1","source":"sources/w11-creature-a-creature-find-lead-yaw/creature_find_lead_yaw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":720,"name":"creatureSlamEffect__8CreatureP8Creaturei"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"creature_slam_effect.o","profile":"authored_cpp","sha256":"efcc4fcd8c0c4f02e9876bd19d8650098c75a43785acdbce3521ad9eb20c3b3c","source":"sources/w11-creature-a-creature-slam-effect/creature_slam_effect.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"enemyInZone__8Creaturefs"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"enemy_in_zone.o","profile":"authored_cpp","sha256":"c71abbd045d5bce927edd61246d6c8a592fd8738b0fc36a0f3f19fca76461ec0","source":"sources/w11-creature-a-enemy-in-zone/enemy_in_zone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":568,"name":"enemyInZone2__8Creatureffssfi"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"enemy_in_zone2.o","profile":"authored_cpp","sha256":"3fbaf032d4719cc61e378e4935a2144ad7b4cb8521fb95451e0a9f2241e25711","source":"sources/w11-creature-a-enemy-in-zone2/enemy_in_zone2.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1000,"name":"findClearPath__8CreatureG6Point3scf"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"find_clear_path.o","profile":"authored_cpp","sha256":"fccf58a0a3061cf30fec561eda6ae19ffd8cfebe63ff4e405770c6c872adc943","source":"sources/w11-creature-a-find-clear-path/find_clear_path.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"findEnemyLeadPosition__8CreaturefP6Point3"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"find_enemy_lead_position.o","profile":"authored_cpp","sha256":"5721e5a2f38e40aeffd0293d9d38fade30a4d4349c0822e184f390e063ecc5db","source":"sources/w11-creature-a-find-enemy-lead-position/find_enemy_lead_position.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1044,"name":"flee2__8CreatureP10GameObject"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"flee2.o","profile":"authored_cpp","sha256":"fbda644040881604ee3dfebc641a3b5cd16cc31d115bf97e6217724fda1af47b","source":"sources/w11-creature-a-flee2/flee2.cpp"}
{"emitted_functions":[{"compiled_body_bytes":296,"name":"ISeeDeadPeople__8Creaturef"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"i_see_dead_people.o","profile":"authored_cpp","sha256":"7fbabca4711ab066b9b3d7378725e003ea85df41ca43c7dc3ed664212ec7eec7","source":"sources/w11-creature-a-i-see-dead-people/i_see_dead_people.cpp"}
{"emitted_functions":[{"compiled_body_bytes":192,"name":"numInRegion__8creatureG6Point3f"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"num_in_region.o","profile":"authored_cpp","sha256":"a231100b75c19e4fffe317bc6ec0c880ff98097e4a254ec8b02a9cb9d1d2b59e","source":"sources/w11-creature-a-num-in-region/num_in_region.cpp"}
{"emitted_functions":[{"compiled_body_bytes":448,"name":"objectIsCrowded__8CreatureP10GameObjectffi"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"object_is_crowded.o","profile":"authored_cpp","sha256":"90fd304a17dcedb0b93808e06c6001fcbafbf7703f8c38fe56b38dd3a452b0a0","source":"sources/w11-creature-a-object-is-crowded/object_is_crowded.cpp"}
{"emitted_functions":[{"compiled_body_bytes":212,"name":"plotRoute__8CreatureG6Point3ffi"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"plot_route.o","profile":"authored_cpp","sha256":"1737d43b38d9b2737a71cb01b0968af78e5fbfe5e9c35f4f2d8b362fb5914062","source":"sources/w11-creature-a-plot-route/plot_route.cpp"}
{"emitted_functions":[{"compiled_body_bytes":440,"name":"pointInZone__8CreatureG6Point3ffss"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"point_in_zone.o","profile":"authored_cpp","sha256":"e2b250716f8e711ec50a4bfbd7c0ad106d85172a13f1ed8dbb98058c1ecca797","source":"sources/w11-creature-a-point-in-zone/point_in_zone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":340,"name":"produceGeneralDamage__8CreatureffssiiiiPQ24item11description"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"produce_general_damage.o","profile":"authored_cpp","sha256":"25e0aa6ed08b9c4dd4b0f355f318a6d860a26fb8961f8ac02deb7d4b52d90710","source":"sources/w11-creature-a-produce-general-damage/produce_general_damage.cpp"}
{"emitted_functions":[{"compiled_body_bytes":404,"name":"produceMeleeDamage__8CreaturefiiiiPQ24item11description"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"produce_melee_damage.o","profile":"authored_cpp","sha256":"6e066a5c93a15462c20101948626bbaba28f5bb3db56a235258f3c31dc09ed11","source":"sources/w11-creature-a-produce-melee-damage/produce_melee_damage.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"steerAndRouteToEnemy__8Creature"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"steer_and_route_to_enemy.o","profile":"authored_cpp","sha256":"a71ef8a1a4158fdbbdb29a45844ced2b662b9affbf73187d7ed5ab18e51aa9ce","source":"sources/w11-creature-a-steer-and-route-to-enemy/steer_and_route_to_enemy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":760,"name":"steerToPosition__8CreatureG6Point3"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"steer_to_position.o","profile":"authored_cpp","sha256":"69ee8fe807b6cb8f3d0d01ffedf6aa5d342624d396722f00d4d496805fbdfa1b","source":"sources/w11-creature-a-steer-to-position/steer_to_position.cpp"}
{"emitted_functions":[{"compiled_body_bytes":716,"name":"superorcSlamEffect__8SuperOrcP8Creaturei"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"superorc_slam_effect.o","profile":"authored_cpp","sha256":"a6533c7d88f423f9cca178380774074e1f99ebd37a3883be109e089b5afd2c1f","source":"sources/w11-creature-a-superorc-slam-effect/superorc_slam_effect.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"turnToFaceEnemy__8Creature"}],"header_closure":["sources/w11-creature-a-common/words.hpp"],"kind":"independently_authored_source","object_name":"turn_to_face_enemy.o","profile":"authored_cpp","sha256":"ec381d2b7db02d09fe56271ddbccccfe91b3e9828951f63dec70ba09a7f23102","source":"sources/w11-creature-a-turn-to-face-enemy/turn_to_face_enemy.cpp"}
```

## Authored but never executed

All of it on real hardware or in the game. All 20 functions have run only inside the private comparison
interpreter, with their callees replaced by recorded stand-ins; none has run against real callees or alongside
the plan's other objects. There are no host builds: the bodies read 32-bit pointer words directly and are
target-only (`static_assert(sizeof(void*) == 4)` in the shared header).

## Unknown

- The types and meaning of nearly every field read through offsets (see the source headers).
- What the virtual slots at table byte 128, 160 and 176 do, and what the last two ints of
  `worldHitscanWithObjects` / `worldHitscan` select.
- Behavior on NaN, infinity or denormal inputs: the interpreter does not model the EE's FPU saturation, so a
  scenario with overflowing coordinates was excluded rather than trusted.
- `flee2`, `steerToPosition` and `findClearPath` were compared with the hook for each trig/route helper returning
  chosen values, so the helpers' own behavior is not covered here.
- The by-value `signed char` argument of `findClearPath` is sign-extended inside the original but not in the
  authored build; calls that pass a non-extended byte (not possible from compiled callers) were not compared.
