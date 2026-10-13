# Lighting, model, world-query and low-level effect functions

Package work is on `agents/w12-light-model`. It supplies twenty-three new source units (twenty functions, three data objects), one committed host check for data objects that earlier waves authored but never selected, and a re-verification of one earlier unselected function. Every function was compared against the original in a private interpreter run; every unit has a host contract. Addresses and extents are from the pinned `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`, the executable recorded in `docs/BASELINE.md`). Compiled body sizes come from the reviewed EE `authored_cpp` profile and differ from the original extents; nothing here is a byte match.

## Evidence

Observed, inferred and unknown are kept in each source's header comment. Short form:

| Source directory | Symbol | Original | Checks |
| --- | --- | --- | --- |
| `w12-light-model-model-get-collision` | `modelGetCollision__FP12_modelHeaderi` | `0x16c740`, 52 | 80 randomised headers; 1 mutation caught |
| `w12-light-model-model-get-primary-bone` | `modelGetPrimaryBone__FP12_modelHeaderi` | `0x16c778`, 48 | 120 randomised headers; 2 mutations caught |
| `w12-light-model-model-set-attachment-flip` | `modelSetAttachmentFlip__Fv` | `0x16cc58`, 16 | 6 starting words; 1 mutation caught |
| `w12-light-model-light-disable-volumetric` | `lightDisableVolumetricLighting__Fv` | `0x13e720`, 12 | 5 starting words; 1 mutation caught |
| `w12-light-model-world-query-light-map` | `worldQueryLightMap__FP12_worldHeaderG6Point3` | `0x17c368`, 44 | 10 runs, float result register compared; 1 mutation caught |
| `w12-light-model-p-lightning-is-live` | `P_LightningIsLive__Fi` | `0x184b78`, 100 | 300 runs; first draft failed on the last branch, then clean; 1 mutation caught |
| `w12-light-model-model-draw-character` | `modelDrawCharacter__FP6PacketP12_modelHeaderli` | `0x16c680`, 192 | 400 randomised headers, masks and indices; 5 mutations caught |
| `w12-light-model-model-get-weapon-trail-pos` | `modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1` | `0x16c5c0`, 192 | 60 runs; 3 mutations caught |
| `w12-light-model-light-setup-ambient-only` | `lightSetupAmbientOnly__FP6Packet` | `0x13de48`, 164 | 40 randomised cursors and colours; 3 mutations caught |
| `w12-light-model-light-setup-non-directional-directional` | `lightSetupNonDirectionalDirectional__FP6Packet` | `0x13def0`, 164 | 40 randomised cursors and colours; 2 mutations caught |
| `w12-light-model-sound-pan-2d` | `soundPan2D__FiP6Point3f` | `0x24f7f8`, 132 | 60 runs, hooked `pan` and `soundChangeVolume`; 3 mutations caught |
| `w12-light-model-get-dynamic-light-number` | `getDynamicLightNumber__FP13PolysetHeader` | `0x17ca40`, 212 | 300 randomised tables (arena host check); 3 mutations caught |
| `w12-light-model-p-line` | `P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf` | `0x181660`, 248 | 120 runs with stack-passed bytes and float register; 3 mutations caught |
| `w12-light-model-light-setup-ambient-and-dir-only` | `lightSetupAmbientAndDirOnly__FP6PacketG6Point3f` | `0x13df98`, 456 | 200 runs, hooked `worldQueryLightMap`; 4 mutations caught (a fifth changed nothing observable) |
| `w12-light-model-vi-point-light-ctor` | `__12VIPointLight` | `0x10c3950`, 80 | 5 runs, hooked base constructor; 3 mutations caught |
| `w12-light-model-anim-play-sound` | `animPlaySound__FPvP6Point3P9animSoundi` | `0x160490`, 460 | 400 runs, hooked `powf` and `soundPlaySound`; 5 mutations caught |
| `w12-light-model-find-lights-in-box-for-world` | `findLightsInBox_forWorld__FG3BoxPP5Lighti` | `0x13c6f8`, 540 | 300 randomised grids and boxes; 5 mutations caught |
| `w12-light-model-model-setup-attachment` | `modelSetupAttachment__FP6PacketP12_modelHeaderT1i` | `0x169528`, 800 | 500 randomised models, options and cursors, hooked lighting call; 11 mutations caught |
| `w12-light-model-p-extend-swoosh` | `P_ExtendSwoosh__FiG6Point3T1f` | `0x183ed0`, 428 | 500 randomised trail records and float inputs; 6 mutations caught |
| `w12-light-model-smooth-patch` | `smoothPatch__FP12_worldHeaderP16WaterPolysetData` | `0x178840`, 636 | 60 randomised patch tables incl. self-neighbour; 6 mutations caught (one more was equivalent); host cells from independent float32 arithmetic |
| `w12-light-model-data-light-grid` | `lightGrid` | `0x4f40f0` | bss object, 32,400 bytes; host extent/zero check |
| `w12-light-model-data-attachment-light-pos` | `attachmentLightPos` | `0x6e1d50` | bss object, 12 bytes established; host extent/zero check |
| `w12-light-model-data-lightning-sword-particle` | `lightningSwordParticle` | `0x7d63d0` | bss object, 80 bytes; host extent/zero check; initializer caveat below |

