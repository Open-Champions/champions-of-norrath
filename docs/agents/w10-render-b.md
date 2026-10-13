# Animation, model, light and render part 2 of 4 (`w10-render-b`)

Branch `agents/w10-render-b`. Private executable `work/private/SLUS_205.65`; addresses and names are
the image's own symbol table, and nothing here ran on hardware or inside the game. Nothing derived
from the executable (disassembly, decompiler output, the executable itself) is committed.

## Method and checks performed

- Each function was read privately, rewritten as C++ and compiled alone with the reviewed
  `authored_cpp` flags (EE GCC 15.2.0). Every unit compiled on first or second try.
- Each function unit has a `host_check.cpp` (host compiler, `-ffp-contract=off`, recorded stubs for
  callees) that exited 0.
- Private paired runs (outside git, a small R5900 interpreter with the same snapshot and identical
  stub callees for the original and for my compiled function) compared the call log (argument
  registers, float registers, the bytes behind pointer arguments), changed memory outside the stack,
  and the saved eRandState where used. Scenario counts are in the commit messages. The two large
  functions had their instruction coverage measured: `fxRunCreatureShock` 451 of 451 words,
  `fxRunCreatureGlow` 236 of 237 (the one word is a nop). Mutated copies were run through the same
  comparison and detected (changed constants, changed comparison direction, changed scale).
- Test inputs for `short` parameters must be sign-extended 64-bit values like a real caller supplies;
  an early run with zero-extended values showed a false difference that was a harness artefact.
- Data units (below) were checked by compiling them and comparing symbol sizes with the image's
  symbol extents and the initialised bytes with the image's file bytes; they have no behaviour to test.
- `python3 -m unittest discover -s tools -p 'test_*.py'` was run at the end (see the final message).
- I did not run `tools/build_partial.py` on the full plan (the plan is not mine to edit).
- No byte match to the original is claimed. Compiled sizes differ from the original sizes.

## Functions (all complete, one commit each)

| Symbol | Address | Original extent |
|---|---|---|
| `matScale__FP8Matrix34f`, `matScale__FP8Matrix44f`, `matTrans__FP8Matrix44fff`, `matInvert__FP8Matrix34T0`, `matMakeCharViewMatrix__FP8Matrix44fffffff`, `matMakeWorldViewMatrix__FP8Matrix44fffffff`, `matPostRot__FP8Matrix33is` | earlier commits of this branch | see source headers |
| `lightSetColorScale__Ffff`, `lightSetPrimaryDirectional__FG6Point3Ps`, `lightSetSecondaryDirectional__FG6Point3Ps`, `lightSetSceneLights__FG6Point3iiiiiiiii` | earlier commits | see source headers |
| `hudLetterBox__Fv`, `hudGetFadeIconPos__Fii`, `getVifAndTex__FPcT0PP12_modelHeaderPP8_texture` | earlier commits | see source headers |
| `lightCharacterShadow__FG6Point3P11LightResult` | `0x0013d3c0` | 828 |
| `fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi` | `0x001dbbb0` | 1804 |
| `fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi` | `0x001db2d8` | 948 |

Observed facts for the three new functions are in each source header. Points worth repeating:

- `lightCharacterShadow` keeps the three smallest keys `(int)(dist^2 - radius^2) << 7 | index` by
  insertion, nulls list entry 31 only after the selection, publishes the three lights to
  `saveBestList` and sets `saveValid`, then writes three (x, y, strength) byte triples and two bytes
  derived from `scene` plus the constant 24 into the result.
- `fxRunCreatureShock` owns three 8-byte records at state+8 (two halfwords, then a lightning handle
  at +12+8i). With a handle present the generator is advanced once and the bolt rebuilt when
  `value < 20000`; otherwise moved. The generator is the same inline LCG as `eRand`.
- `fxRunCreatureGlow` never reads its state pointer or its first float.
- Inferred only: purposes (shadow-light selection, shock bolt, glow halo) from callers and callees.
- Unknown: `LightResult` size/meaning, `Light` layout beyond +28/+48, the roles of the float inputs
  of both fx functions, and the particle definition fields.

## Data (closed by data-only units, one commit per group)

