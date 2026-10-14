# Skill cast and canCast methods (part 2 of 2): w13-skill-cast-b

Branch `agents/w13-skill-cast-b`. Thirty-seven functions, one per commit (plus two small follow-up commits, listed below),
closing the `canCast__*` and `cast__*` methods of the skill classes assigned to this part. Addresses and sizes are from the
private image whose hash is the snapshot `target_sha256` (`SLUS_205.65`); nothing from the image is committed beyond symbol
names, addresses, sizes, immediate constants and the resource/key strings the functions pass.

## Observed

- Every `canCast` here takes `(skill, creature)` and returns an int. Three shapes occur. (a) Ten of them are a leaf that either
  returns one (`SkillShockOfFrost`, `SkillBlindingLight`: 8 bytes, arguments not read) or returns one exactly when
  `checkIfSpellIsRunning__8CreatureRC9type_info(creature, __tf<SpellClass>())` is null (eight classes: FireWeapons, DiseaseTrail,
  UndeadShield, AncestralCall, DiseaseShield, HammerOfWrath, LightningWeapons, RangerCriticalHit). (b) `GroundPound` and `Bash` read the
  player record (`player` + creature word at +960 times 23984, the stride other authored code uses) and show a message
  (`textGetString__FPc` + `hudSetInfoMessage__FPUsiic(text, 180, index, 0)`) when their condition fails. (c) `SummonSkeleton` and
  `ConvertUndead` consult the world (`objectFindObject__FPc` on the per-player `skelightName` entry; `objectFindInBox` around the creature).
- The simple `cast` methods allocate a spell block with `blockAlloc__Fi` and construct the matching spell class in place, returning the
  block (null when the allocation fails): FireWeapons 96 bytes, UndeadShield 100, LightningWeapons 36, RangerCriticalHit 92, and, after
  a call through the skill's own table entry at byte 120, MinorHealing 36 and AncestralCall 132 (that entry is `tweekRank__5Skillii`
  in both class tables, read from the image). DiseaseShield (432 bytes) and DiseaseTrail (48) first write a four-halfword colour group
  into the creature at +148; ConvertEnemy and ConvertUndead (120 bytes each) create and destroy an empty local list.
- The projectile/effect casts return zero and ignore the block pointer: ShockOfFrost (208), DismissUndead (HolyBolt, 192), PoisonArrow
  (176, rank through the table first), ExplodeArrow (116, rank through the table first). `Regeneration` does nothing and returns null.
