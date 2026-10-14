# Skill energy cost, level requirement, point cost and icon accessors, part 1: w13-skill-stat-a

Branch `agents/w13-skill-stat-a`. Forty functions, one per commit, each with a host contract and a paired run
against the private image function. Reference: the hash-pinned executable `SLUS-20565-sha256-9a60f13c5c31`.
No game binary, raw disassembly, decompiler output or extracted asset is committed; the only image-derived
content is symbol names, addresses, sizes, small integer/float constants and the two resource-name strings each
icon accessor passes.

## Observed

- Level requirement and point cost accessors are leaf functions. Constants: level requirement Blessed 8, Cyclone 18,
  Healing 8, Riposte 12, Entangle 25, FireStorm 20; point cost Blessed 1, Riposte 1, FireStorm 1. Darkness level
  requirement is the rank shifted left by two and its point cost is three times the rank (low 32 bits, wrapping).
- Thirteen icon accessors tail-call `lumpFindResource__FPcT0("icons.lmp", "<name>.tex")` and return its result;
  the receiver is not read. The names are in each source file.
- Eleven energy cost accessors (Shock 10, Charge 20, Archery 20, Blessed 1, Cyclone 20, Healing 14, Darkness 1,
  Entangle 1, ColdArrow 20, FireArrow 20, HarmTouch 18) read the table word at object+96, take the entry at byte 128
  (halfword adjusting the receiver, function word at +4) and call it with (receiver + adjust, rank unchanged, constant),
  returning its float result untouched. In the 22 `_vt$` tables of `docs/agents/w12-skill-a.md` byte 128 is slot 16,
  `energyCostRamp4__5Skillii`.
- Dodge and LifeTap energy cost call the entry at byte 120 (slot 15, `tweekRank__5Skillii`) with (receiver, rank, 1),
  ignore the result and return 0.0f.
- FireStorm energy cost is `(float)(rank - 1) * 6.0000048 + 120.0` without touching the receiver; the factor is a
  4-byte small-data word (bits 0x40c0000a) of unknown owner.
- `getDeflectionChance__10SkillDodgei` returns 0.0f for rank 0; otherwise it calls slot 15 with (receiver, rank, 1),
  then slot 11 (byte 88, `evaluateRampAtRank__5Skillii`) with (receiver, 0, that result) and returns
  `(float)result * 0.01f` (bits 0x3c23d70a, small-data word of unknown owner).
- `getAmount__12SkillLifeTapi` calls slot 15 with (receiver, rank, 1) then slot 11 with (receiver, 1, that result)
  and returns the int result.