| Symbol | Address | Size | Content |
|---|---|---|---|
| `globalShadowBrightness` | `0x484090` | 4 | float 0.25 |
| `halos` | `0x484084` | 4 | pointer to `defaultHalos` (`0x34c018`) |
| `haloColor` | `0x408570` | 64 | 255,200,100,0 twice, then zeros |
| `lightInc` | `0x4085b0` | 16 | zeros, used as four floats |
| `lightIntensity` | `0x4085c0` | 16 | zeros, four ints |
| `lightStart` | `0x37f960` | 36 | zeros. Object-infra also defines this object under a view name; link only one |
| `lockAlpha` | `0x486d08` | 4 | bss word |
| `lavaGoopParticle` | `0x8d56f8` | 80 | bss, extent to next symbol |
| `lavaSteamParticle` | `0x8d5748` | 80 | bss, extent to next symbol |
| `lightInfo` | `0x8d5e40` | 384 | bss, 96 floats |
| `invHeader` | `0x4848e0` | 4 | pointer word |
| `marble` | `0x4848ec` | 4 | pointer word |

The zero-initialised ones are file-backed `.data`/`.sdata` in the original and land in `.bss` here;
the same compromise other packages accepted. Small-data (gp-relative) placement is not reproduced.

## Imports closed

`fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi`,
`fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi`,
`getVifAndTex__FPcT0PP12_modelHeaderPP8_texture`, `globalShadowBrightness`, `haloColor`, `halos`,
`hudGetFadeIconPos__Fii`, `hudLetterBox__Fv`, `invHeader`, `lavaGoopParticle`, `lavaSteamParticle`,
`lightCharacterShadow__FG6Point3P11LightResult`, `lightInc`, `lightInfo`, `lightIntensity`,
`lightSetColorScale__Ffff`, `lightSetPrimaryDirectional__FG6Point3Ps`,
`lightSetSceneLights__FG6Point3iiiiiiiii`, `lightSetSecondaryDirectional__FG6Point3Ps`, `lightStart`,
`lockAlpha`, `marble`, `matInvert__FP8Matrix34T0`, `matMakeCharViewMatrix__FP8Matrix44fffffff`,
`matMakeWorldViewMatrix__FP8Matrix44fffffff`, `matPostRot__FP8Matrix33is`, `matScale__FP8Matrix34f`,
`matScale__FP8Matrix44f`, `matTrans__FP8Matrix44fff`.

Not closed here: `lightAdd__FP5Light`. It is already authored in `loop-grid-fx-light-add` (another
package), so I did not write a second copy.

## Imports opened (unresolved externs of my units, other packages own them)

