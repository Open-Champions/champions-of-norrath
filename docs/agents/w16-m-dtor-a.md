# w16-m-dtor-a: spell and projectile destructors

Branch `agents/w16-m-dtor-a`. Each function is its own commit under `experimental/sources/w16-m-dtor-a-dtor-<class>/`.
Each source starts with a comment giving the observed symbol address and size, what the function does, what is
inferred, and that nothing ran on the EE. No bytes, disassembly or image data are committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Addresses and sizes are that file's symbol values (`nm -S`). "Compiled bytes" is the size of the function compiled with
the reviewed EE g++ (GCC 15.2.0) and the `authored_cpp` flags. It is not a byte match with the image.

## Scope

The seventeen names in the wave list are destructors of spell and projectile classes. Each name is the
`_$_<class>` symbol. Their callers are reached through vtables: the image has no direct call sites to any of
these seventeen addresses. The call-count ordering was therefore not usable, and the work is ordered by shared shape
(GameObject base, Spell base, projectile, multi-step, global counter).

Status: 6 of 17 units have sources and checks in this branch.

## What was done

Each destructor stores the class table word(s) at the offsets seen in the image, calls the same callees in the same
order, and then, when bit 0 of the incoming flags is set, calls `blockFree__FPv` on the object (a tail call in the image).
Other flag bits do not free. The flags are read once, before the first call, as the image does.

- GameObject: 12GlowingShaft, 18FrostStormIceShard, 19BlindingLightEffect
- Spell base: 12SpellHealing, 15SpellHolyShield, 17SpellMinorHealing

## Units

| source directory | symbol | image address and size | compiled bytes | source sha256 |
|---|---|---|---|---|
| `w16-m-dtor-a-dtor-12glowingshaft` | `_$_12GlowingShaft` | `0x00310720`, 100 bytes (weak) | 88 | `60e9f2a1ff1aa79d6f9e7384185f23778a2494f36aa6fb0fb2864e26c8ed7170` |
| `w16-m-dtor-a-dtor-18froststormiceshard` | `_$_18FrostStormIceShard` | `0x00313d20`, 100 bytes (weak) | 88 | `cf92d4b1cf36a15381214dadb7e3759062fb6ea4d8da6879af06950f8f5dbdab` |
| `w16-m-dtor-a-dtor-19blindinglighteffect` | `_$_19BlindingLightEffect` | `0x00319c68`, 100 bytes (weak) | 88 | `a5e318ac792d5ca0c3912985dbd1df183cc81618f10c4a4480d19542da43b63d` |
| `w16-m-dtor-a-dtor-12spellhealing` | `_$_12SpellHealing` | `0x00319208`, 100 bytes (weak) | 92 | `db6cf1c8f5b9432d28fb641778c913c8a56018ad732a9aba210b72dd35836ea1` |
| `w16-m-dtor-a-dtor-15spellholyshield` | `_$_15SpellHolyShield` | `0x00319068`, 100 bytes (weak) | 92 | `1aa97b1e82e523232bca03c1c4b3d615556288a8fc53fbab44824d60b3b3968c` |
| `w16-m-dtor-a-dtor-17spellminorhealing` | `_$_17SpellMinorHealing` | `0x003186b0`, 100 bytes (weak) | 92 | `830c02726b6503281b35bb05b8e86de0523243e9cfbfbf6e9642544b46ab6a80` |

## Observed behaviour worth knowing

- Table words are stored before and between callee calls. A callee can observe the table word at call time, so the
  paired comparison logs that word at each call.
- Classes with a base table store the derived table, make some calls, then store the base table (`_vt$5Spell` or
  `_vt$10Projectile`) before the remaining calls. The base-table store is at the same offset as the derived one.
- `_$_13HammerOfWrath` reads the byte `wrathHammerCount` once, stores its value minus one when bit 7 of that
  difference is clear and zero otherwise, and does this after the derived-table store. The counter is a gp-relative
  global in the image. Its name is the image symbol name; the other package that defines it is listed under imports.
- `_$_16SpellConeOfFrost` calls `soundStopSound__Fi` with the word at object+84 before the light destructor.
- `_$_13SpellEntangle` and `_$_15SpellClericRoot` call `P_DeleteParticleSystem__Fi` with the word at object+36.

## Checks performed

- **EE compile.** 6 of 6 sources compile with `/Users/case/norrath-agents/repo/work/toolchains/ee` (GCC 15.2.0)
  and the `authored_cpp` flags from `experimental/build-plan.json`, with `-c` into a private directory. Each object exports
  one function, under its image symbol, and leaves the callees and tables undefined.
- **Host contract.** Each unit has `host_check.cpp`, built with `clang++ -std=c++17`. It runs the destructor over every
  flag value in the image-observed set, and for `_$_13HammerOfWrath` over eight counter values, and compares the call
  trace, including the table word at each call, and the final counter. All pass.
