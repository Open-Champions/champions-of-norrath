# Render, matrix, light, texture and particle (b) (w9-render-math-b)

Branch `agents/w9-render-math-b`. New source is under `experimental/sources/w9-render-math-b-*`; the shared header is `w9-render-math-b-common/math_types.hpp`. Reference executable: the private `SLUS_205.65` named in the docs, read locally only. No game bytes, listings or decompiler output are committed. Addresses and extents below come from that file's symbol table, and the reviewed EE `objdump` was used to read each function.

Nothing here has run on the EE. Every unit compiles with the reviewed `authored_cpp` profile. Each function has a `host_check.cpp` that compiles with the host compiler and exits non-zero when a store, call order, bound, result or branch is wrong; host checks include host headers and are not build-plan inputs. EE objects are not original-byte matches and no match is claimed.

## What was checked

- Every function was also compared against the original in a private paired run: the original function and the authored object were each executed in a small R5900 interpreter from identical memory snapshots, with the same callees (the original's own, or a stub that logs and returns the same data for both), and the return value (or float result), the callee call log and every changed byte outside the stack were compared. Scenarios mixed random and boundary floats (zero, negative zero, 2^24, 3e38), aliased pointers, null and non-null optional arguments, and every sign case of the integer arguments. Deliberately mutated copies of `matMakeTransYawScale`, `matMakeViewMatrix`, `modelGetCharAttachmentPos`, `modelGetShadowPos`, `makeExplosion` and `modelDrawShadow` were also run and failed. For `modelDrawShadow` the glist buffer was pre-filled with a non-zero byte so unwritten or unpadded bytes would show, and 1390 of the original's 1420 instructions were executed (the rest are alignment-padding loops that cannot run on an aligned cursor, an unreachable not-CNT branch, two nops and a few clip sub-branches). The interpreter and harness live outside git and are not published.
- The interpreter's `rsqrt.s` is a double-precision divide and square root rounded to float; it is the same for both sides of every comparison, but it is not the R5900's reduced-precision unit. Results of `makeExplosion` therefore match the original under that model; hardware precision is untested.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Compiled with the portable build's exact flags (see `docs/PORTABLE_BUILD.md`) per unit. A full partial build was not run on this branch; the sizes below are from single-unit compiles.

## Functions and data

| unit directory (`experimental/sources/w9-render-math-b-…`) | emitted symbol | image address | image extent | EE body bytes |
|---|---|---|---|---|
| `lilog2` | `lilog2__Fl` | 0x15a030 | 64 | 64 |
| `light-set-ambient-color` | `lightSetAmbientColor__FPs` | 0x13e7a8 | 40 | 76 |
| `light-set-ambient-offset` | `lightSetAmbientOffset__FPs` | 0x13e730 | 116 | 68 |
| `light-set-ambient-only` | `lightSetAmbientOnly__Fv` | 0x13e7d0 | 52 | 96 |
| `mat-ident34` | `matIdent__FP8Matrix34` | 0x1590c0 | 60 | 60 |
| `mat-make-trans` | `matMakeTrans__FP8Matrix34G6Point3` | 0x15a248 | 104 | 72 |
| `mat-trans` | `matTrans__FP8Matrix34fff` | 0x159550 | 136 | 136 |
| `mat-untransform33` | `matUntransform__FP6Point3P8Matrix33T0` | 0x1597e0 | 160 | 128 |
| `mat-transform44` | `matTransform__FP6Point3P8Matrix44T0` | 0x159920 | 220 | 188 |
| `mat-make-view-matrix` | `matMakeViewMatrix__FP8Matrix44fffffff` | 0x159390 | 128 | 112 |
| `mat-make-trans-yaw-scale` | `matMakeTransYawScale__FP8Matrix34G6Point3sf` | 0x1577d0 | 364 | 384 |
| `mat-mul34` | `matMul__FP8Matrix34N20` | 0x1564b8 | 344 | 252 |
| `mat-mul44` | `matMul__FP8Matrix44P8Matrix34T0` | 0x156348 | 368 | 252 |
| `mat-rot34` | `matRot__FP8Matrix34is` | 0x156cb0 | 520 | 324 |
| `model-set-halo` | `modelSetHalo__FP7HaloDefi` | 0x16cbe8 | 12 | 20 |
| `model-set-no-z-write` | `modelSetNoZWrite__Fv` | 0x16cc68 | 16 | 20 |
| `model-set-shadow-scale` | `modelSetShadowScale__Ff` | 0x16cbe0 | 8 | 12 |
| `model-get-nm-shadow-points` | `modelGetNmShadowPoints__FP12_modelHeader` | 0x16cd50 | 20 | 20 |
| `model-get-char-attachment-pos` | `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f` | 0x168a38 | 464 | 508 |
| `model-get-shadow-pos` | `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f` | 0x16c238 | 780 | 696 |
| `make-explosion` | `makeExplosion__FG6Point3f` (and one local helper) | 0x1d98b8 | 1504 | 892 (+196) |
| `model-draw-shadow` | `modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec` (and one local helper) | 0x169848 | 5680 | 3584 (+156) |

Data units (no function bodies): `map-scissor` defines `mapScissorX` (0x486b10) and `mapScissorY` (0x486b14); `megatile-seed-rot` defines `megatileSeed` (0x485c64) and `megatileRot` (0x485c68); `camera-angles` defines `myPitch` (0x48431e, 2 bytes, file value 0x2710), `myRoll` (0x484320, 2 bytes, file value 0) and `myFOV` (0x484324, 4 bytes, file value 0x42480000, the float 50.0).

Order of commits is the order of the table, except `modelGetNmShadowPoints` was corrected after `modelGetShadowPos` showed its result is a count (see below).

## Observed

- `lilog2__Fl`: negative input returns 0; otherwise the number of one-bit arithmetic right shifts after the first that it takes to reach zero, i.e. floor(log2(v)) for v >= 1 and 0 for 0 and 1.
- `lightSetAmbientColor__FPs` copies four halfwords to `charAmbient` (0x486850). `lightSetAmbientOffset__FPs` adds four signed halfwords to the four bytes at `scene`+48..51 and stores each sum clamped below at 0 (no upper clamp) to `charAmbient`. `lightSetAmbientOnly__Fv` stores 1 to `ambientOnly` and zeroes `charDirect` and `charDirect2` (it does not touch `charAmbient`).
- Matrix routines: `matMakeTrans`, `matTrans`, `matUntransform`, `matTransform` (Matrix44), `matMakeViewMatrix`, `matMakeTransYawScale`, `matMul` (both) and `matRot` (Matrix34) use the float orders written in each source header comment; the host checks compare bit for bit with floating-point contraction off. In the mangled names `N20` and `T0` repeat an earlier parameter's type, so `matMul__FP8Matrix44P8Matrix34T0` is (Matrix44 out, Matrix34 a, Matrix44 b).
- `matTransform__FP6Point3P8Matrix44T0` returns a float: the fourth column sum, which it computes and does not store.
- `modelGetShadowPos` compares the word that `modelGetNmShadowPoints` returns, signed, with an index, so that word is a count. Records are 8 bytes with one 16-byte step per 45 records (signed division by 45).
- `modelDrawShadow` emits GS display-list packets (state setup, one matrix/sprite/reference/textured-quad group per strong light, state restore); the packet layout, register numbers, flag semantics and every constant are in the source header. The `movz` in the light-threshold loop selects the largest intensity (threshold = max(1, intensities)), not the smallest.
- `makeExplosion` generates 44 + 40*n particles with an inline linear congruential generator (`s = s*0x19660d + 0x3c6ef35f`, high halfword) on `eRandState`; details and constants are in the source header.

## Inferred

- Names are the executable's. Behavioural readings in the source comments (identity/translation matrices, shadow table, explosion trail under gravity, attachment point) are inferences from operations and names, marked as such there.
- `modelGetCharAttachmentPos` and `modelGetShadowPos` read the same kind of per-model table as `modelGetNmShadowPoints`, but their record layouts differ (52-byte records at header + 16*byte(+34) versus 8-byte records at header + 16*byte(+36) + 32).

## Imports closed

lilog2__Fl, lightSetAmbientColor__FPs, lightSetAmbientOffset__FPs, lightSetAmbientOnly__Fv, makeExplosion__FG6Point3f, mapScissorX, mapScissorY, matIdent__FP8Matrix34, matMakeTransYawScale__FP8Matrix34G6Point3sf, matMakeTrans__FP8Matrix34G6Point3, matMakeViewMatrix__FP8Matrix44fffffff, matMul__FP8Matrix34N20, matMul__FP8Matrix44P8Matrix34T0, matRot__FP8Matrix34is, matTrans__FP8Matrix34fff, matTransform__FP6Point3P8Matrix44T0, matUntransform__FP6Point3P8Matrix33T0, megatileRot, megatileSeed, modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f, modelGetNmShadowPoints__FP12_modelHeader, modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f, modelSetHalo__FP7HaloDefi, modelSetNoZWrite__Fv, modelSetShadowScale__Ff, myFOV, myPitch, myRoll, modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec.

Already closed on the branch by `w8-object-list-math` before this package started (no new work): matIdent__FP8Matrix33, matMakeTransYaw__FP8Matrix34G6Point3s, matRot__FP8Matrix33is, matToQuaternion__FP4QuatP8Matrix34, matTransform34Vec__FP6Point3P8Matrix34T0, matTransform__FP6Point3P8Matrix33T0.

## Imports opened

- `animGetBoneMatrix__FP14AnimationStateiP8Matrix34` (0x1627a0): `modelGetCharAttachmentPos`, `modelGetShadowPos`.
- `matTransform__FP6Point3P8Matrix34T0` (0x159ae0): same two.
- `matPostRot__FP8Matrix33is` (0x156aa0): `modelGetShadowPos`.
- `P_AddParticle__FP11ParticleDefP6Point3T1f` (0x180da0): `makeExplosion`. (Probably already provided by `loop-grid-fx-add-particle`; name matches that unit's emitted symbol.)
- `fptodp`, `dpsub`, `dptofp` (0x2fb320, 0x2fb618, 0x2fbc20): `makeExplosion`. Already on the unresolved list of the w9-startup-crt packages.
- `ballParticle` (0x7d6010) and `trailParticle` (0x7d6060): `makeExplosion`. In the original both are file-local objects (the executable has two different `ballParticle` and two different `trailParticle` locals), and `w8-fx-particles-init-fx` defines them as file-local statics too, so no other unit can link to them. They stay unresolved until one definition is exported under a name that both units share.
- `modelDrawShadow` also calls `animLoadBoneMatricies__FP6PacketP14AnimationState` (0x1628e0), `lightCharacterShadow__FG6Point3P11LightResult` (0x13d3c0), `matScale__FP8Matrix44f` (0x1595d8), `matTrans__FP8Matrix44fff` (0x159648), `worldFindHeight__FP12_worldHeaderG6Point3f` (0x176378), `get__11ResourceMap` (0x166f48) and `strcmp`, and references `worldViewMatrix`, `world`, `worldCameraPos`, `worldCameraMatrix`, `engineCurrentLevelName` and the engine* words by their original names; of these only the functions above were not found among the plan's emitted functions (the plan already emits `__11ResourceMapPcT1`, `drawAddToList__FPviP8_textureP11_drawRecordi` and `matTransform__FP6Point3P8Matrix44T0` from this package). Whether each data object has a definition on the merged branch was not checked with a full build.
- Already provided by authored units and not opened: `icos__Fi`, `isin__Fi`, `matIdent__FP8Matrix33`, `eRandState`, `charAmbient`, `charDirect`, `charDirect2`, `ambientOnly`, `halos`, `nmHalos`, `modelOption`, `modelShadowScale`. Declared but not defined by this package: `scene` (0x4f4060).

## Build-plan entries needed

Append these 25 authored inputs and assign fresh `input_order` values (they are in alphabetical order of directory, not in a required order). Each `header_closure` lists the headers the unit includes; `files` needs entries for those headers, including the new `sources/w9-render-math-b-common/math_types.hpp` (sha256 `7185ef1b3277757da6f213ff0a470f9fa9bbc29c982ed3ef8a392353d9a4b1c9`) and every `.cpp` below. Do not add any `host_check.cpp`: each includes host headers. The `compiled_body_bytes` are from the reviewed EE g++ `authored_cpp` profile, not from the original executable. `default_authored_inputs` increases by 25. The `mat-make-trans-yaw-scale` unit includes `sources/w8-object-list-math-common/trig_core.hpp`; `mat-rot34` calls `icos__Fi` and `isin__Fi`.

```json
{"header_closure":[],"kind":"independently_authored_source","object_name":"camera_angles.o","profile":"authored_cpp","sha256":"b90459b7c8726f9c321e2c201cbed8c6e034d64cc37e5b26554a24eb72c52ae8","source":"sources/w9-render-math-b-camera-angles/camera_angles.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"light_set_ambient_color.o","profile":"authored_cpp","sha256":"4589525fa275433bf991d24b3fab45453c8fbd9363eafbbfc8694bce013951fc","source":"sources/w9-render-math-b-light-set-ambient-color/light_set_ambient_color.cpp","emitted_functions":[{"name":"lightSetAmbientColor__FPs","compiled_body_bytes":76}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"light_set_ambient_offset.o","profile":"authored_cpp","sha256":"b78f2b17eec862260cf155a8a58f24023e8a40eb83804e33671fb8a242ee525b","source":"sources/w9-render-math-b-light-set-ambient-offset/light_set_ambient_offset.cpp","emitted_functions":[{"name":"lightSetAmbientOffset__FPs","compiled_body_bytes":68}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"light_set_ambient_only.o","profile":"authored_cpp","sha256":"d7d0195012d1b5088a7239a616af7dcd184c7de9e8e8550aa25b920a3adcc2c6","source":"sources/w9-render-math-b-light-set-ambient-only/light_set_ambient_only.cpp","emitted_functions":[{"name":"lightSetAmbientOnly__Fv","compiled_body_bytes":96}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"lilog2.o","profile":"authored_cpp","sha256":"98d9f12539418281ee407fc76a8e45a09a182d1680b3ae536c63493cc2f9852d","source":"sources/w9-render-math-b-lilog2/lilog2.cpp","emitted_functions":[{"name":"lilog2__Fl","compiled_body_bytes":64}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"make_explosion.o","profile":"authored_cpp","sha256":"3d50257f745a1590668311f07ec5794e158fc0081022ec535a20c4f6a08cc64b","source":"sources/w9-render-math-b-make-explosion/make_explosion.cpp","emitted_functions":[{"name":"_ZN12_GLOBAL__N_1L9ballPointERjR6Point3","compiled_body_bytes":196},{"name":"makeExplosion__FG6Point3f","compiled_body_bytes":892}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"map_scissor.o","profile":"authored_cpp","sha256":"210137bc4315e267d23ad663f26524a9959f5878f3c2132a74aa25f5fee2294f","source":"sources/w9-render-math-b-map-scissor/map_scissor.cpp","emitted_functions":[]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"mat_ident34.o","profile":"authored_cpp","sha256":"25fee9fd1bb776d55ef1b803a60e414797ca9386e867e1f722410b015326e4f4","source":"sources/w9-render-math-b-mat-ident34/mat_ident34.cpp","emitted_functions":[{"name":"matIdent__FP8Matrix34","compiled_body_bytes":60}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_make_trans.o","profile":"authored_cpp","sha256":"94ec7b5cb0eaf6f94268ce09ff68fb6fea1862bace91332dd79c732880cad728","source":"sources/w9-render-math-b-mat-make-trans/mat_make_trans.cpp","emitted_functions":[{"name":"matMakeTrans__FP8Matrix34G6Point3","compiled_body_bytes":72}]}
{"header_closure":["sources/w8-object-list-math-common/trig_core.hpp","sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_make_trans_yaw_scale.o","profile":"authored_cpp","sha256":"9c6164f6bed56f6a62026678c9cc009a75e8114d7f65e0d43fedd786cf17852a","source":"sources/w9-render-math-b-mat-make-trans-yaw-scale/mat_make_trans_yaw_scale.cpp","emitted_functions":[{"name":"matMakeTransYawScale__FP8Matrix34G6Point3sf","compiled_body_bytes":384}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_make_view_matrix.o","profile":"authored_cpp","sha256":"f3ef67d70b9dc32ee4e88850fc316b675284f19b4658ba6fc1acd122bea6ff8f","source":"sources/w9-render-math-b-mat-make-view-matrix/mat_make_view_matrix.cpp","emitted_functions":[{"name":"matMakeViewMatrix__FP8Matrix44fffffff","compiled_body_bytes":112}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_mul34.o","profile":"authored_cpp","sha256":"922bc0ddf52c96f248b3a2e299fae163bf1823127801e144ebe0e88c313b7c3b","source":"sources/w9-render-math-b-mat-mul34/mat_mul34.cpp","emitted_functions":[{"name":"matMul__FP8Matrix34N20","compiled_body_bytes":252}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_mul44.o","profile":"authored_cpp","sha256":"02f31dd1ba0b19b6bd36b9f506de08f3d4c4f752b8c2d1f8ffcb853e9e3bcefe","source":"sources/w9-render-math-b-mat-mul44/mat_mul44.cpp","emitted_functions":[{"name":"matMul__FP8Matrix44P8Matrix34T0","compiled_body_bytes":252}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_rot34.o","profile":"authored_cpp","sha256":"2574bd71028e8ead0882ac1d2ae89084113091a3dde1d3f06dac37126934ce2c","source":"sources/w9-render-math-b-mat-rot34/mat_rot34.cpp","emitted_functions":[{"name":"matRot__FP8Matrix34is","compiled_body_bytes":324}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_trans.o","profile":"authored_cpp","sha256":"c541f54c0de98105834740ef8ef7302e2b48988e1a41db866f7b45c1b34f2728","source":"sources/w9-render-math-b-mat-trans/mat_trans.cpp","emitted_functions":[{"name":"matTrans__FP8Matrix34fff","compiled_body_bytes":136}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_transform44.o","profile":"authored_cpp","sha256":"6650b8e7661fe55e1bcb844b98521ef283a58e6260c985b6d4f3915966f3dc02","source":"sources/w9-render-math-b-mat-transform44/mat_transform44.cpp","emitted_functions":[{"name":"matTransform__FP6Point3P8Matrix44T0","compiled_body_bytes":188}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"mat_untransform33.o","profile":"authored_cpp","sha256":"0b90c1a61fbd9918f1678b8b751d3b5ec3e43862b368db38e7f9f64fd9a83c2d","source":"sources/w9-render-math-b-mat-untransform33/mat_untransform33.cpp","emitted_functions":[{"name":"matUntransform__FP6Point3P8Matrix33T0","compiled_body_bytes":128}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"megatile_seed_rot.o","profile":"authored_cpp","sha256":"f50b2396c1f227931e332fbe173e388c81320237f8c141436f7ff3cd8a446df6","source":"sources/w9-render-math-b-megatile-seed-rot/megatile_seed_rot.cpp","emitted_functions":[]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"model_draw_shadow.o","profile":"authored_cpp","sha256":"fe6e9a2e71f8468c2a5775f270a0341d8158947e5e474d93d0a3cb7cb00205b3","source":"sources/w9-render-math-b-model-draw-shadow/model_draw_shadow.cpp","emitted_functions":[{"name":"_ZN12_GLOBAL__N_1L6finishER12ShadowPacketRKNS_4OpenE","compiled_body_bytes":156},{"name":"modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec","compiled_body_bytes":3584}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"model_get_char_attachment_pos.o","profile":"authored_cpp","sha256":"5bb67b555548af5e01159dd67047bb5d01867476a8870af8d48191ff45220356","source":"sources/w9-render-math-b-model-get-char-attachment-pos/model_get_char_attachment_pos.cpp","emitted_functions":[{"name":"modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f","compiled_body_bytes":508}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"model_get_nm_shadow_points.o","profile":"authored_cpp","sha256":"fb81e79907f1f37ff53ff557c0cb05236b6ee6f7ee10dabc135a590ce163d245","source":"sources/w9-render-math-b-model-get-nm-shadow-points/model_get_nm_shadow_points.cpp","emitted_functions":[{"name":"modelGetNmShadowPoints__FP12_modelHeader","compiled_body_bytes":20}]}
{"header_closure":["sources/w9-render-math-b-common/math_types.hpp"],"kind":"independently_authored_source","object_name":"model_get_shadow_pos.o","profile":"authored_cpp","sha256":"6651b65b9a86d8286e2c7a0efadd408a4715aea7f28138adc508c84ec6a07a51","source":"sources/w9-render-math-b-model-get-shadow-pos/model_get_shadow_pos.cpp","emitted_functions":[{"name":"modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f","compiled_body_bytes":696}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"model_set_halo.o","profile":"authored_cpp","sha256":"7a841a26ccb959a6b876ebac23c624be8f04eea8496890c2809654558e1e5478","source":"sources/w9-render-math-b-model-set-halo/model_set_halo.cpp","emitted_functions":[{"name":"modelSetHalo__FP7HaloDefi","compiled_body_bytes":20}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"model_set_no_z_write.o","profile":"authored_cpp","sha256":"b556dd9591e65b1434a5303bc4442d806d8d4bb8a20f50ed78602570b75d016c","source":"sources/w9-render-math-b-model-set-no-z-write/model_set_no_z_write.cpp","emitted_functions":[{"name":"modelSetNoZWrite__Fv","compiled_body_bytes":20}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"model_set_shadow_scale.o","profile":"authored_cpp","sha256":"4ea40d63cff92effadb029659238eab022ba5c3518062ea1309f48b657023dc8","source":"sources/w9-render-math-b-model-set-shadow-scale/model_set_shadow_scale.cpp","emitted_functions":[{"name":"modelSetShadowScale__Ff","compiled_body_bytes":12}]}
```

## Authored but never executed

Everything in this package. The paired runs executed the original functions and the authored objects only inside a private interpreter on this Mac; nothing ran on the EE, under the original process or against gameplay. Specifically unexercised: the R5900's reduced-precision `rsqrt.s`, divide and square-root results, exception flags, denormal mode, cache and bus behavior, and every caller (`w8-core-loop-b-draw-dome`, `w8-actor-virtuals-b-msg-draw-autoobject`, `w8-core-loop-b-cut-run-frame`).

## Strings kept

`modelDrawShadow` compares `engineCurrentLevelName` with one engine level identifier and builds a static resource map from two resource file names. They are engine identifiers used by the code path, not dialogue or asset content; they are the only literal strings in this package. Remove them if the project prefers to hold them out.

## Unknown

- The element type and layout of `HaloDef`, the model header beyond the bytes read (+34, +36), the 52-byte and 8-byte shadow/attachment records beyond the fields read, and the particle definition objects.
- The roles of the seven floats of `matMakeViewMatrix` beyond the arithmetic, the units of `myPitch`/`myRoll`, the signedness and range of `mapScissorX/Y` and `megatileSeed/Rot`.
- Whether the register-only countdown loop in `makeExplosion` (omitted) is a timing wait or a leftover; it has no memory or call effects.
- Original section placement (.sdata or .data versus .bss) of the data units: they are defined here with their initial values and no placement claim.
- The meaning of the 'mode' character of `modelDrawShadow` beyond -1 / 0 / other, and why the cull test returns for `-3000 < dot` (the test is as observed, not interpreted).
- Which of the two original `ballParticle` / `trailParticle` locals a future shared definition should reuse; `makeExplosion` uses the ones at 0x7d6010 and 0x7d6060.
