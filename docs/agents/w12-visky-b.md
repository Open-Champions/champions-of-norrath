# VI scene, atmosphere, world, sound-track and battle-music methods (B) — w12-visky-b

Branch `agents/w12-visky-b`. Package scope: the 22 unresolved imports of the shared partial build (827 imports at
`c7243a6d`) that belong to the VI scene render loop, the time-of-day lighting and sky, the world profile table, the
battle-music object, the name-sprite and effect-volume sprites, the sound-track table, the sound device's stop call
and the splash-effect state. Private reference: `work/private/SLUS_205.65` of the main checkout (not committed). All
addresses below are that file's symbol values; the exact executable identity is the one the other w12 notes use and
is not re-established here. No game bytes, disassembly or decompiler output is committed: every file is
independently written C++ and prose.

## How each function was checked

- **Compile.** Each unit compiles with the reviewed EE g++ and the plan's `authored_cpp` flags, one unit at a time.
  A single private full partial build with a private plan copy then compiled all 5,130 inputs and linked once.
- **Host check.** Every function and data unit has a `host_check.cpp` (not an EE input) built with the host
  compiler (`-Wall -Wextra -Werror -ffp-contract=off`); `main` returns non-zero when a store, a call argument, the
  call order or a result is wrong. Host pointers are eight bytes wide while the EE's are four, so units that
  follow 32-bit pointer words read them through a small seam (`NORRATH_WORD_ADDRESS` or the existing
  `NORRATH_HOST_CONTRACT` translate function) that the host check defines.
- **Paired run against the original.** In a private R5900 interpreter (outside git) each authored function was compiled,
  linked with every undefined symbol bound to the original's address, and run next to the original function from
  identical memory snapshots with identical Python stand-ins for the callees. A scenario passes only when the
  call logs (callee, argument words, words behind pointer arguments, float arguments), the return value where one
  exists, and every changed byte outside the stack are identical. The interpreter's float model is double
  arithmetic rounded to single, which is exact for add, subtract and multiply; no scenario produced infinity or NaN.
  Mutated copies (a constant, an offset or a branch rule changed) were run and failed the harness.
- **Not byte matches.** Compiled sizes differ from the originals. These are behaviour reconstructions;
  nothing claims instruction equivalence or a percentage.

## Functions and data

| source | symbol | original address, bytes | compiled bytes | paired scenarios / original instructions executed |
|---|---|---|---|---|
| `w12-visky-b-battle-music-ctor/ctor.cpp` | `__13VIBattleMusic` | `0x0106de00`, 8 | 8 | 1 / 2 of 2 |
| `w12-visky-b-battle-music-dtor/dtor.cpp` | `_$_13VIBattleMusic` | `0x0106de08`, 84 | 76 | 6 / both flag paths run, cumulative coverage not recorded |
| `w12-visky-b-scene-sprite/sprite.cpp` | `Sprite__7VIScenei` | `0x01144ba8`, 28 | 28 | 15 / 7 of 7 |
| `w12-visky-b-name-defer-sort/sort.cpp` | `VINameDeferSort__FPCvT0` | `0x010a12a8`, 44 | 64 | 7 / less, equal and greater all run, cumulative coverage not recorded |
| `w12-visky-b-world-tree-ctor/ctor.cpp` | `__11VIWorldTree` | `0x01100190`, 52 | 52 | 2 / 13 of 13 |
| `w12-visky-b-world-tree-dtor/dtor.cpp` | `_$_11VIWorldTree` | `0x011001c8`, 108 | 112 | 6 / both flag paths run, cumulative coverage not recorded |
| `w12-visky-b-sound-track-set-tracks/set_tracks.cpp` | `SetTracks__17VISceneSoundTrack` | `0x0114e9f0`, 320 | 152 | 3 / straight-line code, run once per scenario |
| `w12-visky-b-name-sprite-ctor/ctor.cpp` | `__12VINameSprite` | `0x010a1000`, 80 | 60 | 3 / 20 of 20 |
| `w12-visky-b-effect-volume-sprite-ctor/ctor.cpp` | `__20VIEffectVolumeSprite` | `0x0114df60`, 124 | 88 | 3 / 31 of 31 |
| `w12-visky-b-sound-device-stop/stop.cpp` | `Stop__13VISoundDevicei` | `0x011252a0`, 260 | 356 | 386 / 64 of 65 |
| `w12-visky-b-world-set-profiles/set_profiles.cpp` | `SetProfiles__7VIWorld` | `0x010facf8`, 2020 | 272 | 3 / 505 of 505 |
| `w12-visky-b-scene-setup-time-of-day-sky/setup_sky.cpp` | `SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend` | `0x01141780`, 1780 | 884 | 51 / 445 of 445 |
| `w12-visky-b-scene-setup-time-of-day-lights/setup_lights.cpp` | `SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend` | `0x01140250`, 3056 | 1604 | 94 / 763 of 764 |
| `w12-visky-b-scene-update-sound-emitters/update_sound_emitters.cpp` | `UpdateSoundEmitters__7VISceneR8VICamera` | `0x0113d4a8`, 1500 | 1324 | 122 / 370 of 375 |
| `w12-visky-b-name-sprite-defers/name_defers.cpp` | `_12VINameSprite$NumNameDefers`, `_12VINameSprite$NameDefers` | `0x01190c40` (4), `0x01190c50` (3456) | data | storage only |
| `w12-visky-b-effect-volume-scene/scene_global.cpp` | `_14VIEffectVolume$mScene` | `0x011b1a98`, 4 | data | storage only |
| `w12-visky-b-splash-fx-state/splash_state.cpp` | `VISplashFx$sNextFlockDelay`, `$sLastAnimTime`, `$sGlowStep`, `$sGlowDir`, `$sNextBoltDelay` | `0x011e2800`, `0x011e2810`, `0x011e2818`, `0x011e281c`, `0x011e2820` | data | storage only |

