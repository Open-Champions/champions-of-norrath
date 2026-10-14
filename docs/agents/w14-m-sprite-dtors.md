# w14-m-sprite-dtors: sprite destructors, type-info nodes and vtables

Branch `agents/w14-m-sprite-dtors`. One commit per symbol (35 commits). Sources are under
`experimental/sources/w14-m-sprite-dtors-*`; each unit has the authored source and a `host_check.cpp`
(not a plan input). Addresses and sizes come from the private `SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`, re-checked before extraction). No
bytes, disassembly or image data are committed. Nothing here ran on the EE or in the game.

## Result

All 35 names in this package are defined by the new objects (checked with `nm` on the 35 compiled units). Each
name has the evidence listed below. The partial link was not re-run; the import closure below is an object-level
check against the existing plan build output.

## Units

| symbol | image address | image size | unit directory | how it was checked |
|---|---|---|---|---|
| `__tf9VIHSprite` | `0x01203348` | 80 | `tf-9vihsprite` | paired, 8 scenarios |
| `__tf9VICSprite` | `0x012081f8` | 80 | `tf-9vicsprite` | paired, 8 scenarios |
| `__tf11VILODSprite` | `0x01206fa0` | 80 | `tf-11vilodsprite` | paired, 8 scenarios |
| `__tf12VINameSprite` | `0x01207d68` | 80 | `tf-12vinamesprite` | paired, 8 scenarios |
| `__tf12VIPointLight` | `0x0120bce0` | 80 | `tf-12vipointlight` | paired, 8 scenarios |
| `__tf12VISkinSprite` | `0x01226910` | 80 | `tf-12viskinsprite` | paired, 8 scenarios |
| `__tf13VIFloraSprite` | `0x011f3df8` | 80 | `tf-13viflorasprite` | paired, 8 scenarios |
| `__tf13VIGroupSprite` | `0x011f6430` | 80 | `tf-13vigroupsprite` | paired, 8 scenarios |
| `__tf13VIPointSprite` | `0x0120c7d8` | 80 | `tf-13vipointsprite` | paired, 8 scenarios |
| `__tf13VISoundSprite` | `0x01227800` | 80 | `tf-13visoundsprite` | paired, 8 scenarios |
| `__tf14VISimpleSprite` | `0x01226770` | 80 | `tf-14visimplesprite` | paired, 8 scenarios |
| `__tf16VIParticleSprite` | `0x01207fe0` | 80 | `tf-16viparticlesprite` | paired, 8 scenarios |
| `__tf19VIStreamAudioSprite` | `0x011f5290` | 80 | `tf-19vistreamaudiosprite` | paired, 8 scenarios |
| `__tf20VIEffectVolumeSprite` | `0x012265e0` | 80 | `tf-20vieffectvolumesprite` | paired, 8 scenarios |
| `_$_11VILODSprite` | `0x010767a8` | 100 | `dtor-11vilodsprite` | paired, 18 scenarios |
| `_$_12VINameSprite` | `0x010a1050` | 108 | `dtor-12vinamesprite` | paired, 18 scenarios |
| `_$_12VIPointLight` | `0x010c39a0` | 100 | `dtor-12vipointlight` | paired, 18 scenarios |
| `_$_12VISkinSprite` | `0x0116d458` | 100 | `dtor-12viskinsprite` | paired, 18 scenarios |
| `_$_13VIFloraSprite` | `0x01006220` | 100 | `dtor-13viflorasprite` | paired, 18 scenarios |
| `_$_13VIGroupSprite` | `0x0101db30` | 100 | `dtor-13vigroupsprite` | paired, 18 scenarios |
| `_$_13VIPointSprite` | `0x010cb828` | 100 | `dtor-13vipointsprite` | paired, 18 scenarios |
| `_$_13VISoundSprite` | `0x0117c240` | 100 | `dtor-13visoundsprite` | paired, 18 scenarios |
| `_$_14VISimpleSprite` | `0x01166f88` | 100 | `dtor-14visimplesprite` | paired, 18 scenarios |
| `_$_16VIParticleSprite` | `0x010aa6b0` | 116 | `dtor-16viparticlesprite` | paired, 18 scenarios |
| `_$_19VIStreamAudioSprite` | `0x01018f00` | 100 | `dtor-19vistreamaudiosprite` | paired, 18 scenarios |
| `_$_20VIEffectVolumeSprite` | `0x0114e6f0` | 116 | `dtor-20vieffectvolumesprite` | paired, 18 scenarios |
| `_$_7VILList` | `0x010727a0` | 44 | `dtor-7villist` | paired, 18 scenarios |
| `_$_9VICSprite` | `0x010b2748` | 116 | `dtor-9vicsprite` | paired, 18 scenarios |
| `_$_9VIHSprite` | `0x010633c0` | 188 | `dtor-9vihsprite` | paired, 18 scenarios |
| `_$_11VILightning` | `0x0106fcc0` | 164 | `dtor-11vilightning` | paired, 32 scenarios |
| `_vt$8VISprite` | `0x011f4d10` | 96 | `vt-8visprite` | image slot comparison |
| `_vt$8VIEnvRay` | `0x012267d8` | 24 | `vt-8vienvray` | image slot comparison |
| `_vt$13VISceneFilter` | `0x01222d10` | 24 | `vt-13viscenefilter` | image slot comparison |
| `_vt$13VIActorFilter` | `0x012018e0` | 24 | `vt-13viactorfilter` | image slot comparison |
| `_vt$19__builtin_type_info` | `0x004538c8` | 24 | `vt-19builtin_type_info` | image slot comparison |