- The large casts combine these steps: `GroundPound` (animation header edit plus sound), `Bash` (sound, then spell block), `ShockOfFire`
  (three ramp lookups, a Sparks effect), `BlindingLight` (screen colour, sorted creature list, effect), `Root` (sorted creature list,
  filters, per-target root spell), `HammerOfWrath` and `RepulseUndead` (several effects and an inlined spell object), `SummonSkeleton`
  (232 particles around the model's shadow points, a Skelight on the ground and an inlined spell object). The inlined 28-byte spell
  object in HammerOfWrath and SummonSkeleton has the same layout in both users (see the source comments).
- Four constants read from the small-data pool are not named by symbols: 1.4779719 and 0.1 (truncated) in GroundPound's cast,
  0.2 (truncated) in ShockOfFire's, and four floats in SummonSkeleton's. They are reproduced as exact hexadecimal float literals.

## Inferred

- The `Point3`-and-`short` signature (`G6Point3s`) means every cast has five parameters: skill, rank, creature, point, trailing short. Where
  the short is used it is passed to the effect constructor as an angle-like value; the name "angle" is a working label only.
- The creature bit 0x40 at +12 marks a player-controlled creature in the code that reads the player record; the per-player halfword group
  at +140/+148 is a colour. Neither is confirmed by this package.
- For `canCast` of the eight "no spell running" classes, a skill is blocked while the matching spell is active.

## Unknown

- Meanings of most fields (+12 bit 0x40, +72 table, +96 skill table, +140..+154 halfwords, +794 halfword, +960 player index), of the
  ramp-slot arguments, of the many float/int constants passed to the effect constructors, and of the node layout of the sorted creature
  lists (only the words at +0 and +12 are read).
- What the first `Sparks` / `LostSoul` / `Skelight` constructor arguments mean, and why `ConvertEnemy`, `ConvertUndead` and `BlindingLight`
  create a list head that nothing fills before destroying it (BlindingLight does fill its list).
- Whether rank `-4` (RepulseUndead divisor) or very large ranks can occur.

## Units

| Symbol | Address | Bytes | Source | Words covered of the original, paired scenarios |
|---|---|---|---|---|
| `canCast__18SkillAncestralCallP8Creature` | 0x002a83b8 | 52 | `w13-skill-cast-b-can-cast-ancestral-call/can_cast.cpp` | 13/13, 8 |
| `canCast__9SkillBashP8Creature` | 0x002a5dc8 | 120 | `w13-skill-cast-b-can-cast-bash/can_cast.cpp` | 30/30, 60 |
| `canCast__18SkillBlindingLightP8Creature` | 0x002ba1b0 | 8 | `w13-skill-cast-b-can-cast-blinding-light/can_cast.cpp` | 2/2, 3 |
| `canCast__18SkillConvertUndeadP8Creature` | 0x00298d00 | 268 | `w13-skill-cast-b-can-cast-convert-undead/can_cast.cpp` | 67/67, 40 |
| `canCast__18SkillDiseaseShieldP8Creature` | 0x002968d8 | 52 | `w13-skill-cast-b-can-cast-disease-shield/can_cast.cpp` | 13/13, 8 |
| `canCast__17SkillDiseaseTrailP8Creature` | 0x00295568 | 52 | `w13-skill-cast-b-can-cast-disease-trail/can_cast.cpp` | 13/13, 8 |
| `canCast__16SkillFireWeaponsP8Creature` | 0x002ab4f0 | 52 | `w13-skill-cast-b-can-cast-fire-weapons/can_cast.cpp` | 13/13, 8 scenarios |
| `canCast__16SkillGroundPoundP8Creature` | 0x00290928 | 172 | `w13-skill-cast-b-can-cast-ground-pound/can_cast.cpp` | 43/43, 60 |
| `canCast__18SkillHammerOfWrathP8Creature` | 0x002b5840 | 52 | `w13-skill-cast-b-can-cast-hammer-of-wrath/can_cast.cpp` | 13/13, 8 |
| `canCast__21SkillLightningWeaponsP8Creature` | 0x002a9e80 | 52 | `w13-skill-cast-b-can-cast-lightning-weapons/can_cast.cpp` | 13/13, 8 |
| `canCast__22SkillRangerCriticalHitP8Creature` | 0x002c7ce8 | 52 | `w13-skill-cast-b-can-cast-ranger-critical-hit/can_cast.cpp` | 13/13, 8 |
| `canCast__17SkillShockOfFrostP8Creature` | 0x002be190 | 8 | `w13-skill-cast-b-can-cast-shock-of-frost/can_cast.cpp` | 2/2, 3 |
| `canCast__19SkillSummonSkeletonP8Creature` | 0x002a91a8 | 84 | `w13-skill-cast-b-can-cast-summon-skeleton/can_cast.cpp` | 21/21, 32 |
| `canCast__17SkillUndeadShieldP8Creature` | 0x002b1408 | 52 | `w13-skill-cast-b-can-cast-undead-shield/can_cast.cpp` | 13/13, 8 |
| `cast__18SkillAncestralCalliP8CreatureG6Point3s` | 0x002a8490 | 160 | `w13-skill-cast-b-cast-ancestral-call/cast.cpp` | 40/40, 5 |
| `cast__9SkillBashiP8CreatureG6Point3s` | 0x002a5e40 | 200 | `w13-skill-cast-b-cast-bash/cast.cpp` | 50/50, 5 |
| `cast__18SkillBlindingLightiP8CreatureG6Point3s` | 0x002b9be0 | 396 | `w13-skill-cast-b-cast-blinding-light/cast.cpp` | 99/99, 24 |
| `cast__17SkillConvertEnemyiP8CreatureG6Point3s` | 0x00298b00 | 156 | `w13-skill-cast-b-cast-convert-enemy/cast.cpp` | 39/39, 6 |
| `cast__18SkillConvertUndeadiP8CreatureG6Point3s` | 0x00299558 | 156 | `w13-skill-cast-b-cast-convert-undead/cast.cpp` | 39/39, 6 |
| `cast__18SkillDiseaseShieldiP8CreatureG6Point3s` | 0x00296808 | 148 | `w13-skill-cast-b-cast-disease-shield/cast.cpp` | 38/38, 6 |
| `cast__17SkillDiseaseTrailiP8CreatureG6Point3s` | 0x00295490 | 156 | `w13-skill-cast-b-cast-disease-trail/cast.cpp` | 39/39, 6 |
| `cast__18SkillDismissUndeadiP8CreatureG6Point3s` | 0x002bbc90 | 144 | `w13-skill-cast-b-cast-dismiss-undead/cast.cpp` | 36/36, 6 |
| `cast__17SkillExplodeArrowiP8CreatureG6Point3s` | 0x002a2720 | 192 | `w13-skill-cast-b-cast-explode-arrow/cast.cpp` | 48/48, 6 |
| `cast__16SkillFireWeaponsiP8CreatureG6Point3s` | 0x002ab430 | 132 | `w13-skill-cast-b-cast-fire-weapons/cast.cpp` | 33/33, 5 |
| `cast__16SkillGroundPoundiP8CreatureG6Point3s` | 0x002909d8 | 240 | `w13-skill-cast-b-cast-ground-pound/cast.cpp` | 60/60, 5 |
| `cast__18SkillHammerOfWrathiP8CreatureG6Point3s` | 0x002b50c8 | 644 | `w13-skill-cast-b-cast-hammer-of-wrath/cast.cpp` | 159/161 (two padding nops), 80 |
| `cast__21SkillLightningWeaponsiP8CreatureG6Point3s` | 0x002a9dc0 | 132 | `w13-skill-cast-b-cast-lightning-weapons/cast.cpp` | 33/33, 5 |
| `cast__17SkillMinorHealingiP8CreatureG6Point3s` | 0x002a7320 | 160 | `w13-skill-cast-b-cast-minor-healing/cast.cpp` | 40/40, 5 |
| `cast__16SkillPoisonArrowiP8CreatureG6Point3s` | 0x002a0c30 | 204 | `w13-skill-cast-b-cast-poison-arrow/cast.cpp` | 51/51, 6 |
| `cast__22SkillRangerCriticalHitiP8CreatureG6Point3s` | 0x002c7c60 | 132 | `w13-skill-cast-b-cast-ranger-critical-hit/cast.cpp` | 33/33, 5 |
| `cast__17SkillRegenerationiP8CreatureG6Point3s` | 0x002b6ba8 | 40 | `w13-skill-cast-b-cast-regeneration/cast.cpp` | 10/10, 5 |
| `cast__18SkillRepulseUndeadiP8CreatureG6Point3s` | 0x002ba1d8 | 732 | `w13-skill-cast-b-cast-repulse-undead/cast.cpp` | 183/183, 24 |
| `cast__9SkillRootiP8CreatureG6Point3s` | 0x002ac4f0 | 500 | `w13-skill-cast-b-cast-root/cast.cpp` | 125/125, 150 |
| `cast__16SkillShockOfFireiP8CreatureG6Point3s` | 0x002b2e70 | 416 | `w13-skill-cast-b-cast-shock-of-fire/cast.cpp` | 104/104, 12 |
| `cast__17SkillShockOfFrostiP8CreatureG6Point3s` | 0x002be0c8 | 144 | `w13-skill-cast-b-cast-shock-of-frost/cast.cpp` | 36/36, 6 |
| `cast__19SkillSummonSkeletoniP8CreatureG6Point3s` | 0x002a86b0 | 1972 | `w13-skill-cast-b-cast-summon-skeleton/cast.cpp` | 493/493, 131 |
| `cast__17SkillUndeadShieldiP8CreatureG6Point3s` | 0x002b1250 | 132 | `w13-skill-cast-b-cast-undead-shield/cast.cpp` | 33/33, 5 |

Shared header: `experimental/sources/w13-skill-cast-b-common/skill_access.hpp` (`Point3`, a word-to-pointer accessor that maps
32-bit words to host pointers only under the host-check macro, and the skill/creature table-entry call helpers). Fourteen of the
units include it. Each unit is its own commit; two further commits are corrections to earlier units
(`Correct two byte sizes in cast comments`: DiseaseTrail is 156 bytes and ShockOfFrost 144; and
`Keep the word_of helper in the hammer-of-wrath cast always inlined`).

## Checks performed

- Every unit compiles with the reviewed `authored_cpp` flags (`cc.sh`) and leaves no local helper symbol (checked with `nm`).
- Every unit has a host contract (`host_check.cpp`, native compiler, stubs that log calls) that fails on a wrong result. The large
  ones (`Root`, `SummonSkeleton`, `ConvertUndead` canCast, `HammerOfWrath`) are exhaustive over their branches or compare against an
  independently written model (4,000 randomised trials for Root; 1,536 for SummonSkeleton, built with `-ffp-contract=off`).
- Every unit was run in the private R5900 interpreter next to the original image function with identical stubbed callees, from
  identical memory snapshots: call logs (including float arguments, stack-passed arguments and the contents behind by-reference
  arguments), return values and every changed byte outside the stack were compared. The table above gives the instruction-word
  coverage of the original and the scenario count. All runs reported zero mismatches and no `HALT`.
- Mutation checks (inverted test, wrong constant, swapped argument, changed size, dropped call, one-ulp float constants, boundary
  values): every hand-written unit and one representative of each generated family had at least one mutant rejected by the paired run;
  the larger units (BlindingLight, ShockOfFire, Root, HammerOfWrath, RepulseUndead, SummonSkeleton, ConvertUndead canCast) had 7 to
  more than 25. Not mutated individually, because they come from the same generator and text as a mutated sibling: the canCast of
  DiseaseTrail, UndeadShield, AncestralCall, DiseaseShield, HammerOfWrath, LightningWeapons and RangerCriticalHit (sibling: FireWeapons),
  the casts of UndeadShield, LightningWeapons and RangerCriticalHit (sibling: FireWeapons), the cast of AncestralCall (sibling:
  MinorHealing) and the canCast of BlindingLight (sibling: ShockOfFrost). Survivors, all understood: `1.0 <= squared length` in
  SummonSkeleton's unit-ball test (equality cannot occur for the draws), and, for BlindingLight, the null-creature guard (the
  interpreter halts in both runs for a null creature, so the guard is covered only as an untaken branch). Some mutants first appeared to
  survive because half-ulp float edits compile to the same constant; they were redone with true one-ulp edits and rejected. The SummonSkeleton commit message says thirty mutants; "more than 25" above is the accurate count.
