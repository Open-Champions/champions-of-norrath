# Skill class family (B): skill imports

Branch: `agents/w12-skill-b`. Address evidence came from the hash-pinned private `SLUS_205.65`; no game data or raw disassembly is included.

## Observed

- `canCast__5SkillP8Creature` is an 8-byte function at `0x003131b8`: `jr ra` with a delay-slot load of integer 1 into `v0`. It reads neither argument. A fresh EE compile matched the pinned body byte for byte.
- All 30 requested `_vt$...` symbols are 144-byte `.data` objects with 18 8-byte slots. Every adjustment word is zero, the final function slot is null, and every other pointer resolves to a sized function symbol.
- The host check invokes all 510 non-null entries and checks the expected name, each zero adjustment, and the null final slot. EE object validation checks all 30 symbol sizes and all 510 relocation names at their table offsets.

## Inferred

- The tables encode dispatch entries represented by their observed function symbols. No further runtime meaning is inferred.

## Unknown

- Skill behavior beyond the verified `canCast` leaf, type-info initialization, ramp state, runtime dispatch, and gameplay remain unknown.

## Checks performed

- Host `canCast`: nine pointer pairs returned 1. EE body comparison matched all 8 bytes.
- Host table check passed for all 30 tables and 510 non-null slots. EE object check passed all sizes and slot relocations.
- The branch-tip build baseline completed with 5113 compiler inputs and 827 unresolved imports. A separate private-plan build added both units and completed with 5115 compilations, one relocatable link, 3910 function bodies, 1031 unresolved imports, and zero COMMON symbols. No requested import remains unresolved. The new vtable references introduce 235 distinct imports.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed. No authored code ran on EE hardware or in the game.

## Imports closed

`_vt$15SkillPoisonRain`, `_vt$15SkillShieldBash`, `_vt$15SkillWizardBeam`, `_vt$16SkillColdWeapons`, `_vt$16SkillConeOfFrost`, `_vt$16SkillCriticalHit`, `_vt$16SkillDiseaseBolt`, `_vt$16SkillFireWeapons`, `_vt$16SkillGroundPound`, `_vt$16SkillPoisonArrow`, `_vt$16SkillShockOfFire`, `_vt$17SkillConvertEnemy`, `_vt$17SkillDiseaseTrail`, `_vt$17SkillExplodeArrow`, `_vt$17SkillMinorHealing`, `_vt$17SkillRegeneration`, `_vt$17SkillShockOfFrost`, `_vt$17SkillUndeadShield`, `_vt$18SkillAncestralCall`, `_vt$18SkillBlindingLight`, `_vt$18SkillConvertUndead`, `_vt$18SkillDiseaseShield`, `_vt$18SkillDismissUndead`, `_vt$18SkillHammerOfWrath`, `_vt$18SkillRepulseUndead`, `_vt$19SkillSummonSkeleton`, `_vt$21SkillLightningWeapons`, `_vt$22SkillRangerCriticalHit`, `_vt$9SkillBash`, `_vt$9SkillRoot`, `canCast__5SkillP8Creature`.

## Imports opened

The vtables introduce 235 distinct unresolved slot imports:

