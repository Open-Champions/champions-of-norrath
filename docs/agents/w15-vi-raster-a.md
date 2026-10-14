# w15-vi-raster-a: raster, font, surface cache, material and primbuffer (1 of 2)

Branch `agents/w15-vi-raster-a`. One commit per function, each pushed. Source is under `experimental/sources/w15-vi-raster-a-*`; every
directory holds the authored source (with an Observed / Inferred / Unknown comment) and a `host_check.cpp` (host headers, not a plan
input). Addresses and sizes come from the pinned private `SLUS_205.65` read locally with the reviewed EE binutils; no executable bytes,
disassembly listing or decompiler output is committed. Compiled sizes are those of the reviewed EE g++ with the `authored_cpp` flags and
are not claims of byte identity (most bodies differ in size and schedule from the image).

## Result

All 34 assigned names are authored, compiled and paired against the original. A link (`ld -m elf32lr5900 -r`) of the 34 objects has no
duplicate definition. `build_partial.py --check-plan` accepts a private plan (outside git, `work-w15-vi-raster-a-plan`) with the 34 entries
added: 6,797 selected inputs. The full per-unit driver build was not run for this package (the shared tree is large and other agents
share the machine); the check above is the plan check plus an object link, not the driver's inventory comparison.

## Units

| unit | symbol | image address | image bytes | compiled bytes |
|---|---|---|---|---|
| `w15-vi-raster-a-font-drawstring` | `DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii` | `0x01011c90` | 180 | 244 |
| `w15-vi-raster-a-font-getheight` | `GetHeight__6VIFontP8VIRasterP12VIDictionary` | `0x01011f90` | 124 | 144 |
| `w15-vi-raster-a-material-process` | `Process__10VIMateriall` | `0x010ceb70` | 264 | 324 |
| `w15-vi-raster-a-material-constructtextpass` | `ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord` | `0x010cedf8` | 1192 | 800 |
| `w15-vi-raster-a-material-constructsolidfillpass` | `ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord` | `0x010cf2a0` | 784 | 468 |
| `w15-vi-raster-a-material-constructwirepass` | `ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord` | `0x010cf5b0` | 368 | 288 |
| `w15-vi-raster-a-raster-calcclip` | `CalcClipToScreen__8VIRaster` | `0x01111c68` | 252 | 228 |
| `w15-vi-raster-a-raster-createsurface` | `CreateSurface__8VIRasteri` | `0x01113730` | 152 | 152 |
| `w15-vi-raster-a-raster-setsolidfilluimaterial` | `SetSolidFillUIMaterial__8VIRaster` | `0x01113948` | 80 | 64 |
| `w15-vi-raster-a-raster-materialpal` | `MaterialPal__8VIRasteri` | `0x01113ac8` | 28 | 28 |
| `w15-vi-raster-a-raster-sharematerialpal` | `ShareMaterialPal__8VIRasteri` | `0x01113ae8` | 44 | 44 |
| `w15-vi-raster-a-raster-createfont` | `CreateFont__8VIRasteri` | `0x01113bd0` | 140 | 152 |
| `w15-vi-raster-a-raster-sharefont` | `ShareFont__8VIRasteri` | `0x01113c60` | 44 | 44 |
| `w15-vi-raster-a-raster-font` | `Font__8VIRasteri` | `0x01113c90` | 28 | 28 |
| `w15-vi-raster-a-raster-gettime` | `GetTime__8VIRaster` | `0x01113e80` | 8 | 8 |
| `w15-vi-raster-a-raster-pickprimbuffer-ii` | `PickPrimBuffer__8VIRasterR13VIPickSegmentii` | `0x0111d380` | 532 | 500 |
| `w15-vi-raster-a-raster-pickprimbuffer-iii` | `PickPrimBuffer__8VIRasterR13VIPickSegmentiii` | `0x0111d598` | 424 | 444 |
| `w15-vi-raster-a-eessurfcache-decache` | `Decache__13VIEESurfCacheR9VISurface` | `0x01132418` | 240 | 232 |
| `w15-vi-raster-a-eessurfcache-calccacheinfo` | `CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo` | `0x01132508` | 756 | 808 |
| `w15-vi-raster-a-eessurfcache-set` | `Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass` | `0x01132800` | 800 | 676 |
| `w15-vi-raster-a-eessurfcache-findlru` | `FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster` | `0x01132bb8` | 320 | 324 |
| `w15-vi-raster-a-eessurfcache-allocate` | `Allocate__13VIEESurfCacheR13VIEESurfAlloc` | `0x01132df8` | 596 | 568 |
| `w15-vi-raster-a-eessurfcache-movetolruend` | `MoveToLRUEnd__13VIEESurfCachei` | `0x011335f0` | 380 | 208 |
| `w15-vi-raster-a-eessurfcache-removefromlru` | `RemoveFromLRU__13VIEESurfCachei` | `0x01133770` | 284 | 136 |
| `w15-vi-raster-a-eessurfcache-download` | `Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag` | `0x01133d18` | 100 | 96 |
| `w15-vi-raster-a-rfont-init` | `Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi` | `0x01135e40` | 304 | 296 |
| `w15-vi-raster-a-rfont-ispalettized` | `IsPalettized__7VIRFont` | `0x01136008` | 12 | 12 |
| `w15-vi-raster-a-rfont-pixelstore` | `PixelStore__7VIRFont` | `0x01136018` | 8 | 8 |
| `w15-vi-raster-a-rfont-lock` | `Lock__7VIRFontR8VIRaster` | `0x01136028` | 32 | 32 |
| `w15-vi-raster-a-rfont-unlock` | `Unlock__7VIRFontR8VIRaster` | `0x01136048` | 60 | 56 |
| `w15-vi-raster-a-rfont-palette` | `Palette__7VIRFontRiR12VIPixelStorePPUc` | `0x01136088` | 48 | 48 |
| `w15-vi-raster-a-rfont-beginchar` | `BeginChar__7VIRFontUsRiPPUc` | `0x011360b8` | 60 | 60 |
| `w15-vi-raster-a-rfont-endchar` | `EndChar__7VIRFont` | `0x011360f8` | 20 | 20 |
| `w15-vi-raster-a-skinsprite-setmaterialpal` | `SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary` | `0x0116d600` | 120 | 104 |

