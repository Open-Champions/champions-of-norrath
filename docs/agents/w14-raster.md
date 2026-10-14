# w14-raster: rasterizer, surfaces, clipping, vector and matrix math

Branch `agents/w14-raster`. One commit per function or data word, 36 commits, each pushed. Source is under
`experimental/sources/w14-raster-*`; every directory holds the authored source and a `host_check.cpp` (host headers, not a plan
input). Addresses and sizes come from the pinned private `SLUS_205.65` read locally with the reviewed EE binutils; no executable bytes,
disassembly listing or decompiler output is committed. Every source opens with an Observed / Inferred / Unknown comment. Compiled sizes
are those of the reviewed EE g++ with the `authored_cpp` flags and are not claims of byte identity.

## Result

Of the 38 assigned names, **36 are closed** and **2 are not authored** (see "Not done"). A link of the shared tip's partial object
(clean `build_partial.py` run at `a7bb4e51`: 6,235 inputs, 4,921 function bodies, **957 unresolved**, 0 COMMON) with the 36
objects is accepted by `ld -m elf32lr5900 -r` and leaves **950 unresolved**: 36 closed, 29 opened, 0 duplicate definitions.
`build_partial.py --check-plan` accepts a private plan (outside git) with the 36 entries added: 6,271 selected inputs. The full per-unit
driver build with these entries was not run; the closure check above is a link of objects, not the driver's per-unit compile and
function-inventory checks. `python3 -m unittest discover -s tools -p 'test_*.py'` runs 214 tests, all OK.

## Units

| unit | symbol | image address | image bytes | compiled bytes |
|---|---|---|---|---|
| `w14-raster-actorfilter-init` | `Init__13VIActorFilter` | `0x0105ec08` | 16 | 16 |
| `w14-raster-atmosphere-recomputelightningpatches` | `RecomputeLightningPatches__12VIAtmosphere` | `0x0106a020` | 620 | 776 |
| `w14-raster-bbox-bound` | `Bound__6VIBBoxiRC7VIVect3` | `0x01072480` | 164 | 152 |
| `w14-raster-bbox-store` | `Store__C6VIBBoxP7VIVect3` | `0x01071718` | 236 | 128 |
| `w14-raster-cameraeffect-reset` | `Reset__14VICameraEffect` | `0x01079ce8` | 24 | 24 |
| `w14-raster-clipstack-clipbytop` | `ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3` | `0x010990f0` | 688 | 584 |
| `w14-raster-clipstack-cullbytop` | `CullByTop__11VIClipStackRC8VISphere` | `0x01099588` | 152 | 160 |
| `w14-raster-clipstack-pushportal` | `PushPortal__11VIClipStackR8VICameraiP7VIVect3` | `0x01098730` | 712 | 536 |
| `w14-raster-collray-precalc` | `Precalc__9VICollRay` | `0x0109c528` | 232 | 212 |
| `w14-raster-eessurfcache-cache` | `Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi` | `0x011320f0` | 804 | 848 |
| `w14-raster-hsprite-calcnodetransform` | `CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2` | `0x0105fca0` | 432 | 392 |
| `w14-raster-material-constructdmapacket` | `ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord` | `0x010cec78` | 380 | 472 |
| `w14-raster-matrix33-assign` | `__as__10VIMatrix33RC10VIMatrix33` | `0x0108adb0` | 96 | 80 |
| `w14-raster-matrix44-invertaffine` | `InvertAffine__10VIMatrix44RC10VIMatrix44` | `0x010d0ab0` | 164 | 404 |
| `w14-raster-matrix44-submatrixmul` | `SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3` | `0x010d0a30` | 128 | 128 |
| `w14-raster-matrix44-transform` | `Transform__10VIMatrix44RC7VIVect3fT1` | `0x010999f0` | 436 | 364 |
| `w14-raster-plane-fromtriangle` | `__cl__7VIPlaneRC7VIVect3N21` | `0x010c2f90` | 344 | 228 |
| `w14-raster-raster-setgsdithermatrix` | `SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag` | `0x011149c0` | 216 | 180 |
| `w14-raster-raster-sharesurface` | `ShareSurface__8VIRasteri` | `0x011137e8` | 44 | 44 |
| `w14-raster-raster-surface` | `Surface__8VIRasteri` | `0x011137c8` | 28 | 28 |
| `w14-raster-raster-uploadpermaterial` | `UploadPerMaterial__8VIRasterPP10_sceDmaTag` | `0x01112310` | 336 | 236 |
| `w14-raster-scene-queryemitters` | `QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery` | `0x01143618` | 236 | 256 |
| `w14-raster-scene-releaseactor` | `ReleaseActor__7VIScenei` | `0x0113f058` | 148 | 152 |
| `w14-raster-scenefilter-init` | `Init__13VISceneFilter` | `0x01138310` | 28 | 28 |
| `w14-raster-sphere-init-from-bbox` | `Init__8VISphereRC6VIBBox` | `0x01005108` | 176 | 152 |
| `w14-raster-surface-lockmiplevel` | `LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc` | `0x0112c950` | 100 | 96 |
| `w14-raster-surface-unlockmiplevel` | `UnlockMipLevel__9VISurfacei` | `0x0112c9b8` | 32 | 40 |
| `w14-raster-vect3-normalizemagn` | `NormalizeMagn__7VIVect3` | `0x0104f360` | 112 | 104 |
| `w14-raster-weatherfilter-filteractor` | `FilterActor__13WeatherFilterR7VIScenei` | `0x0106b308` | 8 | 8 |
| `w14-raster-zone-renderbyvolume` | `RenderByVolume__6VIZoneR15VISceneRendVarsi` | `0x011192b8` | 120 | 96 |
| `w14-raster-zoneroom-allocate` | `Allocate__10VIZoneRoom` | `0x0111f5a8` | 72 | 72 |
| `w14-raster-zoneroom-calcdatasize` | `CalcDataSize__10VIZoneRoom` | `0x0111f578` | 48 | 48 |