- A private partial build with all 37 units added to the shared plan (outside git): 5,565 compiler inputs, one relocatable link, 4,277
  function bodies, 1,162 unresolved imports, zero COMMON symbols. None of the 37 requested names is unresolved.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.

## Imports closed

`canCast__16SkillFireWeaponsP8Creature`, `cast__16SkillFireWeaponsiP8CreatureG6Point3s`, `canCast__16SkillGroundPoundP8Creature`, `cast__16SkillGroundPoundiP8CreatureG6Point3s`, `cast__16SkillPoisonArrowiP8CreatureG6Point3s`, `cast__16SkillShockOfFireiP8CreatureG6Point3s`, `cast__17SkillConvertEnemyiP8CreatureG6Point3s`, `canCast__17SkillDiseaseTrailP8Creature`, `cast__17SkillDiseaseTrailiP8CreatureG6Point3s`, `cast__17SkillExplodeArrowiP8CreatureG6Point3s`, `cast__17SkillMinorHealingiP8CreatureG6Point3s`, `cast__17SkillRegenerationiP8CreatureG6Point3s`, `canCast__17SkillShockOfFrostP8Creature`, `cast__17SkillShockOfFrostiP8CreatureG6Point3s`, `canCast__17SkillUndeadShieldP8Creature`, `cast__17SkillUndeadShieldiP8CreatureG6Point3s`, `canCast__18SkillAncestralCallP8Creature`, `cast__18SkillAncestralCalliP8CreatureG6Point3s`, `canCast__18SkillBlindingLightP8Creature`, `cast__18SkillBlindingLightiP8CreatureG6Point3s`, `canCast__18SkillConvertUndeadP8Creature`, `cast__18SkillConvertUndeadiP8CreatureG6Point3s`, `canCast__18SkillDiseaseShieldP8Creature`, `cast__18SkillDiseaseShieldiP8CreatureG6Point3s`, `cast__18SkillDismissUndeadiP8CreatureG6Point3s`, `canCast__18SkillHammerOfWrathP8Creature`, `cast__18SkillHammerOfWrathiP8CreatureG6Point3s`, `cast__18SkillRepulseUndeadiP8CreatureG6Point3s`, `canCast__19SkillSummonSkeletonP8Creature`, `cast__19SkillSummonSkeletoniP8CreatureG6Point3s`, `canCast__21SkillLightningWeaponsP8Creature`, `cast__21SkillLightningWeaponsiP8CreatureG6Point3s`, `canCast__22SkillRangerCriticalHitP8Creature`, `cast__22SkillRangerCriticalHitiP8CreatureG6Point3s`, `canCast__9SkillBashP8Creature`, `cast__9SkillBashiP8CreatureG6Point3s`, `cast__9SkillRootiP8CreatureG6Point3s`.

