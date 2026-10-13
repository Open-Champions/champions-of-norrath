# w13-vi-c: VI engine, part 3 of 4

Package: rasterizer, scene, world, sprites, sound, files and containers. Branch
`agents/w13-vi-c`. Every function below is one commit, authored independently from
observed behaviour of the pinned private executable (`SLUS_205.65`, read only locally;
nothing derived from it is committed). Each source states Observed, Inferred and Unknown
in its header comment. No member names are claimed; only mangled symbol names are used.

## How each function was checked

1. Compiled with the reviewed EE g++ `authored_cpp` flags (a per-unit compile, not a full
   partial build).
2. A host contract (`host_contract.cpp` beside each source, not a plan input) built with
   the host compiler and run: it checks the semantics stated in the header.
3. A private paired run: the authored object is linked next to the original image, run with the
   same memory snapshot and arguments in a private R5900 interpreter, with undefined callees
   either bound to the original code or replaced by identical logging stubs. Return
   value, float result register where relevant, call logs and every changed byte outside
   the stack were compared. Mutated variants of each source were also run and fail
   (equivalent mutants and the exceptions are listed under Unknown).

## Imports closed (38 sources, 38 symbols)

`AttenuationMode__C13VISoundSprite`, `Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry`, `Begin__Ct6VIPool1ZP13VISpellEffect`, `Begin__Ct6VIPool1ZP20VIParticleDefinition`, `CalcPlanes__9VIFrustum`, `CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44`, `ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT`, `Clear__9VITrailFx`, `Close__9VIObjFile`, `DistanceSquared__C6VIBBoxRC7VIVect3`, `Distance__C7VIVect3RC7VIVect3`, `EnablePointLight__8VIRasterii`, `End__Ct10VIMultiMap2ZfZ14VIEmitterEntry`, `End__Ct6VIPool1ZP13VISpellEffect`, `End__Ct6VIPool1ZP20VIParticleDefinition`, `FileType__9VIObjFile`, `FindLeaf__C6VIZoneRC7VIVect3`, `GetActorLocation__C7VIScenei`, `GetActorSpritePtr__C7VIScenei`, `GetHitPoint__9VICollRay`, `GetHitPoint__C8VIEnvRay`, `GetLayerColor__C10VIMateriali`, `GetMaterialPalEntry__8VIRasterii`, `GetMaterialPal__C14VISimpleSprite`, `GetPriority__C19VIStreamAudioSprite`, `GetRelativePathname__C19VIStreamAudioSprite`, `Init__9VICollRayRC7VIVect3T1`, `IsLooping__C19VIStreamAudioSprite`, `IsPanEnabled__C13VISoundSprite`, `Length__C7VILList`, `Mul__C10VIMatrix44RC7VIVect4R7VIVect3`, `Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi`, `NumObjects__9VIObjFile`, `PostOpen__9VIObjFile`, `SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight`, `Transform__C6VIBBoxRC10VIMatrix44R6VIBBox`, `_$_10VIMatrix33`, `_$_12VICollBuffer`

## Imports opened

Calls these sources make that are not defined by this package (left unresolved; some may
already be closed by other packages):

- `WriteEnd__9VIObjFile`, `Flush__9VIObjFile`, `Close__6VIFile` (from `Close__9VIObjFile`)
- `Seek__6VIFilelQ26VIFile10SeekOrigin`, `Tell__6VIFile`, `WriteFileHeader__9VIObjFilei`,
  `ReadFileHeader__9VIObjFilei`, `AbortOpen__9VIObjFile` (from `PostOpen__9VIObjFile`)
- `__builtin_delete` (the two destructors), `Clear__12VICollBuffer`
- `__builtin_vec_delete`, `Init__7VILList`, `Remove__7VILNode`, `AddTail__7VILListP7VILNode` (VITrailFx)
- `Precalc__9VICollRay`, `__cl__7VIPlaneRC7VIVect3N21`
- `Mul__C10VIMatrix44RC7VIVect3R7VIVect3` (SetPointLight, BBox Transform; authored in w12-vimath-a)
- `GetActorTransform__7VISceneiR10VIMatrix44`,
  `CalcNodeTransform__9VIHSpriteiR10VIMatrix44R7VISceneT2`, `_$_10VIMatrix44`
- `End__Ct6VIPool1ZP13VISpellEffect` and `End__Ct6VIPool1ZP20VIParticleDefinition` are
  called by the matching Begin sources and are closed here.

## Build-plan entries needed

No shared file was edited. The plan needs one `independently_authored_source` input per row
below (profile `authored_cpp`, empty header closure, source path relative to `experimental/`).
The integrator computes `input_order`, `object_name` and the full sha256; the prefix shown
is for orientation. Body sizes are from the per-unit compile and are not an original-size claim.

