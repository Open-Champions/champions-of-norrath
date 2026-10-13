# VI renderer, scene and world (2 of 3)

Package `w11-vi-render-b`. All 31 assigned imports are closed by 31 independently authored units under
`experimental/sources/w11-vi-render-b-*`, one function per unit and one commit per function. Addresses,
sizes and instruction order come from the pinned SLUS-20565 image, disassembled privately with the EE
binutils 2.45.1 (nothing from it is committed). Each source opens with an `Observed` / `Inferred` /
`Unknown` comment. Fresh compilations are not original-byte matches (the original was built by another
compiler and schedules differently), so sizes differ from the image's.

## Verification performed

- Every unit compiles with the `authored_cpp` flags of the reviewed EE toolchain. All 31 objects link
  together with `ld -r` without a duplicate definition, and the defined set is exactly the 31 assigned
  names. No full partial build was run (a private plan copy was not needed for this); the repository
  unit tests (`python3 -m unittest discover -s tools -p 'test_*.py'`, 214 tests) pass.
- Every unit was run against the original in a private R5900 interpreter (outside git): the same
  arguments and memory snapshot, callees replaced by logging stubs, comparing the call log
  (callee, arguments, and the memory they point at), every changed byte outside the stack and the
  result. Scratchpad (0x7000xxxx) and GS privileged-register (0x1200xxxx) stores are inside the compared
  memory. Mutating a stored offset, constant, or condition in an authored copy made the comparison fail
  for every unit that was tried (all but the one-line functions were mutated at least once).
  Scenario counts: the nine one-to-few-instruction functions 3 to 6 each; End2D 3; Begin2D 4;
  BeginTriStrip 18; Frustum 20 random float sets; SetOverrideMaterial 81 combinations; Flush 8;
  Vertex 12; CreateMaterial 12; SetMaterial 4; EndTriStrip 6; SetGSScissoring 14; ReleaseDeferred 14
  random list shapes (including null elements and a failing Erase); ReleaseSurface 98 (every
  combination of the four callee results); ReleaseMaterial 240; Flip 10; Clear 24 (each failing step in
  turn, twice; 216 of 219 instruction words reached, the rest are nops); Init 15 (each failing step, both
  display sizes and flag values; 220 of 221 words reached); the flora constructor 4 and Init 9;
  CalcOutdoorsIntensity 60 (pick hit or miss, previous time zero or not, clamp cases; 101 of 102 words);
  Update 400 random cached states and locations (336 of 337 words); Render 60 (164 of 164 words).
- Two interpreter additions were needed and are private: the vector-unit zeroing the image's
  `VIMatrix44` constructor inlines (`vsub.xyzw`, `sqc2`) and float arguments in f12 and up.
- A harness trap found along the way: a stubbed callee that returns a negative value must return it
  sign-extended to 64 bits (the image tests the full register with `bltz`/`bgez`). An early version of
  the scripts returned zero-extended values, so those negative branches were not actually exercised;
  the scripts were corrected and every unit with a negative branch was rerun and passes.
- Two behaviours the image has that a naive source loses, both handled and covered by the scenarios:
  `ReleaseDeferred` reads the element's word before it tests the element for null, and `Flip` loads the
  two display-buffer doublewords after the swap call.

## Imports closed (31)

`Init__19VIRadialFloraSystemR7VIScene`, `Render__19VIRadialFloraSystemR8VICamera`,
`Update__19VIRadialFloraSystemRC7VIVect3`, `__19VIRadialFloraSystem`, `Begin2D__8VIRaster`,
`BeginTriStrip__8VIRaster`, `Clear__8VIRaster`, `Color__8VIRasterRC10VIColor32F`,
`CreateMaterial__8VIRasteri`, `EnableAllLights__8VIRasteri`, `End2D__8VIRaster`,
`EndTriStrip__8VIRaster`, `Flip__8VIRaster`, `Flush__8VIRaster`,
`Frustum__8VIRasterR10VIMatrix44ffffff`, `InitSPRDoubleBuffer__8VIRaster`, `Init__8VIRaster`,
`Material__8VIRasteri`, `ReleaseDeferred__8VIRaster`,
`ReleaseMaterial__8VIRasteriR12VIDictionary`, `ReleaseSurface__8VIRasteriR12VIDictionary`,
`SetGSScissoring__8VIRaster`, `SetMaterial__8VIRasteri`,
`SetModelView__8VIRasterRC10VIMatrix44`, `SetOverrideMaterial__8VIRaster`,
`SetProjection__8VIRasterRC10VIMatrix44`, `SetStaticLighting__8VIRasterRC10VIColor32F`,
`SetTime__8VIRasterl`, `Vertex__8VIRasterRC8VIVect2I`,
`CalcOutdoorsIntensity__7VISceneR8VICamera`, `ClearShadowQueue__7VIScene`.

