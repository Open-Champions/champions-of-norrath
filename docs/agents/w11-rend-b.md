# Rendering, lighting, model and camera (2 of 2)

Package work is on `agents/w11-rend-b`. It supplies fourteen model, matrix and light routines and one data unit for the draw-frame, world-render, model and camera callers. Every function was compared with the original in the private paired-run harness (an R5900 interpreter that runs the original function and the freshly compiled authored function from identical memory, with identical stand-in callees). Each also has a host check beside the source. Host checks include host headers, so they are not EE build inputs.

Addresses and extents are from the pinned `SLUS_205.65`. Compiled body sizes come from the reviewed EE `authored_cpp` profile and differ from the original extents; none of this is a byte match.

## Evidence

Observed, inferred and unknown are kept in each source's header comment. Short form:

| Source directory | Symbol | Original | Checks |
| --- | --- | --- | --- |
| `w11-rend-b-model-set-fade` | `modelSetFade__Fc` | `0x16cbd8`, 8 | 7 byte values, memory compared |
| `w11-rend-b-model-set-relight-attachment` | `modelSetRelightAttachment__FG6Point3` | `0x16cbf8`, 80 | 60 random runs |
| `w11-rend-b-mat-untransform34-vec` | `matUntransform34Vec__FP6Point3P8Matrix34T0` | `0x159c38`, 160 | 400 runs, special floats, aliasing |
| `w11-rend-b-model-get-attachment-pos` | `modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3` | `0x16cb00`, 212 | 120 runs, hooked callees |
| `w11-rend-b-model-get-shadow-pos` | `modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf` | `0x16cd68`, 252 | 200 runs; divisor mutation caught |
| `w11-rend-b-model-get-char-attachment-matrix` | `modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f` | `0x1688c8`, 368 | 200 runs; mutation caught |
| `w11-rend-b-model-draw-attachment` | `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii` | `0x16c060`, 468 | 300 runs, 4 mutations caught |
| `w11-rend-b-mat-make-ypr-scale34-really-slow` | `matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf` | `0x157940`, 336 | 200 runs; mutation caught |
| `w11-rend-b-mat-invert44` | `matInvert__FP8Matrix44T0` | `0x157a90`, 744 | 600 runs plus the 0.0001 threshold; 3 mutations caught |
| `w11-rend-b-light-update-position` | `lightUpdatePosition__FP5Light` | `0x13eb60`, 192 | 400 runs; mutation caught |
| `w11-rend-b-light-setup-ambient-world` | `lightSetupAmbientWorld__FP6Packeti` | `0x13e160`, 208 | 40 runs; mutation caught |
| `w11-rend-b-light-setup-for-world` | `lightSetupForWorld__FP6PacketP13PolysetHeader` | `0x13e230`, 1160 | 500 runs; 6 mutations caught |
| `w11-rend-b-light-setup-for-character` | `lightSetupForCharacter__FP6PacketG6Point3` | `0x13d970`, 1240 | 600 runs; 5 of 6 mutations caught |
| `w11-rend-b-data-globals` | data objects (below) | symbol table | host sizes and zero values |

Observed facts worth repeating:

- `lightUpdatePosition` returns at once when the light's previous-link word is zero. A first reading missed that; the paired run caught it. It also does not clear the light's own links, unlike `lightAdd` (`0x13d230`).
- `matInvert__FP8Matrix44T0` is Gauss-Jordan elimination over columns with the pivot searched along a row. A pivot magnitude of at most `0.0001f` (bits `0x38d1b717`, a literal at `0x47fef4`) gives the identity. The boundary value itself is in the paired set.
- `lightSetupForWorld` and `lightSetupForCharacter` keep the nearest lights by a key of `(int)distance << 7 | index`, both starting at `0x1000001f`, and read `list[31]` (set to null after the search) for unused keys. `lightSetupForCharacter` reads the float literals `charScaleR/G/B` and the `1/32767` literal (`0x38000100` at `0x47fcc4`, used by `lightSetupForWorld`).
- `lightSetupAmbientWorld` writes a 144-byte packet containing the float `1.0e21` (bits `0x6258d726`, a literal at `0x47fcc0`).
- `modelDrawAttachment` closes the packet with the same tag logic as `modelDraw`. The count in a REF tag is the signed address difference shifted right 4, so it sign-extends into the upper word; the paired run covers `end < start`.
- `rsqrt.s`, `max.s` and `min.s` in `lightSetupForCharacter` are reproduced with the same inline-assembly idiom that the existing light sources use, guarded by `__mips__`.

