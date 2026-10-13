# w10-render-c: animation, model, light and render, part 3 of 4

Branch `agents/w10-render-c`. Private executable `work/private/SLUS_205.65` (the symbol table is the
image's own; GP is `0x004879f0`). Addresses, sizes and names are from that symbol table. Nothing here ran
on hardware or inside the game.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++ and
  compiled alone with the reviewed `authored_cpp` flags (EE GCC, `-G0 -O2 ... -march=r5900`). Every unit
  compiled.
- Every function has a `host_check.cpp` built with the host C++ compiler (flags noted in the file where
  needed: `megatile-bank-init` and `put-block` need `-DNORRATH_HOST_CONTRACT=1` for the address mapping).
  All exited 0. `put-block` also ran clean under ASan and UBSan.
- Private paired runs (outside git): a small R5900 interpreter loaded the original function and my freshly
  compiled one from the same memory snapshot with identical Python stand-ins for the callees, then compared
  the call log (argument registers, pointed-to contents, global state at the call), the return value where
  there is one, and every changed byte outside the stack. Scenario counts are in the commit messages.
  Several functions were also run with a deliberately mutated copy, which the comparison caught; for
  `putBlock` the original's instruction coverage was measured (179 of 180 words; the other is a nop). Two
  scenario-generator bugs of mine (a closure over a loop variable, and header fields overwritten by random
  record data) were found and fixed in the private generators, not in the sources.
- Defects noticed in the private interpreter copy: none new. `HALT` was grepped in every run.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).
- I did not run `tools/build_partial.py` (the plan is not mine to edit). Compilation was per unit.

## Functions authored (one commit each)

| Function | Address, original size | Notes |
| --- | --- | --- |
| `quatMul__FP4QuatN20` | | Hamilton product, result staged so the destination may alias |
| `matTransform__FP6Point3P8Matrix34T0` | | point times 3x4 matrix, translation row last |
| `rayHit__FP6Point3N30` | | float result; two pool literals are not the nearest floats, written as exact hex floats |
| `segmentLineHit__FP6Point3N50` | 0x00157430, 564 | segment against plane with clamped denominator, then a span check |
| `resetScissor__Fv` | 0x0018bf30, 328 | GIF packet setting two registers to a 0x4ff by 0x1bf area, queued on list 4 |
| `modelSetGlobalShadowBrightness__Ff` | 0x0016ccf8, 24 | argument times 0.25f stored to `shadowBrightness` and `globalShadowBrightness` |
| `modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f` | 0x0016ca18, 228 | 52-byte table entry matrix, bone matrix or identity, uniform scale |
| `modelGetBoundingBox__FP12_modelHeaderlPiN52` | 0x0016c820, 336 | mask-selected signed-byte boxes merged to six ints |
| `modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34` | 0x00169110, 452 | only callee effects are observable (see below) |
| `megatileBankInit__FP16MegatileInstBankPv` | 0x002660a8, 84 | relocation by a base value |
| `megatileItemInit__FP8MegatilePv` | 0x00266100, 40 | relocation of three words |
| `modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc` | 0x00168c08, 728 | character setup DMA packet |
| `modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi` | 0x00168410, 1204 | halo draw packets |
| `putBlock__FP8_textureP6PacketPPP9uncompMapPPii` | 0x0016f5e8, 720 | texture block cache claim and scratchpad DMA |

Data unit `w10-render-c-data-render-globals`: `modelDrawFade`, `modelShadowScale`, `modelOption`,
`shadowBrightness`, the 48-byte `scissor` float table, `scx1`, `scy1`, `scx2`, `scy2`, `rainSplashParticle`
(the one at 0x006ebaf0) and the four `slideDust*Particle` objects. Initial values are file bytes; sizes of the
uninitialised objects are the distance to the next symbol (a bound, not proof).

## Imports closed

`matTransform__FP6Point3P8Matrix34T0`, `megatileBankInit__FP16MegatileInstBankPv`,
`megatileItemInit__FP8MegatilePv`, `modelDrawFade`, `modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi`,
`modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f`,
`modelGetBoundingBox__FP12_modelHeaderlPiN52`, `modelOption`,
`modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34`,
`modelSetGlobalShadowBrightness__Ff`, `modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc`,
`modelShadowScale`, `putBlock__FP8_textureP6PacketPPP9uncompMapPPii`, `quatMul__FP4QuatN20`,
`rainSplashParticle`, `rayHit__FP6Point3N30`, `resetScissor__Fv`, `scissor`, `scx1`, `scx2`, `scy1`, `scy2`,
`segmentLineHit__FP6Point3N50`, `shadowBrightness`, `slideDustParticle`, `slideDustParticleBig`,
`slideDustParticleBigDark`.