"Mutations" counts the deliberate source changes that made the paired run report a mismatch; a mutation that changed nothing observable is not counted. Two commit messages state the count loosely: the `findLightsInBox_forWorld` commit says six (five made the run fail; two attempted ones did not apply) and the `smoothPatch` commit says seven (six made it fail, one was equivalent). The numbers in this table are the right ones. Each paired run compared the return value where the function has one, every byte the call changed outside the stack, the logged arguments of hooked callees, and chosen output words (the packet cursor, a float result register, the generator state).

Observed facts worth repeating:

- `P_LightningIsLive` returns 1 only when the word at record+1828 is **non-zero**. The branch delay slot of the last test loads 1 and the fall-through returns 0, so reading it as "must be zero" is wrong. The first draft of the source had exactly that error and the paired run caught it.
- The decimal literal `0.01666666567325592f` does not round to the original constant in the reviewed compiler: it compiled to `0x3c888887`, one unit short of the original `0x3c888888`. (`1.0594630241394043f` happened to match.) `animPlaySound` and `smoothPatch` therefore write their float constants as hexadecimal float literals (`0x1.11111p-6f`, `0x1.0f38f8p+0f`, `0x1.999998p-4f`). The original constants are the floats `0x3c888888`, `0x3f879c7c` and `0x3dcccccc`.
- `modelSetupAttachment` writes the packet's words at +0, +4 and +8 (cursor, head tag, GIF-style block). The caller `modelDrawAttachment` keeps a 16-byte packet on its stack, so +8 is inside it; the authored caller's two-word struct declaration is only a view of the first two words.
- `worldQueryLightMap` ignores both arguments and returns 1.0f. The lighting routines that call it scale by that value, so in this build the world light map has no effect.
- `modelDrawCharacter`'s mask is a 64-bit argument (`long long` in the authored declaration; the symbol's `l`) shifted with doubleword instructions.
- `smoothPatch` first term assigns, later terms add: the first neighbour's contribution is set rather than added to 0.0, which only matters for -0.0 and does not change the stored integer.
- `getDynamicLightNumber` reads ph[4] a second time after clearing the old owner's number. Clearing keeps bit 31, so the second read cannot differ and the authored source does not distinguish them.

## Re-verification of units authored in earlier waves

