# w12-visprite-a: VI sprite classes, trail/splash effects, materials, raster and spell-effect system (A)

Branch `agents/w12-visprite-a`. One commit per function (the one data symbol is its own commit). Source is under
`experimental/sources/w12-visprite-a-*`; each directory holds the authored source and a `host_contract.cpp` that includes
host headers and stays out of the plan. Addresses and sizes are from the pinned private `SLUS_205.65`. Nothing here ran on
the EE or as gameplay; compiled sizes are the reviewed EE `authored_cpp` objects and are not claims of byte identity.

## How each function was checked

* Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags, and links in a private partial build
  (5,136 inputs: the shared tip plus these 23 units; `--check-plan` passed on the private plan).
* Every unit has a host contract (`c++ -std=c++17 -w host_contract.cpp && ./a.out`, return 0). The contracts stub the callees
  and check call order, arguments and stored bytes; the larger ones check exact output values.
* Every function was also compared with the image in a private paired run (a small R5900 interpreter outside git): the original
  function and the freshly compiled authored function run from identical memory snapshots with identical stubbed callees, and
  the call logs (arguments, plus memory read at call time where order matters), the return value and every changed byte outside
  the stack must be equal. Scenario counts and original-word coverage are in the commit messages (for instance
  `Render__9VITrailFx...` 184/184 words, `BeginBillboards...` 403/404, `DrawCenteredString...` 375/376; the missed words are
  annulled delay slots or a nop). Deliberately wrong variants (constants, offsets, branch senses) of every function were run
  and fail, except where a commit says a variant cannot be told apart (the sign of a zero product in `SetFog`).
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
* Private partial build with the 23 units added: 833 unresolved imports against 827 for the unchanged tip, i.e. all 23 scope
  names closed and 29 callee names newly visible (listed below). 3,933 function bodies, zero COMMON.
* The private interpreter needed one extension for `BeginBillboards`: the VU zeroing instruction `vsub.xyzw vf4,vf0,vf0` and the
  quadword store `sqc2`; it still halts on any other COP2 instruction.

## Units

| Function | Image address | Image bytes |
|---|---|---|
| `SetZWrite__10VIMateriali` | 0x010cf720 | 44 |
| `SetZTest__10VIMateriali` | 0x010cf750 | 44 |
| `SetDither__10VIMateriali` | 0x010cf780 | 36 |
| `SetLayerFillType__10VIMateriali18VIMaterialFillType` | 0x010cf7f0 | 92 |
| `SetLayerModulate__10VIMaterialii` | 0x010cf960 | 80 |
| `SetLayerTextureLODBias__10VIMaterialif` | 0x010cfb00 | 36 |
| `Init__10VIMateriali` | 0x010ce568 | 240 |
| `FirstPlayback__19VISpellEffectSystem` | 0x01003108 | 16 |
| `NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi` | 0x01003118 | 16 |
| `Playback__19VISpellEffectSystemi` | 0x010030c0 | 72 |
| `DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi` | 0x01003080 | 64 |
| `Stop__19VISpellEffectSystemi` | 0x01002e58 | 124 |
| `ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary` | 0x010025b8 | 204 |
| `Process__19VISpellEffectDriver` | 0x01182fc8 | 124 |
| `Clear__13VIHSpriteAnim` | 0x0105d008 | 116 |
| `ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary` | 0x010b8f70 | 124 |
| `Update__9VITrailFxl` | 0x01027550 | 196 |
| `Render__9VITrailFxR8VIRasterR8VICamera` | 0x01026c40 | 736 |
| `SetFog__8VIRasteriffRC10VIColor32F` | 0x011141e8 | 216 |
| `EndBillboards__8VIRaster` | 0x0110d1e0 | 224 |
| `BeginBillboards__8VIRasterPP11VIBillboardi` | 0x0110cb90 | 1616 |
| `DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc` | 0x0110f1b0 | 1504 |
| `VITrailFx$sActiveTrailFx` (data) | 0x011e51f8 | 16 (to next symbol) |

## Observed, inferred, unknown

The leading comment of each source file is the record. Short form:

