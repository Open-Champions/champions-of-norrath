# Skill class family (w12-skill-c)

Branch: `agents/w12-skill-c`. Work is limited to the Skill class method/ramp family. The private reference is the hash-pinned SLUS-20565 executable (`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`); no binary, raw disassembly or generated decompiler output is included here.

## Observed

- `getSkillPointCost__5Skilli`, `getLevelRequirement__5Skilli`, `getEnergyCost__5Skilli`, `tweekRank__5Skillii`, `initRamps__18SkillDiseaseEffect`, `initRamps__5Skill`, `cast__5SkilliP8CreatureG6Point3s` at `0x00313248` has an 8-byte extent. Its two instructions return the second integer argument unchanged. The receiver is not read.
- The authored source defines that symbol and is self-contained. The reviewed EE toolchain compiled it as an 8-byte function. Its generated second instruction uses `or` where the original uses `addu`; this is a semantic reconstruction, not a byte match.
- The host contract calls it for `INT_MIN`, negative, zero, positive, and `INT_MAX` inputs and checks the returned integer equals the argument. The host check passed. The test and source are `experimental/sources/w12-skill-c-base-point-cost/`.
- `getLevelRequirement__5Skilli` at `0x00313250` has an 8-byte extent. It returns zero for every integer argument and does not read the receiver. Its reviewed EE compilation succeeded; the host contract passed on seven boundary and representative integer inputs. The test and source are `experimental/sources/w12-skill-c-base-level-requirement/`.
- `getEnergyCost__5Skilli` at `0x00313258` has a 16-byte extent and returns `1.0f` for every integer argument. Its reviewed EE compilation succeeded with a 12-byte body; the host contract passed on five representative/boundary inputs. The body-size difference is not an exact match. The test and source are `experimental/sources/w12-skill-c-base-energy-cost/`.
- A fresh partial build of this branch selected 5,113 input files and reported 827 unresolved symbols. All 31 symbols in the requested Skill list were unresolved at that baseline.

## Inferred

- The member behaves as a rank-to-point-cost identity mapping for every 32-bit input. No gameplay range or use is established.
- The base level-requirement method returns zero for all 32-bit inputs. Its gameplay interpretation is not established.
- The base energy-cost method returns the single-precision value 1.0 for every 32-bit input. Its gameplay interpretation is not established. The base rank tweak returns its second integer argument unchanged; effect on the first integer is absent in the observed routine.

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

- `getLevelRequirement__13SkillSlashingi` at `0x00313c30` has an 8-byte extent and returns the low 32 bits shifted left by two. The reviewed EE build emitted an 8-byte function; its host contract passed on boundary and representative inputs. Source/test: `experimental/sources/w12-skill-c-slashing-level-requirement/`. No byte match or game execution is claimed.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `experimental/sources/w12-skill-c-slashing-level-requirement/get_level_requirement.cpp`, object `get_level_requirement.o`, empty `header_closure`, emitted function `getLevelRequirement__13SkillSlashingi` (8 bytes), SHA-256 `8e317681e20fd95bef665999c80ed1935cb4bb6ee433e53df010489867b545bf`; add the source hash to `files` and let the integrator assign order.

- Inferred: `getLevelRequirement__13SkillSlashingi` shifts the 32-bit input left by two with wraparound; gameplay range/use remains unknown.

- `getSkillPointCost__17SkillResistPoisoni` at `0x00313d10` has an 8-byte extent and returns its integer argument unchanged. The reviewed EE build emitted an 8-byte function; its host contract passed on boundary and representative inputs. Source/test: `experimental/sources/w12-skill-c-resist-poison-point-cost/`. No byte match or game execution is claimed.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `experimental/sources/w12-skill-c-resist-poison-point-cost/get_skill_point_cost.cpp`, object `get_skill_point_cost.o`, empty `header_closure`, emitted function `getSkillPointCost__17SkillResistPoisoni` (8 bytes), SHA-256 `438d35e6b62f598f445ad2d4ad3d0689e0f8a60eb22a2925abf5f4feae4b598e`; add the source hash to `files` and let the integrator assign order.

