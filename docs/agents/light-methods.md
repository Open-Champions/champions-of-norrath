# Light class methods and logo draw

Package notes for `agents/light-methods`. Addresses and extents are from the
private hash-pinned `SLUS_205.65` symbol table. They are local evidence, not
values reproduced by the relocatable build. Host contracts are not plan
inputs. `input_order` values assume this branch's plan: 758 inputs, orders 0
through 757, `default_authored_inputs` 756. The integrator renumbers.

The partial build of that plan
(`work/light-methods-baseline`, 756 selected inputs, 632 function bodies,
829 unresolved imports) still listed 17 of the 18 names render-lights opened.
`frameNm` is already the 4-byte object in `frame-loop-frame-nm`.
`displaySOELogo__Fv` (188 bytes at `0x002cd7d8`) and
`displaySBLogoScreen__Fv` (128 bytes at `0x002d0cc8`) are already authored
under `render-lights-soe-logo` and `render-lights-sb-logo`. This package does
not define them again.

Each host contract below failed when the behavior it checks was wrong. Each
implementation compiled alone with the reviewed `authored_cpp` profile. None
of the compiled bodies is an original-byte match.

## Texture draw

`textureDrawColor__FP8_textureiiiUi` is 28 bytes at `0x172708`. It keeps the
five incoming arguments and returns the result of
`_textureDrawColor__FP8_textureiiiUi` (516 bytes at `0x16e658`). The reviewed
compile is one 8-byte body. The inner function stays unresolved.

## Typeinfo

Each typeinfo function is 76 bytes. Its 12-byte typeinfo object is COMMON,
section type NOBITS, so the file image is zero. The first little-endian word
gates initialization. A zero word calls `norrath_vt_tf_10GameObject` and then
`norrath_rtti_si` with this object, the class-name literal, and
`norrath_ti_10GameObject` (`__ti10GameObject`, 8 bytes at `0x00486dc8`). A
nonzero first word returns the object and makes neither call. The reviewed
compile of each function is 92 bytes. Each object is 12 BSS bytes under
`-G0`.

| Authored function | Observed symbol | Address | Typeinfo object | Address |
|---|---|---|---|---|
| `norrath_vt_tf_17charAmbient_Light` | `__tf17charAmbient_Light` | `0x30d838` | `norrath_ti_17charAmbient_Light` | `0x8eb258` |
| `norrath_vt_tf_21charDirectional_Light` | `__tf21charDirectional_Light` | `0x30d8f0` | `norrath_ti_21charDirectional_Light` | `0x8eb168` |
| `norrath_vt_tf_22charDirectional_LightD` | `__tf22charDirectional_LightD` | `0x30d9a8` | `norrath_ti_22charDirectional_LightD` | `0x8eb1a8` |
| `norrath_vt_tf_12CosmeticProp` | `__tf12CosmeticProp` | `0x30e318` | `norrath_ti_12CosmeticProp` | `0x8eb0e8` |

The class-name literals are `17charAmbient_Light`, `21charDirectional_Light`,
`22charDirectional_LightD`, and `12CosmeticProp`. `__rtti_si` is 32 bytes at
`0x2fd348` and stays unresolved as `norrath_rtti_si`.

## Destructors

Each destructor is 100 bytes. It stores the class vtable at object+72, calls
`norrath_vt_dtor_10GameObject` with flags 0, and calls `norrath_block_free`
only when bit 0 of the incoming flag is set. Other flag bits do nothing. The
reviewed compile of each function is 112 bytes.

| Authored function | Observed symbol | Address | Vtable stored |
|---|---|---|---|
| `norrath_vt_dtor_17charAmbient_Light` | `_$_17charAmbient_Light` | `0x30d7d0` | `norrath_factory_ambient_vtable` |
| `norrath_vt_dtor_21charDirectional_Light` | `_$_21charDirectional_Light` | `0x30d888` | `norrath_factory_directional_vtable` |
| `norrath_vt_dtor_22charDirectional_LightD` | `_$_22charDirectional_LightD` | `0x30d940` | `norrath_factory_directional_d_vtable` |
| `norrath_vt_dtor_12CosmeticProp` | `_$_12CosmeticProp` | `0x30e2b0` | `norrath_factory_cosmetic_vtable` |

## Clones

