# Skill energy cost, level requirement, point cost and icon accessors, part 2: w13-skill-stat-b

Branch `agents/w13-skill-stat-b`. Forty functions, one per commit, each with a host contract and a paired run against the
private image function. Reference: the hash-pinned executable `SLUS-20565-sha256-9a60f13c5c31`. No game binary, raw
disassembly, decompiler output or extracted asset is committed; the only image-derived content is symbol names, addresses,
sizes, small integer/float constants and the resource-name strings each icon accessor passes. Same shapes as
`docs/agents/w13-skill-stat-a.md` (part 1); the table-calling accessors reuse its call-through-table helper.

## Observed

- Level requirement and point cost accessors are leaf functions returning a constant: level requirement ClericRoot 20,
  ConeOfFire 28, FireFlares 20, HolyStrike 1, PoisonRain 20, ShieldBash 5, WizardBeam 10, ColdWeapons 15, CriticalHit 8;
  point cost 1 for ConeOfFire, FireFlares, HolyStrike, PoisonRain, WizardBeam and ColdWeapons.
- Twelve icon accessors tail-call `lumpFindResource__FPcT0("icons.lmp", "<name>.tex")` and return its result; the receiver
  is not read. The names are in each source file.
- Seven energy cost accessors (MultiFire 20, ClericRoot 20, FrostStorm 20, HolyShield 20, CriticalHit 20, ShieldBash 0,
  ConeOfFrost 1) read the table word at object+96, take the entry at byte 128 (halfword adjusts the receiver, function word
  at +4) and call it with (receiver + adjust, rank unchanged, constant), returning its float result untouched. In the
  `_vt$` tables of `docs/agents/w12-skill-a.md` byte 128 is slot 16, `energyCostRamp4__5Skillii`.
- Six energy cost accessors are linear and do not touch the receiver: `(float)(rank - 1) * step + base` with
  (step, base) = FireFlares (6, 122), HolyStrike (3, 7), PoisonRain (12, 222), ColdWeapons (9, 137) as immediates, and
  ConeOfFire (bits 0x40000008, 42) and WizardBeam (bits 0x40000021, 22) whose step is a 4-byte small-data word.

