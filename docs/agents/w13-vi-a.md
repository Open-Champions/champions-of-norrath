# VI engine: rasterizer, scene, world, sprites, sound, files and containers (part 1)

Package work is on `agents/w13-vi-a`. It supplies 38 VI engine functions, one per commit, each with its own directory `experimental/sources/w13-vi-a-<unit>/` holding the implementation (`*.cpp`) and a `host_contract.cpp`.

## Checks performed

- Each implementation compiled with the reviewed EE g++ using the plan's `authored_cpp` flags. Each object defines exactly the one function named in its unit. None defines a COMMON symbol.
- Each `host_contract.cpp` compiled with the host C++ compiler and ran to a zero exit. The contracts use stub callees that record call order and arguments, so a wrong branch, offset or call sequence changes the recorded trace and fails the run. They include host headers, so they are not EE build inputs and must not be added to the plan.
- Body sizes below are from the EE profile on this machine, not from the original executable. No unit is claimed to be an original-byte match. No EE or gameplay execution happened.
- A full partial build was not rerun for this note, so the official unresolved-count delta is not measured here. The lead integrator's link gives that figure.

## Imports closed

- `AddSplash__12VIAtmosphereR7VIVect3`
- `GetLightning__12VIAtmospherePf`
- `ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera`
- `ResetLightning__12VIAtmosphere`
- `SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32`
- `Unload__13VIBattleMusic`
- `Init__6VIBBoxiP7VIVect3`
- `CullByTop__11VIClipStackRC6VIBBox`
- `Remove__12VIDictionaryR10VIResource`
- `Init__14VIEmitterQueryi12VISpriteType`
- `ParseObject__10VIESFParse`
- `BufferFlush__6VIFile`
- `IsAsynchComplete__6VIFile`
- `CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary`
- `SetLayerColor__10VIMaterialiRC9VIColor32`
- `SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary`
- `SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary`
- `Zero__10VIMatrix33`
- `SwapRows__10VIMatrix44ii`
- `Clear__12VINameSprite`
- `Destroy__17VIParticleEmitter`
- `ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary`
- `Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f`
- `DeallocateChannels__13VISoundDeviceUl`
- `DeletePlayback__13VISoundDeviceP10VIPlayback`
- `Play__13VISoundDeviceii12VISOUNDCLASS`
- `Playback__13VISoundDevicei`
- `PlayBgm__13VISoundDeviceiPCc`
- `SetPan__13VISoundDeviceif`
- `SetPitch__13VISoundDeviceii`
- `SetVolume__13VISoundDeviceif`
- `StopBgm__13VISoundDevicei`
- `StopChannels__13VISoundDeviceUl`
- `Destroy__13VISpellEffect`
- `Destroy__19VISpellEffectDriver`
- `Process__18VISpellEventDriver`
- `Init__10VIZoneRoomiiUiUiR6VIBBox`
- `Init__10VIZoneTreeii`

## Imports opened

The units call the symbols below and do not define them. Some of them may already be defined by other packages or by the plan, so this is an upper bound on what the integrator will see newly unresolved. Compiler helpers (`__divdi3`, `dpcmp`, `dpsub`, `fptodp`, `__builtin_new`, `__builtin_delete`, `cosf`, `rand`, `strcmp`, `strcpy`) and `norrath_overlay_sIopFunctionData_11edf08` are among them.