## How each unit was checked

* Compiled with the reviewed EE g++ and the plan's `authored_cpp` flags, one unit at a time.
* Native host check (`c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp`, exit 0) with stubbed callees. Objects store
  32-bit target addresses, so units that follow pointer words use the arena convention of earlier packages (`NORRATH_HOST_ARENA`,
  `NORRATH_WORD_TO_POINTER`). The two `VIFont` string routines' host checks silence `-Wunused-but-set-variable` for the function-local
  `ResourceID` word, which is written and never read (as in the image).
* Paired run in a private R5900 interpreter (outside git, with the shared `SLUS_205.65` loaded): the original function and the freshly
  compiled authored function run from identical memory snapshots with identical Python callee stubs that log their arguments; call logs,
  return values and every changed byte outside the stack must match, a halted run counts as a failure, and original-word coverage is
  reported. Coverage is full except for the image's padding nops and branch-delay leftovers. `CalcCacheInfo` and `Set` (surface cache)
  and `ConstructTextPass` ran 500+ randomized scenarios; the rest ran a scenario list that covers each branch.
* Mutation: every unit had at least one deliberate edit (a constant, offset, comparison sense, call or operand order, stored value, or a
  skipped re-read after a call) and the paired run failed each time. Mutations were applied to code, not to the header comment, which
  repeats the same text; the first attempts that hit the comment and survived were redone against code-only strings.
