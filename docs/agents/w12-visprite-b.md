# w12-visprite-b: VI sprite classes, trail effect and spell-effect support

Package branch `agents/w12-visprite-b`. New source is under `experimental/sources/w12-visprite-b-*`. Addresses and
sizes come from the pinned SLUS-20565 image (`work/private/SLUS_205.65`, read locally with the reviewed EE binutils; no
bytes, listings or decompiler output are committed). Every unit's header comment separates Observed, Inferred and
Unknown. Each function is its own commit and compiles with the plan's `authored_cpp` flags (reviewed EE g++). Compiled
bodies are not the original sizes and are not byte matches.

## Checks performed

- Every function: a host check (`host_check.cpp`, not a plan input) with recording stubs that compares the call order,
  this-relative arguments and every byte of the object against an expectation built from the observed stores.
- Every function: a private paired run on a R5900 interpreter (outside git) that executes the original from the image and
  the freshly compiled function from identical snapshots with identical stub callees, comparing the call log and every
  changed byte outside the stack. Mutations of constants, offsets, counts and branch conditions were each detected.
  `lightFloorSprite__FG6Point3` ran 120 random scenarios (lights, flags, scene bytes, light-map factors, world null and
  non-null); the others ran 3 to 6 random-memory scenarios each (destructors with flags 0, 1, 2, 3, 0x10001, 0xffffffff).
- Data words: width (`static_assert`) and initial value checked on the host against the image bytes.
- All 22 objects link together with `ld -m elf32lr5900 -r` without duplicate definitions, and the defined names equal the
  22 imports below. No full partial build was run (kept to single units as asked); the shared plan was not touched.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.

## Functions

| source | symbol | image | compiled bytes |
|---|---|---|---|
| `sources/w12-visprite-b-actor-dtor/dtor.cpp` | `_$_7VIActor` | `0x010659a8`, 84 bytes | 88 |
| `sources/w12-visprite-b-csprite-ctor/ctor.cpp` | `__9VICSprite` | `0x010ada18`, 772 bytes | 540 |
| `sources/w12-visprite-b-flora-sprite-ctor/ctor.cpp` | `__13VIFloraSprite` | `0x010061d8`, 68 bytes | 64 |
| `sources/w12-visprite-b-group-sprite-ctor/ctor.cpp` | `__13VIGroupSprite` | `0x0101dab8`, 116 bytes | 96 |
| `sources/w12-visprite-b-hsprite-ctor/ctor.cpp` | `__9VIHSprite` | `0x0105f7d8`, 532 bytes | 284 |
| `sources/w12-visprite-b-light-floor-sprite/light_floor_sprite.cpp` | `lightFloorSprite__FG6Point3` | `0x0013d700`, 624 bytes | 604 |
| `sources/w12-visprite-b-lod-sprite-ctor/ctor.cpp` | `__11VILODSprite` | `0x01076768`, 60 bytes | 56 |
| `sources/w12-visprite-b-particle-emitter-ctor/ctor.cpp` | `__17VIParticleEmitter` | `0x010b6010`, 440 bytes | 220 |
| `sources/w12-visprite-b-particle-emitter-dtor/dtor.cpp` | `_$_17VIParticleEmitter` | `0x010b8a30`, 140 bytes | 144 |
| `sources/w12-visprite-b-particle-sprite-ctor/ctor.cpp` | `__16VIParticleSprite` | `0x010aa660`, 76 bytes | 76 |
| `sources/w12-visprite-b-point-sprite-ctor/ctor.cpp` | `__13VIPointSprite` | `0x010cb7e8`, 60 bytes | 56 |
| `sources/w12-visprite-b-simple-sprite-ctor/ctor.cpp` | `__14VISimpleSprite` | `0x01166f40`, 72 bytes | 68 |
| `sources/w12-visprite-b-skin-sprite-ctor/ctor.cpp` | `__12VISkinSprite` | `0x0116d418`, 60 bytes | 56 |
| `sources/w12-visprite-b-sound-sprite-ctor/ctor.cpp` | `__13VISoundSprite` | `0x0117c1d8`, 100 bytes | 96 |
| `sources/w12-visprite-b-spell-effect-driver-dtor/dtor.cpp` | `_$_19VISpellEffectDriver` | `0x01182bb8`, 92 bytes | 96 |
| `sources/w12-visprite-b-stream-audio-sprite-ctor/ctor.cpp` | `__19VIStreamAudioSprite` | `0x01018eb8`, 72 bytes | 68 |
| `sources/w12-visprite-b-trail-fx-ctor/ctor.cpp` | `__9VITrailFx` | `0x01027240`, 56 bytes | 56 |
| `sources/w12-visprite-b-trail-fx-dtor/dtor.cpp` | `_$_9VITrailFx` | `0x01027278`, 108 bytes | 112 |