Sizes are the image symbol sizes. The authored units compile with the EE g++ to 92 bytes per type-info function and
32 to 192 bytes per destructor. Compiled size is not a byte match and no byte-identity is claimed.

## What the functions do

Observed from the image, with inference marked:

- **Type-info (`__tf<cls>`).** Returns the 12-byte node `__ti<cls>`. A nonzero first word of the node returns at
  once. A zero word calls the base class's `__tf` (`__tf8VISprite` for all but `9VICSprite`, which uses
  `__tf9VIHSprite`), then calls `__rtti_si(node, "<cls>", base node)`. Inferred: the nodes are runtime type
  descriptors and the chain is single inheritance (the name strings and the registration shape say so; no runtime
  code consumes them in this unit).
- **Deleting destructor (`_$_<cls>`).** Stores the class table at object+60 first. `9VICSprite`, `12VINameSprite`
  and `9VIHSprite` then call their `Clear__` function. Then it runs the sub-object and member destructors in image
  order with count 2 (for example `_$_10VIMatrix44` at +320 in `9VIHSprite`), calls the base destructor
  `_$_8VISprite` (or `_$_9VIHSprite` for `9VICSprite`) with flag 0, then tail-calls `__builtin_delete(object)` when
  bit 0 of the flags is set. `_$_7VILList` only frees.
- **`_$_11VILightning`.** Compares the object pointer with the sign-extended value -112. When equal, the array loop
  is skipped. Otherwise it destroys four 1552-byte elements, at object+6256, +4704, +3152 and +1600, stopping when
  the pointer reaches object+112. Then `_$_10VIMatrix44(object+32, 2)`, then the optional free. The meaning of the
  -112 value is unknown (see below).
- **Vtables (`_vt$<cls>`).** The image stores 8-byte slots: a zero adjustment word, then a function word
  (only the second word is a pointer; the layout matches the sibling `_vt$9VICSprite` unit in
  `w13-m-startup-data-b`). The first function word is the type-info function (`__tf<cls>`), then the virtual
  functions in slot order, then a null slot. `_vt$8VISprite` has 12 slots (10 virtual functions), the four
  filter and ray tables have 3 slots each.

## Checks performed

- **Private image identity.** SHA-256 of `work/private/SLUS_205.65` checked before extraction; symbols from
  `mips64r5900el-ps2-elf-nm -S` and disassembly from `objdump` on the private file (kept outside git).
- **EE compile.** All 35 units compile with the reviewed EE g++ (GCC 15.2.0, `work/toolchains/ee`) using the
  `authored_cpp` flags, the same argument list as the `commands.json` record of an authored unit in the shared
  build (checked against `w13-m-startup-data-b-vt-vilod`). 35 of 35 compile.
