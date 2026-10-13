# w11-data-b: particle, resource-map and item data (2 of 3)

Package `agents/w11-data-b`. Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that file's
symbol values and file bytes; they are local evidence, not values the relocatable build reproduces.
No game bytes, listings or decompiler output are committed. The resource-map file names are the
same kind of observed text already published for `skeletonResMap`. Nothing here ran on an EE or as
gameplay.

## Authored units

| unit | symbol | image | check |
|---|---|---|---|
| `w11-data-b-vehicle-construct` | `norrath_construct_Vehicle` | `__7VehicleG6Point3sPPci` `0x2702d8`, 700 bytes (compiled 1172) | paired run against the original image on a private R5900 interpreter: 60 scenarios, 0 mismatches, 174 of 175 original words covered; three one-field mutations (height constant, a cleared word offset, the second saved-type store) are each caught (20, 60, 60 mismatches); host contract passes |
| `w11-data-b-resource-map-male-barb` | `maleBarbResourceMap` | `0x420a48`, 904 bytes, .data | all 113 entries compared against the image through the compiled object (string text and second word); a one-name mutation is caught by that check and by the host contract |
| `w11-data-b-resource-map-male-dark-elf` | `maleDarkElfResourceMap` | `0x420dd0`, 904 bytes | same |
| `w11-data-b-resource-map-male-high-elf` | `maleHighElfResourceMap` | `0x421868`, 904 bytes | same |
| `w11-data-b-resource-map-male-wood-elf` | `maleWoodElfResourceMap` | `0x421f78`, 904 bytes | same |
| `w11-data-b-resource-map-male-erudite` | `maleEruditeResourceMap` | `0x422688`, 904 bytes | same |
| `w11-data-b-mummy-name-buffer` | `mummy_miniboss_name_buffer_7d7e20` | `0x7d7e20`, 16 bytes, bss | host contract (size, zero start, longest formatted result) and symbol size in the compiled object |

One unit per commit. Host files are not EE inputs. The Vehicle host contract must be built with
`c++ -std=c++17 -O0 -fno-builtin -ffp-contract=off`; at -O1 the host compiler turns the test's own
fill loop into a `memset` call that recurses into the test's `memset` and never ends.

The Vehicle unit also defines two storage objects it needs, `norrath_vehicle_count_word` (4 bytes) and
`norrath_vehicle_list_bytes` (128 bytes). In the image these are the globals at `0x485e40` (count) and
`0x436fa8` (32-entry list); the paired run compared them at those addresses against the authored ones.
Their original names are not established here.

## Observed, inferred, unknown

Observed: each of the five male resource maps is 113 eight-byte entries: a pointer to a file-name
string and a word that is zero in the image, ending with an all-zero entry. Entry 0 is a model lump
name; later entries are animation, sound and texture file names with many repeats (31 to 72 distinct
names per map). The Vehicle constructor's stores, call order and constants are those of the paired runs.
`typeToResource` (authored in `w10-world-d-resource-tables`) points at these objects.

Inferred: entry position selects what a name is used for; the buffer extent of 16 bytes is the gap to
the next image symbol.

Unknown: the meaning of the second word of a resource-map entry and the entry-position-to-animation
mapping; what the Vehicle object fields written by the constructor mean beyond their observed stores;
the original names of the vehicle count and list globals.

## Names that stay open, and why

- `norrath_item_create`: observed to be the same function as `create__4itemPcPQ24item11descriptionii`
  (`0x105f60`) and `norrath_player_create_item` (unit `game-item-create-build-v2`). Closing it needs an
  alias in that unit or a link-level alias entry; both belong to another package, so no function was
  written (a forwarding wrapper would not be the original body).
- `norrath_item_create_random`: already authored as an alias of `createRandom__4itemPQ24item11descriptioniiii`
  in `w9-hud-inventory-items-c-create-random`, which is not in the current plan. Selecting that unit closes it.
- `lavaGoopParticle`, `lavaSteamParticle`: authored in `w10-render-b-data-lava-particles`, not in the plan.
- `norrath_container_msg_run_fuseParticle_bytes`, `norrath_container_msg_run_smokeParticle_bytes`: authored
  (exported, 80 bytes each, constructed by the static initialiser) in `w9-render-math-c-switch-static-init`,
  not in the plan.
- `lampParticle`: the torch static initialiser (`w8-spell-static-init-a-torch`, in the plan) constructs it,
  but as an object in an anonymous namespace, while `w10-actor-h-msg-run-lamp` imports the exported name. In
  the image it is a local bss object (`0x7d7d80`, 80 bytes) shared by both functions' original unit. A second,
  exported definition here would stay uninitialised, so none was written. The fix is to export that one
  object from the torch unit.