Data (each its own unit, with `host_check.cpp`): `VITrailFx$sFreeTrailFx` (0x011e51e8, 12 bytes, zero),
`VITrailFx$sInactiveTrailFx` (0x011e5208, 12 bytes, zero), `VITrailFx$sTrailFxPool` (0x011e5214, 4 bytes, zero),
`VITrailFx$sMaterialHandle` (0x0118e7d8, 4 bytes, -1). The list words are 12-byte list objects (inferred from the
destructor offsets +28 and +40 of the 56-byte trail element); the 4 bytes between the free and active lists are
alignment, not defined here.

## Observed, inferred, unknown

- Observed: every constructor of a sprite subclass calls `__8VISprite(this)`, stores a small integer at this+52 (LOD 7,
  skin 3, flora 13, point 2, simple 1, stream audio 12, sound 9, particle 10, group 11, H sprite 4, C sprite 5), stores
  its class table address at this+60, then sets class-specific fields; the C sprite constructor calls the H sprite
  constructor first. Stores and constants are listed in each unit.
- Inferred: the integer at +52 is a sprite-kind code; the template member constructors in the H and C sprite
  constructors are what their mangled names say. Not proved.
- Unknown: the meaning of every sprite field, the object sizes, what `Destroy__17VIParticleEmitter`,
  `Destroy__19VISpellEffectDriver` and `Clear__9VITrailFx` do, the layout of a trail element beyond +0 (node), +28 and
  +40 (lists) and +52, and which class owns the trail statics.
- A dead path is kept as observed in `lightFloorSprite__FG6Point3`: the light's flag byte is tested twice (skip when
  zero, then multiply by 0.25f when zero after the float stores); the second path cannot run unless the byte changes in
  between.

## Imports closed

`VITrailFx$sFreeTrailFx`, `VITrailFx$sInactiveTrailFx`, `VITrailFx$sMaterialHandle`, `VITrailFx$sTrailFxPool`,
`_$_17VIParticleEmitter`, `_$_19VISpellEffectDriver`, `_$_7VIActor`, `_$_9VITrailFx`, `__11VILODSprite`,
`__12VISkinSprite`, `__13VIFloraSprite`, `__13VIGroupSprite`, `__13VIPointSprite`, `__13VISoundSprite`,
`__14VISimpleSprite`, `__16VIParticleSprite`, `__17VIParticleEmitter`, `__19VIStreamAudioSprite`, `__9VICSprite`,
`__9VIHSprite`, `__9VITrailFx`, `lightFloorSprite__FG6Point3`. All 22 were in the 827-import list of the last shared
build.

## Imports opened

