# w10-vi-b: VI front-end framework classes (VIPad to VIUI)

Thirty complete functions of the pinned SLUS-20565 image, authored as independent C++ under
`experimental/sources/w10-vi-b-*`, one function per directory and one per commit (plus one correction
commit for EndScene, see below). Addresses and sizes were read from the image with EE binutils 2.45.1;
every source file opens with an `Observed` comment giving what the original does. Names are the
image's own symbol names; no field names were guessed and offsets are written as byte offsets.
Each unit compiles with the plan's `authored_cpp` flags. Fresh compilations are **not** claimed to be
original-byte matches; they differ in schedule and size from the originals. No game bytes, extracted
data or disassembly are committed.

## How each function was checked

1. Compiled with the reviewed toolchain (all 30 objects also link together with `ld -r`, no duplicate
   definitions).
2. A `host_contract.cpp` next to each source (not a plan input) builds on the host, runs the function
   against stub callees and fails (nonzero exit) on a wrong call order, argument, stored value or
   result. Pointer-chasing code (`Clear__7VIScene`) uses a small `NORRATH_HOST_CONTRACT` translation
   hook; absolute hardware stores use a host hook as in the vi-launcher package.
3. A private paired run with the R5900 interpreter outside git (see the memory note about the
   private paired-run harness): the authored object is linked with every undefined symbol bound to the
   original address, and original and authored functions run from identical memory snapshots with
   identical call-logging callees. Call logs (arguments, dereferenced stack blocks, float arguments),
   every changed byte outside the stack, and return values had to be equal. Randomised object memory
   and scripted callee results were used so each branch was taken. Coverage of original words:
   Flush 96/97, EndScene 48/48, BeginScene 49/49, Raster Init 198/204, Scene Clear 206/209,
   Scene Init 231/231, Scene Render 227/228 (missed words are nops or unreachable-by-script paths).
   The ctor/dtor/trivial functions compare memory fully but have no branches.
   A mutated copy of one source (a field offset moved by four bytes) was confirmed to be reported, so the
   comparison does detect errors.
4. The paired run caught a real bug: `EndScene__8VIRaster` first had the DMA sync inside the bit-0
   branch; the original runs it on every call. It was corrected in its own commit with the host contract
   updated.

Limits of the comparison: data symbols that an authored unit *defines* (`_5VIWnd$mUI`, `_5VIWnd$mFocus`,
`VISplashFx$*`) land at new addresses in the harness, so those three units are compared by
callee logs and by the equivalent stores to relocated addresses rather than to the original words.
The interpreter orders NaN differently from the R5900 FPU, so the camera-limit comparison in
`Render__7VISceneR8VICamera` was only exercised with finite values.

## Imports closed (30 functions)

`__5VIPad`, `BeginScene__8VIRaster`, `Close__8VIRaster`, `EndScene__8VIRaster`,
`Init__8VIRasterPviii`, `SetClearColor__8VIRasterRC10VIColor32F`, `SetWireframe__8VIRasteri`,
`_$_8VIRaster`, `__8VIRaster`, `Clear__7VIScene`,
`Init__7VIScenelR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide`, `Render__7VISceneR8VICamera`,
`SetTimeOfDay__7VIScenef`, `SetTime__7VIScenel`, `_$_7VIScene`, `__7VIScene`, `VISetWindowUI__FP4VIUI`,
`StartAmbientSoundtrack__7VISetup`, `StopAmbientSoundtrack__7VISetup`, `Close__13VISoundDevice`,
`Flush__13VISoundDevice`, `Init__13VISoundDevice`, `Process__13VISoundDevice`,
`_$_13VISoundDevice`, `__13VISoundDevice`, `VISplashFxFree__Fv`,
`VISplashFxInit__FP8VIRasterP12VIDictionary`, `GetTime__7VITimer`, `Close__4VIUI`,
`Init__4VIUIP8VIRasterP13VISoundDeviceP12VIDictionary`.