The three light clones are 256 bytes and allocate 76 bytes. The cosmetic
clone is 296 bytes and allocates 96 bytes. A null allocation returns null and
does not call the list-head copy. Otherwise `norrath_game_object_vtable_view`
is stored at the new object+72 before three
`norrath_list_head_copy_construct` calls at offsets 0, 4, and 8. Bytes 12..41,
44..57, and 60..71 are copied. Bytes 42, 43, 58, and 59 stay as the allocator
left them. The class vtable then replaces the GameObject vtable at offset 72.
Cosmetic also copies bytes 76..95. The reviewed light clones are 284 bytes.
The reviewed cosmetic clone is 324 bytes.

| Authored function | Observed symbol | Address |
|---|---|---|
| `norrath_vt_clone_17charAmbient_Light` | `clone__17charAmbient_Light` | `0x127280` |
| `norrath_vt_clone_21charDirectional_Light` | `clone__21charDirectional_Light` | `0x127380` |
| `norrath_vt_clone_22charDirectional_LightD` | `clone__22charDirectional_LightD` | `0x127480` |
| `norrath_vt_clone_12CosmeticProp` | `clone__12CosmeticProp` | `0x115a18` |

## Cosmetic save, load, and draw

`msg_save__12CosmeticPropPc` is 160 bytes at `0x155448`. It writes 20 bytes
and returns 20. Words at object+76, +80, and +84 go to
`objectSaveAddToStringPool__FPCc`, and those results are stored at
destination+0, +4, and +8. Destination+12 is the raw word at object+12. The
halfword at destination+16 is `cvt.w.s` of the float at object+32. The
halfword at destination+18 is `cvt.w.s` of the float at object+28.
Round-to-nearest, ties to even, is the MIPS reset mode; this function does
not change it. A result outside a signed halfword keeps its low 16 bits. The
reviewed body is 876 bytes because that conversion is inlined.

`msg_load__12CosmeticPropPc` is 216 bytes at `0x14b328`. It reads 20 bytes
and returns 20. The first three words go to
`objectSaveGetFromStringPool__Fi`, and the returned pointers are stored at
object+76, +80, and +84. The fourth word is stored at object+12. The halfword
at buffer+16 becomes the float at object+32 and its `cvt.w.s` low byte at
object+41. The halfword at buffer+18 becomes the float at object+28 and its
low byte at object+40. `lumpFindResource__FPcT0` then receives the pointers
at +76 and +80, and that result is stored at +88. A second call reloads +76
and uses +84, and that result is stored at +92. The reviewed body is 944
bytes.

`msg_draw__12CosmeticProp` is 168 bytes at `0x1554e8`. It copies the 12 bytes
at object+16, adds 16384 to the unsigned halfword at object+56, and
sign-extends that 16-bit sum. `matMakeTransYaw__FP8Matrix34G6Point3s` receives
the matrix, that point, and the sum. `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`
receives the words at object+88 and object+92, the constant 1, a second copy
of the same 12 bytes, the matrix, a zero animation pointer, `0xffffffff`, a
zero draw-record pointer, and a zero final argument. The return word is
`modelDraw`'s result. The local matrix is 64 bytes, the gap between the
matrix and the first point copy in the original frame. The reviewed body is
204 bytes.

## Line constructor

`__4LineG6Point3sPPc` is 1016 bytes at `0x278f08`. The authored name is
`norrath_construct_Line`. It copies the 12-byte point, clears words 0, 4, 12,
and 20, and sets word 8 to -1. The halfword type is the later `matRot` angle.
Arguments are walked until a null pointer. The arguments pointer itself is
not checked.

`name=` stores the address one byte after `=`. When that text compares equal
to `barrier` without case, bit 0 of word 12 is set and is not cleared by a
later name. The last `name=` replaces the stored pointer. `continous` is the
observed spelling. `continous=%d` stores its integer at word 8.
`norotate=%d` stores a local integer. Any other argument is parsed as
`pcount=%d`. A zero `sscanf` result only advances the index. A negative count
is negated as a 32-bit unsigned value, so the most negative 32-bit count stays
negative, and the following arguments are read in reverse when the stored
count is positive. Each point is 64 bytes. The first 12 bytes come from
`p=%f,%f,%f`, and the word at offset 60 is the slot index. `memAlloc__FiPc`
receives `count << 6` and the label `point list`.

