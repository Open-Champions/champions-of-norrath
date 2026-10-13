# VI container template instances (3 of 3)

Package `w11-tmpl-c`, branch `agents/w11-tmpl-c`. Addresses and bytes were read from
the private pinned image `work/private/SLUS_205.65` with the reviewed EE binutils.
No game bytes or listings are committed. Each source opens with an
`Observed`/`Inferred`/`Unknown` comment. Every unit is its own commit, pushed after
the commit.

## Checks performed

- Each unit compiles with the `authored_cpp` profile (reviewed EE g++), one unit at a time;
  no full partial build was run.
- Each has `host_contract.cpp` (not a plan input) that returns nonzero on a wrong stored word
  (constructors, buffer with guard words on both sides) or a wrong call order, callee argument
  or delete decision (destructors). Run with
  `c++ -arch x86_64 -std=c++17 -Wall -Wextra -Werror -o /tmp/chk <unit>/host_contract.cpp && /tmp/chk`.
  All 27 pass.
- A private paired run (R5900 interpreter, original function vs the authored object, identical
  memory snapshots and Python stub callees) matched with zero mismatches on all 27: constructors
  over random surrounding memory, destructors over empty/nonempty storage word x flags 0..3 and a
  destroy callee that does or does not clear the storage word. Two deliberate mutants (a wrong
  pool word, a dropped vector delete) were caught, so the comparison is not vacuous.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: passed (214 tests, OK).

## Functions