In the paired runs the instructions never executed in the original were nops, branch-delay or annulled slots, or
the `break` of a divide-by-constant check that cannot fire (`Stop`, `UpdateSoundEmitters`).

## Observed, inferred, unknown (selected points; each source file carries its own)

- Observed: `SetProfiles` only stores constants: two floats at +88/+92 and twenty-two 40-byte rows from +96, whose
  colours are either four 1.0f or an 8-bit RGBA converted as byte times 0x3b808080 at run time; a hash over its 880 written bytes is in
  its host check. `SetTracks` stores twenty 32-bit words and two zeros. `Stop` splits a 64-bit channel mask
  into two words of the IOP function data when the sound's type word reads 1 (the image calls the type
  query twice). `SetupTimeOfDayLights` has an odd lightning term: the third colour channel misses the 0.9 factor the
  first two get. `SetupTimeOfDaySky` and `SetupTimeOfDayLights` read the same 24 hour records at scene+8180..8287 with 108-byte stride.
- Inferred: the profile and hour-record meanings given in the source comments (colour tint, ambient, sun, direction) come from how
  the numbers are combined; the names of fields are positional.
- Unknown: the names and meanings of most scene and world fields, of IOP function 9, of what writes the name-defer
  records, and the type of the sound-track words (hash-like, not established).

## Toolchain facts found on the way

- The reviewed compiler gives `unsigned long` four bytes, so 64-bit values are declared `unsigned long long`/`long long` (the image passes them in one register).
- Decimal float literals can land a unit in the last place off; constants whose image bits matter are written with
  `__builtin_bit_cast` (0.9f = 0x3f666666, 0x3b03126e, 0x3d4ccccc, 0x3b808080).
- `max.s`/`min.s` are written as inline assembly under `#if defined(__mips__)`.
- Helper functions that are not inlined show up in the compiled inventory (`lerp4`, `blend4` in the lights unit); they are local symbols.
- The earlier `UpdateSoundEmitters` host contract (w11 unit) crashed on hosts whose stack lies far from its arena
  because it truncated a stack address to an arena offset. The copy in this package writes through the pointer.

## Imports closed

SetProfiles__7VIWorld, SetTracks__17VISceneSoundTrack, SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend,
SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend, Sprite__7VIScenei, Stop__13VISoundDevicei,
UpdateSoundEmitters__7VISceneR8VICamera, VINameDeferSort__FPCvT0, VISplashFx$sGlowDir, VISplashFx$sGlowStep,
VISplashFx$sLastAnimTime, VISplashFx$sNextBoltDelay, VISplashFx$sNextFlockDelay, _$_11VIWorldTree,
_$_13VIBattleMusic, _12VINameSprite$NameDefers, _12VINameSprite$NumNameDefers, _14VIEffectVolume$mScene,
__11VIWorldTree, __12VINameSprite, __13VIBattleMusic, __20VIEffectVolumeSprite — all 22, confirmed by a private partial build
(827 imports before; 849 after, because these sources open the 44 names below).

## Imports opened

Referenced by these units and not defined by this package (the callees below belong to other packages or are not authored yet):

