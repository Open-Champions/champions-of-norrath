# Skill class family (w12-skill-c)

Branch: `agents/w12-skill-c`. All files are independently authored. Private reference: hash-pinned executable `SLUS-20565-sha256-9a60f13c5c31` (`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). No game binary, raw disassembly, decompiler output, or extracted asset is included.

## Observed and checked

The initial 5,113-input partial build completed with 827 unresolved symbols. At that baseline, all 31 names in this package were unresolved. Each function below was authored from its complete private symbol extent, compiled separately with the reviewed EE toolchain, and tested through a host contract. The table reports the original declared extent and generated source body separately. These are semantic reconstructions, not original-byte matches.

| Function | Original address / bytes | Compiled bytes | Source | SHA-256 |
|---|---:|---:|---|---|
| `cast__5SkilliP8CreatureG6Point3s` | `0x003131c0` / 40 | 8 | `experimental/sources/w12-skill-c-base-cast/cast.cpp`; host test `experimental/sources/w12-skill-c-base-cast/host_check.cpp` | `86a83d14bc1cce2ff41093d2a7f85c0c9aa3bc9d57d0724bb6fd4a170e560ef7` |
| `getEnergyCost__5Skilli` | `0x00313258` / 16 | 12 | `experimental/sources/w12-skill-c-base-energy-cost/get_energy_cost.cpp`; host test `experimental/sources/w12-skill-c-base-energy-cost/host_check.cpp` | `738d8bb6aa433f85b4e99f27482e1d1692783095a941a47dbbb9931d6d4f8e3e` |
| `getLevelRequirement__5Skilli` | `0x00313250` / 8 | 8 | `experimental/sources/w12-skill-c-base-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w12-skill-c-base-level-requirement/host_check.cpp` | `2d798d9d99254a95dc7abb2fb36d16bd3e7f378f19b305fe55ec8801f909330d` |
| `piggyBackCast__5SkilliP8CreatureG6Point3s` | `0x003131e8` / 40 | 8 | `experimental/sources/w12-skill-c-base-piggyback-cast/piggyback_cast.cpp`; host test `experimental/sources/w12-skill-c-base-piggyback-cast/host_check.cpp` | `f35c8269ab75757ced83981edeb2ccd2ff635b6549b9f882474f7977ad9f89db` |
| `getSkillPointCost__5Skilli` | `0x00313248` / 8 | 8 | `experimental/sources/w12-skill-c-base-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w12-skill-c-base-point-cost/host_check.cpp` | `e15fe82c7f4a11da37131f4e985a6f57a980b1305be2170b291168557461b95c` |
| `getLevelRequirement__10SkillBlunti` | `0x00313b40` / 8 | 8 | `experimental/sources/w12-skill-c-blunt-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w12-skill-c-blunt-level-requirement/host_check.cpp` | `fe74c131e903a8b9dd14e54a32c992e47dcb734eb99f281080a12a1a1c2723f5` |
| `getSkillPointCost__10SkillBlunti` | `0x00313b38` / 8 | 8 | `experimental/sources/w12-skill-c-blunt-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w12-skill-c-blunt-point-cost/host_check.cpp` | `5693b5d4a1a5384643351c21c7ccc088dc752cbe1f3a22385da3cffdbc4a4e47` |
| `initRamps__15SkillBallOfHate` | `0x0015ea90` / 32 | 32 | `experimental/sources/w12-skill-c-init-ramps-ball-of-hate/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-ball-of-hate/host_check.cpp` | `3d2095075a595cf93baadef362c8f474e884c61bc2bea1db4f33c41d06cb7d04` |
| `initRamps__5Skill` | `0x00313a08` / 76 | 60 | `experimental/sources/w12-skill-c-init-ramps-base/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-base/host_check.cpp` | `c95746d6a7ad1306988fb3f750356f643a44e5c27d23aead2d046eb779d889b1` |
| `initRamps__18SkillDiseaseEffect` | `0x0015eae0` / 44 | 44 | `experimental/sources/w12-skill-c-init-ramps-disease-effect/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-disease-effect/host_check.cpp` | `5b6b1ec563bdd13172b31c412822918b50b4ccc27b37ba78acef973fe639b479` |
| `initRamps__19SkillDiseaseWeapons` | `0x0015e9a0` / 32 | 32 | `experimental/sources/w12-skill-c-init-ramps-disease-weapons/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-disease-weapons/host_check.cpp` | `16462c39c6bc401b6830d2cdd7b3cba15c721e8da80a7bb216c8e41358426dc6` |
| `initRamps__13SkillHateBeam` | `0x0015ea40` / 32 | 32 | `experimental/sources/w12-skill-c-init-ramps-hate-beam/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-hate-beam/host_check.cpp` | `f14114a26c694c6a510ea01fcd48505cf5e6ee8b2b617424ee4c463306203501` |
| `initRamps__15SkillHateEffect` | `0x0015eb40` / 32 | 32 | `experimental/sources/w12-skill-c-init-ramps-hate-effect/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-hate-effect/host_check.cpp` | `e68a44c87632cc9dd7334d5a966af52f00362985c647427c3004beab5a377d1d` |
| `initRamps__18SkillPoisonWeapons` | `0x0015e9f0` / 32 | 32 | `experimental/sources/w12-skill-c-init-ramps-poison-weapons/init_ramps.cpp`; host test `experimental/sources/w12-skill-c-init-ramps-poison-weapons/host_check.cpp` | `dfc173f94468b33ba823303494f239a8f62721689d5f40a73598d5dbf077fecb` |
| `getLevelRequirement__17SkillResistPoisoni` | `0x00313d18` / 8 | 8 | `experimental/sources/w12-skill-c-resist-poison-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w12-skill-c-resist-poison-level-requirement/host_check.cpp` | `8a70fc9f7376de0317ee2761b0bc6d678b632697abc169424cd18f2dfd5c7daf` |
| `getSkillPointCost__17SkillResistPoisoni` | `0x00313d10` / 8 | 8 | `experimental/sources/w12-skill-c-resist-poison-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w12-skill-c-resist-poison-point-cost/host_check.cpp` | `438d35e6b62f598f445ad2d4ad3d0689e0f8a60eb22a2925abf5f4feae4b598e` |
| `getLevelRequirement__13SkillSlashingi` | `0x00313c30` / 8 | 8 | `experimental/sources/w12-skill-c-slashing-level-requirement/get_level_requirement.cpp`; host test `experimental/sources/w12-skill-c-slashing-level-requirement/host_check.cpp` | `8e317681e20fd95bef665999c80ed1935cb4bb6ee433e53df010489867b545bf` |
| `getSkillPointCost__13SkillSlashingi` | `0x00313c28` / 8 | 8 | `experimental/sources/w12-skill-c-slashing-point-cost/get_skill_point_cost.cpp`; host test `experimental/sources/w12-skill-c-slashing-point-cost/host_check.cpp` | `d79579ad1b15aea0dac46fd634ca9c0e6b2c2b4facd8ed7811225154a5c1d352` |
| `tweekRank__5Skillii` | `0x00313780` / 8 | 8 | `experimental/sources/w12-skill-c-tweek-rank/tweek_rank.cpp`; host test `experimental/sources/w12-skill-c-tweek-rank/host_check.cpp` | `6ec28c2d9bfea4a35a20d0f346442c954e864460695f83a38ea46794da8c32f1` |

Host contracts passed for all 19 functions. Scalar checks cover signed boundary values and wraparound shift inputs. Ramp initialization checks every byte of guarded aligned fixtures. Cast checks zero returns with inaccessible receiver/creature pointers and valid by-value point inputs. The contracts do not establish invalid-pointer fault equivalence or execution on the PS2.

## Inferred

- Base and three class override point costs return their integer argument unchanged.
- Base level requirement returns zero; Blunt, Slashing and Resist Poison level requirements shift the low 32 input bits left by two.
- Base energy cost returns `1.0f`; `tweekRank` returns its second integer argument.
- The authored base cast and piggyback cast return zero for valid arguments and produce no observed state writes.
- The six authored per-class ramp initializers write only the documented halfword and float values at offsets in their source comments; the base initializer performs five repetitions at three offsets. The byte offsets are observed; member names and a complete class layout are unknown.

## Imports closed

`cast__5SkilliP8CreatureG6Point3s`
`getEnergyCost__5Skilli`
`getLevelRequirement__5Skilli`
`piggyBackCast__5SkilliP8CreatureG6Point3s`
`getSkillPointCost__5Skilli`
`getLevelRequirement__10SkillBlunti`
`getSkillPointCost__10SkillBlunti`
`initRamps__15SkillBallOfHate`
`initRamps__5Skill`
`initRamps__18SkillDiseaseEffect`
`initRamps__19SkillDiseaseWeapons`
`initRamps__13SkillHateBeam`
`initRamps__15SkillHateEffect`
`initRamps__18SkillPoisonWeapons`
`getLevelRequirement__17SkillResistPoisoni`
`getSkillPointCost__17SkillResistPoisoni`
`getLevelRequirement__13SkillSlashingi`
`getSkillPointCost__13SkillSlashingi`
`tweekRank__5Skillii`

These 19 names have authored definitions in this branch. They are not yet selected by the shared plan, so no post-integration import-count reduction is claimed.

## Imports opened

None. The 19 implementation translation units have no external symbol references.

## Build-plan entries needed

The integrator should add these entries under the private plan update. Each source uses `authored_cpp`, has empty `header_closure`, and needs the matching hash in the plan `files` map. The integrator assigns `input_order`.

```json
{"emitted_functions":[{"compiled_body_bytes":8,"name":"cast__5SkilliP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"cast.o","profile":"authored_cpp","sha256":"86a83d14bc1cce2ff41093d2a7f85c0c9aa3bc9d57d0724bb6fd4a170e560ef7","source":"sources/w12-skill-c-base-cast/cast.cpp"},
{"emitted_functions":[{"compiled_body_bytes":12,"name":"getEnergyCost__5Skilli"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_energy_cost.o","profile":"authored_cpp","sha256":"738d8bb6aa433f85b4e99f27482e1d1692783095a941a47dbbb9931d6d4f8e3e","source":"sources/w12-skill-c-base-energy-cost/get_energy_cost.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getLevelRequirement__5Skilli"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_level_requirement.o","profile":"authored_cpp","sha256":"2d798d9d99254a95dc7abb2fb36d16bd3e7f378f19b305fe55ec8801f909330d","source":"sources/w12-skill-c-base-level-requirement/get_level_requirement.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"piggyBackCast__5SkilliP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"piggyback_cast.o","profile":"authored_cpp","sha256":"f35c8269ab75757ced83981edeb2ccd2ff635b6549b9f882474f7977ad9f89db","source":"sources/w12-skill-c-base-piggyback-cast/piggyback_cast.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getSkillPointCost__5Skilli"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_skill_point_cost.o","profile":"authored_cpp","sha256":"e15fe82c7f4a11da37131f4e985a6f57a980b1305be2170b291168557461b95c","source":"sources/w12-skill-c-base-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getLevelRequirement__10SkillBlunti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_level_requirement.o","profile":"authored_cpp","sha256":"fe74c131e903a8b9dd14e54a32c992e47dcb734eb99f281080a12a1a1c2723f5","source":"sources/w12-skill-c-blunt-level-requirement/get_level_requirement.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getSkillPointCost__10SkillBlunti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_skill_point_cost.o","profile":"authored_cpp","sha256":"5693b5d4a1a5384643351c21c7ccc088dc752cbe1f3a22385da3cffdbc4a4e47","source":"sources/w12-skill-c-blunt-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions":[{"compiled_body_bytes":32,"name":"initRamps__15SkillBallOfHate"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"3d2095075a595cf93baadef362c8f474e884c61bc2bea1db4f33c41d06cb7d04","source":"sources/w12-skill-c-init-ramps-ball-of-hate/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":60,"name":"initRamps__5Skill"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"c95746d6a7ad1306988fb3f750356f643a44e5c27d23aead2d046eb779d889b1","source":"sources/w12-skill-c-init-ramps-base/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":44,"name":"initRamps__18SkillDiseaseEffect"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"5b6b1ec563bdd13172b31c412822918b50b4ccc27b37ba78acef973fe639b479","source":"sources/w12-skill-c-init-ramps-disease-effect/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":32,"name":"initRamps__19SkillDiseaseWeapons"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"16462c39c6bc401b6830d2cdd7b3cba15c721e8da80a7bb216c8e41358426dc6","source":"sources/w12-skill-c-init-ramps-disease-weapons/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":32,"name":"initRamps__13SkillHateBeam"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"f14114a26c694c6a510ea01fcd48505cf5e6ee8b2b617424ee4c463306203501","source":"sources/w12-skill-c-init-ramps-hate-beam/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":32,"name":"initRamps__15SkillHateEffect"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"e68a44c87632cc9dd7334d5a966af52f00362985c647427c3004beab5a377d1d","source":"sources/w12-skill-c-init-ramps-hate-effect/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":32,"name":"initRamps__18SkillPoisonWeapons"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_ramps.o","profile":"authored_cpp","sha256":"dfc173f94468b33ba823303494f239a8f62721689d5f40a73598d5dbf077fecb","source":"sources/w12-skill-c-init-ramps-poison-weapons/init_ramps.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getLevelRequirement__17SkillResistPoisoni"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_level_requirement.o","profile":"authored_cpp","sha256":"8a70fc9f7376de0317ee2761b0bc6d678b632697abc169424cd18f2dfd5c7daf","source":"sources/w12-skill-c-resist-poison-level-requirement/get_level_requirement.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getSkillPointCost__17SkillResistPoisoni"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_skill_point_cost.o","profile":"authored_cpp","sha256":"438d35e6b62f598f445ad2d4ad3d0689e0f8a60eb22a2925abf5f4feae4b598e","source":"sources/w12-skill-c-resist-poison-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getLevelRequirement__13SkillSlashingi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_level_requirement.o","profile":"authored_cpp","sha256":"8e317681e20fd95bef665999c80ed1935cb4bb6ee433e53df010489867b545bf","source":"sources/w12-skill-c-slashing-level-requirement/get_level_requirement.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getSkillPointCost__13SkillSlashingi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_skill_point_cost.o","profile":"authored_cpp","sha256":"d79579ad1b15aea0dac46fd634ca9c0e6b2c2b4facd8ed7811225154a5c1d352","source":"sources/w12-skill-c-slashing-point-cost/get_skill_point_cost.cpp"},
{"emitted_functions":[{"compiled_body_bytes":8,"name":"tweekRank__5Skillii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"tweek_rank.o","profile":"authored_cpp","sha256":"6ec28c2d9bfea4a35a20d0f346442c954e864460695f83a38ea46794da8c32f1","source":"sources/w12-skill-c-tweek-rank/tweek_rank.cpp"},
```

## Authored but never executed

All 19 authored functions compiled with the reviewed EE toolchain and passed their host contracts. None ran on an EE, through game dispatch, or in a game session. The emitted functions are not claimed as byte matches.

## Unknown and unfinished

The remaining 12 requested imports are not authored:
- `energyCostRamp4__5Skillii`
- `evaluateCreatureRampAtRank__5Skillii`
- `evaluateRampAtRank__5Skillii`
- `evaluateRampAtRank__5SkilliiP10GameObject`
- `evaluateRampAtRank__5SkilliiP8ListHead`
- `getDeflectionChance__10SkillDodgei`
- `getIcon__5Skill`
- `getRampDamage__5SkilliifiPiT5`
- `getRankDescription__10SkillBluntPUsi`
- `getRankDescription__13SkillSlashingPUsi`
- `getRankDescription__17SkillResistPoisonPUsi`
- `getRankDescription__5SkillPUsi`

Their behavior remains unknown here. The rank-description and icon methods depend on private text/resource data; the ramp evaluators and damage/deflection methods need more verified ramp/class layout and interaction evidence. No source placeholders or guessed data were added. Object layouts beyond observed stores, gameplay ranges, runtime dispatch, and private-to-public plan integration remain unknown. No game runtime was executed.
