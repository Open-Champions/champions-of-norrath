# Skill rank description text builders, part 2: w13-skill-rank-b

Branch `agents/w13-skill-rank-b`. Twenty-eight functions, one per commit, each in its own source directory
`experimental/sources/w13-skill-rank-b-<name>/` with `get_rank_description.cpp` (the plan input) and `host_check.cpp`.
Addresses and sizes are from the private image whose hash is the snapshot `target_sha256`; nothing from the image is
committed beyond symbol names, addresses, sizes, and the text keys these functions pass (short identifiers such as
`bash_rankdesc1`, which are lookup keys, not game text).

## Observed

- Every function has the shape `getRankDescription(Skill* this, unsigned short* out, int rank)`, no result. Except the
  base `Skill` version (which copies the text for the key `no` with `ustrcpy__FPUsT0`) and `SkillResistPoison` (which
  never touches the object), each one reads the word at `this + 96`, treats it as a table of 8-byte entries
  (a halfword adjustment, then the function word at entry + 4), and calls:
  - the entry at table + 88 as `fn(this + adjustment, index, rank)` returning an int, and/or
  - the entry at table + 72 as `fn(this + adjustment, index, rank, 0.2f, 0, int* first, int* second)` returning two ints
    through pointers (the float 0.2 is the same pooled constant everywhere it is passed).
  In the `_vt$` tables published for the Skill classes (docs/agents/w12-skill-a.md) these two positions are the
  slots 11 and 9 (`evaluateRampAtRank__5Skillii` and `getRampDamage__5SkilliifiPiT5` in the base table).
- It then looks up one text key with `textGetString__FPc` and calls `usprintf__FPUsT0ffffffff(out, text, f x 8)` with the
  numbers as eight float arguments (unused ones zero). Which key is used, and which numbers, depends per class on the values
  returned by the calls; see each unit's header comment for the exact rule.
- Common sub-rules: several functions pass `v` and `v` plus a fifth of itself rounded to the nearest int, halves away
  from zero (the fifth is the pooled 0.2 again); `SkillExplodeArrow` divides two table results by 12 (signed);
  `SkillRoot`, `SkillConvertEnemy` and `SkillConvertUndead` call `getEnemyWord__Fi` on the first value and pass its result.
- `getRankDescription__5SkillPUsi` and `getRankDescription__17SkillResistPoisonPUsi` are weak symbols in the image.

## Inferred

- The functions build the per-rank description line shown on the skill and spell menus: the key names a printf-style
  format and the floats are its numbers. The table entry at +88 answers per-rank values for a ramp index; the entry at +72
  answers a pair of ints for a ramp index (a damage pair). These role names are not asserted; neither the format text nor
  the ramp indices were read or named here.
- The object word at `+96` is a table pointer. An earlier note says a Skill class constructor stores its `_vt$` address at
  object + 72; that is a different offset (the Skill object seen here may be a subobject), and neither is exercised here.

## Unknown

- What any format text says or how many arguments it consumes; what each ramp index means; what `getEnemyWord__Fi`
  returns; why the `+96` word and not the `+72` word is the one read here; whether `Skill` has a second base.
- Compiled bodies are larger than the image functions (table below): the authored source loads 32-bit words and widens
  them so that one source also runs in a 64-bit host check. No byte match is claimed.

## Checks performed

- EE compile of every unit with the reviewed `authored_cpp` flags (the private harness `cc.sh`, identical flags).
- Host: each `host_check.cpp` builds natively (`c++ -std=c++17 -Wall -Wextra -Werror -DW13_RANK_HOST host_check.cpp`)
  and steps the stubbed table results over every combination of a seven-value set (including 0, 1, negatives, and values that
  make the divide by 12 and the round-to-nearest differ) for four ranks, comparing a call trace (every table call with its
  adjusted pointer, index and rank, the key, the eight float bit patterns) with a trace built from the stated rule using
  `lround`. Run counts: 4 to 67,228 per unit. All pass. A wrong adjusted pointer, scale or zero argument fails the run.
- Paired run (private R5900 interpreter, outside git): each authored function against the image function from identical
  states. A fake table at `object + 96` with distinct adjustments for the two entries, hooked entry functions returning
  seeded values from a set including 0, 1, negatives, 25, 100, 1000 and 99999, hooked `textGetString__FPc`,
  `usprintf__FPUsT0ffffffff` (logs all eight float bit patterns), `ustrcpy__FPUsT0` and `getEnemyWord__Fi`, all logging
  string contents and arguments. 12 seeds x 8 ranks (-1, 0, 1, 2, 3, 5, 9, 10) per function, zero mismatches, no HALT.
  Instruction coverage of the original: all words except one to five delay-slot nops or unreachable negative-side words per
  function (every branch side of every function was taken; the remaining words are `nop`s in delay slots).
