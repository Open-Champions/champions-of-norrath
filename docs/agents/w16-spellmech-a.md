# w16-spellmech-a: spell constructors, three clone functions and two actor constructors

Package `w16-spellmech-a`, branch `agents/w16-spellmech-a`. Twenty-one functions, one commit each, under
`experimental/sources/w16-spellmech-a-*`; four later comment-only commits correct image sizes in headers and three commits
inline a helper (see "Findings"). Each source begins with a comment giving the observed address and size, the function's
behaviour in numbered steps, what is inferred, what is unknown, and that nothing ran on the EE. No bytes, listings or image
data are committed. Each unit directory holds the source and a `host_check.cpp` (a host test, not an EE input).

Private reference executable: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and sizes below are that file's symbol values
(`nm -S`). "Compiled bytes" is the size of the function as compiled by the reviewed EE g++ (GCC 15.2.0) with the `authored_cpp`
flags; it is not a byte match with the image and the sizes differ from the image's. No match percentage is claimed.

Private tooling, outside git (it loads the image; never commit it or its output):
`/Users/case/norrath-agents/priv-w16-spellmech-a/` (paired-run R5900 harness, per-unit scenario scripts `t_*.py`, mutation
runners `mut.sh` and `hm.sh`, plan generator `mkplan.py`).

## Units

