# Megatile agent notes

Package: world megatile system. Branch `agents/megatile`. Private executable
`work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
GP is `0x004879f0`.

The partial-build unresolved `norrath_megatile_*` set used here is the 23 names
from `work/initsys-build-InitTimer/unresolved-imports.json` (2026-10-11). It
matches the portable expanded build. A fresh `build_partial.py` of the published
plan was not rerun: `tools/build_partial.py` refuses a plan unless
`experimental/snapshot.json` already lists every new source, and that snapshot
is shared.

## specialparse

Observed `specialparse__FPPciiG6Point3Pc` at `0x00268928`, size `0x630`.
Host check: `c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror -DNORRATH_MEGATILE_HOST experimental/sources/megatile-specialparse/specialparse.cpp experimental/sources/megatile-specialparse/host_test.cpp`.
EE check uses the reviewed `authored_cpp` flags and emits only
`norrath_megatile_specialparse`.

Point is a pointer to 12 bytes. The fifth argument is the name. Trigger `w=`
and `h=` are case-sensitive prefixes. Selector 1 swaps then negates height,
selector 2 negates both, selector 3 swaps then negates width. `!!!` arguments
that are not the four dynamic forms are dropped. Closest level with mode `<= 0`
returns null before the pointer cap. Dynamic level records are 28 bytes.
`levelName` offsets 81 and 161 are interior bytes of the 322-byte view.

## do_entity_dispatch

Observed `doEntityDispatch__FP8MTObjDefiP13delayedEntityPib` at `0x002695e0`,
size `0x59c`.
Host check: `c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror -DNORRATH_MEGATILE_HOST experimental/sources/megatile-do-entity-dispatch/dispatch.cpp experimental/sources/megatile-do-entity-dispatch/host_test.cpp`.
EE check emits only `norrath_megatile_do_entity_dispatch`.

Region rows are 10 bytes. `strncmp` of the name against `MSpawn` uses length 6.
A null parse result writes one 28-byte delayed record and stores the region
kind in its type half, after first storing the combined type. Byte 24 of that
record is not written. Kitten is a case-insensitive prefix and reads the kitten
counter before incrementing it.

## tables

Zero-filled extents, confirmed against the private executable: regions
`0x140`, x `0x400`, y `0x400`, grid origin `0x18`, pm pointers `0x400`, pm
characters `0x1000`, dynlevels `0x1c0`, level name `0x142`, counters 20 bytes,
world word 4 bytes. Scale bytes are `60 0b b6 3a`. Closest-limit bytes are
`eb 78 ad 60`. Name, pm, and format objects are the NUL-terminated literals.
Alignment bytes between those literals are not part of the symbols. The pm
barrier flag literal is `continous=1`.

Host check: `c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror experimental/sources/megatile-tables/tables.cpp experimental/sources/megatile-tables/host_test.cpp`.
The EE object defines those symbols and no text.

## Build-plan entries needed

Do not add the host tests. `tools/build_partial.py` also needs these paths in
`files` and in a successor `snapshot.json` before it will accept the inputs.

```json
{
  "header_closure": ["sources/megatile-specialparse/specialparse.hpp"],
  "kind": "independently_authored_source",
  "object_name": "specialparse.o",
  "profile": "authored_cpp",
  "sha256": "1a1a3224c6799b87af6160d7c6944809efa5e39cecef2c5a22e822ec864e59d4",
  "source": "sources/megatile-specialparse/specialparse.cpp"
}
```

```json
{
  "header_closure": ["sources/megatile-do-entity-dispatch/dispatch.hpp"],
  "kind": "independently_authored_source",
  "object_name": "dispatch.o",
  "profile": "authored_cpp",
  "sha256": "9976fcd499deb22413716f79ff7fea84ad54c9e0d45fc52f43728dd429aff873",
  "source": "sources/megatile-do-entity-dispatch/dispatch.cpp"
}
```

```json
{
  "header_closure": ["sources/megatile-tables/tables.hpp"],
  "kind": "independently_authored_source",
  "object_name": "tables.o",
  "profile": "authored_cpp",
  "sha256": "beb9897dc126d9e0422f8b10eff35a95e54581dc9d6a6a1cec7a77ab3c1b47d4",
  "source": "sources/megatile-tables/tables.cpp"
}
```

Header hashes for the plan `files` map:

- `sources/megatile-specialparse/specialparse.hpp` `6e98207c17962c429d34aa9e7a0c5cca9a0986e440a00173839141b0f13272bf`
- `sources/megatile-do-entity-dispatch/dispatch.hpp` `dbf4fd3bd6a2e9b3f2db5112d8ced3bbcd2433fe5e614b07b501d3154d9614bc`
- `sources/megatile-tables/tables.hpp` `f1f29335a879b25f7016f918825b41953ed92d7cb0e78079275eafa2cd45f925`

`input_order` is assigned by the integrator. The private compile used the
reviewed `authored_cpp` flags directly because the published plan rejects new
paths.

## Imports closed

`norrath_megatile_specialparse`, `norrath_megatile_do_entity_dispatch`,
`norrath_megatile_regions_view`, `norrath_megatile_x_view`,
`norrath_megatile_y_view`, `norrath_megatile_grid_origin_view`,
`norrath_megatile_pm_pointers_view`, `norrath_megatile_pm_characters_view`,
`norrath_megatile_dynlevels_view`, `norrath_megatile_level_name_view`,
`norrath_megatile_counters_view`, `norrath_megatile_world_word`,
`norrath_megatile_scale_view`, `norrath_megatile_closest_limit_view`,
`norrath_megatile_name_a_view`, `norrath_megatile_name_b_view`,
`norrath_megatile_name_c_view`, `norrath_megatile_name_d_view`,
`norrath_megatile_name_e_view`, `norrath_megatile_name_f_view`,
`norrath_megatile_pm_name_view`, `norrath_megatile_pm_a_view`,
`norrath_megatile_pm_b_view`, `norrath_megatile_pm_c_view`,
`norrath_megatile_count_format_view`, `norrath_megatile_point_format_view`,
`norrath_megatile_trigger_text`, `norrath_megatile_width_key`,
`norrath_megatile_height_key`, `norrath_megatile_bang_text`,
`norrath_megatile_dynamic_new_world_text`, `norrath_megatile_scan_pair_format`,
`norrath_megatile_suffix_number_format`, `norrath_megatile_level_assignment_format`,
`norrath_megatile_dynamic_shift_text`, `norrath_megatile_pair_join_format`,
`norrath_megatile_decimal_format`, `norrath_megatile_dynamic_orientation_text`,
`norrath_megatile_orientation_format`, `norrath_megatile_closest_level_text`,
`norrath_megatile_from_format`, `norrath_megatile_width_format`,
`norrath_megatile_height_format`, `norrath_megatile_mspawn_text`,
`norrath_megatile_push_trigger_text`, `norrath_megatile_kitten_text`,
`norrath_megatile_kitten_name_format`.

## Imports opened

Left unresolved for other packages:

- `norrath_factory_atoi`
- `norrath_factory_ctype_minimum_view`
- `norrath_factory_sprintf`
- `norrath_factory_sscanf`
- `norrath_factory_strcasecmp`
- `norrath_factory_strchr`
- `norrath_factory_strcpy`
- `norrath_factory_strncmp`
- `norrath_factory_strstr`
- `norrath_swap_int` (two int words; observed callee is 20 bytes)
- `norrath_megatile_add_pm_list`
- `norrath_megatile_cap_pm_pointers`
- `norrath_monster_spawn`
- `norrath_object_create_by_name`
- `norrath_restore_filter_word`

`sscanf`, `strchr`, `strcpy`, `swap_int`, and `monster_spawn` are not defined
by the sources on this branch. The other names already have definitions
elsewhere in `experimental/sources`.

## Authored but never executed

The EE objects were compiled with the reviewed toolchain and were not run on
the EE. Host tests executed specialparse, entity dispatch, and the table bytes.

No host test calls specialparse or dispatch when a `(` is missing. A kitten
name combined with a null argument list is not executed; that path reads the
first parsed pointer. Region indexes other than 0 and 1, and region kinds other
than 0, 1, 2, 3, and 4, are not executed.

## Unknown

The 20 bytes after the five counter words are a gap before the next symbol and
are not part of `norrath_megatile_counters_view`. Later stores into the zero
tables belong to their callers. The filter word is read as a code address and
is owned by the restoration package. `norrath_monster_spawn` and
`norrath_object_create_by_name` stay outside this package.