## Imports opened

The units reference 33 symbols that are still unresolved in the private build. 31 of them are referenced by no other input of that build,
so these units open them; two were already imported by other inputs:

`__12GlowingShaftG6Point3iiii`, `__13HammerOfWrathG6Point3sP10GameObjectiP5Skill`, `__16SpellFireWeaponsiP8CreatureP5Skill`, `__17SpellConvertEnemyiP8CreatureP5Skill`, `__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill`, `__17SpellMinorHealingiP8CreatureP5Skill`, `__17SpellUndeadShieldiP8CreatureP5Skill`, `__18SpellAncestralCalliP8CreatureP5Skill`, `__18SpellConvertUndeadiP8CreatureP5Skill`, `__19BlindingLightEffectG6Point3iP8CreatureP5Skill`, `__21SpellLightningWeaponsiP8CreatureP5Skill`, `__22SpellRangerCriticalHitiP8CreatureP5Skill`, `__8HolyBoltG6Point3iiP8CreatureP5Skill`, `__8LostSoulG6Point3T1iP10_vagHeaderii`, `__8SkelightG6Point3sPPcci`, `__9SpellBashiP8CreatureP5Skill`, `__9SpellRootiP8CreatureP5Skill`, `__tf16SpellFireWeapons`, `__tf17SpellDiseaseTrail`, `__tf17SpellUndeadShield`, `__tf18SpellAncestralCall`, `__tf18SpellHammerOfWrath`, `__tf21SpellLightningWeapons`, `__tf22SpellRangerCriticalHit`, `_vt$18SpellHammerOfWrath`, `_vt$19SpellSummonSkeleton`, `gameRadialDamage__FP10GameObjectG6Point3iffiii`, `skelightName`, `skelightParams`, `summonskeletonParticle1`, `summonskeletonParticle2`.

