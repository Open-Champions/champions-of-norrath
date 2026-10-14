# w14-m-spell-ctors-a: spell constructors, type-info and vtables (part a)

Branch `agents/w14-m-spell-ctors-a`. Each function is its own commit under
`experimental/sources/w14-m-spell-ctors-a-*`. Each source begins with a comment giving the observed address and
size, what the function does, what is inferred, and that nothing ran on the EE. No bytes, disassembly or image
data are committed.

Private reference executable: `work/private/SLUS_205.65` (ignored by git). Addresses and sizes are that file's
symbol values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the
`authored_cpp` flags; it is not a byte match with the image.

## Units

| source directory | symbol | image address and size | compiled bytes |
|---|---|---|---|
| `w14-m-spell-ctors-a-ctor-cleric-root` | `__15SpellClericRootiP8CreatureP5Skill` | `0x002c6320`, 164 bytes | 172 |
| `w14-m-spell-ctors-a-ctor-entangle` | `__13SpellEntangleiP8CreatureP5Skill` | `0x002c6f98`, 164 bytes | 172 |
| `w14-m-spell-ctors-a-dtor-5spell` | `_$_5Spell` | `0x00314990`, 100 bytes | 92 |
| `w14-m-spell-ctors-a-ctor-hate-effect` | `__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill` | `0x001ff6f8`, 264 bytes | 260 |
| `w14-m-spell-ctors-a-ctor-cone-of-frost` | `__16SpellConeOfFrostiP8CreatureP5Skill` | `0x002af390`, 376 bytes | 328 |
| `w14-m-spell-ctors-a-ctor-critical-hit` | `__16SpellCriticalHitiP8CreatureP5Skill` | `0x002ad3f8`, 304 bytes | 288 |
| `w14-m-spell-ctors-a-ctor-minor-healing` | `__17SpellMinorHealingiP8CreatureP5Skill` | `0x002a7068`, 340 bytes | 352 |
| `w14-m-spell-ctors-a-ctor-healing` | `__12SpellHealingiP8CreatureP5Skill` | `0x002ae9d8`, 352 bytes | 372 |
| `w14-m-spell-ctors-a-ctor-holy-shield` | `__15SpellHolyShieldiP8CreatureP5Skill` | `0x002adf40`, 288 bytes | 316 |

## Observed shape shared by the spell constructors

Observed in `__15SpellClericRoot` and `__13SpellEntangle`. The two instruction streams differ only in the vtable address
and in a trailing nop (checked by diffing the disassembly):

- byte K at +0; word 0 at +4; skill S at +8; creature C at +12;
- the sync random state (`syncRandState`) is stepped once, `state * 0x19660d + 0x3c6ef35f`; its high half is at +16;
- the class vtable at +20;
- zero at +28; `P_CreateParticleSystem__Fv()` result at +36;
- a call through the skill: the table pointer at S+96, a halfword adjustment at table+88 and a function address at
  table+92; receiver `S + adjustment`, arguments `(receiver, 1, (signed char)K)`; the int result at +32.

Inferred: the same spell family. Unknown: the meanings of the table at S+96 and of the call target.

## Checks performed

- **EE compile.** The source compiles with `work/toolchains/ee` (GCC 15.2.0), `authored_cpp` flags, `-c`. The symbol
  `__15SpellClericRootiP8CreatureP5Skill` is defined; undefined references are `P_CreateParticleSystem__Fv`,
  `_vt$15SpellClericRoot` and `syncRandState`.
- **Host contract.** `host_check.cpp`, built with `clang++ -std=c++17`, checks the stored fields, the random step,
  the particle handle, call order, the adjusted receiver, arguments and the result for three kinds. It passes. A
  deliberate mutation (unsigned kind byte) makes it fail.
- **Paired R5900 comparison.** Original and authored function run from identical memory snapshots, with stub
  callees for the particle system and the skill target. 210 scenarios (7 kinds x 5 adjustments x 6 seeds) with
  0 mismatches and no HALT. Three mutations (unsigned adjustment, unsigned kind, shift by 15 in the random high
  half) produce 84, 90 and 210 mismatches.

