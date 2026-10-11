# Object and constructor infrastructure

Package notes for `agents/object-infra`. Addresses are from the private
hash-pinned `SLUS_205.65` symbol table and the reviewed EE decoder. They are
local evidence, not values reproduced by the relocatable build.

## Observed

- `objectAddToSlowRunList__FP10GameObject` is 100 bytes at `0x1438a0`.
  It removes `object+4` when that word is nonzero, increments `slowListCycle`
  (`0x483ad0`, 4 bytes), replaces the sum with zero when the signed result is
  at least 60, and calls `listOnewayLink` with `slowRunList + index*4`,
  `object+4`, and the object. The cycle store is in the call delay slot.
  The returned word is the link call's primary node.
- `slowRunList` is at `0x4fbfd0`. The next symbol, `objectGrid`, is 240 bytes
  later, which is 60 words. The authored name is
  `norrath_object_slow_run_list_view`.
- `slowListCycle` is already imported as `norrath_actor_view_483ad0`. This
  package does not define a second symbol for that word.

## Inferred

- The same signed wrap is what the authored actor slow-list wrapper describes.
  The actor view named `norrath_actor_view_50bfd0` is not this list. The object
  routine's base resolves to `0x4fbfd0`.

## Unknown

- Hardware timing, interrupts, and whether a short free-node pool faults inside
  the existing link routine. This function does not add a failure return.

## Build-plan entries needed

```json
{
  "header_closure": [],
  "input_order": 298,
  "kind": "independently_authored_source",
  "object_name": "add_to_slow_run_list.o",
  "profile": "authored_cpp",
  "sha256": "70f0df24eb2fdc22c25061ec679378f38f32786d1d499bc780d63d7d3d53bbd9",
  "source": "sources/object-infra-slow-run-list/add_to_slow_run_list.cpp"
}
```

The host contract `add_to_slow_run_list_contract.cpp` is not a plan input.

```json
{
  "header_closure": [],
  "input_order": 299,
  "kind": "independently_authored_source",
  "object_name": "state.o",
  "profile": "authored_cpp",
  "sha256": "b55eefe01504a82d6937f31956d8104c3b6e0c0e60d33b55abfe5a93b90fad98",
  "source": "sources/object-infra-state/state.cpp"
}
```

`state_contract.cpp` is not a plan input. `input_order` values assume this
branch's plan length; the integrator renumbers when other packages land.

## State evidence

Observed symbol, address, extent, and initial value:

| Authored name | Observed symbol | Address | Extent | Initial |
|---|---|---|---|---|
| `norrath_constructor_default_head_view` | `offTheGrid` | `0x486890` | 4 | BSS zero |
| `norrath_create_by_name_function_word` | `userCreateByNameFunction` | `0x486894` | 4 | BSS zero |
| `norrath_constructor_string_count_view` | `objectStrings` | `0x486898` | 4 | BSS zero |
| `norrath_constructor_list_pool_view` | `freeNodes` | `0x486a6c` | 4 | BSS zero |
| `norrath_constructor_random_state_view` | `syncRandState` | `0x483e5c` | 4 | file zero |
| `norrath_constructor_resource_frame_view` | `resourceFrame` | `0x483f88` | 4 | `01 00 00 00` |
| `norrath_constructor_lump_count_view` | `nmLumps` | `0x483f8c` | 4 | file zero |
| `norrath_constructor_grid_a_view` | `objectGrid` | `0x4fc0c0` | 57600 | BSS zero |
| `norrath_constructor_grid_b_view` | `ghostObjectGrid` | `0x50a1c0` | 57600 | BSS zero |
| `norrath_constructor_lump_cache_view` | `lumps` | `0x6cb280` | 5120 | BSS zero |
| `norrath_constructor_string_names_view` | `stringHash` | `0x5182c0` | 2048 | BSS zero |
| `norrath_constructor_string_objects_view` | `objectHash` | `0x518ac0` | 2048 | BSS zero |
| `norrath_constructor_tag_a_view` | unnamed bytes | `0x483ae0` | through NUL | `hide` |
| `norrath_constructor_tag_b_view` | unnamed bytes | `0x483ae8` | through NUL | `name` |
| `norrath_factory_marker_data_view` | `lightStart` | `0x37f960` | 36 | file zero |

`objectAddString` and `objectFindObject` both address `stringHash` and
`objectHash`. The hash-slot names are `.set` aliases of the constructor
tables, not a second pair of arrays. There is another `offTheGrid` at
`0x486848`; the default head is the one at `0x486890`. Tag bytes stop at the
first NUL. Neighboring literals after those NULs are not claimed. The reviewed
`-G0` profile puts the 36 zero marker bytes in BSS; the original object is
file-backed `.data`.

## Imports closed

- `norrath_object_add_to_slow_run_list`
- `norrath_create_by_name_function_word`
- `norrath_constructor_default_head_view`
- `norrath_constructor_grid_a_view`
- `norrath_constructor_grid_b_view`
- `norrath_constructor_list_pool_view`
- `norrath_constructor_lump_cache_view`
- `norrath_constructor_lump_count_view`
- `norrath_constructor_random_state_view`
- `norrath_constructor_resource_frame_view`
- `norrath_constructor_string_count_view`
- `norrath_constructor_string_names_view`
- `norrath_constructor_string_objects_view`
- `norrath_constructor_tag_a_view`
- `norrath_constructor_tag_b_view`
- `norrath_object_name_hash_slots_view`
- `norrath_object_pointer_hash_slots_view`
- `norrath_factory_marker_data_view`

## Imports opened

- none beyond symbols already imported by existing units
  (`norrath_list_remove`, `norrath_list_oneway_link`, `norrath_actor_view_483ad0`)

## Authored but never executed

- The slow-run insertion, its 60 heads, and the state objects above.
  Host contracts passed. No PS2 execution.

## Still open in this package

The game-object vtable and the factory text literals. The 80
`norrath_construct_<Actor>` constructors are out of scope.