- **Native host checks.** Each unit's `host_check.cpp` builds with `c++ -std=c++17 -Wall -Wextra -Werror` and
  runs with stubbed callees. Type-info units: a zero node registers, a nonzero node skips work, for four
  first-word patterns. Destructors: six flag values, call order and offsets, and the table store. Vtables: slot
  count and null last slot (host pointers are wider, so the EE layout is checked separately below). 35 of 35 pass.
- **Paired R5900 runs.** A private interpreter (kept outside git) runs the original image function and the
  authored function from identical memory with identical callee hooks, and compares call logs, return values and
  changed memory. Results: type-info, 14 units, 8 scenarios each (node set or zero, four first-word patterns),
  0 mismatches. Destructors, 15 units, 18 scenarios each (nine flag values, two fills), 0 mismatches.
  `_$_11VILightning`, 32 scenarios (eight flag values, object and -112 pointer, two fills), 0 mismatches. No
  HALT was reported. Coverage of the original instruction words (the harness's count of executed words over the
  symbol's word-aligned span): every type-info unit 20 of 20; destructors 10 to 47 of 11 to 47 (`_$_7VILList`
  10 of 11, `_$_9VIHSprite` 47 of 47; the others 24 to 29 of 25 to 29); `_$_11VILightning` 40 of 41. The words that
  were not executed were not classified.
- **Mutation checks.** Each family was checked with a deliberate wrong variant, which the comparison caught:
  `__tf13VISoundSprite` with an extra node write (4 mismatches) and with the base call removed (4 mismatches);
  `_$_11VILODSprite` with the flag mask changed (10 mismatches); `_$_9VICSprite` with the array count changed
  (18 mismatches); `_$_11VILightning` with the sentinel changed from -112 to -111 (16 mismatches).
- **Vtable image comparison.** For each of the 24 slots in the five tables, the adjustment word is zero, the
  function word equals the image address of the symbol named in the source, and the last slot is null. This used
  the private symbol table and reads the image bytes. 0 mismatches.
- **Symbol definitions.** All 35 names are defined by the new objects (`nm` on the compiled units).
- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

Correction for the record: the commit messages of the fourteen type-info commits say "20 node-state scenarios".
The paired run for each type-info unit used 8 scenarios; the "20" is the harness's coverage count. The
destructor commit messages and the `_$_11VILightning` message give the scenario counts above.

## Imports closed

Names defined by this package's objects (35): the 14 `__tf...` type-info functions, the 16 `_$_...` destructors,
and the 5 vtables. All 35 were in the baseline unresolved list (`work/plan14-build/unresolved-imports.json`).

`_$_11VILODSprite`, `_$_11VILightning`, `_$_12VINameSprite`, `_$_12VIPointLight`, `_$_12VISkinSprite`,
`_$_13VIFloraSprite`, `_$_13VIGroupSprite`, `_$_13VIPointSprite`, `_$_13VISoundSprite`, `_$_14VISimpleSprite`,
`_$_16VIParticleSprite`, `_$_19VIStreamAudioSprite`, `_$_20VIEffectVolumeSprite`, `_$_7VILList`, `_$_9VICSprite`,
`_$_9VIHSprite`, `__tf11VILODSprite`, `__tf12VINameSprite`, `__tf12VIPointLight`, `__tf12VISkinSprite`,
`__tf13VIFloraSprite`, `__tf13VIGroupSprite`, `__tf13VIPointSprite`, `__tf13VISoundSprite`, `__tf14VISimpleSprite`,
`__tf16VIParticleSprite`, `__tf19VIStreamAudioSprite`, `__tf20VIEffectVolumeSprite`, `__tf9VICSprite`,
`__tf9VIHSprite`, `_vt$13VIActorFilter`, `_vt$13VISceneFilter`, `_vt$19__builtin_type_info`, `_vt$8VIEnvRay`,
`_vt$8VISprite`.

## Imports opened

Names this package references that no authored unit in the tree defines. They are unresolved in the partial
build after these objects are added. Each needs an owner or a decision by the integrator:

- Base class: `_$_8VISprite` (base destructor), `__tf8VISprite` (base type-info), `__ti8VISprite` (base node).
  No package in the wave list names them.
- Type-info of the filter and ray classes: `__tf8VIEnvRay`, `__tf13VISceneFilter`, `__tf13VIActorFilter`,
  `__tf19__builtin_type_info`.