- `norrath_auto_chest_msg_run_fuseParticle_bytes`, `norrath_auto_chest_msg_run_smokeParticle_bytes`: observed to be
  the local bss objects at `0x7d7950` and `0x7d79a0` (80 bytes), read by both AutoChest and AutoContainer
  `msg_run`. They are constructed by `w8-fx-particles-init-has-light-or-particle-number`
  (`_GLOBAL_$I$hasLightOrParticleNumber__FPPci`), which keeps them `static`. Same situation as `lampParticle`: export
  them from that unit under the importing names.
- `norrath_ctors_ant_lantern_view_34b474`: observed to be a word inside `skills` (`0x34b440`, offset 52) that
  holds an object pointer; `norrath_ctors_ant_lantern_view_34b6b1`: inside `levelName` (`0x34b660`, offset 81), the
  level text. Interior symbols cannot be defined from a separate unit (an assembler `.set name, sym+N` in a unit
  that only imports `sym` leaves the name undefined; tried). They need a plan-level symbol alias with an offset, or
  a definition in the unit that owns `skills` / `levelName`.
- The ten `norrath_rtti_entities_a_view_*` names in the task list were no longer unresolved in the current
  integrator build.

## Imports closed (names)

`norrath_construct_Vehicle`, `maleBarbResourceMap`, `maleDarkElfResourceMap`, `maleHighElfResourceMap`,
`maleWoodElfResourceMap`, `maleEruditeResourceMap`, `mummy_miniboss_name_buffer_7d7e20`. Measured by a private
partial build (4,715 inputs) of the shared plan plus these seven entries: 879 unresolved names (integrator build b11)
became 873.

## Imports opened

`processParams__7VehiclePPc` (called by the Vehicle constructor; it is the only new unresolved name).

## Build-plan entries needed

Generated by `docs/agents/w11-data-b-plan-entries.py --toolchain <ee>` (`--private-root DIR` writes a private plan
copy for building). Profile `authored_cpp`, no header closure:

```json
{
 "emitted_functions": [],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_mummy_name_buffer.o",
 "profile": "authored_cpp",
 "sha256": "0c6daec649da4aa0419f4484335bea3f4e6775d85c88be4126a8c90e3e4c836c",
 "source": "sources/w11-data-b-mummy-name-buffer/name_buffer.cpp"
}
{
 "emitted_functions": [],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_resource_map_male_barb.o",
 "profile": "authored_cpp",
 "sha256": "974f4248ef9f29b483857994796c5fc175663fad8b41db01d8f5712bfbc8e026",
 "source": "sources/w11-data-b-resource-map-male-barb/resource_map.cpp"
}
{
 "emitted_functions": [],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_resource_map_male_dark_elf.o",
 "profile": "authored_cpp",
 "sha256": "b8cba7c7899e52f355e51c4f2bf769767b010cf735a4dea0f9391a6f141c7b8c",
 "source": "sources/w11-data-b-resource-map-male-dark-elf/resource_map.cpp"
}
{
 "emitted_functions": [],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_resource_map_male_erudite.o",
 "profile": "authored_cpp",
 "sha256": "da9b626fe3d567417e88584845e617779cb58d9fe0e805ab85bd4096b07dcdc5",
 "source": "sources/w11-data-b-resource-map-male-erudite/resource_map.cpp"
}
{
 "emitted_functions": [],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_resource_map_male_high_elf.o",
 "profile": "authored_cpp",
 "sha256": "07da2b1139e355f0ef456e7dd185b880b146b4144aa9d3a4cdc1d77f0a145a70",
 "source": "sources/w11-data-b-resource-map-male-high-elf/resource_map.cpp"
}
{
 "emitted_functions": [],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_resource_map_male_wood_elf.o",
 "profile": "authored_cpp",
 "sha256": "0e93018abb4ec2613aec61994087ce55fb8b411a8a183b422c55705a514141eb",
 "source": "sources/w11-data-b-resource-map-male-wood-elf/resource_map.cpp"
}
{
 "emitted_functions": [
  {
   "compiled_body_bytes": 1172,
   "name": "norrath_construct_Vehicle"
  }
 ],
 "header_closure": [],
 "kind": "independently_authored_source",
 "object_name": "w11db_vehicle_construct.o",
 "profile": "authored_cpp",
 "sha256": "32ffb8eceddc9a3eaad95f232c817c98cf8a2243e6e99058147ffa907d27b549",
 "source": "sources/w11-data-b-vehicle-construct/construct_vehicle.cpp"
}
```

## Authored but never executed

All seven units are authored only. The Vehicle constructor ran only inside the private interpreter against
stubbed callees; the data objects were compared to image bytes but nothing reads them at run time. No unit ran on
an EE or in the game.

## Unknown

See "Observed, inferred, unknown" above, plus: who owns and names the vehicle count and list globals; whether the
second word of a resource-map entry is ever non-zero at run time (the five maps' image words are all zero);
the female resource maps and `skeletonResMap` use the same entry layout but are not part of this package.