Inferred (names only): the attachment, shadow and "relight" names come from the original symbol names; no behavior beyond the stores and calls shown is claimed.

## Data

`w11-rend-b-data-globals/data_globals.cpp` defines these zero-initialised objects. Sizes are symbol-table distances, not proven element types:

- `mt_dyndat` (`0x485c58`) and `mt_throwaway` (`0x485c5c`), 4 bytes each;
- `scaleTex` (`0x427248`), 28 bytes;
- `waterTexture` (`0x4841a0`), 4 bytes;
- `waterList` (`0x4869b8`) and `waterListBack` (`0x4869bc`), 4 bytes each;
- `splashInfoBits` (`0x601fd0`), 504 bytes (distance to `noiseTable`);
- `waveMainAlternate`, `waveRandomFoam`, `waveTrail`, `waveSplashBig`, `waveSplashSmall` and `waveFroth` (`0x600db0` to `0x600f40`), 80 bytes each.

`splashInfoBits` at `0x601fd0` is also covered by the existing view object `norrath_ctors_lava_wraith_view_601fd0` in `w9-ctors-wraith-d-views`; the two are separate objects under different names, so they do not clash, but the integrator may prefer one.

## Imports closed

- `lightSetupAmbientWorld__FP6Packeti`
- `lightSetupForCharacter__FP6PacketG6Point3`
- `lightSetupForWorld__FP6PacketP13PolysetHeader`
- `lightUpdatePosition__FP5Light`
- `matInvert__FP8Matrix44T0`
- `matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf`
- `matUntransform34Vec__FP6Point3P8Matrix34T0`
- `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`
- `modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3`
- `modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f`
- `modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf`
- `modelSetFade__Fc`
- `modelSetRelightAttachment__FG6Point3`
- `mt_dyndat`, `mt_throwaway`, `scaleTex`, `splashInfoBits`, `waterList`, `waterListBack`, `waterTexture`
- `waveFroth`, `waveMainAlternate`, `waveRandomFoam`, `waveSplashBig`, `waveSplashSmall`, `waveTrail`

Already defined by authored units that the current build plan does not select (their definitions only need plan entries; this package did not duplicate them):

- `lightIntensity` and `lightStart`: `w10-render-b-data-light-slots`;
- `marble`: `w10-render-b-data-inventory-textures`.

## Imports opened

Four functions that earlier packages and this one call, which no authored unit defines:

- `findLightsAtPoint__FG6Point3PP5Lighti` (already called by `lightCharacterShadow`; now also by `lightSetupForCharacter`)
- `findLightsInBox_forWorld__FG3BoxPP5Lighti`
- `modelSetupAttachment__FP6PacketP12_modelHeaderT1i`
- `worldQueryLightMap__FP12_worldHeaderG6Point3`

All other new references (`glistp`, `halos`, `nmHalos`, `modelOption`, `modelDrawFade`, `attachmentLightPos`, `saveValid`, `saveBestList`, `charAmbient`, `charDirect`, `charDirect2`, `charScaleR/G/B`, `charDirectDir`, `charDirectDir2`, `world`, `interfaceMode`, `disableVolumetricLighting`, `worldCameraMatrix`, `norrath_floor_light_grid_view`, `norrath_light_off_grid_head_view`, and the matrix, animation, draw and halo callees) are provided by authored units elsewhere in the tree. `attachmentLightPos` (`0x6e1d50`, 12 bytes) is defined by none of them except the host check; it is a data import this package opened.