- Virtual functions named in `_vt$8VISprite` and the filter/ray tables: `Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi`,
  `Collide__8VISpriteR12VICollSphereR13VISceneFilterR7VISceneR9VICollide`, `Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster`,
  `Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei`, `Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide`,
  `Collide__8VIEnvRayUifR7VIVect3`, `FilterActor__13VISceneFilterR7VIScenei`, `FilterActor__13VIActorFilterR7VIScenei`.
  (`GetInstType__C8VISpriteR7VIScene` is owned by `w14-sprite-raster`; the ray `Collide__8VISpriteR9VICollRay...` by
  `w14-sprite-collide`; `QueryEmitters__8VISprite...` and `QueryIntersection__8VISprite...` by `w14-sprite-pick`.
  They are unresolved in the existing plan build and stay with those owners.)
- Destructors and helpers called from the destructors: `Clear__9VICSprite`, `Clear__9VIHSprite`,
  `_$_14VIEffectVolume`, `_$_t6VIPool1Z13VIHSpritePlay`, `_$_t6VIPool1Z17VIHSpritePlayNode`,
  `_$_t6VIPool1Z19VIHSpriteAttachment`, `_$_t7VIArray1Z16VIHSpriteTrigger`, `_$_t7VIArray1Z20VICSpritePartEmitter`,
  `_$_t8VIVector1Z10VIMatrix44`, `_$_t8VIVector1Z13VIHSpriteNode`, `_$_19__builtin_type_info`.
  (`_$_10VIMatrix44`, `_$_17VIParticleEmitter` and `Clear__12VINameSprite` are referenced by other packages'
  units, which define them or leave them open.)
- Class tables defined by other packages (references only, no new symbol): `_vt$11VILODSprite`, `_vt$9VICSprite`,
  `_vt$9VIHSprite`, `_vt$12VIPointLight`, `_vt$12VISkinSprite`, `_vt$12VINameSprite`, `_vt$13VIFloraSprite`,
  `_vt$13VIGroupSprite`, `_vt$13VIPointSprite`, `_vt$13VISoundSprite`, `_vt$14VISimpleSprite`,
  `_vt$16VIParticleSprite`, `_vt$19VIStreamAudioSprite`, `_vt$20VIEffectVolumeSprite`. Their defining units are
  `w13-m-startup-data-b-vt-*` (for example `vt_vilod.cpp`); the constructor units only reference them.

## Build-plan entries needed

