# Lights and non-floor render state

Package notes for `agents/render-lights`. Addresses are from the private
hash-pinned `SLUS_205.65` symbol table and the reviewed EE decoder. They are
local evidence, not values reproduced by the relocatable build. Host contracts
are not plan inputs. `input_order` values assume this branch's 341-input plan
(339 authored plus two optional external inputs). The integrator renumbers
when other packages land.

## Line insertion

`gameLineAdd__FG6Point3sPPc` is 164 bytes at `0x279940`. It copies 12 point
bytes, sign-extends the halfword type, increments `gameLineCount`, and calls
`blockAlloc(24)` with that new count already stored. A null allocation is
written into the table and does not call `__4LineG6Point3sPPc`. The reviewed
compile of `norrath_game_line_add` is one 172-byte body. That is not an
original-byte match.

`gameLineCount` at `0x485ea8` is a 4-byte file zero. `needEnforce` begins at
`0x485ec0`. `gameLines` at `0x8d57a8` has a zero size field; the next symbol,
`p1`, begins at `0x8d5938`, 400 bytes later. This unit owns both objects as
BSS zeros under the reviewed `-G0` profile. The original count is file-backed
`.sdata`. The add has no capacity test.

`gameLineAdd__FP4Line` at `0x2799e8` is a different function and is not an
unresolved import of this partial build.

## Border color

`enginePushBorderColor__FUi` at `0x191ba8` and `enginePopBorderColor__Fv` at
`0x191bb0` are each 8 bytes: a return and a delay nop. The unsigned color is
not stored. No other unresolved import in this build is a border clip.
`textureDrawBorder__FiiiiiUiPcii` and `textureSmoothBorder__FiiiiiUiiii` are
present in the executable and are not imports of the partial build.

## Clear color and tags

| Authored name | Observed symbol or bytes | Address | Extent | Initial |
|---|---|---|---|---|
| `norrath_factory_clear_color_word` | `coreClearColor` | `0x48667c` | 4 | `00 00 00 80` |
| `norrath_factory_dynamic_light_tag_bytes` | bytes through the first NUL | `0x470cc8` | 13 | `DynamicLight` |
| `norrath_factory_halo_tag_bytes` | bytes through the first NUL | `0x485470` | 5 | `Halo` |

`coreLoading` begins at `0x486680`. `hasLightOrParticle__FPPc` compares 12 and
4 bytes and folds case into these two objects until NUL. The dynamic-light
bytes are in `.rodata`; the halo bytes are in `.sdata`. Both are published
writable because that walk stores. Padding and the following literals are not
part of these objects. The clear-color word is nonzero, so the reviewed
profile places it in `.data` rather than `.sdata`.

## Vtables

Each light vtable is 144 bytes in `.data`: eighteen 8-byte slots. Each slot is
a zero word followed by a function address, and the last function word is
zero. The factory stores the object's address at object+72. The reviewed
compiles emit 144 `.data` bytes and seventeen `R_MIPS_32` relocations at
offsets 4, 12, ..., 132.

| Authored name | Observed symbol | Address |
|---|---|---|
| `norrath_factory_ambient_vtable` | `_vt$17charAmbient_Light` | `0x44d258` |
| `norrath_factory_directional_vtable` | `_vt$21charDirectional_Light` | `0x44d1c8` |
| `norrath_factory_directional_d_vtable` | `_vt$22charDirectional_LightD` | `0x44d138` |
| `norrath_factory_cosmetic_vtable` | `_vt$12CosmeticProp` | `0x44ca78` |

Ambient, directional, and directional-d use the GameObject message slots and
their own type-info, destructor, and clone. Cosmetic replaces draw, load,
save, and clone; its other message slots are the GameObject ones. Shared
GameObject slot names match the object-infra vtable.

## Private build

A private copy of this branch's plan, kept outside git, accepted all ten
units. The driver reported 349 authored inputs, one link, 445 function
bodies, and 970 unresolved imports. Every name under Imports closed was absent
from that list. `norrath_block_alloc` was already defined. `runtime_verified`
and `game_executable` were false. `python3 -m unittest discover -s tools -p
'test_*.py'` passed, 214 tests.

Snapshot entries need `emitted_functions` of `norrath_game_line_add` at 172
bytes, each border function at 8 bytes, and an empty list for the seven data
units.

## Imports closed

- `norrath_game_line_add`
- `norrath_archive_engine_push_border`
- `norrath_archive_engine_pop_border`
- `norrath_factory_clear_color_word`
- `norrath_factory_dynamic_light_tag_bytes`
- `norrath_factory_halo_tag_bytes`
- `norrath_factory_ambient_vtable`
- `norrath_factory_directional_vtable`
- `norrath_factory_directional_d_vtable`
- `norrath_factory_cosmetic_vtable`

## Imports opened

