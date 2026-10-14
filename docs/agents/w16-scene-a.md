# Scene, zone, material, font, surface and raster (part 1)

Package work is on `agents/w16-scene-a`. It supplies 22 complete functions: the font loader helpers (`VIRFont`), the surface layout routines (`VISurface`), raster accessors, the raster string and surface-rectangle drawing routines, material layer access and copy, the zone-room clear, the scene reference-map constructor and the zone emitter tree walk. Each is its own commit with a host check beside the source.

All addresses are in the pinned `SLUS_205.65` image on this machine (symbol sizes from its `nm` dump). Nothing here is an exact-match claim: the compiled bodies differ in size from the originals, and the verification is behavioural (see the checks below).

## Checks performed

- Every function compiles with the reviewed EE g++ `authored_cpp` flags on its own, and its host check (`host_check.cpp`, never a build input) passes on this Mac.
- Every function was run against the original in a private R5900 interpreter harness (outside git): the original and the authored function start from identical memory snapshots with identical Python stand-ins for every callee, and the call log, the return value and every changed byte outside the stack are compared. Instruction coverage of the original is recorded per function in its commit message (the uncovered words were nops, branch-delay padding, a zero-divide trap word and one unreachable default case). Mutating the authored source (an offset, a compare, a constant, a call argument) made the comparison fail in each case, except for the equivalent rewrites named in the commit messages.
- The private harness needed one addition: the interpreter now models `div1`.
- A private copy of the snapshot and build plan with the 22 units appended (7,351 planned inputs, 7,349 selected) passed `--check-plan`, then compiled in full with the portable driver: 7,349 source compilations, one relocatable link, 5,856 function bodies, 828 unresolved imports (841 in the integrator's last private build, `plan16-build`). None of the 22 names is unresolved any more; 9 names were opened.

## Evidence

Observed, inferred and unknown are separated in each source comment. Short form of what the group shows:

- Raster object offsets read here: font table pointer at `+19516` (12-byte slots, first word the font), surface table at `+19388`, colour-buffer table at `+19364`, material list at `+19416`, texture table head at `+19380`, glyph index array at `+17900` (up to 256 ints), DMA tag slot at `+19492`, scratch material pointer at `+17848` with its cached id and blend at `+17852`/`+17856`, vertex count at `+48`, packet pointer at `+52`, viewport floats at `+17732..+17744`. `_8VIRaster$DBuffer+480` is a 64-bit word returned by `GetZBufferReg`.
- `VIRFont` fields: `+12` surface id, `+16` material id, `+20` the value `Init` passes as its `extra` (also what `GetPointSize` returns), `+24` widest glyph (from `CalcMaxWidth`), `+28` pixel-store selector, `+36` record count for the binary search, `+40` entry count, `+64/+68/+76/+80/+84` table offsets and the allocated block, `+72` block size. The selector steps (0: `(b+1)>>1`, 1: `b`, 2/3/6: `b<<2`, 4: `3*b`, 5: `b<<1`) are shared by `CalcDataSize`, `InitChars` and `InitSlots`.
- `InitSlots` builds a doubly linked list of 56-byte cells (`+56` first, `+60` last, `+8` previous, `+4` next, `+16/+20` x/y), one per glyph cell of the font texture; `DrawString` reads those cells' floats at `+24..+55` as the four texture corners of each glyph.
- `CalcSurfaceOffsets` lays out up to 10 mip levels in 24-byte records from `+64`, with an optional palette block first (pixel-store value 2 or 5 at `+52`, entry count at `+40`) and the total size at `+308`, which `Allocate`/`Deallocate` use together with `+312`.
- `CopyMaterial` copies a material's 144-byte layer records (texture id at record start `+48`, colour at `+56`, a matrix assigned through `__as__10VIMatrix33RC10VIMatrix33`, floats at `+100..+116`, a 64-byte tail from `+128`) and takes one reference on every surface the layers use. Its `Add__t6VIList1Z10VIMaterial` call takes no argument; the mangled `ii` types hide that the first int is a record address.
- `DrawCenteredString__6VIFont...PCUsiiUi` passes the `VIFont` object itself as the colour reference, like its 8-bit sibling already in the tree.
- `QueryEmitters` is a plane/sphere tree walk over 24-byte nodes at the zone's `+72` pointer; node numbers with bit 0x40000000 go to `QueryEmittersLeaf__6VIZoneiR23VISceneEmitterQueryVars`.
- `CreateResources` does the 256/`extra` divide without the usual zero check (R5900 divide-by-zero result), so `extra == 0` returns -1 instead of trapping. The source reproduces that with an explicit helper.

Per-function sizes (original bytes from the image, compiled bytes from the reviewed EE g++):

| function | address | original | compiled |
|---|---|---|---|
| `DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi` | `0x01011e00` | 196 | 268 |
| `GetLayerTexture__10VIMateriali` | `0x010cf8f0` | 24 | 24 |
| `Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef` | `0x0110b4b0` | 2648 | 936 |
| `CopyMaterial__8VIRasterii` | `0x0110da70` | 432 | 440 |
| `DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs` | `0x0110e750` | 748 | 708 |
| `ColorBuffer__8VIRasteri` | `0x011136e0` | 28 | 28 |
| `GetPointSize__8VIRasteri` | `0x01113e40` | 32 | 32 |
| `GetZBufferReg__C8VIRaster` | `0x01114930` | 12 | 20 |
| `QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars` | `0x01118cd8` | 404 | 368 |
| `Clear__10VIZoneRoom` | `0x0111efb0` | 212 | 216 |
| `CalcSurfaceOffsets__9VISurface` | `0x0112c2b0` | 592 | 424 |
| `Allocate__9VISurface` | `0x0112cac8` | 72 | 72 |
| `Deallocate__9VISurface` | `0x0112cb10` | 108 | 116 |
| `CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi` | `0x01134800` | 532 | 496 |
| `GetStringWidth__7VIRFontPCci` | `0x011354b0` | 396 | 296 |
| `GetStringWidth__7VIRFontPCUsi` | `0x01135640` | 396 | 296 |
| `InitSlots__7VIRFontR8VIRaster` | `0x011357d0` | 548 | 524 |
| `InitChars__7VIRFontPUc` | `0x011359f8` | 288 | 212 |
| `CalcDataSize__7VIRFontPUc` | `0x01135c80` | 320 | 500 |
| `CalcMaxWidth__7VIRFontiPUc` | `0x01136208` | 68 | 52 |
| `Allocate__7VIRFont` | `0x01136250` | 72 | 72 |
| `CreateRefMap__7VIScenei` | `0x01144f00` | 164 | 156 |

## Imports closed

- `Allocate__7VIRFont`
- `Allocate__9VISurface`
- `Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef`
- `CalcDataSize__7VIRFontPUc`
- `CalcMaxWidth__7VIRFontiPUc`
- `CalcSurfaceOffsets__9VISurface`
- `Clear__10VIZoneRoom`
- `ColorBuffer__8VIRasteri`
- `CopyMaterial__8VIRasterii`
- `CreateRefMap__7VIScenei`
- `CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi`
- `Deallocate__9VISurface`
- `DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi`
- `DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs`
- `GetLayerTexture__10VIMateriali`
- `GetPointSize__8VIRasteri`
- `GetStringWidth__7VIRFontPCUsi`
- `GetStringWidth__7VIRFontPCci`
- `GetZBufferReg__C8VIRaster`
- `InitChars__7VIRFontPUc`
- `InitSlots__7VIRFontR8VIRaster`
- `QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars`

## Imports opened

These are callees the new units reference that were not unresolved in the integrator's last private build. They are plain imports, not stubs:

- `Add__t6VIPool1ZP8VIRefMapRCP8VIRefMap`
- `BlitBegin__8VIRasterPP10_sceDmaTag`
- `BlitEnd__8VIRasterPP10_sceDmaTag`
- `Cache__7VIRFontR8VIRasterPCUsiPi`
- `CalcMipLevelStride__9VISurfacei`
- `DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUsUi`
- `QueryEmittersLeaf__6VIZoneiR23VISceneEmitterQueryVars`
- `ShareMaterial__8VIRasteri`
- `__t5VIMap2ZiZi`

Imports the units use that other authored sources or the platform already define (so they did not appear in the final unresolved list) include `Surface__8VIRasteri`, `ShareSurface__8VIRasteri`, `CreateSurface__8VIRasteri`, `Init__9VISurface...`, `LockMipLevel__9VISurface...`, `UnlockMipLevel__9VISurfacei`, `Init__10VIMateriali`, `SetLayerTexture/BlendMode/Color`, `SetDither__10VIMateriali`, `AddMaterial__8VIRasteriPP10_sceDmaTag`, `UploadRasterMicro/Matrices`, `CalcMatrices__8VIRaster`, `EndTriFan__8VIRaster`, `FlushDMABufferIfFull__8VIRaster`, `FindTyped__12VIDictionary...`, `Clear__t7VIArray1Zi`, `__builtin_new`, `__builtin_vec_new`, `__builtin_vec_delete`, `__as__10VIMatrix33RC10VIMatrix33`, `_6VIFont$sm_FontName` and `_8VIRaster$DBuffer`. Still unresolved from the older list and used here: `Add__t6VIList1Z10VIMaterial`, `CreateMaterial__8VIRasteri`, `Material__8VIRasteri`, `SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode`, `VIHashResourceID`, `Vertex__8VIRasterRC8VIVect2I`.

## Build-plan entries needed

Append these 22 authored inputs to the plan and to the snapshot, assigning fresh `input_order` values (the private trial used 7329 through 7350 against revision `5ff11ae1`, which had 7,329 inputs; that numbering is not stable). Every entry has `header_closure: []`, `profile: authored_cpp` and `kind: independently_authored_source`. Do not add any `host_check.cpp`: they include host headers. `default_authored_inputs` increases by 22. The `sha256` values are of the sources at the commit that adds this note. The private trial had no `private_object_sha256`, so none is claimed.

```json
{"source":"sources/w16-scene-a-font-drawcenteredstring-u16/drawcenteredstring.cpp","object_name":"w16_scene_a_font_drawcenteredstring_u16.o","sha256":"e1db9531e9f99ecaeb5bf9963bb50ddf1695b809162f74339ea2f694460c2f5e","emitted_functions":[{"name":"DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCUsiiUi","compiled_body_bytes":268}]}
{"source":"sources/w16-scene-a-material-getlayertexture/get_layer_texture.cpp","object_name":"w16_scene_a_material_getlayertexture.o","sha256":"deba26f50d7e0471e1b0ffb91e108ae2f7229f8d2c4efe744d70cac823f96afe","emitted_functions":[{"name":"GetLayerTexture__10VIMateriali","compiled_body_bytes":24}]}
{"source":"sources/w16-scene-a-raster-blit/blit.cpp","object_name":"w16_scene_a_raster_blit.o","sha256":"98fe6338af4ee30fe915a022046d1b7b73a68ae75fb7dc6a5f257adf05be50cf","emitted_functions":[{"name":"Blit__8VIRasteriUiRC8VIVect2IN2316VIAlphaBlendModef","compiled_body_bytes":936}]}
{"source":"sources/w16-scene-a-raster-colorbuffer/color_buffer.cpp","object_name":"w16_scene_a_raster_colorbuffer.o","sha256":"5f37ddc5ffa80400a622ba2c23dd3d95aac3ccdecda96ff7d094a526e00e3a4a","emitted_functions":[{"name":"ColorBuffer__8VIRasteri","compiled_body_bytes":28}]}
{"source":"sources/w16-scene-a-raster-copymaterial/copy_material.cpp","object_name":"w16_scene_a_raster_copymaterial.o","sha256":"1c2d8b767e7d37b340f69ad1643a741c89052ebb1ba319e802a51b5bc618f9ab","emitted_functions":[{"name":"CopyMaterial__8VIRasterii","compiled_body_bytes":440}]}
{"source":"sources/w16-scene-a-raster-drawstring/draw_string.cpp","object_name":"w16_scene_a_raster_drawstring.o","sha256":"2744094783fbef9e60ad01a5e9bced1cf88fda629dddd4a9962f794c6a6d6b0e","emitted_functions":[{"name":"DrawString__8VIRasteriRC8VIVect2IRC10VIColor32FPCUs","compiled_body_bytes":708}]}
{"source":"sources/w16-scene-a-raster-getpointsize/get_point_size.cpp","object_name":"w16_scene_a_raster_getpointsize.o","sha256":"f3b4ea50e0d97e7771ea1f453b2def2f84fb8d9a1c56a1622ddfb0334449a499","emitted_functions":[{"name":"GetPointSize__8VIRasteri","compiled_body_bytes":32}]}
{"source":"sources/w16-scene-a-raster-getzbufferreg/get_zbuffer_reg.cpp","object_name":"w16_scene_a_raster_getzbufferreg.o","sha256":"79df8bdb1fae319223f4e188a5419dd989a60edf149d692e603a5ed62602993f","emitted_functions":[{"name":"GetZBufferReg__C8VIRaster","compiled_body_bytes":20}]}
{"source":"sources/w16-scene-a-rfont-allocate/allocate.cpp","object_name":"w16_scene_a_rfont_allocate.o","sha256":"ce01fece4201381efe969ef54ba502b05c243619f7094e6480866c9b2ec7949d","emitted_functions":[{"name":"Allocate__7VIRFont","compiled_body_bytes":72}]}
{"source":"sources/w16-scene-a-rfont-calcdatasize/calc_data_size.cpp","object_name":"w16_scene_a_rfont_calcdatasize.o","sha256":"2a9c71fbf60e9d73c8911727ac41ca18222c597ba87b1eb1005403bde38bd0db","emitted_functions":[{"name":"CalcDataSize__7VIRFontPUc","compiled_body_bytes":500}]}
{"source":"sources/w16-scene-a-rfont-calcmaxwidth/calc_max_width.cpp","object_name":"w16_scene_a_rfont_calcmaxwidth.o","sha256":"6c28debb69d55351cdbcc664c50a15c241b97a467d545c466d5ad52c3a7fc869","emitted_functions":[{"name":"CalcMaxWidth__7VIRFontiPUc","compiled_body_bytes":52}]}
{"source":"sources/w16-scene-a-rfont-createresources/create_resources.cpp","object_name":"w16_scene_a_rfont_createresources.o","sha256":"93a0767c14b524a46c5068126ffb62e0d45c840c32999c970d5e2e5c7c127951","emitted_functions":[{"name":"CreateResources__7VIRFontR8VIRasterR12VIDictionaryii10VIBitDepthi","compiled_body_bytes":496}]}
{"source":"sources/w16-scene-a-rfont-getstringwidth-char/get_string_width.cpp","object_name":"w16_scene_a_rfont_getstringwidth_char.o","sha256":"00894826cb991156aede3dc72f652b558703d5260efd0488a16674bdd39cda62","emitted_functions":[{"name":"GetStringWidth__7VIRFontPCci","compiled_body_bytes":296}]}
{"source":"sources/w16-scene-a-rfont-getstringwidth-short/get_string_width.cpp","object_name":"w16_scene_a_rfont_getstringwidth_short.o","sha256":"8e9aa50c675c477f8bfe8ced51d2b40814d96d0ae01d2efd3fcd75cc0f8d49f1","emitted_functions":[{"name":"GetStringWidth__7VIRFontPCUsi","compiled_body_bytes":296}]}
{"source":"sources/w16-scene-a-rfont-initchars/init_chars.cpp","object_name":"w16_scene_a_rfont_initchars.o","sha256":"91980fe64596a41206c21df1d54d31bcd41a268136519a518d11638d3c93df60","emitted_functions":[{"name":"InitChars__7VIRFontPUc","compiled_body_bytes":212}]}
{"source":"sources/w16-scene-a-rfont-initslots/init_slots.cpp","object_name":"w16_scene_a_rfont_initslots.o","sha256":"66d23ec10364685949e2ae0639caae6a6ad81055c2380270da9d1e19b3e3acde","emitted_functions":[{"name":"InitSlots__7VIRFontR8VIRaster","compiled_body_bytes":524}]}
{"source":"sources/w16-scene-a-scene-createrefmap/create_ref_map.cpp","object_name":"w16_scene_a_scene_createrefmap.o","sha256":"d8fb14e9a56a03f7f2ad9344f1d04397d7935311e101c5258bcb80bff656a14a","emitted_functions":[{"name":"CreateRefMap__7VIScenei","compiled_body_bytes":156}]}
{"source":"sources/w16-scene-a-surface-allocate/allocate.cpp","object_name":"w16_scene_a_surface_allocate.o","sha256":"2f02096265b9e4fd424554585df44361a2869f48496c9c26b81d9c7d7cb71552","emitted_functions":[{"name":"Allocate__9VISurface","compiled_body_bytes":72}]}
{"source":"sources/w16-scene-a-surface-calcsurfaceoffsets/calc_surface_offsets.cpp","object_name":"w16_scene_a_surface_calcsurfaceoffsets.o","sha256":"0f95f57d6cfbedfbbf275d50d3f92c3cf72b64c535e74cfe676d78587f696907","emitted_functions":[{"name":"CalcSurfaceOffsets__9VISurface","compiled_body_bytes":424}]}
{"source":"sources/w16-scene-a-surface-deallocate/deallocate.cpp","object_name":"w16_scene_a_surface_deallocate.o","sha256":"ee07a8a45dabc9a6a9ace5416b72ea76fdd3fc80196d67b2a913d28d746fd9d0","emitted_functions":[{"name":"Deallocate__9VISurface","compiled_body_bytes":116}]}
{"source":"sources/w16-scene-a-zone-queryemitters/query_emitters.cpp","object_name":"w16_scene_a_zone_queryemitters.o","sha256":"c3922edc160c12d38ddf4e3ca9a55f2d948be9ba83e958f48401d1b7e3f5a0c7","emitted_functions":[{"name":"QueryEmitters__6VIZoneiR23VISceneEmitterQueryVars","compiled_body_bytes":368}]}
{"source":"sources/w16-scene-a-zoneroom-clear/clear.cpp","object_name":"w16_scene_a_zoneroom_clear.o","sha256":"c2f80eddc91cb84a84cc21c28c0305faa33318a51b0482b015e58acb459f99fd","emitted_functions":[{"name":"Clear__10VIZoneRoom","compiled_body_bytes":216}]}
```

## Authored but never executed

All 22 functions. They ran only inside the private interpreter harness (against the original's code, with Python stand-ins for callees) and on this Mac as host checks; none ran on the EE, under the original process, or in the game. The harness does not model hardware timing, GS/DMA behaviour, the VU, interrupts or physical memory access, so the DMA-packet writes in `Blit` and `DrawString` are checked as memory contents only. The private build link only checked symbol closure; no executable was linked. Function-local statics (the hash word in `DrawCenteredString`) are local `.bss` symbols here, not the original address.

## Unknown

- What each reset or copied field means in `VIZoneRoom::Clear`, `VISurface::Deallocate`, `CopyMaterial` (`+12`, `+24`, `+32`) and `CreateResources` (`+28`, `+32`, `+52`).
- The meaning of the 16-byte groups after position and texture coordinates in the `Blit` vertex records, and of the raster state words `+17884`, `+48`, `+52`, `+56` in `DrawString`.
- Whether the first int of `CopyMaterial` and the material argument of `AddMaterial__8VIRasteriPP10_sceDmaTag` are ids or addresses (the code uses them both ways); the units declare plain `int`.
- The exact types and names behind `VIPixelStore` values 2 and 5 at the surface `+52`, the blend and wrap mode values used (3 for wrap, 0 or 3 for blend in `CreateResources`), and what `+60`/`+84` of the mip records are used for.
- Whether `QueryEmitters` nodes flagged without 0x40000000 occur, and what the other flag bits mean.
- Every import's behaviour (all callees are unresolved or owned elsewhere).
- The default branch of the `Blit` mirror switch (`flags & 3` above 3) cannot occur and is not reproduced.