After the arguments, the signed word at offset 112 of the object addressed by
`norrath_route_world_word_bytes` (the existing 4-byte `world` object) is the
megatile mode. A nonzero mode and a zero `norotate` subtract
`getMegatileOriginalCenter__FG6Point2` from each point, then add
`getMegatileCenter__FG6Point2` to the x and y produced by `matIdent`,
`matRot` axis 2, and `matTransform`. The transformed z is replaced by the
point's own z. Those four calls still run when the count is not positive.
`initializeMe__4Line` always runs, including when the argument list is empty.
The reviewed body is 2008 bytes. The host pointer round-trip is compiled only
when `NORRATH_LINE_HOST_CHECK` is set, and the EE object does not define it.

## Imports closed

- `textureDrawColor__FP8_textureiiiUi`
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
- `norrath_construct_Line`

Also defined, and not previous imports: `norrath_ti_17charAmbient_Light`,
`norrath_ti_21charDirectional_Light`, `norrath_ti_22charDirectional_LightD`,
and `norrath_ti_12CosmeticProp`. `frameNm`, `displaySOELogo__Fv`, and
`displaySBLogoScreen__Fv` were already defined.

## Imports opened

- `_textureDrawColor__FP8_textureiiiUi`
- `norrath_rtti_si` for observed `__rtti_si`
- `objectSaveAddToStringPool__FPCc`
- `objectSaveGetFromStringPool__Fi`
- `matMakeTransYaw__FP8Matrix34G6Point3s`
- `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`
- `getMegatileOriginalCenter__FG6Point2`
- `getMegatileCenter__FG6Point2`
- `matIdent__FP8Matrix33`
- `matRot__FP8Matrix33is`
- `matTransform__FP6Point3P8Matrix33T0`
- `initializeMe__4Line`

These were already unresolved and stay unresolved:
`norrath_list_head_copy_construct`, `lumpFindResource__FPcT0`, and
`memAlloc__FiPc`. The typeinfo, destructor, and clone units use the existing
`norrath_vt_tf_10GameObject`, `norrath_ti_10GameObject`,
`norrath_vt_dtor_10GameObject`, `norrath_block_alloc`, `norrath_block_free`,
`norrath_game_object_vtable_view`, and the four factory vtables. The line
constructor uses the existing `norrath_factory_strstr`,
`norrath_factory_strchr`, `norrath_factory_strcasecmp`, `sscanf`, and
`norrath_route_world_word_bytes`.

## Build-plan entries needed

Append these authored inputs. Do not add any `host_check.cpp`. Profile
`authored_cpp`, kind `independently_authored_source`, empty header closure.
The orders below assume this branch's 758-entry plan. The integrator
renumbers. `emitted_functions` is the one GLOBAL function in that object.
Each typeinfo unit also defines the named 12-byte BSS object.

