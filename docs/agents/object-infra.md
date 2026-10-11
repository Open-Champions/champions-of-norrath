# Object and constructor infrastructure

Package notes for `agents/object-infra`. Addresses are from the private
hash-pinned `SLUS_205.65` symbol table and the reviewed EE decoder. They are
local evidence, not values reproduced by the relocatable build. Host contracts
are not plan inputs. `input_order` values assume this branch's 298-input plan;
the integrator renumbers when other packages land.

## Slow run list

`objectAddToSlowRunList__FP10GameObject` is 100 bytes at `0x1438a0`. It removes
`object+4` when that word is nonzero, increments `slowListCycle` (`0x483ad0`),
replaces the sum with zero when the signed result is at least 60, and calls
`listOnewayLink` with `slowRunList + index*4`, `object+4`, and the object. The
cycle store is in the call delay slot. The returned word is the link call's
primary node. `slowRunList` at `0x4fbfd0` runs 240 bytes, to `objectGrid`.
`slowListCycle` stays the existing import `norrath_actor_view_483ad0`. The
actor view `norrath_actor_view_50bfd0` is not this list.

## Constructor and hash state

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
`objectHash`. The hash-slot names are `.set` aliases of those tables. Another
`offTheGrid` exists at `0x486848`; the default head is `0x486890`. The reviewed
`-G0` profile puts the 36 zero marker bytes in BSS. The original object is
file-backed `.data`.

## Vtable

`_vt$10GameObject` at `0x44d498` is 144 bytes in `.data`: eighteen 8-byte slots.
Each slot is a zero word followed by a function address, and the last function
word is zero. The constructor stores this address at object+72. The reviewed
compile emits 144 `.data` bytes and seventeen `R_MIPS_32` relocations at offsets
4, 12, ..., 132.

## Factory text

All 263 unresolved `norrath_factory_text_*` views are the C string at that
address through its first NUL. 191 are in `.rodata` and 72 are in `.sdata`.
They are writable because the factory headers declare them mutable. Bytes after
the NUL are not claimed. In particular, `Fire`, `Blocker`, and `Critter` are
followed by other literals.

## Imports closed

