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

`textureInitTexture__FP8_texture`, `textureInit__Fii`, `initVIF__Fv`, `drawInit__Fv`, `initSupersample__Fv`, `textureBufferStartAddress`, `textureBufferSize`, `textureMaxBlockSize`, `dmaVif0`, `dmaVif1`, `drawRecords`, `drawHead`, `endTagVU1`, `writeBuffer`, `nmDrawUsed`, `endDrawSemaphore`, `vif0HID`, `vif1HID`, `gifHID`.

## Imports opened

`textureInternalInit__Fv`, `sceDmaSend`, `sceDmaSync`, `FlushCache`, `sceGsSyncPath`, `sceGsSyncV`, `sceDmaGetChan`, `memset`, `CreateSema`, `AddDmacHandler`, `EnableDmac`, `AddIntcHandler`, `EnableIntc`, `reloadDMA__Fi`, `gsIntHandler__Fi`, `packetBuffer`, `dispEnv`, `engineEnvTexAddress`, `engineDrawBufferStartAddress`, `zwritesOn`, `zwritesOff`, `dmaGif`, `fullFrameMode`, `makeSamplePacket__Fb`. `FlushCache` and `memset` are already selected. The DMA and interrupt services stay imports.

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

The integrator assigns `input_order`. No shared plan file was edited.

## Authored but never executed

`textureInitTexture__FP8_texture`, `textureInit__Fii`, `initVIF__Fv`, `drawInit__Fv`, and `initSupersample__Fv` were compiled with the reviewed EE toolchain and checked on the host. None has run on the EE. No startup stage has run.

## Unknown

The `_texture` bytes outside offsets 6, 8, 10, 16, and 28 are not established by this function. What `0xa7fb` selects is unknown. Whether a callee of another routine changes those words before this call is unknown.
