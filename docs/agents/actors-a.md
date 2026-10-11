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

## Status

`norrath_construct_Candle2` is `__7Candle2G6Point3` at `0x002310a8`, 120 bytes. It clears both base-call pointers, stores view `0x0044a930`, ORs flag `1`, writes `0xf0` at byte 40, clears bytes 76 and 77, and returns `norrath_actor_add_run_list`.

`norrath_construct_Container` is `__9ContainerG6Point3sPPc` at `0x0021d578`, 180 bytes. It passes the argument list to the base call, stores view `0x0044ccb8`, ORs `0x10`, stores the type at offset 56, `-1` at 120 and zero at 116, then calls `worldCheckForWater` with the object position, radius bits `0x41400000`, and a null output. V0 is that result; byte 124 stores only its low 8 bits.

`norrath_construct_Candle` is `__6CandleG6Point3i` at `0x002301a8`, 332 bytes. `lightAdd` sees the first range words at the light stored at object+76. Later stores replace byte 85 and the halves at 94, 96 and 98. V0 is the run-list result.

imports closed: `norrath_construct_Blocker`, `norrath_construct_Candle2`, `norrath_construct_Container`, `norrath_construct_Candle`

imports opened: `norrath_world_check_for_water` (retained `worldCheckForWater__FP12_worldHeaderG6Point3fPf`); `norrath_light_add` (retained `lightAdd__FP5Light`). Container also reads the existing `norrath_factory_world_pointer_word`.

build-plan entries needed: the Blocker, Candle2, Container, and Candle entries above.

authored but never executed: `norrath_construct_Blocker`, `norrath_construct_Candle2`, `norrath_construct_Container`, `norrath_construct_Candle` (compared against the original bodies; not run as gameplay).

unknown: the remaining A–F constructor imports, and NaN behavior of the height compare.