`__tf15SkillPoisonRain`, `__tf15SkillShieldBash`, `__tf15SkillWizardBeam`, `__tf16SkillColdWeapons`, `__tf16SkillConeOfFrost`, `__tf16SkillCriticalHit`, `__tf16SkillDiseaseBolt`, `__tf16SkillFireWeapons`, `__tf16SkillGroundPound`, `__tf16SkillPoisonArrow`, `__tf16SkillShockOfFire`, `__tf17SkillConvertEnemy`, `__tf17SkillDiseaseTrail`, `__tf17SkillExplodeArrow`, `__tf17SkillMinorHealing`, `__tf17SkillRegeneration`, `__tf17SkillShockOfFrost`, `__tf17SkillUndeadShield`, `__tf18SkillAncestralCall`, `__tf18SkillBlindingLight`, `__tf18SkillConvertUndead`, `__tf18SkillDiseaseShield`, `__tf18SkillDismissUndead`, `__tf18SkillHammerOfWrath`, `__tf18SkillRepulseUndead`, `__tf19SkillSummonSkeleton`, `__tf21SkillLightningWeapons`, `__tf22SkillRangerCriticalHit`, `__tf9SkillBash`, `__tf9SkillRoot`, `canCast__15SkillPoisonRainP8Creature`, `canCast__15SkillShieldBashP8Creature`, `canCast__16SkillColdWeaponsP8Creature`, `canCast__16SkillCriticalHitP8Creature`, `canCast__16SkillDiseaseBoltP8Creature`, `canCast__16SkillFireWeaponsP8Creature`, `canCast__16SkillGroundPoundP8Creature`, `canCast__17SkillDiseaseTrailP8Creature`, `canCast__17SkillShockOfFrostP8Creature`, `canCast__17SkillUndeadShieldP8Creature`, `canCast__18SkillAncestralCallP8Creature`, `canCast__18SkillBlindingLightP8Creature`, `canCast__18SkillConvertUndeadP8Creature`, `canCast__18SkillDiseaseShieldP8Creature`, `canCast__18SkillHammerOfWrathP8Creature`, `canCast__19SkillSummonSkeletonP8Creature`, `canCast__21SkillLightningWeaponsP8Creature`, `canCast__22SkillRangerCriticalHitP8Creature`, `canCast__9SkillBashP8Creature`, `cast__15SkillPoisonRainiP8CreatureG6Point3s`, `cast__15SkillShieldBashiP8CreatureG6Point3s`, `cast__15SkillWizardBeamiP8CreatureG6Point3s`, `cast__16SkillColdWeaponsiP8CreatureG6Point3s`, `cast__16SkillConeOfFrostiP8CreatureG6Point3s`, `cast__16SkillCriticalHitiP8CreatureG6Point3s`, `cast__16SkillDiseaseBoltiP8CreatureG6Point3s`, `cast__16SkillFireWeaponsiP8CreatureG6Point3s`, `cast__16SkillGroundPoundiP8CreatureG6Point3s`, `cast__16SkillPoisonArrowiP8CreatureG6Point3s`, `cast__16SkillShockOfFireiP8CreatureG6Point3s`, `cast__17SkillConvertEnemyiP8CreatureG6Point3s`, `cast__17SkillDiseaseTrailiP8CreatureG6Point3s`, `cast__17SkillExplodeArrowiP8CreatureG6Point3s`, `cast__17SkillMinorHealingiP8CreatureG6Point3s`, `cast__17SkillRegenerationiP8CreatureG6Point3s`, `cast__17SkillShockOfFrostiP8CreatureG6Point3s`, `cast__17SkillUndeadShieldiP8CreatureG6Point3s`, `cast__18SkillAncestralCalliP8CreatureG6Point3s`, `cast__18SkillBlindingLightiP8CreatureG6Point3s`, `cast__18SkillConvertUndeadiP8CreatureG6Point3s`, `cast__18SkillDiseaseShieldiP8CreatureG6Point3s`, `cast__18SkillDismissUndeadiP8CreatureG6Point3s`, `cast__18SkillHammerOfWrathiP8CreatureG6Point3s`, `cast__18SkillRepulseUndeadiP8CreatureG6Point3s`, `cast__19SkillSummonSkeletoniP8CreatureG6Point3s`, `cast__21SkillLightningWeaponsiP8CreatureG6Point3s`, `cast__22SkillRangerCriticalHitiP8CreatureG6Point3s`, `cast__9SkillBashiP8CreatureG6Point3s`, `cast__9SkillRootiP8CreatureG6Point3s`, `getEnergyCost__15SkillPoisonRaini`, `getEnergyCost__15SkillShieldBashi`, `getEnergyCost__15SkillWizardBeami`, `getEnergyCost__16SkillColdWeaponsi`, `getEnergyCost__16SkillConeOfFrosti`, `getEnergyCost__16SkillCriticalHiti`, `getEnergyCost__16SkillDiseaseBolti`, `getEnergyCost__16SkillFireWeaponsi`, `getEnergyCost__16SkillGroundPoundi`, `getEnergyCost__16SkillPoisonArrowi`, `getEnergyCost__16SkillShockOfFirei`, `getEnergyCost__17SkillConvertEnemyi`, `getEnergyCost__17SkillDiseaseTraili`, `getEnergyCost__17SkillExplodeArrowi`, `getEnergyCost__17SkillMinorHealingi`, `getEnergyCost__17SkillShockOfFrosti`, `getEnergyCost__17SkillUndeadShieldi`, `getEnergyCost__18SkillAncestralCalli`, `getEnergyCost__18SkillBlindingLighti`, `getEnergyCost__18SkillConvertUndeadi`, `getEnergyCost__18SkillDiseaseShieldi`, `getEnergyCost__18SkillDismissUndeadi`, `getEnergyCost__18SkillHammerOfWrathi`, `getEnergyCost__18SkillRepulseUndeadi`, `getEnergyCost__19SkillSummonSkeletoni`, `getEnergyCost__21SkillLightningWeaponsi`, `getEnergyCost__22SkillRangerCriticalHiti`, `getEnergyCost__9SkillBashi`, `getEnergyCost__9SkillRooti`, `getIcon__15SkillShieldBash`, `getIcon__15SkillWizardBeam`, `getIcon__16SkillColdWeapons`, `getIcon__16SkillConeOfFrost`, `getIcon__16SkillCriticalHit`, `getIcon__16SkillDiseaseBolt`, `getIcon__16SkillFireWeapons`, `getIcon__16SkillGroundPound`, `getIcon__16SkillPoisonArrow`, `getIcon__16SkillShockOfFire`, `getIcon__17SkillConvertEnemy`, `getIcon__17SkillDiseaseTrail`, `getIcon__17SkillExplodeArrow`, `getIcon__17SkillMinorHealing`, `getIcon__17SkillShockOfFrost`, `getIcon__17SkillUndeadShield`, `getIcon__18SkillAncestralCall`, `getIcon__18SkillBlindingLight`, `getIcon__18SkillConvertUndead`, `getIcon__18SkillDiseaseShield`, `getIcon__18SkillDismissUndead`, `getIcon__18SkillHammerOfWrath`, `getIcon__18SkillRepulseUndead`, `getIcon__19SkillSummonSkeleton`, `getIcon__21SkillLightningWeapons`, `getIcon__22SkillRangerCriticalHit`, `getIcon__9SkillBash`, `getIcon__9SkillRoot`, `getLevelRequirement__15SkillPoisonRaini`, `getLevelRequirement__15SkillShieldBashi`, `getLevelRequirement__15SkillWizardBeami`, `getLevelRequirement__16SkillColdWeaponsi`, `getLevelRequirement__16SkillCriticalHiti`, `getLevelRequirement__16SkillDiseaseBolti`, `getLevelRequirement__16SkillFireWeaponsi`, `getLevelRequirement__16SkillShockOfFirei`, `getLevelRequirement__17SkillDiseaseTraili`, `getLevelRequirement__17SkillMinorHealingi`, `getLevelRequirement__17SkillRegenerationi`, `getLevelRequirement__17SkillShockOfFrosti`, `getLevelRequirement__17SkillUndeadShieldi`, `getLevelRequirement__18SkillBlindingLighti`, `getLevelRequirement__18SkillDiseaseShieldi`, `getLevelRequirement__18SkillDismissUndeadi`, `getLevelRequirement__18SkillHammerOfWrathi`, `getLevelRequirement__18SkillRepulseUndeadi`, `getLevelRequirement__19SkillSummonSkeletoni`, `getLevelRequirement__21SkillLightningWeaponsi`, `getLevelRequirement__22SkillRangerCriticalHiti`, `getLevelRequirement__9SkillBashi`, `getLevelRequirement__9SkillRooti`, `getRankDescription__15SkillPoisonRainPUsi`, `getRankDescription__15SkillShieldBashPUsi`, `getRankDescription__15SkillWizardBeamPUsi`, `getRankDescription__16SkillColdWeaponsPUsi`, `getRankDescription__16SkillConeOfFrostPUsi`, `getRankDescription__16SkillCriticalHitPUsi`, `getRankDescription__16SkillDiseaseBoltPUsi`, `getRankDescription__16SkillFireWeaponsPUsi`, `getRankDescription__16SkillGroundPoundPUsi`, `getRankDescription__16SkillPoisonArrowPUsi`, `getRankDescription__16SkillShockOfFirePUsi`, `getRankDescription__17SkillConvertEnemyPUsi`, `getRankDescription__17SkillDiseaseTrailPUsi`, `getRankDescription__17SkillExplodeArrowPUsi`, `getRankDescription__17SkillMinorHealingPUsi`, `getRankDescription__17SkillRegenerationPUsi`, `getRankDescription__17SkillShockOfFrostPUsi`, `getRankDescription__17SkillUndeadShieldPUsi`, `getRankDescription__18SkillAncestralCallPUsi`, `getRankDescription__18SkillBlindingLightPUsi`, `getRankDescription__18SkillConvertUndeadPUsi`, `getRankDescription__18SkillDiseaseShieldPUsi`, `getRankDescription__18SkillDismissUndeadPUsi`, `getRankDescription__18SkillHammerOfWrathPUsi`, `getRankDescription__18SkillRepulseUndeadPUsi`, `getRankDescription__19SkillSummonSkeletonPUsi`, `getRankDescription__21SkillLightningWeaponsPUsi`, `getRankDescription__22SkillRangerCriticalHitPUsi`, `getRankDescription__9SkillBashPUsi`, `getRankDescription__9SkillRootPUsi`, `getSkillPointCost__15SkillPoisonRaini`, `getSkillPointCost__15SkillWizardBeami`, `getSkillPointCost__16SkillColdWeaponsi`, `getSkillPointCost__16SkillDiseaseBolti`, `getSkillPointCost__16SkillFireWeaponsi`, `getSkillPointCost__17SkillDiseaseTraili`, `getSkillPointCost__17SkillRegenerationi`, `getSkillPointCost__17SkillShockOfFrosti`, `getSkillPointCost__17SkillUndeadShieldi`, `getSkillPointCost__18SkillBlindingLighti`, `getSkillPointCost__18SkillDiseaseShieldi`, `getSkillPointCost__18SkillDismissUndeadi`, `getSkillPointCost__18SkillHammerOfWrathi`, `getSkillPointCost__18SkillRepulseUndeadi`, `getSkillPointCost__19SkillSummonSkeletoni`, `getSkillPointCost__21SkillLightningWeaponsi`, `initRamps__15SkillPoisonRain`, `initRamps__15SkillShieldBash`, `initRamps__15SkillWizardBeam`, `initRamps__16SkillColdWeapons`, `initRamps__16SkillConeOfFrost`, `initRamps__16SkillCriticalHit`, `initRamps__16SkillDiseaseBolt`, `initRamps__16SkillFireWeapons`, `initRamps__16SkillGroundPound`, `initRamps__16SkillPoisonArrow`, `initRamps__16SkillShockOfFire`, `initRamps__17SkillConvertEnemy`, `initRamps__17SkillDiseaseTrail`, `initRamps__17SkillExplodeArrow`, `initRamps__17SkillMinorHealing`, `initRamps__17SkillRegeneration`, `initRamps__17SkillShockOfFrost`, `initRamps__17SkillUndeadShield`, `initRamps__18SkillAncestralCall`, `initRamps__18SkillBlindingLight`, `initRamps__18SkillConvertUndead`, `initRamps__18SkillDiseaseShield`, `initRamps__18SkillDismissUndead`, `initRamps__18SkillHammerOfWrath`, `initRamps__18SkillRepulseUndead`, `initRamps__19SkillSummonSkeleton`, `initRamps__21SkillLightningWeapons`, `initRamps__22SkillRangerCriticalHit`, `initRamps__9SkillBash`, `initRamps__9SkillRoot`.

