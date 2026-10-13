# w13-m-spell-dtor-a: spell and projectile destructors (part 1 of 2)

Branch `agents/w13-m-spell-dtor-a`. Sources are under `experimental/sources/w13-m-spell-dtor-a-*`, with the shared
header `w13-m-spell-dtor-a-common/dtor_common.hpp`. Each destructor is its own commit. Each source states what was
observed in the private image, what is inferred and what is unknown. No image bytes, disassembly or strings are
committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses below are that file's symbol values.

## Status

26 of the 27 assigned `_$_` destructors are authored and verified. `_$_15SpellConeOfFire` is not authored (see
"Blocked").

Each destructor has the form `_$_X(object, flags)`: it stores one or two vtable words, runs member and base
destructors with the flags the image passes, then calls the block free when bit 0 of `flags` is set. Member
releases are `listDestroy__FP8ListHead` on a field address, and embedded lights go through `_$_5Light` with flag 2.
Two classes (`_$_20VIParticleAttributes` and the Spell-derived pair) differ: the first only runtime-deletes,
and the second calls no base destructor.

## Imports closed (26)

- `_$_10MummySnake`
- `_$_10PoisonRain`
- `_$_10SpawnMummy`
- `_$_11BoltGoRound`
- `_$_11DiseaseBolt`
- `_$_12ShockOfFrost`
- `_$_14HateProjectile`
- `_$_14SpellHarmTouch`
- `_$_14poisonGasSpell`
- `_$_15FrostStormSpell`
- `_$_15ShockProjectile`
- `_$_16PoisonProjectile`
- `_$_16beetleProjectile`
- `_$_18SpellDiseaseShield`
- `_$_19ColdArrowProjectile`
- `_$_19FireArrowProjectile`
- `_$_19MultiFireProjectile`
- `_$_20VIParticleAttributes`
- `_$_21PoisonArrowProjectile`
- `_$_22ExplodeArrowProjectile`
- `_$_5Wheel`
- `_$_7AntWave`
- `_$_8HateBeam`
- `_$_8HateSoul`
- `_$_9DustDevil`
- `_$_9FireStorm`

Measured by linking the baseline partial object with these 26 objects (see "Checks performed"): the undefined set falls
from 1,168 to 1,142 names, exactly these 26.

## Imports not closed

- `_$_15SpellConeOfFire` (image 0x002b5a00, 216 bytes). Not authored. It calls `lumpFindResource__FPcT0` with the
  addresses of two image data labels (`BSpeed.1071+1008` and `edgingoffsetsy+15496`). The first is a local name
  absent from the symbol table, and the second is a local read-only symbol. Another package has to export these
  as global symbols, or the arguments must be supplied by the owner of that data, before the call can be written
  without a fixed address. The rest of the body (sound stop, light deactivate, copy of 12 bytes from `*(obj+12)+16`,
  `soundPlaySequence__FP15AnimationHeaderG6Point3f` with 1.0, light destructor with flag 2, Spell table at +20,
  list release at +4) is observed but not written, to keep the draft from carrying a guessed data argument.

## Imports this package references that stay unresolved

The baseline partial already defines all of these except `_vt$5Spell`, so linking my objects adds none of them to the
unresolved list.

- Destructors: `_$_10GameObject`, `_$_5Light`, `_$_8Creature`, `_$_13MissileWeapon`, `_$_14AnimationState`
- Helpers: `listDestroy__FP8ListHead`, `blockFree__FPv`, `lightDeactivate__FP5Light`, `P_DeleteParticleSystem__Fi`,
  `soundStopSound__Fi`, `__builtin_delete` (the runtime delete; a weak definition exists in the baseline)
- Vtables (defined in the baseline): `_vt$10MummySnake`, `_vt$10PoisonRain`, `_vt$10SpawnMummy`, `_vt$9DustDevil`,
  `_vt$11BoltGoRound`, `_vt$11DiseaseBolt`, `_vt$10Projectile`, `_vt$12ShockOfFrost`, `_vt$14HateProjectile`,
  `_vt$14poisonGasSpell`, `_vt$15FrostStormSpell`, `_vt$15ShockProjectile`, `_vt$16PoisonProjectile`,
  `_vt$16beetleProjectile`, `_vt$19ColdArrowProjectile`, `_vt$19FireArrowProjectile`, `_vt$21PoisonArrowProjectile`,
  `_vt$5Wheel`, `_vt$7AntWave`, `_vt$8HateBeam`, `_vt$8HateSoul`, `_vt$9FireStorm`, and the rest of the table names
  used by my units.
- `_vt$5Spell`: **still unresolved in the baseline and used by SpellHarmTouch and SpellDiseaseShield.** It is the one
  name these units need that no defined symbol in the baseline supplies. The owner of the Spell base should define
  it; this package does not.

## Build-plan entries needed

Not merged into the shared plan (the integrator merges). The entries are generated, not hand-written:

```sh
python3 docs/agents/w13-m-spell-dtor-a-plan-entries.py --toolchain /path/to/reviewed/ee
```

This prints 26 `independently_authored_source` entries, one per unit, in directory order. Each lists one emitted
function and the header closure (`w13-m-spell-dtor-a-common/dtor_common.hpp`). A private copy with the entries
appended can be built with `--private-root DIR`, outside git. Object names use the prefix `w13msd_`.

## Checks performed

- Toolchain: every unit was compiled with the reviewed EE `g++` and the `authored_cpp` profile flags. All 26 compile
  with no errors.
- Paired runs: every unit was run against the original `_$_X` from the private image in a private R5900 paired-run
  harness. Stub callees log their arguments in both runs. 36 scenarios per unit (12 flag values x zero, pattern and
  random object contents), comparing call logs, every changed byte of the object and the return value. Result: zero
  mismatches for all 26 units. Instruction coverage of the original was 10 to 41 words of 11 to 42 per unit (the smallest is VIParticleAttributes); the
  missing words are nops and delay-slot padding.
- Mutation checks: for MummySnake, four deliberate mutations (a list offset, a flag test, a base-flag value, a dropped
  vtable store) each produced mismatches. For AntWave, dropping the conditional release and changing the head offset
  each produced mismatches. The mutated files were restored before committing.
- Link check: the baseline partial (`game-components-partial.o`, shared plan, 5,528 inputs, 1,168 unresolved) was
  linked with my 26 objects using `ld -m elf32lr5900 -r --warn-common`. The result has 1,142 undefined names, and
  the removed set is exactly the 26 closed imports.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

The paired-run harness and scripts are private tooling outside git (`priv-w13-m-spell-dtor-a/`). They are not
committed, because they load the private executable.

## Authored but never executed

All 26 destructors were executed only in the private R5900 interpreter against the original image's code and stubs.
None ran on the EE, in the game, or in any linked executable. The partial build is relocatable; no executable was
produced.

## Unknown

- The meaning of most list heads and embedded light members, and whether they are the same type across classes.
- Whether `_$_20VIParticleAttributes` has other destructor-time work placed in a different routine.
- Whether the double list release in `_$_7AntWave` is deliberate or a compiler artifact of the original source.
- Whether `_vt$5Spell` is the Spell base's table or a shared table defined elsewhere.
- Whether the constructor and RTTI owners (`__tf`, `__ti`, `__<class>` constructors) agree with the layouts used here;
  those are other packages' symbols and were not checked.
- The data arguments of `_$_15SpellConeOfFire` (see above).
