# w15-vi-particle-b: particle emitter, effect volume, light and point sprite (2 of 2)

Branch `agents/w15-vi-particle-b`. Private reference executable `work/private/SLUS_205.65` (never committed). Addresses and sizes
below are from the pinned SLUS-20565 symbol table. Every function is one commit, compiled with the reviewed EE g++ `authored_cpp`
profile, and compared against the original in a private R5900 interpreter (outside git, a copy of the earlier paired-run harness):
the original and the authored function start from identical memory, call the same Python stand-ins for their callees, and the
return register, the callee call log and every changed byte outside the stack must be identical. Instruction coverage of the
original is recorded per function. Each function also has a `host_check.cpp` (not a plan input) for the contract on the host, and
for each non-trivial function a handful of deliberate mutations were confirmed to be detected by the paired run. Fresh EE objects
are not original-byte matches; nothing here ran on the EE hardware or in a full game run.

## Functions

| Symbol | Address | Original bytes | Fresh bytes | Directory under `experimental/sources/` |
|---|---|---|---|---|
| `TextureConfiguration__C22VIParticleDefinitionEx` | `0x010b4c98` | 8 | 8 | `w15-vi-particle-b-definition-texture-configuration` |
| `Density__C14VIEffectVolume` | `0x0114a2a0` | 8 | 8 | `w15-vi-particle-b-effectvolume-density` |
| `Flags__C14VIEffectVolume` | `0x0114a2b8` | 8 | 8 | `w15-vi-particle-b-effectvolume-flags` |
| `Surface__C14VIEffectVolumei` | `0x0114a318` | 16 | 16 | `w15-vi-particle-b-effectvolume-surface` |
| `Type__C14VIEffectVolume` | `0x0114a238` | 8 | 8 | `w15-vi-particle-b-effectvolume-type` |
| `Direction__C17VIParticleEmitter` | `0x010b8be0` | 8 | 8 | `w15-vi-particle-b-emitter-direction` |
| `Location__C17VIParticleEmitter` | `0x010b8b68` | 8 | 8 | `w15-vi-particle-b-emitter-location` |
| `Space__C17VIParticleEmitter` | `0x010b8b40` | 8 | 8 | `w15-vi-particle-b-emitter-space` |
| `StaticBounds__C17VIParticleEmitter` | `0x010b7308` | 396 | 320 | `w15-vi-particle-b-emitter-static-bounds` |
| `Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry` | `0x011f4a08` | 376 | 380 | `w15-vi-particle-b-multimap-allocate-emitterentry` |
| `Clear__t10VIMultiMap2ZfZ14VIEmitterEntry` | `0x01226698` | 116 | 152 | `w15-vi-particle-b-multimap-clear-emitterentry` |
| `Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi` | `0x011f4168` | 512 | 660 | `w15-vi-particle-b-multimap-erase-emitterentry` |
| `Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry` | `0x011f3e48` | 796 | 1008 | `w15-vi-particle-b-multimap-insert-emitterentry` |
| `Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry` | `0x011f4de0` | 20 | 20 | `w15-vi-particle-b-multimap-size-emitterentry` |
| `Begin__Ct6VIPool1ZP15VIParticleMotif` | `0x012088c8` | 44 | 32 | `w15-vi-particle-b-pool-begin-particlemotif` |
| `DestroyPool__t6VIPool1ZP20VIParticleDefinition` | `0x01208e08` | 92 | 88 | `w15-vi-particle-b-pool-destroypool-particledefinition` |
| `End__Ct6VIPool1ZP15VIParticleMotif` | `0x012088f8` | 12 | 12 | `w15-vi-particle-b-pool-end-particlemotif` |
| `Erase__t6VIPool1ZP15VIParticleMotifi` | `0x01208690` | 204 | 192 | `w15-vi-particle-b-pool-erase-particlemotif` |
| `Next__Ct6VIPool1ZP15VIParticleMotifi` | `0x01208908` | 44 | 60 | `w15-vi-particle-b-pool-next-particlemotif` |
| `Size__Ct6VIPool1ZP15VIParticleMotif` | `0x01208938` | 8 | 8 | `w15-vi-particle-b-pool-size-particlemotif` |
| `GetParticleUISprite__7VIScene` | `0x01144d40` | 8 | 8 | `w15-vi-particle-b-scene-particle-ui-sprite` |
| `VITrailFxEnd__FP9VITrailFx` | `0x01027110` | 52 | 40 | `w15-vi-particle-b-trailfx-end` |

## What was observed

