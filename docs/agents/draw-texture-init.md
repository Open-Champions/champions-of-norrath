# draw-texture-init

Package for draw setup, VIF setup, display-buffer words, and texture init. Evidence is the hash-pinned `SLUS_205.65` image. Host contracts live next to each source and are not partial-build inputs. Fresh EE compilation is not an original-byte match. Nothing here has run on the EE.

The fresh partial link of this branch, before these sources, has 692 unresolved imports. `origin/agents/render-lights` already defines `defaultFont`, `haloTexture1`, and `textureAllocateCacheBuffer__Fv`. Those stay on that branch.

## textureInitTexture__FP8_texture

Observed at `0x0016d160`, 276 bytes. A nonzero word at offset 28 returns with no stores. The other path stores `0xa7fb` at offset 10, `0xffffffff` at offset 28, and the texture address plus the old word at offset 16. Bit 0 of the halfword at offset 8 stops there. Bit `0x100` is the path that relocates a leading word and then each word of the following records. Records end at a `0xff` byte. The halfword at offset 6 is 72 plus one truncated quotient per record. The 64 bytes at `ntsc_legallineTex` take the header path only: their halfword at offset 8 is `0x0060`, so the quotient is left at `0x8c08`.

## textureInit__Fii

Observed at `0x001725e0`, 64 bytes. It stores the arithmetic right-shift by 8 of `address + 255` at `0x0048695c` and of `size - (address & 0xff)` at `0x00486960`, calls `textureInternalInit__Fv`, then stores that reloaded size minus one at `textureMaxBlockSize` (`0x004840b0`, four zero bytes). A second `textureBufferStartAddress` label is at `0x00486a34`. This unit publishes one external word under that name, the word this function stores.

## initVIF__Fv

Observed at `0x0018f118`, 224 bytes. COP2 register 28 receives 1024. `sceDmaSend` / `sceDmaSync` run on `dmaVif1` with `0x0031b790` (`loadVU1Program`) and on `dmaVif0` with `0x0031b510` (`loadVU0Program`). Both syncs use mode 0 and timeout 0. `FlushCache(0)` and `sceGsSyncPath(0, 0)` follow. The words at `0x10003820` and `0x10003c20` are set to 2, then `sceGsSyncV(0)` runs twice. `0x1100ffb4`, `0x1100ffb8`, and `0x1100ff60` receive `0x30ae4000`, `0x30be4000`, and `0x8000`; the next three words are zero. The channel pointers are the words at `0x00486a40` and `0x00486a44`. Separate local words with the same names are at `0x004868d8` and `0x004868dc`; `drawInit` uses those.

## drawInit__Fv

Observed at `0x0015f6d0`, 596 bytes. `endTagVU1` is `0x00614240` and is 128 bytes before `texDmaList`. With a 16-byte-aligned base the 64-bit tag is `0x10000002` and the following words are `0x11000000`, `0x50000002`, `0x8001`, `0x10000000`, 14, 0, 0, 2, 97, and 0. `drawRecords` is the 65536 bytes at `0x00604200`. Eight pointers at `drawHead+32` (`drawHead` is `0x00614200`, 64 bytes) address `(2048+index)<<4` into that buffer, and each record word at offset 12 is cleared. `writeBuffer` becomes 1. `nmDrawUsed` becomes 8. The semaphore word at offset 4 is `0x186a0`. `reloadDMA__Fi` is the 2488-byte function at `0x0015ed18`. `gsIntHandler__Fi` is the 40-byte function at `0x0015fac8`. 32000 is stored at `0x12001010` and 4 at `0x10003010`. The DMA channel pointers written here are the local words at `0x004868d4`, `0x004868d8`, and `0x004868dc`.

## initSupersample__Fv

