# w11-rend-a: rendering, lighting, model and camera (part 1 of 2)

Branch `agents/w11-rend-a`. Private reference executable `work/private/SLUS_205.65`
(SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`); symbol names,
addresses and extents are the image's own symbol table. Nothing here ran on hardware or inside
the game, and no unit is claimed as an original-byte match.

An earlier interrupted run of this package had already committed nineteen units (camera getters,
trail and lightning helpers, `drawTexture`, `drawWorldDynamicLight`, two weapon effects, the animation
helpers and two data objects) and left `analDecompress` uncommitted. That draft was reviewed (paired
run repeated, host check added, mutations run) and committed first. Everything below is one unit per
directory `experimental/sources/w11-rend-a-*`, each with a `host_check.cpp` (a host test, not a plan
input) and one commit.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable C++
  and compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0). Every unit compiled. A
  private partial build with all units of this package appended to a copy of the shared plan linked
  once; its result is summarised under "Private partial build".
- Every unit has a host check built with the host C++ compiler (`-ffp-contract=off`) and exits 0.
- Private paired runs (a small R5900 interpreter outside git): the original function and the freshly
  compiled unit run from the same memory snapshot with identical stub callees; the call logs
  (argument registers, float registers, bytes behind pointer arguments) and every changed byte outside
  the stack are compared. A halted run counts as a failure. Scenario counts, coverage and the
  mutation checks are in each commit message. Mutated copies of each unit (changed constants,
  comparisons, counts, offsets) were run; survivors are named in the commit messages.
- Where the original reads `gp`-relative float constants, the exact bit patterns were read from the image
  and written as C++17 hex float literals (the EE compiler truncates decimal literals toward zero).
- NaN float inputs are excluded from the paired scenarios: the compiler treats float compares as NaN-free
  (it may swap `!(a < b)` into `b <= a`), and the R5900 has no NaN. Zero-length vectors that the original
  normalises with `rsqrt.s` are excluded for the same reason.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Units added in this run (observed, inferred, unknown)

Each source file states its own observed/inferred/unknown split in its leading comment.

| Symbol | Address, extent | Notes |
|---|---|---|
| `analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii` | 0x00133f60, 1100 | bit-reader driven 8-bit sample decoder; 150 paired runs, 273/275 words covered (two branch-likely delay slots not reached) |
| `lightAdd__FP5Light` | 0x0013d230, 200 | link a light into the 90 by 90 `lightGrid` or the overflow list `offTheGrid` (0x00486848) |
| `checkYawReset__6Camera` | 0x0026ed68, 296 | yaw re-aim when the stick is centred; calls `fastestRotDirection` |
| `findLightsAtPoint__FG6Point3PP5Lighti` | 0x0013c528, 464 | collects lights within 300 units of a point from the grid cells around it |
| `DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState` | 0x0020d520, 1864 | web lines fanning between two points, optionally on model shadow points |
| `drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi` | 0x00178ac0, 1928 | recycles a water record, builds a GS/VIF packet, occasional rain splash |
| `drawWorldObject__FP12_worldHeaderP13PolysetHeaderi` | 0x00179248, 2292 | resource lookup, animation state, packet, lights, `modelDrawCharacter` |
| `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34` | 0x001dc2c0, 10448 | weapon trail, blood spray, halo sprites, lightning, ice and flame effects |
| `evaluateAnimation__FP14AnimationStateb` | 0x00161df0, 2476 | pose blending into a matrix record; see the VU0 limitation below |

The units from the earlier run are `GetToggle__6Camera`, `SetNoTargetUpdates__6Camerai`,
`GetLastTarget__6Camera`, `clampZoomMag__6Camera`, `gameCloseToCamera__FP6Point3`,
`fastestRotDirection__FssPi`, `P_DeleteTrail__Fi`, `animRandomizeTime__FP14AnimationState`,
`drawTexture__FR6Packetiiiiiiiii`, `P_RevolveTrail__FiG6Point3N31i`, `P_MoveLightning__FiG6Point3T1ii`,
`P_StartLightning__FG6Point3T0iiffUi`, `__4LineP4LineT1i`, `animRemove__FP14AnimationStateP15AnimationHeaderf`,
`fxWeaponToSparkles__...`, `fxWeaponToDust__...`, `drawWorldDynamicLight__FP13PolysetHeader`,
`crystalTex` and `halfFrustumMode`. Their checks are in their commit messages.

Findings worth keeping (all observed in the image unless marked inferred):

- `T1` in these mangled names is the type of the *second* parameter (index 1), not the first: in
  `fxRunWeapon` it is a second `_modelHeader*`, and `T0` in `DrawWebSplay` is the first `Point3`. Passing
  structs by value gives the callee a pointer to a copy; floats go in `f12`/`f13` without using integer
  argument registers, which shifts the stack arguments.
- `evaluateAnimation` ends with VU0 macro-mode code: `lqc2`, `qmtc2`, `ctc2`, `vcallms 0x10` and
  `vcallms 0x20`, `qmfc2.i`, plus the MMI `pextlh`, `psraw`, `ppach`. The authored unit uses `lqc2`,
  `ctc2`, `vcallms` and `sqc2` inline assembly (a host build uses a software stand-in). What programs
  0x10 and 0x20 compute is unknown; the paired run replaces `vcallms` with a stand-in that only hashes
  its inputs, so it proves the same registers reach the programs in the same order, not what the
  programs do. (inferred: they build three 4-halfword matrix rows per bone from the quaternion, position,
  a scale vector and a parent index.)
- `evaluateAnimation`'s second angle-wrap loop (`while (w1 - w2 < -pi) w2 += 2 pi`) can never run after the
  first loop, and would not terminate if it did; it is reproduced as found. The original's `lq`/`sq`
  spills around calls are not reproduced.
- `drawWaterPatch` and `drawWorldObject` both end with the same tag rewrite as `drawWorldPolyset`
  (ID 1 to 6). In `drawWaterPatch` the tag value can never have ID 1, so that branch is unreachable.
- `fxRunWeapon` contains a clamp `if (t < 0) t = 0` after `t = 2 |v|` that cannot fire.
- The mod table of an `item::description` is four 8-byte entries from +36 (halfword type, halfword
  value); types 24, 22 and 21 select the lightning, ice and flame sweeps. (inferred names)

## Private partial build

`docs/agents/w11-rend-a-plan-entries.py --private-root` wrote a plan copy outside git with the units of
this package appended to the shared selection; `tools/build_partial.py` compiled 4,482 sources and linked
once (no retries): 3,431 function definitions and 956 unresolved symbols, of which only the five data
objects above are in this package's scope. This is a relocatable partial object, not an executable.

## Imports closed

`analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii`, `GetLastTarget__6Camera`, `GetToggle__6Camera`, `SetNoTargetUpdates__6Camerai`, `checkYawReset__6Camera`, `clampZoomMag__6Camera`, `DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState`, `__4LineP4LineT1i`, `P_DeleteTrail__Fi`, `P_MoveLightning__FiG6Point3T1ii`, `P_RevolveTrail__FiG6Point3N31i`, `P_StartLightning__FG6Point3T0iiffUi`, `animRandomizeTime__FP14AnimationState`, `animRemove__FP14AnimationStateP15AnimationHeaderf`, `crystalTex`, `drawTexture__FR6Packetiiiiiiiii`, `drawWaterPatch__FP12_worldHeaderP13PolysetHeaderi`, `drawWorldDynamicLight__FP13PolysetHeader`, `drawWorldObject__FP12_worldHeaderP13PolysetHeaderi`, `evaluateAnimation__FP14AnimationStateb`, `fastestRotDirection__FssPi`, `findLightsAtPoint__FG6Point3PP5Lighti`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi`, `fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi`, `gameCloseToCamera__FP6Point3`, `halfFrustumMode`, `lightAdd__FP5Light`.

## Imports in scope that stay open

`globalShadowBrightness`, `haloColor`, `halos`, `lightInc`, `lightInfo`.

`evaluateAnimation__FP14AnimationStateb` is authored and no longer appears in the private build's unresolved
list. The five data objects (`globalShadowBrightness`, `halos`, `haloColor`,
`lightInc`, `lightInfo`) are already defined by the existing `w10-render-b-data-*` sources, which are not yet
in the shared build plan; defining them again here would clash once those are selected, so they were not
authored. (`lightInfo` is `float[96]` there, `lightInc` `float[4]`, `haloColor` `int[4][4]`.)

## Imports opened

Symbols the new and earlier units of this package reference without defining (`nm -u` of the compiled
units). Still unresolved after the private build:

`P_ExtendSwoosh__FiG6Point3T1f`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `P_LightningIsLive__Fi`, `P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf`, `addEffect__FiG6Point3`, `emergencyResult`, `extractRotation__FP4Quat`, `getDynamicLightNumber__FP13PolysetHeader`, `iceSwordParticle`, `lightDisableVolumetricLighting__Fv`, `lightGrid`, `lightSetupAmbientAndDirOnly__FP6PacketG6Point3f`, `lightSetupAmbientOnly__FP6Packet`, `lightSetupForCharacter__FP6PacketG6Point3`, `lightSetupNonDirectionalDirectional__FP6Packet`, `lightningSwordParticle`, `linePos`, `makeQuaternion__FP4Quatsfff`, `modelDrawCharacter__FP6PacketP12_modelHeaderli`, `modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf`, `modelGetWeaponTrailPos__FP12_modelHeaderP6Point3T1`, `netTrueStep`, `newFlameSwordParticle1`, `newFlameSwordParticle2`, `newFlameSwordParticle3`, `newIceSwordParticle1`, `newIceSwordParticle2`, `particleWeaponBlood`, `particleWeaponEcto`, `particleWeaponSlime`, `quatTransform__FP6Point3P4QuatT0`, `smoothPatch__FP12_worldHeaderP16WaterPolysetData`, `startFractalLine__FP4Boltfi`, `waterList`, `waterListBack`, `waterTexture`.

Defined by other selected sources (resolved in the private build): `DynamicLightArray`, `P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_ExtendTrail__FiG6Point3`, `P_StartTrail__Fiifiiiii`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animLoadBoneMatricies__FP6PacketP14AnimationState`, `animResetFramestates__FP14AnimationState`, `arList`, `arListBack`, `bolt`, `drawAddToList__FPviP8_textureP11_drawRecordi`, `drawFrameNm`, `droplet`, `eRandState`, `ef1Noise__Fi`, `evalHermite__Ffffff`, `floorf`, `frameNm`, `fxDust`, `g_BloodLevel`, `gameIsPaused__Fv`, `glistp`, `haloTexture2`, `initializeMe__4Line`, `interfaceMode`, `isin__Fi`, `levelFlags`, `lightHaloTexture`, `lightResetOptionalParams__Fv`, `lightSetAmbientColor__FPs`, `lumpFindResource__FPcT0`, `matIdent__FP8Matrix34`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `matMul__FP8Matrix34N20`, `matMul__FP8Matrix44P8Matrix34T0`, `matTransform__FP6Point3P8Matrix34T0`, `memAlloc__FiPc`, `modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `offTheGrid`, `pad`, `player`, `quatNormalize__FP4Quat`, `rainSplashParticle`, `scanMask`, `sprintf`, `strchr`, `strcpy`, `trail`, `trailSequenceNm`, `updateAnimation__FP13animComponent`, `worldAnimation`, `worldCharViewMatrix`, `worldFindHeight__FP12_worldHeaderG6Point3f`, `worldPerturbWater__FP12_worldHeaderG6Point3ff`, `worldTileEntityLightScale`, `worldViewCenter`, `worldWaterAlpha`, `worldWaterAlpha2`, `worldWaterColor`, `worldWaterColor2`, `worldWaterWavyness`, `worldWaterWavyness2`.

New data imports that need a defining unit from their owners: `lightGrid` (32,400 bytes at 0x004f40f0,
see `floor-storage`), `offTheGrid` (two image symbols of that name, 0x00486848 is the one used),
`interfaceMode`, `levelFlags`, `pad`, `player`, `worldAnimation`, the water globals and `waterList` /
`waterListBack`, `arList` / `arListBack`, the sword and weapon `ParticleDef` objects.

## Build-plan entries needed

28 entries; the script `docs/agents/w11-rend-a-plan-entries.py --toolchain /path/to/ee`
reprints them with the current source hashes and, with `--private-root DIR`, writes a private plan copy
outside git. Do not trust the hashes below after any source edit; rerun the script.

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_anal_decompress.o",
  "profile": "authored_cpp",
  "sha256": "e0a422f95943752e08df717cd405e0a8620c6222a54d4170b1ff18cfd4afaddd",
  "source": "sources/w11-rend-a-anal-decompress/anal_decompress.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_anim_randomize_time.o",
  "profile": "authored_cpp",
  "sha256": "ffd41b4cae2f3b715ed3c48e2a9f6a3055c9d791b3c095ad7677ac5c2be3db3d",
  "source": "sources/w11-rend-a-anim-randomize-time/anim_randomize_time.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_anim_remove.o",
  "profile": "authored_cpp",
  "sha256": "e8e943fd7426cdfbece1f25499a167f642e0513395ba38edccc7706d1e3a3bb1",
  "source": "sources/w11-rend-a-anim-remove/anim_remove.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_check_yaw_reset.o",
  "profile": "authored_cpp",
  "sha256": "7a505a7daf28676c7dac59d7fe4268f2babb7ac81b8a6e79c97f6cd54d72305c",
  "source": "sources/w11-rend-a-check-yaw-reset/check_yaw_reset.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_clamp_zoom_mag.o",
  "profile": "authored_cpp",
  "sha256": "daee980098b983f311f3077858133cf0cbcb1bfc8cffd1d72bcb3194610181f9",
  "source": "sources/w11-rend-a-clamp-zoom-mag/clamp_zoom_mag.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_data_crystal_tex.o",
  "profile": "authored_cpp",
  "sha256": "a4e716647ea3f7aaffc1abd12f7c5649a03853c651a6552a268b5668eb3e9f23",
  "source": "sources/w11-rend-a-data-crystal-tex/data_crystal_tex.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_data_half_frustum_mode.o",
  "profile": "authored_cpp",
  "sha256": "6036899c4397eae516077ddc1abb911b69d4b10e1a49a0dd9b2480b1d47aea80",
  "source": "sources/w11-rend-a-data-half-frustum-mode/data_half_frustum_mode.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_draw_texture_9.o",
  "profile": "authored_cpp",
  "sha256": "ff1c8ef4effcb6e51bebc81b9dc826b33851c15a23dee4c9ed9360801e633d08",
  "source": "sources/w11-rend-a-draw-texture-9/draw_texture_9.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_draw_water_patch.o",
  "profile": "authored_cpp",
  "sha256": "aa58f669d8463a5a81bb17a5f0391acca2ed2565bf2462521261e25f7bb5e4a2",
  "source": "sources/w11-rend-a-draw-water-patch/draw_water_patch.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_draw_web_splay.o",
  "profile": "authored_cpp",
  "sha256": "a4ddef0311a86af3cfdcd7c490af47a1e7d90fa3b5e5161d4cf84fa56b0f727d",
  "source": "sources/w11-rend-a-draw-web-splay/draw_web_splay.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_draw_world_dynamic_light.o",
  "profile": "authored_cpp",
  "sha256": "8893ccf5987fcd88dc03725123fc5992bf222f291b04ce2225e2a6cddea714a6",
  "source": "sources/w11-rend-a-draw-world-dynamic-light/draw_world_dynamic_light.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_draw_world_object.o",
  "profile": "authored_cpp",
  "sha256": "084502223f216f5bff62e4268d4b79a6f70c9804e6ba1b04a8a2acbfde39170d",
  "source": "sources/w11-rend-a-draw-world-object/draw_world_object.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_evaluate_animation.o",
  "profile": "authored_cpp",
  "sha256": "b4b1f0757ef6519164b3cce6b37405d2d2ff79d99ac0d203a50140124c731862",
  "source": "sources/w11-rend-a-evaluate-animation/evaluate_animation.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_fastest_rot_direction.o",
  "profile": "authored_cpp",
  "sha256": "e2daf5187d6d743967acd31089e32705188ef04e19a87c21724f7e442a7bd15d",
  "source": "sources/w11-rend-a-fastest-rot-direction/fastest_rot_direction.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_find_lights_at_point.o",
  "profile": "authored_cpp",
  "sha256": "e60bee2d804d32179fa381488ef99ce2b02a03c0121e8a75dad27677c7cba805",
  "source": "sources/w11-rend-a-find-lights-at-point/find_lights_at_point.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_fx_run_weapon.o",
  "profile": "authored_cpp",
  "sha256": "a62ada4284a39f8365cb368408f4ccc7a4aa9924529810939618b3f0aa11f975",
  "source": "sources/w11-rend-a-fx-run-weapon/fx_run_weapon.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_fx_weapon_to_dust.o",
  "profile": "authored_cpp",
  "sha256": "bc864ee7363938aee264c3736435fb71f4f68bda7d51776b409b074b6ac0f750",
  "source": "sources/w11-rend-a-fx-weapon-to-dust/fx_weapon_to_dust.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_fx_weapon_to_sparkles.o",
  "profile": "authored_cpp",
  "sha256": "5327652eddda4e0c58f66a06c3aff4fde366a21b10ece3c774a0df101939213c",
  "source": "sources/w11-rend-a-fx-weapon-to-sparkles/fx_weapon_to_sparkles.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_game_close_to_camera.o",
  "profile": "authored_cpp",
  "sha256": "841512bf13c709014c956109a215f16680e21648ac59d0c87f430cdeb959d068",
  "source": "sources/w11-rend-a-game-close-to-camera/game_close_to_camera.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_get_last_target.o",
  "profile": "authored_cpp",
  "sha256": "ba397307e71f463e862c47ea2466be2363e718e40774b965b7f623ec63bd73c2",
  "source": "sources/w11-rend-a-get-last-target/get_last_target.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_get_toggle.o",
  "profile": "authored_cpp",
  "sha256": "07d8ee80ee12ca7557c843509b7f89d94ebf80f704a4c63d6698e1ef067e55c2",
  "source": "sources/w11-rend-a-get-toggle/get_toggle.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_light_add.o",
  "profile": "authored_cpp",
  "sha256": "e16e44d6cde3670a1e2a951c1c25886d1b55f10aa21f691fcc2831fa30fbbba8",
  "source": "sources/w11-rend-a-light-add/light_add.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_line_join_ctor.o",
  "profile": "authored_cpp",
  "sha256": "5d024a18ea8f037c99c23590679d2b26efa42453d6d1e417efb62d9dbaf576a1",
  "source": "sources/w11-rend-a-line-join-ctor/line_join_ctor.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_p_delete_trail.o",
  "profile": "authored_cpp",
  "sha256": "1b640c2c9151a1e49c15af49a1a0eb9b62bc4c34678f82005ce52ded4e694612",
  "source": "sources/w11-rend-a-p-delete-trail/p_delete_trail.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_p_move_lightning.o",
  "profile": "authored_cpp",
  "sha256": "e1da4906cb30bc4b27e027a2265e195a2172966908afd50f520a7e743a57d894",
  "source": "sources/w11-rend-a-p-move-lightning/p_move_lightning.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_p_revolve_trail.o",
  "profile": "authored_cpp",
  "sha256": "c0138035fbc7348add92faf36dee8ae818230985c2bc9a1c6ffbc32409edaf10",
  "source": "sources/w11-rend-a-p-revolve-trail/p_revolve_trail.cpp"
}
```

```json
{
  "header_closure": [
    "sources/w11-rend-a-common/rend.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "w11ra_p_start_lightning.o",
  "profile": "authored_cpp",
  "sha256": "ec9fd7ab5722635cf7732be4a823b11354404d57dc9e1da8aede4267cd18cc6c",
  "source": "sources/w11-rend-a-p-start-lightning/p_start_lightning.cpp"
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w11ra_set_no_target_updates.o",
  "profile": "authored_cpp",
  "sha256": "59c17c11bbfdbfe40331bf7952a742d15021e7e10d1cef6805f80118aed54fd9",
  "source": "sources/w11-rend-a-set-no-target-updates/set_no_target_updates.cpp"
}
```


## Authored but never executed

- Everything: no unit has run on the EE or in the original process. The paired runs executed the original
  and the authored code inside a private interpreter with stub callees; results depend on that stub
  behaviour.
- `evaluateAnimation`: VU0 programs 0x10 and 0x20 (see above), the DMA header word semantics, and
  records with 64 bones (the paired runs stop at 8).
- `drawWaterPatch`: records with null list neighbours (the original writes through them), and the tag
  branch that cannot be reached.
- `drawWorldObject`, `drawWaterPatch`, `DrawWebSplay`: all callee behaviour (lights, model drawing,
  shadow positions, `floorf`, `matMul`) is stubbed.
- `fxRunWeapon`: every callee is stubbed; `P_AddParticle` may read or change `eRandState` on the EE,
  which the stubs do not.
- `lightAdd`, `findLightsAtPoint`: grid memory is synthetic; real light records are not exercised.
- Boundary cases survive some mutants (an exact cell edge for `findLightsAtPoint`'s lower column bound,
  one-ulp constants in `fxRunWeapon`, 63 or 64 bones in `evaluateAnimation`); see the commit messages.

## Unknown

- Layouts of `GameObject`, `_modelHeader`, `PolysetHeader`, `AnimationState`, `fxWeaponState`,
  `item::description`, the water record and the object record beyond the offsets used in each source.
- What the numeric GS/VIF tags written by the packet builders mean, and the two VU0 programs.
- The meaning of most float constants (named only by address in the notes of each source).
- Where the data objects `lightGrid`, `offTheGrid` (second one at 0x00486890), `waterList` and the
  particle definitions are initialised.
