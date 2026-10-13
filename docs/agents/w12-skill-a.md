# Skill class family (A): w12-skill-a

Branch `agents/w12-skill-a`. Thirty-one data/function units, one per commit, closing the
nine `__tf` type-info functions and twenty-two `_vt$` tables of the package, plus 22 more `__tf`
functions that the new tables reference. Addresses are from the private image whose hash is the
snapshot `target_sha256`; nothing from the image is committed beyond symbol names, addresses,
sizes and the class-name strings the functions pass.

## Observed

- Every `__tf<Class>` is a type-info function: it returns the 12-byte BSS node `__ti<Class>`. When
  the node's first word is zero it registers the class: `__rtti_si(node, "<Class name>", &__ti5Skill)`.
  The base node `__ti5Skill` is an 8-byte small-data object at 0x00486dd0.
- Two shapes. Shape A (`SkillBlunt`, `SkillSlashing`, `SkillResistPoison`): the base setup is inlined,
  that is, when `__ti5Skill`'s first word is zero the function first calls `__rtti_user(&__ti5Skill, "5Skill")`.
  Shape B (the other 28): the function calls `__tf5Skill` first. Nothing else differs.
- The 22 `_vt$` tables (SkillDodge ... SkillHolyStrike) are 144 bytes, 18 slots, zero adjustment words,
  last function word zero; slot 0 is the class `__tf`, slots 1..16 are Skill methods. Same layout as the three
  tables in `docs/agents/w11-vt-a.md`. Slot-by-slot names are in each `vt_*.cpp`.
- Several tables reuse base Skill methods (`getEnergyCost__5Skilli`, `initRamps__5Skill` ...) and
  others name class-specific ones (`cast__<Class>...`, `getIcon__<Class>`, `initRamps__<Class>`).

## Inferred

- A node is the class's run-time type descriptor and `Skill` is its single base. The constructor of each
  class stores its table's address at object+72 (stated by earlier packages for other classes); not exercised.

## Unknown

- Dispatch through any table, the meaning of the null last slot, which function first fills `__ti5Skill`
  (`__tf5Skill` is not authored here), and the weak binding of the image's `_vt$` symbols (not reproduced).
- Compiled bodies are not image-sized: the image reaches the base node through the global pointer, this
  build (`-G0`) uses an address pair. No byte match is claimed.

## Checks performed

- Host (all 31 `__tf` units): `host_check.cpp` stubs `__rtti_si`/`__rtti_user`/`__tf5Skill`, builds with
  `c++ -std=c++17 -Wall -Wextra -Werror`, and checks the returned node, the exact call sequence for an empty node,
  no calls for any nonzero byte of the first word, and that nonzero bytes outside the first word do not skip. All pass.
  Inverting the first-word test (`== 0u` to `!= 0u`) failed the check (then restored).
- Paired run (private R5900 interpreter, outside git): each authored `__tf` against the image function from
  identical states (outer node 0/1/0x100 x base node 0/1/0x7f000000), hooked `__rtti_si`/`__rtti_user`/`__tf5Skill`
  logging arguments and string contents, plus the changed outer word: 9 scenarios per function, zero mismatches,
  no HALT. Changing the string `"5Skill"` in SkillBlunt made one scenario fail (then restored).
- Tables: host check calls every slot through the table. `docs/agents/w12-skill-a-object-check.py` compiles each with
  the reviewed `authored_cpp` flags and compares with the image the symbol size, zero adjustments and that every
  relocated slot names the symbol at the same slot of the image table: 22 PASS; renaming one slot (SkillDodge
  `getEnergyCost`) made it FAIL (then restored).
- EE compile of every unit with the reviewed flags. Private partial build with all 53 entries added to a copy of the
  plan outside git (`docs/agents/w12-skill-a-plan-entries.py --private-root`): see "Private build result" below.

## Private build result

With all 53 entries added to a plan copy outside git (5166 selected inputs, 3940 function bodies, zero COMMON), the
unresolved count is 926. Compared with the same copy holding only the first 31 entries (948), the 22 `__tf` imports
opened by the new tables are gone; none of the 31 package names, and no `__tf*Skill*` other than `__tf5Skill`, remain
unresolved. The other `_vt$*Skill*` names are the sibling package's. The totals are not comparable to the shared
branch's 827, because the private copy was built from this worktree's plan and sources with the newly added entries.

## Imports closed

