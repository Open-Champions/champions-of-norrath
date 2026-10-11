# actors-a

Package: `norrath_construct_*` imports whose class name starts with A–F.
Work is on `agents/actors-a`. Shared base helpers are imported, not re-authored.

## Observed

Private executable SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
`_gp` is `0x004879f0`. Constructor symbols below are the retained GCC 2.95 names.

`norrath_construct_Blocker` is `__7BlockerG6Point3sPPc` at `0x00216600`, 336 bytes.
It calls `norrath_game_object_construct` with the incoming argument list, stores view `0x0044c830`, writes `24.0` at offset 28 and the low 16 bits of the type at offset 56, then repeats two tag-string checks (`0x00485400`, `0x00485408`) before converting a following tag int to a float at the same offset. A third tag int at `0x00470830` sets offset 32 to `1.0` and ORs `0x10` when nonzero. A zero height ORs `0x205` into V0 before the common OR of `0x2`. V0 is not the final flag word on that path.

The comparison in `docs/agents/actors-a-compare.py` runs the original instructions and the reviewed EE object with the same stubs. Six scenarios matched calls, the copied point, object bytes, and V0.

## Inferred

The three tag addresses are the words those `lui`/`addiu` pairs form. No string text is copied into source.

## Unknown

Retail edition, whether a zero height uses IEEE equality for every NaN pattern, and every constructor in this package that is not listed as closed below.

## Build-plan entry

```json
{
  "header_closure": ["sources/actors-a-blocker/blocker.hpp"],
  "kind": "independently_authored_source",
  "object_name": "blocker.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-blocker/blocker.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-candle2/candle2.hpp"],
  "kind": "independently_authored_source",
  "object_name": "candle2.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-candle2/candle2.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-container/container.hpp"],
  "kind": "independently_authored_source",
  "object_name": "container.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-container/container.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-candle/candle.hpp"],
  "kind": "independently_authored_source",
  "object_name": "candle.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-candle/candle.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-container-resources/container_resources.hpp"],
  "kind": "independently_authored_source",
  "object_name": "container_resources.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-container-resources/container_resources.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-eggsack/eggsack.hpp"],
  "kind": "independently_authored_source",
  "object_name": "eggsack.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-eggsack/eggsack.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-doorsecret/doorsecret.hpp"],
  "kind": "independently_authored_source",
  "object_name": "doorsecret.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-doorsecret/doorsecret.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-doorswing/doorswing.hpp"],
  "kind": "independently_authored_source",
  "object_name": "doorswing.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-doorswing/doorswing.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-floorswitch/floorswitch.hpp"],
  "kind": "independently_authored_source",
  "object_name": "floorswitch.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-floorswitch/floorswitch.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-anttrap/anttrap.hpp"],
  "kind": "independently_authored_source",
  "object_name": "anttrap.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-anttrap/anttrap.cpp"
}
```

```json
{
  "header_closure": ["sources/actors-a-firebomb/firebomb.hpp"],
  "kind": "independently_authored_source",
  "object_name": "firebomb.o",
  "profile": "authored_cpp",
  "source": "sources/actors-a-firebomb/firebomb.cpp"
}
```

## Status

`norrath_construct_Candle2` is `__7Candle2G6Point3` at `0x002310a8`, 120 bytes. It clears both base-call pointers, stores view `0x0044a930`, ORs flag `1`, writes `0xf0` at byte 40, clears bytes 76 and 77, and returns `norrath_actor_add_run_list`.

`norrath_construct_Container` is `__9ContainerG6Point3sPPc` at `0x0021d578`, 180 bytes. It passes the argument list to the base call, stores view `0x0044ccb8`, ORs `0x10`, stores the type at offset 56, `-1` at 120 and zero at 116, then calls `worldCheckForWater` with the object position, radius bits `0x41400000`, and a null output. V0 is that result; byte 124 stores only its low 8 bits.

`norrath_construct_Candle` is `__6CandleG6Point3i` at `0x002301a8`, 332 bytes. `lightAdd` sees the first range words at the light stored at object+76. Later stores replace byte 85 and the halves at 94, 96 and 98. V0 is the run-list result.

`norrath_construct_ContainerWithResources` is `__9ContainerG6Point3sPcN43iiiPPc` at `0x0021d6f0`, 380 bytes. It stores the same view as Container, converts the first two stack integers to floats at offsets 28 and 32, and ORs `0x00100000` when bit 2 of the third integer is set. The resource queries are `lumpFindResource(first, second)` then `lumpFindResource(first, third)`.

`norrath_construct_EggSack` is `__7EggSackPcG6Point3sPPc` at `0x002260b8`, 136 bytes. It forwards the name, a copied point, the sign-extended type, and the argument list to `norrath_construct_AutoPropLP`, stores view `0x0044f2a0`, writes 2 at offsets 452, 448, 444 and 440, and ORs 2 into the word at 316. V0 is object+436.