```
AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi
AddTail__7VILListP7VILNode
AllocateChannels__13VISoundDeviceii
Allocate__10VIZoneRoom
AsWave__7VISound
AsXm__7VISound
Bound__6VIBBoxiRC7VIVect3
CDSync__6VIFilei
CalcDataSize__10VIZoneRoom
ChannelCount__C4VIXm
ChannelCount__C6VIWave
Collide__7VISceneR8VIEnvRay
Collide__7VISceneR9VICollRayR13VISceneFilter
Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition
DeltaTime__C16VIParticleSystem
DestroyMotif__22VIParticleDefinitionExP15VIParticleMotif
Destroy__16VISpellEventList
End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry
Erase__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi
Erase__t6VIPool1ZP20VIParticleDefinitioni
FindIopFunction__FUi
Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi
FirstChannel__13VISoundDeviceUl
FirstMotif__C22VIParticleDefinitionEx
GetHitPoint__9VICollRay
GetHitPoint__C8VIEnvRay
GetLayerColor__C10VIMateriali
GetMaterialPalEntry__8VIRasterii
GetMaterialPalSize__8VIRasteri
GetMaterialPal__C14VISimpleSprite
GetTime__7VIScene
Identity__10VIMatrix33
Identity__10VIMatrix44
Init__8VIEnvRayRC7VIVect3T1Ui
Init__9VICollRayRC7VIVect3T1
Init__t10VIMultiMap2ZfZ14VIEmitterEntryi
Init__t7VIArray1Z10VIZoneLeafi
Init__t7VIArray1Z10VIZoneNodei
Length__C7VILList
Material__8VIRasteri
NewCreateEmitter__18VISpellEventDriverP12VISpellEvent
NewCreateSound__18VISpellEventDriverP12VISpellEvent
NewCreateSprite__18VISpellEventDriverP12VISpellEvent
NewDestroySound__18VISpellEventDriverP12VISpellEvent
NewDestroySprite__18VISpellEventDriverP12VISpellEvent
NewFadeSprite__18VISpellEventDriverP12VISpellEvent
NewPlayAttack__18VISpellEventDriverP12VISpellEvent
NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent
NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent
NewPlayback__13VISoundDevice
NewSetAction__18VISpellEventDriverP12VISpellEvent
NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent
NewShakeCamera__18VISpellEventDriverP12VISpellEvent
NextChannel__13VISoundDeviceUli
ParseActorObj__10VIESFParse
ParseAdpcmObj__10VIESFParse
ParseCSpriteObj__10VIESFParse
ParseEffectVolumeSpriteObj__10VIESFParse
ParseFloraSpriteObj__10VIESFParse
ParseFontObj__10VIESFParse
ParseGroupSpriteObj__10VIESFParse
ParseHSpriteAnimObj__10VIESFParse
ParseHSpriteObj__10VIESFParse
ParseLODSpriteObj__10VIESFParse
ParseMaterialPalObj__10VIESFParse
ParseParticleDefinitionObj__10VIESFParse
ParseParticleSpriteObj__10VIESFParse
ParsePointLightObj__10VIESFParse
ParsePointSpriteObj__10VIESFParse
ParseResourceDirObj__10VIESFParseiR13VIResourceDir
ParseSimpleSpriteObj__10VIESFParse
ParseSoundSpriteObj__10VIESFParse
ParseSpellEffectObj__10VIESFParse
ParseStreamAudioSpriteObj__10VIESFParse
ParseSurfaceArrayObj__10VIESFParse
ParseSurfaceObj__10VIESFParse
ParseWorldObj__10VIESFParsei
ParseXmObj__10VIESFParse
ParticleCount__C17VIParticleEmitter
ParticleDefinition__16VIParticleSystemi
ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb
ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb
ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb
ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent
ProcessInactiveEvents__18VISpellEventDriver
ProcessLightningEvent__18VISpellEventDriver
ProcessProjectileEvent__18VISpellEventDriver
ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent
ProcessWaitEvent__18VISpellEventDriver
ReadBegin__9VIObjFileRUsT1
ReadEnd__9VIObjFile
RecomputeLightningPatches__12VIAtmosphere
ReleaseMaterial__8VIRasteriR12VIDictionary
ReleaseSound__13VISoundDeviceiR12VIDictionary
ReleaseSurface__8VIRasteriR12VIDictionary
RemHead__7VILList
Remove__7VILNode
Reset__14VICameraEffect
SampleRate__C6VIWave
SetIopFunction__FUii
SetLocation__17VIParticleEmitterRC7VIVect3
ShareSurface__8VIRasteri
Sprite__7VIScenei
Stop__13VISoundDevicei
Type__C7VISound
UnlinkAllMotifBlenders__17VIParticleEmitter
Update__17VIParticleEmitter
VIGetFeatureFlags__Fv
Volume__14VIQueryTriListRC6VIBBox
_$_18VISpellEventDriver
_$_22VIParticleDefinitionEx
_$_t7VIArray1Z14VIResourceElem
__13VISceneFilter
__18VISpellActiveEventP12VISpellEventP7VIScene
__8VIEnvRay
__9VICollRay
__as__10VIMatrix33RC10VIMatrix33
__builtin_delete
__builtin_new
__divdi3
__t7VIArray1Z14VIResourceElem
_vt$13WeatherFilter
cosf
dpcmp
dpsub
fptodp
norrath_overlay_sIopFunctionData_11edf08
rand
sceLseek
sceWrite
strcmp
strcpy
```

