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

## Status

imports closed: `norrath_construct_Blocker`

imports opened: none. Blocker uses existing `norrath_game_object_construct`, `norrath_actor_find_tag_string`, `norrath_game_object_find_tag_int`, and four address views.

build-plan entries needed: the Blocker entry above.

authored but never executed: `norrath_construct_Blocker` (compared against the original body; not run as gameplay).

unknown: the remaining A–F constructor imports, and NaN behavior of the height compare.