## Not closed

- `megatileGenerate__FP12_worldHeader` (0x0025e3a8, 19680 bytes). Not attempted: it is a 4900-instruction
  function with try/catch machinery (`longjmp`, stack checks), software double-precision calls (`dpmul`,
  `dpadd`, `litodp`) and calls into the megatile package's own helpers; it belongs with the megatile owner
  and is not authored on `origin/agents/megatile` either.
- `norrath_container_msg_run_fuseParticle_bytes` and `norrath_container_msg_run_smokeParticle_bytes`: not
  defined by me. The existing source `w9-render-math-c-switch-static-init/switch_static_init.cpp` already
  defines both (they stand for the 80-byte objects at 0x007d7720 and 0x007d7770), so they close once that
  unit is in the plan; I removed my duplicate definitions in a follow-up commit.

## Imports opened

Symbols my new units reference but do not define:
`animGetBoneMatrix__FP14AnimationStateiP8Matrix34`, `matIdent__FP8Matrix34`, `matMul__FP8Matrix34N20`,
`matScale__FP8Matrix34f`, `matTransform34Vec__FP6Point3P8Matrix34T0`, `worldCameraMatrix`,
`lightSetupForCharacter__FP6PacketG6Point3`, `modelClippingOn` (a byte at 0x0048407c),
`globalShadowBrightness` (defined by the w10-render-b data unit), `drawAddToList__FPviP8_textureP11_drawRecordi`,
`glistp`, `coreLoadingScreenRunning`, `memcpy`, `uncompSpin`, `uncompCache`, `drawFrameNm`, `uncompMap`.
All are image symbols owned by other packages (most are already defined by authored sources).

## Build-plan entries needed

One entry per unit, as JSON lines in the shape the private plan builder used (`source`, `object_name`, `sha256`,
`emitted_functions` with `compiled_body_bytes` from the reviewed toolchain). The lead integrator adds the
`input_order`, `profile` and header closure. The hashes are for the final source bytes at the head of this
branch.

