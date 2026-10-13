# Sprite raster and instance-type virtuals

Package `w14-sprite-raster`, branch `agents/w14-sprite-raster`, base `a7bb4e51` of the shared reconstruction draft. The 21 names in the brief were all unresolved imports of the partial build at that base. Addresses and extents are symbol values of the private reference `work/private/SLUS_205.65` (EE `nm`), and bodies were read with the EE `objdump`. No game bytes, tables or listings are committed; constants quoted are the values the code uses.

Each function is its own commit, compiled alone with the reviewed EE `g++` `authored_cpp` flags, and has a `host_check.cpp` (system `c++ -std=c++17 -Wall -Wextra -Werror`) that returns non-zero when a call, argument, order, store or compare is wrong. Host checks are not build inputs. No full partial build was run (one-build-at-a-time rule); I compared each unit's undefined symbols with the `unresolved-imports.json` of the newest earlier partial build on this machine (`work/plan14-build`). Compiled bodies are not byte matches of the image extents (these are behaviour reconstructions), and nothing here has run on the EE or as gameplay.

## Functions

| symbol | image | extent | compiled | source |
|---|---|---|---|---|
| `GetInstType__C13VIGroupSpriteR7VIScene` | `0x101db98` | 168 | 260 | `sources/w14-sprite-raster-getinst-group/get_inst_type.cpp` |
| `GetInstType__C13VISoundSpriteR7VIScene` | `0x117c2a8` | 8 | 8 | `sources/w14-sprite-raster-getinst-sound/get_inst_type.cpp` |
| `GetInstType__C16VIParticleSpriteR7VIScene` | `0x10aa728` | 8 | 8 | `sources/w14-sprite-raster-getinst-particle/get_inst_type.cpp` |
| `GetInstType__C19VIStreamAudioSpriteR7VIScene` | `0x1018fd0` | 8 | 8 | `sources/w14-sprite-raster-getinst-streamaudio/get_inst_type.cpp` |
| `GetInstType__C20VIEffectVolumeSpriteR7VIScene` | `0x114e768` | 8 | 8 | `sources/w14-sprite-raster-getinst-effectvolume/get_inst_type.cpp` |
| `GetInstType__C8VISpriteR7VIScene` | `0x1008120` | 8 | 8 | `sources/w14-sprite-raster-getinst-vsprite/get_inst_type.cpp` |
| `GetInstType__C9VICSpriteR7VIScene` | `0x10b2d88` | 8 | 8 | `sources/w14-sprite-raster-getinst-csprite/get_inst_type.cpp` |
| `Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x10762e8` | 388 | 580 | `sources/w14-sprite-raster-raster-lod/raster.cpp` |
| `Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x10a0938` | 548 | 648 | `sources/w14-sprite-raster-raster-namesprite/raster.cpp` |
| `Raster__12VIPointLightR8VICameraR7VISceneR8VIRasterUi` | `0x10c3aa8` | 256 | 236 | `sources/w14-sprite-raster-raster-pointlight/raster.cpp` |
| `Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x116d4f0` | 72 | 64 | `sources/w14-sprite-raster-raster-skin/raster.cpp` |
| `Raster__13VIFloraSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x1006288` | 40 | 32 | `sources/w14-sprite-raster-raster-flora/raster.cpp` |
| `Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x101cb18` | 536 | 440 | `sources/w14-sprite-raster-raster-group/raster.cpp` |
| `Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x10cb890` | 256 | 236 | `sources/w14-sprite-raster-raster-pointsprite/raster.cpp` |
| `Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x117bc58` | 692 | 404 | `sources/w14-sprite-raster-raster-sound/raster.cpp` |
| `Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x1166ff0` | 132 | 124 | `sources/w14-sprite-raster-raster-simple/raster.cpp` |
| `Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x10a9b98` | 1656 | 1200 | `sources/w14-sprite-raster-raster-particle/raster.cpp` |
| `Raster__19VIStreamAudioSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x1018fd8` | 256 | 236 | `sources/w14-sprite-raster-raster-streamaudio/raster.cpp` |
| `Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x114e0a8` | 1280 | 1172 | `sources/w14-sprite-raster-raster-effectvolume/raster.cpp` |
| `Raster__9VICSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x10b2d90` | 260 | 236 | `sources/w14-sprite-raster-raster-csprite/raster.cpp` |
| `Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi` | `0x1060548` | 632 | 532 | `sources/w14-sprite-raster-raster-hsprite/raster.cpp` |