| source | symbol | compiled bytes | sha256 |
|---|---|---|---|
| `w11-tmpl-c-array-clip-entry-ctor/ctor.cpp` | `__t7VIArray1Z11VIClipEntry` | 12 | `b2a75bbf57eb7c879d0a25663100894dc5cb651aa1030d01adc903e096edfddf` |
| `w11-tmpl-c-array-int-ctor/ctor.cpp` | `__t7VIArray1Zi` | 12 | `68e09d10b34422909f370abd146abb82dc457f5606a0bf866d1047cb3f1ec9e7` |
| `w11-tmpl-c-array-int-dtor/dtor.cpp` | `_$_t7VIArray1Zi` | 144 | `183c280c91971bde90cf5728671cf2d24d0b6f45f4773624c1fe35dfd560566f` |
| `w11-tmpl-c-array-plane-ctor/ctor.cpp` | `__t7VIArray1Z7VIPlane` | 12 | `a6790380eba7d1856a93759e50fa82c8ca2b0d8d0154a8d1c5206db7ee57e133` |
| `w11-tmpl-c-array-plane-dtor/dtor.cpp` | `_$_t7VIArray1Z7VIPlane` | 144 | `02a956abaff91a632769bd850c703bee26dc44f3a94ce15ce6c5fdaf47e2b225` |
| `w11-tmpl-c-array-vect3-ctor/ctor.cpp` | `__t7VIArray1Z7VIVect3` | 12 | `60aa796236cde3b93b5cbe48f58fe8432cd9bf56df9d1368aff9e36a521eb872` |
| `w11-tmpl-c-array-vect3-dtor/dtor.cpp` | `_$_t7VIArray1Z7VIVect3` | 144 | `05213d3a6e16d505ef115676daa1aed89f189dea748494db0e2e2486bb06d8ca` |
| `w11-tmpl-c-list-actor-ctor/ctor.cpp` | `__t6VIList1Z7VIActor` | 16 | `e027822bfd24468f127a6307d1fd37b730dafcce33f9367b2423a28181091c3e` |
| `w11-tmpl-c-list-material-ctor/ctor.cpp` | `__t6VIList1Z10VIMaterial` | 16 | `e17754cd3b0a6a36b17bb896a13068f72e05a295d3e23754436a6a3f1dc5c8e1` |
| `w11-tmpl-c-list-p-gui-page-ctor/ctor.cpp` | `__t6VIList1ZP9VIGUIPage` | 16 | `d08fbef4410012e3641704439524f90dff0f586eb69b89c6a08ac96e2e8c659d` |
| `w11-tmpl-c-list-p-prim-buffer-ctor/ctor.cpp` | `__t6VIList1ZP12VIPrimBuffer` | 16 | `9ccebda4e1e7bd3b8aa36c5dd7f512d6047e9a956653a58b4b2c90f5c9e8b5b1` |
| `w11-tmpl-c-list-p-surface-ctor/ctor.cpp` | `__t6VIList1ZP9VISurface` | 16 | `a77bc89befb866fd1b71c4b999555eddb823a988e58f76c26bcad58d903f2ebc` |
| `w11-tmpl-c-list-scene-occup-ctor/ctor.cpp` | `__t6VIList1ZQ27VIScene12VISceneOccup` | 16 | `62102e29d938192e77a836a7db142d145d0ef87ea7b123b216d8820c5514082f` |
| `w11-tmpl-c-map-dict-entry-ctor/ctor.cpp` | `__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry` | 28 | `6d80b319b50ec4d4a7c9c68fa1b5ffa434e99203a4f2656cff0fe6ef9de7b592` |
| `w11-tmpl-c-pool-p-coll-buffer-ctor/ctor.cpp` | `__t6VIPool1ZP12VICollBuffer` | 32 | `9e2aec6dd3a6b3089912912096a8af9d2ff6c4db9d5762fa75023b98d77fc207` |
| `w11-tmpl-c-pool-p-color-buffer-ctor/ctor.cpp` | `__t6VIPool1ZP13VIColorBuffer` | 32 | `f8e0e8625f5578590cda18d269f6a2ae5658085a6c130eaf87ba06709795f215` |
| `w11-tmpl-c-pool-p-hsprite-anim-ctor/ctor.cpp` | `__t6VIPool1ZP13VIHSpriteAnim` | 32 | `43b60affc57b3df28d5a041ff49102b0a4a2ee23a899a9eb21ecd152e8a1ded5` |
| `w11-tmpl-c-pool-p-material-pal-ctor/ctor.cpp` | `__t6VIPool1ZP13VIMaterialPal` | 32 | `3476390fb60b7be9b6cd75e6dee31cb72a44747694593478fdd401e4a71d0235` |
| `w11-tmpl-c-pool-p-prim-buffer-ctor/ctor.cpp` | `__t6VIPool1ZP12VIPrimBuffer` | 32 | `b4b40fcd5e1cfbfd6c3ff3ed5be6b576de80ff67a1cc0391859a1a9b53fac30d` |
| `w11-tmpl-c-pool-p-ref-map-ctor/ctor.cpp` | `__t6VIPool1ZP8VIRefMap` | 32 | `40d9705d7739b7ff5ecb95d9e1760c87a8e67a35c33da2302e944227094265b9` |
| `w11-tmpl-c-pool-p-rfont-ctor/ctor.cpp` | `__t6VIPool1ZP7VIRFont` | 32 | `f5974d891504cdb28c1b3822d26335469471332d696dcb12f90bd23776a634c7` |
| `w11-tmpl-c-pool-p-sprite-ctor/ctor.cpp` | `__t6VIPool1ZP8VISprite` | 32 | `e1dccf59ebc4ff9a4326a9090fee4e65ce08bb977a2795014a60c02c026fd1f1` |
| `w11-tmpl-c-pool-p-static-lighting-ctor/ctor.cpp` | `__t6VIPool1ZP16VIStaticLighting` | 32 | `57b679e64458664e154c094f46b3a4380a231bfe94761253c443c87cd88538fa` |
| `w11-tmpl-c-pool-p-surface-ctor/ctor.cpp` | `__t6VIPool1ZP9VISurface` | 32 | `a34d8c93402b7fbab27004a98a2160511901c1704b9d8327e07e0b571f341fb6` |
| `w11-tmpl-c-pool-raster-material-lru-ctor/ctor.cpp` | `__t6VIPool1Z19VIRasterMaterialLRU` | 32 | `d2470b8274182b9c35617bea3cebac22027f5fa6acaed836d6533ef5bc139d18` |
| `w11-tmpl-c-vector-scene-display-elem-ctor/ctor.cpp` | `__t8VIVector1Z18VISceneDisplayElem` | 16 | `cd2175280e47c2c176e5763792b7de1a810e916b8b65ccaa8647b753f0b3f8c1` |
| `w11-tmpl-c-vector-scene-display-elem-dtor/dtor.cpp` | `_$_t8VIVector1Z18VISceneDisplayElem` | 144 | `730421f90adb04b68c30bd9a9f1090badf2d6b2b992e400313ac73b680766716` |