* Observed: the material is a record with a flags word at +12 (bit 0 z-test, bit 1 z-write, bit 4 dither), a changed word at +32
  that most setters set to 1 (`SetDither` does not), and 144-byte layer records from +48 whose word +4 carries a modulate bit (bit 0)
  and a fill-type bit (bit 5), word +12 the blend mode, float +52 the texture LOD bias. `Init` writes the defaults, including the LOD
  bias bit pattern 0x3f4ccccc (stored by bit pattern because the reviewed EE compiler rounds the decimal literal one bit differently).
* Observed: the spell-effect system keeps a 24-entry table of playback records at +4 keyed by the low byte of an id (one record per
  slot, id compared as the full int at record +28), a list of live playbacks at +100 and a list of spare ones at +112, and a pool
  of effects at +1112 with 12-byte entries. `Stop` runs the driver destructor with flags 3, clears the table slot and moves the record
  to the spare list. A list whose first node has a zero first link is empty (the end marker is a node).
* Observed: `Update__9VITrailFxl` takes a 64-bit time (the image uses doubleword arithmetic; the mangled `l` is a 32-bit long in this
  profile, so the authored parameter is `long long`), converts only the low 32 bits of the difference and scales by 0.001. It keeps
  one expired segment and clears from the second expired segment onward.
* Observed: `Render__9VITrailFx...` draws two triangle strips over the segment list (opposite vertex order), restarting a strip every
  24 vertices on the last pair, and recomputes the segment alpha in both passes.
* Observed: `SetFog` writes two scratchpad words (0x700001f0, 0x700001f4) computed from near and far; `BeginBillboards` and `EndBillboards`
  build and close a DMA packet at the raster's write pointer (word at raster +19492); the constant table of 28 doublewords and the VIF-like
  codes are reproduced exactly but unexplained.
* Inferred: all names of fields are guesses at purpose and are not used in the source (offsets only). `DrawCenteredString` projects the
  position with two scratchpad matrices, centres the string and sends one fan of four vertices per glyph.
* Unknown: the meaning of the packet qualifier words and the billboard table; the unit of trail time; why `Identity__10VIMatrix44` is
  applied to both scratchpad matrices and not restored in `DrawCenteredString`; the true return type of `Stop__19VISpellEffectSystemi`
  (the image leaves v0 unspecified; the authored function returns nothing, while `Clear__19VISpellEffectSystem` declares it as `int`,
  which is harmless); the further parameters implied by the mangled names of `NextPlayback`/`DeletePlayback` (the callers pass two);
  the size of `VITrailFx$sActiveTrailFx` beyond the 12-byte list (16 is the span to the next symbol).

## Imports closed

BeginBillboards__8VIRasterPP11VIBillboardi, Clear__13VIHSpriteAnim, DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi, DrawCenteredString__8VIRasteriRC7VIVect3RC10VIColor32FPCc, EndBillboards__8VIRaster, FirstPlayback__19VISpellEffectSystem, Init__10VIMateriali, NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi, Playback__19VISpellEffectSystemi, Process__19VISpellEffectDriver, ReleaseAllParticleDefinitions__16VIParticleSystemR12VIDictionary, ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary, Render__9VITrailFxR8VIRasterR8VICamera, SetDither__10VIMateriali, SetFog__8VIRasteriffRC10VIColor32F, SetLayerFillType__10VIMateriali18VIMaterialFillType, SetLayerModulate__10VIMaterialii, SetLayerTextureLODBias__10VIMaterialif, SetZTest__10VIMateriali, SetZWrite__10VIMateriali, Stop__19VISpellEffectSystemi, Update__9VITrailFxl, VITrailFx$sActiveTrailFx.

## Imports opened

These are undefined in the private build and not defined by this package (callees owned elsewhere); 29 were not previously on the
list. `Identity__10VIMatrix33`, `Identity__10VIMatrix44`, `Material__8VIRasteri`, `Mul__10VIMatrix44RC10VIMatrix44T1`,
`Remove__7VILNode`, `AddTail__7VILListP7VILNode`, `SetMaterial__8VIRasteri`, `SetModelView__8VIRasterRC10VIMatrix44`, `Color__8VIRasterRC10VIColor32F`,
`BeginTriStrip__8VIRaster`, `EndTriStrip__8VIRaster`, `__builtin_vec_delete` and `VITrailFx$sMaterialHandle` were already on the list.