- Accessors return fields at fixed offsets: `VIEffectVolume` type at +36, density (float) at +44, flags at +60, per-surface words from +64
  (index times four, no bounds check); `VIParticleEmitter` returns the addresses `this+16` (Location), `this+40` (Direction) and
  `this+96` (Space); `VIParticleDefinitionEx` texture configuration word at +760; `VIScene` particle UI sprite word at +1136.
- `VIPool<VIParticleMotif*>` shares the 12-byte-entry layout of the existing `VIParticleDefinition*` instantiation: count at +0,
  entry table pointer at +8, free-chain head at +12, first/last live entry at +16/+20, entry link words at +4 (next) and +8 (previous,
  -2 when free). End is the constant 0x7fffffff (inlined in Next and Erase).
- `VIMultiMap<float, VIEmitterEntry>` nodes are 112 bytes: +0 flag word, +4 parent, +8 left, +12 right, +16 float key, +32/+36 two
  value words, +48 a VIMatrix44; node 0 is a null/sentinel node that some stores reach. Insert is a red-black style insertion
  (equal keys go right) and Erase unlinks with a fix-up only when the removed node's flag word is zero.
- `StaticBounds` transforms the eight corners of the box member at +916 by the matrix at +40, adds the vector at +16, and grows a local
  box with `Bound`; the box is returned through a hidden first argument.
- `VITrailFxEnd` moves its node from the active list to the inactive list (`Remove` then `AddTail` on the static at 0x011e5208).

## What is inferred (not proven)

- The flag word of multimap nodes is a colour (zero = black). The pool count/chain words have the roles given in the source comments.
- `Direction` returns a matrix (it is used as the VIMatrix33 operand of the vector-matrix multiply); the accessor name suggests otherwise.
- `DestroyPool` for `VIParticleDefinition*` only walks the pool: no per-entry destructor runs in the image.

## Imports closed

`TextureConfiguration__C22VIParticleDefinitionEx`, `Density__C14VIEffectVolume`, `Flags__C14VIEffectVolume`, `Surface__C14VIEffectVolumei`, `Type__C14VIEffectVolume`, `Direction__C17VIParticleEmitter`, `Location__C17VIParticleEmitter`, `Space__C17VIParticleEmitter`, `StaticBounds__C17VIParticleEmitter`, `Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry`, `Clear__t10VIMultiMap2ZfZ14VIEmitterEntry`, `Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry`, `Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry`, `Begin__Ct6VIPool1ZP15VIParticleMotif`, `DestroyPool__t6VIPool1ZP20VIParticleDefinition`, `End__Ct6VIPool1ZP15VIParticleMotif`, `Erase__t6VIPool1ZP15VIParticleMotifi`, `Next__Ct6VIPool1ZP15VIParticleMotifi`, `Size__Ct6VIPool1ZP15VIParticleMotif`, `GetParticleUISprite__7VIScene`, `VITrailFxEnd__FP9VITrailFx`

## Imports opened

New undefined references introduced by these units (none of them is defined by an authored unit in the current plan):
`AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii`, `EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi`,
`LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi`,
`Next__Ct6VIPool1ZP20VIParticleDefinitioni`, `Remove__7VILNode`, `__10VIMatrix44RC10VIMatrix44`, `__ml__FRC7VIVect3RC10VIMatrix33`, and the local data
`VITrailFx$sInactiveTrailFx` (published by earlier work under that name; the image symbol is `sInactiveTrailFx` at 0x011e5208).
Already planned elsewhere and used here: `AddTail__7VILListP7VILNode`, `Begin__Ct6VIPool1ZP20VIParticleDefinition`, `Bound__6VIBBoxiRC7VIVect3`,
`DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry`, `Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi`, `Store__C6VIBBoxP7VIVect3`, `__builtin_vec_delete`, `__builtin_vec_new`.
Some of these (the multimap helpers, `Remove`, the matrix copy) may be owned by other packages; they are left unresolved here.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values after the current tip (profile `authored_cpp`; no `host_check.cpp` is a plan
input; they include host headers). Body sizes are from the reviewed EE g++ `authored_cpp` profile, not from the original executable.
No `private_object_sha256` is claimed. Do not add the helper header names: none of the units includes a shared header.

