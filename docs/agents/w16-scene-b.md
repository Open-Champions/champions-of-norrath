# w16-scene-b: scene, zone, material, font, surface and raster (part 2)

Branch `agents/w16-scene-b`, one verified complete function per commit. Target edition: the pinned SLUS-20565 image kept privately (addresses below are from that image; nothing derived from it is committed).

## What was done and how it was checked

Each function was authored from reading the image's code, compiled with the reviewed `authored_cpp` flags, and run side by side with the original in a private R5900 paired-run harness (outside git). Each paired run starts the original and the authored function from identical memory snapshots with identical callee stand-ins, and compares every changed byte outside the stack, the call log of the stand-ins, and the return value. Each function also has a `host_check.cpp` (host build, callee mocks, expected values written independently of the source). Mutation probes (changing one constant or branch) were confirmed to fail the paired run for: `SetLayerUVSet`, `SetLayerUVRate`, `SetDMAHeaders`, `ResetCache` (two mutants), `UpdateActor` (two mutants) and `StretchBlit` (three mutants).

| unit directory (`experimental/sources/w16-scene-b-...`) | symbol | address | paired scenarios | original words covered |
|---|---|---|---|---|
| material-set-emissive-color | `SetEmissiveColor__10VIMaterialRC9VIColor32` | 0x010cf7d0 | 60 | 8/8 |
| material-set-tessellate | `SetTessellate__10VIMateriali` | 0x010cf7a8 | 60 | 9/9 |
| material-set-layer-uv-transform | `SetLayerUVTransform__10VIMaterialiRC10VIMatrix33` | 0x010cfa50 | 60 | 18/18 |
| material-set-layer-uv-set | `SetLayerUVSet__10VIMaterialii` | 0x010cfb78 | 60 | 17/17 |
| material-set-layer-uv-rate | `SetLayerUVRate__10VIMaterialiRC7VIVect2` | 0x010cfa98 | 60 | 25/25 |
| material-set-layer-wrap-mode | `SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode` | 0x010ce920 | 140 | 59/62 (3 uncovered words are padding nops) |
| surface-ctor | `__9VISurface` | 0x0112c750 | 60 | 29/29 |
| surface-set-dma-headers | `SetDMAHeaders__9VISurface` | 0x0112c500 | 80 | 84/84 |
| surface-reorder-palette | `ReorderPalette__9VISurface` | 0x0112c650 | 120 | 64/64 |
| font-ctor | `__7VIRFont` | 0x01135dc0 | 60 | 9/9 |
| font-dtor | `_$_7VIRFont` | 0x01135de8 | 60 | 20/21 (one padding word) |
| font-reset-cache | `ResetCache__7VIRFontR8VIRaster` | 0x01135b18 | 130 | 88/89 (one padding word) |
| scene-ref-map | `RefMap__7VIScenei` | 0x01144fa8 | 40 | 7/7 |
| staticlighting-release-resources | `ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary` | 0x010122f8 | 60 | 35/35 |
| scene-remove-all-actors | `RemoveAllActors__7VIScene` | 0x01145d80 | 40 | 29/30 (one padding word) |
| scene-update-all-actors | `UpdateAllActors__7VIScene` | 0x01145df8 | 60 | 32/33 (one padding word) |
| scene-replace-proxy-rooms | `ReplaceProxyRooms__7VISceneR6VIZone` | 0x01144138 | 120 | 56/56 |
| scene-replace-static-proxy-actors | `ReplaceStaticProxyActors__7VISceneR6VIZone` | 0x01144218 | 160 | 76/76 |
| zone-render-leaf | `RenderLeaf__6VIZoneiR15VISceneRendVars` | 0x01119330 | 120 | 32/32 |
| zone-query-intersection | `QueryIntersection__6VIZoneR20VISceneIntersectVars` | 0x01118e70 | 100 | 33/34 (one padding word) |
| scene-update-actor | `UpdateActor__7VIScenei` | 0x01143ad8 | 80 | 109/109 |
| raster-stretch-blit | `StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef` | 0x0110bf08 | 200 | 664/670 (six uncovered words: delay-slot nops, an unreachable mode fall-through) |

The 'covered' column counts instruction words of the original that the paired scenarios executed; the missed words are alignment padding or delay-slot nops, except where noted.

## Observed / inferred / unknown

