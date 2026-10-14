# Actors-b constructor notes

Package: unresolved `norrath_construct_<Class>` imports whose class name starts
with G–Z. Branch `agents/actors-b`. Shared base-class machinery is imported.

The partial-build import list used here is
`work/t3-build-check-20261011/unresolved-imports.json` (848 names, 55 of them
G–Z constructors). Every one of those 55 has consumer count 1, all from
`factory.cpp`, so size order was used after that tie. Private reference:
`work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Addresses below are that file's symbol values. No game bytes were copied into
this tree. Vtable and tag addresses are unresolved views, not payloads.

Each constructor below was compiled with the reviewed EE g++ `authored_cpp`
flags and has a host contract under `host/` that fails if the stores, call
order, or forwarded result are wrong. Host contracts are not EE inputs.
`snapshot.json` still rejects unlisted files under `experimental/sources/`;
this package does not edit that file. Record every file below, and add only
the `.cpp` units that are not under `host/` to the plan.

`detail.hpp` sha256 `bfcdcef9d66e9ae63dc4f93ca51e481ee6da2790cf7452ffe4a35583faa9737d`.

## Build-plan entries

Profile `authored_cpp`. Header closure for each unit is
`sources/actors-b-constructors/detail.hpp`.

| source | sha256 | object | emitted | bytes |
|---|---|---|---|---|
| `hack_door.cpp` | `68c5ab661c21dfab5c890cf870f087156c116f2c56ecee53a7c756b7135f01da` | `hack_door.o` | `norrath_construct_HackDoor` | 164 |
| `hate_gate.cpp` | `7ef114f03aeef07cd45c77f929312dacf7cb6cec62f61ad26b561768f71e91d4` | `hate_gate.o` | `norrath_construct_HateGate` | 164 |
| `hate_into.cpp` | `899fe15c2b236d5b0f165bf1155569d0f8ab4c4e9917dde6ae491dc89b61e182` | `hate_into.o` | `norrath_construct_HateInto` | 192 |
| `goth_trap.cpp` | `473019f40c46213fcbc870f1c78c3507d5c2f683e6127183c64754ed9eab0be8` | `goth_trap.o` | `norrath_construct_GothTrap` | 188 |
| `hate_hole.cpp` | `597b8d7f1ff4ee00644cd9b1cb8e76bf3944f4ac801dcbea09c5eccb01378dc2` | `hate_hole.o` | `norrath_construct_HateHole` | 272 |
| `hate_bridge.cpp` | `77b8f692c7b05dd836758d7ad546940fc0dcaf231b2784bf636fa18ec8b863c2` | `hate_bridge.o` | `norrath_construct_HateBridge` | 196 |
| `teleporter.cpp` | `a622b17974c627c235074778610e1af7102eff6919431d5cf0e4b3b98bb49ee6` | `teleporter.o` | `norrath_construct_Teleporter` | 228 |
| `push_physics_prop_simple.cpp` | `2c2a9a31e5e525fb251f4d284969fddd3ecb1dbe759f319bb776e972a727d11c` | `push_physics_prop_simple.o` | `norrath_construct_PushPhysicsPropSimple` | 300 |
| `missile_trap.cpp` | `0b0dc1bf8f5393fe82f1771df856fc5bdcce3c707d840f582e696ab976c24762` | `missile_trap.o` | `norrath_construct_MissileTrap` | 296 |
| `shooter.cpp` | `61c14b8add8cd1e4ba28fbf50e5907e2b2ca366579cbdab38e56f77a2a3fb1c4` | `shooter.o` | `norrath_construct_Shooter` | 288 |
| `trigger.cpp` | `0d9f5c37abf054d4d7e60f1acba63a2d3c22197fade9f8e9f3111296bbe69f75` | `trigger.o` | `norrath_construct_Trigger` | 704 |
| `lava_tractor.cpp` | `61d29c41c76f3feae35df56376df2ff10178ad0596f93ad331044d802385da5b` | `lava_tractor.o` | `norrath_construct_LavaTractor` | 136 |
| `skul_boat.cpp` | `545e15f4de876c6094a3218c757697bdbb49e06df46abf12af3cb2183cdf6173` | `skul_boat.o` | `norrath_construct_SkulBoat` | 396 |

One exact entry, repeated for the other rows with the table's source, object,
sha256, and emitted function:

```json
{
  "header_closure": ["sources/actors-b-constructors/detail.hpp"],
  "kind": "independently_authored_source",
  "object_name": "hack_door.o",
  "profile": "authored_cpp",
  "source": "sources/actors-b-constructors/hack_door.cpp",
  "sha256": "68c5ab661c21dfab5c890cf870f087156c116f2c56ecee53a7c756b7135f01da",
  "emitted_functions": [{"name": "norrath_construct_HackDoor", "compiled_body_bytes": 164}]
}
```

## What was reconstructed

Observed originals, not byte-match claims:

- HackDoor `0x2264e8` / 96: AutoProp, then `_vt$8HackDoor` at +72 and flag bit 1 at +12. V0 is AutoProp's.
- HateGate `0x2261b8` / 96: same parent; vtable `0x44f0a8`; bit 1 at +208.
- HateInto `0x226140` / 116: vtable `0x44f150`, clear byte 156, `animAdd` of the word at +296 onto state +152 with flag 1, then bit 1 at +208. V0 is animAdd's.
- GothTrap `0x226218` / 116: vtable `0x44f000`, bit 1 at +12, run list, then `setPassableState__8GothTrapb(object, 1)`.
- HateHole `0x226298` / 128: vtable `0x44ef58`, bit 1 at +12, `worldGetDoorHeight` of the `world` pointer at `0x4844a0` and the word at +396, `cvt.s.w` stored at +408, then run list.
- HateBridge `0x2263e8` / 124: vtable `0x44eeb0`, bit 1 at +12, `setPassableState__10HateBridgeb(object, 1)`, run list, byte 40 = low 8 bits of -80.
- Teleporter `0x21ce00` / 120: Creature with the single at `0x4816d4`, null map, cleared half 572, vtable `0x448e30`, flags ORed with `0xf`, slow run list.
- PushPhysicsProp simple `0x155ce8` / 128: GameObject with null map and arguments, vtable `0x448a70`, flag `0x10`, run list, copy 12 bytes from +16 to +232. Low V0 is the word at +16.
- MissileTrap `0x1e5bc8` / 172: GameObject, sscanf of the first argument string through format address `0x4850f0` when that string is non-null, vtable `0x44a4b0`, low halves at 56/76/78, flags ORed with 9. V0 is that flags word.
- Shooter `0x1e5ca8` / 216: sscanf through format address `0x4850f8`, then half 84 is replaced with 100, half 82 keeps the low 16 of the int, bytes 80..77 cleared by the BGEZ loop and byte 76 cleared separately, flags ORed with 9, medium run list. Vtable `0x44a420`.
- Trigger `0x1d1470` / 256: two `findTagInt` calls, `negu` on the second, flags replaced with 3, `findTagString`, finite `min.s`/`max.s` bounds at 76/80/84/88, medium run list, then word 104 = -1. Tag addresses `0x484fd8`, `0x484fe0`, `0x484fe8`. Vtable `0x44c8c0`.
- LavaTractor `0x284b88` / 128: Vehicle with integer 0, vtable `0x450250`, `Reset__7Vehiclei(object, 0)`.
- SkulBoat `0x2bfaf0` / 208: Vehicle with integer 1, vtable `0x453230`, `worldCheckForWater` with immediate single 32 and output at object+24, copy of the 12 bytes at +16 to +1632, clear bit `0x00100000`, then Vehicle reset with 0.

## Imports closed

`norrath_construct_HackDoor`, `norrath_construct_HateGate`, `norrath_construct_HateInto`,
`norrath_construct_GothTrap`, `norrath_construct_HateHole`, `norrath_construct_HateBridge`,
`norrath_construct_Teleporter`, `norrath_construct_PushPhysicsPropSimple`,
`norrath_construct_MissileTrap`, `norrath_construct_Shooter`, `norrath_construct_Trigger`,
`norrath_construct_LavaTractor`, `norrath_construct_SkulBoat`.

## Imports opened

New names this package references and does not define:

- `norrath_goth_trap_set_passable_state` — original `setPassableState__8GothTrapb` at `0x225cc0`
- `norrath_hate_bridge_set_passable_state` — original `setPassableState__10HateBridgeb` at `0x225df0`
- `norrath_world_get_door_height` — original `worldGetDoorHeight__FP12_worldHeaderi` at `0x17d3d0` (world-state)
- `norrath_world_check_for_water` — original `worldCheckForWater__FP12_worldHeaderG6Point3fPf` at `0x17db78` (world-state)
- `norrath_actor_add_medium_run_list` — original `objectAddToMediumRunList__FP10GameObject` at `0x143908`
- `norrath_construct_Vehicle` — original `__7VehicleG6Point3sPPci` at `0x2702d8`, 700 bytes. Alphabet V, but it was not in the partial-build import list until LavaTractor and SkulBoat.
- `norrath_vehicle_reset` — original `Reset__7Vehiclei` at `0x270f28`, 576 bytes
- `sscanf` — same symbol launchSRS already calls; MissileTrap passes two int slots, Shooter passes one
- views, address only: `norrath_actors_b_view_44ee08`, `44f0a8`, `44f150`, `44f000`, `44ef58`, `44eeb0`, `4844a0` (`world`), `448e30`, `4816d4`, `448a70`, `44a4b0`, `4850f0`, `44a420`, `4850f8`, `44c8c0`, `484fd8`, `484fe0`, `484fe8`, `450250`, `453230`

Already present and only called: `norrath_construct_AutoProp` (still unresolved; actors-a),
`norrath_game_object_construct`, `norrath_creature_construct`, `norrath_actor_add_run_list`,
`norrath_actor_add_slow_run_list`, `norrath_game_object_find_tag_int`,
`norrath_actor_find_tag_string`, `norrath_actor_animation_add`.

The published `norrath_actor_animation_add` is `void`. HateInto forwards the
callee's machine result. That V0 is not established until the callee returns
the original `animAdd` value.

## Authored but never executed

All 13 constructors were executed only as host processes with stub callees.
They were not executed on the EE, against the pinned executable, or in a game.
Nothing here is an original-byte match.

## Unknown

Still unresolved G–Z constructors, in original-size order: Switch, Lamp, Ice,
Lantern, PushPhysicsProp, RollDoor, Torch, Rondo, Generator, Savepoint,
WeaponRack, Gnome, Soul, Wraith, SeaMonster, Vampire, StaticFire, Skelight,
SuperOrc, WebTrap, Scorpion, MummyKing, Ulthork, UndeadKnight, Ghoul, Minion,
NPC, WoodElfSoldier, UndeadKnightClone, JumpGate, VampireLord, SpiderQueen,
LavaMonster, Innoruuk, WaveMaker, Mummy, Goblin, MaleDarkElf, Orc, Skeleton.
WaterSpout (`0x1e5700`, 232 bytes) was read and not authored: it calls `icos`,
`isin`, and the software double helpers around the double at `0x46d0c0`.

Also unknown: vtable bytes, the owned initializers of the views above,
`min.s`/`max.s` NaN and signed-zero results, Vehicle and both passable-state
bodies, and whether any caller uses these constructors' V0. The factory
comments say the machine word is ignored.