New (in no existing plan unit and not in the previous unresolved list): `__8VISprite`, `Clear__9VITrailFx`,
`Destroy__17VIParticleEmitter`, `Destroy__19VISpellEffectDriver`, `_$_10VIMatrix33`, `__20VIParticleAttributes`,
`_$_20VIParticleAttributes`, `__t6VIPool1Z13VIHSpritePlay`, `__t6VIPool1Z17VIHSpritePlayNode`,
`__t6VIPool1Z19VIHSpriteAttachment`, `__t7VIArray1Z16VIHSpriteTrigger`, `__t7VIArray1Z20VICSpritePartEmitter`,
`__t8VIVector1Z10VIMatrix44`, `__t8VIVector1Z13VIHSpriteNode`, and the class tables `_vt$11VILODSprite`,
`_vt$12VISkinSprite`, `_vt$13VIFloraSprite`, `_vt$13VIGroupSprite`, `_vt$13VIPointSprite`, `_vt$13VISoundSprite`,
`_vt$14VISimpleSprite`, `_vt$16VIParticleSprite`, `_vt$19VIStreamAudioSprite`, `_vt$9VICSprite`, `_vt$9VIHSprite`.
Already defined by other plan units and only referenced: `__7VILNode`, `__7VILList`, `_$_7VILList`, `__10VIMatrix33`,
`_$_10VIMatrix44`, `Identity__10VIMatrix33`, `Identity__10VIMatrix44`, `__builtin_delete`,
`findLightsAtPoint__FG6Point3PP5Lighti`, `worldQueryLightMap__FP12_worldHeaderG6Point3`, `scene`, `world`.