| source directory | symbol | image address | image bytes | compiled bytes | source sha256 |
|---|---|---|---|---|---|
| `w16-spellmech-a-clone-glowing-shaft` | `clone__12GlowingShaft` | `0x124948` | 296 | 288 | `ac1ecedd6b68e6fc25b0c223f1290f4c2ecea8cb653fc8194c693737be555a02` |
| `w16-spellmech-a-clone-holy-bolt` | `clone__8HolyBolt` | `0x125170` | 544 | 560 | `53762252b3dbeddf25d37abc7963eae9a1b3b3da0e83393d8ee2ec084c07df62` |
| `w16-spellmech-a-clone-lost-soul` | `clone__8LostSoul` | `0x1255a0` | 512 | 528 | `f4dd0b9c8e0e98d04bdc4b5091593d905c2c35d8977981ffc5a996d4409f4e73` |
| `w16-spellmech-a-ctor-loose-fire` | `__9LooseFireG6Point3P10GameObject` | `0x1e6490` | 212 | 176 | `74e2126c1f132d7d3bca3497cfcdba9230a00a1e5240614136823bf705130746` |
| `w16-spellmech-a-ctor-mummy` | `__5MummyG6Point3sPPcPc` | `0x236b68` | 1600 | 1580 | `e3f6b9ffbf9f88b1e6d8bcd4eea5795c46c0d6480133729bd130f5090b5930c1` |
| `w16-spellmech-a-ctor-charge` | `__11SpellChargeiP8CreatureP5Skill` | `0x290e78` | 632 | 560 | `5f89f2de825a2fb0873b62214a240fef9cd006d4f2dd55b913761182e557d3d4` |
| `w16-spellmech-a-ctor-disease-trail` | `__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill` | `0x294308` | 708 | 680 | `2feb9ba632b99289c7b0b8b39b239c56bc8bcea02c791f15f5f1c1a2df9f809e` |
| `w16-spellmech-a-ctor-convert-enemy` | `__17SpellConvertEnemyiP8CreatureP5Skill` | `0x298360` | 484 | 524 | `58eb5beace4455882314e962b15511c8d48ca037d2ea300bacc519eb8f96748c` |
| `w16-spellmech-a-ctor-convert-undead` | `__18SpellConvertUndeadiP8CreatureP5Skill` | `0x298e10` | 508 | 548 | `a98c30dddf60f9a42889755d4651c4d14797859fb1fa7b9b127cae8cdcd171af` |
| `w16-spellmech-a-ctor-cyclone` | `__12SpellCycloneiP8CreatureP5Skill` | `0x299758` | 684 | 692 | `7a3c7d46ff47bebf399a55067a5c5cf5dca3cdcf59f645186067c08e636e777f` |
| `w16-spellmech-a-ctor-fire-flares` | `__15SpellFireFlaresiP8CreatureG6Point3P5Skill` | `0x2a3ad8` | 1032 | 1032 | `407ee2a4a386e5fc4885244c491236496bdb69afad945c7b2abec5bdd3f36f16` |
| `w16-spellmech-a-ctor-bash` | `__9SpellBashiP8CreatureP5Skill` | `0x2a52c0` | 384 | 428 | `5006cdcf203e531352d6ac69149d44da1cc12e58861e8fc7aa1ddd3815b51bc3` |
| `w16-spellmech-a-ctor-blessed` | `__12SpellBlessediP8CreatureP5Skill` | `0x2a5f70` | 856 | 792 | `20ae703fd2adb1321ad22d95db9948fbef2a80cdb33cddf7607561cffa9e2e0a` |
| `w16-spellmech-a-ctor-ancestral-call` | `__18SpellAncestralCalliP8CreatureP5Skill` | `0x2a7620` | 844 | 800 | `4a8afafdd80fe45174074efb9c724bacb1094ee3e9152c99ee11b6bd33066805` |
| `w16-spellmech-a-ctor-lightning-weapons` | `__21SpellLightningWeaponsiP8CreatureP5Skill` | `0x2a9220` | 1176 | 1192 | `07daa719fe93067dd7dd6e2d200401dfc29487c9856b87a38b2e3f0071b2613e` |
| `w16-spellmech-a-ctor-fire-weapons` | `__16SpellFireWeaponsiP8CreatureP5Skill` | `0x2a9eb8` | 1124 | 1160 | `db77162df752f9aeba5f5a175a24ae9ae151a1384a110f9d67c0daf6169778f0` |
| `w16-spellmech-a-ctor-cold-weapons` | `__16SpellColdWeaponsiP8CreatureP5Skill` | `0x2ab548` | 1168 | 1200 | `4af360c91171c75631115e16e66cebe5bbf7124e2600e39f7368a755e636bd3e` |
| `w16-spellmech-a-ctor-undead-shield` | `__17SpellUndeadShieldiP8CreatureP5Skill` | `0x2affa8` | 440 | 416 | `442cd1d9252751212f781b6adc7d180854a1019e8b0207ccbccce5ba182f76b7` |
| `w16-spellmech-a-ctor-wizard-beam` | `__15SpellWizardBeamiP8CreatureP5Skill` | `0x2b6ca0` | 1080 | 1032 | `9c6b48d30d9605b2ef33d4ccf30b458b3b1787479496ab1cb358284b44365f19` |
| `w16-spellmech-a-ctor-ranger-crit` | `__22SpellRangerCriticalHitiP8CreatureP5Skill` | `0x2c7498` | 308 | 300 | `d5a0f8c64c0c253cd3a99c7f55a3464a58515b76875da19238b7f42925ed2fd5` |
| `w16-spellmech-a-ctor-shield-bash` | `__15SpellShieldBashiP8CreatureP5Skill` | `0x2c7f38` | 384 | 428 | `456068f9a9ccf7f092e35bef557c75e64e831fe50d3de40ec77e766138f202c6` |

Three of these (`ctor-bash`, `ctor-convert-undead`, `ctor-ranger-crit`) started from the sources on the unmerged branch
`agents/w14-m-spell-ctors-b` (`ctor.cpp` in its `-ctor-bash`, `-ctor-convert-undead` and `-ctor-ranger-crit` directories).
Those were not merged because they had no committed check. Here the helper that turns a stored 32-bit address back into a
pointer was made switchable for host builds, and a `host_check.cpp` was added. The EE object code is unchanged by that edit
(the disassembly of the EE objects was compared before and after: identical).

## Checks performed

- **EE compile.** Every unit compiles with `work/toolchains/ee` (GCC 15.2.0) and the `authored_cpp` flags with `-c`
  (`cc.sh`). Each object defines exactly the expected function (see the plan entries) and no other function symbol.
- **Private link.** The 21 objects were linked with `ld -m elf32lr5900 -r` against the baseline
  `work/plan16-build/game-components-partial.o` (841 undefined symbols, the current shared-branch baseline). No duplicate
  definition; the 21 names left the undefined list and 23 names entered it (listed under "Imports opened"): 841 to 843. This
  is a private link; it does not replace the integrator's full partial build.
