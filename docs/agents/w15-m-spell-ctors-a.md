# w15-m-spell-ctors-a: spell and projectile constructors and destructors

Branch `agents/w15-m-spell-ctors-a`. Each function is its own commit under
`experimental/sources/w15-m-spell-ctors-a-*`. Each source begins with a comment giving the observed address and
size, what the function does, what is inferred, and that nothing ran on the EE. No bytes, disassembly or image
data are committed.

Private reference executable: `work/private/SLUS_205.65` (ignored by git). Addresses and sizes are that file's
symbol values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the
`authored_cpp` flags; it is not a byte match with the image.

## Units

| source directory | symbol | image address and size | status |
|---|---|---|---|
| `w15-m-spell-ctors-a-ctor-9spellroot` | `__9SpellRootiP8CreatureP5Skill` | `0x002ad058`, 164 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-13firebomblet` | `_$_13FireBombletJr` | `0x003121b8`, 100 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-14firestormflare` | `_$_14FireStormFlare` | `0x003179c0`, 100 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-18hammerofwrath` | `_$_18SpellHammerOfWrath` | `0x00319768`, 116 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-dtor-19summonskeleton` | `_$_19SpellSummonSkeleton` | `0x003188d8`, 116 bytes | authored, paired-checked |
| `w15-m-spell-ctors-a-ctor-blinding-light` | `__18SpellBlindingLightiP8CreatureT2P5Skill` | `0x002ba028`, 184 bytes | authored, paired-checked |

## Checks performed

- **`__9SpellRootiP8CreatureP5Skill`.** Same shape as the cleric-root and entangle constructors, with the class
  vtable `_vt$9SpellRoot`. The EE compile (`work/toolchains/ee` GCC 15.2.0, `authored_cpp` flags, `-c`) defines the
  symbol; undefined references are `P_CreateParticleSystem__Fv`, `_vt$9SpellRoot` and `syncRandState`. The host
  contract (`host_check.cpp`, `clang++ -std=c++17`) prints `ok`. The paired R5900 comparison gives 0 mismatches
  over 210 scenarios (7 kinds x 5 adjustments x 6 seeds) with no HALT. A mutation that reads the kind byte
  unsigned gives 90 mismatches, so the comparison detects the defect it targets.

- **`_$_13FireBombletJr`.** Stores `_vt$13FireBombletJr` at +72, calls the GameObject base destructor
  `_$_10GameObject` with flags 0, then `blockFree__FPv` when bit 0 is set. The host contract (7 flag values) prints
  `ok`. The paired comparison (8 flag values x 3 fills) has 0 mismatches, with no HALT. Mutation (bit 1 tested
  instead of bit 0) gives 12 mismatches.

- **`_$_14FireStormFlare`.** The same shape as the fire-bomblet destructor, with `_vt$14FireStormFlare`. Host
  contract `ok` (7 flag values). Paired comparison 0 mismatches (8 flag values x 3 fills), no HALT. Mutation
  (bit 1 tested) gives 12 mismatches.

- **`_$_18SpellHammerOfWrath`.** Stores `_vt$18SpellHammerOfWrath` at +20, calls `listDestroy__FP8ListHead` on +24,
  stores `_vt$5Spell` at +20, calls it on +4, then frees when bit 0 is set. Host contract `ok` (7 flag values).
  Paired comparison 0 mismatches (8 flag values x 3 fills), no HALT. Mutation (bit 1 tested) gives 12 mismatches.

- **`_$_19SpellSummonSkeleton`.** The same shape as the hammer-of-wrath destructor, with `_vt$19SpellSummonSkeleton`.
  Host contract `ok` (7 flag values). Paired comparison 0 mismatches (8 flag values x 3 fills), no HALT. Mutation
  (bit 1 tested) gives 12 mismatches.

- **`__18SpellBlindingLightiP8CreatureT2P5Skill`.** Same Spell base and random step as the root constructor. It
  stores the base table at +20, then the class table `_vt$18SpellBlindingLight`. The skill call receives
  `S + adjustment`, the constant 2, and the full int kind. After the call it stores `frameNm + 60 * result` at +24
  and the first creature at +28; the second creature is at +12. Host contract `ok`. Paired comparison: 210
  scenarios, 0 mismatches, no HALT. The integer return register is excluded (`compare_ret=False`): the function is
  `void`, and the authored build leaves a different value in `v0`; logs and memory match in every scenario. Mutation
  (call argument 2 changed to 1) gives 210 mismatches.

## Authored but never executed

Units were executed only by the private R5900 interpreter, against stub callees, not on the EE and not in the game.

## Unknown

- Whether `__9SpellBash` and the other larger constructors share this family. `__9SpellBash` (`0x002a52c0`,
  384 bytes) is not a clean match: it looks up a resource, starts an animation, and walks a sorted creature list.
  It is not authored yet.

- The class behind the skill's secondary table at +96 and the call target at +92 (shared with the cleric-root and
  entangle family).
