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

## Imports closed

`__15SpellClericRootiP8CreatureP5Skill`, `__13SpellEntangleiP8CreatureP5Skill`, `_$_5Spell`.

## Imports opened

To record once the build is complete.

## Build-plan entries needed

Not yet; these go in the list below once the entry is final.

## Unknown

- The class name behind the skill's table at +96 and the call target at +92.
- The name of `P_CreateParticleSystem__Fv`'s owner package (not defined in this package).