AttenuationMode__C13VISoundSprite, Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry, Clear__12VINameSprite,
DeallocateChannels__13VISoundDeviceUl, DeletePlayback__13VISoundDeviceP10VIPlayback, Distance__C7VIVect3RC7VIVect3,
EnableAllDirLights__8VIRasteri, EnableAmbient__8VIRasteri, EnablePointLight__8VIRasterii, EnableSunLight__8VIRasteri,
EnableTessellation__8VIRasteri, End__Ct10VIMultiMap2ZfZ14VIEmitterEntry, GetLightning__12VIAtmospherePf,
GetPriority__C19VIStreamAudioSprite, GetRelativePathname__C19VIStreamAudioSprite, Init__14VIEmitterQueryi12VISpriteType,
IsIopStreamingEnabled__6VIFile, IsLooping__C19VIStreamAudioSprite, IsPanEnabled__C13VISoundSprite,
MaxPointLights__8VIRaster, Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi, PlayBgm__13VISoundDeviceiPCc,
Play__13VISoundDeviceii12VISOUNDCLASS, Playback__13VISoundDevicei,
QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery, QueryEmitters__7VIWorldR23VISceneEmitterQueryVars,
SetAmbient__8VIRasterRC10VIColor32F, SetPan__13VISoundDeviceif,
SetPointLight__8VIRasteriRC10VIMatrix44RC18VIRasterPointLight, SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32,
SetSunLight__8VIRasterRC10VIMatrix44RC16VIRasterDirLight, SetVolume__13VISoundDeviceif, StopBgm__13VISoundDevicei,
StopChannels__13VISoundDeviceUl, _$_t10VIMultiMap2ZfZ14VIEmitterEntry, _$_t7VIArray1Z11VIWorldLeaf,
_$_t7VIArray1Z11VIWorldNode, __14VIEffectVolume, __8VISprite, __t10VIMultiMap2ZfZ14VIEmitterEntry,
__t7VIArray1Z11VIWorldLeaf, __t7VIArray1Z11VIWorldNode, _vt$12VINameSprite, _vt$20VIEffectVolumeSprite.

Also referenced but not opened as new imports (already defined elsewhere in the tree): `Clear__11VIWorldTree`
(opened earlier by other units), `sIopFunctionData` (declared as `norrath_overlay_sIopFunctionData_11edf08`, the name
the w11 `SetIopFunction` unit uses), `SetIopFunction__FUii`, `Type__C7VISound`, `GetOvercastInterp__12VIAtmosphere`,
`SetLavastormIntensity__12VIAtmospheref`, `Free__13VIBattleMusic`, `Normalize__7VIVect3`, `__floatdisf`.

## Build-plan entries needed

Add these 17 entries to `experimental/build-plan.json` (one object per line; `header_closure` is empty; sha256 is of the
committed source; the integrator assigns `input_order` and snapshot rows). The script
`docs/agents/w12-visky-b-plan-entries.py --toolchain <ee>` regenerates them and can write a private plan copy. The
earlier `w11-vi-render-c-scene-update-sound-emitters` unit is not meant to be planned alongside
`w12-visky-b-scene-update-sound-emitters` (both define `UpdateSoundEmitters__7VISceneR8VICamera`).