## Imports opened (referenced here, defined elsewhere or by nobody yet)

All of these are names the 31 units call or read; none is in this package. Three of them,
`_$_10VIMatrix44`, `__builtin_delete` and `sceGsSyncV`, are defined by other authored units that are
already in the plan; the rest should be compared with `unresolved-imports.json` after integration.

`AddMaterial__8VIRasteriPP10_sceDmaTag`, `Add__t6VIList1Z10VIMaterial`, `Add__t6VIList1ZP9VISurfaceRCP9VISurface`, `Begin__Ct6VIList1ZP12VIPrimBuffer`, `Begin__Ct6VIList1ZP9VISurface`, `Begin__Ct6VIPool1ZP12VIPrimBuffer`, `Begin__Ct6VIPool1ZP13VIColorBuffer`, `Begin__Ct6VIPool1ZP13VIMaterialPal`, `Begin__Ct6VIPool1ZP7VIRFont`, `Begin__Ct6VIPool1ZP9VISurface`, `CalcClipToScreen__8VIRaster`, `CalcMatrices__8VIRaster`, `CalcRadialLoc__19VIRadialFloraSystemG7VIVect3R16VIRadialFloraLoc`, `CalcZoneIndex__19VIRadialFloraSystemR16VIRadialFloraLoc`, `ClearDMADoubleBuffer__8VIRaster`, `ClearMaterialLRU__8VIRaster`, `Clear__13VIEESurfCache`, `Clear__19VIRadialFloraSystem`, `Clear__t6VIList1Z10VIMaterial`, `Clear__t6VIList1ZP12VIPrimBuffer`, `Clear__t6VIList1ZP9VISurface`, `Clear__t6VIPool1ZP12VIPrimBuffer`, `Clear__t6VIPool1ZP13VIColorBuffer`, `Clear__t6VIPool1ZP13VIMaterialPal`, `Clear__t6VIPool1ZP7VIRFont`, `Clear__t6VIPool1ZP9VISurface`, `CreateDefaultResources__8VIRaster`, `CreateSwatch__19VIRadialFloraSystemiR16VIRadialFloraLoc`, `Cull__C9VIFrustumRC6VIBBoxRC10VIMatrix44`, `Decache__13VIEESurfCacheR9VISurface`, `DrawPrimBuffer__8VIRasteriifUi`, `DrawSwatch__19VIRadialFloraSystemiR8VICamera`, `End__Ct6VIList1ZP12VIPrimBuffer`, `End__Ct6VIList1ZP9VISurface`, `End__Ct6VIPool1ZP12VIPrimBuffer`, `End__Ct6VIPool1ZP13VIColorBuffer`, `End__Ct6VIPool1ZP13VIMaterialPal`, `End__Ct6VIPool1ZP7VIRFont`, `End__Ct6VIPool1ZP9VISurface`, `Erase__t6VIList1Z10VIMateriali`, `Erase__t6VIList1ZP12VIPrimBufferi`, `Erase__t6VIList1ZP9VISurfacei`, `Erase__t6VIPool1ZP9VISurfacei`, `FlushCache`, `FlushDMABufferIfFull__8VIRaster`, `GetHitColor__13VIPickSegment`, `GetWorldGridNumCols__7VIScene`, `GetWorldGridNumRows__7VIScene`, `Identity__10VIMatrix44`, `InitDMADoubleBuffer__8VIRaster`, `InitGSRegisters__8VIRasterPP10_sceDmaTag`, `InitMaterialLRU__8VIRasteri`, `Init__12VIRasterTess`, `Init__13VIEESurfCacheii`, `Init__13VIPickSegmentRC7VIVect3T1`, `Init__t6VIList1Z10VIMaterial`, `Init__t6VIList1ZP12VIPrimBuffer`, `Init__t6VIList1ZP9VISurface`, `Init__t6VIPool1ZP12VIPrimBufferi`, `Init__t6VIPool1ZP13VIMaterialPali`, `Init__t6VIPool1ZP9VISurfacei`, `Mul__10VIMatrix44RC10VIMatrix44T1`, `Next__Ct6VIList1ZP12VIPrimBufferi`, `Next__Ct6VIList1ZP9VISurfacei`, `Next__Ct6VIPool1ZP12VIPrimBufferi`, `Next__Ct6VIPool1ZP13VIColorBufferi`, `Next__Ct6VIPool1ZP13VIMaterialPali`, `Next__Ct6VIPool1ZP7VIRFonti`, `Next__Ct6VIPool1ZP9VISurfacei`, `Pick__7VISceneR13VIPickSegmentR13VISceneFilter`, `QSortCompare__19VIRadialFloraSystemPCvT1`, `ReleaseSwatch__19VIRadialFloraSystemi`, `Remove__12VIDictionaryR10VIResource`, `Restore2D__8VIRaster`, `SetClipAndLight__8VIRasteri`, `SetDefaultLights__8VIRasterRC10VIMatrix44`, `SetFog__8VIRasteriffRC10VIColor32F`, `SetSolidFillMaterial__8VIRaster`, `Store2D__8VIRaster`, `SurfCache`, `UploadMatrices__8VIRasterPP10_sceDmaTag`, `UploadRasterMicro__8VIRasterPP10_sceDmaTag`, `VIEEDMASend__FiPv`, `VIEEDMASync__Fi`, `_$_10VIMatrix44`, `_$_12VIPrimBuffer`, `_$_13VIColorBuffer`, `_$_7VIRFont`, `_$_9VISurface`, `_$_t7VIArray1Zi`, `_8VIRaster$DBuffer`, `_8VIRaster$RStripOffset`, `__13VIPickSegment`, `__13VISceneFilter`, `__builtin_delete`, `_vt$21VISceneOutdoorsFilter`, `qsort`, `sceGsSwapDBuffDc`, `sceGsSyncPath`, `sceGsSyncV`, `sceVu0CopyMatrix`.