- `w11-rend-a-p-start-lightning` (`P_StartLightning__FG6Point3T0iiffUi`, 684 bytes at `0x1848c8`) was not selected at integration because its host check "only builds as 32-bit-pointer x86_64". It does build and pass here as native arm64 when compiled with `-DNORRATH_HOST_ARENA` (the check already supplies `norrath_host_pointer` and `norrath_host_address`). I also ran it against the original: 200 randomised scenarios with hooked `P_StartTrail`, `P_ExtendTrail`, `P_DeleteTrail` and `startFractalLine` matched return value, logged callee arguments, written bytes and the two updated globals; five source mutations (the count clamp, `frameNm + 10`, the `netTrueStep` test, the handle shift, a `P_StartTrail` argument) each made it fail. It needs a plan entry with header closure `sources/w11-rend-a-common/rend.hpp` and the host check should be built with `-DNORRATH_HOST_ARENA`. I did not touch that directory.
- `w10-render-b-data-halo-color`, `-model-globals`, `-light-slots`, `-light-info`, `-lock-alpha` and `-inventory-textures` were not selected for lacking a committed check. `w12-light-model-data-check/host_check.cpp` includes them unchanged and checks extents and initial values. A private comparison of the compiled objects with the image: `haloColor` (64 bytes) and `globalShadowBrightness` (0x3e800000) match byte for byte, `halos` relocates against `defaultHalos` whose address is the word stored in the image (`0x0034c018`); `lightInc`, `lightIntensity`, `lightStart`, `marble` and `invHeader` are zero in the image but sit in `.data` there, while the authored C++ places them in `.bss`. That is a layout difference, not a value difference.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values after the current tip; `default_authored_inputs` grows by the number of entries appended. Profile `authored_cpp`. `host_check.cpp` files are not inputs. Body sizes come from the reviewed EE g++ build of exactly these files. No private object hash is claimed.

Mine (23 entries):

