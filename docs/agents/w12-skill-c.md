# Skill class family (w12-skill-c)

Branch: `agents/w12-skill-c`. Work is limited to the Skill class method/ramp family. The private reference is the hash-pinned SLUS-20565 executable (`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`); no binary, raw disassembly or generated decompiler output is included here.

## Observed

- `getSkillPointCost__5Skilli`, `getLevelRequirement__5Skilli`, `getEnergyCost__5Skilli` at `0x00313248` has an 8-byte extent. Its two instructions return the second integer argument unchanged. The receiver is not read.
- The authored source defines that symbol and is self-contained. The reviewed EE toolchain compiled it as an 8-byte function. Its generated second instruction uses `or` where the original uses `addu`; this is a semantic reconstruction, not a byte match.
- The host contract calls it for `INT_MIN`, negative, zero, positive, and `INT_MAX` inputs and checks the returned integer equals the argument. The host check passed. The test and source are `experimental/sources/w12-skill-c-base-point-cost/`.
- `getLevelRequirement__5Skilli` at `0x00313250` has an 8-byte extent. It returns zero for every integer argument and does not read the receiver. Its reviewed EE compilation succeeded; the host contract passed on seven boundary and representative integer inputs. The test and source are `experimental/sources/w12-skill-c-base-level-requirement/`.
- `getEnergyCost__5Skilli` at `0x00313258` has a 16-byte extent and returns `1.0f` for every integer argument. Its reviewed EE compilation succeeded with a 12-byte body; the host contract passed on five representative/boundary inputs. The body-size difference is not an exact match. The test and source are `experimental/sources/w12-skill-c-base-energy-cost/`.
- A fresh partial build of this branch selected 5,113 input files and reported 827 unresolved symbols. All 31 symbols in the requested Skill list were unresolved at that baseline.

## Inferred

- The member behaves as a rank-to-point-cost identity mapping for every 32-bit input. No gameplay range or use is established.
- The base level-requirement method returns zero for all 32-bit inputs. Its gameplay interpretation is not established.
- The base energy-cost method returns the single-precision value 1.0 for every 32-bit input. Its gameplay interpretation is not established.

## Unknown

- Runtime dispatch, gameplay meaning, object layout, and all other Skill methods/ramp behavior remain unverified. No game execution was performed.

## Imports closed

`getSkillPointCost__5Skilli`

## Imports opened

None. The implementation has no external references.

## Build-plan entries needed

Add three `independently_authored_source` inputs using profile `authored_cpp`, each with empty `header_closure`: `sources/w12-skill-c-base-point-cost/get_skill_point_cost.cpp` (`get_skill_point_cost.o`, symbol `getSkillPointCost__5Skilli`, 8 compiled bytes, SHA-256 `e15fe82c7f4a11da37131f4e985a6f57a980b1305be2170b291168557461b95c`) and `sources/w12-skill-c-base-level-requirement/get_level_requirement.cpp` (`get_level_requirement.o`, symbol `getLevelRequirement__5Skilli`, 8 compiled bytes, SHA-256 `2d798d9d99254a95dc7abb2fb36d16bd3e7f378f19b305fe55ec8801f909330d`), and `sources/w12-skill-c-base-energy-cost/get_energy_cost.cpp` (`get_energy_cost.o`, symbol `getEnergyCost__5Skilli`, 12 compiled bytes, SHA-256 `738d8bb6aa433f85b4e99f27482e1d1692783095a941a47dbbb9931d6d4f8e3e`). Add all three hashes to `files`; the integrator assigns `input_order` and updates snapshot metadata.

## Authored but never executed

All three methods were compiled by the reviewed EE toolchain and exercised through their host contracts. None was run on the EE or through game dispatch.

- `getSkillPointCost__10SkillBlunti` at `0x00313b38` has an 8-byte extent and returns its integer argument unchanged. The reviewed EE build emitted an 8-byte function; its host contract passed on boundary and representative inputs. Source/test: `experimental/sources/w12-skill-c-blunt-point-cost/`. No byte match or game execution is claimed.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `experimental/sources/w12-skill-c-blunt-point-cost/get_skill_point_cost.cpp`, object `get_skill_point_cost.o`, empty `header_closure`, emitted function `getSkillPointCost__10SkillBlunti` (8 bytes), SHA-256 `5693b5d4a1a5384643351c21c7ccc088dc752cbe1f3a22385da3cffdbc4a4e47`; add the source hash to `files` and let the integrator assign order.

- Inferred: `getSkillPointCost__10SkillBlunti` provides identity mapping for all 32-bit inputs; gameplay range/use remains unknown.

- `getLevelRequirement__10SkillBlunti` at `0x00313b40` has an 8-byte extent and returns the low 32 bits shifted left by two. The reviewed EE build emitted an 8-byte function; its host contract passed on boundary and representative inputs. Source/test: `experimental/sources/w12-skill-c-blunt-level-requirement/`. No byte match or game execution is claimed.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `experimental/sources/w12-skill-c-blunt-level-requirement/get_level_requirement.cpp`, object `get_level_requirement.o`, empty `header_closure`, emitted function `getLevelRequirement__10SkillBlunti` (8 bytes), SHA-256 `fe74c131e903a8b9dd14e54a32c992e47dcb734eb99f281080a12a1a1c2723f5`; add the source hash to `files` and let the integrator assign order.

- Inferred: `getLevelRequirement__10SkillBlunti` shifts the 32-bit input left by two with wraparound; gameplay range/use remains unknown.

- `getSkillPointCost__13SkillSlashingi` at `0x00313c28` has an 8-byte extent and returns its integer argument unchanged. The reviewed EE build emitted an 8-byte function; its host contract passed on boundary and representative inputs. Source/test: `experimental/sources/w12-skill-c-slashing-point-cost/`. No byte match or game execution is claimed.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `experimental/sources/w12-skill-c-slashing-point-cost/get_skill_point_cost.cpp`, object `get_skill_point_cost.o`, empty `header_closure`, emitted function `getSkillPointCost__13SkillSlashingi` (8 bytes), SHA-256 `d79579ad1b15aea0dac46fd634ca9c0e6b2c2b4facd8ed7811225154a5c1d352`; add the source hash to `files` and let the integrator assign order.

- Inferred: `getSkillPointCost__13SkillSlashingi` provides identity mapping for all 32-bit inputs; gameplay range/use remains unknown.