```json
[
  {
    "header_closure": [],
    "input_order": 758,
    "kind": "independently_authored_source",
    "object_name": "light_methods_texture_draw_color.o",
    "profile": "authored_cpp",
    "sha256": "c86aff1f1461226e401815549208b1a77b29ea80aba84030088eb04a2e2da12d",
    "source": "sources/light-methods-texture-draw-color/texture_draw_color.cpp",
    "emitted_functions": [{"name": "textureDrawColor__FP8_textureiiiUi", "compiled_body_bytes": 8}]
  },
  {
    "header_closure": [],
    "input_order": 759,
    "kind": "independently_authored_source",
    "object_name": "light_methods_ambient_typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "2fe1185875851a94dd181bca7c5dc8b08264ca2aeeadfff19fb1c8fe6007991c",
    "source": "sources/light-methods-ambient-typeinfo/typeinfo.cpp",
    "emitted_functions": [{"name": "norrath_vt_tf_17charAmbient_Light", "compiled_body_bytes": 92}]
  },
  {
    "header_closure": [],
    "input_order": 760,
    "kind": "independently_authored_source",
    "object_name": "light_methods_directional_typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "4c67a96aa963545b75964a61b3fdc3356d657761491aacf666e0714abed874a2",
    "source": "sources/light-methods-directional-typeinfo/typeinfo.cpp",
    "emitted_functions": [{"name": "norrath_vt_tf_21charDirectional_Light", "compiled_body_bytes": 92}]
  },
  {
    "header_closure": [],
    "input_order": 761,
    "kind": "independently_authored_source",
    "object_name": "light_methods_directional_d_typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "6143a8cdea3ff96c2b2aa58a61e7a56cb33c56776127ee2497a4b0b00aa07e01",
    "source": "sources/light-methods-directional-d-typeinfo/typeinfo.cpp",
    "emitted_functions": [{"name": "norrath_vt_tf_22charDirectional_LightD", "compiled_body_bytes": 92}]
  },
  {
    "header_closure": [],
    "input_order": 762,
    "kind": "independently_authored_source",
    "object_name": "light_methods_cosmetic_typeinfo.o",
    "profile": "authored_cpp",
    "sha256": "b0283cf38c99618ee1f46fdce2f7cbeb206e73cc283880d64a43f8ae744eee36",
    "source": "sources/light-methods-cosmetic-typeinfo/typeinfo.cpp",
    "emitted_functions": [{"name": "norrath_vt_tf_12CosmeticProp", "compiled_body_bytes": 92}]
  },
  {
    "header_closure": [],
    "input_order": 763,
    "kind": "independently_authored_source",
    "object_name": "light_methods_ambient_dtor.o",
    "profile": "authored_cpp",
    "sha256": "7938a84c9e0f2dd6e9dbe5eb9a826ef811d10d91b2bd08c75f7c73ceb88237d4",
    "source": "sources/light-methods-ambient-dtor/dtor.cpp",
    "emitted_functions": [{"name": "norrath_vt_dtor_17charAmbient_Light", "compiled_body_bytes": 112}]
  },
  {
    "header_closure": [],
    "input_order": 764,
    "kind": "independently_authored_source",
    "object_name": "light_methods_directional_dtor.o",
    "profile": "authored_cpp",
    "sha256": "3a219056ea893ffde75732dad1ed4961ab22b0c6807e4a827d4dae3bb07f66cb",
    "source": "sources/light-methods-directional-dtor/dtor.cpp",
    "emitted_functions": [{"name": "norrath_vt_dtor_21charDirectional_Light", "compiled_body_bytes": 112}]
  },
  {
    "header_closure": [],
    "input_order": 765,
    "kind": "independently_authored_source",
    "object_name": "light_methods_directional_d_dtor.o",
    "profile": "authored_cpp",
    "sha256": "ec3a52c89a8299b7ed35afc26e7aab11e18726c9e12f80c40fcc4fdaa460b84b",
    "source": "sources/light-methods-directional-d-dtor/dtor.cpp",
    "emitted_functions": [{"name": "norrath_vt_dtor_22charDirectional_LightD", "compiled_body_bytes": 112}]
  },
  {
    "header_closure": [],
    "input_order": 766,
    "kind": "independently_authored_source",
    "object_name": "light_methods_cosmetic_dtor.o",
    "profile": "authored_cpp",
    "sha256": "2b3a23ec74be3e1e86ab434e8731a02e1bbfd7996b7f352fc52ca85bcca19f32",
    "source": "sources/light-methods-cosmetic-dtor/dtor.cpp",
    "emitted_functions": [{"name": "norrath_vt_dtor_12CosmeticProp", "compiled_body_bytes": 112}]
  },
  {
    "header_closure": [],
    "input_order": 767,
    "kind": "independently_authored_source",
    "object_name": "light_methods_ambient_clone.o",
    "profile": "authored_cpp",
    "sha256": "131e29db82047cb460e85410e644fca7dec6168780b44c545eef4c8c458cb677",
    "source": "sources/light-methods-ambient-clone/clone.cpp",
    "emitted_functions": [{"name": "norrath_vt_clone_17charAmbient_Light", "compiled_body_bytes": 284}]
  },
  {
    "header_closure": [],
    "input_order": 768,
    "kind": "independently_authored_source",
    "object_name": "light_methods_directional_clone.o",
    "profile": "authored_cpp",
    "sha256": "bac726d34719059e9e7dcace348b9ae81996f12e4285e5a7bda87acc95bb078c",
    "source": "sources/light-methods-directional-clone/clone.cpp",
    "emitted_functions": [{"name": "norrath_vt_clone_21charDirectional_Light", "compiled_body_bytes": 284}]
  },
  {
    "header_closure": [],
    "input_order": 769,
    "kind": "independently_authored_source",
    "object_name": "light_methods_directional_d_clone.o",
    "profile": "authored_cpp",
    "sha256": "6e2d0a621f16477c7dafebf078109663a3187b93945cf2b8f379340facd1f528",
    "source": "sources/light-methods-directional-d-clone/clone.cpp",
    "emitted_functions": [{"name": "norrath_vt_clone_22charDirectional_LightD", "compiled_body_bytes": 284}]
  },
  {
    "header_closure": [],
    "input_order": 770,
    "kind": "independently_authored_source",
    "object_name": "light_methods_cosmetic_clone.o",
    "profile": "authored_cpp",
    "sha256": "5b519215c86468979eb0325c8ff6d11eb9ebf31a26b75bc15ea3e1a154bb2732",
    "source": "sources/light-methods-cosmetic-clone/clone.cpp",
    "emitted_functions": [{"name": "norrath_vt_clone_12CosmeticProp", "compiled_body_bytes": 324}]
  },
  {
    "header_closure": [],
    "input_order": 771,
    "kind": "independently_authored_source",
    "object_name": "light_methods_cosmetic_draw.o",
    "profile": "authored_cpp",
    "sha256": "f22089a96b5503b3977096c706cad62f785c4811dc63ef17e8c09428adddc659",
    "source": "sources/light-methods-cosmetic-draw/msg_draw.cpp",
    "emitted_functions": [{"name": "norrath_vt_msg_draw_12CosmeticProp", "compiled_body_bytes": 204}]
  },
  {
    "header_closure": [],
    "input_order": 772,
    "kind": "independently_authored_source",
    "object_name": "light_methods_cosmetic_save.o",
    "profile": "authored_cpp",
    "sha256": "9a1088ae076cabfc9b4af9357a6f46918c9ac704d83cafbfe849831a96fdf40c",
    "source": "sources/light-methods-cosmetic-save/msg_save.cpp",
    "emitted_functions": [{"name": "norrath_vt_msg_save_12CosmeticProp", "compiled_body_bytes": 876}]
  },
  {
    "header_closure": [],
    "input_order": 773,
    "kind": "independently_authored_source",
    "object_name": "light_methods_cosmetic_load.o",
    "profile": "authored_cpp",
    "sha256": "36e4ede0051aaf93b0f487136418ed4bd6c96814163947c2b122a769a6ac0c5e",
    "source": "sources/light-methods-cosmetic-load/msg_load.cpp",
    "emitted_functions": [{"name": "norrath_vt_msg_load_12CosmeticProp", "compiled_body_bytes": 944}]
  },
  {
    "header_closure": [],
    "input_order": 774,
    "kind": "independently_authored_source",
    "object_name": "light_methods_line_construct.o",
    "profile": "authored_cpp",
    "sha256": "c7ba7db8a0fff244ab894f0b6c59e3162ed87a2ef509e1308ea65205b167cbf2",
    "source": "sources/light-methods-line-construct/line_construct.cpp",
    "emitted_functions": [{"name": "norrath_construct_Line", "compiled_body_bytes": 2008}]
  }
]
```