- `norrath_object_add_to_slow_run_list`
- `norrath_create_by_name_function_word`
- `norrath_game_object_vtable_view`
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
- `norrath_factory_text_461fc0`
- `norrath_factory_text_461fd0`
- `norrath_factory_text_461fe0`
- `norrath_factory_text_461ff0`
- `norrath_factory_text_462000`
- `norrath_factory_text_462010`
- `norrath_factory_text_462028`
- `norrath_factory_text_462040`
- `norrath_factory_text_462058`
- `norrath_factory_text_462068`
- `norrath_factory_text_4623c0`
- `norrath_factory_text_4623d0`
- `norrath_factory_text_4623e0`
- `norrath_factory_text_4623f0`
- `norrath_factory_text_462400`
- `norrath_factory_text_462410`
- `norrath_factory_text_462420`
- `norrath_factory_text_462430`
- `norrath_factory_text_462440`
- `norrath_factory_text_462450`
- `norrath_factory_text_462460`
- `norrath_factory_text_462470`
- `norrath_factory_text_462480`
- `norrath_factory_text_462490`
- `norrath_factory_text_4624a0`
- `norrath_factory_text_4624b0`
- `norrath_factory_text_4624c0`
- `norrath_factory_text_4624d0`
- `norrath_factory_text_4624e0`
- `norrath_factory_text_4624f0`
- `norrath_factory_text_462500`
- `norrath_factory_text_462510`
- `norrath_factory_text_462520`
- `norrath_factory_text_462530`
- `norrath_factory_text_462540`
- `norrath_factory_text_462550`
- `norrath_factory_text_462560`
- `norrath_factory_text_462570`
- `norrath_factory_text_462580`
- `norrath_factory_text_462590`
- `norrath_factory_text_4625a0`
- `norrath_factory_text_4625b0`
- `norrath_factory_text_4625c0`
- `norrath_factory_text_4625d0`
- `norrath_factory_text_4625e0`
- `norrath_factory_text_4625f0`
- `norrath_factory_text_462600`
- `norrath_factory_text_462610`
- `norrath_factory_text_462620`
- `norrath_factory_text_462630`
- `norrath_factory_text_462640`
- `norrath_factory_text_462650`
- `norrath_factory_text_462660`
- `norrath_factory_text_462670`
- `norrath_factory_text_462680`
- `norrath_factory_text_462690`
- `norrath_factory_text_4626a8`
- `norrath_factory_text_4626c0`
- `norrath_factory_text_4626d0`
- `norrath_factory_text_4626e0`
- `norrath_factory_text_4626f0`
- `norrath_factory_text_462700`
- `norrath_factory_text_462718`
- `norrath_factory_text_462728`
- `norrath_factory_text_462738`
- `norrath_factory_text_462748`
- `norrath_factory_text_462758`
- `norrath_factory_text_462770`
- `norrath_factory_text_462788`
- `norrath_factory_text_462798`
- `norrath_factory_text_4627a8`
- `norrath_factory_text_4627b8`
- `norrath_factory_text_4627d0`
- `norrath_factory_text_4627e8`
- `norrath_factory_text_4627f8`
- `norrath_factory_text_462808`
- `norrath_factory_text_462818`
- `norrath_factory_text_462828`
- `norrath_factory_text_462840`
- `norrath_factory_text_462858`
- `norrath_factory_text_462868`
- `norrath_factory_text_462878`
- `norrath_factory_text_462888`
- `norrath_factory_text_462898`
- `norrath_factory_text_4628a8`
- `norrath_factory_text_4628c0`
- `norrath_factory_text_4628d8`
- `norrath_factory_text_4628e8`
- `norrath_factory_text_4628f8`
- `norrath_factory_text_462908`
- `norrath_factory_text_462918`
- `norrath_factory_text_462930`
- `norrath_factory_text_462948`
- `norrath_factory_text_462958`
- `norrath_factory_text_462968`
- `norrath_factory_text_462978`
- `norrath_factory_text_462988`
- `norrath_factory_text_4629a0`
- `norrath_factory_text_4629b8`
- `norrath_factory_text_4629c8`
- `norrath_factory_text_4629d8`
- `norrath_factory_text_4629e8`
- `norrath_factory_text_4629f8`
- `norrath_factory_text_462a10`
- `norrath_factory_text_462a28`
- `norrath_factory_text_462a38`
- `norrath_factory_text_462a48`
- `norrath_factory_text_462a60`
- `norrath_factory_text_462a78`
- `norrath_factory_text_462a88`
- `norrath_factory_text_462a98`
- `norrath_factory_text_462ab0`
- `norrath_factory_text_462ac0`
- `norrath_factory_text_462ad0`
- `norrath_factory_text_462ae8`
- `norrath_factory_text_462af8`
- `norrath_factory_text_462b08`
- `norrath_factory_text_462b20`
- `norrath_factory_text_462b30`
- `norrath_factory_text_462b40`
- `norrath_factory_text_462b50`
- `norrath_factory_text_462b68`
- `norrath_factory_text_462b78`
- `norrath_factory_text_462b88`
- `norrath_factory_text_462b98`
- `norrath_factory_text_462bb0`
- `norrath_factory_text_462bc8`
- `norrath_factory_text_462bd8`
- `norrath_factory_text_462be8`
- `norrath_factory_text_462bf8`
- `norrath_factory_text_462c10`
- `norrath_factory_text_462c28`
- `norrath_factory_text_462c38`
- `norrath_factory_text_462c48`
- `norrath_factory_text_462c58`
- `norrath_factory_text_462c68`
- `norrath_factory_text_462c80`
- `norrath_factory_text_462c98`
- `norrath_factory_text_462ca8`
- `norrath_factory_text_462cb8`
- `norrath_factory_text_462cc8`
- `norrath_factory_text_462ce0`
- `norrath_factory_text_462cf8`
- `norrath_factory_text_462d08`
- `norrath_factory_text_462d18`
- `norrath_factory_text_462d28`
- `norrath_factory_text_462d38`
- `norrath_factory_text_462d50`
- `norrath_factory_text_462d68`
- `norrath_factory_text_462d78`
- `norrath_factory_text_462d88`
- `norrath_factory_text_462d98`
- `norrath_factory_text_462db0`
- `norrath_factory_text_462dc8`
- `norrath_factory_text_462dd8`
- `norrath_factory_text_462de8`
- `norrath_factory_text_462df8`
- `norrath_factory_text_462e08`
- `norrath_factory_text_462e18`
- `norrath_factory_text_462e28`
- `norrath_factory_text_462e38`
- `norrath_factory_text_462e48`
- `norrath_factory_text_462e58`
- `norrath_factory_text_462e68`
- `norrath_factory_text_462e78`
- `norrath_factory_text_462e88`
- `norrath_factory_text_462e98`
- `norrath_factory_text_462ea8`
- `norrath_factory_text_462eb8`
- `norrath_factory_text_462ec8`
- `norrath_factory_text_462ed8`
- `norrath_factory_text_462ef8`
- `norrath_factory_text_462f18`
- `norrath_factory_text_462f28`
- `norrath_factory_text_462f48`
- `norrath_factory_text_462f68`
- `norrath_factory_text_462f78`
- `norrath_factory_text_462f88`
- `norrath_factory_text_462f98`
- `norrath_factory_text_462fb0`
- `norrath_factory_text_462fc0`
- `norrath_factory_text_462fd0`
- `norrath_factory_text_462fe0`
- `norrath_factory_text_462ff0`
- `norrath_factory_text_463000`
- `norrath_factory_text_463010`
- `norrath_factory_text_463020`
- `norrath_factory_text_463030`
- `norrath_factory_text_463040`
- `norrath_factory_text_463050`
- `norrath_factory_text_463060`
- `norrath_factory_text_483b38`
- `norrath_factory_text_483b40`
- `norrath_factory_text_483b48`
- `norrath_factory_text_483b50`
- `norrath_factory_text_483b60`
- `norrath_factory_text_483b68`
- `norrath_factory_text_483ba8`
- `norrath_factory_text_483c38`
- `norrath_factory_text_483c40`
- `norrath_factory_text_483c48`
- `norrath_factory_text_483c50`
- `norrath_factory_text_483c58`
- `norrath_factory_text_483c60`
- `norrath_factory_text_483c68`
- `norrath_factory_text_483c70`
- `norrath_factory_text_483c78`
- `norrath_factory_text_483c80`
- `norrath_factory_text_483c88`
- `norrath_factory_text_483c90`
- `norrath_factory_text_483c98`
- `norrath_factory_text_483ca0`
- `norrath_factory_text_483ca8`
- `norrath_factory_text_483cb0`
- `norrath_factory_text_483cb8`
- `norrath_factory_text_483cc0`
- `norrath_factory_text_483cc8`
- `norrath_factory_text_483cd0`
- `norrath_factory_text_483cd8`
- `norrath_factory_text_483ce0`
- `norrath_factory_text_483ce8`
- `norrath_factory_text_483cf0`
- `norrath_factory_text_483cf8`
- `norrath_factory_text_483d00`
- `norrath_factory_text_483d08`
- `norrath_factory_text_483d10`
- `norrath_factory_text_483d18`
- `norrath_factory_text_483d20`
- `norrath_factory_text_483d28`
- `norrath_factory_text_483d30`
- `norrath_factory_text_483d38`
- `norrath_factory_text_483d40`
- `norrath_factory_text_483d48`
- `norrath_factory_text_483d50`
- `norrath_factory_text_483d58`
- `norrath_factory_text_483d60`
- `norrath_factory_text_483d68`
- `norrath_factory_text_483d70`
- `norrath_factory_text_483d78`
- `norrath_factory_text_483d80`
- `norrath_factory_text_483d88`
- `norrath_factory_text_483d90`
- `norrath_factory_text_483d98`
- `norrath_factory_text_483da0`
- `norrath_factory_text_483da8`
- `norrath_factory_text_483db0`
- `norrath_factory_text_483db8`
- `norrath_factory_text_483dc0`
- `norrath_factory_text_483dc8`
- `norrath_factory_text_483dd0`
- `norrath_factory_text_483dd8`
- `norrath_factory_text_483de0`
- `norrath_factory_text_483de8`
- `norrath_factory_text_483df0`
- `norrath_factory_text_483df8`
- `norrath_factory_text_483e00`
- `norrath_factory_text_483e08`
- `norrath_factory_text_483e10`
- `norrath_factory_text_483e18`
- `norrath_factory_text_483e20`
- `norrath_factory_text_483e28`
- `norrath_factory_text_483e30`
- `norrath_factory_text_483e38`