`P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`,
`P_MoveLightning__FiG6Point3T1ii`, `P_StartLightning__FG6Point3T0iiffUi`, `ballSmokeParticle`,
`ballSparkParticle` (both have several same-named local symbols in the image; the ones used are
`0x7d6380` and `0x7d6330`), `charAmbient`, `charDirect`, `charDirect2`, `charDirectDir`,
`charDirectDir2`, `charScaleB/G/R`, `defaultHalos` (`0x34c018`, 16 bytes), `eRandState`,
`engineScreenSlide`, `fadeTimer`, `findLightsAtPoint__FG6Point3PP5Lighti`, `frameNm`,
`gameCloseToCamera__FP6Point3`, `haloTexture2`, `icos__Fi`, `isin__Fi`,
`lightResetOptionalParams__Fv`, `lumpFindResource__FPcT0`, `matInvert__FP8Matrix44T0`,
`matTransform34Vec__FP6Point3P8Matrix34T0`, `modelGetNmShadowPoints__FP12_modelHeader`,
`modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `npcLumpName`,
`rectangleDraw__FiiiiiUi`, `saveBestList` (`0x4fbf80`, three pointers), `saveValid` (`0x483ab8`, word),
`scene`, `sprintf`, `transformSceneLightTask__Fv`, `worldCameraMatrix`.
Most of these are already defined elsewhere; the ones I believe nobody defines are `saveBestList`,
`saveValid`, `ballSparkParticle`, `ballSmokeParticle` and `defaultHalos`.

## Build-plan entries needed

Entries for `experimental/build-plan.json` `inputs` (append in this order; the integrator assigns
`input_order`) and the matching `files` hashes are generated from the committed sources. The JSON
below is exact for the sources at the last commit of this branch. `files` needs one
`"sources/<dir>/<name>.cpp": "<sha256>"` per entry (the `sha256` field below).

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_halo_color.o",
  "profile": "authored_cpp",
  "sha256": "9a12d6fd71420ad10923ddd6dd6a37541437d18e02e36898daab8748db3e428c",
  "source": "sources/w10-render-b-data-halo-color/data_halo_color.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_inventory_textures.o",
  "profile": "authored_cpp",
  "sha256": "b7c8662d2e84a7d0227c54bdafae2eebf45dd41f503160f5510413cc6535e757",
  "source": "sources/w10-render-b-data-inventory-textures/data_inventory_textures.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_lava_particles.o",
  "profile": "authored_cpp",
  "sha256": "24b2fa42d858969747819bee88daf95271267ce18e6b7b99a924e9b155a82350",
  "source": "sources/w10-render-b-data-lava-particles/data_lava_particles.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_light_info.o",
  "profile": "authored_cpp",
  "sha256": "cd803a4c2eb225e89ce468368a9c2c8a7694d7d79d8b5de4f141024159f62844",
  "source": "sources/w10-render-b-data-light-info/data_light_info.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_light_slots.o",
  "profile": "authored_cpp",
  "sha256": "b8da875a196430e9c1b900acfdafce6d53db28b58b22be27c47eb6e9f240b479",
  "source": "sources/w10-render-b-data-light-slots/data_light_slots.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_lock_alpha.o",
  "profile": "authored_cpp",
  "sha256": "285891ab0ff47876c9627ac743c6efbb5b1085bbd1fd8e051ee1713762c40212",
  "source": "sources/w10-render-b-data-lock-alpha/data_lock_alpha.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "data_model_globals.o",
  "profile": "authored_cpp",
  "sha256": "5d1e5e1c26e8eb3b09ff5f37a766139fd859fb32eb06f3ec250dad203b2d4763",
  "source": "sources/w10-render-b-data-model-globals/data_model_globals.cpp",
  "emitted_functions": []
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "fx_run_creature_glow.o",
  "profile": "authored_cpp",
  "sha256": "c0d670ef406be948ba2b4257729abaf0c7de58ea5166dee419872f904ad1fb60",
  "source": "sources/w10-render-b-fx-run-creature-glow/fx_run_creature_glow.cpp",
  "emitted_functions": [
   {
    "name": "fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi",
    "compiled_body_bytes": 904
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "fx_run_creature_shock.o",
  "profile": "authored_cpp",
  "sha256": "1f40cf877738872487e81e2ea3714c46c06b450c4dd259bdec9689dd0e9dd2a3",
  "source": "sources/w10-render-b-fx-run-creature-shock/fx_run_creature_shock.cpp",
  "emitted_functions": [
   {
    "name": "fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi",
    "compiled_body_bytes": 1884
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "get_vif_and_tex.o",
  "profile": "authored_cpp",
  "sha256": "5b2531f4429801e194473e51bf23fc49185e127d7d2f64d98dfb14a523b1ca69",
  "source": "sources/w10-render-b-get-vif-and-tex/get_vif_and_tex.cpp",
  "emitted_functions": [
   {
    "name": "getVifAndTex__FPcT0PP12_modelHeaderPP8_texture",
    "compiled_body_bytes": 160
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "hud_get_fade_icon_pos.o",
  "profile": "authored_cpp",
  "sha256": "a189dcd3ba8abf054997bb6c6ffb73e4f81a4ab9cbfd827a5049e226806c3e92",
  "source": "sources/w10-render-b-hud-get-fade-icon-pos/hud_get_fade_icon_pos.cpp",
  "emitted_functions": [
   {
    "name": "hudGetFadeIconPos__Fii",
    "compiled_body_bytes": 324
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "hud_letter_box.o",
  "profile": "authored_cpp",
  "sha256": "83034241cdcc05cb0d6ca9bca7630857a92555ffd7daa6c3b12f8ef624c9bf84",
  "source": "sources/w10-render-b-hud-letter-box/hud_letter_box.cpp",
  "emitted_functions": [
   {
    "name": "hudLetterBox__Fv",
    "compiled_body_bytes": 84
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "light_character_shadow.o",
  "profile": "authored_cpp",
  "sha256": "04cfdd9bcef2da97bcfd520fb0f53fefa2772243b013a7110495a993ba713f04",
  "source": "sources/w10-render-b-light-character-shadow/light_character_shadow.cpp",
  "emitted_functions": [
   {
    "name": "lightCharacterShadow__FG6Point3P11LightResult",
    "compiled_body_bytes": 732
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "light_set_color_scale.o",
  "profile": "authored_cpp",
  "sha256": "fe6c4a23fdda4d842998ab633c694e00d2fda1a93227a19d756b2a84119b8177",
  "source": "sources/w10-render-b-light-set-color-scale/light_set_color_scale.cpp",
  "emitted_functions": [
   {
    "name": "lightSetColorScale__Ffff",
    "compiled_body_bytes": 628
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "light_set_primary_directional.o",
  "profile": "authored_cpp",
  "sha256": "9464d69fa40c9732074087b96957bf5725cbc3ed5a5d77b0a28de97aef3e3463",
  "source": "sources/w10-render-b-light-set-primary-directional/light_set_primary_directional.cpp",
  "emitted_functions": [
   {
    "name": "lightSetPrimaryDirectional__FG6Point3Ps",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "light_set_scene_lights.o",
  "profile": "authored_cpp",
  "sha256": "c320eccee1242242579dcf7ac331f9b0c322a2e47460c4cb648f11eec0033af5",
  "source": "sources/w10-render-b-light-set-scene-lights/light_set_scene_lights.cpp",
  "emitted_functions": [
   {
    "name": "lightSetSceneLights__FG6Point3iiiiiiiii",
    "compiled_body_bytes": 204
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "light_set_secondary_directional.o",
  "profile": "authored_cpp",
  "sha256": "e61e42d1f8b952fb195752bfb2a43c96178c2a664016aa2bf5cb50abe9f2bfd1",
  "source": "sources/w10-render-b-light-set-secondary-directional/light_set_secondary_directional.cpp",
  "emitted_functions": [
   {
    "name": "lightSetSecondaryDirectional__FG6Point3Ps",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_invert34.o",
  "profile": "authored_cpp",
  "sha256": "a8e66e48b2a3349e714574daf96717798c3e88767f2bc9d540d6d1f45358aa7c",
  "source": "sources/w10-render-b-mat-invert34/mat_invert34.cpp",
  "emitted_functions": [
   {
    "name": "matInvert__FP8Matrix34T0",
    "compiled_body_bytes": 176
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_make_char_view.o",
  "profile": "authored_cpp",
  "sha256": "434167499c0dba208e0ef2f91b3aaf4b6a05b6488724597b0abc8a759c3fbcc4",
  "source": "sources/w10-render-b-mat-make-char-view/mat_make_char_view.cpp",
  "emitted_functions": [
   {
    "name": "matMakeCharViewMatrix__FP8Matrix44fffffff",
    "compiled_body_bytes": 144
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_make_world_view.o",
  "profile": "authored_cpp",
  "sha256": "77cb16df8ff350ded8e1ad6249ec4e08ce59f461c046ed5287cec06ba4cd76ee",
  "source": "sources/w10-render-b-mat-make-world-view/mat_make_world_view.cpp",
  "emitted_functions": [
   {
    "name": "matMakeWorldViewMatrix__FP8Matrix44fffffff",
    "compiled_body_bytes": 144
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_post_rot.o",
  "profile": "authored_cpp",
  "sha256": "08bda9aa1fa1942ac0dd65f4545025c76b77c78daa653ce1b918d93977f711dc",
  "source": "sources/w10-render-b-mat-post-rot/mat_post_rot.cpp",
  "emitted_functions": [
   {
    "name": "matPostRot__FP8Matrix33is",
    "compiled_body_bytes": 340
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_scale34.o",
  "profile": "authored_cpp",
  "sha256": "a419384df8d30031cce0bc08e10ef2923073360f9d8d7b6b8ac26290831809d9",
  "source": "sources/w10-render-b-mat-scale34/mat_scale34.cpp",
  "emitted_functions": [
   {
    "name": "matScale__FP8Matrix34f",
    "compiled_body_bytes": 40
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_scale44.o",
  "profile": "authored_cpp",
  "sha256": "5971be2f5b4ac352aed0cf239e06b9ec3f7356d5c57a3d6a78e5c5677bddd5d2",
  "source": "sources/w10-render-b-mat-scale44/mat_scale44.cpp",
  "emitted_functions": [
   {
    "name": "matScale__FP8Matrix44f",
    "compiled_body_bytes": 60
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "mat_trans.o",
  "profile": "authored_cpp",
  "sha256": "c9884a4c2e1d3ca3cb901745b712a97760ff32684267c3b0bd8cc8fce64641fe",
  "source": "sources/w10-render-b-mat-trans/mat_trans.cpp",
  "emitted_functions": [
   {
    "name": "matTrans__FP8Matrix44fff",
    "compiled_body_bytes": 64
   }
  ]
 }
]
```

## Authored but never executed

All of it. Nothing ran in the game or on hardware. The paired runs executed my compiled functions in
a private interpreter with stubbed callees; the host checks ran on the build host.
The data units were only size- and byte-compared.

## Unknown

- Exact `Light`, `LightResult`, `FxCreatureState` and `GameObject` layouts beyond the used offsets.
- Purpose of float `a`, `b`, `c` and the trailing word of `fxRunCreatureShock`/`fxRunCreatureGlow`.
- The meaning of the nine `lightStart` words, the `lightInfo` record layout, and the particle
  definition fields of the two lava particles.
- Whether `halos`/`globalShadowBrightness` initial values matter before the first reset.
