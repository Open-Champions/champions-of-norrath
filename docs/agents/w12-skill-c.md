# Skill class family (w12-skill-c)

Branch: `agents/w12-skill-c`. Work is limited to the Skill class method/ramp family. The private reference is the hash-pinned SLUS-20565 executable (`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`); no binary, raw disassembly or generated decompiler output is included here.

## Observed

- `getSkillPointCost__5Skilli` at `0x00313248` has an 8-byte extent. Its two instructions return the second integer argument unchanged. The receiver is not read.
- The authored source defines that symbol and is self-contained. The reviewed EE toolchain compiled it as an 8-byte function. Its generated second instruction uses `or` where the original uses `addu`; this is a semantic reconstruction, not a byte match.
- The host contract calls it for `INT_MIN`, negative, zero, positive, and `INT_MAX` inputs and checks the returned integer equals the argument. The host check passed. The test and source are `experimental/sources/w12-skill-c-base-point-cost/`.
- A fresh partial build of this branch selected 5,113 input files and reported 827 unresolved symbols. All 31 symbols in the requested Skill list were unresolved at that baseline.

## Inferred

- The member behaves as a rank-to-point-cost identity mapping for every 32-bit input. No gameplay range or use is established.

## Unknown

- Runtime dispatch, gameplay meaning, object layout, and all other Skill methods/ramp behavior remain unverified. No game execution was performed.

## Imports closed

`getSkillPointCost__5Skilli`

## Imports opened

None. The implementation has no external references.

## Build-plan entries needed

Add one `independently_authored_source` input using profile `authored_cpp`, source `sources/w12-skill-c-base-point-cost/get_skill_point_cost.cpp`, object name `get_skill_point_cost.o`, empty `header_closure`, emitted function `getSkillPointCost__5Skilli` with `compiled_body_bytes: 8`, and source SHA-256 `e15fe82c7f4a11da37131f4e985a6f57a980b1305be2170b291168557461b95c`. Add the same source hash to `files`; the integrator assigns `input_order` and updates snapshot metadata.

## Authored but never executed

`getSkillPointCost__5Skilli` was compiled by the reviewed EE toolchain and exercised through the host contract. It was not run on the EE or through game dispatch.
