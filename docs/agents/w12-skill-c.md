# Skill class family (w12-skill-c)

Branch: `agents/w12-skill-c`. Work is limited to the Skill class method/ramp family. The private reference is the hash-pinned SLUS-20565 executable (`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`); no binary, raw disassembly or generated decompiler output is included here.

## Observed

- `getSkillPointCost__5Skilli`, `getLevelRequirement__5Skilli` at `0x00313248` has an 8-byte extent. Its two instructions return the second integer argument unchanged. The receiver is not read.
- The authored source defines that symbol and is self-contained. The reviewed EE toolchain compiled it as an 8-byte function. Its generated second instruction uses `or` where the original uses `addu`; this is a semantic reconstruction, not a byte match.
- The host contract calls it for `INT_MIN`, negative, zero, positive, and `INT_MAX` inputs and checks the returned integer equals the argument. The host check passed. The test and source are `experimental/sources/w12-skill-c-base-point-cost/`.
- `getLevelRequirement__5Skilli` at `0x00313250` has an 8-byte extent. It returns zero for every integer argument and does not read the receiver. Its reviewed EE compilation succeeded; the host contract passed on seven boundary and representative integer inputs. The test and source are `experimental/sources/w12-skill-c-base-level-requirement/`.
- A fresh partial build of this branch selected 5,113 input files and reported 827 unresolved symbols. All 31 symbols in the requested Skill list were unresolved at that baseline.

## Inferred

- The member behaves as a rank-to-point-cost identity mapping for every 32-bit input. No gameplay range or use is established.
- The base level-requirement method returns zero for all 32-bit inputs. Its gameplay interpretation is not established.

## Unknown

- Runtime dispatch, gameplay meaning, object layout, and all other Skill methods/ramp behavior remain unverified. No game execution was performed.

## Imports closed

`getSkillPointCost__5Skilli`

## Imports opened

None. The implementation has no external references.

## Build-plan entries needed

Add two `independently_authored_source` inputs using profile `authored_cpp`, each with empty `header_closure`: `sources/w12-skill-c-base-point-cost/get_skill_point_cost.cpp` (`get_skill_point_cost.o`, symbol `getSkillPointCost__5Skilli`, 8 compiled bytes, SHA-256 `e15fe82c7f4a11da37131f4e985a6f57a980b1305be2170b291168557461b95c`) and `sources/w12-skill-c-base-level-requirement/get_level_requirement.cpp` (`get_level_requirement.o`, symbol `getLevelRequirement__5Skilli`, 8 compiled bytes, SHA-256 `2d798d9d99254a95dc7abb2fb36d16bd3e7f378f19b305fe55ec8801f909330d`). Add both hashes to `files`; the integrator assigns `input_order` and updates snapshot metadata.

## Authored but never executed

Both methods were compiled by the reviewed EE toolchain and exercised through their host contracts. Neither was run on the EE or through game dispatch.