| source | sha256 prefix | emitted function and compiled body bytes |
|---|---|---|
| `sources/w13-vi-c-bbox-distance-squared/bbox-distance-squared.cpp` | `d6fea333c0b8988d…` | `DistanceSquared__C6VIBBoxRC7VIVect3` 212 |
| `sources/w13-vi-c-bbox-transform/bbox-transform.cpp` | `df816b87f145fe32…` | `Transform__C6VIBBoxRC10VIMatrix44R6VIBBox` 348 |
| `sources/w13-vi-c-collbuffer-dtor/dtor.cpp` | `43eb2482e1065cf0…` | `_$_12VICollBuffer` 76 |
| `sources/w13-vi-c-collray-get-hit-point/collray-get-hit-point.cpp` | `163c9c62a92823bd…` | `GetHitPoint__9VICollRay` 72 |
| `sources/w13-vi-c-collray-init/collray-init.cpp` | `00e61cefd58cfb9a…` | `Init__9VICollRayRC7VIVect3T1` 140 |
| `sources/w13-vi-c-csprite-calc-slot-world-transform/csprite-calc-slot-world-transform.cpp` | `fb1670e114361321…` | `CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44` 224 |
| `sources/w13-vi-c-envray-get-hit-point/envray-get-hit-point.cpp` | `d7966b1830ccb355…` | `GetHitPoint__C8VIEnvRay` 84 |
| `sources/w13-vi-c-frustum-calc-planes/frustum-calc-planes.cpp` | `e875f334c1bc8838…` | `CalcPlanes__9VIFrustum` 272 |
| `sources/w13-vi-c-llist-length/llist-length.cpp` | `14814f631771fca3…` | `Length__C7VILList` 48 |
| `sources/w13-vi-c-material-get-layer-color/material-get-layer-color.cpp` | `98205108f2c4224a…` | `GetLayerColor__C10VIMateriali` 56 |
| `sources/w13-vi-c-matrix33-dtor/dtor.cpp` | `95224b77d1fcdef1…` | `_$_10VIMatrix33` 32 |
| `sources/w13-vi-c-matrix44-mul-vect4-vect3/mul_vect4_vect3.cpp` | `fbe6152b1d5aa500…` | `Mul__C10VIMatrix44RC7VIVect4R7VIVect3` 96 |
| `sources/w13-vi-c-multimap-begin-emitterentry/multimap-begin-emitterentry.cpp` | `3d8ad7b00283818e…` | `Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry` 64 |
| `sources/w13-vi-c-multimap-end-emitterentry/multimap-end-emitterentry.cpp` | `ed12d3c01458fde9…` | `End__Ct10VIMultiMap2ZfZ14VIEmitterEntry` 8 |
| `sources/w13-vi-c-multimap-next-emitterentry/multimap-next-emitterentry.cpp` | `88db95d9608e7149…` | `Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi` 160 |
| `sources/w13-vi-c-objfile-close/objfile-close.cpp` | `7aae1c6bc5690852…` | `Close__9VIObjFile` 172 |
| `sources/w13-vi-c-objfile-file-type/objfile-file-type.cpp` | `a3d32de88d8f1dfb…` | `FileType__9VIObjFile` 8 |
| `sources/w13-vi-c-objfile-num-objects/objfile-num-objects.cpp` | `439f47d6a3b7a513…` | `NumObjects__9VIObjFile` 8 |
| `sources/w13-vi-c-objfile-post-open/objfile-post-open.cpp` | `f5ec3378e9b787f8…` | `PostOpen__9VIObjFile` 320 |
| `sources/w13-vi-c-pool-begin-particledefinition/pool-begin-particledefinition.cpp` | `a1fbe16b9ee51df3…` | `Begin__Ct6VIPool1ZP20VIParticleDefinition` 32 |
| `sources/w13-vi-c-pool-begin-spelleffect/pool-begin-spelleffect.cpp` | `09b673bd74039fa8…` | `Begin__Ct6VIPool1ZP13VISpellEffect` 32 |
| `sources/w13-vi-c-pool-end-particledefinition/pool-end-particledefinition.cpp` | `977793760d434ac4…` | `End__Ct6VIPool1ZP20VIParticleDefinition` 12 |
| `sources/w13-vi-c-pool-end-spelleffect/pool-end-spelleffect.cpp` | `96d5f156d804ebbe…` | `End__Ct6VIPool1ZP13VISpellEffect` 12 |
| `sources/w13-vi-c-raster-enable-point-light/raster-enable-point-light.cpp` | `6525ffcefff34bf4…` | `EnablePointLight__8VIRasterii` 32 |
| `sources/w13-vi-c-raster-get-material-pal-entry/raster-get-material-pal-entry.cpp` | `631aabea2c8abb02…` | `GetMaterialPalEntry__8VIRasterii` 44 |
| `sources/w13-vi-c-raster-set-point-light/raster-set-point-light.cpp` | `a7bb5c957f369642…` | `SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight` 156 |
| `sources/w13-vi-c-scene-get-actor-location/scene-get-actor-location.cpp` | `7c33a18769c97c10…` | `GetActorLocation__C7VIScenei` 8 |
| `sources/w13-vi-c-scene-get-actor-sprite-ptr/scene-get-actor-sprite-ptr.cpp` | `0f80065870af8005…` | `GetActorSpritePtr__C7VIScenei` 32 |
| `sources/w13-vi-c-simplesprite-get-material-pal/simplesprite-get-material-pal.cpp` | `04dc10a99cb8a9b8…` | `GetMaterialPal__C14VISimpleSprite` 8 |
| `sources/w13-vi-c-soundsprite-attenuation-mode/soundsprite-attenuation-mode.cpp` | `bb998db55be61202…` | `AttenuationMode__C13VISoundSprite` 8 |
| `sources/w13-vi-c-soundsprite-is-pan-enabled/soundsprite-is-pan-enabled.cpp` | `8a880ec2e34d0bc0…` | `IsPanEnabled__C13VISoundSprite` 8 |
| `sources/w13-vi-c-streamaudio-get-priority/streamaudio-get-priority.cpp` | `0c24a5b5ad47dc7f…` | `GetPriority__C19VIStreamAudioSprite` 8 |
| `sources/w13-vi-c-streamaudio-get-relative-pathname/streamaudio-get-relative-pathname.cpp` | `3b52fb8f06fee0a4…` | `GetRelativePathname__C19VIStreamAudioSprite` 8 |
| `sources/w13-vi-c-streamaudio-is-looping/streamaudio-is-looping.cpp` | `ae069567dccc40d4…` | `IsLooping__C19VIStreamAudioSprite` 8 |
| `sources/w13-vi-c-trailfx-clear/trailfx-clear.cpp` | `1166790340ae0c05…` | `Clear__9VITrailFx` 72 |
| `sources/w13-vi-c-trailfx-clear-segments-forward/trailfx-clear-segments-forward.cpp` | `8ac9f57a6fddf846…` | `ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT` 108 |
| `sources/w13-vi-c-vect3-distance/vect3-distance.cpp` | `0b8c461e68b5ec43…` | `Distance__C7VIVect3RC7VIVect3` 64 |
| `sources/w13-vi-c-zone-find-leaf/zone-find-leaf.cpp` | `146eb37d32e0cc96…` | `FindLeaf__C6VIZoneRC7VIVect3` 200 |