## Observed

- Constructors store only zero or -1 words and call nothing. Pool instances (24-byte object):
  words at offsets 0, 4, 8 are 0 and 12, 16, 20 are -1. Map instance (`VIMap<unsigned, VIDictionary::VIDictEntry>`):
  offsets 0, 4, 8, 12 are 0 and 16 is -1. List and Vector instances: offsets 0, 4, 8 are 0.
  Array instances: offsets 0 and 4 are 0.
- Destructors: when the storage word is nonzero they call the instance's `DestroyPool__...(this)`,
  reload the word and pass it to `__builtin_vec_delete` if still nonzero; when bit 0 of the second
  argument is set they tail-call `__builtin_delete(this)`. The storage word is at offset 4 for
  `VIArray` and offset 8 for `VIVector`.
- All 27 symbols are weak template instantiations in the image.

## Inferred

- Which template each symbol instantiates and that these are default constructors/destructors,
  from the mangled names alone.
- The -1 words in pools and maps are empty-index markers (pattern only; not named).

## Unknown

- Member names for each offset, element sizes, and which class owns each instance.
- Whether any of these pools/lists are also constructed through other paths (inline copies).

## Imports closed (27)

`_$_t7VIArray1Z7VIPlane`, `_$_t7VIArray1Z7VIVect3`, `_$_t7VIArray1Zi`,
`_$_t8VIVector1Z18VISceneDisplayElem`, `__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`,
`__t6VIList1Z10VIMaterial`, `__t6VIList1Z7VIActor`, `__t6VIList1ZP12VIPrimBuffer`,
`__t6VIList1ZP9VIGUIPage`, `__t6VIList1ZP9VISurface`, `__t6VIList1ZQ27VIScene12VISceneOccup`,
`__t6VIPool1Z19VIRasterMaterialLRU`, `__t6VIPool1ZP12VICollBuffer`, `__t6VIPool1ZP12VIPrimBuffer`,
`__t6VIPool1ZP13VIColorBuffer`, `__t6VIPool1ZP13VIHSpriteAnim`, `__t6VIPool1ZP13VIMaterialPal`,
`__t6VIPool1ZP16VIStaticLighting`, `__t6VIPool1ZP7VIRFont`, `__t6VIPool1ZP8VIRefMap`,
`__t6VIPool1ZP8VISprite`, `__t6VIPool1ZP9VISurface`, `__t7VIArray1Z11VIClipEntry`,
`__t7VIArray1Z7VIPlane`, `__t7VIArray1Z7VIVect3`, `__t7VIArray1Zi`,
`__t8VIVector1Z18VISceneDisplayElem`.

## Imports opened (4, not defined here)