Already imported elsewhere: `__tf18SpellDiseaseShield`, `checkIfSpellIsRunning__8CreatureRC9type_info`.

Data symbols among them are declared in the sources as `extern` with no size: `skelightName` (a four-entry table of C-string
pointers), `skelightParams` (four 8-byte records), `summonskeletonParticle1`, `summonskeletonParticle2` and the vtables. `eRandState`,
`syncRandState`, `world` and `player` are also referenced as externs; they are defined by other inputs of the build. The base-class
vtable `_vt$5Spell` is stored by the original inside the inlined spell objects but the store is overwritten before it can be observed, so
the optimizer drops it and no reference remains. Several constructors (`Spell*`, `Sparks`, `Skelight`, ...) are other packages' symbols.

## Build-plan entries needed

The shared plan and snapshot were not edited. The 37 inputs below go at the end of the plan, in this order (the integrator assigns
`input_order`); also add these hashes to the `files` map of both plan and snapshot:

- `sources/w13-skill-cast-b-common/skill_access.hpp`: `07c373a0c057da5b07b65a9c1604bd43f063554434e696f35455a1ed228daf5c`
- each `source` below with its own `sha256`.

`docs/agents/w13-skill-cast-b-plan-entries.py --toolchain <ee> [--private-root DIR]` reprints these entries from the checked-out sources
(it compiles each unit with the plan's `authored_cpp` flags to get `compiled_body_bytes`) and can write a private plan and source copy
outside the repository. Entries (`header_closure` is the shared header for the fourteen units that include it, otherwise empty):

```json
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__18SkillAncestralCallP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_ancestral_call.o", "profile": "authored_cpp", "sha256": "7fef86fc3d7f8803b7e2e7e649e6130206dd00badb3203f1a7fdc8eb179bdfe0", "source": "sources/w13-skill-cast-b-can-cast-ancestral-call/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "canCast__9SkillBashP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_bash.o", "profile": "authored_cpp", "sha256": "ccf8a08ab117c9318816102c4bb3ca586ecf013b6da9f25f04adf752c9cd48c1", "source": "sources/w13-skill-cast-b-can-cast-bash/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "canCast__18SkillBlindingLightP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_blinding_light.o", "profile": "authored_cpp", "sha256": "54053b1d31e31d3557f171daf757ceb78ac96dfaa9a78c75f0e61a32cbd640b9", "source": "sources/w13-skill-cast-b-can-cast-blinding-light/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 280, "name": "canCast__18SkillConvertUndeadP8Creature"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_convert_undead.o", "profile": "authored_cpp", "sha256": "4bbcb728d55c1e7243f3be49df7e0b111eedc795740c2de8f3a169a644a45db6", "source": "sources/w13-skill-cast-b-can-cast-convert-undead/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__18SkillDiseaseShieldP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_disease_shield.o", "profile": "authored_cpp", "sha256": "6c3f6892e949300bdb344ea75ea5c83e94d7670783f325b2b0ef90be6caa6239", "source": "sources/w13-skill-cast-b-can-cast-disease-shield/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__17SkillDiseaseTrailP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_disease_trail.o", "profile": "authored_cpp", "sha256": "5847cd8931f82a9a79dc40ee71787031368333e8b8a2490b7d98e044152ac7a4", "source": "sources/w13-skill-cast-b-can-cast-disease-trail/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__16SkillFireWeaponsP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_fire_weapons.o", "profile": "authored_cpp", "sha256": "3a45dc14d3584287402dd1e4168913a449fb2efff96629d885f691faf993f32b", "source": "sources/w13-skill-cast-b-can-cast-fire-weapons/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 180, "name": "canCast__16SkillGroundPoundP8Creature"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_ground_pound.o", "profile": "authored_cpp", "sha256": "3213138dc541ff93cdb35e96959c8d2a2d75ec346cbb4173ff176756284f20fb", "source": "sources/w13-skill-cast-b-can-cast-ground-pound/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__18SkillHammerOfWrathP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_hammer_of_wrath.o", "profile": "authored_cpp", "sha256": "842720baf4500ec12f29782225a5ae0cba64d9e1c78f518139e8e9c7f12c7af6", "source": "sources/w13-skill-cast-b-can-cast-hammer-of-wrath/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__21SkillLightningWeaponsP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_lightning_weapons.o", "profile": "authored_cpp", "sha256": "e2c54f87ec78f6f913b71597cd016d89543b74b6512df536088a641166dbf32f", "source": "sources/w13-skill-cast-b-can-cast-lightning-weapons/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__22SkillRangerCriticalHitP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_ranger_critical_hit.o", "profile": "authored_cpp", "sha256": "d11d766955f1ce8d793195f3c22662ca35a486c5e520f1cc49758fe6e8cea37a", "source": "sources/w13-skill-cast-b-can-cast-ranger-critical-hit/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "canCast__17SkillShockOfFrostP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_shock_of_frost.o", "profile": "authored_cpp", "sha256": "9bbcb5a0e2c42c3736fb521ab42a8478058ee724b16b6f0b1c19b5ccb4a06141", "source": "sources/w13-skill-cast-b-can-cast-shock-of-frost/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "canCast__19SkillSummonSkeletonP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_summon_skeleton.o", "profile": "authored_cpp", "sha256": "890aaccb5742e15e7322d70c3b24ca573a419e47e52988309c1dccf89c91f19f", "source": "sources/w13-skill-cast-b-can-cast-summon-skeleton/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "canCast__17SkillUndeadShieldP8Creature"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_can_cast_undead_shield.o", "profile": "authored_cpp", "sha256": "c2e9c343d79c0a7e3721476ccf9a65bfe71dde04dce0c9803a5e2ced50e91974", "source": "sources/w13-skill-cast-b-can-cast-undead-shield/can_cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "cast__18SkillAncestralCalliP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_ancestral_call.o", "profile": "authored_cpp", "sha256": "bb9c20fbe44c99be9da6cd8199cfcf71f74ce601579469fb79bfb30334930300", "source": "sources/w13-skill-cast-b-cast-ancestral-call/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "cast__9SkillBashiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_bash.o", "profile": "authored_cpp", "sha256": "7674116fa06e1cd090a74b5434e4c659fc56179614cb80e939dfd8c955181ac9", "source": "sources/w13-skill-cast-b-cast-bash/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 392, "name": "cast__18SkillBlindingLightiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_blinding_light.o", "profile": "authored_cpp", "sha256": "1625d44d9a0965da25386ef76f1a049626475d200196360360169afe88c7b5e6", "source": "sources/w13-skill-cast-b-cast-blinding-light/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "cast__17SkillConvertEnemyiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_convert_enemy.o", "profile": "authored_cpp", "sha256": "597ceb63db2b3c17d8d0d50f440a1abc4609de8874d62f205aaea3ad60c044e4", "source": "sources/w13-skill-cast-b-cast-convert-enemy/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "cast__18SkillConvertUndeadiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_convert_undead.o", "profile": "authored_cpp", "sha256": "f9b0d2b6aca26fd038acee5e7f4c6bc344a36bacae0f02198abde31f66fb2192", "source": "sources/w13-skill-cast-b-cast-convert-undead/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "cast__18SkillDiseaseShieldiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_disease_shield.o", "profile": "authored_cpp", "sha256": "bda4636f2159e2616db41c4659570c6a551919f3e4102f46150c42aa675c2157", "source": "sources/w13-skill-cast-b-cast-disease-shield/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "cast__17SkillDiseaseTrailiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_disease_trail.o", "profile": "authored_cpp", "sha256": "8d71dcd974349c92042f9ff898c97093fed4035d9ecf3ffde03390f2ffde2a91", "source": "sources/w13-skill-cast-b-cast-disease-trail/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "cast__18SkillDismissUndeadiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_dismiss_undead.o", "profile": "authored_cpp", "sha256": "3cfa29609fa1bb98c08fc318d5ac66a8042de8082d0945897f262e951335f73d", "source": "sources/w13-skill-cast-b-cast-dismiss-undead/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "cast__17SkillExplodeArrowiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_explode_arrow.o", "profile": "authored_cpp", "sha256": "937597d624530fb6b78a67a5c3a7f1f3f089a5bf6814306b90de9284e1a5d59b", "source": "sources/w13-skill-cast-b-cast-explode-arrow/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "cast__16SkillFireWeaponsiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_fire_weapons.o", "profile": "authored_cpp", "sha256": "609cdc6ccca4fb2de14073e57ceccb4c526806cffdb1ff784a63b2f1c7ebf5b0", "source": "sources/w13-skill-cast-b-cast-fire-weapons/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "cast__16SkillGroundPoundiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_ground_pound.o", "profile": "authored_cpp", "sha256": "0f1dc1cbe7c369b34b7b390fadddfd8792fbc981f75d4da1b9b672d4cd461e28", "source": "sources/w13-skill-cast-b-cast-ground-pound/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 616, "name": "cast__18SkillHammerOfWrathiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_hammer_of_wrath.o", "profile": "authored_cpp", "sha256": "fe952d3de30989c533ccd8e6788a0b430382edae2cad7423615365411403c3f0", "source": "sources/w13-skill-cast-b-cast-hammer-of-wrath/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "cast__21SkillLightningWeaponsiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_lightning_weapons.o", "profile": "authored_cpp", "sha256": "d946b2b9fa1c5185d5a201d04e5b4becd7c94e6f414d99c12047420927a24a51", "source": "sources/w13-skill-cast-b-cast-lightning-weapons/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "cast__17SkillMinorHealingiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_minor_healing.o", "profile": "authored_cpp", "sha256": "6ad1d6f8396763a276a33bb69acbced7bbe3898de3188cd0c75fa5496ebccf92", "source": "sources/w13-skill-cast-b-cast-minor-healing/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "cast__16SkillPoisonArrowiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_poison_arrow.o", "profile": "authored_cpp", "sha256": "68f1457b75960e00e41b9f85d6098a660d87ced4a1ba4103a1cd2f9538521288", "source": "sources/w13-skill-cast-b-cast-poison-arrow/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "cast__22SkillRangerCriticalHitiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_ranger_critical_hit.o", "profile": "authored_cpp", "sha256": "620d4620bc931e82e31b180428adff9b5bc2734ee1e710c72c40569909129815", "source": "sources/w13-skill-cast-b-cast-ranger-critical-hit/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "cast__17SkillRegenerationiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_regeneration.o", "profile": "authored_cpp", "sha256": "f39e851f9555697393c2a5bb6e252e9c87074d1297ec270866f1a15ef85113e4", "source": "sources/w13-skill-cast-b-cast-regeneration/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 648, "name": "cast__18SkillRepulseUndeadiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_repulse_undead.o", "profile": "authored_cpp", "sha256": "453f6a4a84cc5dcdc6dca5d36ccf5f2077ef55b160e54d6a0c248bc8ecd0bfc8", "source": "sources/w13-skill-cast-b-cast-repulse-undead/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 540, "name": "cast__9SkillRootiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_root.o", "profile": "authored_cpp", "sha256": "3000bcf0a0e4c823859e57c1e8d4659106f0625f3b93d139a5fd1686428a289e", "source": "sources/w13-skill-cast-b-cast-root/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 396, "name": "cast__16SkillShockOfFireiP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_shock_of_fire.o", "profile": "authored_cpp", "sha256": "3dd3a30c3da0b516f9f1427bf2043b3a8c66ba42fd2c774929a499cc6c034cb4", "source": "sources/w13-skill-cast-b-cast-shock-of-fire/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "cast__17SkillShockOfFrostiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_shock_of_frost.o", "profile": "authored_cpp", "sha256": "dd052188693de457b19eef698683d037c0cd557dba61d4ac58b021ccadae8990", "source": "sources/w13-skill-cast-b-cast-shock-of-frost/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1780, "name": "cast__19SkillSummonSkeletoniP8CreatureG6Point3s"}], "header_closure": ["sources/w13-skill-cast-b-common/skill_access.hpp"], "kind": "independently_authored_source", "object_name": "w13scb_cast_summon_skeleton.o", "profile": "authored_cpp", "sha256": "3a00937118ce1de3ff394f18e39f94a6e9ea6292a7dd37176d34a422e7b92b54", "source": "sources/w13-skill-cast-b-cast-summon-skeleton/cast.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "cast__17SkillUndeadShieldiP8CreatureG6Point3s"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13scb_cast_undead_shield.o", "profile": "authored_cpp", "sha256": "0933a08b86b50656f34af83da17d790afeb003d9dc8c8e5bac9068b2672a624a", "source": "sources/w13-skill-cast-b-cast-undead-shield/cast.cpp"}
```

## Authored but never executed

Nothing here has run on an EE or as part of the game. Every function was run only (a) in the private R5900 interpreter beside the original
image function, with the called functions replaced by identical Python stubs that return the same values on both sides, and (b) in the
host contracts. Callee behaviour (the spell, effect, particle, sound, list, model and world calls) is not exercised. The interpreter
rounds float results to nearest while the EE truncates; the compared float computations here are single adds, subtracts and multiplies
executed by the same interpreter on both sides, so the comparison does not depend on that, but the claim about the EE's own results
is not established. The two ring/division paths (`RepulseUndead` with ranks -4 and above 65532) were not run. The creature-null guard
of `BlindingLight` was not run (see above).

## What is unknown

- The semantics of nearly every constant, field offset and table slot named in the source comments; names such as "angle", "colour
  group" and "player-controlled" are working labels.
- What `skelightName`/`skelightParams` entries hold, and the bound behind the `index < 4` guards.
- Whether the empty local lists in `ConvertEnemy`, `ConvertUndead` and the unused first list in `Root` are leftovers or are filled by
  an inlined helper that the optimizer removed.
- The layouts of Skill, Creature, the sorted-list nodes, the animation header edited in GroundPound's cast, the Skelight record and
  the spell objects.
- The return type of the casts that return zero (declared `int`/`void*` in the sources; only the register value is observed).