## Authored but never executed

The texture-draw wrapper, the four typeinfo functions and their 12-byte
objects, the four destructors, the four clones, the cosmetic draw, save, and
load methods, and the line constructor. Host contracts passed. No PS2 or game
execution. The inner texture draw, `__rtti_si`, the string pool, resource
lookup, model draw, yaw matrix, allocation, megatile centers, matrix helpers,
and `initializeMe__4Line` are not executed.

## Unknown

- Every reviewed body is larger or smaller than the original extent. None is
  an original-byte match. The 8-byte texture wrapper is the reviewed tail
  call of a 28-byte original. The save and load bodies are large because
  `cvt.w.s` is inlined.
- The original typeinfo objects are COMMON. Under `-G0` the 12 zero bytes
  land in BSS.
- The `Matrix34` and `Matrix33` field layouts are not established. The draw
  and line locals are 64 bytes.
- Whether a `name=` argument without `=` is a real input. The stored address
  is one byte after `strchr`.
- Whether the most negative 32-bit `pcount` is a real input. Its unsigned
  negation stays negative, so no point slots are parsed and the argument
  index moves backward.
- The arguments pointer itself is not checked. An empty list still calls
  `initializeMe__4Line`.
- Which vtable offset callers use. These functions are the slots the
  render-lights vtables already name.
- What `simpleDrawLoop` does with the logo return values. Those functions
  were already authored by render-lights.
- Interrupts, timing, GP preservation, and gameplay remain unverified.