Observed at `0x0018f7d0`, 752 bytes. The packet base is `packetBuffer` at `0x0037f9e0` (560000 bytes, ending at `spellAlpha`). The word at `dispEnv+16` is masked with `0xfffffe00`. `engineEnvTexAddress >> 8` is merged into the qword at packet offset `0x90`. `engineDrawBufferStartAddress >> 13` is merged into offset `0x20`. That shift plus 140 is merged into offset `0x30`, `zwritesOn` (`0x004843f8`, 8 bytes), and `zwritesOff` (`0x00484400`, 8 bytes, with bit 32 also set). The tag `0x1000000b` is stored and then replaced by `0x6000000b`. The arm that stores `0x60000000` at offset 192 is in the image; this tag's bits 30:28 equal 1, so that arm is not entered. `FlushCache(0)`, `sceDmaSend` of `dmaGif` at `0x00486a3c`, `sceGsSyncPath(0, 0)`, and `makeSamplePacket__Fb(fullFrameMode)` follow.

## Imports closed

`textureInitTexture__FP8_texture`, `textureInit__Fii`, `initVIF__Fv`, `drawInit__Fv`, `initSupersample__Fv`, `textureBufferStartAddress`, `textureBufferSize`, `textureMaxBlockSize`, `dmaVif0`, `dmaVif1`, `drawRecords`, `drawHead`, `endTagVU1`, `writeBuffer`, `nmDrawUsed`, `endDrawSemaphore`, `vif0HID`, `vif1HID`, `gifHID`, `dispEnv`, `displayBuffer1StartAddress`, `displayBuffer2StartAddress`, `engineDrawBufferStartAddress`, `engineEnvTexAddress`, `engineShadowTexAddress`, `ntsc_legallineTex`, `ntsc_legallinebottomTex`, `dmaGif`, `zwritesOn`, `zwritesOff`, `fullFrameMode`, `packetBuffer`.

## Imports opened

`textureInternalInit__Fv`, `sceDmaSend`, `sceDmaSync`, `FlushCache`, `sceGsSyncPath`, `sceGsSyncV`, `sceDmaGetChan`, `memset`, `CreateSema`, `AddDmacHandler`, `EnableDmac`, `AddIntcHandler`, `EnableIntc`, `reloadDMA__Fi`, `gsIntHandler__Fi`, `makeSamplePacket__Fb`. `FlushCache` and `memset` are already selected. The DMA and interrupt services stay imports.