`__tf10SkillBlunt`, `__tf13SkillHateBeam`, `__tf13SkillSlashing`, `__tf15SkillBallOfHate`, `__tf15SkillHateEffect`,
`__tf17SkillResistPoison`, `__tf18SkillDiseaseEffect`, `__tf18SkillPoisonWeapons`, `__tf19SkillDiseaseWeapons`,
`_vt$10SkillDodge`, `_vt$10SkillShock`, `_vt$11SkillCharge`, `_vt$12SkillArchery`, `_vt$12SkillBlessed`,
`_vt$12SkillCyclone`, `_vt$12SkillHealing`, `_vt$12SkillLifeTap`, `_vt$12SkillRiposte`, `_vt$13SkillDarkness`,
`_vt$13SkillEntangle`, `_vt$14SkillColdArrow`, `_vt$14SkillFireArrow`, `_vt$14SkillFireStorm`, `_vt$14SkillHarmTouch`,
`_vt$14SkillMultiFire`, `_vt$15SkillClericRoot`, `_vt$15SkillConeOfFire`, `_vt$15SkillFireFlares`,
`_vt$15SkillFrostStorm`, `_vt$15SkillHolyShield`, `_vt$15SkillHolyStrike`.

Also closed (opened by the tables, then defined here): `__tf10SkillDodge`, `__tf10SkillShock`, `__tf11SkillCharge`,
`__tf12SkillArchery`, `__tf12SkillBlessed`, `__tf12SkillCyclone`, `__tf12SkillHealing`, `__tf12SkillLifeTap`,
`__tf12SkillRiposte`, `__tf13SkillDarkness`, `__tf13SkillEntangle`, `__tf14SkillColdArrow`, `__tf14SkillFireArrow`,
`__tf14SkillFireStorm`, `__tf14SkillHarmTouch`, `__tf14SkillMultiFire`, `__tf15SkillClericRoot`,
`__tf15SkillConeOfFire`, `__tf15SkillFireFlares`, `__tf15SkillFrostStorm`, `__tf15SkillHolyShield`,
`__tf15SkillHolyStrike`. Each `__tf` unit also defines the 12-byte BSS `__ti<Class>` node.

## Imports opened

New by this package: `__ti5Skill` (the 8-byte base node, shared by all 31 functions) and `__tf5Skill` (Shape B, 28 callers),
and `__rtti_user`/`__rtti_si` where not already imported. The tables also name Skill methods that are other packages'
work (`canCast__5SkillP8Creature`, `cast__5SkilliP8CreatureG6Point3s`, `piggyBackCast__5Skill...`,
`getRankDescription__5SkillPUsi`, `getSkillPointCost__5Skilli`, `getLevelRequirement__5Skilli`, `getEnergyCost__5Skilli`,
`getIcon__5Skill`, `getRampDamage__5SkilliifiPiT5`, `initRamps__5Skill`, the four `evaluate*RampAtRank__5Skill...`,
`tweekRank__5Skillii`, `energyCostRamp4__5Skillii`) and class-specific `canCast__/cast__/getEnergyCost__/getIcon__/
getRankDescription__/getLevelRequirement__/getSkillPointCost__/initRamps__` names for the 22 classes (about 140
names; see the private-build unresolved list). Several were already imports of the constructors.

## Build-plan entries needed

Run `python3 docs/agents/w12-skill-a-plan-entries.py --toolchain <ee> [--private-root DIR]` for the 53 exact JSON entries
(one per `experimental/sources/w12-skill-a-{tf,vt}-*/` unit; empty `header_closure`, profile `authored_cpp`; the `tf`
entries emit one function each). Raise `default_authored_inputs` by 53 and add each source hash to the `files` maps.

## Authored but never executed

All 22 tables: only host stubs and relocation comparison; no table was read by game code or the interpreter.
The 31 `__tf` functions ran only in the host check and in the private interpreter against the image function;
none ran in a game. `__rtti_si`, `__rtti_user` and `__tf5Skill` were stubbed in both.

## What is unknown

Dispatch through the tables and the null last slot; the function that first fills `__ti5Skill` and the body of
`__tf5Skill`; whether the weak binding of the image's `_vt$` symbols matters to the linked image; the meaning of
Skill method names beyond what the symbol names state; byte-level equality of the compiled `__tf` bodies (the image
uses global-pointer-relative access that the `-G0` profile cannot emit).