## Not attempted

- `waterStep` (`0x187798`): hand-written assembly with MMI 128-bit operations (`lq`, `sq`, `paddsh`, `qfsrv`, `psrah`), scratchpad saves at `0x70003f00`, and use of `sp`, `gp` and `ra` as work registers. No C++ source is a faithful statement of it, and the harness interpreter has no MMI support. Left unresolved.
- `vqDecompress__FP8_texturei` (`0x1712f8`, 4,676 bytes): DMA channel and GIF packet building, polling of hardware registers at `0x10000000`, `SleepThread` waits and Huffman and block copy loops. Reading it was enough to see that a paired run would need hardware-register modelling that does not exist here. Not attempted.
- `megatileGenerate__FP12_worldHeader` (`0x25e3a8`, 19,680 bytes): far beyond one sitting; left for a dedicated owner.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. They add 14 to `default_authored_inputs`. All use profile `authored_cpp`, kind `independently_authored_source`, and the sources compile to these body sizes with the reviewed toolchain. The 7 units that include `math_types.hpp` list it in `header_closure`; the sha256 values are those of the committed sources. A private copy of the snapshot and plan, kept outside git, accepted these entries (`--check-plan`: 4,468 selected inputs, 5,200 source and header files) and was used for one private partial build.

```json
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_data_globals.o","profile":"authored_cpp","sha256":"5cd67166270dae55ead18385baee9cbdc47b7b4e765231ed0b15ceb24a7683e3","source":"sources/w11-rend-b-data-globals/data_globals.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"lightSetupAmbientWorld__FP6Packeti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_light_setup_ambient_world.o","profile":"authored_cpp","sha256":"3dd0f9d53148d044c644ccff02a30bc4f976cdb0d409f790cda993f0574b20fb","source":"sources/w11-rend-b-light-setup-ambient-world/light_setup_ambient_world.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1268,"name":"lightSetupForCharacter__FP6PacketG6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_light_setup_for_character.o","profile":"authored_cpp","sha256":"f430bbcce3e4f7218e00db9cd30811ac2a291062a6844300ddc686e8c9c07dbe","source":"sources/w11-rend-b-light-setup-for-character/light_setup_for_character.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1020,"name":"lightSetupForWorld__FP6PacketP13PolysetHeader"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_light_setup_for_world.o","profile":"authored_cpp","sha256":"45808bd3954e62ef1b1b6a40ac25d571dcecbc716f1fa04888487694ca88165c","source":"sources/w11-rend-b-light-setup-for-world/light_setup_for_world.cpp"}
{"emitted_functions":[{"compiled_body_bytes":428,"name":"lightUpdatePosition__FP5Light"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_light_update_position.o","profile":"authored_cpp","sha256":"2660b7ba454fecf4deb8db112f766b832c26e614378a83e320267d836e6b6e58","source":"sources/w11-rend-b-light-update-position/light_update_position.cpp"}
{"emitted_functions":[{"compiled_body_bytes":696,"name":"matInvert__FP8Matrix44T0"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_mat_invert44.o","profile":"authored_cpp","sha256":"5d9ca049b66e5bf2c8765794cd156dfad2d0e5788a599d0fd4ab1e89f12e87b5","source":"sources/w11-rend-b-mat-invert44/mat_invert44.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_mat_make_ypr_scale34_really_slow.o","profile":"authored_cpp","sha256":"92096d6b310ab94db485d4a0e946086b159a705a603c809982bab79ac1b829b0","source":"sources/w11-rend-b-mat-make-ypr-scale34-really-slow/mat_make_ypr_scale34.cpp"}
{"emitted_functions":[{"compiled_body_bytes":128,"name":"matUntransform34Vec__FP6Point3P8Matrix34T0"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_mat_untransform34_vec.o","profile":"authored_cpp","sha256":"151e9843b576ff94d78435ab9f345cc3b0b00ab0ba56b4650ec1abdd6d9564cf","source":"sources/w11-rend-b-mat-untransform34-vec/mat_untransform34_vec.cpp"}
{"emitted_functions":[{"compiled_body_bytes":576,"name":"modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_model_draw_attachment.o","profile":"authored_cpp","sha256":"e6362b13aeee4b33f0c90e4b25890a0ed3d214678756d677c3678f900bdb5295","source":"sources/w11-rend-b-model-draw-attachment/model_draw_attachment.cpp"}
{"emitted_functions":[{"compiled_body_bytes":160,"name":"modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_model_get_attachment_pos.o","profile":"authored_cpp","sha256":"e231b6ac29c9a65998893aa8b441b8875f781851e8b6d110f3033e19d0df7a4f","source":"sources/w11-rend-b-model-get-attachment-pos/model_get_attachment_pos.cpp"}
{"emitted_functions":[{"compiled_body_bytes":264,"name":"modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_model_get_char_attachment_matrix.o","profile":"authored_cpp","sha256":"13f66a31855e99222c06352cc2c90fff8c22b933d9b52264c6fdea4db7d1f035","source":"sources/w11-rend-b-model-get-char-attachment-matrix/model_get_char_attachment_matrix.cpp"}
{"emitted_functions":[{"compiled_body_bytes":316,"name":"modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_model_get_shadow_pos.o","profile":"authored_cpp","sha256":"4d2a1ef4547f2288d77980eb5c35c1b4531336d46c023890f56114e4d8e9d45b","source":"sources/w11-rend-b-model-get-shadow-pos/model_get_shadow_pos.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"modelSetFade__Fc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w11_rend_b_model_set_fade.o","profile":"authored_cpp","sha256":"6ed763c5501276ff3149d52082403f3d683194acd68fd731dc884f1da5143a0c","source":"sources/w11-rend-b-model-set-fade/model_set_fade.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"modelSetRelightAttachment__FG6Point3"}],"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_b_model_set_relight_attachment.o","profile":"authored_cpp","sha256":"cc47a0f20ce865fcd809828d92421ed8e82d5d756f4d29fba851bbeea39a3e21","source":"sources/w11-rend-b-model-set-relight-attachment/model_set_relight_attachment.cpp"}
```

