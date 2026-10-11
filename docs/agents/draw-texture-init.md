# draw-texture-init

Package for draw setup, VIF setup, display-buffer words, and texture init. Evidence is the hash-pinned `SLUS_205.65` image. Host contracts live next to each source and are not partial-build inputs. Fresh EE compilation is not an original-byte match. Nothing here has run on the EE.

The fresh partial link of this branch, before these sources, has 692 unresolved imports. `origin/agents/render-lights` already defines `defaultFont`, `haloTexture1`, and `textureAllocateCacheBuffer__Fv`. Those stay on that branch.

## textureInitTexture__FP8_texture

Observed at `0x0016d160`, 276 bytes. A nonzero word at offset 28 returns with no stores. The other path stores `0xa7fb` at offset 10, `0xffffffff` at offset 28, and the texture address plus the old word at offset 16. Bit 0 of the halfword at offset 8 stops there. Bit `0x100` is the path that relocates a leading word and then each word of the following records. Records end at a `0xff` byte. The halfword at offset 6 is 72 plus one truncated quotient per record. The 64 bytes at `ntsc_legallineTex` take the header path only: their halfword at offset 8 is `0x0060`, so the quotient is left at `0x8c08`.

## Imports closed

`textureInitTexture__FP8_texture`.

## Imports opened

None.

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

The integrator assigns `input_order`. No shared plan file was edited.

## Authored but never executed

`textureInitTexture__FP8_texture` was compiled with the reviewed EE toolchain and checked on the host. It has not run on the EE. No startup stage has run.

## Unknown

The `_texture` bytes outside offsets 6, 8, 10, 16, and 28 are not established by this function. What `0xa7fb` selects is unknown. Whether a callee of another routine changes those words before this call is unknown.