```json
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "__13VIBattleMusic"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_battle_music_ctor.o", "profile": "authored_cpp", "sha256": "60a6d0e22de41340675853b511231bd6bcf1fb4515145e5d945d6b6ab5c436e6", "source": "sources/w12-visky-b-battle-music-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "_$_13VIBattleMusic"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_battle_music_dtor.o", "profile": "authored_cpp", "sha256": "2d9e07bc3a8de7a3340ed5420afb98253ce4231f808ff767fad104c749d3fde3", "source": "sources/w12-visky-b-battle-music-dtor/dtor.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_effect_volume_scene.o", "profile": "authored_cpp", "sha256": "242f96711462b75f7337bed94e9217511ccf67b7620725be38355ca5b5a24d31", "source": "sources/w12-visky-b-effect-volume-scene/scene_global.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "__20VIEffectVolumeSprite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_effect_volume_sprite_ctor.o", "profile": "authored_cpp", "sha256": "2415468ee05c51208d622011647b6dd2a7c3ac047eeb2dfb02f13e04126cfb19", "source": "sources/w12-visky-b-effect-volume-sprite-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "VINameDeferSort__FPCvT0"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_name_defer_sort.o", "profile": "authored_cpp", "sha256": "545289b1ce4bff9ccf5a0a5eb6f53a8f11016355aa2ce3034d15453c3736cc66", "source": "sources/w12-visky-b-name-defer-sort/sort.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 60, "name": "__12VINameSprite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_name_sprite_ctor.o", "profile": "authored_cpp", "sha256": "ab542b780935273f674c5d300f88ba4e4938d799ba7cb02919223c111bc6bdd9", "source": "sources/w12-visky-b-name-sprite-ctor/ctor.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_name_sprite_defers.o", "profile": "authored_cpp", "sha256": "cfb52ad41b065b67a74bd2f87d669a3dc9ea4cca1f7985102d98447f46309fb2", "source": "sources/w12-visky-b-name-sprite-defers/name_defers.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 140, "name": "lerp4"}, {"compiled_body_bytes": 140, "name": "blend4"}, {"compiled_body_bytes": 1604, "name": "SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_scene_setup_time_of_day_lights.o", "profile": "authored_cpp", "sha256": "ef4d090ec9ef36c46dc8987a73f6687bf037fe633ec2642484449fc3858a7dce", "source": "sources/w12-visky-b-scene-setup-time-of-day-lights/setup_lights.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 884, "name": "SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_scene_setup_time_of_day_sky.o", "profile": "authored_cpp", "sha256": "842dba7a97a382c3ea37377e489f42db4258904e42a5a4c7af47a0a29f4f31ac", "source": "sources/w12-visky-b-scene-setup-time-of-day-sky/setup_sky.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Sprite__7VIScenei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_scene_sprite.o", "profile": "authored_cpp", "sha256": "e2a4767fb4957b6c6312af8873dbf15e7f858a721d95fbe6ddda7b636564bcfe", "source": "sources/w12-visky-b-scene-sprite/sprite.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1324, "name": "UpdateSoundEmitters__7VISceneR8VICamera"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_scene_update_sound_emitters.o", "profile": "authored_cpp", "sha256": "36250e913040dd1aa9beb052237db5262790fc5780700b1dc440f64ed5a1f7bd", "source": "sources/w12-visky-b-scene-update-sound-emitters/update_sound_emitters.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 356, "name": "Stop__13VISoundDevicei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_sound_device_stop.o", "profile": "authored_cpp", "sha256": "0d24343259243412ac9d2f08e8b6d37ed672cfc04f9022bb4901566c971ae830", "source": "sources/w12-visky-b-sound-device-stop/stop.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "SetTracks__17VISceneSoundTrack"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_sound_track_set_tracks.o", "profile": "authored_cpp", "sha256": "61f460572344b254719aaec172e2c33570f45124345d40a8bf86ecb96cee5364", "source": "sources/w12-visky-b-sound-track-set-tracks/set_tracks.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_splash_fx_state.o", "profile": "authored_cpp", "sha256": "67aee3b45e056b75127d94b8716e02f152711708e8bc47a43a16c4f00aabab85", "source": "sources/w12-visky-b-splash-fx-state/splash_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 272, "name": "SetProfiles__7VIWorld"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_world_set_profiles.o", "profile": "authored_cpp", "sha256": "d933ffa05fcc71d80f577d5eb0d71d54b92644fb32fad430f53b85672d6b6bf6", "source": "sources/w12-visky-b-world-set-profiles/set_profiles.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "__11VIWorldTree"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_world_tree_ctor.o", "profile": "authored_cpp", "sha256": "191a8fd469fff541b02e63ced0c5f7d156b45b79cb66d14b37fafb4ac51665a6", "source": "sources/w12-visky-b-world-tree-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_11VIWorldTree"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12vskb_world_tree_dtor.o", "profile": "authored_cpp", "sha256": "e6ddaf3129761bc4b066dae00b5434b0aae3d561bdd9ff64c7f9e6fd47819f62", "source": "sources/w12-visky-b-world-tree-dtor/dtor.cpp"}
```

## Authored but never executed

All of the above. Every function ran against the original only in the private interpreter with stand-in callees,
and every host check ran with stubs; none ran on PS2 hardware or a full emulator, and the real callers (the VI main
loop, scene setup, time-of-day update, world rendering) have not been run with these bodies. The callees' own behaviour (raster
light calls, multimap and pool templates, atmosphere, sound device) is unchecked here. The data units are
zero-initialised storage only; who writes them was not exercised.

## Unknown

- What the twenty `SetTracks` words identify, what the twenty-two `SetProfiles` rows are called, and what their words and floats mean.
- The sizes of `VIBattleMusic` and of the name-sprite and effect-volume sprite objects beyond the offsets used; the true capacity bound of the name-defer array (24 records is its extent).
- What lies between the `VISplashFx` words at 0x011e2800 and 0x011e2820.
- Whether `Type__C7VISound` is queried twice in `Stop` for a reason (side effect) or by accident of the original build.
- Tool tests: `python3 -m unittest discover -s tools -p 'test_*.py'` passed on this branch (exit 0).