Observed facts are in the header comment of each source file (field offsets, loop shapes, constants, call order). Cross-function summary:

- `VIMaterial` setters share a changed flag: a word at offset 32 set to 1 by every layer setter and by `SetEmissiveColor`; `SetTessellate` does not set it. Layer records are 144 bytes starting at material + 48 (flag word at +4 inside the record, colour/blend at other offsets seen in earlier waves). *Inferred*: the flag means the material needs re-upload.
- `SetLayerUVSet` stores `0xffffffef` when asked to set the bit, not a single-bit set. *Observed*; reproduced, not corrected, and flagged as a probable mask slip in the original.
- `ReorderPalette` swaps the middle two eight-entry groups of every 32-entry palette block for format values 2 (32-bit entries) and 5 (16-bit entries); other formats return -1. *Inferred*: a palette swizzle.
- `SetDMAHeaders` writes four-quadword blocks whose constants match a DMA-tag + GIF-tag pattern. *Inferred only* from the constants; no hardware run.
- `__7VIRFont` returns whatever `Clear__7VIRFont` returns because the call is a tail call (the original loads the object pointer into v0 first, which the tail call overwrites).
- `StretchBlit` writes four 64-byte vertex records per call in order TL, TR, BR, BL; flags bit 2 selects the dither argument and flag bits 0/1 mirror the source rectangle. *Inferred*: bits 0/1 are horizontal/vertical mirror; the names of the raster fields are unknown.
- `UpdateActor` is the actor re-insertion path: recompute world matrix, rescale radius, remove, transform the box corners, insert, bump a counter at scene +1348.

## Imports closed (names)

- `SetEmissiveColor__10VIMaterialRC9VIColor32`
- `SetTessellate__10VIMateriali`
- `SetLayerUVTransform__10VIMaterialiRC10VIMatrix33`
- `SetLayerUVSet__10VIMaterialii`
- `SetLayerUVRate__10VIMaterialiRC7VIVect2`
- `SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode`
- `__9VISurface`
- `SetDMAHeaders__9VISurface`
- `ReorderPalette__9VISurface`
- `__7VIRFont`
- `_$_7VIRFont`
- `ResetCache__7VIRFontR8VIRaster`
- `RefMap__7VIScenei`
- `ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary`
- `RemoveAllActors__7VIScene`
- `UpdateAllActors__7VIScene`
- `ReplaceProxyRooms__7VISceneR6VIZone`
- `ReplaceStaticProxyActors__7VISceneR6VIZone`
- `RenderLeaf__6VIZoneiR15VISceneRendVars`
- `QueryIntersection__6VIZoneR20VISceneIntersectVars`
- `UpdateActor__7VIScenei`
- `StretchBlit__8VIRasteriUiRC8VIVect2IN3316VIAlphaBlendModef`

All 22 names in the package scope are closed. Each of the 22 objects was compiled on its own and `nm` shows exactly the intended global symbol; a full partial build was not run (see below).

## Imports opened

Unresolved references introduced by these units that no other authored unit in this tree defines (they may belong to other packages): `Begin__Ct6VIList1Z7VIActor`, `End__Ct6VIList1Z7VIActor`, `Next__Ct6VIList1Z7VIActori`, `BlitBegin__8VIRasterPP10_sceDmaTag`, `BlitEnd__8VIRasterPP10_sceDmaTag`, `CalcObjectToWorld__7VIActor`, `Clear__7VIRFont`, `InsertActor__7VIWorldR17VISceneInsertVars`, `QueryIntersection__6VIZoneiR20VISceneIntersectVars`, `ReleaseColorBuffer__8VIRasteriR12VIDictionary`, `ReplaceProxyActor__7VISceneii`, `SetPreTranslations__9VICollideiP7VIVect3`, `Transform__C6VIBBoxRC10VIMatrix44P7VIVect3`, `memcpy`, `__builtin_delete`. The remaining references resolve to authored units already in the tree (`AddMaterial__8VIRasteriPP10_sceDmaTag`, `Find__12VIDictionaryUiR14VIResourceTypeRi`, `FlushDMABufferIfFull__8VIRaster`, `LockPalette__9VISurfaceRiR12VIPixelStorePPUc`, `UnlockPalette__9VISurface`, `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`, `RemoveActor__7VIScenei`, `RenderRoom__6VIZoneiR15VISceneRendVars`, `SetDither__10VIMateriali`, `Surface__8VIRasteri`, `UploadRasterMicro__8VIRasterPP10_sceDmaTag`, `__as__10VIMatrix33RC10VIMatrix33`). This list comes from reading each object's undefined symbols and searching the tree's sources for definitions, not from a link.