The four statics (`_8VIRaster$PrimBuffer1`, `PrimBuffer2`, `RInitOffset`, `RFanOffset`) are 4-byte data words; their compiled `.data`
word was compared with the image word at the same address (`0x011b0194` = 0x15, `0x011b0198` = 0x13a, `0x011b01a8` = 0,
`0x011b01e8` = 0).

## How each unit was checked

* Compiled with the reviewed EE g++ and the plan's `authored_cpp` flags, one unit at a time (`-nostdinc`, freestanding).
* Native host check (`c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off host_check.cpp`, exit 0) with stubbed callees. Objects store
  32-bit target addresses, so units that follow pointer words use the arena convention of earlier packages
  (`NORRATH_HOST_ARENA`, `NORRATH_WORD_TO_POINTER`, `NORRATH_WORD_TO_FUNCTION`).
* Paired run in a private R5900 interpreter (outside git, with the shared `SLUS_205.65` loaded): the original function and the freshly
  compiled authored function run from identical memory snapshots with identical Python callee stubs that log their arguments (and,
  where it matters, the memory they see); call logs, return values (or `f0` for float returns) and every changed byte outside the
  stack must match. A halted run counts as a failure. Scenario counts and original-word coverage are in the commit messages; coverage
  is full except for nops, branch-delay leftovers and the unreachable divide-by-zero `break` words.
* Mutation: every unit had one or more deliberate edits (a constant, an offset, a comparison sense, a call or operand order, a stored
  value) and the paired run failed each time. Mutations must be applied to code, not the header comment, which repeats the same text.
* For `InvertAffine__10VIMatrix44RC10VIMatrix44` and `SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3` the interpreter models VU0 macro
  mode with round-to-nearest after each lane operation. Six VU0 instructions had to be added to the private model for `InvertAffine`
  (`vopmula`, `vopmsub`, `vdiv`, `vmulq`, `vmsubax/ay`, `vmsubz`); their semantics were taken from the instruction fields and checked by
  agreement with plain float arithmetic on 26 non-singular matrices. Hardware VU0 rounding is not modelled.

## Observed, inferred, unknown (short form; the source comments are the record)

* Observed: the clip stack (`VIClipStack`) keeps an entry count at +0, 16-byte entries at +8 (plane count, first plane index, vector
  count, first vector index), vector total at +12, plane total at +24, a vector array pointer at +20, a plane array pointer at +32 and
  two scratch vertex buffer pointers at +40 and +48. `CullByTop` tests a sphere against the top entry's planes; `ClipByTop` runs a
  polygon through them, alternating the two scratch buffers; `PushPortal` appends an entry made of the camera's first frustum plane, the
  eye, and one plane plus one projected vector per portal edge.