Not in the brief: `Raster__8VISprite...` (0x1008128, 8 bytes, `move v0,zero`) was left alone.

## Observed

- The six constant `GetInstType` bodies are `jr ra; li v0,<0 or 1>`. `VISprite` returns 0; the stream-audio, sound, particle, CSprite and effect-volume sprites return 1. `VIGroupSprite` loops its members (count word at +64, 32-byte records from +68, first word an index for `VIScene::Sprite`), calls each member sprite's virtual (vtable word at +60, entry at table+16) with the scene and returns 1 on the first answer equal to 1, else 0.
- `Flora`, `Skin` and `Simple` are tail calls to `VIRaster::DrawPrimBuffer`; `Skin` returns 0 when bit 0 of its word at +64 is set; `Simple` picks the five-argument overload when `GetNextColorBuffer` is non-negative.
- `StreamAudio`, `PointLight` and `PointSprite` share one shape: UI-sprite query, copy of the model view, their own `ClampScale`, `SetModelView`, `RasterUISprite`, destructor of the local matrix. `Sound` and `Particle` inline the clamp: each of three matrix rows whose reciprocal length is below 0.25 is rescaled to length 4 (reciprocal through `rsqrt.s`).
- `LOD` chooses a level by squared model-view translation distance against squared thresholds and calls `RasterSprite`; `HSprite`, `Group`, `CSprite` walk members/attachments with matrix locals; `NameSprite` queues a 144-byte record in the global `NameDefers` table (24 records, counter `NumNameDefers`); `EffectVolume` and `Particle` build a rigid-inverse matrix from the camera matrix (fourth row replaced, w column zeroed) and drive `VIEffectVolume` / `VIParticleEmitter`.
- Per-function comments at the top of each source hold the full record, including field offsets used, constants (the `0x3f199999` and `0x3d088888` float patterns are exact image bits) and evaluation order.

## Inferred

- `Transform__10VIMatrix44RC7VIVect3fT1` takes a trailing float (the image loads the float argument register and the callee copies it); the mangled name lists two arguments only. Declared with a float fourth parameter in `Raster__13VIGroupSprite`.
- The 64-byte matrix locals are `VIMatrix44` objects: each is zero-filled inline and destroyed with `_$_10VIMatrix44(local, 2)`. The source models them as aligned `float[16]` plus those destructor calls; the real constructor is inlined in the image and not called.
- The rigid-inverse formula and the 4-length row clamp are described by shape only.

## Unknown

Field names for every sprite/camera/raster offset used (sprite +48, +64, +68, +72, +80, +88, +96, +1072; camera +28, +48), what the sprite-kind answer 1 means, what `Process`/`SetBlendMatrices` answers mean, the `VIRect` layout, the lifetime/owner of `NameDefers`, what the mode word at particle +1072 means, and the R5900 `rsqrt.s` rounding (host checks use IEEE division and `sqrt`). The `Raster` argument "flags" is only known as an unsigned word passed through or tested with bit 0 / bit 1.

## Imports closed

- `GetInstType__C13VIGroupSpriteR7VIScene`
- `GetInstType__C13VISoundSpriteR7VIScene`
- `GetInstType__C16VIParticleSpriteR7VIScene`
- `GetInstType__C19VIStreamAudioSpriteR7VIScene`
- `GetInstType__C20VIEffectVolumeSpriteR7VIScene`
- `GetInstType__C8VISpriteR7VIScene`
- `GetInstType__C9VICSpriteR7VIScene`
- `Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__12VIPointLightR8VICameraR7VISceneR8VIRasterUi`
- `Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__13VIFloraSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__19VIStreamAudioSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__9VICSpriteR8VICameraR7VISceneR8VIRasterUi`
- `Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi`

