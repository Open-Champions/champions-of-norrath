# w14-sprite-pick: sprite pick and query virtuals

Branch `agents/w14-sprite-pick`. One commit per function, each under `experimental/sources/w14-sprite-pick-*` with an authored
`pick.cpp` or `query.cpp` and a `host_check.cpp` (not a plan input). Addresses and sizes come from the pinned private `SLUS_205.65`.
Nothing here ran on the EE or as gameplay; compiled sizes come from the reviewed EE `authored_cpp` profile and are not byte-identity
claims. 25 functions: 13 `Pick`, 5 `QueryEmitters` and 7 `QueryIntersection`.

## How each function was checked

* Each unit compiles alone with the reviewed EE g++ and the `authored_cpp` flags; a private `ld -r` of all 25 objects links with no
  duplicate definitions.
* Each has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp && ./a.out`, exit 0) that stubs the callees and
  checks call order, arguments, stored words and results.
* Each was compared with the image in a private paired run (a small R5900 interpreter outside git): original and freshly compiled
  authored function run from identical memory snapshots with identical stubbed callees; call logs (arguments, plus matrix and
  vector contents where passed), return value and every changed byte outside the stack must be equal. Run counts are in the
  commit messages. Deliberately wrong variants (offsets, slots, argument order, comparison sense, destructor order, constants)
  fail for every non-trivial unit; the trivial return-0 units were compared with three scenarios each. The stack words of the
  16-byte emitter-entry objects that the original never writes are not compared.
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.

## Units

| Function | Image address | Image bytes | Compiled bytes |
|---|---|---|---|
| `QueryIntersection__9VICSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene` | 0x010b2ee0 | 8 | 8 |
| `Pick__20VIEffectVolumeSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x0114e780 | 248 | 256 |
| `Pick__13VIFloraSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x010062c0 | 36 | 20 |
| `Pick__13VIGroupSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x0101d210 | 620 | 460 |
| `QueryEmitters__13VIGroupSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery` | 0x0101d480 | 468 | 356 |
| `QueryIntersection__13VIGroupSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene` | 0x0101d658 | 468 | 376 |
| `Pick__9VIHSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x01060c30 | 712 | 556 |
| `QueryEmitters__9VIHSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery` | 0x01060ef8 | 532 | 512 |
| `QueryIntersection__9VIHSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene` | 0x01061110 | 528 | 504 |
| `Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x01076470 | 364 | 316 |
| `QueryIntersection__11VILODSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene` | 0x010769c0 | 128 | 92 |
| `Pick__12VINameSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x010a1218 | 8 | 8 |
| `Pick__16VIParticleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x010aa210 | 708 | 448 |
| `Pick__12VIPointLightR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x010c3bb0 | 268 | 284 |
| `Pick__13VIPointSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x010cb9a0 | 268 | 284 |
| `Pick__14VISimpleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x011670b8 | 128 | 108 |
| `QueryIntersection__14VISimpleSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene` | 0x01167138 | 96 | 88 |
| `Pick__12VISkinSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x0116d548 | 36 | 20 |
| `Pick__13VISoundSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x0117bf10 | 708 | 448 |
| `QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery` | 0x01007f50 | 408 | 344 |
| `QueryIntersection__8VISpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene` | 0x01008148 | 8 | 8 |
| `Pick__19VIStreamAudioSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster` | 0x010190e8 | 268 | 284 |
| `QueryEmitters__19VIStreamAudioSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery` | 0x01018a20 | 728 | 660 |
| `QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars` | 0x010fdff0 | 444 | 392 |
| `QueryIntersection__7VIWorldR20VISceneIntersectVars` | 0x010feb30 | 108 | 88 |

Image bytes are the symbol-table extents; compiled bytes are the authored objects' extents.

## Observed, inferred, unknown

* Observed: five `Pick` virtuals return 0 or tail-call the raster (`PickPrimBuffer` with two words of the sprite; the simple sprite
  asks `GetNextColorBuffer` first and picks the three-integer overload when it is non-negative). Name sprite `Pick` and the
  generic and CSprite `QueryIntersection` return 0 without touching anything.
* Observed: `VIPointLight`, `VIPointSprite`, `VIStreamAudioSprite`, `VISoundSprite`, `VIParticleSprite` and `VIEffectVolumeSprite`
  picks share one shape: ask the scene for a UI sprite index (a different getter each time, and the effect-volume sprite uses the
  stream-audio getter), return 0 for -1, replace the raster's model-view with a scale-limited copy and call
  `PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri`, then destroy the temporary matrix. The point light, point sprite and stream-audio
  sprite limit the scale through their own `ClampScale` call, the sound and particle sprites inline it (rows longer than 4 are
  rescaled to 4), and the effect-volume sprite does not limit it. Nothing restores the old model-view.
* Observed: `VILODSprite`, `VIGroupSprite` and `VIHSprite` dispatch to child sprites through the word at child+60 (a table of 8-byte
  entries: signed halfword adjustment, function word). Slots seen: +32 sphere collide and +40 ray collide (sibling package),
  +48 pick, +56 emitter query, +64 intersection query. The group and HSprite place each child with `Transform`/`Mul` and set the
  raster's model-view (pick), pass the product (queries) and stop on a negative query result; the pick stops on the first hit in the
  group and keeps walking in the HSprite, storing the last hit index at filter+12. Emitter queries store the entry's sprite index at query+12.
* Observed: `VILODSprite::Pick` selects the first record (count at +68, 8-byte records from +72: float threshold, sprite index) whose
  squared threshold is not below the squared distance between the model-view translation and the segment's first three floats.
* Observed: `VISprite::QueryEmitters` and `VIStreamAudioSprite::QueryEmitters` rank an emitter record in the multimap at query+16 and
  erase its first element when the map is larger than the limit at query+4. The first uses a key of the float at +44 times the matrix
  scale over the translation distance; the second tests the query point (taken through `InvertAffine`) against the box at sprite+8, picks a
  base of 1, 2 or 3 from the word at +68 (any other value returns -1), and subtracts 0.0001 times the distance from the box centre.
* Observed: `VIWorld::QueryEmitters(int, vars)` walks a plane tree (24-byte records at world+56) with a sphere and calls
  `QueryEmitters__6VIZone` at the leaves; `VIWorld::QueryIntersection(vars)` copies the counter at +68, starts the integer-indexed
  traversal at 0 or 0x40000000 by the word at +52, and increments the counter on success.
* Inferred: the names of the roles above (level-of-detail selection, group/attachment placement, UI sprite selection, scale limit,
  plane tree, sphere test). They are based on call shapes, not on symbol names beyond those in the mangled signatures.
* Unknown: the names of all fields, table slots and constants; what filter+12 means to the caller; the meaning of the word at
  sprite+52 (compared with the query kind at query+0); the 16-byte layout of `VIEmitterEntry` (only two words are written); the
  meaning of the world's top-nibble node bits other than 0x40000000; the true types behind the opaque pointers.

## Imports closed

Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__12VINameSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__12VIPointLightR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__12VISkinSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__13VIFloraSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__13VIGroupSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__13VIPointSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__13VISoundSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__14VISimpleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__16VIParticleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__19VIStreamAudioSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__20VIEffectVolumeSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, Pick__9VIHSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster, QueryEmitters__13VIGroupSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery, QueryEmitters__19VIStreamAudioSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery, QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars, QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery, QueryEmitters__9VIHSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery, QueryIntersection__11VILODSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene, QueryIntersection__13VIGroupSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene, QueryIntersection__14VISimpleSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene, QueryIntersection__7VIWorldR20VISceneIntersectVars, QueryIntersection__8VISpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene, QueryIntersection__9VICSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene, QueryIntersection__9VIHSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene

## Imports opened

Callees left unresolved (none authored here; several are probably other packages' symbols and may already be defined in the shared
selection, so check against the integrated list):
Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry, Begin__Ct6VIPool1Z19VIHSpriteAttachment, CalcLODLevelIndex__9VIHSpriteR13VIPickSegmentR8VIRaster, CalcLODLevelIndex__9VIHSpriteRC10VIMatrix44, CalcTransAboutCenter__9VIHSpriteRC10VIMatrix44R7VISceneR10VIMatrix44, ClampScale__12VIPointLightR10VIMatrix44, ClampScale__13VIPointSpriteR10VIMatrix44, ClampScale__19VIStreamAudioSpriteR10VIMatrix44, Contains__C6VIBBoxRC7VIVect3, End__Ct6VIPool1Z19VIHSpriteAttachment, Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi, GetLightUISprite__7VIScene, GetModelView__C8VIRaster, GetNextColorBuffer__7VIScene, GetParticleUISprite__7VIScene, GetPointUISprite__7VIScene, GetSoundUISprite__7VIScene, GetStreamAudioUISprite__7VIScene, Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry, InvertAffine__10VIMatrix44RC10VIMatrix44, Magnitude__C7VIVect3, Mul__10VIMatrix44RC10VIMatrix44T1, Next__Ct6VIPool1Z19VIHSpriteAttachmenti, PickPrimBuffer__8VIRasterR13VIPickSegmentii, PickPrimBuffer__8VIRasterR13VIPickSegmentiii, PickSprite__7VISceneR13VIPickSegmentR13VISceneFilteri, Process__9VIHSpriteiR7VIScene, QueryEmitters__6VIZoneR23VISceneEmitterQueryVars, QueryIntersection__7VIWorldiR20VISceneIntersectVars, QueryObj__9VICollideRC6VIBBoxR14VIQueryTriListi, SetBlendMatrices__9VIHSpriteR8VIRaster, SetModelView__8VIRasterRC10VIMatrix44, SetWorld__9VICollideRC10VIMatrix44, Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry, Sprite__7VIScenei, Transform__10VIMatrix44RC7VIVect3fT1, _$_10VIMatrix44, sceVu0CopyMatrix

## Build-plan entries needed

All `authored_cpp`, empty header closure, `sha256` of the source at the last commit of this branch (`emitted_functions` recorded):

```json
{"emitted_functions":[{"compiled_body_bytes":8,"name":"QueryIntersection__9VICSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_csprite_query_intersection.o","profile":"authored_cpp","sha256":"0139bf8b84b06fc05c51d127664da00a13cd6f6a021b5916199358d3c14edfb3","source":"sources/w14-sprite-pick-csprite-query-intersection/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":256,"name":"Pick__20VIEffectVolumeSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_effect_volume_sprite_pick.o","profile":"authored_cpp","sha256":"fd132656b3352cf567e65a8fcce3f85427babcc54a473b1ba2ca221350f7e03b","source":"sources/w14-sprite-pick-effect-volume-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"Pick__13VIFloraSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_flora_sprite_pick.o","profile":"authored_cpp","sha256":"8418cc962573e2b9e202ed2c60e100e0cc76011a8b4dffd3f724854a8a6b1c29","source":"sources/w14-sprite-pick-flora-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":460,"name":"Pick__13VIGroupSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_group_sprite_pick.o","profile":"authored_cpp","sha256":"d82b7e9894a0b74eb3212a98320129d5223309c77e9691d5ba8791e71a6ec6e0","source":"sources/w14-sprite-pick-group-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":356,"name":"QueryEmitters__13VIGroupSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_group_sprite_query_emitters.o","profile":"authored_cpp","sha256":"04bb07a7b01074ed480900beb80f1ca12f9623af0c10a906046698e476e6c26a","source":"sources/w14-sprite-pick-group-sprite-query-emitters/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":376,"name":"QueryIntersection__13VIGroupSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_group_sprite_query_intersection.o","profile":"authored_cpp","sha256":"6f1cd5aee51a5e1260627f0c4ed7a84e25d7bd815180f44bcdd5e196f9f10ff5","source":"sources/w14-sprite-pick-group-sprite-query-intersection/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":556,"name":"Pick__9VIHSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_hsprite_pick.o","profile":"authored_cpp","sha256":"24d1110b47cfa1284f04dfb9c376e51f5c4421888fac87320cb2a8a34930ac86","source":"sources/w14-sprite-pick-hsprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":512,"name":"QueryEmitters__9VIHSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_hsprite_query_emitters.o","profile":"authored_cpp","sha256":"bb5196dda1e6989fbdd21626b368831226c9ed8c098e5e33d8a254871ce57ecf","source":"sources/w14-sprite-pick-hsprite-query-emitters/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":504,"name":"QueryIntersection__9VIHSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_hsprite_query_intersection.o","profile":"authored_cpp","sha256":"a3b1f82ea068c73e98cf1076b0373d73459fd3240f23af6c6d404590061b9dfd","source":"sources/w14-sprite-pick-hsprite-query-intersection/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":316,"name":"Pick__11VILODSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_lod_sprite_pick.o","profile":"authored_cpp","sha256":"cb5afa4d0dcdedfa01dd27888fdb4e18228b37dd54b5b6d2b6a762f5722b0eca","source":"sources/w14-sprite-pick-lod-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"QueryIntersection__11VILODSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_lod_sprite_query_intersection.o","profile":"authored_cpp","sha256":"4a3e9e90e4864826debfce1a67ff9c95ee7ed999be367e94e7ced2fadcf3f090","source":"sources/w14-sprite-pick-lod-sprite-query-intersection/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"Pick__12VINameSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_name_sprite_pick.o","profile":"authored_cpp","sha256":"bf207f5e29136d028a97743f8e711c071f54eb647d35923a24d9ada950e139a1","source":"sources/w14-sprite-pick-name-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":448,"name":"Pick__16VIParticleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_particle_sprite_pick.o","profile":"authored_cpp","sha256":"ce2676560289f26c6583809f0880a2c45010860acec2bc8d0e5963989caff4ba","source":"sources/w14-sprite-pick-particle-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":284,"name":"Pick__12VIPointLightR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_point_light_pick.o","profile":"authored_cpp","sha256":"c77f9002d11693f2475e0058ee7d6f471334390e9b6265a9a5d805ddf8507058","source":"sources/w14-sprite-pick-point-light-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":284,"name":"Pick__13VIPointSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_point_sprite_pick.o","profile":"authored_cpp","sha256":"15226df3576da97521b146d64dd1f15550d1595413b689818c4bf90e3af773cd","source":"sources/w14-sprite-pick-point-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":108,"name":"Pick__14VISimpleSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_simple_sprite_pick.o","profile":"authored_cpp","sha256":"b8abbd8aeefa7e0690eab337ec33b5ccdc03271b154c0c37711666e2cfc97594","source":"sources/w14-sprite-pick-simple-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"QueryIntersection__14VISimpleSpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_simple_sprite_query_intersection.o","profile":"authored_cpp","sha256":"5fc6cb4571aa34544fe67f7675ecd930f370ec04bc2c243c81e41e89e13d9d0c","source":"sources/w14-sprite-pick-simple-sprite-query-intersection/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"Pick__12VISkinSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_skin_sprite_pick.o","profile":"authored_cpp","sha256":"db7ed5dd4e5cc78ce2ccd62dec8113d524691604cd8d5b29e956836d3b5b13f3","source":"sources/w14-sprite-pick-skin-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":448,"name":"Pick__13VISoundSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_sound_sprite_pick.o","profile":"authored_cpp","sha256":"4c24742245c5e9a30901598e5cadd0e49b6df57197dad3bb1536a970f6c58e0a","source":"sources/w14-sprite-pick-sound-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":344,"name":"QueryEmitters__8VISpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_sprite_query_emitters.o","profile":"authored_cpp","sha256":"f72b9ad428a795ecdcce6a7a023838ff787b12abbf329c9ac5d4d9374cdd97f4","source":"sources/w14-sprite-pick-sprite-query-emitters/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"QueryIntersection__8VISpriteRC6VIBBoxR14VIQueryTriListRC10VIMatrix44R7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_sprite_query_intersection.o","profile":"authored_cpp","sha256":"d9be87b98e6586ae767a33ba7ccc936d5c779384ce6b98e75c40b1337ae9c8ea","source":"sources/w14-sprite-pick-sprite-query-intersection/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":284,"name":"Pick__19VIStreamAudioSpriteR13VIPickSegmentR13VISceneFilterR7VISceneR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_stream_audio_sprite_pick.o","profile":"authored_cpp","sha256":"a91b0a5ca0f2ff077a8c27f7083f2109773b766be181674e8b294f15cdc25574","source":"sources/w14-sprite-pick-stream-audio-sprite-pick/pick.cpp"}
{"emitted_functions":[{"compiled_body_bytes":660,"name":"QueryEmitters__19VIStreamAudioSpriteR7VISceneRC10VIMatrix44R14VIEmitterQuery"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_stream_audio_sprite_query_emitters.o","profile":"authored_cpp","sha256":"56ddf92f50ef700731d025745df8e9e5103e03d430f50383dda2c46a5a3478cb","source":"sources/w14-sprite-pick-stream-audio-sprite-query-emitters/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":392,"name":"QueryEmitters__7VIWorldiR23VISceneEmitterQueryVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_world_query_emitters.o","profile":"authored_cpp","sha256":"568325c89aeff0d6788c6add45660a5d135072fddd45cb5862be73f5d2d9a44d","source":"sources/w14-sprite-pick-world-query-emitters/query.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"QueryIntersection__7VIWorldR20VISceneIntersectVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_sprite_pick_world_query_intersection.o","profile":"authored_cpp","sha256":"deefbe8abf8eccaf98f9f272f4d0a1d4787dfd8bd80160078af039cd460167d3","source":"sources/w14-sprite-pick-world-query-intersection/query.cpp"}
```

## Authored but never executed

All 25 functions. Each was run only under the private R5900 interpreter against stubbed callees and under native host checks;
none ran with real callees, real scene data or on hardware. The 13 `Pick` units and the emitter queries that touch the model-view
or emitter map were compared with stubbed matrix and map routines, so the real `Transform`, `Mul`, `InvertAffine`, multimap and pool
behaviour is outside the checks.

## What is unknown

See the last bullet of the observed/inferred/unknown section. Nothing was blocked on the owner.