```json
[
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "TextureConfiguration__C22VIParticleDefinitionEx"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_definition_texture_configuration.o",
  "profile": "authored_cpp",
  "sha256": "2753fe0ed2d3a1812c6be6cf1c868cf89cf1d57f165ea754766eda70f805ac25",
  "source": "sources/w15-vi-particle-b-definition-texture-configuration/definition_texture_configuration.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Density__C14VIEffectVolume"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_effectvolume_density.o",
  "profile": "authored_cpp",
  "sha256": "b7cf03d65e17f21c7dc52914ae69f5252e0ff50271c15468699c3af31ddf14c0",
  "source": "sources/w15-vi-particle-b-effectvolume-density/effectvolume_density.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Flags__C14VIEffectVolume"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_effectvolume_flags.o",
  "profile": "authored_cpp",
  "sha256": "9e8ca217dc1797f2f8a7bafab33a8e5569817a43e1c6ebfe0576ee24587da58a",
  "source": "sources/w15-vi-particle-b-effectvolume-flags/effectvolume_flags.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 16,
    "name": "Surface__C14VIEffectVolumei"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_effectvolume_surface.o",
  "profile": "authored_cpp",
  "sha256": "40e41ab808bdd9561cc14c4214bda543e5d59562466b52f38a4242c48699c150",
  "source": "sources/w15-vi-particle-b-effectvolume-surface/effectvolume_surface.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Type__C14VIEffectVolume"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_effectvolume_type.o",
  "profile": "authored_cpp",
  "sha256": "bca9091e7ca85bfe221087a2a6a7e41ab89232f9efdb77f3b88c39a83d53fb86",
  "source": "sources/w15-vi-particle-b-effectvolume-type/effectvolume_type.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Direction__C17VIParticleEmitter"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_emitter_direction.o",
  "profile": "authored_cpp",
  "sha256": "c26f2e4147bb4c1cd4eed2e1c2ffbc9743349f68b855dad29b5e9b180e61d8af",
  "source": "sources/w15-vi-particle-b-emitter-direction/emitter_direction.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Location__C17VIParticleEmitter"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_emitter_location.o",
  "profile": "authored_cpp",
  "sha256": "457010535214d6754e6fa924d6675319ecf67d1f3edc2e7cb3c97dbb7342f759",
  "source": "sources/w15-vi-particle-b-emitter-location/emitter_location.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Space__C17VIParticleEmitter"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_emitter_space.o",
  "profile": "authored_cpp",
  "sha256": "a7b38338f62c2212805272d582781a8354e14c0781befbda33a32abf8587370e",
  "source": "sources/w15-vi-particle-b-emitter-space/emitter_space.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 320,
    "name": "StaticBounds__C17VIParticleEmitter"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_emitter_static_bounds.o",
  "profile": "authored_cpp",
  "sha256": "8cb7055f02ef7e296c4146da9848d6b3627eb906928ec2f6e752afcc4ee9421e",
  "source": "sources/w15-vi-particle-b-emitter-static-bounds/static_bounds.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 380,
    "name": "Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_multimap_allocate_emitterentry.o",
  "profile": "authored_cpp",
  "sha256": "35c41b3c4b8ad1be3ac9c36ca6e93352b6b09b366880cb4630a158d7722daeb9",
  "source": "sources/w15-vi-particle-b-multimap-allocate-emitterentry/allocate.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 152,
    "name": "Clear__t10VIMultiMap2ZfZ14VIEmitterEntry"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_multimap_clear_emitterentry.o",
  "profile": "authored_cpp",
  "sha256": "6bb05e824a7784eec600f98c47a20e47b5f4f38a96cb69925adcf582c6b9512f",
  "source": "sources/w15-vi-particle-b-multimap-clear-emitterentry/clear.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 660,
    "name": "Erase__t10VIMultiMap2ZfZ14VIEmitterEntryi"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_multimap_erase_emitterentry.o",
  "profile": "authored_cpp",
  "sha256": "8615c5dc37da1ef939b1140b88f1212ae6bd5e7d1b0951e43fcc9ee3ec325418",
  "source": "sources/w15-vi-particle-b-multimap-erase-emitterentry/erase.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 1008,
    "name": "Insert__t10VIMultiMap2ZfZ14VIEmitterEntryRCfRC14VIEmitterEntry"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_multimap_insert_emitterentry.o",
  "profile": "authored_cpp",
  "sha256": "9183b8e2ba0f43b78805ef888a920c13f96fd45ce4dbd7ec0178010ea1dafffc",
  "source": "sources/w15-vi-particle-b-multimap-insert-emitterentry/insert.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 20,
    "name": "Size__Ct10VIMultiMap2ZfZ14VIEmitterEntry"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_multimap_size_emitterentry.o",
  "profile": "authored_cpp",
  "sha256": "efba02aacc217ebaedf22d84599634db589a7da21065f6ea5c19ff47b3c0c646",
  "source": "sources/w15-vi-particle-b-multimap-size-emitterentry/size.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 32,
    "name": "Begin__Ct6VIPool1ZP15VIParticleMotif"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_pool_begin_particlemotif.o",
  "profile": "authored_cpp",
  "sha256": "4a60a40825a2533c2ce50cbdcad0a92ea93a9c3d9cad3bf5f1cf51309f01631e",
  "source": "sources/w15-vi-particle-b-pool-begin-particlemotif/begin.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 88,
    "name": "DestroyPool__t6VIPool1ZP20VIParticleDefinition"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_pool_destroypool_particledefinition.o",
  "profile": "authored_cpp",
  "sha256": "3914f1cd220e5de8e12f9197c004bfa5e1fd7e7cfb7306bea7fbff7d8e1c4bdc",
  "source": "sources/w15-vi-particle-b-pool-destroypool-particledefinition/destroypool.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 12,
    "name": "End__Ct6VIPool1ZP15VIParticleMotif"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_pool_end_particlemotif.o",
  "profile": "authored_cpp",
  "sha256": "6f89b3794a078e52263e0897c1dbb6a15327efca188774dc5372d5fdaa21792b",
  "source": "sources/w15-vi-particle-b-pool-end-particlemotif/end.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 192,
    "name": "Erase__t6VIPool1ZP15VIParticleMotifi"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_pool_erase_particlemotif.o",
  "profile": "authored_cpp",
  "sha256": "e53b7a92bd253e92210538f8790d21e4134d33f1ea87960502d6995846a47905",
  "source": "sources/w15-vi-particle-b-pool-erase-particlemotif/erase.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 60,
    "name": "Next__Ct6VIPool1ZP15VIParticleMotifi"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_pool_next_particlemotif.o",
  "profile": "authored_cpp",
  "sha256": "5a27826788da89d97143a021f806e0424b189155e2212ff92cd952431c7dfa1b",
  "source": "sources/w15-vi-particle-b-pool-next-particlemotif/next.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "Size__Ct6VIPool1ZP15VIParticleMotif"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_pool_size_particlemotif.o",
  "profile": "authored_cpp",
  "sha256": "0fc5f24667339c43977f2fe239ae8dee4f3a4f1c117d2f3f51e383d8c18de64d",
  "source": "sources/w15-vi-particle-b-pool-size-particlemotif/size.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 8,
    "name": "GetParticleUISprite__7VIScene"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_scene_particle_ui_sprite.o",
  "profile": "authored_cpp",
  "sha256": "fca32670fc0f22ceea9bcc6b0408677c520627a09f72b95fb34e0ad777bbfc60",
  "source": "sources/w15-vi-particle-b-scene-particle-ui-sprite/scene_particle_ui_sprite.cpp"
 },
 {
  "emitted_functions": [
   {
    "compiled_body_bytes": 40,
    "name": "VITrailFxEnd__FP9VITrailFx"
   }
  ],
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_vi_particle_b_trailfx_end.o",
  "profile": "authored_cpp",
  "sha256": "0dfa78fe5e4975b371e5263e3d99a7e4c0578c1e7f08a584206fd160b489edbc",
  "source": "sources/w15-vi-particle-b-trailfx-end/trailfx_end.cpp"
 }
]
```

## Authored but never executed

All 22 functions: none has run on the EE or in a game session. They ran only (a) on the host in `host_check.cpp` and (b) inside the private
interpreter against the original with stand-in callees. Callee behaviour (`AddNode`, rotations, `EraseFixup`, `FreeNode`, `LinkFreeChain`,
the allocator, matrix copy and multiply, `Bound`, `Store`) was stubbed with Python models in those runs, so interactions with the real
helpers are untested. Residual uncovered original words: Insert 1 of 199 and Clear 5 of 29 (the path where `DestroyPool` empties the
array pointer was covered in a separate run; the rest are delay-slot filler), Allocate 1 of 94, Erase pool 2 of 51.

## Unknown

- Member names of every structure above, the element type behind `VIParticleMotif*` pool entries beyond the two link words, and the
  VIEmitterEntry layout beyond the key, two value words and the matrix; which flag value is which colour.
- Whether `DestroyPool` for the definition pool omits a destructor because the element is a pointer or because the template names none.
- The sizes of the surface array in `VIEffectVolume` and of the structures that contain the offsets read here.
