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

## Imports closed

- `norrath_object_add_to_slow_run_list`

## Imports opened

- none beyond symbols already imported by existing units
  (`norrath_list_remove`, `norrath_list_oneway_link`, `norrath_actor_view_483ad0`)

## Authored but never executed

- `norrath_object_add_to_slow_run_list` and `norrath_object_slow_run_list_view`.
  The host contract passed. No PS2 execution.

## Still open in this package

Constructor views, the create-by-name word, both hash names, the game-object
vtable, factory marker bytes, and factory text literals. The 80
`norrath_construct_<Actor>` constructors are out of scope.