* Observed: `VIEESurfCache::Cache` handles a cached surface (LRU move, version compare, `Set`) and an uncached one (`CalcCacheInfo`,
  two `FindLRU`/`Allocate` rounds with an overlapping 16-byte-offset block, `Insert`, `Download`, `Set`), returning 0 or 1.
* Observed: `SetGSDitherMatrix` and `UploadPerMaterial` append four-quadword DMA/VIF/GIF packets; both start with a tag whose byte 3
  is 0x10 and halfword 0 is 3.
* Inferred: field and member names, meanings of the word at +32 of surface and material, the polygon-clip sign convention, that
  `Store` and `Transform` of VIBBox use the same corner order, that the four statics are VU1 offsets.
* Unknown: every callee body listed under "Imports opened", the size of most objects, hardware VU0 rounding, singular-matrix behaviour
  of `InvertAffine` (the divide is unchecked; the private interpreter's divide-by-zero result is not a model of VU0 or FPU saturation,
  so singular inputs are not compared), who writes the four statics.

## Not done

* `VIDMABillboardMicrocode` (image `0x011ee2d0`) and `VIDMARasterMicrocode` (image `0x011ee950`): each is a DMA chain that embeds a
  VU1 program. Reproducing it means copying the program bytes from the image, which is extracted proprietary code and not an
  independent authoring; no source was written. The owner of the VU1 microcode (or an independent re-implementation) has to provide
  them. Both stay unresolved.

## Imports closed

`Allocate__10VIZoneRoom`, `Bound__6VIBBoxiRC7VIVect3`, `Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi`, `CalcDataSize__10VIZoneRoom`, `CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2`, `ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3`, `ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord`, `CullByTop__11VIClipStackRC8VISphere`, `FilterActor__13WeatherFilterR7VIScenei`, `Init__13VIActorFilter`, `Init__13VISceneFilter`, `Init__8VISphereRC6VIBBox`, `InvertAffine__10VIMatrix44RC10VIMatrix44`, `LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc`, `NormalizeMagn__7VIVect3`, `Precalc__9VICollRay`, `PushPortal__11VIClipStackR8VICameraiP7VIVect3`, `QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery`, `RecomputeLightningPatches__12VIAtmosphere`, `ReleaseActor__7VIScenei`, `RenderByVolume__6VIZoneR15VISceneRendVarsi`, `Reset__14VICameraEffect`, `SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag`, `ShareSurface__8VIRasteri`, `Store__C6VIBBoxP7VIVect3`, `SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3`, `Surface__8VIRasteri`, `Transform__10VIMatrix44RC7VIVect3fT1`, `UnlockMipLevel__9VISurfacei`, `UploadPerMaterial__8VIRasterPP10_sceDmaTag`, `_8VIRaster$PrimBuffer1`, `_8VIRaster$PrimBuffer2`, `_8VIRaster$RFanOffset`, `_8VIRaster$RInitOffset`, `__as__10VIMatrix33RC10VIMatrix33`, `__cl__7VIPlaneRC7VIVect3N21`.

## Imports blocked (unresolved, not authored)

`VIDMABillboardMicrocode`, `VIDMARasterMicrocode`.

## Imports opened

These 29 names are referenced by the new units and are not defined by the shared selection plus this package (the numbers come from the
object link described above): `Allocate__13VIEESurfCacheR13VIEESurfAlloc`, `CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo`, `CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44`, `CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44`, `ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord`, `ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord`, `ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord`, `Decache__13VIEESurfCacheR9VISurface`, `Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag`, `Erase__t6VIList1Z7VIActori`, `FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster`, `GetTime__8VIRaster`, `Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo`, `Magnitude__C7VIVect3`, `MoveToLRUEnd__13VIEESurfCachei`, `PointInSolid__C8VISphereRC7VIVect3`, `ProcessPlayList__9VIHSpriteiR7VIScene`, `ProcessUpHierarchy__9VIHSpritei`, `Process__10VIMateriall`, `ReleaseSprite__7VIScenei`, `ReleaseStaticLighting__7VIScenei`, `RemoveActor__7VIScenei`, `RemoveFromLRU__13VIEESurfCachei`, `RenderByVolume__6VIZoneiR15VISceneRendVars`, `SetBindPose__9VIHSprite`, `Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass`, `Size__Ct6VIPool1Z13VIHSpritePlay`, `_8VIRaster$ColorMatrix`, `_8VIRaster$RMaterialOffset`.