These entries are for the lead integrator. They are not in `experimental/build-plan.json`, and
`experimental/snapshot.json` must record the same inputs before `--check-plan` accepts them. The `input_order`
values come from the integrator. Every entry uses the `authored_cpp` profile and an empty header closure. The
`emitted_data` key is not in the shared plan format; the integrator should decide how data symbols (the
type-info nodes `__ti...` and the `_vt$...` tables) are recorded. The source hashes match the committed files.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_11vilightning.o",
  "profile": "authored_cpp",
  "sha256": "7f02bd0fddab1cb43fed3154977810329857bb950a9a4afa5438984dc53750c9",
  "source": "sources/w14-m-sprite-dtors-dtor-11vilightning/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_11VILightning",
    "compiled_body_bytes": 160
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_11vilodsprite.o",
  "profile": "authored_cpp",
  "sha256": "52fac862d11c41ba6e1aa3baa152c045d495fb574e3d991d836f7722a58c513e",
  "source": "sources/w14-m-sprite-dtors-dtor-11vilodsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_11VILODSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_12vinamesprite.o",
  "profile": "authored_cpp",
  "sha256": "d60b7fd0addb2e07c98d75ddac053d8c9a512eb5160f53b1847302bef80c4679",
  "source": "sources/w14-m-sprite-dtors-dtor-12vinamesprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_12VINameSprite",
    "compiled_body_bytes": 112
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_12vipointlight.o",
  "profile": "authored_cpp",
  "sha256": "649b412650fde0d8864dbb7631709d58b4740d69962addfcc9657d334aca3c15",
  "source": "sources/w14-m-sprite-dtors-dtor-12vipointlight/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_12VIPointLight",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_12viskinsprite.o",
  "profile": "authored_cpp",
  "sha256": "553c8063b1109c78fc4f9cfd63f6ec54ee3d58e63c15b7d9d6b53ea31878df31",
  "source": "sources/w14-m-sprite-dtors-dtor-12viskinsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_12VISkinSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_13viflorasprite.o",
  "profile": "authored_cpp",
  "sha256": "cdacee568f12880c50380c590dbc8dd40c7a43910202c4ffaab24175042de1de",
  "source": "sources/w14-m-sprite-dtors-dtor-13viflorasprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13VIFloraSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_13vigroupsprite.o",
  "profile": "authored_cpp",
  "sha256": "bb4f8f7a84c90e8df1571a6865edf7c84ee0c0a039332bb0d9afbbe7cc4c735f",
  "source": "sources/w14-m-sprite-dtors-dtor-13vigroupsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13VIGroupSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_13vipointsprite.o",
  "profile": "authored_cpp",
  "sha256": "388b19e024b91d290a5c85f042f4ff22091c11367b4552293dae03c4000df8d8",
  "source": "sources/w14-m-sprite-dtors-dtor-13vipointsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13VIPointSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_13visoundsprite.o",
  "profile": "authored_cpp",
  "sha256": "577f4ee8f7278424bf2a99cbbd3479c9dd38fd986a4405d0cf286f80387c3076",
  "source": "sources/w14-m-sprite-dtors-dtor-13visoundsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_13VISoundSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_14visimplesprite.o",
  "profile": "authored_cpp",
  "sha256": "dec77b989e31a5bbfa50d866e27e5c559c2e39e678d8c092c9c3d0dca6e19793",
  "source": "sources/w14-m-sprite-dtors-dtor-14visimplesprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_14VISimpleSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_16viparticlesprite.o",
  "profile": "authored_cpp",
  "sha256": "d2a51b004df6b4a1d328cc48b524cecd81a527dfa35942b64a7013f9b23f3c1b",
  "source": "sources/w14-m-sprite-dtors-dtor-16viparticlesprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_16VIParticleSprite",
    "compiled_body_bytes": 120
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_19vistreamaudiosprite.o",
  "profile": "authored_cpp",
  "sha256": "0758038896dee414590d7e7bf2c2ef9c8f32885631700286f0414d8f49c8ac1b",
  "source": "sources/w14-m-sprite-dtors-dtor-19vistreamaudiosprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_19VIStreamAudioSprite",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_20vieffectvolumesprite.o",
  "profile": "authored_cpp",
  "sha256": "65040875e697a597a8a5167799e99795152121fff126721cf9f95d3259ccb0c5",
  "source": "sources/w14-m-sprite-dtors-dtor-20vieffectvolumesprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_20VIEffectVolumeSprite",
    "compiled_body_bytes": 120
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_7villist.o",
  "profile": "authored_cpp",
  "sha256": "227c5ea20fd34cb69c37edb34674df257e3c4fe9dd17accee71fb1155b7bd91c",
  "source": "sources/w14-m-sprite-dtors-dtor-7villist/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_7VILList",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_9vicsprite.o",
  "profile": "authored_cpp",
  "sha256": "5086a35716d9df96283261fc1644648637e3949f4b62314eacb2ee79bca4a73d",
  "source": "sources/w14-m-sprite-dtors-dtor-9vicsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_9VICSprite",
    "compiled_body_bytes": 120
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_dtor_9vihsprite.o",
  "profile": "authored_cpp",
  "sha256": "3a56aff08daa77b220b15bd6eb8f1e7afdb885ace9c8c3fb7e7173b69c71f62b",
  "source": "sources/w14-m-sprite-dtors-dtor-9vihsprite/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_9VIHSprite",
    "compiled_body_bytes": 192
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_11vilodsprite.o",
  "profile": "authored_cpp",
  "sha256": "d55776e460f6d4268f31db0104ad14ca97a1c4dbaa03bd00135608bca0289ee6",
  "source": "sources/w14-m-sprite-dtors-tf-11vilodsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf11VILODSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti11VILODSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_12vinamesprite.o",
  "profile": "authored_cpp",
  "sha256": "348bd1036c1182637cda464d5ac9fe22d2ee75499cdbf04ee02b60589e67b5a3",
  "source": "sources/w14-m-sprite-dtors-tf-12vinamesprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf12VINameSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti12VINameSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_12vipointlight.o",
  "profile": "authored_cpp",
  "sha256": "204c577187f0b0db238ef50143143aa900b7b2bc5d11ef55a7d6dc0b3491c2d2",
  "source": "sources/w14-m-sprite-dtors-tf-12vipointlight/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf12VIPointLight",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti12VIPointLight",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_12viskinsprite.o",
  "profile": "authored_cpp",
  "sha256": "24e064c970223e95fa228868906bc033024be8bd81dc98f19b38f083fb95d230",
  "source": "sources/w14-m-sprite-dtors-tf-12viskinsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf12VISkinSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti12VISkinSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_13viflorasprite.o",
  "profile": "authored_cpp",
  "sha256": "d0b8359adc48d0cd063e92c78ce601dbf580d60aafec23c881e89ddaa750444b",
  "source": "sources/w14-m-sprite-dtors-tf-13viflorasprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13VIFloraSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti13VIFloraSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_13vigroupsprite.o",
  "profile": "authored_cpp",
  "sha256": "e58658ad4003a19e83df33417b459f9be8ef69cac213bad28c7fcf482f3d44e3",
  "source": "sources/w14-m-sprite-dtors-tf-13vigroupsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13VIGroupSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti13VIGroupSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_13vipointsprite.o",
  "profile": "authored_cpp",
  "sha256": "846f251a1121de87c41d6e6c4c08203f360bcafa6401ae2ebb0a2dc154258277",
  "source": "sources/w14-m-sprite-dtors-tf-13vipointsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13VIPointSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti13VIPointSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_13visoundsprite.o",
  "profile": "authored_cpp",
  "sha256": "d5ada2cb081cc5875fa87f7b1ad43b62696eb0797cef6617dec679b6afebb123",
  "source": "sources/w14-m-sprite-dtors-tf-13visoundsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf13VISoundSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti13VISoundSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_14visimplesprite.o",
  "profile": "authored_cpp",
  "sha256": "0739250e56c6ee8ecc58d257056e5b9f8d5643df51dedf0cc55854a4ec52c52d",
  "source": "sources/w14-m-sprite-dtors-tf-14visimplesprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf14VISimpleSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti14VISimpleSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_16viparticlesprite.o",
  "profile": "authored_cpp",
  "sha256": "365667d82e6c7b5d8bf1d5732823603607b702aa97ef9041346938fe0485ce1a",
  "source": "sources/w14-m-sprite-dtors-tf-16viparticlesprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf16VIParticleSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti16VIParticleSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_19vistreamaudiosprite.o",
  "profile": "authored_cpp",
  "sha256": "61f911d866d64ec1eac46ac6b0da95f8a2424b1350652fbc969b370d7ff4511f",
  "source": "sources/w14-m-sprite-dtors-tf-19vistreamaudiosprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf19VIStreamAudioSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti19VIStreamAudioSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_20vieffectvolumesprite.o",
  "profile": "authored_cpp",
  "sha256": "f47d9ccfa93f2bca9c75a31c06f130e87ed6177f9a314e3c72c04c12fb045d9e",
  "source": "sources/w14-m-sprite-dtors-tf-20vieffectvolumesprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf20VIEffectVolumeSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti20VIEffectVolumeSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_9vicsprite.o",
  "profile": "authored_cpp",
  "sha256": "ac2d9e08b821712af25ec5de5ab3afe4804adc45b390af3a5c77a00a35810ea6",
  "source": "sources/w14-m-sprite-dtors-tf-9vicsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9VICSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti9VICSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_tf_9vihsprite.o",
  "profile": "authored_cpp",
  "sha256": "31ecddcd486227cbb0891e65cd6028bbb3b2b51301daec1a173979e39defcd28",
  "source": "sources/w14-m-sprite-dtors-tf-9vihsprite/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9VIHSprite",
    "compiled_body_bytes": 92
   }
  ],
  "emitted_data": [
   {
    "name": "__ti9VIHSprite",
    "bytes": 12
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_vt_13viactorfilter.o",
  "profile": "authored_cpp",
  "sha256": "af2e90393365f763ad895fc06a6ef960196b010fb521ac830837d87142b9cedf",
  "source": "sources/w14-m-sprite-dtors-vt-13viactorfilter/vt.cpp",
  "emitted_functions": [],
  "emitted_data": [
   {
    "name": "_vt$13VIActorFilter",
    "bytes": 24
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_vt_13viscenefilter.o",
  "profile": "authored_cpp",
  "sha256": "687648c2445485911c0760bee59d097b1df35217ae99736c21b922e25d52a3c9",
  "source": "sources/w14-m-sprite-dtors-vt-13viscenefilter/vt.cpp",
  "emitted_functions": [],
  "emitted_data": [
   {
    "name": "_vt$13VISceneFilter",
    "bytes": 24
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_vt_19builtin_type_info.o",
  "profile": "authored_cpp",
  "sha256": "baab17065831122dd9d8cdf00c45773ed1d9f01bf426870a89aa79bf0dea0436",
  "source": "sources/w14-m-sprite-dtors-vt-19builtin_type_info/vt.cpp",
  "emitted_functions": [],
  "emitted_data": [
   {
    "name": "_vt$19__builtin_type_info",
    "bytes": 24
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_vt_8vienvray.o",
  "profile": "authored_cpp",
  "sha256": "b8b02fe71e267225442ed930304185f4eecafd7e90167ca8695ed64e762951e5",
  "source": "sources/w14-m-sprite-dtors-vt-8vienvray/vt.cpp",
  "emitted_functions": [],
  "emitted_data": [
   {
    "name": "_vt$8VIEnvRay",
    "bytes": 24
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14mst_vt_8visprite.o",
  "profile": "authored_cpp",
  "sha256": "305c7c3c905f389fefc459a77760ce6cb0d88f68ab71cea0b53e6f092f83b2e7",
  "source": "sources/w14-m-sprite-dtors-vt-8visprite/vt.cpp",
  "emitted_functions": [],
  "emitted_data": [
   {
    "name": "_vt$8VISprite",
    "bytes": 96
   }
  ]
 }
]
```

## Authored but never executed

- Nothing in this package ran on the EE, in the game, or in any emulator run of the whole program.
- The paired runs executed the authored functions in the private interpreter with callees replaced by hooks. The
  hooks record their arguments and, for `__rtti_si` and the type-info base, set or return the node values. They do
  not reproduce what the real callees do: the image's `__rtti_si` and `__tf8VISprite` are not run, and
  `__builtin_delete`, `Clear__...`, the matrix, pool, vector and array destructors, and `_$_17VIParticleEmitter`,
  `_$_14VIEffectVolume` and `_$_8VISprite` are hooks.
- The host checks are authored checks with their own expected call sequences. They ran natively with clang.
- The vtables are checked only against the image bytes and symbol names. The functions they name are not in this
  package and were not run.

## Unknown

- The meaning of the -112 comparison in `_$_11VILightning`. The authored code compares the 64-bit sign-extended
  value and keeps the image's behaviour for that value. It is not known what the value stands for, or whether any
  caller ever passes it.
- The element type and layout of the 1552-byte array in `_$_11VILightning`. The callee is `_$_10VIMatrix44`, but
  the array's owner and the reason for the count of four were not established.
- The reason `_$_9VICSprite` calls `Clear__9VICSprite` before destroying its array and base. The order is from the
  image; the reason is not known.
- What `__rtti_si` stores in a node beyond the first word, and whether the registration chains match what the game's
  dynamic-cast code expects. Nothing here runs that code.
- Linkage. Every `__tf...` and `_$_...` symbol in the image of this group is weak or global as listed; the authored
  definitions are ordinary global functions. The object-level check found no clash, but the partial-link policy
  for weak symbols was not tested.
- The contents of `_vt$19__builtin_type_info` beyond its three slots, and whether its two functions
  (`__tf19__builtin_type_info`, `_$_19__builtin_type_info`) belong to this package or to the C++ runtime. They are
  listed as opened imports.
- Whether any other sprite-class destructor in the image calls these destructors through a different name. The
  scan covered the 16 symbols in this package only.
- The full link. The import closure was not re-derived from a fresh partial build of the shared plan with these
  entries added; the notes above use the object-level check and the existing plan build output at
  `work/plan14-build`.
