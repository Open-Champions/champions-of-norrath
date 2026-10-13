# w13-skill-cast-a: skill cast and canCast methods (part 1 of 2)

Branch `agents/w13-skill-cast-a`. Every function below is its own commit. The shared accessor header
`experimental/sources/w13-skill-cast-a-common/skill_access.hpp` arrived with the healing cast commit and was extended in the
fire-storm commit (three-argument entry) and the entangle commit (entry on another object's table). Sources are under `experimental/sources/w13-skill-cast-a-*`; each source
starts with a comment stating what was observed in the private image, what is inferred and what is unknown. Nothing from the
image (bytes, disassembly, strings beyond short resource keys) is committed.

Private reference executable: `work/private/SLUS_205.65`. Addresses and sizes below are that file's symbol values
(`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the `authored_cpp` flags; it
differs from the image size because these are behaviour reconstructions, not byte matches. No byte-match claim is made.

## What was done

All 38 assigned imports are authored (38 plan inputs, 38 commits). The work continued an earlier interrupted run of this
same package: 16 commits and 4 uncommitted sources existed; the four uncommitted sources were re-checked (paired run plus
host check) and committed, and every earlier unit was re-run once more at the end (all paired and host checks pass).

- Leaf/no-op hooks: `cast__12SkillLifeTap`, `cast__12SkillRiposte` (return zero), `canCast__15SkillConeOfFire`,
  `canCast__15SkillHolyStrike`, `canCast__15SkillPoisonRain`, `canCast__16SkillDiseaseBolt` (return one).
- Type-info gated casts: `canCast__12SkillBlessed`, `canCast__15SkillFireFlares`, `canCast__16SkillColdWeapons`,
  `canCast__16SkillCriticalHit` (one when no spell of the matching class runs on the creature).
- Block-and-construct casts: `cast__12SkillCyclone`, `cast__14SkillHarmTouch`, `cast__15SkillConeOfFire`,
  `cast__15SkillWizardBeam`, `cast__16SkillColdWeapons`, `cast__16SkillCriticalHit`, `cast__15SkillFireFlares` (point passed on),
  `cast__12SkillHealing`, `cast__15SkillHolyShield`, `cast__15SkillShieldBash`, `cast__16SkillConeOfFrost` (rank first adjusted
  through the skill's own table entry at byte 120), `cast__11SkillCharge` (plus a creature state word).
- Projectile casts (return zero): `cast__10SkillShocki`, `cast__15SkillHolyStrike`, `cast__16SkillDiseaseBolt`,
  `cast__13SkillDarkness`, `cast__14SkillColdArrow`, `cast__14SkillFireArrow`.
- Placement casts using trig, the world height and ramp values: `cast__14SkillFireStorm`, `cast__15SkillPoisonRain`,
  `cast__15SkillFrostStorm`, `canCast__14SkillFireStorm`, `canCast__12SkillCyclone`, `canCast__15SkillShieldBash`.
- Multi-target and multi-projectile: `cast__13SkillEntangle`, `cast__15SkillClericRoot` (creature-list walk),
  `cast__12SkillBlessed` (eight creature halfwords then spell), `cast__14SkillMultiFire` (light effect, N missiles, damage range).

All callees use their real image symbol names. The accessor header models the class-table call (`word at object+96` is the
table; an entry is a signed halfword receiver adjustment, two padding bytes, then a function word). In host contracts the
stored words are offsets from a test arena so a test can log each call. The table entries used are named in the class tables
of the image: byte 88 `evaluateRampAtRank__5Skillii`, byte 104 `evaluateRampAtRank__5SkilliiP10GameObject`, byte 120
`tweekRank__5Skillii` (inferred from those tables, not from the cast bodies; the sources say so).

## Functions

| Symbol | Image address | Image bytes | Compiled bytes | Commit |
|---|---|---|---|---|
| `cast__10SkillShockiP8CreatureG6Point3s` | 0x0028d8b0 | 144 | 136 | 3cd8e6c3 |
| `cast__11SkillChargeiP8CreatureG6Point3s` | 0x00292038 | 160 | 112 | 859c687c |
| `cast__16SkillDiseaseBoltiP8CreatureG6Point3s` | 0x002937c0 | 164 | 148 | f56f3e0e |
| `canCast__16SkillDiseaseBoltP8Creature` | 0x002938a0 | 8 | 8 | f7a9bfa1 |
| `cast__13SkillDarknessiP8CreatureG6Point3s` | 0x00297658 | 312 | 212 | e8754876 |
| `canCast__12SkillCycloneP8Creature` | 0x0029a740 | 132 | 128 | 99c98798 |
| `cast__12SkillCycloneiP8CreatureG6Point3s` | 0x0029a820 | 132 | 84 | 366f2f72 |
| `cast__14SkillHarmTouchiP8CreatureG6Point3s` | 0x0029af78 | 132 | 84 | 66fe73e6 |
| `cast__14SkillFireStormiP8CreatureG6Point3s` | 0x0029c940 | 324 | 308 | cee8c1c5 |
| `canCast__14SkillFireStormP8Creature` | 0x0029cc70 | 260 | 268 | c5fc4e0b |
| `cast__14SkillFireArrowiP8CreatureG6Point3s` | 0x0029e368 | 204 | 192 | 793b1b8f |
| `cast__14SkillColdArrowiP8CreatureG6Point3s` | 0x0029fbf8 | 204 | 192 | 51a1721e |
| `cast__14SkillMultiFireiP8CreatureG6Point3s` | 0x002a0d58 | 1408 | 1392 | aca0e918 |
| `cast__15SkillFireFlaresiP8CreatureG6Point3s` | 0x002a48e0 | 136 | 108 | 7b5d9436 |
| `canCast__15SkillFireFlaresP8Creature` | 0x002a49a0 | 52 | 44 | 3ffa871c |
| `cast__12SkillRiposteiP8CreatureG6Point3s` | 0x002a50e0 | 40 | 8 | b675b2a7 |
| `cast__12SkillBlessediP8CreatureG6Point3s` | 0x002a6e70 | 204 | 196 | 65bafcd6 |
| `canCast__12SkillBlessedP8Creature` | 0x002a6fd8 | 52 | 44 | f8e35792 |
| `cast__16SkillColdWeaponsiP8CreatureG6Point3s` | 0x002ac3d8 | 132 | 84 | 1c25806f |
| `canCast__16SkillColdWeaponsP8Creature` | 0x002ac498 | 52 | 44 | 9cd2d26d |
| `canCast__16SkillCriticalHitP8Creature` | 0x002adb38 | 52 | 44 | d3a3dc09 |
| `cast__16SkillCriticalHitiP8CreatureG6Point3s` | 0x002adbf8 | 132 | 84 | 8e6716a5 |
| `cast__15SkillHolyShieldiP8CreatureG6Point3s` | 0x002ae4b8 | 160 | 120 | 7558bff4 |
| `cast__12SkillHealingiP8CreatureG6Point3s` | 0x002af290 | 160 | 120 | 8ab59d86 |
| `cast__16SkillConeOfFrostiP8CreatureG6Point3s` | 0x002afd28 | 160 | 120 | 34638143 |
| `cast__15SkillHolyStrikeiP8CreatureG6Point3s` | 0x002b2d50 | 192 | 176 | 9dee76de |
| `canCast__15SkillHolyStrikeP8Creature` | 0x002b2e48 | 8 | 8 | 8f740a4b |
| `cast__15SkillFrostStormiP8CreatureG6Point3s` | 0x002b3298 | 580 | 528 | cf7cdd91 |
| `cast__15SkillConeOfFireiP8CreatureG6Point3s` | 0x002b6a70 | 132 | 84 | 0f50ebc6 |
| `canCast__15SkillConeOfFireP8Creature` | 0x002b6b30 | 8 | 8 | 81dad1b8 |
| `cast__15SkillWizardBeamiP8CreatureG6Point3s` | 0x002b93f0 | 132 | 84 | 9914c849 |
| `cast__12SkillLifeTapiP8CreatureG6Point3s` | 0x002be218 | 40 | 8 | 1190531b |
| `cast__15SkillPoisonRainiP8CreatureG6Point3s` | 0x002bed80 | 324 | 308 | dec95424 |
| `canCast__15SkillPoisonRainP8Creature` | 0x002bf268 | 8 | 8 | 6c5568bf |
| `cast__15SkillClericRootiP8CreatureG6Point3s` | 0x002c57e8 | 460 | 500 | 2dff3953 |
| `cast__13SkillEntangleiP8CreatureG6Point3s` | 0x002c6460 | 460 | 500 | d6fe119e |
| `canCast__15SkillShieldBashP8Creature` | 0x002c8a40 | 120 | 136 | 7775b0b3 |
| `cast__15SkillShieldBashiP8CreatureG6Point3s` | 0x002c8ab8 | 160 | 120 | cfec9438 |

The by-value `Point3` copy that several originals make and never read is not re-created, which is why several compiled sizes are smaller.

## Observed, inferred, unknown

Each source carries its own comment; the shared picture is:

- Observed: allocation sizes and the constructor argument orders (every constructor argument is logged in the paired runs),
  every constant (including the float constants read from the small-data area: 373.0 in the darkness cast, 534.5 / 0.01 / 0.2 /
  130.09157 in multi-fire), the creature word and halfword offsets read or written (+12 bit 0x40, +56, +16/+20/+24, +72 table,
  +140 .. +154, +794, +960, +1400), the player record offsets (stride 23984; +212, +216, +21908, +21960, +23572, +23582) and the
  generator updates (`eRandState`, `syncRandState`, state * 0x19660d + 0x3c6ef35f).
- Inferred only: that a block-and-construct cast starts the spell of the matching class, that `canCast` for the type-info group
  means "not already running", that the byte-88/104/120 table entries are the ramp and rank-tweak functions, that bit 0x40 of
  creature+12 marks a player-controlled creature, and the intent of each placement (ring point, 36 units above the creature, ...).
- Unknown: what the creature halfwords at +140 .. +154 are (written by blessed, holy strike and disease bolt), what the player
  record words are (+21908 gates shield bash, byte +23582 gates cyclone), the meaning of the angle unit and of the numeric
  constants (180, 20, 10000, 1069, 16384, 0x40040000, ...), the layouts of skill and spell objects, and what the ramp indices select.

## Checks performed

- Each source compiles with the reviewed EE g++ (flags of `authored_cpp`, via the private `cc.sh`).
- Each source has a host contract (`host_check.cpp`, built natively with `clang++ -std=c++17 -ffp-contract=off`, not an EE input)
  that logs every callee call with its arguments and compares them with an independently written expectation, including
  allocation failure, the boundary and sign cases of each branch, and for float code exact bit patterns. All 38 pass.
- Each function was run against the original image function in the private R5900 interpreter harness (outside git, never
  committed), with identical stub callees and identical starting memory: logs of every callee call (including float arguments and
  stack arguments), return value and every changed byte outside the stack were compared. All 38 show zero mismatches and no halt.
  Scenarios per function: 2 to 27. Original-word coverage: complete except for nops (`cast__14SkillMultiFire` 350/352, the two
  missed words are nops). Hook-keyed raw table addresses stood in for the class table functions.
- Mutation checks (a changed constant, comparison or argument in a scratch copy of the source, outside git) were rejected by the
  paired run for charge (flag mask), darkness (the `+ 0.0` on x), entangle (seven mutants) and multi-fire (about twenty mutants;
  rounding constants needed extra scenarios, which were added), and by the host contracts of fire storm, poison rain and multi-fire.
  The other sources were not mutated; their paired runs log every constructor and table argument. Equivalent mutants noted: in
  multi-fire `0.0f < value` for `0.0f <= value` (zero rounds to zero either way) and `count >= 0` for `count > 0` (the loop test
  already skips zero).
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests). `tools/check_source_snapshot.py` was not run: it
  checks the shared snapshot, which this package does not edit.

Toolchain observation worth keeping: this g++ converts decimal float literals by truncation, not nearest. `0.2f` gives bits
0x3e4ccccc (the value the original loads) and `0.01f` gives 0x3c23d70a, but a 16-digit literal for 130.09157 gives bits
0x43021770 instead of the original 0x43021771; the short literal `130.09157f` is exact. The sources use the short forms.

## Imports closed (38)

`canCast__12SkillBlessedP8Creature`, `canCast__12SkillCycloneP8Creature`, `canCast__14SkillFireStormP8Creature`, `canCast__15SkillConeOfFireP8Creature`, `canCast__15SkillFireFlaresP8Creature`, `canCast__15SkillHolyStrikeP8Creature`, `canCast__15SkillPoisonRainP8Creature`, `canCast__15SkillShieldBashP8Creature`, `canCast__16SkillColdWeaponsP8Creature`, `canCast__16SkillCriticalHitP8Creature`, `canCast__16SkillDiseaseBoltP8Creature`, `cast__10SkillShockiP8CreatureG6Point3s`, `cast__11SkillChargeiP8CreatureG6Point3s`, `cast__12SkillBlessediP8CreatureG6Point3s`, `cast__12SkillCycloneiP8CreatureG6Point3s`, `cast__12SkillHealingiP8CreatureG6Point3s`, `cast__12SkillLifeTapiP8CreatureG6Point3s`, `cast__12SkillRiposteiP8CreatureG6Point3s`, `cast__13SkillDarknessiP8CreatureG6Point3s`, `cast__13SkillEntangleiP8CreatureG6Point3s`, `cast__14SkillColdArrowiP8CreatureG6Point3s`, `cast__14SkillFireArrowiP8CreatureG6Point3s`, `cast__14SkillFireStormiP8CreatureG6Point3s`, `cast__14SkillHarmTouchiP8CreatureG6Point3s`, `cast__14SkillMultiFireiP8CreatureG6Point3s`, `cast__15SkillClericRootiP8CreatureG6Point3s`, `cast__15SkillConeOfFireiP8CreatureG6Point3s`, `cast__15SkillFireFlaresiP8CreatureG6Point3s`, `cast__15SkillFrostStormiP8CreatureG6Point3s`, `cast__15SkillHolyShieldiP8CreatureG6Point3s`, `cast__15SkillHolyStrikeiP8CreatureG6Point3s`, `cast__15SkillPoisonRainiP8CreatureG6Point3s`, `cast__15SkillShieldBashiP8CreatureG6Point3s`, `cast__15SkillWizardBeamiP8CreatureG6Point3s`, `cast__16SkillColdWeaponsiP8CreatureG6Point3s`, `cast__16SkillConeOfFrostiP8CreatureG6Point3s`, `cast__16SkillCriticalHitiP8CreatureG6Point3s`, `cast__16SkillDiseaseBoltiP8CreatureG6Point3s`

## Imports opened

Short resource keys used as literals (`cyclone_warn`, `shieldbash_warn`, `fx.lmp`, `spel_ew_froststorm.anm`, `spel_we_firearrow.anm`)
are the only image strings quoted.

Image symbols whose definitions belong to other packages and that are already defined in the integrated `plan13` object
(no new import for the integrator): `_4item$globalItem`, `__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`, `__11DiseaseBoltsiG6Point3P10GameObjectP5Skill`, `__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description`, `__14SpellHarmTouchiP8CreatureP5Skill`, `__15FrostStormSpellG6Point3ifiP10GameObjectT1`, `__15ShockProjectileG6Point3sP10GameObjectP5Skilli`, `__15SpellConeOfFireiP8CreatureP5Skill`, `__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__9FireStormiP10GameObjectG6Point3P5Skill`, `addSpell__8CreatureP5Spell`, `blockAlloc__Fi`, `eRandState`, `hudSetInfoMessage__FPUsiic`, `icos__Fi`, `isBoss__FP10GameObject`, `isin__Fi`, `listDestroy__FP8ListHead`, `lumpFindResource__FPcT0`, `objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature`, `player`, `playerGetSkillRank__Fii`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `syncRandState`, `textGetString__FPc`, `world`, `worldFindHeight__FP12_worldHeaderG6Point3f`.

Already unresolved in the integrated baseline and now also used here: `checkIfSpellIsRunning__8CreatureRC9type_info`.

Newly opened (not defined or imported anywhere in the integrated baseline; real image symbols, definitions belong to the spell,
projectile and item packages): `__10HolyStrikesiG6Point3P10GameObjectP5Skill`, `__11SpellChargeiP8CreatureP5Skill`, `__12SpellBlessediP8CreatureP5Skill`, `__12SpellCycloneiP8CreatureP5Skill`, `__12SpellHealingiP8CreatureP5Skill`, `__13SpellEntangleiP8CreatureP5Skill`, `__15SpellClericRootiP8CreatureP5Skill`, `__15SpellFireFlaresiP8CreatureG6Point3P5Skill`, `__15SpellHolyShieldiP8CreatureP5Skill`, `__15SpellShieldBashiP8CreatureP5Skill`, `__15SpellWizardBeamiP8CreatureP5Skill`, `__16SpellColdWeaponsiP8CreatureP5Skill`, `__16SpellConeOfFrostiP8CreatureP5Skill`, `__16SpellCriticalHitiP8CreatureP5Skill`, `__18DarknessProjectileG6Point3T1P5SkilliP10GameObject`, `__tf12SpellBlessed`, `__tf15SpellFireFlares`, `__tf16SpellColdWeapons`, `__tf16SpellCriticalHit`, `getAttackBonus__4itemPQ24item11description`.

Note: `__tf12SpellBlessed`, `__tf15SpellFireFlares`, `__tf16SpellColdWeapons`, `__tf16SpellCriticalHit` are weak type-info
functions in the image (class-level data), so a spell package that defines those classes closes them.

## Build-plan entries needed

None of the 38 inputs is in `experimental/build-plan.json` (not edited by this package). Profile `authored_cpp`; the header
closure of the table-calling units is `sources/w13-skill-cast-a-common/skill_access.hpp` (a header-only file, no body). Host checks are
not plan inputs. Entries (sha256 values are of the committed source files at the last commit of this branch):

```json
[
  {"emitted_functions":[{"compiled_body_bytes":44,"name":"canCast__12SkillBlessedP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_blessed.o","profile":"authored_cpp","sha256":"12cc2cebe33b4b772cacdb468893f81a486feb5d6d04f8041b073c027363b75b","source":"sources/w13-skill-cast-a-can-cast-blessed/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":44,"name":"canCast__16SkillColdWeaponsP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_cold_weapons.o","profile":"authored_cpp","sha256":"d495a1e7da022a0fad69200c65ff65e330ed0e6f5d86d354588a9e4451025970","source":"sources/w13-skill-cast-a-can-cast-cold-weapons/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":8,"name":"canCast__15SkillConeOfFireP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_cone_of_fire.o","profile":"authored_cpp","sha256":"7f0d62c04954b123f74ad10eaff9810608a34b64d16661d7571bf683664b24ca","source":"sources/w13-skill-cast-a-can-cast-cone-of-fire/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":44,"name":"canCast__16SkillCriticalHitP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_critical_hit.o","profile":"authored_cpp","sha256":"230199bab94d7e648294b2b2ad5e7ec189780d45ed3c5a049ae90f2466f2f334","source":"sources/w13-skill-cast-a-can-cast-critical-hit/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":128,"name":"canCast__12SkillCycloneP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_cyclone.o","profile":"authored_cpp","sha256":"a539a0efc70e87e159b2b1d211800f80f0f9ddda6e0e8545046f98022aaa7e75","source":"sources/w13-skill-cast-a-can-cast-cyclone/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":8,"name":"canCast__16SkillDiseaseBoltP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_disease_bolt.o","profile":"authored_cpp","sha256":"93cd6bcc31f072d256ac5223597ca53cca995398e21918ad3d0e48575daaade8","source":"sources/w13-skill-cast-a-can-cast-disease-bolt/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":44,"name":"canCast__15SkillFireFlaresP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_fire_flares.o","profile":"authored_cpp","sha256":"381a7a070c0641c08ca95b2f21ef6f2046864eb71a64fecae791c8d9dc6c7285","source":"sources/w13-skill-cast-a-can-cast-fire-flares/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":268,"name":"canCast__14SkillFireStormP8Creature"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_can_cast_fire_storm.o","profile":"authored_cpp","sha256":"46b8af4ab6cb6bd438eb38feb823e18c532a092a766af6b3734fd185b460c40c","source":"sources/w13-skill-cast-a-can-cast-fire-storm/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":8,"name":"canCast__15SkillHolyStrikeP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_holy_strike.o","profile":"authored_cpp","sha256":"bb5b028c36ba8985ef3767917fb463ab0099ea588ffba703d8c728709387931d","source":"sources/w13-skill-cast-a-can-cast-holy-strike/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":8,"name":"canCast__15SkillPoisonRainP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_poison_rain.o","profile":"authored_cpp","sha256":"a948d5a377282980ea93415cb6c4604449b629b040675c9c9d98888cd07c4984","source":"sources/w13-skill-cast-a-can-cast-poison-rain/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":136,"name":"canCast__15SkillShieldBashP8Creature"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_can_cast_shield_bash.o","profile":"authored_cpp","sha256":"67ef0b48410d682e1e392ac020fec02a2da8bbf4e20a456921e57f0a290268e7","source":"sources/w13-skill-cast-a-can-cast-shield-bash/can_cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":196,"name":"cast__12SkillBlessediP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_blessed.o","profile":"authored_cpp","sha256":"82595c4d1051db195274d50cc114b96923ea0ca5467bc8a3634ff7f64da8ea13","source":"sources/w13-skill-cast-a-cast-blessed/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":112,"name":"cast__11SkillChargeiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_charge.o","profile":"authored_cpp","sha256":"dda58939d66e9060f7995b7d33416a968ea27d88a42d7ab0b560d4c024508e43","source":"sources/w13-skill-cast-a-cast-charge/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":500,"name":"cast__15SkillClericRootiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_cleric_root.o","profile":"authored_cpp","sha256":"b7f7f90e0e6c1d64905fed396aed5ea748975cce31b494497b1b802650899b22","source":"sources/w13-skill-cast-a-cast-cleric-root/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":192,"name":"cast__14SkillColdArrowiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_cold_arrow.o","profile":"authored_cpp","sha256":"432144fae4c98853609f907953990f44c50f327299b7f903f7e9f2015da95e35","source":"sources/w13-skill-cast-a-cast-cold-arrow/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":84,"name":"cast__16SkillColdWeaponsiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_cold_weapons.o","profile":"authored_cpp","sha256":"79be1f9971e0568e40f3db97a91efc1036694a0f255dc4054702d80c33e9bbd9","source":"sources/w13-skill-cast-a-cast-cold-weapons/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":84,"name":"cast__15SkillConeOfFireiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_cone_of_fire.o","profile":"authored_cpp","sha256":"9cef7d5b6d8b0736e9d8bc522e8c5b6993e649a7d8ba624e21245aa7ce98e7a0","source":"sources/w13-skill-cast-a-cast-cone-of-fire/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":120,"name":"cast__16SkillConeOfFrostiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_cone_of_frost.o","profile":"authored_cpp","sha256":"0b34f2ee002d63f9ddc8adf9c0c6c939e74c1bf140e9f48d041af89f8ae52a09","source":"sources/w13-skill-cast-a-cast-cone-of-frost/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":84,"name":"cast__16SkillCriticalHitiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_critical_hit.o","profile":"authored_cpp","sha256":"a33e426c117a5a66349c2075b22055fe941fc78ea70bab0e131c6e211edcf87f","source":"sources/w13-skill-cast-a-cast-critical-hit/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":84,"name":"cast__12SkillCycloneiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_cyclone.o","profile":"authored_cpp","sha256":"c617ca11dce04eea02fc22278f2597c9a4ae8dc8de9463c63c1cb2515afdac48","source":"sources/w13-skill-cast-a-cast-cyclone/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":212,"name":"cast__13SkillDarknessiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_darkness.o","profile":"authored_cpp","sha256":"b1dbb9d0548ec81145375a3b4355117a54d98fbcd9c6fb1a068181dabc7dee50","source":"sources/w13-skill-cast-a-cast-darkness/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":148,"name":"cast__16SkillDiseaseBoltiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_disease_bolt.o","profile":"authored_cpp","sha256":"a217adf10bf5ecf464975410cf7e810f75d793632bf22d14d92751004289cf57","source":"sources/w13-skill-cast-a-cast-disease-bolt/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":500,"name":"cast__13SkillEntangleiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_entangle.o","profile":"authored_cpp","sha256":"1d32e8b02244ba7a33a1fd254947ba3784556f001337a4953b3f753d36d94a2f","source":"sources/w13-skill-cast-a-cast-entangle/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":192,"name":"cast__14SkillFireArrowiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_fire_arrow.o","profile":"authored_cpp","sha256":"f818459cc1441a48af107984a61828899ec6c5200f6bc38d5b9494c0e716f3d3","source":"sources/w13-skill-cast-a-cast-fire-arrow/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":108,"name":"cast__15SkillFireFlaresiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_fire_flares.o","profile":"authored_cpp","sha256":"c469c33a9cdaf0cf9140fc61313279f05d4b30a64818e3c158f5a4d4b5ac88f4","source":"sources/w13-skill-cast-a-cast-fire-flares/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":308,"name":"cast__14SkillFireStormiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_fire_storm.o","profile":"authored_cpp","sha256":"a226eb970d9bcefcaba1cdb811273c687b644a19a46abb4145f24751a0c7ed01","source":"sources/w13-skill-cast-a-cast-fire-storm/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":528,"name":"cast__15SkillFrostStormiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_frost_storm.o","profile":"authored_cpp","sha256":"8be53478fb605df611788fb4c5fc55c3376c4746488953eeb1842a269cb84886","source":"sources/w13-skill-cast-a-cast-frost-storm/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":84,"name":"cast__14SkillHarmTouchiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_harm_touch.o","profile":"authored_cpp","sha256":"40495bd2f096f23da45d7b7cac8881fd3b574de9084aefef90835e0e086afd7c","source":"sources/w13-skill-cast-a-cast-harm-touch/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":120,"name":"cast__12SkillHealingiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_healing.o","profile":"authored_cpp","sha256":"0cb66858edac00aea5852e63ca20db540ae9b0ac9c0dce3c6c9c6a863afc8762","source":"sources/w13-skill-cast-a-cast-healing/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":120,"name":"cast__15SkillHolyShieldiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_holy_shield.o","profile":"authored_cpp","sha256":"793a77801e28f230dd71ec7f2e774cb2e27e7f2be5ad3a9a75d0923014ae2513","source":"sources/w13-skill-cast-a-cast-holy-shield/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":176,"name":"cast__15SkillHolyStrikeiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_holy_strike.o","profile":"authored_cpp","sha256":"4d4c247c18ef43227bc63a2be153431e0199337cbb6af2208404bb4cb4e9b9b2","source":"sources/w13-skill-cast-a-cast-holy-strike/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":8,"name":"cast__12SkillLifeTapiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_life_tap.o","profile":"authored_cpp","sha256":"3dd8b66d9bb73309e3902bf412ba5383a08c869c5a56846963b75ed31a4ceb99","source":"sources/w13-skill-cast-a-cast-life-tap/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":1392,"name":"cast__14SkillMultiFireiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_multi_fire.o","profile":"authored_cpp","sha256":"b9650a75b38f23d65eba0403c748be2a680e058aefda228b8573302ecbab4796","source":"sources/w13-skill-cast-a-cast-multi-fire/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":308,"name":"cast__15SkillPoisonRainiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_poison_rain.o","profile":"authored_cpp","sha256":"3ea6779316c31d023643b430c04e8de130f20cebdfa7235d809730e520fa137f","source":"sources/w13-skill-cast-a-cast-poison-rain/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":8,"name":"cast__12SkillRiposteiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_riposte.o","profile":"authored_cpp","sha256":"25e3a40b41ed8690b5a2b6c1fdd7a59a0af0a5bc12c03b703a8b86f281691d5c","source":"sources/w13-skill-cast-a-cast-riposte/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":120,"name":"cast__15SkillShieldBashiP8CreatureG6Point3s"}],"header_closure":["sources/w13-skill-cast-a-common/skill_access.hpp"],"kind":"independently_authored_source","object_name":"w13sca_cast_shield_bash.o","profile":"authored_cpp","sha256":"90dc147ac7e523f8f396b8a7df63b4c7e5a662ed203beb829c15c7114c74b49d","source":"sources/w13-skill-cast-a-cast-shield-bash/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":136,"name":"cast__10SkillShockiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_shocki.o","profile":"authored_cpp","sha256":"e558f7df529c11cd32346e8cf7fe4279cd2ead491142d513b827e3f481bb7eb8","source":"sources/w13-skill-cast-a-cast-shocki/cast.cpp"},
  {"emitted_functions":[{"compiled_body_bytes":84,"name":"cast__15SkillWizardBeamiP8CreatureG6Point3s"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w13sca_cast_wizard_beam.o","profile":"authored_cpp","sha256":"dba31c3d277c8e4660a739c4ab80f1c696d80793e1a92c1fc8464c356012c6b0","source":"sources/w13-skill-cast-a-cast-wizard-beam/cast.cpp"}
]
```

## Authored but never executed

Every function above was executed only (a) as a host contract with stub callees and (b) in a private R5900 interpreter next to
the original image function with stub callees. None has run in a linked game image, on hardware, or with real callees (spell
and projectile constructors, the world height lookup, the player table contents, the creature list). The float-heavy ones
(`cast__14SkillFireStorm`, `cast__15SkillPoisonRain`, `cast__15SkillFrostStorm`, `canCast__14SkillFireStorm`,
`cast__13SkillDarkness`, `cast__14SkillMultiFire`) rely on the interpreter's single-float rounding model matching the console. The
global-item, player-table and generator-state objects are imports here; their storage owners are other packages.

## Unknown / not done

Nothing assigned was skipped. Open questions that need an owner or runtime evidence: the layouts behind the creature
and player-record offsets listed above, what the ramp indices 0..3 mean per class, the angle unit, why `cast__14SkillMultiFire`
passes the original rank to ramp 0 but the tweaked rank to ramps 1 and 2, and why the player/non-player branches of
multi-fire combine damage differently. No names were guessed for any of these.