## Build-plan entries needed

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "texture_init_texture.o",
  "profile": "authored_cpp",
  "sha256": "839e6689eb37e3ceeac2c90afc74dc5503b1a23f241927bffddff71b07276ad1",
  "source": "sources/draw-texture-init-texture-init-texture/texture_init_texture.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "texture_init.o",
  "profile": "authored_cpp",
  "sha256": "8b75a353afcac335116466bbe46f22fe56e94614ec15403b42d32f585dc18d21",
  "source": "sources/draw-texture-init-texture-init/texture_init.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "init_vif.o",
  "profile": "authored_cpp",
  "sha256": "81c34f0f897d8159901b30ea14fabc47209eabd4e31c594e374de354fadf91e4",
  "source": "sources/draw-texture-init-init-vif/init_vif.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "draw_init.o",
  "profile": "authored_cpp",
  "sha256": "5e70bfb04ec241286b6115078c6a9122c5756577c54e7df8f8efe4d668b1b6bc",
  "source": "sources/draw-texture-init-draw-init/draw_init.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "init_supersample.o",
  "profile": "authored_cpp",
  "sha256": "16e2ff8115e8d9fa5130c625c0858f843dc3afb530cfc9ae80aa5937c4f9bccc",
  "source": "sources/draw-texture-init-init-supersample/init_supersample.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "disp_env.o",
  "profile": "authored_cpp",
  "sha256": "1329265ea601c083060616a680f0f240f813fec6efd7eb63c6152843d74bd592",
  "source": "sources/draw-texture-init-disp-env/disp_env.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "display_buffer_1.o",
  "profile": "authored_cpp",
  "sha256": "2ce028d8aa5443c73325ae2b8a8549170f08f55428faed1aa2d34dbb950e161f",
  "source": "sources/draw-texture-init-display-buffer-1/display_buffer_1.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "display_buffer_2.o",
  "profile": "authored_cpp",
  "sha256": "cfb934f9229b5d88131ae023a39126edcb8adf73be02c6ec91dd49e160a03f2f",
  "source": "sources/draw-texture-init-display-buffer-2/display_buffer_2.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "engine_draw_buffer.o",
  "profile": "authored_cpp",
  "sha256": "f3c424518f56851f7f300a178d511369597d7231e10838212633cbbf2883fae6",
  "source": "sources/draw-texture-init-engine-draw-buffer/engine_draw_buffer.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "engine_env_tex.o",
  "profile": "authored_cpp",
  "sha256": "d2cb49771750412c146ed65a3dd60f7a83b289879437c68d2af9b621200f1872",
  "source": "sources/draw-texture-init-engine-env-tex/engine_env_tex.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "engine_shadow_tex.o",
  "profile": "authored_cpp",
  "sha256": "4980f30ce3c439b24dd6830f8b1db239e7f3e51a3f61ba97dfae88060aa11eb7",
  "source": "sources/draw-texture-init-engine-shadow-tex/engine_shadow_tex.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "ntsc_legal_line.o",
  "profile": "authored_cpp",
  "sha256": "ebbad15ff4297513e756951c98b90f7d7da963220887dfeaa3754a9b82e14446",
  "source": "sources/draw-texture-init-ntsc-legal-line/ntsc_legal_line.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "ntsc_legal_line_bottom.o",
  "profile": "authored_cpp",
  "sha256": "f2d496936f01e904714569111e57c60d6c82167c16dfc49c53a3d446a1cda682",
  "source": "sources/draw-texture-init-ntsc-legal-line-bottom/ntsc_legal_line_bottom.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "dma_gif.o",
  "profile": "authored_cpp",
  "sha256": "eb652ad3116b28561c736b51fb6c1cf4594dbec84b813194f64a80ea86399f16",
  "source": "sources/draw-texture-init-dma-gif/dma_gif.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "zwrites_on.o",
  "profile": "authored_cpp",
  "sha256": "9b69dfdfd244e5b127d9b2fbf620e60dea931da3dbdc136691b055ba14e7d5e3",
  "source": "sources/draw-texture-init-zwrites-on/zwrites_on.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "zwrites_off.o",
  "profile": "authored_cpp",
  "sha256": "4aa4020f74376ed33bbe743d2b01027da71bbe82f0cea748a80d7f1c219db092",
  "source": "sources/draw-texture-init-zwrites-off/zwrites_off.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "full_frame_mode.o",
  "profile": "authored_cpp",
  "sha256": "53cd47a66617f353397c6afb365310608fb0a90c1e65eb16a4178886d9d98790",
  "source": "sources/draw-texture-init-full-frame-mode/full_frame_mode.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "packet_buffer.o",
  "profile": "authored_cpp",
  "sha256": "157041b461186207e9be56c7edfa295e715fa3c127fd7de7e743719c73954476",
  "source": "sources/draw-texture-init-packet-buffer/packet_buffer.cpp"
}
```

The integrator assigns `input_order`. No shared plan file was edited.

## Authored but never executed

`textureInitTexture__FP8_texture`, `textureInit__Fii`, `initVIF__Fv`, `drawInit__Fv`, `initSupersample__Fv`, and the display, texture-address, NTSC header, DMA, z-write, frame-mode, and packet-buffer objects were compiled with the reviewed EE toolchain and checked on the host. None has run on the EE. No startup stage has run.

## Unknown

The `_texture` bytes outside offsets 6, 8, 10, 16, and 28 are not established by this function. What `0xa7fb` selects is unknown. Whether a callee of another routine changes those words before this call is unknown. The GS register numbers stored by `initSupersample__Fv` are the observed constants; what each one selects beyond the traced bit packing is unknown. The packet arm that writes offset 192 is in the image and is not entered for the tag this function builds. The local `dmaGif` at `0x004868d4` stays private to `drawInit__Fv`. `simpleDrawLoop__FPFv_i` is an unresolved draw-named import and is not part of this package's assigned symbols.