`norrath_construct_DoorSecret` is `__10DoorSecretG6Point3PPc` at `0x00212f08`, 332 bytes. A present tag at `0x00485358` clears bit `0x00100000` before the door queries, and the last flag store sets that bit again. A negative door index calls `norrath_object_add_to_delete_list`. V0 is `0x00100000`.

`norrath_construct_DoorSwing` is `__9DoorSwingG6Point3PPc` at `0x002127f0`, 352 bytes. The tag-string clear of bit `0x00100000` remains. After the second collision update, a nonzero integer tag at `0x00485360` adds the object to the delete list and that result is V0. A zero integer tag leaves V0 at 0. The final view is `0x004499d8`.

`norrath_construct_FloorSwitch` is `__11FloorSwitchG6Point3PPc` at `0x002132c0`, 296 bytes. It performs one collision update, stores the door height before the negative-index delete, then replaces the view with `0x00449790` and returns `norrath_object_add_to_medium_run_list`. The tag-string clear of bit `0x00100000` remains.

`norrath_construct_AntTrap` is `__7AntTrapPcG6Point3sPPc` at `0x00225fe8`, 204 bytes. It forwards to `norrath_construct_AutoProp`, ORs flag 1, and queries `lumpFindResource` with the bytes at object+109 and the word at `0x00471310`. A null header leaves V0 at 0. Otherwise the state at object+152 is initialized, the header is added with mode 4, set to its end, the half at 158 gains bit `0x80`, and that half is V0. The header receives bits `0x3f800000` at +20 and byte 1 at +24.

`norrath_construct_FireBomb` is `__8FireBombG6Point3sPPc` at `0x002898b0`, 392 bytes. The base call keeps the resource map at `0x00438e98`. Flags are replaced with `0x4001`, offset 28 receives bits `0x42700000`, and byte 77 is cleared before the run list. Absent tags store 240, byte 0, and byte 1. A present first tag stores 60 times its integer at offset 80; the other two store the integer's low byte at 78 and 79. The word at `0x00483e5c` becomes `state * 0x00019660d + 0x3c6ef35f`. V0 is that state shifted right 16. Offset 84 stores the toward-zero remainder of V0 divided by the word at offset 80.

imports closed: `norrath_construct_Blocker`, `norrath_construct_Candle2`, `norrath_construct_Container`, `norrath_construct_Candle`, `norrath_construct_ContainerWithResources`, `norrath_construct_EggSack`, `norrath_construct_DoorSecret`, `norrath_construct_DoorSwing`, `norrath_construct_FloorSwitch`, `norrath_construct_AntTrap`, `norrath_construct_FireBomb`

imports opened: `norrath_world_check_for_water`; `norrath_light_add`; `norrath_construct_AutoPropLP`; `norrath_construct_AutoProp`; `norrath_world_find_closest_door`; `norrath_world_set_door_collision`; `norrath_world_get_door_height`; `norrath_object_add_to_delete_list`; `norrath_object_add_to_medium_run_list` (retained `objectAddToMediumRunList__FP10GameObject`). Container, ContainerWithResources, and AntTrap use the existing `norrath_lump_find_resource`, `norrath_actor_animation_initialize`, `norrath_actor_animation_add`, and `norrath_animation_set_to_end`.

build-plan entries needed: the Blocker, Candle2, Container, Candle, ContainerWithResources, EggSack, DoorSecret, DoorSwing, FloorSwitch, AntTrap, and FireBomb entries above.

authored but never executed: `norrath_construct_Blocker`, `norrath_construct_Candle2`, `norrath_construct_Container`, `norrath_construct_Candle`, `norrath_construct_ContainerWithResources`, `norrath_construct_EggSack`, `norrath_construct_DoorSecret`, `norrath_construct_DoorSwing`, `norrath_construct_FloorSwitch`, `norrath_construct_AntTrap`, `norrath_construct_FireBomb` (compared against the original bodies; not run as gameplay).

unknown: NaN behavior of the Blocker height compare, and the remaining A–F constructor imports `norrath_construct_Ant`, `norrath_construct_AntQueen`, `norrath_construct_Arenabeast`, `norrath_construct_AutoChest`, `norrath_construct_AutoChestLP`, `norrath_construct_AutoContainer`, `norrath_construct_AutoContainerLP`, `norrath_construct_AutoProp`, `norrath_construct_AutoPropLP`, `norrath_construct_Badger`, `norrath_construct_BlackWidow`, `norrath_construct_Boat`, `norrath_construct_Catapult`, `norrath_construct_Chest`, `norrath_construct_CloudGiant`, `norrath_construct_Critter`, `norrath_construct_Cthulu`, `norrath_construct_Cyclops`, `norrath_construct_Demon`, `norrath_construct_FemaleDarkElf`, `norrath_construct_FireBeetle`, `norrath_construct_FireFly`, and `norrath_construct_Froglock`. Cat and CdEyeball are already defined outside this package. `check_source_snapshot.py` will not accept these new files until the integrator records them; that shared file was left unchanged.