```
{"source":"sources/w10-render-c-data-render-globals/data_render_globals.cpp","object_name":"data_render_globals.o","sha256":"293cf37bbd69f2957e4b4edd226d68cb29438bfe0d97e4da72b5f919882dedf8","emitted_functions":[]}
{"source":"sources/w10-render-c-mat-transform34/mat_transform34.cpp","object_name":"mat_transform34.o","sha256":"b88541e6b5aa613d4d0bf2c7c218f395fca8b641533650e0dad59eeede9abb2c","emitted_functions":[{"name":"matTransform__FP6Point3P8Matrix34T0","compiled_body_bytes":148}]}
{"source":"sources/w10-render-c-megatile-bank-init/megatile_bank_init.cpp","object_name":"megatile_bank_init.o","sha256":"90c0e6043a6d7f06db522c7b43eef7f434b2eee5f805fe7ad9d013e7cf5b90d2","emitted_functions":[{"name":"megatileBankInit__FP16MegatileInstBankPv","compiled_body_bytes":76}]}
{"source":"sources/w10-render-c-megatile-item-init/megatile_item_init.cpp","object_name":"megatile_item_init.o","sha256":"c0d7fb6ccc22c74e8ef765cde06f07959575520594e60ef48eccee31492a2e00","emitted_functions":[{"name":"megatileItemInit__FP8MegatilePv","compiled_body_bytes":40}]}
{"source":"sources/w10-render-c-model-draw-halo/model_draw_halo.cpp","object_name":"model_draw_halo.o","sha256":"db14b8185856dc980d4a57d67d4a2bec6f45c4910e0d1b247ecffcaf1e17c134","emitted_functions":[{"name":"modelDrawHalo__FP6PacketP12_modelHeadercP7HaloDefi","compiled_body_bytes":884}]}
{"source":"sources/w10-render-c-model-get-attachment-matrix/model_get_attachment_matrix.cpp","object_name":"model_get_attachment_matrix.o","sha256":"dbb6fda1b25819a2036f9a9fc2767d7281f65e387708be5935d8ca072f544ed6","emitted_functions":[{"name":"modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f","compiled_body_bytes":180}]}
{"source":"sources/w10-render-c-model-get-bounding-box/model_get_bounding_box.cpp","object_name":"model_get_bounding_box.o","sha256":"fc34814be41a71f56de524a2c9f2d4b0105d11d7ced74b95901754c699263979","emitted_functions":[{"name":"modelGetBoundingBox__FP12_modelHeaderlPiN52","compiled_body_bytes":392}]}
{"source":"sources/w10-render-c-model-process-glows/model_process_glows.cpp","object_name":"model_process_glows.o","sha256":"6c001f0ef84f3dda9aef2e70f6ae9ebef0c0e1d1198712e9358278085c96c974","emitted_functions":[{"name":"modelProcessGlows__FP12_modelHeaderP14AnimationStateP8Matrix34","compiled_body_bytes":472}]}
{"source":"sources/w10-render-c-model-set-global-shadow-brightness/model_set_global_shadow_brightness.cpp","object_name":"model_set_global_shadow_brightness.o","sha256":"9840e6dcbec217dfd3a739645c96af50bc7c5ba60d0831c8cb41225b2cc9cd2c","emitted_functions":[{"name":"modelSetGlobalShadowBrightness__Ff","compiled_body_bytes":32}]}
{"source":"sources/w10-render-c-model-setup-character/model_setup_character.cpp","object_name":"model_setup_character.o","sha256":"fa9588918a5453fc30481cfc306b8232978ddb2823079e200ba5713bc29f0acd","emitted_functions":[{"name":"modelSetupCharacter__FP6PacketP8Matrix44P6Point3P12_modelHeaderc","compiled_body_bytes":504}]}
{"source":"sources/w10-render-c-put-block/put_block.cpp","object_name":"put_block.o","sha256":"9bf6042573c8eb93c3d41103d0864d4844f7d3999b7f537329416f61bec48c91","emitted_functions":[{"name":"putBlock__FP8_textureP6PacketPPP9uncompMapPPii","compiled_body_bytes":764}]}
{"source":"sources/w10-render-c-quat-mul/quat_mul.cpp","object_name":"quat_mul.o","sha256":"157d9fd3e5daf1484f835433bba04d89b862c2432bbae22fbc26af0895f05a9f","emitted_functions":[{"name":"quatMul__FP4QuatN20","compiled_body_bytes":164}]}
{"source":"sources/w10-render-c-ray-hit/ray_hit.cpp","object_name":"ray_hit.o","sha256":"acde722795440c84466f5c15e7582a0fceb111b8440e08ef0a66edd1e66616e8","emitted_functions":[{"name":"rayHit__FP6Point3N30","compiled_body_bytes":292}]}
{"source":"sources/w10-render-c-reset-scissor/reset_scissor.cpp","object_name":"reset_scissor.o","sha256":"2d34356399455a3faa4ddb6ca09d701d5abb7d8d7cc6259cff7c3a9a4744712d","emitted_functions":[{"name":"resetScissor__Fv","compiled_body_bytes":164}]}
{"source":"sources/w10-render-c-segment-line-hit/segment_line_hit.cpp","object_name":"segment_line_hit.o","sha256":"1277094f1cc6038a9f981e6507e883e383c03362e57b0c18052270e54fab0b08","emitted_functions":[{"name":"segmentLineHit__FP6Point3N50","compiled_body_bytes":464}]}
```

## Authored but never executed

All of it. Host contracts ran on this Mac and the paired comparisons ran in a private interpreter, not on the
EE and not in the game. In the paired comparisons the callees (`animGetBoneMatrix`, `matIdent`, `matMul`,
`matScale`, `matTransform34Vec`, `matTransform`, `lightSetupForCharacter`, `drawAddToList`, `memcpy`) were
Python stand-ins; their real behaviour was not exercised. The DMA register accesses in `putBlock` were plain
interpreter memory, so the hardware wait loop and register read-backs were never run against a device.
`modelDrawHalo` and `putBlock` pad cursors with a byte loop that assumes 8-byte-aligned input; unaligned
inputs were not tested.

## Observed, inferred, unknown

- Observed: the instruction-level behaviour of each function, every constant, offset and register number above.
- Inferred: register numbers 0x40/0x41 as the two scissor registers, `modelProcessGlows` as a facing test plus
  glow position, the 0x1000d0xx/0x1000e010 addresses as scratchpad-to-memory DMA, `uncompMap` entries as
  {owner, next}.
- Unknown: what the fields of the glow records, halo definitions and model header words at +16, +24, +32 mean;
  the meaning of the 0.25 scale in `modelSetGlobalShadowBrightness`; the unit of the bounding box bytes; the
  GS register names for 26, 27, 63, 66, 67, 71 beyond the numbers.
- `modelProcessGlows` computes a position `q` that the original never stores or passes anywhere after the
  transforms. The authored version keeps the same callee calls and nothing more.
- Note for the owner of `w8-render-world-b-model-draw`: its `ModelDrawPacket` has two words, but the original
  `modelSetupCharacter` and `modelDrawHalo` write a packet record of at least four words (cursor, DMA tag
  address, GIF tag address, VIF word address). Its authored unit would overrun a two-word local if it were
  linked with these.