Data defined by these units: `_5VIWnd$mUI` (0x01192c70) and `_5VIWnd$mFocus` (0x01192c74) by
`VISetWindowUI`; `VISplashFx$sRaster`, `VISplashFx$sDictionary`, `VISplashFx$sBirdTex` by
`VISplashFxInit` (the image's file-local words at 0x011e27d8, 0x011e27dc and 0x011e27e0, published
under these names so a future `VISplashFxReset__Fv` and `VISplashFxProcess__Fi` can share them).

## Names in the package list that were not authored here

`RunPatcher__7VISetup`, `Init__9VIStationiPCcPcT3T2T2i` and `Process__9VIStation` already have complete
authored sources from `w9-frontend-a` (`w9-frontend-a-vi-setup-run-patcher`,
`w9-frontend-a-vi-station-init`, `w9-frontend-a-vi-station-process`). They are in `snapshot.json` but not
in `build-plan.json`, which is why they still show as unresolved. They need the build-plan entries listed
in `docs/agents/w9-frontend-a.md`; duplicating them here was avoided.

## Imports opened

Callees referenced but not defined by this package (172 names). Many are template-instance and
SDK names that other packages own:

`AddTail__7VILListP7VILNode`, `AsWave__7VISound`, `AsXm__7VISound`, `BeginAdpcmTransfer__13VISoundDevicePPvUiiPi`, `BeginXmTransfer__13VISoundDevicePPvUiUiiPi`, `Begin__Ct6VIPool1ZP13VIHSpriteAnim`, `Begin__Ct6VIPool1ZP16VIStaticLighting`, `Begin__Ct6VIPool1ZP8VIRefMap`, `Begin__Ct6VIPool1ZP8VISprite`, `CalcOutdoorsIntensity__7VISceneR8VICamera`, `CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend`, `ClearShadowQueue__7VIScene`, `Clear__11VIClipStack`, `Clear__12VIAtmosphereR7VIScene`, `Clear__14VIQueryTriList`, `Clear__16VIParticleSystemR12VIDictionary`, `Clear__17VISceneSoundTrack`, `Clear__19VIRadialFloraSystem`, `Clear__19VISpellEffectSystem`, `Clear__7VIWorld`, `Clear__8VIRaster`, `Clear__t6VIList1Z7VIActor`, `Clear__t6VIList1ZQ27VIScene12VISceneOccup`, `Clear__t6VIPool1ZP13VIHSpriteAnim`, `Clear__t6VIPool1ZP16VIStaticLighting`, `Clear__t6VIPool1ZP8VIRefMap`, `Clear__t6VIPool1ZP8VISprite`, `Clear__t7VIArray1Zi`, `Clear__t8VIVector1Z18VISceneDisplayElem`, `CreateSprite__7VIScene12VISpriteTypei`, `EnableAllLights__8VIRasteri`, `End__Ct6VIPool1ZP13VIHSpriteAnim`, `End__Ct6VIPool1ZP16VIStaticLighting`, `End__Ct6VIPool1ZP8VIRefMap`, `End__Ct6VIPool1ZP8VISprite`, `Find__12VIDictionaryUiR14VIResourceTypeRi`, `Flip__8VIRaster`, `FlushCache`, `FlushIopCommand__Fb`, `Flush__8VIRaster`, `GetEnvFlags__7VISceneRC7VIVect3`, `GlobalFree__14VIEffectVolume`, `GlobalInit__14VIEffectVolumeP7VIScene`, `Identity__10VIMatrix44`, `IdleTransfer__13VISoundDevice`, `InitSPRDoubleBuffer__8VIRaster`, `InitTimeOfDay__7VIScene`, `Init__11VIClipStack`, `Init__12VIAtmospherelR8VIRasteri`, `Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f`, `Init__16VIParticleSystemlP8VIRasteri`, `Init__17VISceneSoundTrack`, `Init__19VIRadialFloraSystemR7VIScene`, `Init__19VISpellEffectSystemP7VIScene`, `Init__7VILList`, `Init__7VIWorldiiiifRC7VIVect3RC6VIBBox`, `Init__8VIRaster`, `Init__t6VIList1Z7VIActor`, `Init__t6VIList1ZQ27VIScene12VISceneOccup`, `Init__t6VIPool1ZP13VIHSpriteAnimi`, `Init__t6VIPool1ZP16VIStaticLightingi`, `Init__t6VIPool1ZP8VIRefMapi`, `Init__t6VIPool1ZP8VISpritei`, `Init__t7VIArray1Zii`, `Init__t8VIVector1Z18VISceneDisplayElemi`, `Load__13VIStringTablePCc`, `Next__Ct6VIPool1ZP13VIHSpriteAnimi`, `Next__Ct6VIPool1ZP16VIStaticLightingi`, `Next__Ct6VIPool1ZP8VIRefMapi`, `Next__Ct6VIPool1ZP8VISpritei`, `Process__19VISpellEffectSystem`, `RasterDeferred__12VINameSpriteR8VIRaster`, `ReleaseDeferred__8VIRaster`, `ReleaseSurface__8VIRasteriR12VIDictionary`, `ReloadWords__4VIUI`, `RenderByFrustum__7VIWorldR15VISceneRendVars`, `RenderByPortals__7VIWorldR15VISceneRendVars`, `RenderDeferredList__7VISceneR8VICameraii`, `RenderShadows__7VIScene`, `RenderSky__12VIAtmosphereR7VISceneR8VICamera`, `RenderWeather__12VIAtmosphereR7VISceneR8VICamera`, `Render__16VIParticleSystemlR8VIRaster`, `Render__19VIRadialFloraSystemR8VICamera`, `Reset__11VIClipStack`, `Reset__13VISoundDevice`, `SetGSScissoring__8VIRaster`, `SetIopFunction__FUii`, `SetModelView__8VIRasterRC10VIMatrix44`, `SetOverrideMaterial__8VIRaster`, `SetPersonalLightMode__7VIScene19VIPersonalLightMode`, `SetPersonalLight__7VISceneRC7VIVect3fRC10VIColor32F`, `SetProjection__8VIRasterRC10VIMatrix44`, `SetStaticLighting__8VIRasterRC10VIColor32F`, `SetTime__8VIRasterl`, `SetupTimeOfDayFog__7VISceneR21VITerrainProfileBlendUi`, `SetupTimeOfDay__7VISceneR8VICameraR21VITerrainProfileBlendUi`, `SurfCache`, `Type__C7VISound`, `Unload__13VIStringTable`, `UpdateSoundEmitters__7VISceneR8VICamera`, `Update__12VIAtmosphereR7VISceneR8VICameralff`, `Update__19VIRadialFloraSystemRC7VIVect3`, `VIEEDMASync__Fi`, `VIHashResourceID`, `VISplashFxReset__Fv`, `VITrailFxClear__FR8VIRasterR12VIDictionary`, `VITrailFxInit__FR8VIRasterR12VIDictionary`, `VITrailFxRender__FlR8VIRasterR8VICamera`, `_$_10VIMatrix44`, `_$_12VIAtmosphere`, `_$_13VIHSpriteAnim`, `_$_14VIQueryTriList`, `_$_16VIParticleSystem`, `_$_19VISpellEffectSystem`, `_$_7VILList`, `_$_7VIWorld`, `_$_t5VIMap2ZiZi`, `_$_t6VIList1Z10VIMaterial`, `_$_t6VIList1Z7VIActor`, `_$_t6VIList1ZP12VIPrimBuffer`, `_$_t6VIList1ZP9VISurface`, `_$_t6VIList1ZQ27VIScene12VISceneOccup`, `_$_t6VIPool1Z19VIRasterMaterialLRU`, `_$_t6VIPool1ZP12VIPrimBuffer`, `_$_t6VIPool1ZP13VIColorBuffer`, `_$_t6VIPool1ZP13VIHSpriteAnim`, `_$_t6VIPool1ZP13VIMaterialPal`, `_$_t6VIPool1ZP16VIStaticLighting`, `_$_t6VIPool1ZP7VIRFont`, `_$_t6VIPool1ZP8VIRefMap`, `_$_t6VIPool1ZP8VISprite`, `_$_t6VIPool1ZP9VISurface`, `_$_t7VIArray1Z11VIClipEntry`, `_$_t7VIArray1Z7VIPlane`, `_$_t7VIArray1Z7VIVect3`, `_$_t7VIArray1Zi`, `_$_t8VIVector1Z18VISceneDisplayElem`, `_7VITimer$SystemTime`, `_8VIRaster$DBuffer`, `__12VIAtmosphere`, `__14VIQueryTriList`, `__16VIParticleSystem`, `__19VIRadialFloraSystem`, `__19VISpellEffectSystem`, `__7VILList`, `__7VILNode`, `__7VIWorld`, `__builtin_delete`, `__t6VIList1Z10VIMaterial`, `__t6VIList1Z7VIActor`, `__t6VIList1ZP12VIPrimBuffer`, `__t6VIList1ZP9VISurface`, `__t6VIList1ZQ27VIScene12VISceneOccup`, `__t6VIPool1Z19VIRasterMaterialLRU`, `__t6VIPool1ZP12VIPrimBuffer`, `__t6VIPool1ZP13VIColorBuffer`, `__t6VIPool1ZP13VIHSpriteAnim`, `__t6VIPool1ZP13VIMaterialPal`, `__t6VIPool1ZP16VIStaticLighting`, `__t6VIPool1ZP7VIRFont`, `__t6VIPool1ZP8VIRefMap`, `__t6VIPool1ZP8VISprite`, `__t6VIPool1ZP9VISurface`, `__t7VIArray1Z11VIClipEntry`, `__t7VIArray1Z7VIPlane`, `__t7VIArray1Z7VIVect3`, `__t7VIArray1Zi`, `__t8VIVector1Z18VISceneDisplayElem`, `sceGsResetGraph`, `sceGsSetDefDBuffDc`, `sceGsSwapDBuffDc`, `sceGsSyncV`