- **Entangle.** The same host contract and paired comparison (210 scenarios, 0 mismatches, no HALT) for
  `__13SpellEntangleiP8CreatureP5Skill`. The host contract also passes.

- **Base destructor.** `_$_5Spell` (`0x00314990`): stores `_vt$5Spell` at +20, calls `listDestroy__FP8ListHead` on
  +4, and calls `blockFree__FPv` only when bit 0 of the flags is set. Host contract: 7 flag values, passes. Paired
  comparison: 24 scenarios (8 flag values x 3 fills), 0 mismatches. Mutation (test bit 1 instead of bit 0) gives 12
  mismatches.

- **Hate effect.** `__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill` (`0x001ff6f8`). Same Spell base, with two
  embedded list heads at +24 and +28. It links the GameObject into the head at +24 when the GameObject is not null,
  and the creature into the head at +28 when the creature is not null. It stores `frameNm + 60 * first` at +32. The
  host contract (four cases, exact link arguments) passes. The paired comparison has 384 scenarios (4 first values,
  4 kinds, creature and GameObject each present or absent, 3 frame values, 2 seeds) with 0 mismatches. Only the
  integer return register is excluded from the comparison: the function is `void`, and the authored build leaves a
  different value in `v0`.

- **Cone of frost.** `__16SpellConeOfFrostiP8CreatureP5Skill` (`0x002af390`). The Spell base, a light at +24 (the
  same shape as the cone-of-fire unit, with other values), a flash point at +72 built from the creature position
  (x + 0.0, y unchanged, z + 60.0), then `lumpFindResource("fx.lmp", "spel_ew_cfrost.anm")` and
  `soundPlaySequence` with a copy of the creature position and volume 1.0. The handle is stored at +84. The light's
  float constant at `0x00482c8c` is an unnamed global; it is written here as its bit pattern (`0x382c598a`). Host
  contract passes. Paired comparison: 72 scenarios (6 kinds, 3 positions including signed zeros, 4 seeds), 0
  mismatches. Two mutations (drop the `+ 0.0f` on x, and 155.0 for the first light float) give 24 and 72
  mismatches.

- **Critical hit.** `__16SpellCriticalHitiP8CreatureP5Skill` (`0x002ad3f8`). The Spell base, a copy of the creature
  position to +76, a light at +28 (it ends where the copy begins), then the skill's secondary call with level 2, the
  int result converted to float at +24, and the particle system at +88. The light's float constant at
  `0x00482c20` is an unnamed global, written as its bit pattern (`0x3891a2b3`). Host contract passes. Paired
  comparison: 120 scenarios (6 kinds, 5 adjustments, 4 seeds), 0 mismatches. Mutation (call level 1 instead of 2)
  gives 120 of 120 mismatches.

- **Minor healing.** `__17SpellMinorHealingiP8CreatureP5Skill` (`0x002a7068`). The Spell base; then
  `lumpFindResource("fx.lmp", "spel_he_heal.anm")` and `soundPlaySequence` with the creature pointer and volume 1.0;
  `blockAlloc(92)` and, when it is non-null, `__12GlowingShaftG6Point3iiii` with a copy of the creature position and
  90, 250, 200, 100; then the skill's secondary call with level 0. The float at +32 is
  `(player[0xb0 + w] * K_a) * (result * K_b)`, where `w` is the creature word at +960 times 23984. The constants are
  unnamed and written as bit patterns (`0x3c888888`, `0x3c23d70a`). The table is read at `0x004086b0 + w`, which
  lies in `player` at offset 0xb0; this offset is observed, the table's own name is unknown.
  **Finding:** the three-operand R5900 `mult rd,rs,rt` also writes its low word into `rd`. My first reading treated
  the result as discarded, which failed the paired comparison (120 of 192 scenarios mismatched); the source now
  multiplies by 23984 as the image does. Host contract passes (blocks absent and present).
  Paired comparison: 288 scenarios (4 kinds, block present or absent, 6 indexes including two that wrap the 32-bit
  product, 2 seeds, 3 call results) with 0 mismatches. Mutation (23985 for 23984) gives 200 of 288.