- **Host contracts.** Each `host_check.cpp` builds with `clang++ -std=c++17 -Wall -Wextra` and prints `ok`. A host build stores
  32-bit addresses as offsets into an arena (`NORRATH_HOST_ARENA`; the check supplies `norrath_host_pointer` and
  `norrath_host_address`) and mocks every callee, logging call order and arguments. The checks compute expected values with an
  independent model (for example the weapon-record update, the rejection sampling in Blessed, the name table in Mummy), cover
  null and failed-allocation paths where the source has them, and compare floats bit for bit.
- **Mutation checks of the host contracts.** Every unit's host check was run against deliberate single defects in the source
  (changed constant, comparison, offset, call argument, stored value or loop bound), applied one at a time with the source
  restored afterwards. Every unit has at least two mutants that its check rejects; where a mutant first survived (an untested
  boundary: ConvertUndead's `<= 0.0f` test, Blessed's `1.0f < sum` test) the check was extended until it failed. Not caught, by
  design: the last bit of image float constants that are only used through a truncating conversion (the `frameNm` shift
  constants of the cold and lightning weapon spells, Mummy's 0.1 threshold), because no input value distinguishes it.
- **Paired original/authored run (private).** Each function was run on the private R5900 interpreter from identical memory
  snapshots against the image's original, with the same callee stubs, comparing call logs (arguments, floats, stack words for
  stack-passed floats), every changed byte outside the stack, and coverage. All 21 report 0 mismatches and no `HALT` line;
  uncovered original words are only `nop`s and annulled delay slots. Coverage of the original's words: Bash 95/96,
  ConvertUndead 127/127, RangerCrit 77/77, ShieldBash 95/96, ConvertEnemy 121/121, UndeadShield 110/110, Cyclone 171/171,
  Charge 158/158, Blessed 214/214, LooseFire 53/53, GlowingShaft 74/74, HolyBolt 136/136, LostSoul 128/128, DiseaseTrail 176/177,
  AncestralCall 210/211, FireFlares 258/258, WizardBeam 270/270, ColdWeapons 288/292, FireWeapons 277/281, LightningWeapons
  290/294, Mummy 396/400. The paired runs and their mutants are not reproducible from the repository (they need the image).
- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Findings that shaped the code

- Struct arguments over eight bytes (`Point3`) are passed by reference to a caller-made copy on this ABI, and arguments past the
  fourth go in the eighth-register slots (`t0`); the declared prototypes reproduce both (checked by the paired runs, which log
  the registers and the stack words of the twelve-float `LightEffect` constructor call).
- Several constructors store `1.0f / 0.0f` computed at run time as light fields. On this FPU that is the largest float
  (0x7fffffff), and the interpreter models it so. The sources divide by a `volatile` zero so the compiler cannot fold it.
  The host checks compare against the same run-time division on the host, so they pin "stored from 1.0f / 0.0f", not the EE
  bit pattern.
- The three weapon spells (cold, fire, lightning) share one record update. It was first a separate local function, which the
  compiler emitted as a second symbol; it is now `always_inline` so each unit emits exactly one function (commits after the
  first three).
- `__5Mummy...` receives the `short` type in a register and re-extends its low 16 bits (the image does this with a shift pair), so
  the source takes the argument as an `int` and narrows it.
- `__17SpellDiseaseTrail...` and `__15SpellFireFlares...` divide by values computed from the rank; the image emits no zero check
  and the compiled code does. A zero divisor needs a rank in a range no caller is known to use; it is not exercised.
- FireFlares stores `0` in the word it later uses as the flares' radial multiplier, so every flare is created at the point itself.
  This is read from the instructions and confirmed by the paired runs, not assumed.

## Build-plan entries needed