```json
{"emitted_functions":[{"compiled_body_bytes":608,"name":"animPlaySound__FPvP6Point3P9animSoundi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_anim_play_sound.o","profile":"authored_cpp","sha256":"87a0e54f1f204f4651e99018ca528fdd07588a51913af572825c91c71f0a2d18","source":"sources/w12-light-model-anim-play-sound/anim_play_sound.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_data_attachment_light_pos.o","profile":"authored_cpp","sha256":"073e5a2c2cf2e381139df476c1630ab0926394dbb5e0608016cf6ef4a3b66fa5","source":"sources/w12-light-model-data-attachment-light-pos/attachment_light_pos.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_data_light_grid.o","profile":"authored_cpp","sha256":"d87dd2a43ef76f6f81c0935a67bf77158513c34deedb815004e19dc24d5c37b2","source":"sources/w12-light-model-data-light-grid/light_grid.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_data_lightning_sword_particle.o","profile":"authored_cpp","sha256":"fda64951339b1b439dc1da5d01534c58771ddb078baa75487fa400cd49f0be34","source":"sources/w12-light-model-data-lightning-sword-particle/lightning_sword_particle.cpp"}
{"emitted_functions":[{"compiled_body_bytes":544,"name":"findLightsInBox_forWorld__FG3BoxPP5Lighti"}],"header_closure":["sources/w11-rend-a-common/rend.hpp"],"kind":"independently_authored_source","object_name":"w12_light_model_find_lights_in_box_for_world.o","profile":"authored_cpp","sha256":"e141468d77f8d71ad4a51e64249d5da02e76fc149a1819cdd14cc3554c7ef1e9","source":"sources/w12-light-model-find-lights-in-box-for-world/find_lights_in_box_for_world.cpp"}
{"emitted_functions":[{"compiled_body_bytes":212,"name":"getDynamicLightNumber__FP13PolysetHeader"}],"header_closure":["sources/w11-rend-a-common/rend.hpp"],"kind":"independently_authored_source","object_name":"w12_light_model_get_dynamic_light_number.o","profile":"authored_cpp","sha256":"cc2f07c31c749260f5dede6f9c76b2b8066ec8933f84b4f58c63d9b01f7ea597","source":"sources/w12-light-model-get-dynamic-light-number/get_dynamic_light_number.cpp"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"lightDisableVolumetricLighting__Fv"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_light_disable_volumetric.o","profile":"authored_cpp","sha256":"8c63cf6e20c23e54d6468126f02cba56545da127dda5324ef0076d556ac6a5de","source":"sources/w12-light-model-light-disable-volumetric/light_disable_volumetric.cpp"}
{"emitted_functions":[{"compiled_body_bytes":392,"name":"lightSetupAmbientAndDirOnly__FP6PacketG6Point3f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_light_setup_ambient_and_dir_only.o","profile":"authored_cpp","sha256":"d41d2c128c234d220d53d476850031c0e7f9332a27b8f697e67766cae2b5669f","source":"sources/w12-light-model-light-setup-ambient-and-dir-only/light_setup_ambient_and_dir_only.cpp"}
{"emitted_functions":[{"compiled_body_bytes":156,"name":"lightSetupAmbientOnly__FP6Packet"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_light_setup_ambient_only.o","profile":"authored_cpp","sha256":"eb8b95682fee69da32d4fe5598c244aeb1c11235fb5d98e9c4c55e8ddb67a586","source":"sources/w12-light-model-light-setup-ambient-only/light_setup_ambient_only.cpp"}
{"emitted_functions":[{"compiled_body_bytes":156,"name":"lightSetupNonDirectionalDirectional__FP6Packet"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_light_setup_non_directional_directional.o","profile":"authored_cpp","sha256":"078f1ec9380336b8d527b7b80f3670e6af169e84d3b0d3eeb94b811d1c043b82","source":"sources/w12-light-model-light-setup-non-directional-directional/light_setup_non_directional_directional.cpp"}
{"emitted_functions":[{"compiled_body_bytes":176,"name":"modelDrawCharacter__FP6PacketP12_modelHeaderli"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_model_draw_character.o","profile":"authored_cpp","sha256":"8294c9fb1526b09534edb60ae517d8711a809cc0d4b18e37bcc46456be82456f","source":"sources/w12-light-model-model-draw-character/model_draw_character.cpp"}
{"emitted_functions":[{"compiled_body_bytes":48,"name":"modelGetCollision__FP12_modelHeaderi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_model_get_collision.o","profile":"authored_cpp","sha256":"1e012aee7ed6234197532746f5b5376986efecc9507013e0c8d3e275e987e5d3","source":"sources/w12-light-model-model-get-collision/model_get_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":48,"name":"modelGetPrimaryBone__FP12_modelHeaderi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_model_get_primary_bone.o","profile":"authored_cpp","sha256":"a00902d49f3cdb5f694f0f1f36d84dadabc0657b18335a423761fb360453dafe","source":"sources/w12-light-model-model-get-primary-bone/model_get_primary_bone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":128,"name":"modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_model_get_weapon_trail_pos.o","profile":"authored_cpp","sha256":"1d7dcd7afdd6c9bd3d3773694b97d828e9051eb96e211839f3c4c5f236a5fe9c","source":"sources/w12-light-model-model-get-weapon-trail-pos/model_get_weapon_trail_pos.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"modelSetAttachmentFlip__Fv"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_model_set_attachment_flip.o","profile":"authored_cpp","sha256":"4d8e1cc524d3adc3b3acd243fafc9fc730c4651c9225ea0491833dada206ce08","source":"sources/w12-light-model-model-set-attachment-flip/model_set_attachment_flip.cpp"}
{"emitted_functions":[{"compiled_body_bytes":840,"name":"modelSetupAttachment__FP6PacketP12_modelHeaderT1i"}],"header_closure":["sources/w11-rend-a-common/rend.hpp"],"kind":"independently_authored_source","object_name":"w12_light_model_model_setup_attachment.o","profile":"authored_cpp","sha256":"0e574865a9eb9ef00a9684bb8f6e348f804407a8e6bb2e0788ccb09181cddd5f","source":"sources/w12-light-model-model-setup-attachment/model_setup_attachment.cpp"}
{"emitted_functions":[{"compiled_body_bytes":372,"name":"P_ExtendSwoosh__FiG6Point3T1f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_p_extend_swoosh.o","profile":"authored_cpp","sha256":"83438dd7c1cc3b7093dfc4d2cbb72c85e6dfd87840e0423d0233ea99892e3d3d","source":"sources/w12-light-model-p-extend-swoosh/p_extend_swoosh.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"P_LightningIsLive__Fi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_p_lightning_is_live.o","profile":"authored_cpp","sha256":"c146f98ce4d30051e86c11dd73352ee7d8088103b8d8b9ef5d94fe05e84a4b3b","source":"sources/w12-light-model-p-lightning-is-live/p_lightning_is_live.cpp"}
{"emitted_functions":[{"compiled_body_bytes":152,"name":"P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_p_line.o","profile":"authored_cpp","sha256":"bf9800d2b7a3a490b69114df5f184004bfb3e2f8503a4a4e8020a5614a04b20d","source":"sources/w12-light-model-p-line/p_line.cpp"}
{"emitted_functions":[{"compiled_body_bytes":968,"name":"smoothPatch__FP12_worldHeaderP16WaterPolysetData"}],"header_closure":["sources/w11-rend-a-common/rend.hpp"],"kind":"independently_authored_source","object_name":"w12_light_model_smooth_patch.o","profile":"authored_cpp","sha256":"f665a14e6149498721a0b320b4d66186795b2afef6f05db30a9b84882d7beee9","source":"sources/w12-light-model-smooth-patch/smooth_patch.cpp"}
{"emitted_functions":[{"compiled_body_bytes":128,"name":"soundPan2D__FiP6Point3f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_sound_pan_2d.o","profile":"authored_cpp","sha256":"4761ec3dd919f24baf8c0a2f3f311900120301625b7d14da559b343110e0ebd1","source":"sources/w12-light-model-sound-pan-2d/sound_pan_2d.cpp"}
{"emitted_functions":[{"compiled_body_bytes":68,"name":"__12VIPointLight"}],"header_closure":["sources/w11-rend-a-common/rend.hpp"],"kind":"independently_authored_source","object_name":"w12_light_model_vi_point_light_ctor.o","profile":"authored_cpp","sha256":"05f5da5b4eb7a03849b66d5b8b54545ac5cf54580899f8cf7587ebced13373f5","source":"sources/w12-light-model-vi-point-light-ctor/vi_point_light_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"worldQueryLightMap__FP12_worldHeaderG6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w12_light_model_world_query_light_map.o","profile":"authored_cpp","sha256":"92d852b39e5f57ce32edf91cb59ba35736c25b157786c93086d0e8455eafd7e0","source":"sources/w12-light-model-world-query-light-map/world_query_light_map.cpp"}
```

