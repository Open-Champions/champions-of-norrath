# w11-vi-render-c: VI renderer, scene and world (3 of 3)

Branch `agents/w11-vi-render-c`. Every function below is one commit ("Reconstruct ... and check it against the
original"); the two data symbols are their own commits. Each source directory under
`experimental/sources/w11-vi-render-c-*` holds the authored source and, where there is one, a host contract
that passes with the host compiler.

## Method

Each unit compiles with the reviewed `authored_cpp` flags. The check beyond the host contract was a private paired
run on an R5900 interpreter (outside git): the original function and the authored one run from identical memory
snapshots with identical logged stub callees; call logs (arguments, floats included), return values and every
changed byte outside the stack must be equal, and instruction coverage of the original is recorded. Mutants
of constants, offsets and branch rules were run against the harness and reported. Nothing was run on PS2
hardware or an emulator.

## Observed (extents from the verified image)

| symbol | address | bytes |
|---|---|---|
| `CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend` | 0x010fb6d8 | 568 |
| `Clear__17VISceneSoundTrack` | 0x0114e8d8 | 140 |
| `Clear__19VISpellEffectSystem` | 0x01002bc0 | 136 |
| `Clear__7VIWorld` | 0x010fe478 | 96 |
| `CreateSprite__7VIScene12VISpriteTypei` | 0x0113ca98 | 688 |
| `GetEnvFlags__7VISceneRC7VIVect3` | 0x011451c8 | 112 |
| `InitTimeOfDay__7VIScene` | 0x0113f388 | 3780 |
| `Init__12VIWndOptionsP7VIScene` | 0x010c62c8 | 152 |
| `Init__17VISceneSoundTrack` | 0x0114e9b0 | 64 |
| `Init__19VISpellEffectSystemP7VIScene` | 0x01002af8 | 200 |
| `Init__7VIWorldiiiifRC7VIVect3RC6VIBBox` | 0x010faad8 | 540 |
| `Process__19VISpellEffectSystem` | 0x01002898 | 360 |
| `RenderByFrustum__7VIWorldR15VISceneRendVars` | 0x010fb910 | 188 |
| `RenderByPortals__7VIWorldR15VISceneRendVars` | 0x010fb9d0 | 376 |
| `RenderDeferredList__7VISceneR8VICameraii` | 0x01142e80 | 692 |
| `RenderShadows__7VIScene` | 0x01144ac0 | 108 |
| `SetPersonalLightMode__7VIScene19VIPersonalLightMode` | 0x011450e8 | 12 |
| `SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F` | 0x011450f8 | 72 |
| `SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi` | 0x01140e40 | 2088 |
| `SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi` | 0x01145948 | 92 |
| `UpdateSoundEmitters__7VISceneR8VICamera` | 0x0113d4a8 | 1500 |
| `VISplashFxReset__Fv` | 0x01006f98 | 92 |
| `VITrailFxClear__FR8VIRasterR12VIDictionary` | 0x01026fd8 | 184 |
| `VITrailFxInit__FR8VIRasterR12VIDictionary` | 0x01026988 | 376 |
| `VITrailFxRender__FlR8VIRasterR8VICamera` | 0x01027148 | 248 |
| `_$_19VISpellEffectSystem` | 0x01002a00 | 116 |
| `_$_7VIWorld` | 0x010fe400 | 116 |
| `_8VIRaster$DBuffer` | 0x01227f80 | 816 |
| `__19VISpellEffectSystem` | 0x01002a78 | 124 |
| `__7VIWorld` | 0x010fa9b8 | 284 |
| `norrath_world_c_view_4861e8` | ? | ? |

`norrath_world_c_view_4861e8` is the unnamed word at 0x004861e8 (defined in `w11-vi-render-c-world-card-count`).

## Late fixes in this session

- `SetupTimeOfDayFog`: the EE compiler truncates decimal float literals, so nine-digit literals landed one ulp
  low. Short decimals (0.4, 0.18, 0.56, 0.313) and constant expressions (1/255, 50/255, 65/255) reproduce the
  image bits; the last scale 0x3f7fffff needs the literal 0.99999995. The 70 * (1/255) product stays a run-time
  multiply; its register barrier is guarded with `#ifdef __mips__` so the host contract compiles.
- `InitTimeOfDay`: the image widens a float to double (`fptodp`), compares, negates (`dpsub`) and narrows
  (`dptofp`) to take an absolute value. Doing it in single precision gives identical bits, so the source avoids
  the soft-float helper imports. The byte colour tables (48 entries) were read from the original's output
  memory. The factor for the fourth channel (alpha, always zero) is not observable.

## Imports closed

All names in the package scope: GetEnvFlags__7VISceneRC7VIVect3, InitTimeOfDay__7VIScene, RenderShadows__7VIScene, CreateSprite__7VIScene12VISpriteTypei, RenderDeferredList__7VISceneR8VICameraii, SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi, UpdateSoundEmitters__7VISceneR8VICamera, SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F, SetPersonalLightMode__7VIScene19VIPersonalLightMode, SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi, Clear__17VISceneSoundTrack, Init__17VISceneSoundTrack, Clear__19VISpellEffectSystem, __19VISpellEffectSystem, _$_19VISpellEffectSystem, Init__19VISpellEffectSystemP7VIScene, Process__19VISpellEffectSystem, VISplashFxReset__Fv, VITrailFxClear__FR8VIRasterR12VIDictionary, VITrailFxInit__FR8VIRasterR12VIDictionary, VITrailFxRender__FlR8VIRasterR8VICamera, Init__12VIWndOptionsP7VIScene, CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend, Clear__7VIWorld, __7VIWorld, _$_7VIWorld, Init__7VIWorldiiiifRC7VIVect3RC6VIBBox, RenderByFrustum__7VIWorldR15VISceneRendVars, RenderByPortals__7VIWorldR15VISceneRendVars
plus `_8VIRaster$DBuffer` and `norrath_world_c_view_4861e8` (data).

## Imports opened

These are referenced by the authored units and not defined by this package (some are owned by other
packages and will close on merge):

`AddChild__5VIWndP5VIWnd AddTail__7VILListP7VILNode Add__t6VIPool1ZP8VISpriteRCP8VISprite AttenuationMode__C13VISoundSprite Begin__Ct10VIMultiMap2ZfZ14VIEmitterEntry Begin__Ct6VIPool1ZP8VISprite CalcTimeOfDayClipPlanes__7VISceneR21VITerrainProfileBlendRfT2 Clear__11VIWorldTree Clear__t7VIArray1Z13VIWorldRegion Clear__t7VIArray1Z6VIZone CreateMaterial__8VIRasteri DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi Distance__C7VIVect3RC7VIVect3 End__Ct10VIMultiMap2ZfZ14VIEmitterEntry End__Ct6VIPool1ZP8VISprite FindLeaf__C7VIWorldRC7VIVect3RiT2 FirstPlayback__19VISpellEffectSystem GetOvercastInterp__12VIAtmosphere GetPriority__C19VIStreamAudioSprite GetRelativePathname__C19VIStreamAudioSprite GetTime__7VIScene GetTime__7VITimer GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane Init__10VIMateriali Init__11VIWorldTreeii Init__12VIWndMessage Init__13VIBattleMusicP7VIScene Init__14VIEmitterQueryi12VISpriteType Init__6VIZone Init__7VILList Init__t7VIArray1Z13VIWorldRegioni Init__t7VIArray1Z6VIZonei IsIopStreamingEnabled__6VIFile IsLooping__C19VIStreamAudioSprite IsPanEnabled__C13VISoundSprite Material__8VIRasteri Mul__10VIMatrix44RC10VIMatrix44T1 NextPlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi Next__Ct10VIMultiMap2ZfZ14VIEmitterEntryi Next__Ct6VIPool1ZP8VISpritei Normalize__7VIVect3 PlayBgm__13VISoundDeviceiPCc Play__13VISoundDeviceii12VISOUNDCLASS Playback__19VISpellEffectSystemi Pop__11VIClipStack Process__19VISpellEffectDriver PushFrustum__11VIClipStackR8VICamera QueryEmittersAtLocation__7VISceneRC8VICameraR14VIEmitterQuery ReleaseAllSpellEffects__19VISpellEffectSystemR12VIDictionary ReleaseMaterial__8VIRasteriR12VIDictionary RemoveChild__5VIWndP5VIWnd Remove__7VILNode RenderByVolume__7VIWorldR15VISceneRendVars RenderRoom__6VIZoneiR15VISceneRendVars RenderShadow__7VIScenei Render__9VITrailFxR8VIRasterR8VICamera SetDither__10VIMateriali SetFog__8VIRasteriffRC10VIColor32F SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode SetLayerFillType__10VIMateriali18VIMaterialFillType SetLayerModulate__10VIMaterialii SetModelView__8VIRasterRC10VIMatrix44 SetPan__13VISoundDeviceif SetPretranslations__6VIZoneR8VIRasterR8VICamera SetProfiles__7VIWorld SetStaticLighting__8VIRasterRC10VIColor32F SetTracks__17VISceneSoundTrack SetVolume__13VISoundDeviceif SetZTest__10VIMateriali SetZWrite__10VIMateriali SetupTimeOfDayLights__7VISceneR8VICameraR21VITerrainProfileBlend SetupTimeOfDaySky__7VISceneR21VITerrainProfileBlend StopBgm__13VISoundDevicei Stop__13VISoundDevicei Stop__19VISpellEffectSystemi Update__9VITrailFxl VISplashFx$sGlowDir VISplashFx$sGlowStep VISplashFx$sLastAnimTime VISplashFx$sNextBoltDelay VISplashFx$sNextFlockDelay VITrailFx$sActiveTrailFx VITrailFx$sFreeTrailFx VITrailFx$sInactiveTrailFx VITrailFx$sMaterialHandle VITrailFx$sTrailFxPool VIWndOptions$sBattleMusicText _$_10VIMatrix44 _$_11VIWorldTree _$_19VISpellEffectDriver _$_7VILList _$_9VITrailFx _$_t10VIMultiMap2ZfZ14VIEmitterEntry _$_t6VIPool1ZP13VISpellEffect _$_t7VIArray1Z13VIWorldRegion _$_t7VIArray1Z6VIZone __11VILODSprite __11VIWorldTree __12VINameSprite __12VIPointLight __12VISkinSprite __13VIFloraSprite __13VIGroupSprite __13VIPointSprite __13VISoundSprite __14VISimpleSprite __16VIParticleSprite __19VIStreamAudioSprite __20VIEffectVolumeSprite __7VILList __7VILNode __9VICSprite __9VIHSprite __9VITrailFx __builtin_delete __builtin_new __builtin_vec_delete __builtin_vec_new __t10VIMultiMap2ZfZ14VIEmitterEntry __t6VIPool1ZP13VISpellEffect __t7VIArray1Z13VIWorldRegion __t7VIArray1Z6VIZone cosf rand sceVu0CopyMatrix sinf strcat`

## Build-plan entries needed

Add these entries to `experimental/build-plan.json` (one object per line; `header_closure` is empty; sha256 is of the
committed source). The two data units (`raster-dbuffer`, `world-card-count`) are included.

```json
{"emitted_functions":[{"compiled_body_bytes":104,"name":"GetEnvFlags__7VISceneRC7VIVect3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_env_flags.o","profile":"authored_cpp","sha256":"816c32e96c75500837ab9e850b4296d3ec4853bfc6601ef5367a750fedb9d713","source":"sources/w11-vi-render-c-get-env-flags/get_env_flags.cpp"}
{"emitted_functions":[{"compiled_body_bytes":668,"name":"InitTimeOfDay__7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_time_of_day.o","profile":"authored_cpp","sha256":"2d61d2948408e7955fed48a6bd62aa6a9152192bc58edcb054509c4c568508f4","source":"sources/w11-vi-render-c-init-time-of-day/init_time_of_day.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"dbuffer.o","profile":"authored_cpp","sha256":"38ea2bc6d1ed7b13f1005d7af2da5b97d93784cf8e2fe700e544c12f92839acd","source":"sources/w11-vi-render-c-raster-dbuffer/dbuffer.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"RenderShadows__7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"render_shadows.o","profile":"authored_cpp","sha256":"e1369c52c9d91587f58349e76c28e5a5da447732ac89b2293d5fd9fb794d4e47","source":"sources/w11-vi-render-c-render-shadows/render_shadows.cpp"}
{"emitted_functions":[{"compiled_body_bytes":692,"name":"CreateSprite__7VIScene12VISpriteTypei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"create_sprite.o","profile":"authored_cpp","sha256":"e326dd56edc8d80092a27359d5e00b09e52728a22c7a9599aa1112d0924ffb99","source":"sources/w11-vi-render-c-scene-create-sprite/create_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":612,"name":"RenderDeferredList__7VISceneR8VICameraii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"render_deferred_list.o","profile":"authored_cpp","sha256":"dcf07b95c074c16b495a8a80efa1365de01ce683347f70cedb2b3d851aff2dc7","source":"sources/w11-vi-render-c-scene-render-deferred-list/render_deferred_list.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1712,"name":"SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"setup_time_of_day_fog.o","profile":"authored_cpp","sha256":"81e851a308614fdbd7b4995ca534836be9a3677ad54e5c3f488e793c9cd3f6eb","source":"sources/w11-vi-render-c-scene-setup-time-of-day-fog/setup_time_of_day_fog.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1324,"name":"UpdateSoundEmitters__7VISceneR8VICamera"}],"header_closure":[],"kind":"independently_authored_source","object_name":"update_sound_emitters.o","profile":"authored_cpp","sha256":"84f115f4e9dfebf99f30cad14a64d9e6c434842d64e93d63dfa77cb9b27f0cae","source":"sources/w11-vi-render-c-scene-update-sound-emitters/update_sound_emitters.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_personal_light.o","profile":"authored_cpp","sha256":"833ae745826abe097e6850beb9c0c16471f8016670d8e6a2ee7d3ae6cfffe408","source":"sources/w11-vi-render-c-set-personal-light/set_personal_light.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"SetPersonalLightMode__7VIScene19VIPersonalLightMode"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_personal_light_mode.o","profile":"authored_cpp","sha256":"df2eb1acb23e8de1746c64f320f225f43998d6fd8ad606e13f774112c95d11e6","source":"sources/w11-vi-render-c-set-personal-light-mode/set_personal_light_mode.cpp"}
{"emitted_functions":[{"compiled_body_bytes":84,"name":"SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"setup_time_of_day.o","profile":"authored_cpp","sha256":"af10d6871b28046748ca6d12ad2b841ba7aa82de90dc5158533cab0f278cb018","source":"sources/w11-vi-render-c-setup-time-of-day/setup_time_of_day.cpp"}
{"emitted_functions":[{"compiled_body_bytes":108,"name":"Clear__17VISceneSoundTrack"}],"header_closure":[],"kind":"independently_authored_source","object_name":"sound_track_clear.o","profile":"authored_cpp","sha256":"b1f95df4d21b1a524bc4dfda0bcdef5caea9a35a5fd48e0e6d33dcbfe2f2efdf","source":"sources/w11-vi-render-c-sound-track-clear/sound_track_clear.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"Init__17VISceneSoundTrack"}],"header_closure":[],"kind":"independently_authored_source","object_name":"sound_track_init.o","profile":"authored_cpp","sha256":"996959147f8ca51a63a903dd561bdfb20103e8977780eb969b81c3b2f2cb16f9","source":"sources/w11-vi-render-c-sound-track-init/sound_track_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":156,"name":"Clear__19VISpellEffectSystem"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clear.o","profile":"authored_cpp","sha256":"770ba34ca1197eb3acf9dbcca6f31457c6b240e7afd7a1da62d92b55a75cec59","source":"sources/w11-vi-render-c-spell-effect-system-clear/clear.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"__19VISpellEffectSystem"}],"header_closure":[],"kind":"independently_authored_source","object_name":"ctor.o","profile":"authored_cpp","sha256":"3a6eca62d4e8b7ac797ed40bceca59313d41d62b38da72fe7e721281403c6070","source":"sources/w11-vi-render-c-spell-effect-system-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_$_19VISpellEffectSystem"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"a1817e1357b761a04b0d2bc9aa457a2aa9e3147ebdd1998df8c90bd45e6f8c94","source":"sources/w11-vi-render-c-spell-effect-system-dtor/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":176,"name":"Init__19VISpellEffectSystemP7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init.o","profile":"authored_cpp","sha256":"78f0088370783a442237133e853b10649628d3d9dbced4519aac32e8744d928d","source":"sources/w11-vi-render-c-spell-effect-system-init/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":312,"name":"Process__19VISpellEffectSystem"}],"header_closure":[],"kind":"independently_authored_source","object_name":"process.o","profile":"authored_cpp","sha256":"dcb34c63b7d298535fee85ef0c1eef7382daf03058c963d6e38df7d06c0d6a17","source":"sources/w11-vi-render-c-spell-effect-system-process/process.cpp"}
{"emitted_functions":[{"compiled_body_bytes":84,"name":"VISplashFxReset__Fv"}],"header_closure":[],"kind":"independently_authored_source","object_name":"splash_fx_reset.o","profile":"authored_cpp","sha256":"c6e88ba6f59c1f20e1dcb9d8945dad9bac889b245688439c0f9bd55eb7ad6b90","source":"sources/w11-vi-render-c-splash-fx-reset/splash_fx_reset.cpp"}
{"emitted_functions":[{"compiled_body_bytes":188,"name":"VITrailFxClear__FR8VIRasterR12VIDictionary"}],"header_closure":[],"kind":"independently_authored_source","object_name":"trail_fx_clear.o","profile":"authored_cpp","sha256":"ce4064c90aece7cfb2d666427f869404f8f681e572e0ea974e3ecf117c785702","source":"sources/w11-vi-render-c-trail-fx-clear/trail_fx_clear.cpp"}
{"emitted_functions":[{"compiled_body_bytes":352,"name":"VITrailFxInit__FR8VIRasterR12VIDictionary"}],"header_closure":[],"kind":"independently_authored_source","object_name":"trail_fx_init.o","profile":"authored_cpp","sha256":"5a6af633dc69ba939f5874748a6f76336706c36bf183149ee6bd690fc3d3bfce","source":"sources/w11-vi-render-c-trail-fx-init/trail_fx_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":276,"name":"VITrailFxRender__FlR8VIRasterR8VICamera"}],"header_closure":[],"kind":"independently_authored_source","object_name":"trail_fx_render.o","profile":"authored_cpp","sha256":"37a8bc28c1621f348e9b5bde6515206e55c36f48c6b67a6c2ce868efb03f7429","source":"sources/w11-vi-render-c-trail-fx-render/trail_fx_render.cpp"}
{"emitted_functions":[{"compiled_body_bytes":176,"name":"Init__12VIWndOptionsP7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init.o","profile":"authored_cpp","sha256":"19febaa0ff380c0c4e95c25f6e0ae27e3e13f0f68c9e9e4c5da4d8a44e319d1a","source":"sources/w11-vi-render-c-wnd-options-init/init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":512,"name":"CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend"}],"header_closure":[],"kind":"independently_authored_source","object_name":"calc_profile_blend.o","profile":"authored_cpp","sha256":"75d8eec4fcfd0ca1778b81242a18edbcf2fde1d87746729d3ab25495f1c06d78","source":"sources/w11-vi-render-c-world-calc-profile-blend/calc_profile_blend.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"card_count.o","profile":"authored_cpp","sha256":"8fc282010552f1e07e3bec4884e4235f943e50308d7f7066d7d1d46f9a0565fa","source":"sources/w11-vi-render-c-world-card-count/card_count.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"Clear__7VIWorld"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_clear.o","profile":"authored_cpp","sha256":"1397e9d9a8bc0402605a4774d235d88d4fb85569c61f64bd21f89351f2ef039c","source":"sources/w11-vi-render-c-world-clear/world_clear.cpp"}
{"emitted_functions":[{"compiled_body_bytes":216,"name":"__7VIWorld"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_ctor.o","profile":"authored_cpp","sha256":"556f4f252cd3ce1b409dbcae5b8bafec062936d296050dad46f19112b50f560b","source":"sources/w11-vi-render-c-world-ctor/world_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_$_7VIWorld"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_dtor.o","profile":"authored_cpp","sha256":"fb2a235f6845d593765d79aed2d1bbfd860f27e96f810e4e307a9b403e9c5f95","source":"sources/w11-vi-render-c-world-dtor/world_dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":540,"name":"Init__7VIWorldiiiifRC7VIVect3RC6VIBBox"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_init.o","profile":"authored_cpp","sha256":"b587b40c65a61804cbb69eb4cbc8bb4f9307287c7c39c18cb2a2013b4a0c9382","source":"sources/w11-vi-render-c-world-init/world_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":176,"name":"RenderByFrustum__7VIWorldR15VISceneRendVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"render_by_frustum.o","profile":"authored_cpp","sha256":"38ffee117d5169d7ee4b5485e86a0388bfd82f9a45ce1111cecd8d003807bce5","source":"sources/w11-vi-render-c-world-render-by-frustum/render_by_frustum.cpp"}
{"emitted_functions":[{"compiled_body_bytes":344,"name":"RenderByPortals__7VIWorldR15VISceneRendVars"}],"header_closure":[],"kind":"independently_authored_source","object_name":"render_by_portals.o","profile":"authored_cpp","sha256":"5d375b4e9aeb10c5126cb824d416bac0f39941eb340e1a983c62a0c9009180b4","source":"sources/w11-vi-render-c-world-render-by-portals/render_by_portals.cpp"}
```

## Authored but never executed

All of the above. Host contracts passed and each function ran against the original in the interpreter, but none ran on
PS2 hardware or a full emulator, and the real callers (the VI main loop, scene setup, world rendering,
particle/flora/atmosphere code) have not been run with these bodies. In the paired runs every callee was a stub
(Python), so the callees' own behaviour (sinf/cosf, raster, sound device, pool and array templates) is unchecked.

## Unknown

- The names and meanings of most scene fields; offsets are recorded, not named.
- What each flag bit of `SetupTimeOfDayFog` stands for, and what the two clip-plane values are besides distances.
- Whether the `InitTimeOfDay` slots are sun direction, light colours or something else (the layout is observed,
  the labels are not).
- No body is an original-byte match; sizes differ from the original extents.
- The static data referenced (`VISplashFx$s*`, `VITrailFx$s*`, `VIWndOptions$sBattleMusicText`) has no definition in
  this package; sizes and initial values are not established here.