- **Healing.** `__12SpellHealingiP8CreatureP5Skill` (`0x002ae9d8`). The same shape as minor healing, with a particle
  system created after the optional shaft and stored at +36, the skill call at level 0, and the constants
  `0x3c886600` (K_a, `0x00482c60`) and `0x3c23d70a` (K_b, `0x00482c64`). Host contract passes. Paired comparison: 288
  scenarios, 0 mismatches. Mutation (particle handle at +40 instead of +36) gives 288 of 288 mismatches.

- **Holy shield.** `__15SpellHolyShieldiP8CreatureP5Skill` (`0x002adf40`). The minor-healing shape without the table
  scaling: the skill call at level 1 stores its int result, converted to float, at +32; +24 and +28 are zero; the
  shaft gets 90, 86, 126, 250; the particle system is stored at +36. Host contract passes. Paired comparison: 192
  scenarios, 0 mismatches. Mutation (shaft arguments 126 and 250 swapped) gives 96 of 192 mismatches.

## Imports closed

Nine of the 22 assigned names are closed by the units above (each one is defined by its own object):

`__15SpellClericRootiP8CreatureP5Skill`, `__13SpellEntangleiP8CreatureP5Skill`, `_$_5Spell`,
`__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill`, `__16SpellConeOfFrostiP8CreatureP5Skill`,
`__16SpellCriticalHitiP8CreatureP5Skill`, `__17SpellMinorHealingiP8CreatureP5Skill`,
`__12SpellHealingiP8CreatureP5Skill`, `__15SpellHolyShieldiP8CreatureP5Skill`.

## Imports still open in this package

Thirteen assigned names have no unit yet:

`_$_13FireBombletJr`, `__11SpellChargeiP8CreatureP5Skill`, `__12SpellBlessediP8CreatureP5Skill`,
`__12SpellCycloneiP8CreatureP5Skill`, `__15SpellFireFlaresiP8CreatureG6Point3P5Skill`,
`__15SpellShieldBashiP8CreatureP5Skill`, `__15SpellWizardBeamiP8CreatureP5Skill`,
`__16SpellColdWeaponsiP8CreatureP5Skill`, `__16SpellFireWeaponsiP8CreatureP5Skill`,
`__17SpellConvertEnemyiP8CreatureP5Skill`, `__17SpellDiseaseTrailG6Point3iP8CreatureP5Skill`,
`__17SpellUndeadShieldiP8CreatureP5Skill`, `__18SpellAncestralCalliP8CreatureP5Skill`.

Status of the ones I looked at, none of them closed:

- `__15SpellShieldBashiP8CreatureP5Skill` (`0x002c7f38`, 384 bytes) reads the creature list through
  `objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature`, walks linked records with flag tests (`0x00100000`),
  creates an animation one-shot (`animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`), and ends with
  `listDestroy__FP8ListHead`. The walk and the list-head layout are not yet established.
- `__17SpellUndeadShieldiP8CreatureP5Skill` (`0x002affa8`, 440 bytes) uses `div.s`, a light, the skill call at level 1,
  an arithmetic read of the global `frameNm` (times 60 plus its value), a particle system, a lump and sound, and
  `soundMakePersistant__Fi`. It also copies the creature point to +80. Not attempted.
- `__12SpellBlessed`, `__12SpellCyclone`, `__11SpellCharge`, `__15SpellWizardBeam`, `__16SpellFireWeapons` and
  `__16SpellColdWeapons` are 160 to 290 instructions with several calls and float paths each. Not attempted.