## Authored but never executed

All fourteen units. The paired runs executed the compiled authored function inside a Python R5900 interpreter on this Mac, with Python stand-ins for callees; nothing ran on the EE or in the original process. In particular:

- the stand-in callees of `modelGetAttachmentPos`, `modelGetCharAttachmentMatrix`, `modelDrawAttachment`, `matMakeYPRScale34_ReallySlow`, `lightSetupForWorld` and `lightSetupForCharacter` are Python functions; the real callees (`animGetBoneMatrix`, `matMul`, `matRot`, `matTransform34Vec`, `findLightsAtPoint`, `findLightsInBox_forWorld`, `modelSetupAttachment`, `modelDrawHalo`, `worldQueryLightMap`, `drawAddToList`) were not run together with these callers;
- the uninitialised stack words that the original leaves in `lightSetupForWorld` and `lightSetupForCharacter` (list entries past the last light, apart from the terminator) were not sampled; removing `list[31] = nullptr` in `lightSetupForCharacter` was not caught because the harness stack starts zeroed;
- floating-point rounding mode, exception flags, denormal hardware mode, interrupts and cache behaviour were not exercised;
- the data objects have no consumers run against them.

## Unknown

- What the tag words in the light and attachment packets mean to the display-list consumer, and the units of the quad halfwords.
- Element types of `scaleTex` and `splashInfoBits`, and whether `waterList` and `waterListBack` are pointers or counters (their callers treat them as pointers).
- Why `lightAdd` clears a light's links and `lightUpdatePosition` does not, and whether any caller relies on the early return for an unlinked light.
- The meaning of the attachment record fields beyond the bone word, the matrix, the direction and the point.
- Whether the 16-byte gaps every 45 entries in the `modelGetShadowPos` table hold data.
- That these definitions place objects at the original addresses or section; they do not.
