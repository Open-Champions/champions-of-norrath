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

Status: 16 of 17 units have sources and checks in this branch.

## What was done

Each destructor stores the class table word(s) at the offsets seen in the image, calls the same callees in the same
order, and then, when bit 0 of the incoming flags is set, calls `blockFree__FPv` on the object (a tail call in the image).
Other flag bits do not free. The flags are read once, before the first call, as the image does.

- GameObject: 12GlowingShaft, 18FrostStormIceShard, 19BlindingLightEffect
- Spell base: 12SpellHealing, 15SpellHolyShield, 17SpellMinorHealing
- Projectile: 8HolyBolt, 8LostSoul, 10FrostShard, 10HolyStrike, 18DarknessProjectile
- Spell with particle, sound, light or list: 13SpellEntangle, 15SpellClericRoot, 15SpellHateEffect, 16SpellCriticalHit, 16SpellConeOfFrost

## Units

| source directory | symbol | image address and size | compiled bytes | source sha256 |
|---|---|---|---|---|
| `w16-m-dtor-a-dtor-12glowingshaft` | `_$_12GlowingShaft` | `0x00310720`, 100 bytes (weak) | 88 | `60e9f2a1ff1aa79d6f9e7384185f23778a2494f36aa6fb0fb2864e26c8ed7170` |
| `w16-m-dtor-a-dtor-18froststormiceshard` | `_$_18FrostStormIceShard` | `0x00313d20`, 100 bytes (weak) | 88 | `cf92d4b1cf36a15381214dadb7e3759062fb6ea4d8da6879af06950f8f5dbdab` |
| `w16-m-dtor-a-dtor-19blindinglighteffect` | `_$_19BlindingLightEffect` | `0x00319c68`, 100 bytes (weak) | 88 | `a5e318ac792d5ca0c3912985dbd1df183cc81618f10c4a4480d19542da43b63d` |
| `w16-m-dtor-a-dtor-12spellhealing` | `_$_12SpellHealing` | `0x00319208`, 100 bytes (weak) | 92 | `db6cf1c8f5b9432d28fb641778c913c8a56018ad732a9aba210b72dd35836ea1` |
| `w16-m-dtor-a-dtor-15spellholyshield` | `_$_15SpellHolyShield` | `0x00319068`, 100 bytes (weak) | 92 | `1aa97b1e82e523232bca03c1c4b3d615556288a8fc53fbab44824d60b3b3968c` |
| `w16-m-dtor-a-dtor-17spellminorhealing` | `_$_17SpellMinorHealing` | `0x003186b0`, 100 bytes (weak) | 92 | `830c02726b6503281b35bb05b8e86de0523243e9cfbfbf6e9642544b46ab6a80` |
| `w16-m-dtor-a-dtor-8holybolt` | `_$_8HolyBolt` | `0x00310c08`, 132 bytes (weak) | 136 | `a9dc2878825ec2419464bed92d159599fd25a789f068163e53e1aa11f59b8463` |
| `w16-m-dtor-a-dtor-8lostsoul` | `_$_8LostSoul` | `0x00310e28`, 132 bytes (weak) | 136 | `23c49cf70fc4c9a1d13c8f5906638e7ce060b6cab773aebfa2932a3030fbf4c5` |
| `w16-m-dtor-a-dtor-10frostshard` | `_$_10FrostShard` | `0x00319e88`, 132 bytes (weak) | 136 | `2376a18cd63280c9b25c328bc2b77b8578c2f0b941d9095cfcc747f7acd95710` |
| `w16-m-dtor-a-dtor-10holystrike` | `_$_10HolyStrike` | `0x00319598`, 140 bytes (weak) | 144 | `7b31aea3181015dfc6c1619dd1466af13ad4f34e67d3f6bb8bc7e2dbf30dee05` |
| `w16-m-dtor-a-dtor-18darknessprojectile` | `_$_18DarknessProjectile` | `0x003172c0`, 132 bytes (weak) | 136 | `8ef4f7f49779e4586f554bd0af295b3dd157fc26ad3a6b61c4566f77a4cd1b27` |
| `w16-m-dtor-a-dtor-13spellentangle` | `_$_13SpellEntangle` | `0x002c7040`, 116 bytes (global) | 116 | `82de07b8e62fc2ff4ae0524660f11f9f12673e8737243bca5e497cd050545848` |
| `w16-m-dtor-a-dtor-15spellclericroot` | `_$_15SpellClericRoot` | `0x002c63c8`, 116 bytes (global) | 116 | `2ef8d68593a8586353a6adab63e8f61a3eafff1230dba36aba27164ead5ae6b2` |
| `w16-m-dtor-a-dtor-15spellhateeffect` | `_$_15SpellHateEffect` | `0x00314638`, 124 bytes (weak) | 124 | `3c9fb09fa78910cfe99043e7639c39d5b7207f485a4011c30c3b4981f375c31e` |
| `w16-m-dtor-a-dtor-16spellcriticalhit` | `_$_16SpellCriticalHit` | `0x00318f00`, 124 bytes (weak) | 128 | `92b1f5f32f7b76dd30736dcac2f48d827360ef16436d468021c6d0be49a8c749` |
| `w16-m-dtor-a-dtor-16spellconeoffrost` | `_$_16SpellConeOfFrost` | `0x002aff00`, 132 bytes (global) | 136 | `2baa02481eda7e62b51c29917c7cfa9ddab1a81ea7318194cc78dd6ecee5437e` |

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