Data symbols among them: `SurfCache` (file-local in the image at 0x011eda68, also used by the w10-vi-b
raster units), `_8VIRaster$DBuffer` (0x01227f80, 0x330 bytes), `_8VIRaster$RStripOffset` (0x011b01b8,
4 bytes, read by `EndTriStrip`) and `_vt$21VISceneOutdoorsFilter` (0x01224280, 24 bytes, only its
address is taken by `CalcOutdoorsIntensity`). `QSortCompare__19VIRadialFloraSystemPCvT1`
(0x010d5d48) is passed by address to `qsort` by `Render__19VIRadialFloraSystemR8VICamera`.

## Build-plan entries needed

Profile `authored_cpp`, empty header closure, kind `independently_authored_source`, no host-check or
helper files are plan inputs. `input_order` is for the integrator to assign; `snapshot.json` must also
record each source. Neither shared file is edited here.

- `w11_vi_render_b_flora_ctor.o`, `sources/w11-vi-render-b-flora-ctor/ctor.cpp`, SHA-256 `d132f53dbebb21702f7bf509b66803d8db8086205c5a03cc0483048d23589a40`; one function `__19VIRadialFloraSystem`, 72 bytes in the reviewed compile
- `w11_vi_render_b_flora_init.o`, `sources/w11-vi-render-b-flora-init/init.cpp`, SHA-256 `74617e3159668ba1487f52c6ce3fe687b85c556cfaf11194d0df3ae2a8e1f54b`; one function `Init__19VIRadialFloraSystemR7VIScene`, 80 bytes in the reviewed compile
- `w11_vi_render_b_flora_render.o`, `sources/w11-vi-render-b-flora-render/render.cpp`, SHA-256 `10737f75db6037105140c47f7c397a2811536a1a92442c9611342ba828b91fff`; one function `Render__19VIRadialFloraSystemR8VICamera`, 544 bytes in the reviewed compile
- `w11_vi_render_b_flora_update.o`, `sources/w11-vi-render-b-flora-update/update.cpp`, SHA-256 `fbca218991fd1f34709279f8838c674f1a643d2bee738af96efac3f38648d368`; one function `Update__19VIRadialFloraSystemRC7VIVect3`, 1036 bytes in the reviewed compile
- `w11_vi_render_b_raster_begin_tri_strip.o`, `sources/w11-vi-render-b-raster-begin-tri-strip/begin_tri_strip.cpp`, SHA-256 `dc57b6598bae8bb7d8c74ac84518e764749efe303bd6423bb479ea79d27df853`; one function `BeginTriStrip__8VIRaster`, 176 bytes in the reviewed compile
- `w11_vi_render_b_raster_begin2d.o`, `sources/w11-vi-render-b-raster-begin2d/begin2d.cpp`, SHA-256 `60671f37f1c3457e50d10cb58df7ed162fe700622fd146f8ec7627e08b31a6ea`; one function `Begin2D__8VIRaster`, 172 bytes in the reviewed compile
- `w11_vi_render_b_raster_clear.o`, `sources/w11-vi-render-b-raster-clear/clear.cpp`, SHA-256 `a5b3fcd27dc86b09b6b96d1addce5c094d90f0bd77e461211b1bef36034ad3ba`; one function `Clear__8VIRaster`, 928 bytes in the reviewed compile
- `w11_vi_render_b_raster_color.o`, `sources/w11-vi-render-b-raster-color/color.cpp`, SHA-256 `31833897d97fda082bfc2199d4bbd54baa47d4962ae9615c8c5ba6ff89afd727`; one function `Color__8VIRasterRC10VIColor32F`, 40 bytes in the reviewed compile
- `w11_vi_render_b_raster_create_material.o`, `sources/w11-vi-render-b-raster-create-material/create_material.cpp`, SHA-256 `23db4f7e5f15965f2303be457d06023f399ab50722c5e6c86d5f0d42bb10b1a3`; one function `CreateMaterial__8VIRasteri`, 96 bytes in the reviewed compile
- `w11_vi_render_b_raster_enable_all_lights.o`, `sources/w11-vi-render-b-raster-enable-all-lights/enable_all_lights.cpp`, SHA-256 `c9b3887b50908177aa94fcde60e40a4c6d41c25f123ef34f389de9e14feac254`; one function `EnableAllLights__8VIRasteri`, 32 bytes in the reviewed compile
- `w11_vi_render_b_raster_end_tri_strip.o`, `sources/w11-vi-render-b-raster-end-tri-strip/end_tri_strip.cpp`, SHA-256 `4547cda97dbe9f8301fa7726df64a03fbaced460d486e73495d5590f1baff945`; one function `EndTriStrip__8VIRaster`, 188 bytes in the reviewed compile
- `w11_vi_render_b_raster_end2d.o`, `sources/w11-vi-render-b-raster-end2d/end2d.cpp`, SHA-256 `ba759bbe103a6441936c4b88e6eb8f5ff45433e1113bc7e3c8ff864f535afc40`; one function `End2D__8VIRaster`, 36 bytes in the reviewed compile
- `w11_vi_render_b_raster_flip.o`, `sources/w11-vi-render-b-raster-flip/flip.cpp`, SHA-256 `8f962113351d55fb1befc35424cc8e4984a04fda11d68cfec7b891f7d8a652ca`; one function `Flip__8VIRaster`, 220 bytes in the reviewed compile
- `w11_vi_render_b_raster_flush.o`, `sources/w11-vi-render-b-raster-flush/flush.cpp`, SHA-256 `73d2824690396fdfa7f4010e8c1fe5d1ba81008f3600960750181821bd4eee54`; one function `Flush__8VIRaster`, 208 bytes in the reviewed compile
- `w11_vi_render_b_raster_frustum.o`, `sources/w11-vi-render-b-raster-frustum/frustum.cpp`, SHA-256 `bca40a056db7546d687b12ec2eb3fd6a5c852527cdfe2fda93f119c789e5022b`; one function `Frustum__8VIRasterR10VIMatrix44ffffff`, 144 bytes in the reviewed compile
- `w11_vi_render_b_raster_init.o`, `sources/w11-vi-render-b-raster-init/init.cpp`, SHA-256 `522d0e0de12e885afd2bde44371a4afefd3e5490126d25309005d351bcf6dc5d`; one function `Init__8VIRaster`, 816 bytes in the reviewed compile
- `w11_vi_render_b_raster_init_spr_double_buffer.o`, `sources/w11-vi-render-b-raster-init-spr-double-buffer/init_spr_double_buffer.cpp`, SHA-256 `6f5c293bf6aeae4d18a4bde2e048c44cc7f6eb45aadd1135f60d4eb6dd7c9ffe`; one function `InitSPRDoubleBuffer__8VIRaster`, 28 bytes in the reviewed compile
- `w11_vi_render_b_raster_material.o`, `sources/w11-vi-render-b-raster-material/material.cpp`, SHA-256 `f87702ffc0f1d911ca3b76cf9914ebee5e110e97a49534be8b2a40267ee934c6`; one function `Material__8VIRasteri`, 8 bytes in the reviewed compile
- `w11_vi_render_b_raster_release_deferred.o`, `sources/w11-vi-render-b-raster-release-deferred/release_deferred.cpp`, SHA-256 `a1bc5114bf987951971f95b75f6cb8a7a6f07f23b1649fb0681a2491590f47f2`; one function `ReleaseDeferred__8VIRaster`, 368 bytes in the reviewed compile
- `w11_vi_render_b_raster_release_material.o`, `sources/w11-vi-render-b-raster-release-material/release_material.cpp`, SHA-256 `a50145bb5c4b1b466d79b363e4aaa553a35e7adc235744e21431e33b485e8ea7`; one function `ReleaseMaterial__8VIRasteriR12VIDictionary`, 232 bytes in the reviewed compile
- `w11_vi_render_b_raster_release_surface.o`, `sources/w11-vi-render-b-raster-release-surface/release_surface.cpp`, SHA-256 `e9a8051cd0cbe61e8fff42df04a2938d6d4c9a70bc9b3ebdb036328e977eb007`; one function `ReleaseSurface__8VIRasteriR12VIDictionary`, 248 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_gs_scissoring.o`, `sources/w11-vi-render-b-raster-set-gs-scissoring/set_gs_scissoring.cpp`, SHA-256 `83999c2360d1be6ee37abc271e799226cd25674fc136b66ee4194eb7a1ccb80c`; one function `SetGSScissoring__8VIRaster`, 168 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_material.o`, `sources/w11-vi-render-b-raster-set-material/set_material.cpp`, SHA-256 `7fabbd4b77e48268f9f1a420925e29f20e9c7afa645a01e009b56fae15286a71`; one function `SetMaterial__8VIRasteri`, 64 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_model_view.o`, `sources/w11-vi-render-b-raster-set-model-view/set_model_view.cpp`, SHA-256 `f1e027f1b38dceaa4199768aad41cd94eadc12b88c9285350b545ca357a0d2d1`; one function `SetModelView__8VIRasterRC10VIMatrix44`, 56 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_override_material.o`, `sources/w11-vi-render-b-raster-set-override-material/set_override_material.cpp`, SHA-256 `3b8ece3b9c1314ff12d2df549fee08bb2611a7acca2f6dc3e1021bc37cde7545`; one function `SetOverrideMaterial__8VIRaster`, 160 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_projection.o`, `sources/w11-vi-render-b-raster-set-projection/set_projection.cpp`, SHA-256 `532750719b3dcf67f95b02a5ee3c071c36b68ed445535d03b0e97e90eceddea0`; one function `SetProjection__8VIRasterRC10VIMatrix44`, 60 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_static_lighting.o`, `sources/w11-vi-render-b-raster-set-static-lighting/set_static_lighting.cpp`, SHA-256 `c8fa555cdaf0089600466b9e1e2cb909efd19efc2b093bfb46254b72af44aeb6`; one function `SetStaticLighting__8VIRasterRC10VIColor32F`, 36 bytes in the reviewed compile
- `w11_vi_render_b_raster_set_time.o`, `sources/w11-vi-render-b-raster-set-time/set_time.cpp`, SHA-256 `9f043a5e91010cf0c58768d6bc2db106bc5f16ae8acf66d700fe66ec9fb9a09b`; one function `SetTime__8VIRasterl`, 12 bytes in the reviewed compile
- `w11_vi_render_b_raster_vertex.o`, `sources/w11-vi-render-b-raster-vertex/vertex.cpp`, SHA-256 `9e2b964133d0f668dddbae0291044af1ea97ef2e216f80793ac3a2a4f420325f`; one function `Vertex__8VIRasterRC8VIVect2I`, 212 bytes in the reviewed compile
- `w11_vi_render_b_scene_calc_outdoors_intensity.o`, `sources/w11-vi-render-b-scene-calc-outdoors-intensity/calc_outdoors_intensity.cpp`, SHA-256 `651641a2224c215f4cd70748d6ea70a1289ba011fe0f6b66961604ae34c696ae`; one function `CalcOutdoorsIntensity__7VISceneR8VICamera`, 432 bytes in the reviewed compile
- `w11_vi_render_b_scene_clear_shadow_queue.o`, `sources/w11-vi-render-b-scene-clear-shadow-queue/clear_shadow_queue.cpp`, SHA-256 `e1c7d5a92cc5af53372e1d009e08e5a884d634d827ae5bf266e02b1de987b9ef`; one function `ClearShadowQueue__7VIScene`, 8 bytes in the reviewed compile