| Function | Original address / bytes | Compiled bytes | Source |
|---|---:|---:|---|
| `getLevelRequirement__15SkillClericRooti` | `0x002c6318` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-cleric-root/get_level_requirement.cpp` |
| `getLevelRequirement__15SkillConeOfFirei` | `0x002b6b00` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-cone-of-fire/get_level_requirement.cpp` |
| `getLevelRequirement__15SkillFireFlaresi` | `0x002a4970` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-fire-flares/get_level_requirement.cpp` |
| `getLevelRequirement__15SkillHolyStrikei` | `0x002b2e18` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-holy-strike/get_level_requirement.cpp` |
| `getLevelRequirement__15SkillPoisonRaini` | `0x002bf238` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-poison-rain/get_level_requirement.cpp` |
| `getLevelRequirement__15SkillShieldBashi` | `0x002c8b90` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-shield-bash/get_level_requirement.cpp` |
| `getLevelRequirement__15SkillWizardBeami` | `0x002b9480` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-wizard-beam/get_level_requirement.cpp` |
| `getLevelRequirement__16SkillColdWeaponsi` | `0x002ac468` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-cold-weapons/get_level_requirement.cpp` |
| `getLevelRequirement__16SkillCriticalHiti` | `0x002addb0` / 8 | 8 | `experimental/sources/w13-skill-stat-b-level-requirement-skill-critical-hit/get_level_requirement.cpp` |
| `getSkillPointCost__15SkillConeOfFirei` | `0x002b6af8` / 8 | 8 | `experimental/sources/w13-skill-stat-b-point-cost-skill-cone-of-fire/get_skill_point_cost.cpp` |
| `getSkillPointCost__15SkillFireFlaresi` | `0x002a4968` / 8 | 8 | `experimental/sources/w13-skill-stat-b-point-cost-skill-fire-flares/get_skill_point_cost.cpp` |
| `getSkillPointCost__15SkillHolyStrikei` | `0x002b2e10` / 8 | 8 | `experimental/sources/w13-skill-stat-b-point-cost-skill-holy-strike/get_skill_point_cost.cpp` |
| `getSkillPointCost__15SkillPoisonRaini` | `0x002bf230` / 8 | 8 | `experimental/sources/w13-skill-stat-b-point-cost-skill-poison-rain/get_skill_point_cost.cpp` |
| `getSkillPointCost__15SkillWizardBeami` | `0x002b9478` / 8 | 8 | `experimental/sources/w13-skill-stat-b-point-cost-skill-wizard-beam/get_skill_point_cost.cpp` |
| `getSkillPointCost__16SkillColdWeaponsi` | `0x002ac460` / 8 | 8 | `experimental/sources/w13-skill-stat-b-point-cost-skill-cold-weapons/get_skill_point_cost.cpp` |
| `getIcon__15SkillClericRoot` | `0x0031a498` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-cleric-root/get_icon.cpp` |
| `getIcon__15SkillConeOfFire` | `0x003199d8` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-cone-of-fire/get_icon.cpp` |
| `getIcon__15SkillFireFlares` | `0x003182d0` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-fire-flares/get_icon.cpp` |
| `getIcon__15SkillFrostStorm` | `0x00319740` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-frost-storm/get_icon.cpp` |
| `getIcon__15SkillHolyShield` | `0x00319040` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-holy-shield/get_icon.cpp` |
| `getIcon__15SkillHolyStrike` | `0x00319570` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-holy-strike/get_icon.cpp` |
| `getIcon__15SkillShieldBash` | `0x0031a790` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-shield-bash/get_icon.cpp` |
| `getIcon__15SkillWizardBeam` | `0x00319b10` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-wizard-beam/get_icon.cpp` |
| `getIcon__16SkillColdWeapons` | `0x00318d98` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-cold-weapons/get_icon.cpp` |
| `getIcon__16SkillConeOfFrost` | `0x00319330` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-cone-of-frost/get_icon.cpp` |
| `getIcon__16SkillCriticalHit` | `0x00318ed8` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-critical-hit/get_icon.cpp` |
| `getIcon__16SkillDiseaseBolt` | `0x00316c50` / 36 | 20 | `experimental/sources/w13-skill-stat-b-icon-skill-disease-bolt/get_icon.cpp` |
| `getEnergyCost__14SkillMultiFirei` | `0x002a1d60` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-multi-fire/get_energy_cost.cpp` |
| `getEnergyCost__15SkillClericRooti` | `0x002c62e0` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-cleric-root/get_energy_cost.cpp` |
| `getEnergyCost__15SkillFrostStormi` | `0x002b36c8` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-frost-storm/get_energy_cost.cpp` |
| `getEnergyCost__15SkillHolyShieldi` | `0x002ae558` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-holy-shield/get_energy_cost.cpp` |
| `getEnergyCost__15SkillShieldBashi` | `0x002c8b58` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-shield-bash/get_energy_cost.cpp` |
| `getEnergyCost__16SkillConeOfFrosti` | `0x002afec8` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-cone-of-frost/get_energy_cost.cpp` |
| `getEnergyCost__16SkillCriticalHiti` | `0x002add78` / 52 | 24 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-critical-hit/get_energy_cost.cpp` |
| `getEnergyCost__15SkillConeOfFirei` | `0x002b6b08` / 36 | 40 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-cone-of-fire/get_energy_cost.cpp` |
| `getEnergyCost__15SkillFireFlaresi` | `0x002a4978` / 40 | 40 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-fire-flares/get_energy_cost.cpp` |
| `getEnergyCost__15SkillHolyStrikei` | `0x002b2e20` / 40 | 40 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-holy-strike/get_energy_cost.cpp` |
| `getEnergyCost__15SkillPoisonRaini` | `0x002bf240` / 40 | 40 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-poison-rain/get_energy_cost.cpp` |
| `getEnergyCost__15SkillWizardBeami` | `0x002b9488` / 36 | 40 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-wizard-beam/get_energy_cost.cpp` |
| `getEnergyCost__16SkillColdWeaponsi` | `0x002ac470` / 40 | 40 | `experimental/sources/w13-skill-stat-b-energy-cost-skill-cold-weapons/get_energy_cost.cpp` |

## Inferred

- Slot names come from the table sources of the sibling package; the names are not shown to be what the code means.
  "Rank" is the name given to the integer argument because callers pass a skill rank; the linear form suggests an
  energy cost that grows per rank, which is a reading of the arithmetic only.
- Return types: float for energy cost, int for level requirement and point cost, pointer-sized for the icon result.

## Unknown

- What the constants passed to slot 16 (0, 1, 20) mean; the owner and names of the two small-data floats (nearly 2.0:
  2.0000019 and 2.0000079); what `lumpFindResource` returns for an icon.
- Compiled bodies are not image-sized in some cases (different delay-slot filling and the image's global-pointer-relative
  access that the `-G0` profile cannot emit); no byte match is claimed for any function.

## Checks performed

- Host contract for each unit: `host_check.cpp` (`c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST`). Constant accessors are
  checked over signed boundary ranks; icon accessors check the call count and both strings; table-calling accessors use a
  table with distinct entries at bytes 88, 120 and 128 and check which entry ran, the adjusted receiver, rank and constant;
  linear accessors check the formula over boundary ranks and rank 1 and 2 exactly. All 40 pass.
- Paired run (private R5900 interpreter, outside git): each authored function against the image function from identical
  states, 12 ranks including 0, 0x7fffffff, 0x80000000 and 0xffffffff, hooked `lumpFindResource` and the table entries
  logging arguments and string contents, comparing return values (float register bits where the result is float) and every
  changed byte: 40 functions x 12 scenarios, zero mismatches, no HALT. Mutations (PoisonRain base 222 to 221, ShieldBash
  `icbash` to `icbasx`) failed 10 of 12 and 12 of 12 scenarios; both restored and re-passed.
- EE compile of every unit with the reviewed `authored_cpp` flags.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- No private partial build was run for this package (one at a time on a loaded machine); the closed names below were
  checked against the unresolved-imports list of the shared plan only by name.

## Imports closed

`getLevelRequirement__15SkillClericRooti`, `getLevelRequirement__15SkillConeOfFirei`, `getLevelRequirement__15SkillFireFlaresi`, `getLevelRequirement__15SkillHolyStrikei`, `getLevelRequirement__15SkillPoisonRaini`, `getLevelRequirement__15SkillShieldBashi`, `getLevelRequirement__15SkillWizardBeami`, `getLevelRequirement__16SkillColdWeaponsi`, `getLevelRequirement__16SkillCriticalHiti`, `getSkillPointCost__15SkillConeOfFirei`, `getSkillPointCost__15SkillFireFlaresi`, `getSkillPointCost__15SkillHolyStrikei`, `getSkillPointCost__15SkillPoisonRaini`, `getSkillPointCost__15SkillWizardBeami`, `getSkillPointCost__16SkillColdWeaponsi`, `getIcon__15SkillClericRoot`, `getIcon__15SkillConeOfFire`, `getIcon__15SkillFireFlares`, `getIcon__15SkillFrostStorm`, `getIcon__15SkillHolyShield`, `getIcon__15SkillHolyStrike`, `getIcon__15SkillShieldBash`, `getIcon__15SkillWizardBeam`, `getIcon__16SkillColdWeapons`, `getIcon__16SkillConeOfFrost`, `getIcon__16SkillCriticalHit`, `getIcon__16SkillDiseaseBolt`, `getEnergyCost__14SkillMultiFirei`, `getEnergyCost__15SkillClericRooti`, `getEnergyCost__15SkillFrostStormi`, `getEnergyCost__15SkillHolyShieldi`, `getEnergyCost__15SkillShieldBashi`, `getEnergyCost__16SkillConeOfFrosti`, `getEnergyCost__16SkillCriticalHiti`, `getEnergyCost__15SkillConeOfFirei`, `getEnergyCost__15SkillFireFlaresi`, `getEnergyCost__15SkillHolyStrikei`, `getEnergyCost__15SkillPoisonRaini`, `getEnergyCost__15SkillWizardBeami`, `getEnergyCost__16SkillColdWeaponsi`.

## Imports opened

None new. The icon accessors import `lumpFindResource__FPcT0`, which other packages already import or define. The
table-calling accessors call through the object's table and add no symbol import.

## Build-plan entries needed

Run `python3 docs/agents/w13-skill-stat-b-plan-entries.py --toolchain <ee> [--private-root DIR]` for the 40 exact JSON
entries (one per `experimental/sources/w13-skill-stat-b-*/` unit; empty `header_closure`, profile `authored_cpp`). Raise
`default_authored_inputs` by 40 and add each source hash to the `files` maps. The shared plan was not edited.

## Authored but never executed

All 40 functions ran only in the host contract and in the private interpreter against the image function with stubbed
callees. None ran in a game. `energyCostRamp4__5Skillii` was never executed as a real callee; a hook stood in for it. The
real `lumpFindResource` was never executed.

## What is unknown

See "Unknown" above: the meaning of the constants, the small-data float owners, the icon record, and byte-level equality.