AddMaterial__8VIRasteriPP10_sceDmaTag, Begin__Ct6VIPool1ZP13VISpellEffect, Begin__Ct6VIPool1ZP20VIParticleDefinition, Cache__7VIRFontR8VIRasterPCciPi, CalcMatrices__8VIRaster, ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT, Clear__t6VIPool1ZP13VISpellEffect, Clear__t6VIPool1ZP20VIParticleDefinition, Destroy__13VISpellEffect, EndTriFan__8VIRaster, End__Ct6VIPool1ZP13VISpellEffect, End__Ct6VIPool1ZP20VIParticleDefinition, Erase__t6VIPool1ZP13VISpellEffecti, FlushDMABufferIfFull__8VIRaster, Length__C7VILList, Process__18VISpellEventDriver, ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary, Remove__12VIDictionaryR10VIResource, Restore2D__8VIRaster, SetFogRegisters__8VIRaster, Store2D__8VIRaster, UploadBillboardMicro__8VIRasterPP10_sceDmaTag, UploadMatrices__8VIRasterPP10_sceDmaTag, UploadRasterMicro__8VIRasterPP10_sceDmaTag, Vertex__8VIRasterRC7VIVect3, _$_13VISpellEffect, _$_18VISpellEventDriver, __ml__FRC7VIVect3RC10VIMatrix44, __ml__FRC7VIVect4RC10VIMatrix44.

`ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT` is called by `Update__9VITrailFxl`; the image defines it at 0x01027618 (100 bytes).
Nothing in the shared selection defines it, so it appears as a new import; it belongs to this package's area but was not in the scope list.

## Build-plan entries needed