## Imports opened

- `norrath_vt_tf_10GameObject` for observed `__tf10GameObject`
- `norrath_vt_dtor_10GameObject` for observed `_$_10GameObject`
- `norrath_vt_msg_hurt_10GameObject` for observed `msg_hurt__10GameObjectfP10GameObjecti`
- `norrath_vt_msg_trigger_10GameObject` for observed `msg_trigger__10GameObjecti`
- `norrath_vt_msg_run_10GameObject` for observed `msg_run__10GameObject`
- `norrath_vt_msg_draw_10GameObject` for observed `msg_draw__10GameObject`
- `norrath_vt_msg_collision_10GameObject` for observed `msg_collision__10GameObjectP10GameObject`
- `norrath_vt_msg_route_10GameObject` for observed `msg_route__10GameObjecti`
- `norrath_vt_msg_load_10GameObject` for observed `msg_load__10GameObjectPc`
- `norrath_vt_msg_save_10GameObject` for observed `msg_save__10GameObjectPc`
- `norrath_vt_msg_collision_world_10GameObject` for observed `msg_collisionWorld__10GameObjectG6Point3`
- `norrath_vt_msg_move_collision_10GameObject` for observed `msg_moveCollision__10GameObjectPP10GameObject`
- `norrath_vt_msg_alert_10GameObject` for observed `msg_alert__10GameObjectifG6Point3P10GameObject`
- `norrath_vt_msg_get_poke_name_10GameObject` for observed `msg_getPokeName__10GameObjectPUsi`
- `norrath_vt_msg_poke_10GameObject` for observed `msg_poke__10GameObjecti`
- `norrath_vt_clone_10GameObject` for observed `clone__10GameObject`
- `norrath_vt_get_model_10GameObject` for observed `getModel__10GameObject`
- already imported elsewhere: `norrath_list_remove`, `norrath_list_oneway_link`, `norrath_actor_view_483ad0`

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