Earlier-wave units that close names in my list and need selecting (7 entries; `w11-rend-a-p-start-lightning` also needs its host check built with `-DNORRATH_HOST_ARENA`):

```json
{"emitted_functions":[{"compiled_body_bytes":664,"name":"P_StartLightning__FG6Point3T0iiffUi"}],"header_closure":["sources/w11-rend-a-common/rend.hpp"],"kind":"independently_authored_source","object_name":"w11_rend_a_p_start_lightning.o","profile":"authored_cpp","sha256":"ec9fd7ab5722635cf7732be4a823b11354404d57dc9e1da8aede4267cd18cc6c","source":"sources/w11-rend-a-p-start-lightning/p_start_lightning.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w10_render_b_data_halo_color.o","profile":"authored_cpp","sha256":"9a12d6fd71420ad10923ddd6dd6a37541437d18e02e36898daab8748db3e428c","source":"sources/w10-render-b-data-halo-color/data_halo_color.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w10_render_b_data_inventory_textures.o","profile":"authored_cpp","sha256":"b7c8662d2e84a7d0227c54bdafae2eebf45dd41f503160f5510413cc6535e757","source":"sources/w10-render-b-data-inventory-textures/data_inventory_textures.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w10_render_b_data_light_info.o","profile":"authored_cpp","sha256":"cd803a4c2eb225e89ce468368a9c2c8a7694d7d79d8b5de4f141024159f62844","source":"sources/w10-render-b-data-light-info/data_light_info.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w10_render_b_data_light_slots.o","profile":"authored_cpp","sha256":"b8da875a196430e9c1b900acfdafce6d53db28b58b22be27c47eb6e9f240b479","source":"sources/w10-render-b-data-light-slots/data_light_slots.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w10_render_b_data_lock_alpha.o","profile":"authored_cpp","sha256":"285891ab0ff47876c9627ac743c6efbb5b1085bbd1fd8e051ee1713762c40212","source":"sources/w10-render-b-data-lock-alpha/data_lock_alpha.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w10_render_b_data_model_globals.o","profile":"authored_cpp","sha256":"5d1e5e1c26e8eb3b09ff5f37a766139fd859fb32eb06f3ec250dad203b2d4763","source":"sources/w10-render-b-data-model-globals/data_model_globals.cpp"}
```