`DestroyPool__t7VIArray1Z7VIPlane`, `DestroyPool__t7VIArray1Z7VIVect3`,
`DestroyPool__t7VIArray1Zi`, `DestroyPool__t8VIVector1Z18VISceneDisplayElem`
(presumably other template packages' work), plus the runtime helpers `__builtin_vec_delete`
and `__builtin_delete` (may already be owned elsewhere).

## Build-plan entries needed

27 `independently_authored_source` inputs, `profile` `authored_cpp`, empty `header_closure`;
`input_order` to be assigned by the integrator. Entries (sha256 and compiled sizes as measured here):

```json
[
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_clip_entry_ctor.o",
  "profile": "authored_cpp",
  "sha256": "b2a75bbf57eb7c879d0a25663100894dc5cb651aa1030d01adc903e096edfddf",
  "source": "sources/w11-tmpl-c-array-clip-entry-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t7VIArray1Z11VIClipEntry",
    "compiled_body_bytes": 12
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_int_ctor.o",
  "profile": "authored_cpp",
  "sha256": "68e09d10b34422909f370abd146abb82dc457f5606a0bf866d1047cb3f1ec9e7",
  "source": "sources/w11-tmpl-c-array-int-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t7VIArray1Zi",
    "compiled_body_bytes": 12
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_int_dtor.o",
  "profile": "authored_cpp",
  "sha256": "183c280c91971bde90cf5728671cf2d24d0b6f45f4773624c1fe35dfd560566f",
  "source": "sources/w11-tmpl-c-array-int-dtor/dtor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "_$_t7VIArray1Zi",
    "compiled_body_bytes": 144
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_plane_ctor.o",
  "profile": "authored_cpp",
  "sha256": "a6790380eba7d1856a93759e50fa82c8ca2b0d8d0154a8d1c5206db7ee57e133",
  "source": "sources/w11-tmpl-c-array-plane-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t7VIArray1Z7VIPlane",
    "compiled_body_bytes": 12
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_plane_dtor.o",
  "profile": "authored_cpp",
  "sha256": "02a956abaff91a632769bd850c703bee26dc44f3a94ce15ce6c5fdaf47e2b225",
  "source": "sources/w11-tmpl-c-array-plane-dtor/dtor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "_$_t7VIArray1Z7VIPlane",
    "compiled_body_bytes": 144
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_vect3_ctor.o",
  "profile": "authored_cpp",
  "sha256": "60aa796236cde3b93b5cbe48f58fe8432cd9bf56df9d1368aff9e36a521eb872",
  "source": "sources/w11-tmpl-c-array-vect3-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t7VIArray1Z7VIVect3",
    "compiled_body_bytes": 12
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_array_vect3_dtor.o",
  "profile": "authored_cpp",
  "sha256": "05213d3a6e16d505ef115676daa1aed89f189dea748494db0e2e2486bb06d8ca",
  "source": "sources/w11-tmpl-c-array-vect3-dtor/dtor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "_$_t7VIArray1Z7VIVect3",
    "compiled_body_bytes": 144
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_list_actor_ctor.o",
  "profile": "authored_cpp",
  "sha256": "e027822bfd24468f127a6307d1fd37b730dafcce33f9367b2423a28181091c3e",
  "source": "sources/w11-tmpl-c-list-actor-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIList1Z7VIActor",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_list_material_ctor.o",
  "profile": "authored_cpp",
  "sha256": "e17754cd3b0a6a36b17bb896a13068f72e05a295d3e23754436a6a3f1dc5c8e1",
  "source": "sources/w11-tmpl-c-list-material-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIList1Z10VIMaterial",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_list_p_gui_page_ctor.o",
  "profile": "authored_cpp",
  "sha256": "d08fbef4410012e3641704439524f90dff0f586eb69b89c6a08ac96e2e8c659d",
  "source": "sources/w11-tmpl-c-list-p-gui-page-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIList1ZP9VIGUIPage",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_list_p_prim_buffer_ctor.o",
  "profile": "authored_cpp",
  "sha256": "9ccebda4e1e7bd3b8aa36c5dd7f512d6047e9a956653a58b4b2c90f5c9e8b5b1",
  "source": "sources/w11-tmpl-c-list-p-prim-buffer-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIList1ZP12VIPrimBuffer",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_list_p_surface_ctor.o",
  "profile": "authored_cpp",
  "sha256": "a77bc89befb866fd1b71c4b999555eddb823a988e58f76c26bcad58d903f2ebc",
  "source": "sources/w11-tmpl-c-list-p-surface-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIList1ZP9VISurface",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_list_scene_occup_ctor.o",
  "profile": "authored_cpp",
  "sha256": "62102e29d938192e77a836a7db142d145d0ef87ea7b123b216d8820c5514082f",
  "source": "sources/w11-tmpl-c-list-scene-occup-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIList1ZQ27VIScene12VISceneOccup",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_map_dict_entry_ctor.o",
  "profile": "authored_cpp",
  "sha256": "6d80b319b50ec4d4a7c9c68fa1b5ffa434e99203a4f2656cff0fe6ef9de7b592",
  "source": "sources/w11-tmpl-c-map-dict-entry-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry",
    "compiled_body_bytes": 28
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_coll_buffer_ctor.o",
  "profile": "authored_cpp",
  "sha256": "9e2aec6dd3a6b3089912912096a8af9d2ff6c4db9d5762fa75023b98d77fc207",
  "source": "sources/w11-tmpl-c-pool-p-coll-buffer-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP12VICollBuffer",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_color_buffer_ctor.o",
  "profile": "authored_cpp",
  "sha256": "f8e0e8625f5578590cda18d269f6a2ae5658085a6c130eaf87ba06709795f215",
  "source": "sources/w11-tmpl-c-pool-p-color-buffer-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP13VIColorBuffer",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_hsprite_anim_ctor.o",
  "profile": "authored_cpp",
  "sha256": "43b60affc57b3df28d5a041ff49102b0a4a2ee23a899a9eb21ecd152e8a1ded5",
  "source": "sources/w11-tmpl-c-pool-p-hsprite-anim-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP13VIHSpriteAnim",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_material_pal_ctor.o",
  "profile": "authored_cpp",
  "sha256": "3476390fb60b7be9b6cd75e6dee31cb72a44747694593478fdd401e4a71d0235",
  "source": "sources/w11-tmpl-c-pool-p-material-pal-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP13VIMaterialPal",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_prim_buffer_ctor.o",
  "profile": "authored_cpp",
  "sha256": "b4b40fcd5e1cfbfd6c3ff3ed5be6b576de80ff67a1cc0391859a1a9b53fac30d",
  "source": "sources/w11-tmpl-c-pool-p-prim-buffer-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP12VIPrimBuffer",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_ref_map_ctor.o",
  "profile": "authored_cpp",
  "sha256": "40d9705d7739b7ff5ecb95d9e1760c87a8e67a35c33da2302e944227094265b9",
  "source": "sources/w11-tmpl-c-pool-p-ref-map-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP8VIRefMap",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_rfont_ctor.o",
  "profile": "authored_cpp",
  "sha256": "f5974d891504cdb28c1b3822d26335469471332d696dcb12f90bd23776a634c7",
  "source": "sources/w11-tmpl-c-pool-p-rfont-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP7VIRFont",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_sprite_ctor.o",
  "profile": "authored_cpp",
  "sha256": "e1dccf59ebc4ff9a4326a9090fee4e65ce08bb977a2795014a60c02c026fd1f1",
  "source": "sources/w11-tmpl-c-pool-p-sprite-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP8VISprite",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_static_lighting_ctor.o",
  "profile": "authored_cpp",
  "sha256": "57b679e64458664e154c094f46b3a4380a231bfe94761253c443c87cd88538fa",
  "source": "sources/w11-tmpl-c-pool-p-static-lighting-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP16VIStaticLighting",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_p_surface_ctor.o",
  "profile": "authored_cpp",
  "sha256": "a34d8c93402b7fbab27004a98a2160511901c1704b9d8327e07e0b571f341fb6",
  "source": "sources/w11-tmpl-c-pool-p-surface-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1ZP9VISurface",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_pool_raster_material_lru_ctor.o",
  "profile": "authored_cpp",
  "sha256": "d2470b8274182b9c35617bea3cebac22027f5fa6acaed836d6533ef5bc139d18",
  "source": "sources/w11-tmpl-c-pool-raster-material-lru-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t6VIPool1Z19VIRasterMaterialLRU",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_vector_scene_display_elem_ctor.o",
  "profile": "authored_cpp",
  "sha256": "cd2175280e47c2c176e5763792b7de1a810e916b8b65ccaa8647b753f0b3f8c1",
  "source": "sources/w11-tmpl-c-vector-scene-display-elem-ctor/ctor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "__t8VIVector1Z18VISceneDisplayElem",
    "compiled_body_bytes": 16
   }
  ]
 },
 {
  "kind": "independently_authored_source",
  "object_name": "w11_tmpl_c_vector_scene_display_elem_dtor.o",
  "profile": "authored_cpp",
  "sha256": "730421f90adb04b68c30bd9a9f1090badf2d6b2b992e400313ac73b680766716",
  "source": "sources/w11-tmpl-c-vector-scene-display-elem-dtor/dtor.cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "_$_t8VIVector1Z18VISceneDisplayElem",
    "compiled_body_bytes": 144
   }
  ]
 }
]
```

## Authored but never executed

All 27 run only in host contracts and in the private interpreter pairing. None has run in a linked
build, on hardware, or in gameplay. Compiled destructors are 144 bytes against 116 in the image
(behavior reconstructions, not byte matches).

## Unknown

See the Unknown section above; also the full partial build with these inputs was not run.
