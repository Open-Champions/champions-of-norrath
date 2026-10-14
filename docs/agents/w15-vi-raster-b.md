# w15-vi-raster-b: raster, font, surface cache, material and primbuffer (2 of 2)

Branch `agents/w15-vi-raster-b`. One commit per function or data object, each pushed. Source is under
`experimental/sources/w15-vi-raster-b-*`; every directory holds the authored source and a `host_check.cpp` (host headers, not a plan
input). Addresses and sizes come from the pinned private `SLUS_205.65` read locally with the reviewed EE binutils; no executable bytes,
disassembly listing or decompiler output is committed. Every source opens with an Observed / Inferred / Unknown comment. Compiled sizes
are those of the reviewed EE g++ with the `authored_cpp` flags and are not claims of byte identity.

## Result

Of the 34 assigned names, **32 are closed** and **2 are not authored** (the two VU1 microcode chains, see "Not done"). No full partial
build was run (the instructions limit it to one at a time and the units were compiled singly); closure was checked per unit by compiling
with the reviewed toolchain and listing the undefined symbols (`nm`), not by linking against the shared tip. `docs/agents/w15-vi-raster-b-plan-entries.py`
(a copy of the w14-raster script with this package's prefix) generates the entries below and a private plan copy.

## Units

| unit | symbol | image address | image bytes | compiled bytes |
|---|---|---|---|---|
| `w15-vi-raster-b-csprite-inittextslots` | `InitTextSlots__9VICSpriteR8VIRaster` | `0x010b2ee8` | 168 | 132 |
| `w15-vi-raster-b-csprite-share` | `Share__9VICSpriteR7VISceneP8VIRaster` | `0x010b1e60` | 620 | 640 |
| `w15-vi-raster-b-csprite-updateparticleemitters` | `UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44` | `0x010b0280` | 968 | 840 |
| `w15-vi-raster-b-hsprite-calclodlevelindex` | `CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster` | `0x01063280` | 316 | 248 |
| `w15-vi-raster-b-hsprite-setblendmatrices` | `SetBlendMatrices__9VIHSpriteR8VIRaster` | `0x01062ee0` | 368 | 352 |
| `w15-vi-raster-b-raster-blackouttexcopy` | `blackoutTexCopy` | `0x008dd340` | 64 | 64 |
| `w15-vi-raster-b-raster-calccenteredstringscreenrect` | `CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect` | `0x0110f790` | 500 | 304 |
| `w15-vi-raster-b-raster-colormatrix` | `_8VIRaster$ColorMatrix` | `0x011b0190` | 4 | 4 |
| `w15-vi-raster-b-raster-copymaterialpal` | `CopyMaterialPal__8VIRasterii` | `0x0110e1d0` | 360 | 368 |
| `w15-vi-raster-b-raster-creatematerialpal` | `CreateMaterialPal__8VIRasterii` | `0x0110e078` | 340 | 392 |
| `w15-vi-raster-b-raster-drawcenteredstring` | `DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi` | `0x0110ea40` | 948 | 884 |
| `w15-vi-raster-b-raster-drawprimbuffer-3` | `DrawPrimBuffer__8VIRasteriiUi` | `0x0110d6d0` | 412 | 284 |
| `w15-vi-raster-b-raster-drawprimbuffer-4` | `DrawPrimBuffer__8VIRasteriiiUi` | `0x01113560` | 240 | 192 |
| `w15-vi-raster-b-raster-foregrndtexcopy` | `foregrndTexCopy` | `0x008dd380` | 64 | 64 |
| `w15-vi-raster-b-raster-getmodelview` | `GetModelView__C8VIRaster` | `0x01112f58` | 8 | 8 |
| `w15-vi-raster-b-raster-getstringwidth-char` | `GetStringWidth__8VIRasteriPCc` | `0x01113dd0` | 52 | 36 |
| `w15-vi-raster-b-raster-getstringwidth-ushort` | `GetStringWidth__8VIRasteriPCUs` | `0x01113e08` | 52 | 36 |
| `w15-vi-raster-b-raster-releasefont` | `ReleaseFont__8VIRasteriR12VIDictionary` | `0x01113cb0` | 164 | 160 |
| `w15-vi-raster-b-raster-releasematerialpal` | `ReleaseMaterialPal__8VIRasteriR12VIDictionary` | `0x0110e338` | 292 | 324 |
| `w15-vi-raster-b-raster-releaseprimbuffer` | `ReleasePrimBuffer__8VIRasteriR12VIDictionary` | `0x011133c0` | 196 | 196 |
| `w15-vi-raster-b-raster-rmaterialoffset` | `_8VIRaster$RMaterialOffset` | `0x011b01b0` | 4 | 4 |
| `w15-vi-raster-b-raster-shareprimbuffer` | `SharePrimBuffer__8VIRasteri` | `0x01113390` | 44 | 44 |
| `w15-vi-raster-b-sprite-pick` | `Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | `0x01008140` | 8 | 8 |
| `w15-vi-raster-b-sprite-raster` | `Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi` | `0x01008128` | 8 | 8 |
| `w15-vi-raster-b-surface-init` | `Init__9VISurfaceR8VIRasterii10VIBitDepthi` | `0x0112c820` | 304 | 256 |
| `w15-vi-raster-b-surface-lockpalette` | `LockPalette__9VISurfaceRiR12VIPixelStorePPUc` | `0x0112c9d8` | 168 | 184 |
| `w15-vi-raster-b-surface-unlockpalette` | `UnlockPalette__9VISurface` | `0x0112ca80` | 68 | 72 |
| `w15-vi-raster-b-tex-drawcolorspecialhalo` | `textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii` | `0x0016e050` | 1016 | 928 |
| `w15-vi-raster-b-tex-normalizedheight` | `texNormalizedHeight__FP8_texture` | `0x00172ab0` | 8 | 8 |
| `w15-vi-raster-b-tex-normalizedwidth` | `texNormalizedWidth__FP8_texture` | `0x00172aa8` | 8 | 8 |
| `w15-vi-raster-b-vifont-fontname` | `_6VIFont$sm_FontName` | `0x011c3090` | 8 | 8 |
| `w15-vi-raster-b-vimap-insert` | `Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo` | `0x012205d0` | 1160 | 856 |

`Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo` is a weak (template) symbol in the image; the authored object defines it
as a normal global, as the other template instantiations authored earlier in the tree do. The five data objects
(`_8VIRaster$ColorMatrix`, `_8VIRaster$RMaterialOffset`, `_6VIFont$sm_FontName`, `blackoutTexCopy`, `foregrndTexCopy`) were compared with the
image words (initialised words) or with the symbol extent (the two `.bss` objects, 64 bytes each).

## How each unit was checked

* Compiled with the reviewed EE g++ and the plan's `authored_cpp` flags, one unit at a time (`-nostdinc`, freestanding).
* Native host check (`c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp`, exit 0) with stubbed callees; pointer words
  use the arena convention (`NORRATH_HOST_ARENA`) where objects store 32-bit target addresses.
* Paired run in a private R5900 interpreter (outside git, with `SLUS_205.65` loaded): the original function and the freshly compiled
  authored function run from identical memory snapshots with identical Python callee stubs that log their arguments (and the memory they
  see); call logs, return values and every changed byte outside the stack must match, and a halted run counts as a failure. Scenario counts
  and original-word coverage are in the commit messages; coverage is full except for nops and unreachable branches (for
  `textureDrawColorSpecialHalo` the tag-id-not-1 branch, which cannot be reached because the id is always 1; it was kept as in the image).
* Mutation: each unit had deliberate edits (constants, offsets, comparison senses, call or operand order) and the paired run failed each
  time except for the equivalent mutants named in the commit messages. Mutations were applied to code, not to the header comment.
* `VICSprite::UpdateParticleEmitters`, `textureDrawColorSpecialHalo` and `VIMap::Insert` were written in this last session; their stubs
  are described here because the commit messages are short. `UpdateParticleEmitters` stubs `GetTime`, `Mul`, `CalcTransAboutCenter`, the five
  emitter callbacks and both matrix destructors (logging operand contents); `textureDrawColorSpecialHalo` stubs `drawTexture` (advancing
  the cursor by a chosen number of bytes), `drawAddToList` and `engineGetField`; `Insert` ran the **real** `AddNode`, `LeftRotate` and
  `RightRotate` from the image on valid red-black trees built by an independent Python implementation (0..20 keys, both 8-byte alignments
  of the entry array, ascending and descending runs), plus three scenarios with a stubbed failing `AddNode`.

## Observed, inferred, unknown (short form; the source comments are the record)

* Observed: `VIMap<int, VIEESurfCacheInfo>` is a red-black tree in an index-addressed array: count +0, capacity +4, entry array +8, root
  index +12, free list +16; entries are 104 bytes (colour +0, parent +4, left +8, right +12, key +16, 84-byte value +20) and entry 0 is a
  nil entry. `Insert` returns an existing key's index without replacing its value.
* Observed: `textureDrawColorSpecialHalo` is a sibling of the earlier `textureDrawSuperColor`: same packet frame and clip handling, a fixed
  vertex colour 0x80808080, the caller's colour masked to 24 bits in a second register, an extra int that is shifted into a GS register word,
  and an extra quadword before the scissor reset.
* Observed: `UpdateParticleEmitters` runs once per scene time value (the 64-bit stamp at sprite +4608), places each emitter on its
  skeleton node, purges emitters farther than 100 units from the camera (squared distance above 10000.0f) and updates the rest unless an
  emitter with mode 1 has run past start plus duration.
* Inferred: all names of fields; colour 1 = red / 0 = black in the tree; that the emitter mode word, start and duration are an expiry.
* Unknown: every callee body listed below, the full sprite, camera, scene and raster layouts, what the GS register values select, and the
  other members of the `VIMap` instantiation (Erase, EraseFixup, Allocate, Clear, FreeNode, LinkFreeChain are not in this package).

## Not done

* `VIDMABillboardMicrocode` (image `0x011ee2d0`) and `VIDMARasterMicrocode` (image `0x011ee950`): each is a DMA chain that embeds a VU1
  program. Reproducing them means copying program bytes out of the image, which is extracted proprietary code and not independent authoring;
  no source was written (the same decision as w14-raster). They stay unresolved until a VU1 microcode owner provides an independently written
  implementation.

## Imports closed

`SharePrimBuffer__8VIRasteri`, `GetStringWidth__8VIRasteriPCUs`, `GetStringWidth__8VIRasteriPCc`, `ReleaseFont__8VIRasteriR12VIDictionary`, `ReleaseMaterialPal__8VIRasteriR12VIDictionary`, `ReleasePrimBuffer__8VIRasteriR12VIDictionary`, `CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect`, `DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi`, `CopyMaterialPal__8VIRasterii`, `CreateMaterialPal__8VIRasterii`, `DrawPrimBuffer__8VIRasteriiUi`, `DrawPrimBuffer__8VIRasteriiiUi`, `Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster`, `Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi`, `Share__9VICSpriteR7VISceneP8VIRaster`, `UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44`, `InitTextSlots__9VICSpriteR8VIRaster`, `CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster`, `SetBlendMatrices__9VIHSpriteR8VIRaster`, `UnlockPalette__9VISurface`, `Init__9VISurfaceR8VIRasterii10VIBitDepthi`, `LockPalette__9VISurfaceRiR12VIPixelStorePPUc`, `GetModelView__C8VIRaster`, `texNormalizedHeight__FP8_texture`, `texNormalizedWidth__FP8_texture`, `textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii`, `_6VIFont$sm_FontName`, `_8VIRaster$ColorMatrix`, `_8VIRaster$RMaterialOffset`, `blackoutTexCopy`, `foregrndTexCopy`, `Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo`.

## Imports blocked (unresolved, not authored)

`VIDMABillboardMicrocode`, `VIDMARasterMicrocode`.

## Imports opened

The units reference these names (taken from the undefined symbols of each compiled object). Those that another package or the shared tree
already defines are not new; the ones that no authored source in this worktree defines, and which therefore stay unresolved for the
integrator, are the first group.

* Not defined by any authored source seen in this worktree: `DrawPrimBufferPackVUNBW__8VIRasteriiUi`, `DrawPrimBufferPackVUNCG__8VIRasteriiUi`,
  `DrawPrimBufferPackVUNC__8VIRasteriiUi`, `DrawPrimBufferVUNC__8VIRasteriiUi`.
* Data globals of the render list (owned elsewhere): `glistp`, `gstart`, `GLISTSIZE`, `engineScreenSlide`, `engineScissory1`,
  `engineScissory2`, `supersampleMode`.
* Defined by other authored sources (checked by name in this worktree, not linked): `AddMaterial__8VIRasteriPP10_sceDmaTag`,
  `AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii`, `Add__t6VIList1Z10VIMaterial`, `Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer`,
  `Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal`, `Allocate__9VISurface`, `Begin__Ct6VIPool1Z19VIHSpriteAttachment`, `Cache__7VIRFontR8VIRasterPCciPi`,
  `CalcMatrices__8VIRaster`, `CalcSurfaceOffsets__9VISurface`, `CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44`,
  `CopyMaterial__8VIRasterii`, `Deallocate__9VISurface`, `DrawPrimBufferPackVUNC__8VIRasteriiiUi`, `DrawPrimBufferPackVU__8VIRasteriifUi`,
  `EndTriFan__8VIRaster`, `End__Ct6VIPool1Z19VIHSpriteAttachment`, `Erase__t6VIPool1ZP12VIPrimBufferi`, `Erase__t6VIPool1ZP13VIMaterialPali`,
  `Erase__t6VIPool1ZP7VIRFonti`, `GetLayerTexture__10VIMateriali`, `GetMaterialPalEntry__8VIRasterii`, `GetMaterialPalSize__8VIRasteri`,
  `GetModelView__C8VIRaster`, `GetStringWidth__7VIRFontPCUsi`, `GetStringWidth__7VIRFontPCci`, `GetTime__7VIScene`, `Init__t7VIArray1Zii`,
  `LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Next__Ct6VIPool1Z19VIHSpriteAttachmenti`,
  `Purge__17VIParticleEmitter`, `ReleaseMaterial__8VIRasteriR12VIDictionary`, `Remove__12VIDictionaryR10VIResource`, `ReorderPalette__9VISurface`,
  `RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi`, `SetDMAHeaders__9VISurface`, `SetDirection__17VIParticleEmitterRC10VIMatrix33`,
  `SetDirection__17VIParticleEmitterRC7VIVect3`, `SetLocation__17VIParticleEmitterRC7VIVect3`, `SetObjectBlends__8VIRasteriP10VIMatrix44`,
  `SetSpace__17VIParticleEmitterRC10VIMatrix44`, `ShareAnimation__7VIScenei`, `ShareMaterialPal__8VIRasteri`,
  `ShareParticleDefinition__16VIParticleSystemi`, `ShareRefMap__7VIScenei`, `ShareSound__13VISoundDevicei`, `ShareSprite__7VIScenei`,
  `ShareSurface__8VIRasteri`, `Update__17VIParticleEmitter`, `UploadMatrices__8VIRasterPP10_sceDmaTag`, `UploadRasterMicro__8VIRasterPP10_sceDmaTag`,
  `Vertex__8VIRasterRC8VIVect2I`, `_$_10VIMatrix33`, `_$_10VIMatrix44`, `_$_7VIRFont`, `_$_t7VIArray1Zi`, `__10VIMatrix33`, `__builtin_delete`,
  `__builtin_new`, `__ml__FRC7VIVect3RC10VIMatrix44`, `__ml__FRC7VIVect4RC10VIMatrix44`, `__t7VIArray1Zi`, `drawAddToList__FPviP8_textureP11_drawRecordi`,
  `drawTexture__FR6PacketP8_textureiiiiii`, `engineGetField__Fv`.

The four `DrawPrimBuffer*VUNC/VUNBW/VUNCG` functions are the VU-packing draw callees of the two `DrawPrimBuffer` overloads; this package
authored only the dispatchers.

## Build-plan entries needed

Append these 32 entries (assign fresh `input_order` values and recompute file digests, the `default_authored_inputs` count and the snapshot
digest). `docs/agents/w15-vi-raster-b-plan-entries.py --toolchain /path/to/ee [--private-root DIR]` regenerates them and writes a private
plan and snapshot copy. All have an empty `header_closure`.

```json
{"emitted_functions": [{"compiled_body_bytes": 132, "name": "InitTextSlots__9VICSpriteR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_csprite_inittextslots.o", "profile": "authored_cpp", "sha256": "ba3e020cb5fc724ce2c54dfc6a915c5ab60ce2e1d3dfd140d015664881f6cebc", "source": "sources/w15-vi-raster-b-csprite-inittextslots/init_text_slots.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 640, "name": "Share__9VICSpriteR7VISceneP8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_csprite_share.o", "profile": "authored_cpp", "sha256": "cecfeb3fc6bbb29b8ae6144e76be8d1051aec5e79488993a41a3d5358d2c43cb", "source": "sources/w15-vi-raster-b-csprite-share/share.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 840, "name": "UpdateParticleEmitters__9VICSpriteR8VICameraR7VISceneR8VIRasterR10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_csprite_updateparticleemitters.o", "profile": "authored_cpp", "sha256": "f71672f09fe9c0325509a9a8b016132d63bb666a926d42587ddfb37d3ce2d062", "source": "sources/w15-vi-raster-b-csprite-updateparticleemitters/update_particle_emitters.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_hsprite_calclodlevelindex.o", "profile": "authored_cpp", "sha256": "aa4a756bc325831da5c18a6c86a82318973ddeea1708787ade9e2326a64549ca", "source": "sources/w15-vi-raster-b-hsprite-calclodlevelindex/calc_lod_level_index.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 352, "name": "SetBlendMatrices__9VIHSpriteR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_hsprite_setblendmatrices.o", "profile": "authored_cpp", "sha256": "6235e821fa5dd5e9b331453a87bcd02fe2531d036a1a7413279e5fdbdc7f39a5", "source": "sources/w15-vi-raster-b-hsprite-setblendmatrices/set_blend_matrices.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_blackouttexcopy.o", "profile": "authored_cpp", "sha256": "f3db246b93a38fd0cc29e16be929fb11fbe2261ba06bd2e1d17eed8e8aef9161", "source": "sources/w15-vi-raster-b-raster-blackouttexcopy/blackout_tex_copy.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 304, "name": "CalcCenteredStringScreenRect__8VIRasteriRC7VIVect3PCcR6VIRect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_calccenteredstringscreenrect.o", "profile": "authored_cpp", "sha256": "56cfc21d46f18f1957fa540c33a5be4ae9ae375bbd2c8d9862cb1b1dbf030d93", "source": "sources/w15-vi-raster-b-raster-calccenteredstringscreenrect/calc_centered_string_screen_rect.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_colormatrix.o", "profile": "authored_cpp", "sha256": "48a308c2f98a05159095fa734b6e16a8853c7a429f2064e50de1a6d7144277c2", "source": "sources/w15-vi-raster-b-raster-colormatrix/color_matrix.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 368, "name": "CopyMaterialPal__8VIRasterii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_copymaterialpal.o", "profile": "authored_cpp", "sha256": "3fcf9058a9d789c8140d2e755b81781d4e4301c2b15760871113aaccdda3a4d6", "source": "sources/w15-vi-raster-b-raster-copymaterialpal/copy_material_pal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 392, "name": "CreateMaterialPal__8VIRasterii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_creatematerialpal.o", "profile": "authored_cpp", "sha256": "d560917c39d41731b90879e857b5627e858bab6e2e718cecab9df76334b124e3", "source": "sources/w15-vi-raster-b-raster-creatematerialpal/create_material_pal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 884, "name": "DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_drawcenteredstring.o", "profile": "authored_cpp", "sha256": "a84fd5463fe8846dc21120e0a8c3ae923b83267223b215c9a5de1264487606a8", "source": "sources/w15-vi-raster-b-raster-drawcenteredstring/draw_centered_string.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 284, "name": "DrawPrimBuffer__8VIRasteriiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_drawprimbuffer_3.o", "profile": "authored_cpp", "sha256": "6b4f22def03a3dbc1419a8c846655f92258cceda6e5965676233ad8815dfb978", "source": "sources/w15-vi-raster-b-raster-drawprimbuffer-3/draw_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "DrawPrimBuffer__8VIRasteriiiUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_drawprimbuffer_4.o", "profile": "authored_cpp", "sha256": "fa9cb6848819454b001e079e4ddafe613a259b2ab5c5d373ff5f63680a574c7b", "source": "sources/w15-vi-raster-b-raster-drawprimbuffer-4/draw_prim_buffer.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_foregrndtexcopy.o", "profile": "authored_cpp", "sha256": "940f1141d83c47afbf3ef8ee8083abf098ff788c634ea9ef2757cbb12cad8d73", "source": "sources/w15-vi-raster-b-raster-foregrndtexcopy/foregrnd_tex_copy.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "GetModelView__C8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_getmodelview.o", "profile": "authored_cpp", "sha256": "8acd6e60adda019f8c99808c73b6fd3eebfc74f97d026d21b81f45438937ecb2", "source": "sources/w15-vi-raster-b-raster-getmodelview/get_model_view.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "GetStringWidth__8VIRasteriPCc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_getstringwidth_char.o", "profile": "authored_cpp", "sha256": "65b56eb09af3039210dbcfc3ac859d2ee0cfcd8ebee1485a1667a5331af5678d", "source": "sources/w15-vi-raster-b-raster-getstringwidth-char/get_string_width.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "GetStringWidth__8VIRasteriPCUs"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_getstringwidth_ushort.o", "profile": "authored_cpp", "sha256": "25db0615ba8de792dac9ca6eed0ea2193a7e25046031114ef958dff95374f6a8", "source": "sources/w15-vi-raster-b-raster-getstringwidth-ushort/get_string_width.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "ReleaseFont__8VIRasteriR12VIDictionary"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_releasefont.o", "profile": "authored_cpp", "sha256": "d5242869c1f3d0e67157c399095c5a5e161d4f9545e84b13fd6a95a420ac7544", "source": "sources/w15-vi-raster-b-raster-releasefont/release_font.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 324, "name": "ReleaseMaterialPal__8VIRasteriR12VIDictionary"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_releasematerialpal.o", "profile": "authored_cpp", "sha256": "06ac5fc7a0b59c5d58faabb2d5e549e2428a29e36e9bd1abee28205793473e24", "source": "sources/w15-vi-raster-b-raster-releasematerialpal/release_material_pal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 196, "name": "ReleasePrimBuffer__8VIRasteriR12VIDictionary"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_releaseprimbuffer.o", "profile": "authored_cpp", "sha256": "f3f1ee46ea6cce37d4a00b35d0ac6422d362e0430f413b6cd4371c936db512d4", "source": "sources/w15-vi-raster-b-raster-releaseprimbuffer/release_prim_buffer.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_rmaterialoffset.o", "profile": "authored_cpp", "sha256": "a183b8a517457f4a906b1aec916197a337c3ff607f61d4734bbf4ded0223206e", "source": "sources/w15-vi-raster-b-raster-rmaterialoffset/r_material_offset.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "SharePrimBuffer__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_raster_shareprimbuffer.o", "profile": "authored_cpp", "sha256": "6e012d444ca1a6bd6d9ae7a3779ddb9549c7e6741abbefe3d8d60787d1b582d2", "source": "sources/w15-vi-raster-b-raster-shareprimbuffer/share_prim_buffer.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Pick__8VISpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_sprite_pick.o", "profile": "authored_cpp", "sha256": "b6e3a9d0d1c0a77d332a6edff74909722d2385f1e5146072b1fead55510b42a7", "source": "sources/w15-vi-raster-b-sprite-pick/pick.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Raster__8VISpriteR8VICameraR7VISceneR8VIRasterUi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_sprite_raster.o", "profile": "authored_cpp", "sha256": "85d29790d4a984c90e10d9e966373c437cdd16c96eaaea1d3cf0d979a70edb78", "source": "sources/w15-vi-raster-b-sprite-raster/raster.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 256, "name": "Init__9VISurfaceR8VIRasterii10VIBitDepthi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_surface_init.o", "profile": "authored_cpp", "sha256": "1934cb0e8e6b9f3fdaeab88c25e606649c442f43ea9ea50eb865cdd78386092f", "source": "sources/w15-vi-raster-b-surface-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "LockPalette__9VISurfaceRiR12VIPixelStorePPUc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_surface_lockpalette.o", "profile": "authored_cpp", "sha256": "a9b2fb935e64df6aa05e4f1b5c9004878479899a22f7c403135a1c8a57b78c7a", "source": "sources/w15-vi-raster-b-surface-lockpalette/lock_palette.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "UnlockPalette__9VISurface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_surface_unlockpalette.o", "profile": "authored_cpp", "sha256": "3918e82254e70cf6dd194b3adbaac572215b36803f0130d47a8eaccf1d0a8626", "source": "sources/w15-vi-raster-b-surface-unlockpalette/unlock_palette.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 928, "name": "textureDrawColorSpecialHalo__FP8_textureiiiUiiiiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_tex_drawcolorspecialhalo.o", "profile": "authored_cpp", "sha256": "0dcbda6fdd782b0b05f8d99ce177bcf740ee030f5f05d2014010b7e1a8a8a6dc", "source": "sources/w15-vi-raster-b-tex-drawcolorspecialhalo/texture_draw_color_special_halo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "texNormalizedHeight__FP8_texture"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_tex_normalizedheight.o", "profile": "authored_cpp", "sha256": "5b8e270dd7fa399d6314e1a7a73b9b25cec7ced6cfd556a47681e84b74306a71", "source": "sources/w15-vi-raster-b-tex-normalizedheight/tex_normalized_height.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "texNormalizedWidth__FP8_texture"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_tex_normalizedwidth.o", "profile": "authored_cpp", "sha256": "722b2ed2812a554ce0e52b3110bcfd7f56943813a54a05b086e39b9f49aec3dc", "source": "sources/w15-vi-raster-b-tex-normalizedwidth/tex_normalized_width.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_vifont_fontname.o", "profile": "authored_cpp", "sha256": "7723cc11433fb3648a7e61563e9687c66bd58aa826e8aff694a6bc3afb3a5cad", "source": "sources/w15-vi-raster-b-vifont-fontname/font_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 856, "name": "Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15rasterb_vimap_insert.o", "profile": "authored_cpp", "sha256": "8d0a7821fb1a3706bdb77e99637c1b962561beb9ea4ae06db8d399f55b8892ae", "source": "sources/w15-vi-raster-b-vimap-insert/insert.cpp"}
```

## Authored but never executed

All 32 units are authored from the image and checked only against stubs: host checks plus paired runs in the private interpreter. None has
run on the EE or in any emulator; the callees were stubs (Python logging functions) in every paired run except `Insert`, which executed the
real `AddNode`, `LeftRotate` and `RightRotate` from the image. The authored objects have not been linked into the shared partial build (no
full `build_partial.py` run was made in this package). The paired interpreter models no VU0 or VU1 and has only a partial MMI model, so
float-heavy callees were stubbed.

## Unknown

* The meaning of most flag words and offsets named only by offset (sprite, camera, scene and raster fields).
* The two VU1 microcodes and their DMA chains.
* The `VIMap` members other than `Insert` and which colour value the original uses for red (inferred 1 from the value `AddNode` stores).
* What the GS register values in `textureDrawColorSpecialHalo` (0x48/0x42, 0x3d, 0x176, 0x1212303012123030, 0x3131757531317575) select.
* Whether `UpdateParticleEmitters`' 100-unit distance limit is a design value or leftover; callers are not authored here.

Note: the commit message of `textureDrawColorSpecialHalo` says compiled 888 bytes; that was measured before a fix made during the same session (an extra quadword); the final source compiles to 928 bytes (table above).