The plan's `files` map also needs the source hashes of every entry above (the `sha256` field) and nothing else: only `sources/w11-rend-a-common/rend.hpp`, already present, is a header.

I also had to add these inputs to `snapshot.json` for a private build; I changed no shared file. The snapshot entries are the same objects with a `component` field (the directory name).

## Imports closed

A private full partial build (a private copy of `experimental/` with the plan and snapshot extended by the entries above, nothing shared edited) of 5,143 authored inputs linked with **798** unresolved imports against the shared branch's **827**. 33 of the 35 names in the task list are closed by it:

`P_ExtendSwoosh__FiG6Point3T1f`, `P_LightningIsLive__Fi`, `P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf`, `P_StartLightning__FG6Point3T0iiffUi`, `__12VIPointLight`, `animPlaySound__FPvP6Point3P9animSoundi`, `attachmentLightPos`, `findLightsInBox_forWorld__FG3BoxPP5Lighti`, `getDynamicLightNumber__FP13PolysetHeader`, `globalShadowBrightness`, `haloColor`, `halos`, `lightDisableVolumetricLighting__Fv`, `lightGrid`, `lightInc`, `lightInfo`, `lightIntensity`, `lightSetupAmbientAndDirOnly__FP6PacketG6Point3f`, `lightSetupAmbientOnly__FP6Packet`, `lightSetupNonDirectionalDirectional__FP6Packet`, `lightStart`, `lightningSwordParticle`, `lockAlpha`, `marble`, `modelDrawCharacter__FP6PacketP12_modelHeaderli`, `modelGetCollision__FP12_modelHeaderi`, `modelGetPrimaryBone__FP12_modelHeaderi`, `modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1`, `modelSetAttachmentFlip__Fv`, `modelSetupAttachment__FP6PacketP12_modelHeaderT1i`, `smoothPatch__FP12_worldHeaderP16WaterPolysetData`, `soundPan2D__FiP6Point3f`, `worldQueryLightMap__FP12_worldHeaderG6Point3`

The build ran before two comment-only corrections to header comments (the extents in `model_get_primary_bone.cpp` and `smooth_patch.cpp`); the entries above carry the final hashes, and no code changed. Of those, 10 depend on the earlier-wave units listed above being selected (the nine data names `globalShadowBrightness`, `haloColor`, `halos`, `lightInc`, `lightInfo`, `lightIntensity`, `lightStart`, `lockAlpha`, `marble`, and `P_StartLightning__FG6Point3T0iiffUi`).

Still open from the task list: `megatileGenerate__FP12_worldHeader`, `waterStep`.

- `megatileGenerate__FP12_worldHeader` (19,680 bytes at `0x25e3a8`, about 4,900 instructions): not attempted beyond reading its call structure. It contains `setjmp`/`longjmp` (about 15 `longjmp` sites), soft-double helper calls (`litodp`, `dpmul`, `dpadd`, `dptoli`, about 50 sites), `scePrintf` and `sprintf` diagnostics, `_engineCheckStack`/`memCheckHeapReal` checks at 38 sites, calls into the `MGWorld` placement code, `doBigUnitPlacement`, `makeLayer`, `doWatertile`, `getSegmentBlocks` and `texAddAlwaysLoadedPage`, and a number of `strcmp`/`strncmp` calls. A reconstruction needs the layout of `MGWorld`, `MegatileInst`, `EdgingGeneral`, `collisionPart` and the world header, none of which is established, and a paired run would need stubs for dozens of callees. This is a function for the megatile owner with those layouts, not one to attempt in a single sitting.
- `waterStep` (the label at `0x187798`, about 0x2c0 bytes up to `waterLoop` and `bailo`): it is hand-written R5900 multimedia code (`lq`/`sq`, `paddsh`, `psubsh`, `psrah`, `psllh`, `qfsrv`, `pand`) that runs on scratchpad memory at `0x70003f00`, apparently an eight-lane signed-halfword water simulation step (inferred from the instruction mix, not tested). It has no C++ expression that I could find, and an inline-assembly copy would be an assembly dump, which the repository rules do not credit. The private interpreter has no multimedia opcodes either, so there is no way to compare it. It stays an import until the repository decides how to represent hand-written vector code.