- Inferred: `getSkillPointCost__17SkillResistPoisoni` provides identity mapping for all 32-bit inputs; gameplay range/use remains unknown.

- `getLevelRequirement__17SkillResistPoisoni` at `0x00313d18` has an 8-byte extent and returns the low 32 bits shifted left by two. The reviewed EE build emitted an 8-byte function; its host contract passed on boundary and representative inputs. Source/test: `experimental/sources/w12-skill-c-resist-poison-level-requirement/`. No byte match or game execution is claimed.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `experimental/sources/w12-skill-c-resist-poison-level-requirement/get_level_requirement.cpp`, object `get_level_requirement.o`, empty `header_closure`, emitted function `getLevelRequirement__17SkillResistPoisoni` (8 bytes), SHA-256 `8a70fc9f7376de0317ee2761b0bc6d678b632697abc169424cd18f2dfd5c7daf`; add the source hash to `files` and let the integrator assign order.

- Inferred: `getLevelRequirement__17SkillResistPoisoni` shifts the 32-bit input left by two with wraparound; gameplay range/use remains unknown.

- `tweekRank__5Skillii` at `0x00313780` has an 8-byte extent and returns its second integer argument unchanged; its receiver and first integer are not read. Reviewed EE compilation emitted 8 bytes; the host test passed the cross-product of six boundary and representative values for both integer arguments. Source/test: `experimental/sources/w12-skill-c-tweek-rank/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-tweek-rank/tweek_rank.cpp`, object `tweek_rank.o`, empty `header_closure`, emitted function `tweekRank__5Skillii` (8 bytes), SHA-256 `6ec28c2d9bfea4a35a20d0f346442c954e864460695f83a38ea46794da8c32f1`; add the source hash to `files` and let the integrator assign order.

- `initRamps__13SkillHateBeam` at `0x0015ea40` has a 32-byte extent and writes halfword 1 at receiver+56, single-precision 2.0 at +76, and halfword 2 at +66. The host test checks these bytes and that every other byte in an 80-byte aligned fixture remains unchanged. Reviewed EE compilation succeeded; generated extent: 32 bytes. Source/test: `experimental/sources/w12-skill-c-init-ramps-hate-beam/`. No field names are inferred; no runtime/dispatch execution.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-hate-beam/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__13SkillHateBeam`, SHA-256 `f14114a26c694c6a510ea01fcd48505cf5e6ee8b2b617424ee4c463306203501`; add source hash to `files`, then integrator assigns order.

- `initRamps__18SkillPoisonWeapons` at `0x0015e9f0` has a 32-byte extent and writes halfword 1 at receiver+56, single-precision 2.0 at +76, and halfword 2 at +66. The host test checks these bytes and that every other byte in an 80-byte aligned fixture remains unchanged. Reviewed EE compilation succeeded; generated extent: 32 bytes. Source/test: `experimental/sources/w12-skill-c-init-ramps-poison-weapons/`. No field names are inferred; no runtime/dispatch execution.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-poison-weapons/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__18SkillPoisonWeapons`, SHA-256 `dfc173f94468b33ba823303494f239a8f62721689d5f40a73598d5dbf077fecb`; add source hash to `files`, then integrator assigns order.

- `initRamps__15SkillHateEffect` at `0x0015eb40` has a 32-byte extent and writes halfword 1 at receiver+56, single-precision 2.0 at +76, and halfword 2 at +66. The host test checks those bytes and confirms all other fixture bytes are unchanged. Reviewed EE compilation succeeded; object reports: `00000000 00000020 T initRamps__15SkillHateEffect`. Source/test: `experimental/sources/w12-skill-c-init-ramps-hate-effect/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-hate-effect/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__15SkillHateEffect`, SHA-256 `e68a44c87632cc9dd7334d5a966af52f00362985c647427c3004beab5a377d1d`; add source hash to `files`, then assign order in integration.