- **Paired comparison.** A private R5900 interpreter (kept outside git) runs the original function from the image and the
  authored function from identical memory snapshots, with the same Python callee hooks, and compares call logs and
  changed memory. Scenarios: nine flag values times four fill patterns (36 per unit). For `_$_13HammerOfWrath`
  the scenarios are nine flag values by four fills by eight counter values (288). Result: 0 mismatches for every
  unit, and no HALT reported.
- **Mutation checks.** Deliberate mutants were run through the same comparison and caught: `_$_8LostSoul` with the
  light call flag changed (36 mismatches) and with the free check removed (16); `_$_8HolyBolt` with the list offset
  changed (36); `_$_13HammerOfWrath` with the saturating branch changed (8 mismatches across the counter sweep).
- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`, run at the end of this package.

## Build-plan entries needed

These entries are for the lead integrator's successor plan. They are not in `experimental/build-plan.json`, and
`experimental/snapshot.json` is not changed here. Each needs an `input_order` from the integrator. The header closure is
empty and the profile is `authored_cpp`. `tools/build_partial.py --check-plan` will reject a plan that adds these
entries until the snapshot records the same inputs.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_12glowingshaft.o",
  "profile": "authored_cpp",
  "sha256": "60e9f2a1ff1aa79d6f9e7384185f23778a2494f36aa6fb0fb2864e26c8ed7170",
  "source": "sources/w16-m-dtor-a-dtor-12glowingshaft/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_12GlowingShaft",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_18froststormiceshard.o",
  "profile": "authored_cpp",
  "sha256": "cf92d4b1cf36a15381214dadb7e3759062fb6ea4d8da6879af06950f8f5dbdab",
  "source": "sources/w16-m-dtor-a-dtor-18froststormiceshard/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_18FrostStormIceShard",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_19blindinglighteffect.o",
  "profile": "authored_cpp",
  "sha256": "a5e318ac792d5ca0c3912985dbd1df183cc81618f10c4a4480d19542da43b63d",
  "source": "sources/w16-m-dtor-a-dtor-19blindinglighteffect/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_19BlindingLightEffect",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_12spellhealing.o",
  "profile": "authored_cpp",
  "sha256": "db6cf1c8f5b9432d28fb641778c913c8a56018ad732a9aba210b72dd35836ea1",
  "source": "sources/w16-m-dtor-a-dtor-12spellhealing/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_12SpellHealing",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_15spellholyshield.o",
  "profile": "authored_cpp",
  "sha256": "1aa97b1e82e523232bca03c1c4b3d615556288a8fc53fbab44824d60b3b3968c",
  "source": "sources/w16-m-dtor-a-dtor-15spellholyshield/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_15SpellHolyShield",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_17spellminorhealing.o",
  "profile": "authored_cpp",
  "sha256": "830c02726b6503281b35bb05b8e86de0523243e9cfbfbf6e9642544b46ab6a80",
  "source": "sources/w16-m-dtor-a-dtor-17spellminorhealing/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_17SpellMinorHealing",
    "compiled_body_bytes": 92
   }
  ]
 }
]
```

## Imports closed

Defined by these units (one symbol each): `_$_12GlowingShaft`, `_$_18FrostStormIceShard`, `_$_19BlindingLightEffect`, `_$_12SpellHealing`, `_$_15SpellHolyShield`, `_$_17SpellMinorHealing`.

The partial build's unresolved list is checked in the section below.

## Imports opened

Referenced, not defined here. Each is owned elsewhere, and the list of external names is the same for every unit:

- Callee destructors and helpers: `_$_10GameObject`, `_$_5Light`, `listDestroy__FP8ListHead`, `blockFree__FPv`,
  `P_DeleteParticleSystem__Fi`, `soundStopSound__Fi`.
- Global: `wrathHammerCount` (defined by `w8-net-play-a-global-init-wrath-hammer-count`).
- Vtables: `_vt$5Spell`, `_vt$10Projectile`, and the class tables for each unit, which the `w15-m-rtti-a-vt-*`
  units define.


## Authored but never executed

- Nothing ran on the EE. The paired comparison runs the authored object in the private interpreter, against the image.
- The callees are hooks in that interpreter. No callee was executed as game code in these checks.
- No destructor was reached from a running game. Which vtable slot calls each one, and when, is not shown.

## Unknown

- The meaning of the list heads that each destructor tears down (offsets 4, 24, 28, 76, 80, 84, 88, 164, 168).
- The meaning of `wrathHammerCount`, beyond the observed clamp.
- Flag bits other than bit 0. All seventeen free only on bit 0.
- Whether the compiled bodies are the same instructions as the image. Compiled sizes are not a match claim.
- The baseline partial build's final unresolved list, and whether these entries will close the 17 names in the lead's plan.