All `authored_cpp`, empty header closure, `sha256` of the source at the last commit of this branch. `docs/agents/w12-visprite-a-plan-entries.py
--toolchain DIR [--private-root DIR]` regenerates them (with `emitted_functions`) and writes a private plan and snapshot copy.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_hspriteanim_clear.o", "profile": "authored_cpp", "sha256": "d7a427b174e7bdee75cea2a113ad0b4d50bf7fff37f99c5c0f8c425660843b7b", "source": "sources/w12-visprite-a-hspriteanim-clear/clear.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_init.o", "profile": "authored_cpp", "sha256": "9230970fa4a1683550caf9d5ece69f682fe42389fd194a00c7b6963eb241d7a0", "source": "sources/w12-visprite-a-material-init/init.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_set_dither.o", "profile": "authored_cpp", "sha256": "f1f505cb40e7ab501b5fe435484bbdc2e7cffe75999ace32cce334e96fe878e3", "source": "sources/w12-visprite-a-material-set-dither/set_dither.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_set_layer_fill_type.o", "profile": "authored_cpp", "sha256": "74eea721d0f753b8cb010e1ace804e828cafbef4497d11f4c84cbffbc2687eb8", "source": "sources/w12-visprite-a-material-set-layer-fill-type/set_layer_fill_type.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_set_layer_modulate.o", "profile": "authored_cpp", "sha256": "0c1d751e39bec2a88a7bd58b35a5f61bddf3df0146569e3be96b6cea0b50d9e4", "source": "sources/w12-visprite-a-material-set-layer-modulate/set_layer_modulate.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_set_layer_texture_lod_bias.o", "profile": "authored_cpp", "sha256": "4963b07b251ade9562cd010a61a2e3e25964db9545f189418ecbd8cb36250640", "source": "sources/w12-visprite-a-material-set-layer-texture-lod-bias/set_layer_texture_lod_bias.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_set_z_test.o", "profile": "authored_cpp", "sha256": "8de105632aa620f29adb0c95bb6c3b15e633e6ad555692e3bb4969d293bbf8e0", "source": "sources/w12-visprite-a-material-set-z-test/set_z_test.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_material_set_z_write.o", "profile": "authored_cpp", "sha256": "15c17966597e7eb6a59e2bec5ff068aac8d942b37a4c196a836b7ba51604c576", "source": "sources/w12-visprite-a-material-set-z-write/set_z_write.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_particlesystem_release_all_definitions.o", "profile": "authored_cpp", "sha256": "dfd6612db524e74619c62af3f868ccb60174ce6890b813bc849893dcad9e5276", "source": "sources/w12-visprite-a-particlesystem-release-all-definitions/release_all.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_raster_begin_billboards.o", "profile": "authored_cpp", "sha256": "e1ed24f81c145c134c6086eb32fd6dca6bde1a7c08a81e076ede673aa0e7af27", "source": "sources/w12-visprite-a-raster-begin-billboards/begin_billboards.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_raster_draw_centered_string.o", "profile": "authored_cpp", "sha256": "16fa77ed440e6752c0761fbe7b31665c45f28949d077c88e62c25124c591f3b7", "source": "sources/w12-visprite-a-raster-draw-centered-string/draw_centered_string.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_raster_end_billboards.o", "profile": "authored_cpp", "sha256": "b8b956ac9978c048e3d56100bacae3cc5e2405fd6af3d8798783fbc220d6307c", "source": "sources/w12-visprite-a-raster-end-billboards/end_billboards.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_raster_set_fog.o", "profile": "authored_cpp", "sha256": "6d3f08ef02b6d60baa97c2a534043fc8b674664b0018af30bc0371f0986031a7", "source": "sources/w12-visprite-a-raster-set-fog/set_fog.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_delete_playback.o", "profile": "authored_cpp", "sha256": "6ce960741037c1a07d6511a4bd15209be2878d48368bd937087046b349d32d8e", "source": "sources/w12-visprite-a-spell-effect-delete-playback/delete_playback.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_driver_process.o", "profile": "authored_cpp", "sha256": "9489f383259b6deed41e3e1c83f8d50bdbdfa8024bbe54ef77e6e882317da8b8", "source": "sources/w12-visprite-a-spell-effect-driver-process/process.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_first_playback.o", "profile": "authored_cpp", "sha256": "8f076e8dd63f85995ba9bca75d77e755741ec13febb9f8e32b19800b3039d65e", "source": "sources/w12-visprite-a-spell-effect-first-playback/first_playback.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_next_playback.o", "profile": "authored_cpp", "sha256": "3f2db4eace73373883c71d0191b93a6a2cff68a608d1c306af9a6d660cd4bbc4", "source": "sources/w12-visprite-a-spell-effect-next-playback/next_playback.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_playback.o", "profile": "authored_cpp", "sha256": "06f6916f458901bc95315db2b9055db75f3ced7daad213e28f5dcba4cb2e7410", "source": "sources/w12-visprite-a-spell-effect-playback/playback.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_release_all.o", "profile": "authored_cpp", "sha256": "90614a57f2cb30b33f001159d4e911c726b0d84cbe16b89a64bbca814daa67b3", "source": "sources/w12-visprite-a-spell-effect-release-all/release_all.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_spell_effect_stop.o", "profile": "authored_cpp", "sha256": "5a1f127a049493ff10b6dd6482ce24d79a5ed4410713666177671703af0abfc6", "source": "sources/w12-visprite-a-spell-effect-stop/stop.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_trailfx_active_list.o", "profile": "authored_cpp", "sha256": "1b4cac57e6bb1d4e99a974f7d126f01abcd18511bf498f7789c0aa02b1c5f287", "source": "sources/w12-visprite-a-trailfx-active-list/active_list.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_trailfx_render.o", "profile": "authored_cpp", "sha256": "e712dadd9ad9a1aba327af0990ea8763b9b35b92144eebf32b59edcc7394c4b0", "source": "sources/w12-visprite-a-trailfx-render/render.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w12va_trailfx_update.o", "profile": "authored_cpp", "sha256": "8d2965dc91b8a709a5fd729eaae82150f14c9cc85bb1cb5773f2ad57e9ee596b", "source": "sources/w12-visprite-a-trailfx-update/update.cpp"}
```

## Authored but never executed

Everything. No unit has run on the EE, an emulator or as gameplay. The paired runs execute the image's original function on a small
interpreter with stubbed callees, and the authored function compiled by the reviewed EE compiler on the same interpreter. The callees
(raster state, DMA flush, matrix products, list removal, pool iteration, font cache) are stubs in every check, so call-order and
memory effects are verified but the real callee behaviour is not. The scratchpad addresses used by `SetFog`, `BeginBillboards` and
`DrawCenteredString` were simulated as ordinary memory.

## Still unknown or not done

* `ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT` (above) and the callees listed under imports opened are not authored here.
* `VITrailFx$sFreeTrailFx` and `VITrailFx$sInactiveTrailFx` (the neighbouring list storage) are still unresolved imports of other scopes.
* The overlay placement of `VITrailFx$sActiveTrailFx` (it sits in the SRS overlay section in the image) is a linker-layout matter.