## Imports opened

None new relative to `work/plan14-build/unresolved-imports.json`. Four names my sources reference are still unresolved there (owned elsewhere): `SetLocation__17VIParticleEmitterRC7VIVect3`, `Update__17VIParticleEmitter`, `Transform__10VIMatrix44RC7VIVect3fT1`, `SetModelView__8VIRasterRC10VIMatrix44`. The data symbols `_12VINameSprite$NumNameDefers` (4 bytes) and `_12VINameSprite$NameDefers` (3456 bytes) are referenced by `Raster__12VINameSprite...` and are defined by the image; the base build resolved them (not listed as unresolved there). Every other callee is defined in that build.

## Build-plan entries needed

The 21 entries below (add after the last input, `input_order` to be assigned by the integrator; `files` and `snapshot` hashes also need the per-file sha256 of every listed source and header). Hash values are of the committed source files at the last commit of this branch. `bytes.hpp` is the existing `sources/vslots-b-bytes/bytes.hpp`.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_csprite.o",
  "profile": "authored_cpp",
  "sha256": "cbbd193bba14d88eeaa8254b8afc172bcba7ccf8c9a8168bd0b84b89ab575175",
  "source": "sources/w14-sprite-raster-getinst-csprite/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C9VICSpriteR7VIScene",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_effectvolume.o",
  "profile": "authored_cpp",
  "sha256": "661af9fe699b4caef51278ec9ffab505525ef343c58eb95938fe445543651d17",
  "source": "sources/w14-sprite-raster-getinst-effectvolume/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C20VIEffectVolumeSpriteR7VIScene",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_group.o",
  "profile": "authored_cpp",
  "sha256": "5a9640f5efff766c8bca8f10c83fae8b251f8ece74d1dd650d918644bc80b548",
  "source": "sources/w14-sprite-raster-getinst-group/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C13VIGroupSpriteR7VIScene",
    "compiled_body_bytes": 260
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_particle.o",
  "profile": "authored_cpp",
  "sha256": "2366549701870199cede6d74d3a8534c7461e3976eb98f26cb203f61cbca9fd2",
  "source": "sources/w14-sprite-raster-getinst-particle/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C16VIParticleSpriteR7VIScene",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_sound.o",
  "profile": "authored_cpp",
  "sha256": "ead2313563309b5b01bf0829f8db936a8e1896c7f733a5c874b4844a79755ccd",
  "source": "sources/w14-sprite-raster-getinst-sound/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C13VISoundSpriteR7VIScene",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_streamaudio.o",
  "profile": "authored_cpp",
  "sha256": "b8ae650f64d3e450152be04679c7e386c6d2f08bc4ddecb4785880e0f295bbb4",
  "source": "sources/w14-sprite-raster-getinst-streamaudio/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C19VIStreamAudioSpriteR7VIScene",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_getinst_vsprite.o",
  "profile": "authored_cpp",
  "sha256": "1674c348b510f4a1ff7ec622ebf35a276637b36212195795482091f3f0a0fdb5",
  "source": "sources/w14-sprite-raster-getinst-vsprite/get_inst_type.cpp",
  "emitted_functions": [
   {
    "name": "GetInstType__C8VISpriteR7VIScene",
    "compiled_body_bytes": 8
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_csprite.o",
  "profile": "authored_cpp",
  "sha256": "25f8040b0141d2b61c628e434b2943165c76a2880682bf826fa4fd2625437994",
  "source": "sources/w14-sprite-raster-raster-csprite/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__9VICSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 236
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_effectvolume.o",
  "profile": "authored_cpp",
  "sha256": "e2427719b2bc19f6bed90b4f8da8af76864e9164af7bf96e50c293d4f5267dc5",
  "source": "sources/w14-sprite-raster-raster-effectvolume/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__20VIEffectVolumeSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 1172
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_flora.o",
  "profile": "authored_cpp",
  "sha256": "53c477a169ee47d933660271991ab4dfb6b0115ef64aed2592961146ecbadc47",
  "source": "sources/w14-sprite-raster-raster-flora/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__13VIFloraSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_group.o",
  "profile": "authored_cpp",
  "sha256": "0d35bbfbd20ac1591aeab94067d4f8e8b3147277edca14fc7526159f2838e207",
  "source": "sources/w14-sprite-raster-raster-group/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__13VIGroupSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 440
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_hsprite.o",
  "profile": "authored_cpp",
  "sha256": "8c262b291215e5ca97017b005d16135e2eeb0114f5caf0f1d3fc7c099655bc17",
  "source": "sources/w14-sprite-raster-raster-hsprite/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__9VIHSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 532
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_lod.o",
  "profile": "authored_cpp",
  "sha256": "d7fb9717158022da631079892315b4019c19fa93d4a4fe037516ad9ff03ec339",
  "source": "sources/w14-sprite-raster-raster-lod/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__11VILODSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 580
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_namesprite.o",
  "profile": "authored_cpp",
  "sha256": "206aa7fd2e1de03532b7dbba88b94f8d108ec9a36f041024e8144e216863eff7",
  "source": "sources/w14-sprite-raster-raster-namesprite/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__12VINameSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 648
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_particle.o",
  "profile": "authored_cpp",
  "sha256": "da7655fc90146211d664d47a96eae7641246282ee95a0126984653fc005fe34f",
  "source": "sources/w14-sprite-raster-raster-particle/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__16VIParticleSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 1200
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_pointlight.o",
  "profile": "authored_cpp",
  "sha256": "ff6265a1f9d585ebf3dca71bc9150ee9ddfad1e6cdfab3ad164a6d0295e55948",
  "source": "sources/w14-sprite-raster-raster-pointlight/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__12VIPointLightR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 236
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_pointsprite.o",
  "profile": "authored_cpp",
  "sha256": "f5180aa0d12c2626bf0e4a65bb3847144b554cda40660c3bf032305593c934dd",
  "source": "sources/w14-sprite-raster-raster-pointsprite/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__13VIPointSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 236
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_simple.o",
  "profile": "authored_cpp",
  "sha256": "507e36c32f1b24911d6fd9929a3b847caea11409464d1566bf136cfe8fe41aab",
  "source": "sources/w14-sprite-raster-raster-simple/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__14VISimpleSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 124
   }
  ]
 },
 {
  "header_closure": [
   "sources/vslots-b-bytes/bytes.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_skin.o",
  "profile": "authored_cpp",
  "sha256": "746e06803d8af29e6f61499f14ac4a98f011371310bb6a727bd41b6c83d3a632",
  "source": "sources/w14-sprite-raster-raster-skin/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__12VISkinSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 64
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_sound.o",
  "profile": "authored_cpp",
  "sha256": "8752ae46849d35a4ca8babf963d13fa6a31f3b1831b3ba974e8e924c8e5e62cf",
  "source": "sources/w14-sprite-raster-raster-sound/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__13VISoundSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 404
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w14_sprite_raster_raster_streamaudio.o",
  "profile": "authored_cpp",
  "sha256": "a97a4fdd1324e765b8077f232d5ce749bd950a061e2235cf8efb899ad9404f67",
  "source": "sources/w14-sprite-raster-raster-streamaudio/raster.cpp",
  "emitted_functions": [
   {
    "name": "Raster__19VIStreamAudioSpriteR8VICameraR7VISceneR8VIRasterUi",
    "compiled_body_bytes": 236
   }
  ]
 }
]
```

## Authored but never executed

All 21 functions: they are host-checked only with mocks for the callees and with fake object memory; none has run on the EE, in an interpreter, or against the original code in a paired run. Floating-point behaviour on the R5900 (denormals, rounding, `rsqrt.s`) is not covered. The 64-byte matrix locals do not call the real `VIMatrix44` constructor.

## Mutation checks

`Raster__20VIEffectVolumeSprite` was run against two hand-made mutants (swapped `Mul` operands, wrong index in the fourth-row formula); both were caught. Other host checks assert order, argument identity, thresholds on both sides of each compare, and untouched-state cases, but were not each mutation-tested.