* Two things the paired runs cannot show: the function-local `ResourceID` store in `GetHeight` and `DrawString` is compared by value
  through a post hook (the authored static lives elsewhere than the image's `0x011e283c` / `0x011e282c`), and the static
  `_13VIEESurfCache$Key` is bound to the image address `0x011b0254` in the paired run (it is an undefined import in the authored object).

## Observed, inferred, unknown (short form; the source comments are the record)

* Observed: the raster owns four 12-byte-entry tables used here: surfaces (pointer at +19388, pool at +19380), fonts (pool at +19508,
  entry pointer at +19516), material palettes (entry pointer at +19436), prim buffers (pointer at +19328); each entry's first word is an
  object pointer whose word at +4 is incremented by the share routines. The EE surface cache keeps a bucket pool at cache+32 (24-byte
  buckets, entry array pointer at cache+40), an LRU entry array pointer at cache+64 (104-byte entries, first/last indices at cache+8/+12,
  links at entry+92/+96) and a multimap at cache+76.
* Observed: `Set` and the three material pass builders write GS register values (TEX0, TEX1, MIPTBP1/2, TEST, ALPHA, CLAMP, ZBUF) as
  64-bit value / register-number pairs; the numeric encodings are recorded in each source comment.
* Inferred: the field names in all comments, that the surface cache info record is the layout `CalcCacheInfo` fills and `Set` consumes,
  that the pass builders' quadword 5-8 copy in `ConstructTextPass` forwards the four entries `Set` stores in the pass at +80..+143, and the
  GS register interpretation of the encoded constants.
* Unknown: every callee body listed under "Imports opened", the layout of `VIPickSegment` (the local copy is at least 92 bytes; 96 is
  reserved), the `VIPrimBuffer` mode names, the meaning of `kind` (pass record +12) beyond the alpha choice, the asymmetric height
  rounding in `CalcCacheInfo`, and who reads `ResourceID`. No unit ran on the EE or in the game.

## Imports closed

`Allocate__13VIEESurfCacheR13VIEESurfAlloc`, `BeginChar__7VIRFontUsRiPPUc`, `CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo`, `CalcClipToScreen__8VIRaster`, `ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord`, `ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord`, `ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord`, `CreateFont__8VIRasteri`, `CreateSurface__8VIRasteri`, `Decache__13VIEESurfCacheR9VISurface`, `Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag`, `DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii`, `EndChar__7VIRFont`, `FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster`, `Font__8VIRasteri`, `GetHeight__6VIFontP8VIRasterP12VIDictionary`, `GetTime__8VIRaster`, `Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi`, `IsPalettized__7VIRFont`, `Lock__7VIRFontR8VIRaster`, `MaterialPal__8VIRasteri`, `MoveToLRUEnd__13VIEESurfCachei`, `Palette__7VIRFontRiR12VIPixelStorePPUc`, `PickPrimBuffer__8VIRasterR13VIPickSegmentii`, `PickPrimBuffer__8VIRasterR13VIPickSegmentiii`, `PixelStore__7VIRFont`, `Process__10VIMateriall`, `RemoveFromLRU__13VIEESurfCachei`, `SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary`, `SetSolidFillUIMaterial__8VIRaster`, `Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass`, `ShareFont__8VIRasteri`, `ShareMaterialPal__8VIRasteri`, `Unlock__7VIRFontR8VIRaster`.

## Imports opened

These 36 names are referenced by the new units and are neither defined by the shared selection plus this package nor in the
`work/plan15-build/unresolved-imports.json` list at the start of the task (another package of the wave may supply some of them):
`AddToFreeMap__13VIEESurfCachei`, `Add__t6VIPool1ZP7VIRFontRCP7VIRFont`, `Add__t6VIPool1ZP9VISurfaceRCP9VISurface`, `Allocate__7VIRFont`, `CalcDataSize__7VIRFontPUc`, `CalcMaxWidth__7VIRFontiPUc`, `ColorBuffer__8VIRasteri`, `ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag`, `CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi`, `Cull__8VIRasterR13VIPickSegmentR6VIBBox`, `Deallocate__13VIEESurfCacheR13VIEESurfAlloc`, `DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs`, `Erase__t10VIMultiMap2ZUiZii`, `Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi`, `FindFree__13VIEESurfCacheR13VIEESurfAlloc`, `GetPointSize__8VIRasteri`, `GetZBufferReg__C8VIRaster`, `InitChars__7VIRFontPUc`, `InitSlots__7VIRFontR8VIRaster`, `Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44`, `Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi`, `Key__Ct10VIMultiMap2ZUiZii`, `Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi`, `Log2__13VIEESurfCachei`, `Next__Ct6VIPool1Z10VIEEBucketi`, `PickPrimBufferPackVUNBW__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`, `PickPrimBufferPackVUNCG__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`, `PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIColorBufferR13VIMaterialPal`, `PickPrimBufferPackVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`, `PickPrimBufferVUNC__8VIRasterR13VIPickSegmentR12VIPrimBufferR13VIMaterialPal`, `PrimBuffer__8VIRasteri`, `ResetCache__7VIRFontR8VIRaster`, `_13VIEESurfCache$Key`, `__13VIPickSegment`, `__7VIRFont`, `__9VISurface`.

Already unresolved or satisfied elsewhere and used here: `FindTyped__12VIDictionaryUi14VIResourceTypeRi`, `VIHashResourceID`,
`_6VIFont$sm_FontName`, `ReleaseMaterialPal__8VIRasteriR12VIDictionary` (unresolved before this package; `_6VIFont$sm_FontName` is in
`w15-vi-raster-b`'s scope); `AddMaterial__8VIRasteriPP10_sceDmaTag`, `FlushDMABufferIfFull__8VIRaster`, `Identity__10VIMatrix44`,
`Invert__10VIMatrix44RC10VIMatrix44`, `Surface__8VIRasteri` and `__builtin_new` (defined by the shared plan).

## Build-plan entries needed

Append these 34 entries (assign fresh `input_order` values and recompute file digests, the `default_authored_inputs` count and the
snapshot digest). `docs/agents/w15-vi-raster-a-plan-entries.py --toolchain /path/to/ee [--private-root DIR]` regenerates them and writes a
private plan and snapshot copy. All have an empty `header_closure`.

```json
{"emitted_functions": [{"compiled_body_bytes": 568, "name": "Allocate__13VIEESurfCacheR13VIEESurfAlloc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_allocate.o", "profile": "authored_cpp", "sha256": "0fb76107dda6a992b31422f1879b7f83eee1e915a8d01feae35f20519541f3f9", "source": "sources/w15-vi-raster-a-eessurfcache-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 808, "name": "CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_calccacheinfo.o", "profile": "authored_cpp", "sha256": "14d2ef8b16b918adb31113701eed535bab60972fac28e2e8e14f52f0fcc959a9", "source": "sources/w15-vi-raster-a-eessurfcache-calccacheinfo/calc_cache_info.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 232, "name": "Decache__13VIEESurfCacheR9VISurface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_decache.o", "profile": "authored_cpp", "sha256": "55a8ab6816218ec1d631505b6cf3c4f58d8b34f15ef6c7d021e8cdccd670b63c", "source": "sources/w15-vi-raster-a-eessurfcache-decache/decache.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_download.o", "profile": "authored_cpp", "sha256": "65af2610556a0ba8d0a39c89c8c00dfd0b368a5f244c7f6d37feee106529d4fc", "source": "sources/w15-vi-raster-a-eessurfcache-download/download.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 324, "name": "FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_findlru.o", "profile": "authored_cpp", "sha256": "1239426d2fbb04520a922a521ec2734b1c7f06b6d648f1971bc8ad5310839a8f", "source": "sources/w15-vi-raster-a-eessurfcache-findlru/find_lru.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 208, "name": "MoveToLRUEnd__13VIEESurfCachei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_movetolruend.o", "profile": "authored_cpp", "sha256": "dc1ace64a548dfd28f5dfdb0419f736b21febd5946f32aed500eedaa453b00df", "source": "sources/w15-vi-raster-a-eessurfcache-movetolruend/move_to_lru_end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "RemoveFromLRU__13VIEESurfCachei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_removefromlru.o", "profile": "authored_cpp", "sha256": "3508a0f89a802a94dc29584f9bf87c6f2ae5a377f496b4c719bec751b2cabe4d", "source": "sources/w15-vi-raster-a-eessurfcache-removefromlru/remove_from_lru.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 676, "name": "Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_eessurfcache_set.o", "profile": "authored_cpp", "sha256": "e0ef272bd21a27c0ddddd45d1f309766aea441be1cf8df15e844f4befeab27c7", "source": "sources/w15-vi-raster-a-eessurfcache-set/set.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 244, "name": "DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_font_drawstring.o", "profile": "authored_cpp", "sha256": "8d1c20abf43969895de4c6f40e261717f0229877637ae1df41f0065cabd6989f", "source": "sources/w15-vi-raster-a-font-drawstring/drawstring.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "GetHeight__6VIFontP8VIRasterP12VIDictionary"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_font_getheight.o", "profile": "authored_cpp", "sha256": "42ca2d9444f4220f81ac7fc54b8700b11eb641816735be129d0d22ab86038c7e", "source": "sources/w15-vi-raster-a-font-getheight/getheight.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 468, "name": "ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_material_constructsolidfillpass.o", "profile": "authored_cpp", "sha256": "e2348f720629043071ecf12eac60861764ec385299d9dd9b80abbcb0a8677435", "source": "sources/w15-vi-raster-a-material-constructsolidfillpass/construct_solid_fill_pass.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 800, "name": "ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_material_constructtextpass.o", "profile": "authored_cpp", "sha256": "21c164014d177549f23e8fbb104de649eb466cc343dda3cc177d9d5170c41e08", "source": "sources/w15-vi-raster-a-material-constructtextpass/construct_text_pass.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 288, "name": "ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_material_constructwirepass.o", "profile": "authored_cpp", "sha256": "43bb77403169b730a47ae9c0fb96d239c30a54ce2c995e4cfdb6fd518d791b41", "source": "sources/w15-vi-raster-a-material-constructwirepass/construct_wire_pass.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 324, "name": "Process__10VIMateriall"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_material_process.o", "profile": "authored_cpp", "sha256": "02601f425e2f287f662876418830641150c9d33ed7469f3929d83332a8adc9ef", "source": "sources/w15-vi-raster-a-material-process/process.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 228, "name": "CalcClipToScreen__8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_calcclip.o", "profile": "authored_cpp", "sha256": "9a2c28086641af952e7abdd993e81fd3b28f1359ba69e161be7b23e24bb9d8fc", "source": "sources/w15-vi-raster-a-raster-calcclip/calc_clip_to_screen.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "CreateFont__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_createfont.o", "profile": "authored_cpp", "sha256": "c749fe414f63bc23354cc1ffeaa334cbadf9d3c3a7ee9454f27e709f94d56369", "source": "sources/w15-vi-raster-a-raster-createfont/create_font.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "CreateSurface__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_createsurface.o", "profile": "authored_cpp", "sha256": "c50847fe30604a32fc50300f19fdbb1d5ebaaf06d228d56ca724730063277657", "source": "sources/w15-vi-raster-a-raster-createsurface/create_surface.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Font__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_font.o", "profile": "authored_cpp", "sha256": "f3ace898ee3795c92e3111718b806ceb69e702083508a0573a25f2fbe8ad8446", "source": "sources/w15-vi-raster-a-raster-font/font.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "GetTime__8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_gettime.o", "profile": "authored_cpp", "sha256": "e39919ee296c1a92e08d8a1e3a6d1ac5f24b43b95b3e3bcfb0c7e6dbe7a9c4bc", "source": "sources/w15-vi-raster-a-raster-gettime/gettime.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "MaterialPal__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_materialpal.o", "profile": "authored_cpp", "sha256": "1d5fd2e031d4a48db26fa178ffba178bf777616aa119a4d290e9b8c0de046785", "source": "sources/w15-vi-raster-a-raster-materialpal/material_pal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 500, "name": "PickPrimBuffer__8VIRasterR13VIPickSegmentii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_pickprimbuffer_ii.o", "profile": "authored_cpp", "sha256": "2d9ba78ec1a1b0e569a4d54538b4094c3c3789f3cf4b599c95c1eae947da2ac9", "source": "sources/w15-vi-raster-a-raster-pickprimbuffer-ii/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 444, "name": "PickPrimBuffer__8VIRasterR13VIPickSegmentiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_pickprimbuffer_iii.o", "profile": "authored_cpp", "sha256": "600f4f53fcdc87611deba3294bc0ae774971179cfbdd77a5eacb9965a56ba2e3", "source": "sources/w15-vi-raster-a-raster-pickprimbuffer-iii/pick_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "SetSolidFillUIMaterial__8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_setsolidfilluimaterial.o", "profile": "authored_cpp", "sha256": "422c52c57546a5ffc6e1fafa76ac105b0434efb94b3c5516f5a30aed8621d5b3", "source": "sources/w15-vi-raster-a-raster-setsolidfilluimaterial/set_solid_fill_ui_material.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "ShareFont__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_sharefont.o", "profile": "authored_cpp", "sha256": "79598bedd91f10a72fd4d5ac6fae92ecab3e68add24390dedcab51e3a40a548e", "source": "sources/w15-vi-raster-a-raster-sharefont/share_font.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "ShareMaterialPal__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_raster_sharematerialpal.o", "profile": "authored_cpp", "sha256": "cbaa16db2b978f01ae407c1546bbebc1fb474b778d6a6763d63955e998df0726", "source": "sources/w15-vi-raster-a-raster-sharematerialpal/share_material_pal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 60, "name": "BeginChar__7VIRFontUsRiPPUc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_beginchar.o", "profile": "authored_cpp", "sha256": "2dffe64e8a3a7351c22d7600c6980ecc7e15ad72970f3f3a925119ab5645bbba", "source": "sources/w15-vi-raster-a-rfont-beginchar/begin_char.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "EndChar__7VIRFont"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_endchar.o", "profile": "authored_cpp", "sha256": "b66abd40427cf98babbdbf72eabbe1b6187065a802bd19b29b89323312a80b3f", "source": "sources/w15-vi-raster-a-rfont-endchar/end_char.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 296, "name": "Init__7VIRFontR8VIRasterR12VIDictionaryiPUci10VIBitDepthi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_init.o", "profile": "authored_cpp", "sha256": "1c0aa1a28566bb20bbf847dae6bc80d5cfd50c80a159dfdbe36a9e2b83f1cd91", "source": "sources/w15-vi-raster-a-rfont-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "IsPalettized__7VIRFont"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_ispalettized.o", "profile": "authored_cpp", "sha256": "a2065d90f5b8ef1b2edd8980126e3a7a1cc30a2b0614db1ff11bdd66fcd439d8", "source": "sources/w15-vi-raster-a-rfont-ispalettized/is_palettized.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Lock__7VIRFontR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_lock.o", "profile": "authored_cpp", "sha256": "e244f17b9346289b7e9cde93b6552e2cc8397a01677f5597617850be3786986a", "source": "sources/w15-vi-raster-a-rfont-lock/lock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "Palette__7VIRFontRiR12VIPixelStorePPUc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_palette.o", "profile": "authored_cpp", "sha256": "ba41b364cc59d147db7602e061a1f14a51fa2d679a96058a6d2e5b10001c3307", "source": "sources/w15-vi-raster-a-rfont-palette/palette.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "PixelStore__7VIRFont"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_pixelstore.o", "profile": "authored_cpp", "sha256": "b77d188aa6604d2854866067a68eda4107913a51da07df524833598ba8736d2b", "source": "sources/w15-vi-raster-a-rfont-pixelstore/pixel_store.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "Unlock__7VIRFontR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_rfont_unlock.o", "profile": "authored_cpp", "sha256": "bbcaa95f1eeccec19628fbbc1beb5633e8de770c18f208d7f6d55b7ed855cb27", "source": "sources/w15-vi-raster-a-rfont-unlock/unlock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "SetMaterialPal__12VISkinSpriteiR8VIRasterR12VIDictionary"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15raster_a_skinsprite_setmaterialpal.o", "profile": "authored_cpp", "sha256": "19036df9cb56d0f9edcb7b607a0f70c6a6789f0974064eb2c5fe8d279e624420", "source": "sources/w15-vi-raster-a-skinsprite-setmaterialpal/set_material_pal.cpp"}
```

## Authored but never executed

All 34. They compiled and, in the private interpreter, ran against the image's own callers' data with stubbed callees, but nothing ran on
real EE hardware or in a running game. The VU1/GS interpretation of the packets is unverified. Specifically not covered by any run: the
real callees (every helper listed under "Imports opened"), hardware float rounding, and reads of the scratchpad contents set up by
other code (the paired runs write random scratchpad words).

## Unknown

* The callee bodies above, and whether the other package of this pair (`w15-vi-raster-b`) or another wave package supplies them.
* The layout and size of `VIPickSegment`, `VIEESurfAlloc`, `VIEEBucket`, `VIPrimBuffer`, `VIMaterialPal`, `VIEESurfCacheInfo` beyond the
  fields used here.
* Why `CalcCacheInfo` pads width but only height-pads when the width is larger than the minimum, and why `Allocate` gives the tail
  remainder the request's tag.
