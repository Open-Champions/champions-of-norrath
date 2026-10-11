# Lights and non-floor render state

Package notes for `agents/render-lights`. Addresses are from the private
hash-pinned `SLUS_205.65` symbol table and the reviewed EE decoder. They are
local evidence, not values reproduced by the relocatable build. Host contracts
are not plan inputs. `input_order` values assume the merged plan on this
branch: 346 inputs, orders 0 through 345, 344 authored plus two optional
external inputs. The integrator renumbers when other packages land.

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

## Original names from main and launchSRS

The official partial link of the merged plan, before these units, had 344
authored inputs, 443 function bodies, and 647 unresolved imports. These
sixteen were unresolved and are the whole render-lights set in that list:
`displaySOELogo__Fv`, `displaySBLogoScreen__Fv`, `engineSetScreenColor__Fiiii`,
`textureFreeCacheBuffer__Fv`, `textureAllocateCacheBuffer__Fv`, `defaultFont`,
`mediumFont`, `smallFont`, and `haloTexture1` through `haloTexture8`.

None shares an address or extent with an object already authored here. The
halo tag remains the five bytes at `0x485470`. The clear-color word remains
`0x48667c`. The border returns remain `0x191ba8` and `0x191bb0`. Each name
below is therefore a new body, not a second symbol on an existing one.

| Original name | Address | Image extent | Compiled body |
|---|---|---|---|
| `engineSetScreenColor__Fiiii` | `0x1910c0` | 28 | 40 |
| `textureFreeCacheBuffer__Fv` | `0x172558` | 48 | 68 |
| `textureAllocateCacheBuffer__Fv` | `0x172588` | 88 | 104 |
| `displaySBLogoScreen__Fv` | `0x2d0cc8` | 128 | 220 |
| `displaySOELogo__Fv` | `0x2cd7d8` | 188 | 284 |
| `defaultFont` | `0x4844b0` | 4 | 4-byte BSS zero |
| `mediumFont` | `0x484ca0` | 4 | 4-byte BSS zero |
| `smallFont` | `0x484ca4` | 4 | 4-byte BSS zero |
| `haloTexture1` | `0x4844bc` | 4 | 4-byte BSS zero |
| `haloTexture2` | `0x484f68` | 4 | 4-byte BSS zero |
| `haloTexture3` | `0x484f6c` | 4 | 4-byte BSS zero |
| `haloTexture4` | `0x484f70` | 4 | 4-byte BSS zero |
| `haloTexture5` | `0x484f74` | 4 | 4-byte BSS zero |
| `haloTexture6` | `0x484f78` | 4 | 4-byte BSS zero |
| `haloTexture7` | `0x484f7c` | 4 | 4-byte BSS zero |
| `haloTexture8` | `0x484f80` | 4 | 4-byte BSS zero |

`engineSetScreenColor__Fiiii` adds 128 to red, green, and blue and stores
those words in `compositeR`, `compositeG`, and `compositeB` at `0x48442c`,
`0x484430`, and `0x484434`. Each file word is `0x00000080`. The fourth
argument is not stored. `compositeAlpha` at `0x484428` is the previous word.
`vblankCount` begins at `0x484438`.

`textureFreeCacheBuffer__Fv` loads `uncompCacheAlloc` at `0x4840a4`. A null
word returns without a free and without clearing `uncompCacheBuffer` at
`0x4840a8`. A non-null word is passed to `memFree__FPv`, and only then are
those two words cleared. `uncompCache` at `0x486974` is left unchanged.
`decodeQHead` begins four bytes later. `textureAllocateCacheBuffer__Fv` frees
a non-null allocation without clearing the words first, then calls
`memAlloc__FiPc` with size `0x500100` and the label `textureCacheBuffer`. It
stores the returned pointer, that address plus 127 masked with `~127`, and
the aligned address plus 112. A null allocation stores 0, 0, and 112.

Both logo functions return 0 or 1 even though the symbol spelling does not
encode a return type. `displaySBLogoScreen__Fv` finds `snowlogo.tex` in
`sblogo.lmp` and draws it with arguments 0, 0, 3 and color `0x80808080`.
`displaySOELogo__Fv` finds `soe_splash_url.tex` and `soe_splash.tex` in
`soelogo.lmp`, then draws them at 192, 330, 3 and 64, 50, 3 with the same
color. Those rodata strings have no ELF symbol, so each function carries its
own copy. After the draws, `frameNm` at `0x4844a4` is compared with 301. At
or above 301 the function returns 1 and does not write `skipIntro`. Below
301, a nonzero little-endian word at `pad+100` or `pad+12` stores 1 in
`skipIntro` and returns 1. Both words zero returns 0. `pad` is the 352-byte
object at `0x34beb8`. `skipIntro` is the word at `0x4866c0`. The next symbol
after `haloTexture8` begins at `0x484f88`; the pointer claimed here is the
4-byte symbol.