- Mutation checks (each made the paired run report mismatches, then restored): swapped argument order (CriticalHit),
  wrong key (base Skill), rounding sign (BlindingLight), branch condition (GroundPound, DiseaseTrail), divisor 12 to 10
  (ExplodeArrow), wrong ramp index (ShockOfFire), wrong argument to `getEnemyWord__Fi` (Root), `rank * 5` to `rank * 4`
  (ResistPoison).

## Compiled sizes

| symbol | image address | image bytes | compiled bytes | unit |
|---|---|---|---|---|
| getRankDescription__16SkillGroundPoundPUsi | 0x0028f608 | 396 | 728 | w13-skill-rank-b-ground-pound |
| getRankDescription__16SkillDiseaseBoltPUsi | 0x00293318 | 436 | 592 | w13-skill-rank-b-disease-bolt |
| getRankDescription__17SkillDiseaseTrailPUsi | 0x00294fb0 | 436 | 560 | w13-skill-rank-b-disease-trail |
| getRankDescription__18SkillDiseaseShieldPUsi | 0x00296480 | 488 | 704 | w13-skill-rank-b-disease-shield |
| getRankDescription__17SkillConvertEnemyPUsi | 0x00298ba0 | 260 | 484 | w13-skill-rank-b-convert-enemy |
| getRankDescription__18SkillConvertUndeadPUsi | 0x002995f8 | 260 | 484 | w13-skill-rank-b-convert-undead |
| getRankDescription__16SkillPoisonArrowPUsi | 0x0029fe20 | 356 | 576 | w13-skill-rank-b-poison-arrow |
| getRankDescription__17SkillExplodeArrowPUsi | 0x002a27e0 | 280 | 648 | w13-skill-rank-b-explode-arrow |
| getRankDescription__9SkillBashPUsi | 0x002a5138 | 392 | 728 | w13-skill-rank-b-bash |
| getRankDescription__17SkillMinorHealingPUsi | 0x002a73c0 | 136 | 204 | w13-skill-rank-b-minor-healing |
| getRankDescription__18SkillAncestralCallPUsi | 0x002a7488 | 404 | 700 | w13-skill-rank-b-ancestral-call |
| getRankDescription__19SkillSummonSkeletonPUsi | 0x002a90a8 | 196 | 336 | w13-skill-rank-b-summon-skeleton |
| getRankDescription__21SkillLightningWeaponsPUsi | 0x002a9ad0 | 592 | 888 | w13-skill-rank-b-lightning-weapons |
| getRankDescription__16SkillFireWeaponsPUsi | 0x002aaf68 | 592 | 888 | w13-skill-rank-b-fire-weapons |
| getRankDescription__9SkillRootPUsi | 0x002acf10 | 260 | 484 | w13-skill-rank-b-root |
| getRankDescription__16SkillCriticalHitPUsi | 0x002adc80 | 244 | 468 | w13-skill-rank-b-critical-hit |
| getRankDescription__16SkillConeOfFrostPUsi | 0x002afdc8 | 252 | 484 | w13-skill-rank-b-cone-of-frost |
| getRankDescription__17SkillUndeadShieldPUsi | 0x002b12d8 | 244 | 468 | w13-skill-rank-b-undead-shield |
| getRankDescription__16SkillShockOfFirePUsi | 0x002b3010 | 352 | 592 | w13-skill-rank-b-shock-of-fire |
| getRankDescription__18SkillHammerOfWrathPUsi | 0x002b5350 | 384 | 664 | w13-skill-rank-b-hammer-of-wrath |
| getRankDescription__17SkillRegenerationPUsi | 0x002b6bd0 | 136 | 204 | w13-skill-rank-b-regeneration |
| getRankDescription__18SkillBlindingLightPUsi | 0x002b9d70 | 364 | 664 | w13-skill-rank-b-blinding-light |
| getRankDescription__18SkillRepulseUndeadPUsi | 0x002ba668 | 196 | 336 | w13-skill-rank-b-repulse-undead |
| getRankDescription__18SkillDismissUndeadPUsi | 0x002bb888 | 384 | 664 | w13-skill-rank-b-dismiss-undead |
| getRankDescription__17SkillShockOfFrostPUsi | 0x002bda50 | 540 | 832 | w13-skill-rank-b-shock-of-frost |
| getRankDescription__22SkillRangerCriticalHitPUsi | 0x002c70d8 | 348 | 564 | w13-skill-rank-b-ranger-critical-hit |
| getRankDescription__5SkillPUsi | 0x00313210 | 56 | 52 | w13-skill-rank-b-skill-base |
| getRankDescription__17SkillResistPoisonPUsi | 0x00313ca0 | 112 | 104 | w13-skill-rank-b-resist-poison |