Of those, `_7VITimer$SystemTime` is defined by the vi-launcher `Init__7VITimer` unit; `SurfCache`
(a file-local table in the image at 0x011eda68, word 7 cleared by BeginScene) and `_8VIRaster$DBuffer`
(0x01227f80, 0x330 bytes, referenced by BeginScene, Init and SetClearColor) need an owner.

## Build-plan entries needed

Profile `authored_cpp`, empty header closure, kind `independently_authored_source`. `snapshot.json`
should also record each source and its `host_contract.cpp`. Object names are suggestions, unique in the
current plan. Neither shared file is edited here.

- `w10_vi_b_pad_ctor.o`, `sources/w10-vi-b-pad-ctor/pad_ctor.cpp`, SHA-256 `31247aaa8d4704e374059e9d43ad2a6943a3b0b4d46ae4eb565cd2690b27629f`
- `w10_vi_b_raster_begin_scene.o`, `sources/w10-vi-b-raster-begin-scene/raster_begin_scene.cpp`, SHA-256 `0b6193444016693045db4220d4cfb0b653c357e1651fdf114c6bdf56b431defc`
- `w10_vi_b_raster_close.o`, `sources/w10-vi-b-raster-close/raster_close.cpp`, SHA-256 `ead3341ddcf58b819d38ff7f2acc98aa54ac272440312b5e00df8251425dacf1`
- `w10_vi_b_raster_ctor.o`, `sources/w10-vi-b-raster-ctor/raster_ctor.cpp`, SHA-256 `611183f19b5e6c8cf26648c66fec6fdd99a12a91d4356b25a26e7b1c98b46a58`
- `w10_vi_b_raster_dtor.o`, `sources/w10-vi-b-raster-dtor/raster_dtor.cpp`, SHA-256 `523b2a44da9c9d011df7e63a76b3f5ed1c441df2e17157b7580555a6156c9453`
- `w10_vi_b_raster_end_scene.o`, `sources/w10-vi-b-raster-end-scene/raster_end_scene.cpp`, SHA-256 `17ff66b93549a3d71bface3ec582b0047ed09dd82857e7514cdc4a01f38c351c`
- `w10_vi_b_raster_init.o`, `sources/w10-vi-b-raster-init/raster_init.cpp`, SHA-256 `3612ab1b2adde3f7ee6f3895d0d90db5dd98df40c4d7d15dbf78fa69a32664f4`
- `w10_vi_b_raster_set_clear_color.o`, `sources/w10-vi-b-raster-set-clear-color/set_clear_color.cpp`, SHA-256 `c1f02fb9d8f017b00d8a64cd69638dd2a0b05e3ba081ed0b35996c164a270300`
- `w10_vi_b_raster_set_wireframe.o`, `sources/w10-vi-b-raster-set-wireframe/set_wireframe.cpp`, SHA-256 `90eadba3b338fa80136d660ff691756ae34c9c71c1f28dde9d66d93e32fb52dc`
- `w10_vi_b_scene_clear.o`, `sources/w10-vi-b-scene-clear/scene_clear.cpp`, SHA-256 `c4f5503ca681fd73378fc5285adb1962234e1b6d900479c6b4427bb40dbccf14`
- `w10_vi_b_scene_ctor.o`, `sources/w10-vi-b-scene-ctor/scene_ctor.cpp`, SHA-256 `2eaa6f70869983799df71ebd7466ffcdada0dd324fae17932ee35ff024987921`
- `w10_vi_b_scene_dtor.o`, `sources/w10-vi-b-scene-dtor/scene_dtor.cpp`, SHA-256 `64ca10bc0b869c8c75beb2a199a1fe186951c062911df21da0df26f180c569ca`
- `w10_vi_b_scene_init.o`, `sources/w10-vi-b-scene-init/scene_init.cpp`, SHA-256 `63f3efaf5caa55db06667475be46458c2e7f7af3e6877f334c7f46af548c9dba`
- `w10_vi_b_scene_render.o`, `sources/w10-vi-b-scene-render/scene_render.cpp`, SHA-256 `5163315d30f4c73431aeea1a3c3263e5c9553251f78a89ac4ede174ab70dff8f`
- `w10_vi_b_scene_set_time.o`, `sources/w10-vi-b-scene-set-time/set_time.cpp`, SHA-256 `7872f7310775e414917a02cbaec7b6a4e161534c6a6bb36eb757a9bb86c1ee8e`
- `w10_vi_b_scene_set_time_of_day.o`, `sources/w10-vi-b-scene-set-time-of-day/set_time_of_day.cpp`, SHA-256 `9354ecd3cddbaf6c185a08d438888fd158ab0a4494fd396c491cb6c4fdd8adec`
- `w10_vi_b_setup_start_ambient.o`, `sources/w10-vi-b-setup-start-ambient/start_ambient.cpp`, SHA-256 `a20325c6632e6cff41d0233744a14b1273c9d0988ba138af322022a1d4f8c191`
- `w10_vi_b_setup_stop_ambient.o`, `sources/w10-vi-b-setup-stop-ambient/stop_ambient.cpp`, SHA-256 `e0401fe8b5b22316bf0f8df22a7ba5dc8d155f7ba8ae96d87a114eefda59587a`
- `w10_vi_b_sound_device_close.o`, `sources/w10-vi-b-sound-device-close/close.cpp`, SHA-256 `bda3eb878645fdee889ff68dc0aabc5c27485e164753e4335d5fa19d2ab2900f`
- `w10_vi_b_sound_device_ctor.o`, `sources/w10-vi-b-sound-device-ctor/sound_device_ctor.cpp`, SHA-256 `a907a67e1e52cfef2537a0f2d39b946157593e51b6555bdf72157d7063bf4459`
- `w10_vi_b_sound_device_dtor.o`, `sources/w10-vi-b-sound-device-dtor/sound_device_dtor.cpp`, SHA-256 `4bf7d74d3d6c8e2fc2765027590cce855c60037b24c358d08fc03784d07eaddc`
- `w10_vi_b_sound_device_flush.o`, `sources/w10-vi-b-sound-device-flush/sound_device_flush.cpp`, SHA-256 `0cf808fcfcf1d2bbdf289e17fb18a991e4718c2795a701de82896b6ed80b6e5e`
- `w10_vi_b_sound_device_init.o`, `sources/w10-vi-b-sound-device-init/sound_device_init.cpp`, SHA-256 `f451e79e5d283e0ff786761b31d34bb66fc49178368a2d170f538f7578f448f6`
- `w10_vi_b_sound_device_process.o`, `sources/w10-vi-b-sound-device-process/process.cpp`, SHA-256 `24e74ad98ad3035885e0c990105bd4b8a99f4ef9f5dd65ccfc3ab9a81e64552d`
- `w10_vi_b_splash_fx_free.o`, `sources/w10-vi-b-splash-fx-free/splash_fx_free.cpp`, SHA-256 `5de125559fba7aa3633f0b5b03879d244fc48f36aebec93ff4163fa7491ea5fe`
- `w10_vi_b_splash_fx_init.o`, `sources/w10-vi-b-splash-fx-init/splash_fx_init.cpp`, SHA-256 `3f30430dab2beb5857cad1e7648866a42b2943d8ecedd73f630fae5f810a352f`
- `w10_vi_b_timer_get_time.o`, `sources/w10-vi-b-timer-get-time/get_time.cpp`, SHA-256 `4051aba264825e93a53c751a43f7de1135e31cf7cc90bcf10b29f2228c8f5020`
- `w10_vi_b_ui_close.o`, `sources/w10-vi-b-ui-close/ui_close.cpp`, SHA-256 `e841d46c623632cee465196c91932e854701a64f73e56f246af05b7d278f1217`
- `w10_vi_b_ui_init.o`, `sources/w10-vi-b-ui-init/ui_init.cpp`, SHA-256 `556a649d6ff93abfb41f4d3dafb1348d4ad1caa53ae55406427ed271e38a32f7`
- `w10_vi_b_window_ui.o`, `sources/w10-vi-b-window-ui/window_ui.cpp`, SHA-256 `c4798286da3b51b9847d8ce0d457cea2258048396f6ba13fca759b9ff30a3a94`