## Build-plan entries needed

Append these inputs with fresh `input_order` values. `profile` is `authored_cpp`, `kind` is `independently_authored_source`, and `header_closure` is empty for every unit (each implementation has no `#include`). `default_authored_inputs` increases by 38. Do not add any `host_contract.cpp`. No `private_object_sha256` is claimed.

```json
{"source":"sources/w13-vi-a-atmosphere-addsplash/add_splash.cpp","object_name":"add_splash.o","sha256":"9ba9767400deef816912257543a44d185c9c5aa101f8700997d45ccaeb3ba30e","emitted_functions":[{"name":"AddSplash__12VIAtmosphereR7VIVect3","compiled_body_bytes":200}]}
{"source":"sources/w13-vi-a-atmosphere-getlightning/get_lightning.cpp","object_name":"get_lightning.o","sha256":"04a5c3705286f12b1a1a308a4a046b7a1031c489ae44e76eac002c39bfdd98b6","emitted_functions":[{"name":"GetLightning__12VIAtmospherePf","compiled_body_bytes":768}]}
{"source":"sources/w13-vi-a-atmosphere-processlavastorm/process_lavastorm.cpp","object_name":"process_lavastorm.o","sha256":"918af1f58f5b5451adb551156be59b1a4a697cc66f921cb1e5afa19d45427110","emitted_functions":[{"name":"ProcessLavastorm__12VIAtmosphereR7VISceneR8VICamera","compiled_body_bytes":1416}]}
{"source":"sources/w13-vi-a-atmosphere-resetlightning/reset_lightning.cpp","object_name":"reset_lightning.o","sha256":"a0ecb91439166cc8721a4aa2a93f8fe1ba98edd292cbdba02cd1bffd049449ca","emitted_functions":[{"name":"ResetLightning__12VIAtmosphere","compiled_body_bytes":256}]}
{"source":"sources/w13-vi-a-atmosphere-setskycolor/set_sky_color.cpp","object_name":"set_sky_color.o","sha256":"8149e10cb5eedaef9b2afd8e8d6e9962fb190e1f768792c1ab548debd8140b3d","emitted_functions":[{"name":"SetSkyColor__12VIAtmosphereR7VISceneRC9VIColor32","compiled_body_bytes":344}]}
{"source":"sources/w13-vi-a-battlemusic-unload/unload.cpp","object_name":"unload.o","sha256":"de2179cac08d129fe5f15c6a7132b78df69abf5e4d8fb6c93eaff66808f78e69","emitted_functions":[{"name":"Unload__13VIBattleMusic","compiled_body_bytes":260}]}
{"source":"sources/w13-vi-a-bbox-init/init.cpp","object_name":"init.o","sha256":"407ccef2a7c82c3ce68c392fe2e4d09ad184ebe587ade8a153db6db07201b049","emitted_functions":[{"name":"Init__6VIBBoxiP7VIVect3","compiled_body_bytes":120}]}
{"source":"sources/w13-vi-a-clipstack-cullbytop/cull_by_top.cpp","object_name":"cull_by_top.o","sha256":"e3aaa19c07a3e54927920a6d2e93543e5742b763ebcfa7610533bc053a32382b","emitted_functions":[{"name":"CullByTop__11VIClipStackRC6VIBBox","compiled_body_bytes":304}]}
{"source":"sources/w13-vi-a-dictionary-remove/remove.cpp","object_name":"remove.o","sha256":"177eee0ba34114cad44071c585517c8de02c897779e66442c9d970ca7d40193b","emitted_functions":[{"name":"Remove__12VIDictionaryR10VIResource","compiled_body_bytes":80}]}
{"source":"sources/w13-vi-a-emitterquery-init/init.cpp","object_name":"init.o","sha256":"b417ac6ba758a7240a2ed74e3f4a0e0e7d261f7df1030d91cbc189537d71fc3b","emitted_functions":[{"name":"Init__14VIEmitterQueryi12VISpriteType","compiled_body_bytes":56}]}
{"source":"sources/w13-vi-a-esfparse-parseobject/parse_object.cpp","object_name":"parse_object.o","sha256":"d892daa0637ab92c06543f368fbdaee6ce44b2d43481d7c7072720579a70afe2","emitted_functions":[{"name":"ParseObject__10VIESFParse","compiled_body_bytes":920}]}
{"source":"sources/w13-vi-a-file-bufferflush/buffer_flush.cpp","object_name":"buffer_flush.o","sha256":"5e5779466b321b591358cae962036fd13079052a07680878169d58da2866abfb","emitted_functions":[{"name":"BufferFlush__6VIFile","compiled_body_bytes":224}]}
{"source":"sources/w13-vi-a-file-isasynchcomplete/is_asynch_complete.cpp","object_name":"is_asynch_complete.o","sha256":"93071421965becce028779e7a24a7021d0909a6389effb72a604c6c6a221be48","emitted_functions":[{"name":"IsAsynchComplete__6VIFile","compiled_body_bytes":64}]}
{"source":"sources/w13-vi-a-material-copylayer/copy_layer.cpp","object_name":"copy_layer.o","sha256":"50538dc2f48e1962117db7a03191649bf1614ee8338b73b25e42b4faf2d2ee8d","emitted_functions":[{"name":"CopyLayer__10VIMaterialiiR8VIRasterR12VIDictionary","compiled_body_bytes":368}]}
{"source":"sources/w13-vi-a-material-setlayercolor/set_layer_color.cpp","object_name":"set_layer_color.o","sha256":"323b14a79775e9e37f98a92aa9a7cacaf4105f3793b27d03c807e7701bc548b1","emitted_functions":[{"name":"SetLayerColor__10VIMaterialiRC9VIColor32","compiled_body_bytes":64}]}
{"source":"sources/w13-vi-a-material-setlayertexture/set_layer_texture.cpp","object_name":"set_layer_texture.o","sha256":"1a4b0d17c52969662a7b40e7b45a6dc771b24c1426f808a72437c3925271f7d5","emitted_functions":[{"name":"SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary","compiled_body_bytes":140}]}
{"source":"sources/w13-vi-a-material-setnumlayers/set_num_layers.cpp","object_name":"set_num_layers.o","sha256":"92b13a8cdfb4aa1942575de251c2b4c398363b6343cc50b47ac277bf38bf14d1","emitted_functions":[{"name":"SetNumLayers__10VIMaterialiR8VIRasterR12VIDictionary","compiled_body_bytes":376}]}
{"source":"sources/w13-vi-a-matrix33-zero/zero.cpp","object_name":"zero.o","sha256":"141a9fcee52f104697323550955544d86f8cc61112a5000a28859bb6f42d5ecc","emitted_functions":[{"name":"Zero__10VIMatrix33","compiled_body_bytes":32}]}
{"source":"sources/w13-vi-a-matrix44-swaprows/swap_rows.cpp","object_name":"swap_rows.o","sha256":"22d6f9b6d6e6921304759df722f0d397840178809e2d8f7b84474cdc16c1a9d8","emitted_functions":[{"name":"SwapRows__10VIMatrix44ii","compiled_body_bytes":60}]}
{"source":"sources/w13-vi-a-namesprite-clear/clear.cpp","object_name":"clear.o","sha256":"05b15d80242c9dfb8471d9b27f38991ebbafb9d1e885ff2a8d8a6df6f6ba3553","emitted_functions":[{"name":"Clear__12VINameSprite","compiled_body_bytes":64}]}
{"source":"sources/w13-vi-a-particleemitter-destroy/destroy.cpp","object_name":"destroy.o","sha256":"c2bfd7b961bb555e36de8b7e2f7e1aefb8a0b2d8dc51f534dbad48d490832875","emitted_functions":[{"name":"Destroy__17VIParticleEmitter","compiled_body_bytes":144}]}
{"source":"sources/w13-vi-a-particlesystem-releasedefinition/release_definition.cpp","object_name":"release_definition.o","sha256":"bc25996322e1d75241f3ff5cf8ee8265060caa6071b2bfc57626c4fe427bfdad","emitted_functions":[{"name":"ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary","compiled_body_bytes":376}]}
{"source":"sources/w13-vi-a-querytrilist-reset/reset.cpp","object_name":"reset.o","sha256":"ee3799e45a2d1279ed80421ecc239792a7985557aca6f1f86a2c1eb56cf07d3e","emitted_functions":[{"name":"Reset__14VIQueryTriListRC6VIBBoxRC7VIVect3f","compiled_body_bytes":120}]}
{"source":"sources/w13-vi-a-sounddevice-deallocatechannels/deallocate_channels.cpp","object_name":"deallocate_channels.o","sha256":"cb035cb9798512df49e9e549d9d2ae70dde658c534ad65fdc443194a90fae7c7","emitted_functions":[{"name":"DeallocateChannels__13VISoundDeviceUl","compiled_body_bytes":20}]}
{"source":"sources/w13-vi-a-sounddevice-deleteplayback/delete_playback.cpp","object_name":"delete_playback.o","sha256":"b00c11245356742017dda83df3fdedef9929a2477609bce9eae56f69f948f446","emitted_functions":[{"name":"DeletePlayback__13VISoundDeviceP10VIPlayback","compiled_body_bytes":52}]}
{"source":"sources/w13-vi-a-sounddevice-play/play.cpp","object_name":"play.o","sha256":"e7ac882bd278cfdbafbe74efcd08f125ffff2c9c9b865518c21572afd690a90e","emitted_functions":[{"name":"Play__13VISoundDeviceii12VISOUNDCLASS","compiled_body_bytes":1196}]}
{"source":"sources/w13-vi-a-sounddevice-playback/playback.cpp","object_name":"playback.o","sha256":"f35bfc161139dae4a3347cd946623e7e75ac638f1c82b24151cd3a61e67969c7","emitted_functions":[{"name":"Playback__13VISoundDevicei","compiled_body_bytes":136}]}
{"source":"sources/w13-vi-a-sounddevice-playbgm/play_bgm.cpp","object_name":"play_bgm.o","sha256":"4cd96702ed22e0df31aa143ee5b434d01009172401d919a9e83cfcfd7aaff2b2","emitted_functions":[{"name":"PlayBgm__13VISoundDeviceiPCc","compiled_body_bytes":44}]}
{"source":"sources/w13-vi-a-sounddevice-setpan/set_pan.cpp","object_name":"set_pan.o","sha256":"2da56271d2304974884b0f8249af1ef424da37af7619e32900db42a155812acf","emitted_functions":[{"name":"SetPan__13VISoundDeviceif","compiled_body_bytes":724}]}
{"source":"sources/w13-vi-a-sounddevice-setpitch/set_pitch.cpp","object_name":"set_pitch.o","sha256":"86e9aff14ba558aec6cb2fa86970e4ad7c58598f60a8175ba05a43efd3ceeeb5","emitted_functions":[{"name":"SetPitch__13VISoundDeviceii","compiled_body_bytes":452}]}
{"source":"sources/w13-vi-a-sounddevice-setvolume/set_volume.cpp","object_name":"set_volume.o","sha256":"0b391f09a6925dfdffc42c12ceba093d2f212cef722773abdc0542383a500484","emitted_functions":[{"name":"SetVolume__13VISoundDeviceif","compiled_body_bytes":732}]}
{"source":"sources/w13-vi-a-sounddevice-stopbgm/stop_bgm.cpp","object_name":"stop_bgm.o","sha256":"e0ad0a6d908b473bb184781b1ae41b74bbc2a5ee17eee08faa89c7c775f4f264","emitted_functions":[{"name":"StopBgm__13VISoundDevicei","compiled_body_bytes":96}]}
{"source":"sources/w13-vi-a-sounddevice-stopchannels/stop_channels.cpp","object_name":"stop_channels.o","sha256":"2a658890fb1327dc087f4c5ee4f4cc855f22de33d01ef368aab393873158ceb7","emitted_functions":[{"name":"StopChannels__13VISoundDeviceUl","compiled_body_bytes":128}]}
{"source":"sources/w13-vi-a-spelleffect-destroy/destroy.cpp","object_name":"destroy.o","sha256":"803172adc59a0781cf364a7f66efd8b4778720d09ad85b49272b00bfc3bce850","emitted_functions":[{"name":"Destroy__13VISpellEffect","compiled_body_bytes":88}]}
{"source":"sources/w13-vi-a-spelleffectdriver-destroy/destroy.cpp","object_name":"destroy.o","sha256":"e775c48681936462566bd55626247966df2de305bf1465fe365beec3c1b09155","emitted_functions":[{"name":"Destroy__19VISpellEffectDriver","compiled_body_bytes":208}]}
{"source":"sources/w13-vi-a-spelleventdriver-process/process.cpp","object_name":"process.o","sha256":"cf4887743f0da614c14d465b4ca0e84abd0beefdf9e3fb6e0c3418706909b6ee","emitted_functions":[{"name":"Process__18VISpellEventDriver","compiled_body_bytes":1340}]}
{"source":"sources/w13-vi-a-zoneroom-init/init.cpp","object_name":"init.o","sha256":"810f7769fc38e00db7e9ac5928cfb8fac6f51b7e0a49861809090cfe65f58be0","emitted_functions":[{"name":"Init__10VIZoneRoomiiUiUiR6VIBBox","compiled_body_bytes":192}]}
{"source":"sources/w13-vi-a-zonetree-init/init.cpp","object_name":"init.o","sha256":"0cb2ab6e0a0fa2771551b192865aec06b5b395dff6906c456c3aacce805a24a4","emitted_functions":[{"name":"Init__10VIZoneTreeii","compiled_body_bytes":80}]}
```

## Authored but never executed

All 38 functions. They were compiled for the EE and exercised only on the host against stub callees. Nothing ran on an EE interpreter, the private paired-run harness, or the game.

## Unknown

- Whether any compiled body matches the original instruction for instruction. Sizes were not compared with the original function extents in this note.
- Struct member names and the full layouts behind the offsets the units read and write. The sources use offsets established per function; offsets not touched by these functions are not described.
- The behaviour of every imported callee listed above beyond the signature the mangled name gives. The host stubs assume only what each unit's call site needs.
- Which of the opened imports are already supplied by other packages.