- `norrath_construct_Line` for observed `__4LineG6Point3sPPc`
- `norrath_vt_tf_17charAmbient_Light`
- `norrath_vt_dtor_17charAmbient_Light`
- `norrath_vt_clone_17charAmbient_Light`
- `norrath_vt_tf_21charDirectional_Light`
- `norrath_vt_dtor_21charDirectional_Light`
- `norrath_vt_clone_21charDirectional_Light`
- `norrath_vt_tf_22charDirectional_LightD`
- `norrath_vt_dtor_22charDirectional_LightD`
- `norrath_vt_clone_22charDirectional_LightD`
- `norrath_vt_tf_12CosmeticProp`
- `norrath_vt_dtor_12CosmeticProp`
- `norrath_vt_msg_draw_12CosmeticProp`
- `norrath_vt_msg_load_12CosmeticProp`
- `norrath_vt_msg_save_12CosmeticProp`
- `norrath_vt_clone_12CosmeticProp`
- `norrath_vt_msg_hurt_10GameObject`
- `norrath_vt_msg_trigger_10GameObject`
- `norrath_vt_msg_run_10GameObject`
- `norrath_vt_msg_draw_10GameObject`
- `norrath_vt_msg_collision_10GameObject`
- `norrath_vt_msg_route_10GameObject`
- `norrath_vt_msg_load_10GameObject`
- `norrath_vt_msg_save_10GameObject`
- `norrath_vt_msg_collision_world_10GameObject`
- `norrath_vt_msg_move_collision_10GameObject`
- `norrath_vt_msg_alert_10GameObject`
- `norrath_vt_msg_get_poke_name_10GameObject`
- `norrath_vt_msg_poke_10GameObject`
- `norrath_vt_get_model_10GameObject`

The fifteen `norrath_vt_*_10GameObject` names are the shared GameObject slots.
They collapse with the object-infra vtable when that package is linked.

## Build-plan entries needed

```json
{
  "header_closure": [],
  "input_order": 341,
  "kind": "independently_authored_source",
  "object_name": "game_line_add.o",
  "profile": "authored_cpp",
  "sha256": "db14c58888bd52a9108eb9fdb9b7985cdd863df7bdce712218cfc2a93c34d035",
  "source": "sources/render-lights-line-add/game_line_add.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 342,
  "kind": "independently_authored_source",
  "object_name": "push_border.o",
  "profile": "authored_cpp",
  "sha256": "78bc4e75a76b8caa1b670468edf4843104fbcef7a5719780ba0439b910eaae4a",
  "source": "sources/render-lights-border-push/push_border.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 343,
  "kind": "independently_authored_source",
  "object_name": "pop_border.o",
  "profile": "authored_cpp",
  "sha256": "bda8547c968cbcaf8872e2396172a7364f64b3f620191153037399bc3425977a",
  "source": "sources/render-lights-border-pop/pop_border.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 344,
  "kind": "independently_authored_source",
  "object_name": "clear_color.o",
  "profile": "authored_cpp",
  "sha256": "6a33072b1453b49f1297d98522c4646525a0b444387f892f5ea603b3c21cbe68",
  "source": "sources/render-lights-clear-color/clear_color.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 345,
  "kind": "independently_authored_source",
  "object_name": "dynamic_light_tag.o",
  "profile": "authored_cpp",
  "sha256": "eee92a10c886072daa54822f2c4cfd6d7cf52eac622352314c102c8bc6b4b9fb",
  "source": "sources/render-lights-dynamic-light-tag/dynamic_light_tag.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 346,
  "kind": "independently_authored_source",
  "object_name": "halo_tag.o",
  "profile": "authored_cpp",
  "sha256": "223b08825ced003c71619909b35cd668b757012d7b099a618536ec56bb2c2918",
  "source": "sources/render-lights-halo-tag/halo_tag.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 347,
  "kind": "independently_authored_source",
  "object_name": "ambient_vtable.o",
  "profile": "authored_cpp",
  "sha256": "b1d7a54bbd3264523bedb141efeeae4775b7beaaa09451d40a8a5a1687ab5244",
  "source": "sources/render-lights-ambient-vtable/ambient_vtable.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 348,
  "kind": "independently_authored_source",
  "object_name": "directional_vtable.o",
  "profile": "authored_cpp",
  "sha256": "78316b800bb876d33394479e040417d3e876e77e90e87a5a1b7c6b19156f28b8",
  "source": "sources/render-lights-directional-vtable/directional_vtable.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 349,
  "kind": "independently_authored_source",
  "object_name": "directional_d_vtable.o",
  "profile": "authored_cpp",
  "sha256": "fea7732039891656844d643026e7e8c8631ca4ca553745bde9d398b1d72d422a",
  "source": "sources/render-lights-directional-d-vtable/directional_d_vtable.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 350,
  "kind": "independently_authored_source",
  "object_name": "cosmetic_vtable.o",
  "profile": "authored_cpp",
  "sha256": "3f27f5c03c3eebed21bf0d48dcf8df5e54eda276ed8da17239e51a32c90b5e26",
  "source": "sources/render-lights-cosmetic-vtable/cosmetic_vtable.cpp"
}
```

## Authored but never executed

The line insertion, its count word, the 100 line slots, both border returns,
the clear-color word, both tag strings, and the four vtables. Host contracts
passed. No PS2 or game execution. The vtable slots are not called. The line
constructor is not executed.

## Unknown

- Whether a count at or above 100 is a real game input. The add does not test
  the 400-byte span, and `gameLines` has no symbol size.
- The original count is file-backed `.sdata` zero. Under `-G0` the zero count
  and the line table land in BSS.
- The dynamic-light bytes are `.rodata` in the executable and writable here.
  Whether the original program's case fold succeeds on that mapping is not
  established. Bytes after each NUL are not part of these objects.
- The border functions do not clip, store, or restore a color. Why the
  executable contains only the return is unknown.
- Which vtable offset callers use. The stored words are the observed slots.
- `norrath_has_light_or_particle_number`, `norrath_factory_prop_vtable`,
  `norrath_factory_animation_vtable`, the `haloTexture*` pointers, and every
  `norrath_floor_*` import remain unresolved and were not taken by this
  package.
- Interrupts, timing, GP preservation, and gameplay remain unverified.
- The 172-byte line body is not an original-byte match of the 164-byte
  function.