| Function | Original address / bytes | Compiled bytes | Source |
|---|---:|---:|---|
| `getLevelRequirement__12SkillBlessedi` | `0x002a6fd0` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-blessed/get_level_requirement.cpp` |
| `getLevelRequirement__12SkillCyclonei` | `0x0029a9d8` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-cyclone/get_level_requirement.cpp` |
| `getLevelRequirement__12SkillHealingi` | `0x002af368` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-healing/get_level_requirement.cpp` |
| `getLevelRequirement__12SkillRipostei` | `0x002a5110` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-riposte/get_level_requirement.cpp` |
| `getLevelRequirement__13SkillEntanglei` | `0x002c6f90` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-entangle/get_level_requirement.cpp` |
| `getLevelRequirement__14SkillFireStormi` | `0x0029d238` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-fire-storm/get_level_requirement.cpp` |
| `getLevelRequirement__13SkillDarknessi` | `0x00298300` / 8 | 8 | `experimental/sources/w13-skill-stat-a-level-requirement-skill-darkness/get_level_requirement.cpp` |
| `getSkillPointCost__12SkillBlessedi` | `0x002a6fc8` / 8 | 8 | `experimental/sources/w13-skill-stat-a-point-cost-skill-blessed/get_skill_point_cost.cpp` |
| `getSkillPointCost__12SkillRipostei` | `0x002a5108` / 8 | 8 | `experimental/sources/w13-skill-stat-a-point-cost-skill-riposte/get_skill_point_cost.cpp` |
| `getSkillPointCost__14SkillFireStormi` | `0x0029d230` / 8 | 8 | `experimental/sources/w13-skill-stat-a-point-cost-skill-fire-storm/get_skill_point_cost.cpp` |
| `getSkillPointCost__13SkillDarknessi` | `0x002982f0` / 12 | 12 | `experimental/sources/w13-skill-stat-a-point-cost-skill-darkness/get_skill_point_cost.cpp` |
| `getIcon__10SkillShock` | `0x00316a78` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-shock/get_icon.cpp` |
| `getIcon__11SkillCharge` | `0x00316bd8` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-charge/get_icon.cpp` |
| `getIcon__12SkillBlessed` | `0x00318610` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-blessed/get_icon.cpp` |
| `getIcon__12SkillCyclone` | `0x003176a8` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-cyclone/get_icon.cpp` |
| `getIcon__12SkillHealing` | `0x003191e0` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-healing/get_icon.cpp` |
| `getIcon__12SkillLifeTap` | `0x0031a000` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-life-tap/get_icon.cpp` |
| `getIcon__13SkillDarkness` | `0x00317298` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-darkness/get_icon.cpp` |
| `getIcon__13SkillEntangle` | `0x0031a560` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-entangle/get_icon.cpp` |
| `getIcon__14SkillColdArrow` | `0x00317d00` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-cold-arrow/get_icon.cpp` |
| `getIcon__14SkillFireArrow` | `0x00317bb0` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-fire-arrow/get_icon.cpp` |
| `getIcon__14SkillFireStorm` | `0x00317948` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-fire-storm/get_icon.cpp` |
| `getIcon__14SkillHarmTouch` | `0x00317818` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-harm-touch/get_icon.cpp` |
| `getIcon__14SkillMultiFire` | `0x00317fa0` / 36 | 20 | `experimental/sources/w13-skill-stat-a-icon-skill-multi-fire/get_icon.cpp` |
| `getEnergyCost__10SkillShocki` | `0x0028d940` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-shock/get_energy_cost.cpp` |
| `getEnergyCost__11SkillChargei` | `0x002920d8` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-charge/get_energy_cost.cpp` |
| `getEnergyCost__12SkillArcheryi` | `0x002a2c20` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-archery/get_energy_cost.cpp` |
| `getEnergyCost__12SkillBlessedi` | `0x002a7010` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-blessed/get_energy_cost.cpp` |
| `getEnergyCost__12SkillCyclonei` | `0x0029a9a0` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-cyclone/get_energy_cost.cpp` |
| `getEnergyCost__12SkillHealingi` | `0x002af330` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-healing/get_energy_cost.cpp` |
| `getEnergyCost__13SkillDarknessi` | `0x00298308` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-darkness/get_energy_cost.cpp` |
| `getEnergyCost__13SkillEntanglei` | `0x002c6f58` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-entangle/get_energy_cost.cpp` |
| `getEnergyCost__14SkillColdArrowi` | `0x0029fdc8` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-cold-arrow/get_energy_cost.cpp` |
| `getEnergyCost__14SkillFireArrowi` | `0x0029e538` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-fire-arrow/get_energy_cost.cpp` |
| `getEnergyCost__14SkillHarmTouchi` | `0x0029af40` / 52 | 24 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-harm-touch/get_energy_cost.cpp` |
| `getEnergyCost__10SkillDodgei` | `0x002ae7a0` / 56 | 48 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-dodge/get_energy_cost.cpp` |
| `getEnergyCost__12SkillLifeTapi` | `0x002be308` / 56 | 48 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-life-tap/get_energy_cost.cpp` |
| `getEnergyCost__14SkillFireStormi` | `0x0029d240` / 36 | 40 | `experimental/sources/w13-skill-stat-a-energy-cost-skill-fire-storm/get_energy_cost.cpp` |
| `getDeflectionChance__10SkillDodgei` | `0x002ae7d8` / 116 | 120 | `experimental/sources/w13-skill-stat-a-deflection-chance-skill-dodge/get_deflection_chance.cpp` |
| `getAmount__12SkillLifeTapi` | `0x002be3b0` / 92 | 80 | `experimental/sources/w13-skill-stat-a-amount-skill-life-tap/get_amount.cpp` |

## Inferred

- Slot names come from the table sources of the sibling package; the names are not shown to be what the code means.
  "Rank" is the name given to the second integer argument because callers pass a skill rank; that is inferred.
- Return types: float for energy cost and deflection chance (the float register is the result), int for the
  others. Callers already declared `getAmount` as int and `getDeflectionChance` as float.

## Unknown

- What the constants passed to slot 16 (1, 10, 14, 18, 20) mean, the owner and name of the two small-data floats,
  what `lumpFindResource` returns for an icon, and whether the Dodge/LifeTap energy cost call is meaningful
  (its result is discarded in the image).
- Compiled bodies are not image-sized in some cases (the image has load delay slots filled differently); no byte
  match is claimed for any function.

## Checks performed

- Host contract for each unit: `host_check.cpp` (built with `c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST`).
  Constant accessors are checked over signed boundary ranks; icon accessors check the call count and both strings;
  table-calling accessors use a host table with distinct adjust/function values per slot and check slot byte offset,
  adjusted receiver, argument values, call order and result. All 40 pass.
- Paired run (private R5900 interpreter outside git; harness note in the private memory): each authored function
  against the image function from identical states for 12 ranks (0, 1, 2, 3, 7, 19, 100, 0x7fffffff, 0x80000000,
  0xffffffff, 0xfffffffe and a large negative value), with the table entries pointing at hooked addresses that log
  their arguments and return rank-dependent values, and with `lumpFindResource__FPcT0` hooked to log both strings.
  Return value, the float register, call logs and every changed byte outside the stack compared: 40 functions,
  0 mismatches, no HALT. Mutations that were detected: Shock constant 10 to 11, deflection factor bit, deflection
  second-call argument, FireStorm 120 to 121, Darkness shift 2 to 3, an icon string character, LifeTap amount
  argument, LifeTap energy cost return 0.0 to 1.0.
- EE compile of every unit with the reviewed `authored_cpp` flags.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- Private partial build with all 40 entries added to a plan copy outside git (5568 compiled sources, 4280 function bodies): 1128
  unresolved imports, none of the 40 package names among them. `lumpFindResource__FPcT0` is defined in that build, so no import
  was opened. The total is not comparable to the shared branch because the copy carries other waves' entries.

## Imports closed

`getLevelRequirement__12SkillBlessedi`, `getLevelRequirement__12SkillCyclonei`, `getLevelRequirement__12SkillHealingi`, `getLevelRequirement__12SkillRipostei`, `getLevelRequirement__13SkillEntanglei`, `getLevelRequirement__14SkillFireStormi`, `getLevelRequirement__13SkillDarknessi`, `getSkillPointCost__12SkillBlessedi`, `getSkillPointCost__12SkillRipostei`, `getSkillPointCost__14SkillFireStormi`, `getSkillPointCost__13SkillDarknessi`, `getIcon__10SkillShock`, `getIcon__11SkillCharge`, `getIcon__12SkillBlessed`, `getIcon__12SkillCyclone`, `getIcon__12SkillHealing`, `getIcon__12SkillLifeTap`, `getIcon__13SkillDarkness`, `getIcon__13SkillEntangle`, `getIcon__14SkillColdArrow`, `getIcon__14SkillFireArrow`, `getIcon__14SkillFireStorm`, `getIcon__14SkillHarmTouch`, `getIcon__14SkillMultiFire`, `getEnergyCost__10SkillShocki`, `getEnergyCost__11SkillChargei`, `getEnergyCost__12SkillArcheryi`, `getEnergyCost__12SkillBlessedi`, `getEnergyCost__12SkillCyclonei`, `getEnergyCost__12SkillHealingi`, `getEnergyCost__13SkillDarknessi`, `getEnergyCost__13SkillEntanglei`, `getEnergyCost__14SkillColdArrowi`, `getEnergyCost__14SkillFireArrowi`, `getEnergyCost__14SkillHarmTouchi`, `getEnergyCost__10SkillDodgei`, `getEnergyCost__12SkillLifeTapi`, `getEnergyCost__14SkillFireStormi`, `getDeflectionChance__10SkillDodgei`, `getAmount__12SkillLifeTapi`.

## Imports opened

None new. The thirteen icon accessors import `lumpFindResource__FPcT0`, which other packages already import (and which the
private build above resolves). The table-calling accessors call through the object's table and add no symbol import.

## Build-plan entries needed

Run `python3 docs/agents/w13-skill-stat-a-plan-entries.py --toolchain <ee> [--private-root DIR]` for the 40 exact JSON
entries (one per `experimental/sources/w13-skill-stat-a-*/` unit; empty `header_closure`, profile `authored_cpp`).
Raise `default_authored_inputs` by 40 and add each source hash to the `files` maps. The shared plan was not edited.

## Authored but never executed

All 40 functions ran only in the host contract and in the private interpreter against the image function with
stubbed callees. None ran in a game. `energyCostRamp4`, `tweekRank` and `evaluateRampAtRank` were never executed
as real callees; hooks stood in for them. The real `lumpFindResource` was never executed.