## Build-plan entries needed

The shared plan and snapshot were not edited. Append the two inputs below, add the shown source hashes to each `files` map, and use input orders 5115–5116. The private plan and source copies used for the full build are under ignored `work/w12-skill-b-private-source/`; its plan SHA-256 is `d01e633cbbcd41648b097478f0fa75bc16b832551dad0c9dd632f33b3b2ed375`, snapshot SHA-256 is `eb1251c753c96dd06a63542a890a91280e073959f6bb801ca78d600cf20171c8`. The partial build output is under ignored `work/w12-skill-b-private-plan-build/`.

Plan entries:

```json
[
  {
    "input_order": 5115,
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "header_closure": [],
    "object_name": "w12_skill_b_can_cast.o",
    "source": "sources/w12-skill-b-can-cast/can_cast.cpp",
    "sha256": "635764b40aaa03f530118fcb3100b64bd29541781790a5169eea1d3fa28ef1fd",
    "emitted_functions": [
      {
        "name": "canCast__5SkillP8Creature",
        "compiled_body_bytes": 8
      }
    ]
  },
  {
    "input_order": 5116,
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "header_closure": [],
    "object_name": "w12_skill_b_vtables.o",
    "source": "sources/w12-skill-b-vtables/skill_vtables.cpp",
    "sha256": "9cbd4df7f5db972b2fd31a0a78780381d726b39ae291c990a6006b1dd7041614",
    "emitted_functions": []
  }
]
```