## Build-plan entries needed

`experimental/build-plan.json` and `experimental/snapshot.json` were not edited (shared files). The integrator needs 22 `independently_authored_source` inputs with fresh `input_order` values and matching snapshot records. The hashes below are the SHA-256 of each source at the last commit of this branch; `input_order` and `emitted_functions` are left to the integrator. A full partial build was not run because a valid private plan also needs a private snapshot copy; only per-unit compiles were done.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_font_ctor.o",
  "profile": "authored_cpp",
  "sha256": "f288e12ac391111675bbedefae05c14d2f3ebe0e3f5b4d31532e894ed4db8029",
  "source": "sources/w16-scene-b-font-ctor/font_ctor.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_font_dtor.o",
  "profile": "authored_cpp",
  "sha256": "60eb9ebd16895488dba66aad627dc6d2ea925fb85dbc65d3a7cc1b26d21ddf4f",
  "source": "sources/w16-scene-b-font-dtor/font_dtor.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_reset_cache.o",
  "profile": "authored_cpp",
  "sha256": "bd5cd14c36bfb620d07fa931b33a0fc159a387d7dc002864b206865696fc71f5",
  "source": "sources/w16-scene-b-font-reset-cache/reset_cache.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_emissive_color.o",
  "profile": "authored_cpp",
  "sha256": "15eb8aac626c1dc1143de5495cc4a1fc3e176a884c514d97f04408b6e4f3d2f6",
  "source": "sources/w16-scene-b-material-set-emissive-color/set_emissive_color.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_layer_uv_rate.o",
  "profile": "authored_cpp",
  "sha256": "d3ae976192c6fc6b4859eb8552187f093a2ae214753ff53dd3a7f55d4450e723",
  "source": "sources/w16-scene-b-material-set-layer-uv-rate/set_layer_uv_rate.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_layer_uv_set.o",
  "profile": "authored_cpp",
  "sha256": "f436f711403abeecf369f270ac0859564aa76ad72497a89c23ea6a26f76034a2",
  "source": "sources/w16-scene-b-material-set-layer-uv-set/set_layer_uv_set.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_layer_uv_transform.o",
  "profile": "authored_cpp",
  "sha256": "2e542166ca30f4d094a3a53d19ad55aecd651e03ec9dfb4156fd05224bbcc293",
  "source": "sources/w16-scene-b-material-set-layer-uv-transform/set_layer_uv_transform.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_layer_wrap_mode.o",
  "profile": "authored_cpp",
  "sha256": "68be13a1731c90f8fb16f2f34d3841d341ce6da1dc5b99fb556fd18728b29067",
  "source": "sources/w16-scene-b-material-set-layer-wrap-mode/set_layer_wrap_mode.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_tessellate.o",
  "profile": "authored_cpp",
  "sha256": "52dc1416adc159a1595703741e51c9626a1ecc44bc61a7dd40432911c61eb6ab",
  "source": "sources/w16-scene-b-material-set-tessellate/set_tessellate.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_stretch_blit.o",
  "profile": "authored_cpp",
  "sha256": "3032e414ce9911c757beca9e9e799b3f368e27d41bba9128929e82a5a38de4bb",
  "source": "sources/w16-scene-b-raster-stretch-blit/stretch_blit.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_ref_map.o",
  "profile": "authored_cpp",
  "sha256": "c2aa5dd1527e9ef91533909ba8e98641ea3b50ca278e605edb775e8cf0e993a1",
  "source": "sources/w16-scene-b-scene-ref-map/ref_map.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_remove_all_actors.o",
  "profile": "authored_cpp",
  "sha256": "5afc9fddfdf932d5c00c8904ca21c5f992d0c0360b69cdc2f409b221771ad551",
  "source": "sources/w16-scene-b-scene-remove-all-actors/remove_all_actors.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_replace_proxy_rooms.o",
  "profile": "authored_cpp",
  "sha256": "2b6405b20e96a5881473843415ed9706f146acdea2313189a8fe09d2a90df8e1",
  "source": "sources/w16-scene-b-scene-replace-proxy-rooms/replace_proxy_rooms.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_replace_static_proxy_actors.o",
  "profile": "authored_cpp",
  "sha256": "45c02aa78cd2e928e4831ac6a9e8ce0efa1a265238b54447f5d4a9ba32f1e150",
  "source": "sources/w16-scene-b-scene-replace-static-proxy-actors/replace_static_proxy_actors.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_update_actor.o",
  "profile": "authored_cpp",
  "sha256": "b4f8ef887b14f230124b0c5bb65b48b13785d520fa376563a4e8695cd2de2aba",
  "source": "sources/w16-scene-b-scene-update-actor/update_actor.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_update_all_actors.o",
  "profile": "authored_cpp",
  "sha256": "b55584e59b15416a52185874433366fdc71299df60abe4e9557932098d93d184",
  "source": "sources/w16-scene-b-scene-update-all-actors/update_all_actors.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_release_resources.o",
  "profile": "authored_cpp",
  "sha256": "a52759f07b3fa156fb2a4bf887fea18369caee576057acc87eb3420bbeb5c8a9",
  "source": "sources/w16-scene-b-staticlighting-release-resources/release_resources.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_surface_ctor.o",
  "profile": "authored_cpp",
  "sha256": "14260ce9697155a0244da111592e9965012cf8f66fcdb7563eaa763b79e2429d",
  "source": "sources/w16-scene-b-surface-ctor/surface_ctor.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_reorder_palette.o",
  "profile": "authored_cpp",
  "sha256": "843e89dff3799b41de4ee9760f9cb2ea709212bb8f34d6076aa4e476d949c9d8",
  "source": "sources/w16-scene-b-surface-reorder-palette/reorder_palette.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_set_dma_headers.o",
  "profile": "authored_cpp",
  "sha256": "8fb0d2276a8b425dfa69469fa918d30c264ba9ab78ba752f42785886ce0e1016",
  "source": "sources/w16-scene-b-surface-set-dma-headers/set_dma_headers.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_query_intersection.o",
  "profile": "authored_cpp",
  "sha256": "59320fe335f6e309c3dd5d63e6863da194bd9125bb2eca2743e3a55ace7ba20c",
  "source": "sources/w16-scene-b-zone-query-intersection/query_intersection.cpp"
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w16_scene_b_render_leaf.o",
  "profile": "authored_cpp",
  "sha256": "57ddd0ad29dda22d31b47feadcbdbbf374ae70ba56c37d9fbfe5fe98dd732628",
  "source": "sources/w16-scene-b-zone-render-leaf/render_leaf.cpp"
 }
]
```

## Authored but never executed

All of it on the EE and in the game. The paired runs execute the original and authored code in a private interpreter with stand-in callees, so the callees' real behaviour (list iteration, dictionary lookup, DMA submission, matrix and box transforms, collision setup) was never exercised, and no frame, upload or blit has run. Interpreter limits: no cycle timing, no cache or DMA effects. Float paths in `StretchBlit`, `SetLayerUVRate` and `UpdateActor` were compared bit for bit on the scenario values used (including NaN/zero for the rate checks), not over all inputs. `memcpy` and `__builtin_delete` were stand-ins.

## Unknown

- Field names and sizes of `VIMaterial`, `VISurface`, `VIRFont`, `VIZone`, `VIScene` and `VIRaster` beyond the offsets used; sizes of `VISurface` (at least 316 bytes) and `VIPixelStore` (the stack slot is 8 bytes).
- What reads `SetTessellate`'s flag (mask 0x20), `SetLayerUVRate`'s flag (mask 0x4) and the wrap-mode bits, and which wrap each of values 0..3 names.
- Whether the `0xffffffef` in `SetLayerUVSet` is intended.
- The meaning of the format values 2 and 5 in `ReorderPalette`, and of resource type 4 in `ReplaceProxyRooms`.
- The meaning of `VIZone` words +68, +80, +84, +88, +92, +96 and of render-vars words +0, +24..+44 beyond how `RenderLeaf` and `QueryIntersection` use them.
- The flag semantics of `StretchBlit` other than bits 0, 1 and 2, and the VIAlphaBlendMode values.
- `UpdateAllActors`/`RemoveAllActors` call `Next` on a node that `RemoveActor` may already have unlinked in the remove case; whether that is safe depends on the list implementation, not authored here.
