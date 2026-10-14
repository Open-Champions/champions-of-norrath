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

## Checks performed

- **`__9SpellRootiP8CreatureP5Skill`.** Same shape as the cleric-root and entangle constructors, with the class
  vtable `_vt$9SpellRoot`. The EE compile (`work/toolchains/ee` GCC 15.2.0, `authored_cpp` flags, `-c`) defines the
  symbol; undefined references are `P_CreateParticleSystem__Fv`, `_vt$9SpellRoot` and `syncRandState`. The host
  contract (`host_check.cpp`, `clang++ -std=c++17`) prints `ok`. The paired R5900 comparison gives 0 mismatches
  over 210 scenarios (7 kinds x 5 adjustments x 6 seeds) with no HALT. A mutation that reads the kind byte
  unsigned gives 90 mismatches, so the comparison detects the defect it targets.

## Authored but never executed

Units were executed only by the private R5900 interpreter, against stub callees, not on the EE and not in the game.

## Unknown

- The class behind the skill's secondary table at +96 and the call target at +92 (shared with the cleric-root and
  entangle family).