Corresponding snapshot input entries (object hashes identify the locally compiled EE objects):

```json
[
  {
    "input_order": 5115,
    "component": "w12-skill-b-can-cast",
    "object_name": "w12_skill_b_can_cast.o",
    "private_object_sha256": "72f19f9d7cc2e8eb780721a24626025df44a1ddfce02461068318f870dd3c984",
    "emitted_functions": [
      {
        "name": "canCast__5SkillP8Creature",
        "compiled_body_bytes": 8
      }
    ],
    "kind": "independently_authored_source",
    "source": "sources/w12-skill-b-can-cast/can_cast.cpp"
  },
  {
    "input_order": 5116,
    "component": "w12-skill-b-vtables",
    "object_name": "w12_skill_b_vtables.o",
    "private_object_sha256": "a2e4922184bcdaed511ea49153d4cee9a38fd2332abe12cb0b4ba419d976f70e",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/w12-skill-b-vtables/skill_vtables.cpp"
  }
]
```

File hashes:

```json
{
  "sources/w12-skill-b-can-cast/can_cast.cpp": "635764b40aaa03f530118fcb3100b64bd29541781790a5169eea1d3fa28ef1fd",
  "sources/w12-skill-b-vtables/skill_vtables.cpp": "9cbd4df7f5db972b2fd31a0a78780381d726b39ae291c990a6006b1dd7041614"
}
```

## Authored but never executed

- `canCast` and the 30 tables were compiled with the reviewed EE toolchain and tested on the host. None was run on PS2 hardware, PCSX2, or in the game.

## What is unknown

- Casting, progression, ramp behavior, RTTI initialization, and gameplay have not been executed.
- The slot imports listed above require independent evidence before their behavior is reconstructed.