- `initRamps__15SkillBallOfHate` at `0x0015ea90` has a 32-byte extent and writes halfword 1 at receiver+56, single-precision 2.0 at +76, and halfword 2 at +66. The host test checks those bytes and confirms all other fixture bytes are unchanged. Reviewed EE compilation succeeded; object reports: `00000000 00000020 T initRamps__15SkillBallOfHate`. Source/test: `experimental/sources/w12-skill-c-init-ramps-ball-of-hate/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-ball-of-hate/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__15SkillBallOfHate`, SHA-256 `3d2095075a595cf93baadef362c8f474e884c61bc2bea1db4f33c41d06cb7d04`; add source hash to `files`, then assign order in integration.

- `initRamps__19SkillDiseaseWeapons` at `0x0015e9a0` has a 32-byte extent and writes halfword 1 at receiver+56, single-precision 2.0 at +76, and halfword 4 at +66. The host test checks those bytes and confirms all other fixture bytes are unchanged. Reviewed EE compilation succeeded; object reports: `00000000 00000020 T initRamps__19SkillDiseaseWeapons`. Source/test: `experimental/sources/w12-skill-c-init-ramps-disease-weapons/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-disease-weapons/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__19SkillDiseaseWeapons`, SHA-256 `16462c39c6bc401b6830d2cdd7b3cba15c721e8da80a7bb216c8e41358426dc6`; add source hash to `files`, then assign order in integration.

- `initRamps__18SkillDiseaseEffect` at `0x0015eae0` has a 44-byte extent and writes halfword 1 at receiver+66, float 1.0 at +76, halfword 3 at +24, halfword 1 at +14, float 1.0 at +36, and halfword 1 at +56. The host test checks each changed byte and verifies all other bytes in an 80-byte aligned fixture are unchanged. Reviewed EE compilation emitted 44 bytes. Source/test: `experimental/sources/w12-skill-c-init-ramps-disease-effect/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-disease-effect/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__18SkillDiseaseEffect` (44 bytes), SHA-256 `5b6b1ec563bdd13172b31c412822918b50b4ccc27b37ba78acef973fe639b479`; add source hash to `files`, then assign order in integration.

- `initRamps__5Skill` at `0x00313a08` has a 76-byte extent. For five iterations it writes zero halfwords at offsets +14 through +22, float 2.0 values at +36 through +52, and halfword ones at +24 through +32. The host test checks all 80 fixture bytes. Reviewed EE compilation emitted a 60-byte function; this is not a byte match. Source/test: `experimental/sources/w12-skill-c-init-ramps-base/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-init-ramps-base/init_ramps.cpp`, object `init_ramps.o`, empty `header_closure`, emitted function `initRamps__5Skill` (60 compiled bytes), SHA-256 `c95746d6a7ad1306988fb3f750356f643a44e5c27d23aead2d046eb779d889b1`; add source hash to `files`, then assign order in integration.

- `cast__5SkilliP8CreatureG6Point3s` at `0x003131c0` has a 40-byte extent. With valid arguments it returns zero and has no receiver or creature state writes; the call signature carries `Point3` by value. The host contract uses inaccessible receiver/creature sentinels, valid point values and four ranks; it passed. Reviewed EE compilation emitted an 8-byte body. This is behavioral evidence for valid inputs, not a fault-equivalence or byte match. Source/test: `experimental/sources/w12-skill-c-base-cast/`.

Build-plan entry: `independently_authored_source`, `authored_cpp`, source `sources/w12-skill-c-base-cast/cast.cpp`, object `cast.o`, empty `header_closure`, emitted function `cast__5SkilliP8CreatureG6Point3s` (8 compiled bytes), SHA-256 `86a83d14bc1cce2ff41093d2a7f85c0c9aa3bc9d57d0724bb6fd4a170e560ef7`; add source hash to `files`, then assign order in integration.