## Imports opened

Symbols the new objects reference but do not define, taken from the same build, that were not unresolved before:

`__8VISprite`, `_vt$12VIPointLight`, `lineBuffer`, `linePos`, `startFractalLine__FP4Boltfi`

`__8VISprite` and `_vt$12VIPointLight` come from `__12VIPointLight`; `lineBuffer` from `P_Line`; `linePos` and `startFractalLine__FP4Boltfi` from the earlier-wave `P_StartLightning`. This package defines none of them and I have not checked which package owns each (`lineBuffer` is the 128 x 36-byte table at `0x006f4880`, `nmLines` already has a unit in `particles-nm-lines`). Two items need a decision:

- `lightningSwordParticle`: the shared branch's `w8-fx-particles-init-fx` unit declares its own **file-local** `static unsigned char lightningSwordParticle[80]` and fills it with the particle-definition constructor, while `w11-rend-a-fx-run-weapon` reads an `extern` of that name. My data unit defines the global (80 bytes, zero) so the name resolves, as `w11-data-a` did for `ballSparkParticle`, but the initializer still fills a different object. The initializer's static must become this global for the pair to mean anything.
- `lightSetupForCharacter__FP6PacketG6Point3` is declared in my `modelSetupAttachment` as taking `(void*, const Point3*)`. The original passes the address of `attachmentLightPos` itself; the authored definition takes the Point3 by value, which on the EE is also a pointer to a caller copy, so the two agree at the call.

## Authored but never executed

Nothing ran on the EE or in the game. Everything above ran in the private R5900 interpreter (integer, single float; no multimedia instructions) against the pinned image from identical memory snapshots with identical stub callees, plus host contracts on the build machine. Hooked callees stand in for `lightSetupForCharacter`, `worldQueryLightMap`, `pan`, `soundChangeVolume`, `soundPlaySound`, `powf`, `__8VISprite`, `P_StartTrail`, `P_ExtendTrail`, `P_DeleteTrail` and `startFractalLine`; their behaviour is not verified here. The vtable address stored by `__12VIPointLight` is an unresolved import (`_vt$12VIPointLight`) in this build.

## Unknown

- What `modelOption` bit 0x80 means to the attachment drawing (the three negated matrix rows look like a mirror), what bit 0x40's lighting is lit from, and who clears either.
- What the tag and register words written by `modelSetupAttachment` and `modelDrawCharacter` select, and why `modelDrawCharacter` never draws part `p` (bit `p` of the mask is cleared).
- The meanings of the animSound bytes at +10..+14, of `pitchTab`, and of the three literals 360 and 840 in `soundPan2D`.
- The roles of the second grid in `smoothPatch`, of the trail ring's +6 halfword in `P_ExtendSwoosh`, of the second radius at light+36 against +28, and of the eight bytes in `P_Line`.
- Whether `findLightsInBox_forWorld` is only reached from world rendering (it is a local symbol in the original).
- What else lives in the 48 bytes at `attachmentLightPos` (12 are established), and the true internal layouts of the 60-byte patch records, the 1844-byte bolt records and the 1040-byte trail records.
- The zero-division behaviour of `animPlaySound` for a record byte of -1 at +14 (the reviewed compiler inserts a trap the original does not have; the paired runs avoid that value).

## Summary

- Imports closed: 33 of 35 names (listed above).
- Imports opened: 5 (listed above).
- Build-plan entries needed: 23 of mine plus 7 earlier-wave units; none were added to shared files.
- Authored but never executed: everything on the EE or in the game.
- Unknown: see above. Open: `megatileGenerate__FP12_worldHeader` and `waterStep`.