## Imports closed

All 28 requested names: getRankDescription__16SkillConeOfFrostPUsi, getRankDescription__16SkillCriticalHitPUsi,
getRankDescription__16SkillDiseaseBoltPUsi, getRankDescription__16SkillFireWeaponsPUsi,
getRankDescription__16SkillGroundPoundPUsi, getRankDescription__16SkillPoisonArrowPUsi,
getRankDescription__16SkillShockOfFirePUsi, getRankDescription__17SkillConvertEnemyPUsi,
getRankDescription__17SkillDiseaseTrailPUsi, getRankDescription__17SkillExplodeArrowPUsi,
getRankDescription__17SkillMinorHealingPUsi, getRankDescription__17SkillRegenerationPUsi,
getRankDescription__17SkillResistPoisonPUsi, getRankDescription__17SkillShockOfFrostPUsi,
getRankDescription__17SkillUndeadShieldPUsi, getRankDescription__18SkillAncestralCallPUsi,
getRankDescription__18SkillBlindingLightPUsi, getRankDescription__18SkillConvertUndeadPUsi,
getRankDescription__18SkillDiseaseShieldPUsi, getRankDescription__18SkillDismissUndeadPUsi,
getRankDescription__18SkillHammerOfWrathPUsi, getRankDescription__18SkillRepulseUndeadPUsi,
getRankDescription__19SkillSummonSkeletonPUsi, getRankDescription__21SkillLightningWeaponsPUsi,
getRankDescription__22SkillRangerCriticalHitPUsi, getRankDescription__5SkillPUsi,
getRankDescription__9SkillBashPUsi, getRankDescription__9SkillRootPUsi.

## Imports opened

- `getEnemyWord__Fi` (called by Root, ConvertEnemy, ConvertUndead). It is not in the build plan's emitted functions at
  this revision. Everything else the units call (`textGetString__FPc`, `usprintf__FPUsT0ffffffff`, `ustrcpy__FPUsT0`) is
  already defined by selected units.
- `hostWordAddress` is referenced only under `W13_RANK_HOST` (host checks), never in a plan build.

## Build-plan entries needed

28 entries, one per unit, kind `independently_authored_source`, profile `authored_cpp`, empty `header_closure` (no unit
includes any header). Print them with the exact hashes and compiled sizes:

    python3 docs/agents/w13-skill-rank-b-plan-entries.py --toolchain /path/to/ee [--private-root DIR]

`--private-root DIR` writes a private snapshot/plan copy plus a source copy outside the repository, for a build with
`tools/build_partial.py`. Nothing shared was edited.

## Authored but never executed

All of it, on the EE. The compiled functions were run only in the private interpreter against hooked callees (a fake table
with fixed adjustments), and the host checks use stubs; no real Skill object, no real `evaluateRampAtRank` or
`getRampDamage` implementation, no real `textGetString__FPc` or `usprintf` ran through them. Stack frames and instruction
scheduling differ from the image.

## What remains unknown

See "Unknown". Nothing here names a skill ramp or confirms the roles of the table entries beyond their argument shapes.

## Private build result

A plan copy outside the repository (`--private-root`) holding the shared selection plus these 28 units, built with
`tools/build_partial.py` and the reviewed toolchain: 5,556 selected inputs, 5,556 compilations, one link, 4,268 function
bodies, 1,141 unresolved imports, no COMMON symbols. None of the 28 requested `getRankDescription__*` names is unresolved
in that output; the 28 other `getRankDescription__*Skill*` names that remain belong to the sibling package
(`w13-skill-rank-a`). `getEnemyWord__Fi` is newly unresolved, as listed under "Imports opened". The totals are not comparable
with the shared branch, since that copy also carries other packages' entries.
