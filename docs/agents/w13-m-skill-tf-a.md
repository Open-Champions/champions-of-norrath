# w13-m-skill-tf-a: Skill class type-info functions

Package: Skill class type-info nodes (`__tf<Class>` getters and their 12-byte `__ti<Class>` nodes).
Branch: `agents/w13-m-skill-tf-a`. Sources: `experimental/sources/w13-m-skill-tf-a-tf-*/`, one unit
per directory, each with `typeinfo.cpp` (the plan input) and `host_check.cpp` (host test only).

## Reference target

- Private reference: `work/private/SLUS_205.65` (local only, not committed). Symbol addresses were
  read from its symbol table with the reviewed EE `nm`. Each unit's comment records the observed
  address and size of its `__tf` function.
- Checked in this session: for all 30 units, the address and size in the comment match the symbol
  table entry (`__tf<Class>` weak text, 76 bytes; `__ti<Class>` 12-byte bss node). Zero mismatches.
  Nothing from the image was copied into a commit.

## What each unit contains

Each unit defines one getter, `extern "C" void* __tf<Class>()`, and its node `__ti<Class>[12]`.
The getter does the following.

- Observed in the image: the getter returns the node. If the node's first word is zero, it calls
  `__tf5Skill()`, then `__rtti_si(node, "<len><Class>", __ti5Skill)`. A nonzero first word skips both.
- Inferred: the node is the class's runtime type descriptor, and `Skill` is its single base.
  The class-name string is the Itanium-style length-prefixed name.
- Unknown: whether the first word is written by `__rtti_si` (the host stub sets it to 1, which is
  the test's assumption, not evidence of the real runtime behaviour).

## Verification performed

1. `python3 docs/agents/w13-m-skill-tf-a-object-check.py <ee>/bin/mips64r5900el-ps2-elf experimental/sources /tmp/<dir>`
   - Compiles each unit with the `authored_cpp` flags, checks that the object defines one global
     `__tf<Class>` and a 12-byte bss `__ti<Class>`, and that its only undefined symbols are
     `__tf5Skill`, `__rtti_si` and `__ti5Skill`.
   - Compiles and runs each `host_check.cpp` with `-Wall -Wextra -Werror`.
   - Result: 30/30 PASS, exit 0. Toolchain: the reviewed GCC 15.2.0 EE build under
     `work/toolchains/ee/bin` (prefix `mips64r5900el-ps2-elf`).
2. Address/size comparison against the private symbol table (described above): 30/30 match.
3. `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
4. Duplicate check: no other authored unit defines any of the 30 `__ti<Class>` nodes. The only
   other users of `__tf5Skill` and `__ti5Skill` are `w12-skill-a-*` units, which declare them
   `extern` and do not define them.

Not run in this session: the full `tools/build_partial.py` build (277+ compiles, one link). The
per-unit compile and symbol shape were checked instead, as requested. The link-level closure of
these 30 names has not been re-confirmed against a fresh `unresolved-imports.json`.

## Imports closed (30)

`__tf9SkillBash`, `__tf9SkillRoot`, `__tf15SkillPoisonRain`, `__tf15SkillShieldBash`,
`__tf15SkillWizardBeam`, `__tf16SkillColdWeapons`, `__tf16SkillConeOfFrost`, `__tf16SkillCriticalHit`,
`__tf16SkillDiseaseBolt`, `__tf16SkillFireWeapons`, `__tf16SkillGroundPound`, `__tf16SkillPoisonArrow`,
`__tf16SkillShockOfFire`, `__tf17SkillConvertEnemy`, `__tf17SkillDiseaseTrail`, `__tf17SkillExplodeArrow`,
`__tf17SkillMinorHealing`, `__tf17SkillRegeneration`, `__tf17SkillShockOfFrost`, `__tf17SkillUndeadShield`,
`__tf18SkillAncestralCall`, `__tf18SkillBlindingLight`, `__tf18SkillConvertUndead`, `__tf18SkillDiseaseShield`,
`__tf18SkillDismissUndead`, `__tf18SkillHammerOfWrath`, `__tf18SkillRepulseUndead`, `__tf19SkillSummonSkeleton`,
`__tf21SkillLightningWeapons`, `__tf22SkillRangerCriticalHit`.

Each name is closed at the unit level by its own object (global `T` symbol). The `__ti<Class>`
nodes are defined in the same units as `B` bss symbols, so they are also not imports.

## Imports opened (not defined by this package)

- `__tf5Skill` (the `Skill` base type-info getter). Already referenced by `w12-skill-a-*` units.
  No authored unit defines it. Owner unknown to this package; it is not in my scope.
- `__ti5Skill` (the 8-byte `Skill` base node). Already referenced by `w12-skill-a-*` units.
  No authored unit defines it. Owner unknown to this package.
- `__rtti_si` (the single-inheritance type-info initializer). Defined by two units,
  `w8-runtime-libc-a-rtti-si` and `w8-runtime-libc-b-rtti-si`, which the integrator must reconcile.
  Not defined by this package.

## Build-plan entries needed

The shared `experimental/build-plan.json` and `snapshot.json` are not edited here. To get the 30
entries, run

    python3 docs/agents/w13-m-skill-tf-a-plan-entries.py --toolchain <ee> [--private-root DIR]

This prints one JSON entry per unit (`header_closure` is empty for all 30; profile `authored_cpp`).
With `--private-root DIR` it writes a private `snapshot.json`, `build-plan.json` and a copy of the
sources under `DIR`, which can be built with `tools/build_partial.py --source-root DIR --plan DIR/build-plan.json`.
Nothing is written inside the repository. The integrator should merge these entries into the shared plan.

## Authored but never executed

- All 30 getters and their nodes were compiled and symbol-checked, and their host stubs were run.
  None of the functions has run on the EE, in the game, or under the paired-run harness.
- The host tests check only the control flow against stubs of `__tf5Skill` and `__rtti_si`.
  They do not check what `__rtti_si` writes into the node.
- Calling order at runtime (when each getter is first reached) is unknown.

## Unknown

- What `__rtti_si` actually stores in the node. The host stub's assumption (the first word becomes
  1) is not evidence. The `unset` test depends on it.
- Whether the 12-byte node layout matches the real layout beyond the first word.
- Whether `Skill` is the only base of every class in this package. The units assume so, from the
  single base call in each getter. This is inferred from the call shape only.
- The owners and sources of `__tf5Skill`, `__ti5Skill`, and which `__rtti_si` implementation is correct.
- Whether any of the 30 class names are missing from the game's class list (no inventory was taken
  in this session beyond the names in scope).

## Commits

One commit per function (30 commits, `Author the Skill type-info function for <Class>`), already on
the branch from an earlier run and reviewed here. This session added only this notes file.