- **EE compile.** 16 of 16 sources compile with `/Users/case/norrath-agents/repo/work/toolchains/ee` (GCC 15.2.0)
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
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_8holybolt.o",
  "profile": "authored_cpp",
  "sha256": "a9dc2878825ec2419464bed92d159599fd25a789f068163e53e1aa11f59b8463",
  "source": "sources/w16-m-dtor-a-dtor-8holybolt/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_8HolyBolt",
    "compiled_body_bytes": 136
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_8lostsoul.o",
  "profile": "authored_cpp",
  "sha256": "23c49cf70fc4c9a1d13c8f5906638e7ce060b6cab773aebfa2932a3030fbf4c5",
  "source": "sources/w16-m-dtor-a-dtor-8lostsoul/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_8LostSoul",
    "compiled_body_bytes": 136
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_10frostshard.o",
  "profile": "authored_cpp",
  "sha256": "2376a18cd63280c9b25c328bc2b77b8578c2f0b941d9095cfcc747f7acd95710",
  "source": "sources/w16-m-dtor-a-dtor-10frostshard/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_10FrostShard",
    "compiled_body_bytes": 136
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_10holystrike.o",
  "profile": "authored_cpp",
  "sha256": "7b31aea3181015dfc6c1619dd1466af13ad4f34e67d3f6bb8bc7e2dbf30dee05",
  "source": "sources/w16-m-dtor-a-dtor-10holystrike/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_10HolyStrike",
    "compiled_body_bytes": 144
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_18darknessprojectile.o",
  "profile": "authored_cpp",
  "sha256": "8ef4f7f49779e4586f554bd0af295b3dd157fc26ad3a6b61c4566f77a4cd1b27",
  "source": "sources/w16-m-dtor-a-dtor-18darknessprojectile/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_18DarknessProjectile",
    "compiled_body_bytes": 136
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_13spellentangle.o",
  "profile": "authored_cpp",
  "sha256": "82de07b8e62fc2ff4ae0524660f11f9f12673e8737243bca5e497cd050545848",
  "source": "sources/w16-m-dtor-a-dtor-13spellentangle/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13SpellEntangle",
    "compiled_body_bytes": 116
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_15spellclericroot.o",
  "profile": "authored_cpp",
  "sha256": "2ef8d68593a8586353a6adab63e8f61a3eafff1230dba36aba27164ead5ae6b2",
  "source": "sources/w16-m-dtor-a-dtor-15spellclericroot/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_15SpellClericRoot",
    "compiled_body_bytes": 116
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_15spellhateeffect.o",
  "profile": "authored_cpp",
  "sha256": "3c9fb09fa78910cfe99043e7639c39d5b7207f485a4011c30c3b4981f375c31e",
  "source": "sources/w16-m-dtor-a-dtor-15spellhateeffect/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_15SpellHateEffect",
    "compiled_body_bytes": 124
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_16spellcriticalhit.o",
  "profile": "authored_cpp",
  "sha256": "92b1f5f32f7b76dd30736dcac2f48d827360ef16436d468021c6d0be49a8c749",
  "source": "sources/w16-m-dtor-a-dtor-16spellcriticalhit/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_16SpellCriticalHit",
    "compiled_body_bytes": 128
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16mdb_dtor_16spellconeoffrost.o",
  "profile": "authored_cpp",
  "sha256": "2baa02481eda7e62b51c29917c7cfa9ddab1a81ea7318194cc78dd6ecee5437e",
  "source": "sources/w16-m-dtor-a-dtor-16spellconeoffrost/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_16SpellConeOfFrost",
    "compiled_body_bytes": 136
   }
  ]
 }
]
```

## Imports closed

Defined by these units (one symbol each): `_$_12GlowingShaft`, `_$_18FrostStormIceShard`, `_$_19BlindingLightEffect`, `_$_12SpellHealing`, `_$_15SpellHolyShield`, `_$_17SpellMinorHealing`, `_$_8HolyBolt`, `_$_8LostSoul`, `_$_10FrostShard`, `_$_10HolyStrike`, `_$_18DarknessProjectile`, `_$_13SpellEntangle`, `_$_15SpellClericRoot`, `_$_15SpellHateEffect`, `_$_16SpellCriticalHit`, `_$_16SpellConeOfFrost`.

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