Two sources compile conditionally for the host: `raster-enable-point-light` and
`raster-set-point-light` use `NORRATH_HOST_CONTRACT` for the scratchpad address, and
`multimap-begin-emitterentry` uses it for one pointer load. On the EE the macros are plain
address casts and none of this is in the object.

## Authored but never executed

All of these were executed only in the private interpreter (against the pinned image and
logging stubs) and as host processes. None has run on EE hardware or in the game, and none
is claimed as an original byte match. The VU0 matrix multiply and the `sqrt.s`, `min.s` and
`max.s` helpers use the same instructions in inline assembly as the original; the host
builds use ordinary float arithmetic and do not model hardware rounding.

## Unknown

- All member names, field types, and the enumerations involved; offsets are from the image only.
- `ClearSegmentsForward__9VITrailFx...`: the image reads the link word from the original
  argument node on every round (not from the moving node). It is reproduced literally and
  the paired runs agree, but whether this is intentional in the original source is unknown.
- `FindLeaf__C6VIZone...`: the image addresses plane floats by the unmasked node index and
  links by the index masked with 0x0fffffff. The source reproduces this; behaviour with links
  that have bits 28, 29 or 31 set was not exercised.
- `Transform__C6VIBBox...`: operand order of `max.s` is only partly distinguished by the paired
  scenarios (NaN, signed zero); the paired mutation of the `max.s` order on x and y survives.
- `Distance`: one mutant (swapping the sign of the z difference) is equivalent and survives.
- `Close__9VIObjFile`/`PostOpen__9VIObjFile`: helper return values are ignored as in the image;
  the seek-origin values 1 and 2 are not named.
- `GetActorLocation`/`GetActorSpritePtr`: the mangled int parameter is an address in the image.
- Sizes: the authored bodies are not the original instruction sequences (for example the
  original pads `sqrt.s` with nops); only observable behaviour is compared.
- Pointer-sized fields are 4 bytes on the EE; the host contracts for
  `trailfx-clear`, `zone-find-leaf`, `multimap-next-emitterentry` use host-sized pointers
  and so only check identity and control flow.