## Private build

A private copy of this branch's plan, kept outside git, accepted all 26
units. The driver reported 370 authored inputs, one link, 451 function
bodies, and 639 unresolved imports. Every name under Imports closed was
absent from that list. `runtime_verified` and `game_executable` were false.
`python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

Snapshot entries need `engineSetScreenColor__Fiiii` at 40 bytes,
`textureFreeCacheBuffer__Fv` at 68, `textureAllocateCacheBuffer__Fv` at 104,
`displaySBLogoScreen__Fv` at 220, `displaySOELogo__Fv` at 284,
`norrath_game_line_add` at 172, each border function at 8, and an empty
function list for the other units.

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
- `engineSetScreenColor__Fiiii`
- `textureFreeCacheBuffer__Fv`
- `textureAllocateCacheBuffer__Fv`
- `displaySBLogoScreen__Fv`
- `displaySOELogo__Fv`
- `defaultFont`
- `mediumFont`
- `smallFont`
- `haloTexture1`
- `haloTexture2`
- `haloTexture3`
- `haloTexture4`
- `haloTexture5`
- `haloTexture6`
- `haloTexture7`
- `haloTexture8`

`compositeR`, `compositeG`, and `compositeB` are defined with the screen-color
function. `uncompCacheAlloc`, `uncompCacheBuffer`, and `uncompCache` are
defined with the cache-free function. They were not imports of the official
link.

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
- `frameNm`
- `textureDrawColor__FP8_textureiiiUi`

The fifteen shared `norrath_vt_*_10GameObject` slot names are referenced by
the light vtables and resolve against the merged object-infra objects. They
are not unresolved in this private link. `lumpFindResource__FPcT0`,
`memAlloc__FiPc`, `memFree__FPv`, `pad`, and `skipIntro` stay unresolved and
were already unresolved before these units. `norrath_block_alloc` was already
defined.

## Build-plan entries needed

```json
{
  "header_closure": [],
  "input_order": 346,
  "kind": "independently_authored_source",
  "object_name": "screen_color.o",
  "profile": "authored_cpp",
  "sha256": "23912196c0e735f74e731294e8d13fe64873afa47a45b6f55e944b9c8926c1e2",
  "source": "sources/render-lights-screen-color/screen_color.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 347,
  "kind": "independently_authored_source",
  "object_name": "texture_cache_free.o",
  "profile": "authored_cpp",
  "sha256": "37f4767df00967a45015fce9945da0e76624dae1b40e4cb52927b7f4f02337cc",
  "source": "sources/render-lights-texture-cache-free/texture_cache_free.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 348,
  "kind": "independently_authored_source",
  "object_name": "texture_cache_allocate.o",
  "profile": "authored_cpp",
  "sha256": "53274369f0d4a9627b6fb31f9d8567ac9283c07eabdbcc08a1a2984b00043290",
  "source": "sources/render-lights-texture-cache-allocate/texture_cache_allocate.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 349,
  "kind": "independently_authored_source",
  "object_name": "sb_logo.o",
  "profile": "authored_cpp",
  "sha256": "eb51d13e2161b9df1b6588a75e23791dd888e901d830f11e07c8c3ba74b996ff",
  "source": "sources/render-lights-sb-logo/sb_logo.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 350,
  "kind": "independently_authored_source",
  "object_name": "soe_logo.o",
  "profile": "authored_cpp",
  "sha256": "e78546d38dd5e7be041b6dccf28ec8b108f719c81d70ab4682bc0d52f0750cf3",
  "source": "sources/render-lights-soe-logo/soe_logo.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 351,
  "kind": "independently_authored_source",
  "object_name": "default_font.o",
  "profile": "authored_cpp",
  "sha256": "aa410f57e1715bbe8b2f2b41e209f4b6115c60898c52084a217b6e517000dfea",
  "source": "sources/render-lights-default-font/default_font.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 352,
  "kind": "independently_authored_source",
  "object_name": "medium_font.o",
  "profile": "authored_cpp",
  "sha256": "9f0d032f620bbc4ba0e2e336dd28dc5897eb7a7d65ee8fc3fae7256a63d62fb3",
  "source": "sources/render-lights-medium-font/medium_font.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 353,
  "kind": "independently_authored_source",
  "object_name": "small_font.o",
  "profile": "authored_cpp",
  "sha256": "5e351ae3032ffa721fa33140e748ca8e8192f853b99d5533a61bf613575e2b00",
  "source": "sources/render-lights-small-font/small_font.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 354,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_1.o",
  "profile": "authored_cpp",
  "sha256": "72b60c378ab3152bb7e91a101870ad3249a3ee31d19a7e5e6108d7301a2085ed",
  "source": "sources/render-lights-halo-texture-1/halo_texture_1.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 355,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_2.o",
  "profile": "authored_cpp",
  "sha256": "fc1930e6bf31fb38026b4e22796a1a6f2a1bcdcfaa9dc7b0ac84479a6de307e5",
  "source": "sources/render-lights-halo-texture-2/halo_texture_2.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 356,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_3.o",
  "profile": "authored_cpp",
  "sha256": "a1d75389ef0451b65b1dbca68750b0b0f0b18579eb529d8bd85198d5cb9cadc0",
  "source": "sources/render-lights-halo-texture-3/halo_texture_3.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 357,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_4.o",
  "profile": "authored_cpp",
  "sha256": "a20947693952be34d975a85b2faf5dbd6964ceda85eb3c662dba38f23eb5990d",
  "source": "sources/render-lights-halo-texture-4/halo_texture_4.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 358,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_5.o",
  "profile": "authored_cpp",
  "sha256": "bf5148ca30de40a7892e02a8e9545944f62817fa6cdf98211065fde69f5784f8",
  "source": "sources/render-lights-halo-texture-5/halo_texture_5.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 359,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_6.o",
  "profile": "authored_cpp",
  "sha256": "21aaca50161c1c2ac0b9cc7eae442d42d6525bf7163d573c7cc8eb54ef77f909",
  "source": "sources/render-lights-halo-texture-6/halo_texture_6.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 360,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_7.o",
  "profile": "authored_cpp",
  "sha256": "9e86288a999e0aaa81f89ac84849dcf2e6936da86f69eeb33ae441b1c07c1dde",
  "source": "sources/render-lights-halo-texture-7/halo_texture_7.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 361,
  "kind": "independently_authored_source",
  "object_name": "halo_texture_8.o",
  "profile": "authored_cpp",
  "sha256": "e72cf596d9aa7c005b2a61c36fce951cfae7232e65e3933ce688b635cdf4311f",
  "source": "sources/render-lights-halo-texture-8/halo_texture_8.cpp"
}
```

```json
{
  "header_closure": [],
  "input_order": 362,
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
  "input_order": 363,
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
  "input_order": 364,
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
  "input_order": 365,
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
  "input_order": 366,
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
  "input_order": 367,
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
  "input_order": 368,
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
  "input_order": 369,
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
  "input_order": 370,
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
  "input_order": 371,
  "kind": "independently_authored_source",
  "object_name": "cosmetic_vtable.o",
  "profile": "authored_cpp",
  "sha256": "3f27f5c03c3eebed21bf0d48dcf8df5e54eda276ed8da17239e51a32c90b5e26",
  "source": "sources/render-lights-cosmetic-vtable/cosmetic_vtable.cpp"
}
```

## Authored but never executed

The line insertion, its count word, the 100 line slots, both border returns,
the clear-color word, both tag strings, the four vtables, the screen-color
function and its three channel words, both texture-cache functions and their
three pointers, both logo functions, and the font and halo pointers. Host
contracts passed. No PS2 or game execution. The vtable slots are not called.
The line constructor, `textureDrawColor__FP8_textureiiiUi`, and the logo
resource lookup are not executed.

## Unknown

- Whether a count at or above 100 is a real game input. The add does not test
  the 400-byte span, and `gameLines` has no symbol size.
- The original count, font pointers, halo pointers, and cache allocation words
  are file-backed `.sdata` zeros. `uncompCache` is an `.sbss` zero. Under
  `-G0` those zeros land in BSS. The nonzero channel words are `.sdata` in
  the executable and `.data` here.
- The dynamic-light bytes are `.rodata` in the executable and writable here.
  Whether the original program's case fold succeeds on that mapping is not
  established. Bytes after each NUL are not part of these objects.
- The border functions do not clip, store, or restore a color. Why the
  executable contains only the return is unknown.
- Which vtable offset callers use. The stored words are the observed slots.
- Why the screen-color function ignores its fourth argument, and whether a
  later reader treats the stored channels as signed.
- Whether an allocation address in the last 127 bytes of the 32-bit space is
  a real input. The executable wraps that addition in 32 bits.
- The logo strings have no ELF symbol. These functions carry separate copies
  of the same bytes. Whether `lumpFindResource__FPcT0` writes through those
  pointers is unknown.
- What `simpleDrawLoop__FPFv_i` does with the 0 or 1 returned by the logos.
- `norrath_has_light_or_particle_number`, `norrath_factory_prop_vtable`,
  `norrath_factory_animation_vtable`, and every `norrath_floor_*` import
  remain unresolved and were not taken by this package.
- Interrupts, timing, GP preservation, and gameplay remain unverified.
- The 172-byte line body is not an original-byte match of the 164-byte
  function. The screen-color, cache, and logo bodies are not original-byte
  matches of their image extents.
