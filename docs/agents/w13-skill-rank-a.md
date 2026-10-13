# Skill rank description builders, part 1: w13-skill-rank-a

Branch `agents/w13-skill-rank-a`. Twenty-eight functions, one per commit, from the private image whose hash is the
snapshot `target_sha256`. Nothing from the image is committed beyond symbol names, addresses, sizes and the text
lookup keys the functions pass (the texts themselves were not read).

## Observed

- Every function is `getRankDescription__<Class>PUsi(skill, out, rank)`: it fills `out` (16-bit characters) through
  `usprintf__FPUsT0ffffffff(out, textGetString(key), f0..f7)` with up to eight float arguments, unused ones being 0.0f.
  It returns nothing.
- The values come from calls through the object's table (word at +96; 8-byte entries, signed halfword adjusting the
  object, function address at +4). Entry +88 is called as (object, ramp number, rank) and returns an int; entry +72 as
  (object, ramp, rank, 0.2f, 0, &lo, &hi) writing two ints; entry +120 as (object, rank, 20) (MultiFire only). The
  class tables name these `evaluateRampAtRank__5Skillii`, `getRampDamage__5SkilliifiPiT5` and `tweekRank__5Skillii`.
- Branching on ramp values selects among text keys (`*_rankdesc`, `*_rankdesc1/2/3`) and argument lists. Several
  functions pass `v + v*0.2f` rounded half away from zero (the constant is 0x3e4ccccc, which this toolchain's `0.2f`
  also produces). FireArrow and ColdArrow divide one ramp by 12. Entangle and ClericRoot call `getEnemyWord__Fi` on ramp 0.
- Blunt and Slashing do no table call: value = 5*rank+10 for rank > 0, else 0.

## Inferred

Names for the ramp numbers and meanings of the arguments are not inferred. The 0.2 scaling is "a 20 percent
increase" only as arithmetic.

## Unknown

What each ramp number selects, the displayed texts, the float/int type of `getEnemyWord__Fi`'s result beyond "int
converted to float", and runtime behaviour in the game. Compiled sizes differ from the image (the image uses
gp-relative constants; this profile is `-G0`); no byte match is claimed.

## Checks performed

- Each source compiles with the reviewed `authored_cpp` flags (`docs/agents/w13-skill-rank-a-plan-entries.py` compiled all 28).
- Host: each `host_check.cpp` records every stub call (table entries, text key, usprintf arguments as bit patterns) in a
  trace and compares it with an independently written model, 320 value variants x 15 ranks x 3 object adjustments =
  14400 runs. A mutation (changed ramp number, rounding offset or constant) fails every one that was tried.
- Paired run (private R5900 interpreter, outside git): each authored function against the image function from identical
  states with identical stub callees, same 14400 scenarios; call logs (including argument values and float bits) and all
  changed memory compared; zero mismatches, no HALT. Instruction coverage of the image function is complete except
  branch-delay padding words (verified to be zero words). The same mutation fails the paired run.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass. No full partial build was run.

## Imports closed

All 28 requested names: `getRankDescription__` + `10SkillBlunt`, `10SkillDodge`, `10SkillShock`, `11SkillCharge`,
`12SkillArchery`, `12SkillBlessed`, `12SkillCyclone`, `12SkillHealing`, `12SkillLifeTap`, `12SkillRiposte`,
`13SkillDarkness`, `13SkillEntangle`, `13SkillSlashing`, `14SkillColdArrow`, `14SkillFireArrow`, `14SkillFireStorm`,
`14SkillHarmTouch`, `14SkillMultiFire`, `15SkillClericRoot`, `15SkillConeOfFire`, `15SkillFireFlares`,
`15SkillFrostStorm`, `15SkillHolyShield`, `15SkillHolyStrike`, `15SkillPoisonRain`, `15SkillShieldBash`,
`15SkillWizardBeam`, `16SkillColdWeapons`, each followed by `PUsi`.

## Imports opened

`getEnemyWord__Fi` (Entangle, ClericRoot). `textGetString__FPc` and `usprintf__FPUsT0ffffffff` are used by many
sources already.

## Build-plan entries needed

`python3 docs/agents/w13-skill-rank-a-plan-entries.py --toolchain <ee> [--private-root DIR]` prints the 28 exact JSON
entries (one per `experimental/sources/w13-skill-rank-a-*/`, profile `authored_cpp`, empty `header_closure`); raise
`default_authored_inputs` by 28 and add each source hash to the `files` maps. (The script file is gitignored by
pattern; it is committed with `git add -f`.)

## Authored but never executed

Nothing ran in the game. All functions ran only in the host trace check and in the private interpreter against the
image function with stubbed table entries, text lookup, `usprintf` and `getEnemyWord__Fi`.

## What is unknown

See "Unknown". Dodge was committed first with a simpler single-ramp host check (21 paired scenarios, 3 table adjustments);
the other 27 use the larger scheme above.