These 21 entries belong in `experimental/build-plan.json` `inputs` (and the matching snapshot records), in the order the
integrator assigns; `input_order` is left to the integrator. They were generated from the committed sources with the reviewed
`authored_cpp` flags. `host_check.cpp` files are not inputs. Nothing in shared files was edited here.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_clone_glowing_shaft.o",
  "profile": "authored_cpp",
  "sha256": "ac1ecedd6b68e6fc25b0c223f1290f4c2ecea8cb653fc8194c693737be555a02",
  "source": "sources/w16-spellmech-a-clone-glowing-shaft/clone.cpp",
  "emitted_functions": [
   {
    "name": "clone__12GlowingShaft",
    "compiled_body_bytes": 288
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_clone_holy_bolt.o",
  "profile": "authored_cpp",
  "sha256": "53762252b3dbeddf25d37abc7963eae9a1b3b3da0e83393d8ee2ec084c07df62",
  "source": "sources/w16-spellmech-a-clone-holy-bolt/clone.cpp",
  "emitted_functions": [
   {
    "name": "clone__8HolyBolt",
    "compiled_body_bytes": 560
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_clone_lost_soul.o",
  "profile": "authored_cpp",
  "sha256": "f4dd0b9c8e0e98d04bdc4b5091593d905c2c35d8977981ffc5a996d4409f4e73",
  "source": "sources/w16-spellmech-a-clone-lost-soul/clone.cpp",
  "emitted_functions": [
   {
    "name": "clone__8LostSoul",
    "compiled_body_bytes": 528
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_ancestral_call.o",
  "profile": "authored_cpp",
  "sha256": "4a8afafdd80fe45174074efb9c724bacb1094ee3e9152c99ee11b6bd33066805",
  "source": "sources/w16-spellmech-a-ctor-ancestral-call/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__18SpellAncestralCalliP8CreatureP5Skill",
    "compiled_body_bytes": 800
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_bash.o",
  "profile": "authored_cpp",
  "sha256": "5006cdcf203e531352d6ac69149d44da1cc12e58861e8fc7aa1ddd3815b51bc3",
  "source": "sources/w16-spellmech-a-ctor-bash/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__9SpellBashiP8CreatureP5Skill",
    "compiled_body_bytes": 428
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_blessed.o",
  "profile": "authored_cpp",
  "sha256": "20ae703fd2adb1321ad22d95db9948fbef2a80cdb33cddf7607561cffa9e2e0a",
  "source": "sources/w16-spellmech-a-ctor-blessed/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__12SpellBlessediP8CreatureP5Skill",
    "compiled_body_bytes": 792
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_charge.o",
  "profile": "authored_cpp",
  "sha256": "5f89f2de825a2fb0873b62214a240fef9cd006d4f2dd55b913761182e557d3d4",
  "source": "sources/w16-spellmech-a-ctor-charge/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__11SpellChargeiP8CreatureP5Skill",
    "compiled_body_bytes": 560
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_cold_weapons.o",
  "profile": "authored_cpp",
  "sha256": "4af360c91171c75631115e16e66cebe5bbf7124e2600e39f7368a755e636bd3e",
  "source": "sources/w16-spellmech-a-ctor-cold-weapons/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__16SpellColdWeaponsiP8CreatureP5Skill",
    "compiled_body_bytes": 1200
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_convert_enemy.o",
  "profile": "authored_cpp",
  "sha256": "58eb5beace4455882314e962b15511c8d48ca037d2ea300bacc519eb8f96748c",
  "source": "sources/w16-spellmech-a-ctor-convert-enemy/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__17SpellConvertEnemyiP8CreatureP5Skill",
    "compiled_body_bytes": 524
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_convert_undead.o",
  "profile": "authored_cpp",
  "sha256": "a98c30dddf60f9a42889755d4651c4d14797859fb1fa7b9b127cae8cdcd171af",
  "source": "sources/w16-spellmech-a-ctor-convert-undead/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__18SpellConvertUndeadiP8CreatureP5Skill",
    "compiled_body_bytes": 548
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_cyclone.o",
  "profile": "authored_cpp",
  "sha256": "7a3c7d46ff47bebf399a55067a5c5cf5dca3cdcf59f645186067c08e636e777f",
  "source": "sources/w16-spellmech-a-ctor-cyclone/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__12SpellCycloneiP8CreatureP5Skill",
    "compiled_body_bytes": 692
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_disease_trail.o",
  "profile": "authored_cpp",
  "sha256": "2feb9ba632b99289c7b0b8b39b239c56bc8bcea02c791f15f5f1c1a2df9f809e",
  "source": "sources/w16-spellmech-a-ctor-disease-trail/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill",
    "compiled_body_bytes": 680
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_fire_flares.o",
  "profile": "authored_cpp",
  "sha256": "407ee2a4a386e5fc4885244c491236496bdb69afad945c7b2abec5bdd3f36f16",
  "source": "sources/w16-spellmech-a-ctor-fire-flares/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__15SpellFireFlaresiP8CreatureG6Point3P5Skill",
    "compiled_body_bytes": 1032
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_fire_weapons.o",
  "profile": "authored_cpp",
  "sha256": "db77162df752f9aeba5f5a175a24ae9ae151a1384a110f9d67c0daf6169778f0",
  "source": "sources/w16-spellmech-a-ctor-fire-weapons/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__16SpellFireWeaponsiP8CreatureP5Skill",
    "compiled_body_bytes": 1160
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_lightning_weapons.o",
  "profile": "authored_cpp",
  "sha256": "07daa719fe93067dd7dd6e2d200401dfc29487c9856b87a38b2e3f0071b2613e",
  "source": "sources/w16-spellmech-a-ctor-lightning-weapons/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__21SpellLightningWeaponsiP8CreatureP5Skill",
    "compiled_body_bytes": 1192
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_loose_fire.o",
  "profile": "authored_cpp",
  "sha256": "74e2126c1f132d7d3bca3497cfcdba9230a00a1e5240614136823bf705130746",
  "source": "sources/w16-spellmech-a-ctor-loose-fire/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__9LooseFireG6Point3P10GameObject",
    "compiled_body_bytes": 176
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_mummy.o",
  "profile": "authored_cpp",
  "sha256": "e3f6b9ffbf9f88b1e6d8bcd4eea5795c46c0d6480133729bd130f5090b5930c1",
  "source": "sources/w16-spellmech-a-ctor-mummy/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__5MummyG6Point3sPPcPc",
    "compiled_body_bytes": 1580
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_ranger_crit.o",
  "profile": "authored_cpp",
  "sha256": "d5a0f8c64c0c253cd3a99c7f55a3464a58515b76875da19238b7f42925ed2fd5",
  "source": "sources/w16-spellmech-a-ctor-ranger-crit/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__22SpellRangerCriticalHitiP8CreatureP5Skill",
    "compiled_body_bytes": 300
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_shield_bash.o",
  "profile": "authored_cpp",
  "sha256": "456068f9a9ccf7f092e35bef557c75e64e831fe50d3de40ec77e766138f202c6",
  "source": "sources/w16-spellmech-a-ctor-shield-bash/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__15SpellShieldBashiP8CreatureP5Skill",
    "compiled_body_bytes": 428
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_undead_shield.o",
  "profile": "authored_cpp",
  "sha256": "442cd1d9252751212f781b6adc7d180854a1019e8b0207ccbccce5ba182f76b7",
  "source": "sources/w16-spellmech-a-ctor-undead-shield/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__17SpellUndeadShieldiP8CreatureP5Skill",
    "compiled_body_bytes": 416
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16spellmech_ctor_wizard_beam.o",
  "profile": "authored_cpp",
  "sha256": "9c6b48d30d9605b2ef33d4ccf30b458b3b1787479496ab1cb358284b44365f19",
  "source": "sources/w16-spellmech-a-ctor-wizard-beam/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__15SpellWizardBeamiP8CreatureP5Skill",
    "compiled_body_bytes": 1032
   }
  ]
 }
]
```

## Imports closed (names)

All 21 names of the brief:
- `clone__12GlowingShaft`
- `clone__8HolyBolt`
- `clone__8LostSoul`
- `__9LooseFireG6Point3P10GameObject`
- `__5MummyG6Point3sPPcPc`
- `__11SpellChargeiP8CreatureP5Skill`
- `__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill`
- `__17SpellConvertEnemyiP8CreatureP5Skill`
- `__18SpellConvertUndeadiP8CreatureP5Skill`
- `__12SpellCycloneiP8CreatureP5Skill`
- `__15SpellFireFlaresiP8CreatureG6Point3P5Skill`
- `__9SpellBashiP8CreatureP5Skill`
- `__12SpellBlessediP8CreatureP5Skill`
- `__18SpellAncestralCalliP8CreatureP5Skill`
- `__21SpellLightningWeaponsiP8CreatureP5Skill`
- `__16SpellFireWeaponsiP8CreatureP5Skill`
- `__16SpellColdWeaponsiP8CreatureP5Skill`
- `__17SpellUndeadShieldiP8CreatureP5Skill`
- `__15SpellWizardBeamiP8CreatureP5Skill`
- `__22SpellRangerCriticalHitiP8CreatureP5Skill`
- `__15SpellShieldBashiP8CreatureP5Skill`

They take effect when the entries above are merged and the partial build is repeated. A private link of the 21 objects against
the 841-name baseline gave 843 undefined names (21 closed, 23 opened).

## Imports opened (new unresolved names)

Undefined in these sources, defined by nothing in the current plan: `_vt$11SpellCharge`, `_vt$12DiseaseTrail`, `_vt$12SpellBlessed`, `_vt$12SpellCyclone`, `_vt$15SpellFireFlares`, `_vt$15SpellShieldBash`, `_vt$15SpellWizardBeam`, `_vt$16SpellColdWeapons`, `_vt$16SpellFireWeapons`, `_vt$17SpellConvertEnemy`, `_vt$17SpellDiseaseTrail`, `_vt$17SpellUndeadShield`, `_vt$18SpellAncestralCall`, `_vt$18SpellConvertUndead`, `_vt$21SpellLightningWeapons`, `_vt$22SpellRangerCriticalHit`, `_vt$9FireFlare`, `_vt$9LooseFire`, `_vt$9SpellBash`, `hsv__Ffff`, `mummyResourceMap`, `objectGetListInRadius__FG6Point3sfP8ListHead`, `setWeapon__5Mummy`.

Of these, the `_vt$...` names are the class tables the constructors store at +20 or +72 (data tables owned elsewhere);
`hsv__Ffff` (image 0x1589f8), `setWeapon__5Mummy` (0x23bcf0), `objectGetListInRadius__FG6Point3sfP8ListHead` (0x13f158) are
functions; `mummyResourceMap` is a data table (image 0x42d2b0, 464 bytes). `addEffect__FiG6Point3` was already unresolved and
is still used by the `LooseFire` constructor. The remaining imports used by these sources (`blockAlloc__Fi`,
`lumpFindResource__FPcT0`, `soundPlaySequence__...`, `eRand__...`, the GameObject/Creature base constructors, `frameNm`,
`syncRandState`, `player`, and similar) are already defined by the plan.

## Authored but never executed

All 21 functions. None ran on the EE or in a game. The paired runs are on a private interpreter with stubbed callees (not the
real ones), the host checks are host builds with mocks, and neither establishes physical access or frame behaviour, interrupts,
faults, floating-point flag state, timing, or gameplay. No byte match with the image is claimed.

## Unknown

- The meaning of most object fields (offsets named only by number in the sources), of the rank-derived numbers, and of the many
  image float constants.
- What the player-table byte at entry offset 23572 and the pointer words at 21960 and 21968 are called (they select two
  weapon records in the weapon spells); what "type" 21, 22 and 24 mean; what the twelve `LightEffect` floats mean.
- Which values the unused halves of several records hold; what the words at +112 (FireFlares) and +888 (WizardBeam) are for.
- Whether the three weapon spells' `Skill` calls with ids 0 to 3 are the same skill methods (the table entry is called through
  a pointer; its identity is not established here).
- Behaviour for a zero integer divisor (DiseaseTrail, FireFlares), and for the creature pointer being null in DiseaseTrail
  (the source reads the creature's position afterwards, as the image does; the host check does not exercise that branch, the
  paired run does).