Not mine and still open next to these: `VITrailFx$sActiveTrailFx` (0x011e51f8, the active list between the free and
inactive lists; it was not in this package's list, so it was not defined here to avoid a duplicate),
`Update__9VITrailFxl`, `Render__9VITrailFxR8VIRasterR8VICamera`.

## Build-plan entries needed

Append these 22 entries (hash-checked at commit `HEAD` of this branch; regenerate the sha256 values if a source file
changes). All use profile `authored_cpp`; the `files` map of the plan needs the same source paths with the same hashes,
and the host checks stay out of the plan. Object names are unique.

```json
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_7VIActor"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_actor_dtor.o","profile":"authored_cpp","sha256":"a4ce48724ee0c25f976b24ea6f6b21b2f64f2c7ed32081cbd9266d35f3e077c4","source":"sources/w12-visprite-b-actor-dtor/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":540,"name":"__9VICSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_csprite_ctor.o","profile":"authored_cpp","sha256":"68f5380524f1a89e66fa91b4b192d54cc641976f8779319cdfe710d5a0bc4986","source":"sources/w12-visprite-b-csprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":64,"name":"__13VIFloraSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_flora_sprite_ctor.o","profile":"authored_cpp","sha256":"d4b6f8ad052a186d43085493c3c846d6a2dc53fa0cf5c107d4951c51c2518440","source":"sources/w12-visprite-b-flora-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"__13VIGroupSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_group_sprite_ctor.o","profile":"authored_cpp","sha256":"3d418f68947ab9ed7dadead75a8a03fb483682cd27370443c91487ce45dfef53","source":"sources/w12-visprite-b-group-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":284,"name":"__9VIHSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_hsprite_ctor.o","profile":"authored_cpp","sha256":"ca593df3d66728f8e9c158fc60f6cdd53b2b8875b5e729990d06c5dcc79124bb","source":"sources/w12-visprite-b-hsprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":604,"name":"lightFloorSprite__FG6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_light_floor_sprite.o","profile":"authored_cpp","sha256":"fcdc6ba138c96942266b32270035d0f8d729f1a1cd30906cf1d04975ad0fab93","source":"sources/w12-visprite-b-light-floor-sprite/light_floor_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"__11VILODSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_lod_sprite_ctor.o","profile":"authored_cpp","sha256":"e9d5abe2c4cfc8d9f932cc9c682996fc7ec7f73b131115813fb07b86730cc9fc","source":"sources/w12-visprite-b-lod-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":220,"name":"__17VIParticleEmitter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_particle_emitter_ctor.o","profile":"authored_cpp","sha256":"2ed4c7a610ab7ffb0a8283f72d2f97c9fefd8bc3c6931d28ab3f0f71db6aba7b","source":"sources/w12-visprite-b-particle-emitter-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":144,"name":"_$_17VIParticleEmitter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_particle_emitter_dtor.o","profile":"authored_cpp","sha256":"273410995bf3cc5afc908397b61f284f6fd005877f6ce4e2a227b7f7ef4d6523","source":"sources/w12-visprite-b-particle-emitter-dtor/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":76,"name":"__16VIParticleSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_particle_sprite_ctor.o","profile":"authored_cpp","sha256":"f66beb9206c86d2e2c92ba0dfab4f5825887002543016fdc4ccb96e4510086a5","source":"sources/w12-visprite-b-particle-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"__13VIPointSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_point_sprite_ctor.o","profile":"authored_cpp","sha256":"aa6b124f8337a76c53618adefb0388ffc144c52d18f563479c740626e5ee38fb","source":"sources/w12-visprite-b-point-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"__14VISimpleSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_simple_sprite_ctor.o","profile":"authored_cpp","sha256":"9d3fa7711111008f72ee2c4dcb7a5a31751c76527aebfe875496f3405f9541b7","source":"sources/w12-visprite-b-simple-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"__12VISkinSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_skin_sprite_ctor.o","profile":"authored_cpp","sha256":"bf97bcc19a38d8850e8f3246ff32f4e015fbf1681f4f55dbe8bc74d2ba08d442","source":"sources/w12-visprite-b-skin-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"__13VISoundSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_sound_sprite_ctor.o","profile":"authored_cpp","sha256":"f543dff0b6ac5d1d4e1aef236c171154df121f7e340592908e5d891d67f34204","source":"sources/w12-visprite-b-sound-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"_$_19VISpellEffectDriver"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_spell_effect_driver_dtor.o","profile":"authored_cpp","sha256":"0b6293e6c3fe42fdcc957d0f5406e024a09280b222e5947b24322163f63ed13e","source":"sources/w12-visprite-b-spell-effect-driver-dtor/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"__19VIStreamAudioSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_stream_audio_sprite_ctor.o","profile":"authored_cpp","sha256":"9deb687deb49e2c25abc5351761b497486469bc5613b95ed6bb78d55cb61f6b0","source":"sources/w12-visprite-b-stream-audio-sprite-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"__9VITrailFx"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_trail_fx_ctor.o","profile":"authored_cpp","sha256":"d5f6afc511b6c1ee8e953df61d319dbb71bd6c11dd6467759bf8493352439f89","source":"sources/w12-visprite-b-trail-fx-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_$_9VITrailFx"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_trail_fx_dtor.o","profile":"authored_cpp","sha256":"92e0c45568c772e84dab64e6d8d4bd10897de328fb8d6ef831ed7f4d9d65d313","source":"sources/w12-visprite-b-trail-fx-dtor/dtor.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_trail_fx_free_word.o","profile":"authored_cpp","sha256":"b35f0d4bd4f26c334a9ea2b6f9598b05a3d541153970ff125273cbdfdf6fd5e2","source":"sources/w12-visprite-b-trail-fx-free-word/free.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_trail_fx_inactive_word.o","profile":"authored_cpp","sha256":"3290267e3ca035369e290eb2c38a3a60a96e633cdba6626caafecedaab2072ad","source":"sources/w12-visprite-b-trail-fx-inactive-word/inactive.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_trail_fx_material_word.o","profile":"authored_cpp","sha256":"56cf9d3d21e39d5df5e3075492fe8471b588ca5d7848d0bf3ae18be607975850","source":"sources/w12-visprite-b-trail-fx-material-word/material.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"w12_visprite_b_trail_fx_pool_word.o","profile":"authored_cpp","sha256":"81fb213e06bb7414f61ffb489d3269b5a623fa99642b07a4a5efdf72ec2ed423","source":"sources/w12-visprite-b-trail-fx-pool-word/pool.cpp"}
```

## Authored but never executed

Nothing here has run on the EE or in the game. The functions ran only in a private interpreter against the original
image with stubbed callees, and on the host. The sprite constructors write their class table address, but dispatch
through those tables is not exercised (the tables are imports). The four data words are initial values only.

## Unknown

See above. Also unknown: whether the 66-record and 19-record tables in the C sprite constructor are capacities, and the
size of the trail, sprite, emitter and driver objects.