- `_$_13FireBombletJr` (the destructor) and `__17SpellConvertEnemy`, `__17SpellDiseaseTrail`, `__15SpellFireFlares`,
  `__18SpellAncestralCall` were not decoded.

## Imports opened

Referenced by the units above, and not defined by any source in the shared plan as of this branch (checked by
searching the plan's sources for each name). These belong to other packages or to a later vtable/data pass:

- Vtables of the classes whose constructors this package authors: `_vt$12SpellHealing`, `_vt$13SpellEntangle`,
  `_vt$15SpellClericRoot`, `_vt$15SpellHateEffect`, `_vt$15SpellHolyShield`, `_vt$16SpellConeOfFrost`,
  `_vt$16SpellCriticalHit`, `_vt$17SpellMinorHealing`. Each constructor stores the address of its own vtable and
  references it by this symbol, so the vtable objects are a dependency of these units.
- `__12GlowingShaftG6Point3iiii` (a constructor of the glowing shaft; referenced by minor healing, healing and holy
  shield). Only a comment in another package mentions it.

Defined elsewhere and used as-is: `P_CreateParticleSystem__Fv` (`w11-data-a-create-particle-system`),
`blockAlloc__Fi`, `blockFree__FPv`, `lightAdd__FP5Light`, `listDestroy__FP8ListHead`, `listOnewayLink__FP8ListHeadT0Pv`,
`lumpFindResource__FPcT0`, `soundPlaySequence__FP15AnimationHeaderG6Point3f`,
`soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `frameNm`, `player`, `syncRandState`, and `_vt$5Spell`
(`w13-m-startup-data-b-vt-spell`). The check is a source search of the plan; the official unresolved list was not
read (see Checks).

## Build-plan entries needed

These entries are for the lead integrator's successor plan. They are **not** in `experimental/build-plan.json`, and
each needs an `input_order` from the integrator. Every source has an empty header closure (no `#include`), the
`authored_cpp` profile, and a SHA-256 of the committed file. `emitted_functions` lists each symbol with the compiled
size from the reviewed EE g++ run. `host_check.cpp` files are not plan inputs.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_cleric_root.o",
  "profile": "authored_cpp",
  "sha256": "4f54f41dc9b3c9772934ce5e3725f7e4d2e7713804ea0d87381d2e37532eab16",
  "source": "sources/w14-m-spell-ctors-a-ctor-cleric-root/spell_cleric_root_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__15SpellClericRootiP8CreatureP5Skill",
    "compiled_body_bytes": 172
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_entangle.o",
  "profile": "authored_cpp",
  "sha256": "e17e1a26904e83b8a40b025a6b1f1bdaee004d0d4969d867da47e2b84d2a1ce4",
  "source": "sources/w14-m-spell-ctors-a-ctor-entangle/spell_entangle_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__13SpellEntangleiP8CreatureP5Skill",
    "compiled_body_bytes": 172
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_dtor_5spell.o",
  "profile": "authored_cpp",
  "sha256": "ab41acb46e677aacbc373f5e52d7dca8169b9ef5128d25a3d937f5721c4a658f",
  "source": "sources/w14-m-spell-ctors-a-dtor-5spell/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_5Spell",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_hate_effect.o",
  "profile": "authored_cpp",
  "sha256": "291057126d1aab65fe72b5323f77553c56bfda29317c62de0a5c9f4dec986f05",
  "source": "sources/w14-m-spell-ctors-a-ctor-hate-effect/spell_hate_effect_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill",
    "compiled_body_bytes": 260
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_cone_of_frost.o",
  "profile": "authored_cpp",
  "sha256": "615bd3af70df9899f2add1b92e57a67d75deab22905756ccb6a3b02589b5b6f9",
  "source": "sources/w14-m-spell-ctors-a-ctor-cone-of-frost/spell_cone_of_frost_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__16SpellConeOfFrostiP8CreatureP5Skill",
    "compiled_body_bytes": 328
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_critical_hit.o",
  "profile": "authored_cpp",
  "sha256": "400e8fe449ec15abd5bab70200a49fcb5015ac41f551b8a19fd7d08b6944edc9",
  "source": "sources/w14-m-spell-ctors-a-ctor-critical-hit/spell_critical_hit_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__16SpellCriticalHitiP8CreatureP5Skill",
    "compiled_body_bytes": 288
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_minor_healing.o",
  "profile": "authored_cpp",
  "sha256": "0e28ed334e14ec0cbff80760a2dbf4c854f6c34239b861ba7cbc316a7051169d",
  "source": "sources/w14-m-spell-ctors-a-ctor-minor-healing/spell_minor_healing_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__17SpellMinorHealingiP8CreatureP5Skill",
    "compiled_body_bytes": 360
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_healing.o",
  "profile": "authored_cpp",
  "sha256": "f722dc62d2e07abd0638d2f0713e473e63ed6655701661419f7134d8c1d1ecb9",
  "source": "sources/w14-m-spell-ctors-a-ctor-healing/spell_healing_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__12SpellHealingiP8CreatureP5Skill",
    "compiled_body_bytes": 372
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mspell_ctor_holy_shield.o",
  "profile": "authored_cpp",
  "sha256": "7ba49cc291693cfe1e68c03b4acfffdcb2340a9bc71f2d1566e10a23feeae926",
  "source": "sources/w14-m-spell-ctors-a-ctor-holy-shield/spell_holy_shield_ctor.cpp",
  "emitted_functions": [
   {
    "name": "__15SpellHolyShieldiP8CreatureP5Skill",
    "compiled_body_bytes": 316
   }
  ]
 }
]
```

## Checks performed

- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK (run on this branch).
- **Partial build.** `tools/build_partial.py` with the reviewed EE toolchain was started on the unmodified shared plan
  into `work/w14-m-spell-ctors-a-base` (ignored by git). The first run was killed at its background time limit; the
  second run was still compiling when this note was written (2,241 of about 6,237 inputs), so no
  `unresolved-imports.json` exists yet. The closure statements above come from a source search of the plan, not from
  that list. Once the build finishes, check that the nine closed names are absent and the opened names appear.
- **Compile.** All nine units compile with the reviewed toolchain and `authored_cpp` flags. Unit sizes differ from the
  image sizes (table above); that is expected and not a byte match.
- **Host contracts.** Each unit's `host_check.cpp` builds with `clang++ -std=c++17 -Wall -Wextra` and prints `ok`.
  One mutation (unsigned kind byte in cleric root) was checked against its host contract and caught.
- **Paired R5900 comparison.** A private interpreter (kept outside git) ran the original and each authored function
  from identical memory snapshots, with stub callees. Scenario counts and mutation results are in the unit notes
  above. Every final comparison was re-run for this note: all nine report 0 mismatches and no HALT line.

## Authored but never executed

All nine units were executed only by the private R5900 interpreter, against stub callees, not on the EE and not in
the game. The sync random state, the image vtable addresses, the `player` table lookup (`0x004086b0` plus the
scaled creature word), and the skill's secondary table are read the way the image reads them, but they were never run
with real game objects.

## Unknown

- The class behind the skill's secondary table at +96 and the call target at +92. Every constructor here calls through
  it, so it is shared with the rest of the family.
- The name of the table at `0x004086b0` (it sits inside the `player` symbol at offset 0xb0; the owner is unknown) and
  the creature field at +960 that indexes it.
- The meaning of the float constants at `0x00482aec`, `0x00482af0`, `0x00482c20`, `0x00482c60`, `0x00482c64` and
  `0x00482c8c`. They are unnamed globals and are written as bit patterns.
- The meaning of the stored fields (light records, shaft points, the list heads at +24 and +28 in hate effect, the
  value at +32 in cone and hate effect), beyond the observed stores.
- The owners of `P_CreateParticleSystem__Fv` and `__12GlowingShaftG6Point3iiii` beyond the source search.