```json
{
  "header_closure": [],
  "input_order": 300,
  "kind": "independently_authored_source",
  "object_name": "game_object_vtable.o",
  "profile": "authored_cpp",
  "sha256": "efed16f3f5b9538e6f17edd7a144519164da33f2f1eadde37698e6de7b4f6127",
  "source": "sources/object-infra-vtable/game_object_vtable.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 301,
  "kind": "independently_authored_source",
  "object_name": "factory_text.o",
  "profile": "authored_cpp",
  "sha256": "d673c8cecade9f718a3992996ecd80f5fca413a923c6d5bed3b4944d2d931cd3",
  "source": "sources/object-infra-factory-text/factory_text.cpp"
}
```

The slow-run function's reviewed compile is one 304-byte body. The other three
units emit no functions. Snapshot entries for a private plan need
`emitted_functions` of that one function, and an empty list for the data units.

## Authored but never executed

The slow-run insertion, its 60 heads, the constructor and hash objects, the
36-byte marker, the vtable, and the 263 string views. Host contracts passed.
No PS2 or game execution. The vtable slots are not called.

## Unknown

- Whether a short free-node pool faults inside the existing link routine. This
  package does not add a failure return.
- Interrupts, timing, and GP preservation at runtime.
- Which vtable offset callers use. The stored words are the observed slots.
- The original marker and several zero words are file-backed `.data` or
  `.sdata`. Under the reviewed `-G0` profile, zero objects land in BSS.
- Bytes after the tag NULs, and the literals that follow `Fire`, `Blocker`,
  and `Critter`, are not part of these objects.
- `.rodata` strings are published writable to match the mutable declarations.
  Whether the original program writes them is not established.
- `slowListCycle` remains the actor package's symbol. The 80
  `norrath_construct_<Actor>` constructors were not started. Factory vtables,
  ctype data, clear-color, and world words are outside this package.