## Authored but never executed

All 30 functions. They have run only in the host contracts (stub callees) and in the private
interpreter against the original; none has run on a PS2, in the game, or against real GS/DMA hardware.
Absolute stores (the `0x12000000` store in BeginScene) were only logged, never executed against hardware.
`Flush__13VISoundDevice` can loop while waiting for another context to set a word; the interpreter
runs set that word from a callee stub, so real waiting behaviour is unobserved.

## Unknown

- What `VIPad` is beyond the three words its constructor clears (offsets 0, 4, 52); its size.
- The sizes and layouts of `VIRaster`, `VIScene`, `VISoundDevice` and `VIUI`; the 24-byte and 48-byte
  array elements zeroed by the `VIRaster` constructor and the 108-byte elements zeroed by the `VIScene`
  constructor; the unwritten part of each 24-byte element.
- What the absolute store of 32544 to 0x12000000 in BeginScene programs, and what the packed word
  (RGBA bytes with 0x3f800000 above) written into the double-buffer structure means; which fields of
  `_8VIRaster$DBuffer` the masks (~0x1ff) and the or-values 70 and 140 target.
- The layout of the 56-byte render-variable block and the 32-byte profile blend used by `Render`;
  the unit of the time-of-day value (clamped to 0..23.999989) and of the 64-bit scene time.
- Whether the unwritten first word of the render-variable block matters.
- The meaning of the `Flush__13VISoundDevice` fields (3412 is a gate that the idle loop waits on).
- Whether `StartAmbientSoundtrack`/`StopAmbientSoundtrack` are static or member functions (both are empty
  and take an ignored argument here); the original return type of `Close__4VIUI`.
- The twelve-byte pool entry layout beyond its first word, and the vtable-slot call made on sprites
  (read as slot 0, flag 3: believed to be a deleting destructor, inferred only from the flag value).