`Magnitude__C7VIVect3` is used by `Init__8VISphereRC6VIBBox` and `Precalc__9VICollRay`. The two statics `_8VIRaster$ColorMatrix`
(initial word 0x0f) and `_8VIRaster$RMaterialOffset` (initial word 0) are neighbours of the four I define and belong to the
sprite/raster sibling package if it takes them. `_$_10VIMatrix44`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Mul__C10VIMatrix44RC7VIVect4R7VIVect4`,
`RotateHPR__10VIMatrix44RC7VIVect3`, `Normalize__7VIVect3`, `sinf`, `cosf`, `rand`, `__builtin_vec_new` and `SetPretranslations__6VIZoneR8VIRasterR8VICamera`
are used by the new units but already satisfied or already counted as unresolved before this package.

## Build-plan entries needed

Append these 36 entries (the order below is the discovery order; assign fresh `input_order` values and recompute file digests, the
`default_authored_inputs` count and the snapshot digest). The compiled body sizes are from the reviewed EE g++. `docs/agents/w14-raster-plan-entries.py --toolchain /path/to/ee [--private-root DIR]`
regenerates them and writes a private plan and snapshot copy. All have an empty `header_closure`.

```json
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "Init__13VIActorFilter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_actorfilter_init.o", "profile": "authored_cpp", "sha256": "12cddccb2486cb65dfd55feb3ff206dcaa5af4feb187c2ece3d48f9764e7aa92", "source": "sources/w14-raster-actorfilter-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 776, "name": "RecomputeLightningPatches__12VIAtmosphere"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_atmosphere_recomputelightningpatches.o", "profile": "authored_cpp", "sha256": "29472ad1f5143880bd94314b5f5f39f18bb5913d91aff8be39923ad5d8d59ed0", "source": "sources/w14-raster-atmosphere-recomputelightningpatches/recompute.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "Bound__6VIBBoxiRC7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_bbox_bound.o", "profile": "authored_cpp", "sha256": "8db48776aed726e46cb18d3593245214f2e0fa49b9589ca1222b3afbea32d3bc", "source": "sources/w14-raster-bbox-bound/bound.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "Store__C6VIBBoxP7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_bbox_store.o", "profile": "authored_cpp", "sha256": "180faea4efb379d6d6027f0a1c8b81cebe0e272ef6eb0052e2be28fc6ab78c2e", "source": "sources/w14-raster-bbox-store/store.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "Reset__14VICameraEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_cameraeffect_reset.o", "profile": "authored_cpp", "sha256": "72888273773ed9ae91428149ea091fde40489b3eaa2470dd8d17d42c89b24ef6", "source": "sources/w14-raster-cameraeffect-reset/reset.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 584, "name": "ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_clipstack_clipbytop.o", "profile": "authored_cpp", "sha256": "1f4f89bc8d9617c5072420f5f545dafcdd93650dc64cf05380f07d357072e0f4", "source": "sources/w14-raster-clipstack-clipbytop/clip_by_top.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "CullByTop__11VIClipStackRC8VISphere"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_clipstack_cullbytop.o", "profile": "authored_cpp", "sha256": "db6b1411f3ac6a9e82fd9df8e093a2d9eee586f4b3cb2767d3b87782a055e8aa", "source": "sources/w14-raster-clipstack-cullbytop/cull_by_top.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 536, "name": "PushPortal__11VIClipStackR8VICameraiP7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_clipstack_pushportal.o", "profile": "authored_cpp", "sha256": "f885d4d9b0c2b6763a556d870e7853196f3a0b8126798a77b90ac1373cb3fbbb", "source": "sources/w14-raster-clipstack-pushportal/push_portal.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 212, "name": "Precalc__9VICollRay"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_collray_precalc.o", "profile": "authored_cpp", "sha256": "04181bed71053a73c4ff3a7f906b92add9f17e51617c08dae62a845917825b34", "source": "sources/w14-raster-collray-precalc/precalc.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 848, "name": "Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_eessurfcache_cache.o", "profile": "authored_cpp", "sha256": "b2f91a2de3af30c945909dc11bf562efd34d29c0b052904fc61879bb6fd7cd10", "source": "sources/w14-raster-eessurfcache-cache/cache.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 392, "name": "CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_hsprite_calcnodetransform.o", "profile": "authored_cpp", "sha256": "7d3859dc30ae9e0a6b4e3e7f29317f9bc59839860506236c040f15509aa140c5", "source": "sources/w14-raster-hsprite-calcnodetransform/calc_node_transform.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 472, "name": "ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_material_constructdmapacket.o", "profile": "authored_cpp", "sha256": "d69d67daed6bb7582e827c6dbdaad12221a28b03cd3481e53c60cf0637730554", "source": "sources/w14-raster-material-constructdmapacket/construct.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "__as__10VIMatrix33RC10VIMatrix33"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_matrix33_assign.o", "profile": "authored_cpp", "sha256": "a4040cfc6adeb7296026db6994eefd877df40750ee191a3287157e9aa19a7613", "source": "sources/w14-raster-matrix33-assign/assign.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 404, "name": "InvertAffine__10VIMatrix44RC10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_matrix44_invertaffine.o", "profile": "authored_cpp", "sha256": "f2db710b67d517f7526da6000657bc2cb787a480405817e3bea7f1d820903a69", "source": "sources/w14-raster-matrix44-invertaffine/invert_affine.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_matrix44_submatrixmul.o", "profile": "authored_cpp", "sha256": "3272d00e73e9d1c06b16cbe99bbd40877fe6d69d752a03cea6f5289255131267", "source": "sources/w14-raster-matrix44-submatrixmul/submatrixmul.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 364, "name": "Transform__10VIMatrix44RC7VIVect3fT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_matrix44_transform.o", "profile": "authored_cpp", "sha256": "677546319eb23e0f1ecfe7df53b79caf8458e244482d33b206d6c4a7ff59a7b7", "source": "sources/w14-raster-matrix44-transform/transform.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 228, "name": "__cl__7VIPlaneRC7VIVect3N21"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_plane_fromtriangle.o", "profile": "authored_cpp", "sha256": "a7d3b8609e9af587e1f2eae2d67d8d6deeb3f21c54e080ae345bd32052b207b1", "source": "sources/w14-raster-plane-fromtriangle/from_triangle.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_primbuffer1.o", "profile": "authored_cpp", "sha256": "2ce2bf3460ee628ac673600ce6d24b6a63be413acbbf1ee9f538711b85a428d2", "source": "sources/w14-raster-raster-primbuffer1/prim_buffer1.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_primbuffer2.o", "profile": "authored_cpp", "sha256": "bdbf2659781aa885a996d232658c18dc1ddc2dcc98c826f12fbfe69454b0d377", "source": "sources/w14-raster-raster-primbuffer2/prim_buffer2.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_rfanoffset.o", "profile": "authored_cpp", "sha256": "bc1e689a64622c0b065bd2a50e373027f9b8ee366a522943048fa67276e54d2d", "source": "sources/w14-raster-raster-rfanoffset/r_fan_offset.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_rinitoffset.o", "profile": "authored_cpp", "sha256": "e25dcced9b6f2594c857d65ecfbe4944e70663eb3cc96bdaa9e05681895f4b2a", "source": "sources/w14-raster-raster-rinitoffset/r_init_offset.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 180, "name": "SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_setgsdithermatrix.o", "profile": "authored_cpp", "sha256": "cabdc686550ec2f553e883df9acb1266ebdadda94d66bdac7919584c0a196eb0", "source": "sources/w14-raster-raster-setgsdithermatrix/set_dither.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "ShareSurface__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_sharesurface.o", "profile": "authored_cpp", "sha256": "8e6bd6a53a928dd01bd9424065bf4d6102808664b253503cf8fe49d7d92259f0", "source": "sources/w14-raster-raster-sharesurface/share_surface.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Surface__8VIRasteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_surface.o", "profile": "authored_cpp", "sha256": "8b373cb83b83ed5725781e0a4117b0b3c4a6a03a68b6fe981ce50fc53209ba48", "source": "sources/w14-raster-raster-surface/surface.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "UploadPerMaterial__8VIRasterPP10_sceDmaTag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_raster_uploadpermaterial.o", "profile": "authored_cpp", "sha256": "b971a3cfb7fb22a237d597d06e6a81e567d59c724fdff32ecfd6856fd977ac3a", "source": "sources/w14-raster-raster-uploadpermaterial/upload_per_material.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 256, "name": "QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_scene_queryemitters.o", "profile": "authored_cpp", "sha256": "3fdf0eacdc8284e674befe482e253b6585645b59cf1eccbcdfaf2e2ba24db514", "source": "sources/w14-raster-scene-queryemitters/query_emitters.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "ReleaseActor__7VIScenei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_scene_releaseactor.o", "profile": "authored_cpp", "sha256": "b22056bf51f2d01707c3d0731b491dd03218bd0c4cdd721c24e554940d7ea892", "source": "sources/w14-raster-scene-releaseactor/release_actor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Init__13VISceneFilter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_scenefilter_init.o", "profile": "authored_cpp", "sha256": "0198a9dea9eea1e45555e95c2082b89f62113faf68b4f7bb2ac3d79485ced1e6", "source": "sources/w14-raster-scenefilter-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "Init__8VISphereRC6VIBBox"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_sphere_init_from_bbox.o", "profile": "authored_cpp", "sha256": "efbeb5f4ed9627d0136f568b7460755cab509d35bc629dd975b02f8ced8e47fc", "source": "sources/w14-raster-sphere-init-from-bbox/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_surface_lockmiplevel.o", "profile": "authored_cpp", "sha256": "e3aa23f22311325fa4edeab5f0d118dc45a283c1daa18d252e67dfdcb0478bd0", "source": "sources/w14-raster-surface-lockmiplevel/lock_mip_level.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "UnlockMipLevel__9VISurfacei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_surface_unlockmiplevel.o", "profile": "authored_cpp", "sha256": "7a64ffdab2482cbe17efaca0f078205ae64ecb9efdc69690faa8cf6dc744049b", "source": "sources/w14-raster-surface-unlockmiplevel/unlock.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "NormalizeMagn__7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_vect3_normalizemagn.o", "profile": "authored_cpp", "sha256": "ab1813c8c95b63574a718ba1b5af85e377e577183ed48487eb36df221ed03dce", "source": "sources/w14-raster-vect3-normalizemagn/normalize_magn.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "FilterActor__13WeatherFilterR7VIScenei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_weatherfilter_filteractor.o", "profile": "authored_cpp", "sha256": "4639a24e8f84c90261356f51048a54c7f7829013bf94d6e616463f3833b2f532", "source": "sources/w14-raster-weatherfilter-filteractor/filter_actor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "RenderByVolume__6VIZoneR15VISceneRendVarsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_zone_renderbyvolume.o", "profile": "authored_cpp", "sha256": "334921edbe9a572e00b197264bafe6552774f45b735240c14cfead3841352ae9", "source": "sources/w14-raster-zone-renderbyvolume/render_by_volume.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Allocate__10VIZoneRoom"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_zoneroom_allocate.o", "profile": "authored_cpp", "sha256": "b116a26a05c0e92a311569856045f7ed3ac46fa61c4eaf7fb51b0a473ebb22f8", "source": "sources/w14-raster-zoneroom-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "CalcDataSize__10VIZoneRoom"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w14raster_zoneroom_calcdatasize.o", "profile": "authored_cpp", "sha256": "bf0916e934deaa84b123c946b37e56af873ad22187461d222e9bedfb53d0758a", "source": "sources/w14-raster-zoneroom-calcdatasize/calc.cpp"}
```

## Authored but never executed

All 36 units. They were compiled with the reviewed toolchain, checked on the host with stubbed callees and compared in a private R5900
interpreter with stubbed callees. No routine ran on the EE, under the original process, against the DMA, VIF, GIF or VU hardware, or in
gameplay. Callees (listed under "Imports opened") were replaced by identical logging stubs on both sides, so their behaviour and the
state they would leave is not compared. The two 128-bit VU0 macro-mode units (`SubMatrixMul`, `InvertAffine` in its float form)
depend on the private model's rounding. `sinf`/`cosf` in `Transform` and `rand` in `RecomputeLightningPatches` were stubs.
The scratchpad reads of `UploadPerMaterial` (addresses `0x70000224` and `0x700001e0`) used ordinary interpreter memory.

## Unknown

* The bodies of the 29 opened imports and the meaning of most fields named only by offset.
* Whether the compiler's different instruction selection (several units compile larger or smaller than the image) matters for timing;
  no byte match is claimed for any unit.
* Whether `CullByTop`, `ClipByTop`, and `PushPortal` are ever called with an empty stack (they read before the entry array).
* The two microcode blobs (see above).
