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
  the result as discarded, which failed the paired comparison (200 mismatches on a mutation check, 120 on the
  first pass); the source now multiplies by 23984 as the image does. Host contract passes (blocks absent and present).
  Paired comparison: 288 scenarios (4 kinds, block present or absent, 6 indexes including two that wrap the 32-bit
  product, 2 seeds, 3 call results) with 0 mismatches. Mutation (23985 for 23984) gives 200 of 288.

## Imports closed

`__15SpellClericRootiP8CreatureP5Skill`, `__13SpellEntangleiP8CreatureP5Skill`, `_$_5Spell`,
`__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill`, `__16SpellConeOfFrostiP8CreatureP5Skill`,
`__16SpellCriticalHitiP8CreatureP5Skill`, `__17SpellMinorHealingiP8CreatureP5Skill`.

## Imports opened

To record once the build is complete.

## Build-plan entries needed

Not yet; these go in the list below once the entry is final.

## Unknown

- The class name behind the skill's table at +96 and the call target at +92.
- The name of `P_CreateParticleSystem__Fv`'s owner package (not defined in this package).