## Authored but never executed

All 31 functions. They have run only in the private interpreter against logging stubs for their callees
(the list, pool, DMA, GS, matrix and picking members they call are fabricated, as are the
`Begin`/`End`/`Next` iterator semantics, which are modelled as indexes for pools and slot addresses for
lists). Nothing has run on the EE, against a real `VIRaster`, `VIScene` or `VIRadialFloraSystem`
object, or as part of the startup chain. The scratchpad and GS register stores have never touched real
hardware.

## Unknown

- The sizes and member names of `VIRaster` (about 19.5 KB), `VIScene`, `VIRadialFloraSystem`, a
  surface, a material and a layer record, beyond the offsets used in the source comments. Raster
  offsets seen: 12 to 28 and 32 to 44 (current vertex attributes and color), 48 to 56 (strip count and
  cursors), 17716 to 17728 (scissor rectangle), 17732 to 17768 (half size, reciprocals), 17804 to 17816
  (override material state), 17824 to 17856 (cached ids reset to -1 by `Clear`), 17872 (time), 17880
  (swap counter), 17884 to 17896 (mode and dirty words), 19480 to 19500 (DMA double buffer words), 19492
  (DMA write cursor), 19536 (a byte flag).
- What the scratchpad block at 0x70000200 to 0x7000022c holds beyond what is stored (static lighting
  at +0, the enable bytes at +0x20 to +0x22, 0.0f at +0x10 to +0x1c, 1.0f at +0x28, the float with
  bits 0x3e199999 at +0x2c), and what the two scratchpad matrices at 0x70000000 and 0x70000040 are
  (inferred model-view and projection).
- The VIF and GIF command values in `EndTriStrip` (0x14000000, 0x6c008000, 0x10 in the tag), in
  `SetGSScissoring` (0x10000000, 0x50000003, 0x1000000000008002, registers 64 and 65) and in `Flush`
  (112 in the tag). They are kept as constants; the register readings are inferences.
- `Flip`'s values written to 0x12000000 (32547), 0x120000a0 and 0x12000080 are observed bit patterns;
  that they are GS PMODE/DISPLAY/DISPFB writes is inferred from the standard map.
- What the fourth component of the pick's hit color is for `CalcOutdoorsIntensity` (it is used as an
  intensity), and the time unit (the constant is close to 1/1000, times 0.2).
- The radial flora's record layout: the five-word location (zone and two coordinate pairs), the
  offsets 25 and 50, and that word 1 of the system is passed to `SetModelView` and
  `DrawPrimBuffer` as if it were the raster (so the scene word at 19356 that `Init` copies there is
  most likely the scene's raster pointer; this is inferred from `Render` only).
- Whether `Material__8VIRasteri` is a real function with no body of interest (it returns its
  argument) or the residue of a removed feature.
