# w13-frontend-game-b notes

Package: front end, loading, player selection, shop, memory card and inventory/game-loop support (part 2 of 2).
Every unit below was run against the private image with the private paired-run harness (original and authored function from identical memory, identical stubbed callees, call logs and every changed byte compared; no HALT reported). Each also has a native host contract in its own directory. No unit has run on real hardware or in the game.

## Authored (one commit each)

| Symbol | Image address | Extent | Paired scenarios |
| --- | --- | --- | --- |
| P_Push__Fv | 0x00180578 | 8 | 3 memory states |
| draw__5Spell | 0x00314628 | 8 | 3 memory states |
| modelSetNoZRead__Fv | 0x0016cc78 | 16 | 5 values of modelOption |
| distanceToPlane2__FP6Point3P6Point2T0 | 0x0015a218 | 48 | 8 random vectors |
| matUntransform__FP6Point3P8Matrix44T0 | 0x00159a00 | 220 | 8 random plus 4 aliased in/out |
| textureDrawFloat__FP8_textureffiUiff | 0x001726c8 | 64 | 10 float cases incl. NaN, callee logged |
| P_OpenParticleSystem__Fi | 0x00180338 | 68 | 18 ids over random tables |
| P_CloseParticleSystem__Fi | 0x00180380 | 64 | 18 ids |
| P_SetParticleSystemMatrix__FiP8Matrix34 | 0x001803c0 | 164 | 18 ids |
| P_SetTrailColor__Fiiiii | 0x001829d8 | 72 | 18 ids |
| findCommander__14WoodElfSoldier | 0x00249408 | 232 | 60 random tables, leaveCommander logged |
| playerCheckArmorCacheRaces__Fv | 0x00195560 | 132 | 300 random + 28 consistent/flipped |

Observed / inferred / unknown for each is in the header comment of its source file.

## Imports closed (names)
playerCheckArmorCacheRaces__Fv, findCommander__14WoodElfSoldier, draw__5Spell, matUntransform__FP6Point3P8Matrix44T0, textureDrawFloat__FP8_textureffiUiff, P_CloseParticleSystem__Fi, P_OpenParticleSystem__Fi, P_SetParticleSystemMatrix__FiP8Matrix34, P_SetTrailColor__Fiiiii, P_Push__Fv, modelSetNoZRead__Fv, distanceToPlane2__FP6Point3P6Point2T0.

## Names in the assignment that already had an authored source (not touched)
In a build of the shared plan, only these of the assigned names were unresolved: playerSelectionTask__Fv, norrath_inventory_exit_sound_resource_view, norrath_inventory_message_handler_word_view, norrath_item_create, norrath_item_create_random, draw__5Spell, gameAttackResolver, fxRunBallLightning. The rest were already closed in the plan. Of the unresolved ones:
- fxRunBallLightning__...: authored in `w12-actor-spell-c-fx-run-ball-lightning` (needs only plan integration).
- norrath_inventory_exit_sound_resource_view: authored in `w9-hud-inventory-items-b-view-exit-sound-resource` (plan integration).
- norrath_item_create_random: defined by `w9-hud-inventory-items-c-create-random` (plan integration).
- Already source-authored, found by name search and not built here: init__10StaticFire, initType__8Skeletonb, modelDrawEffect, startFractalLine, playerInitArmorCache (inside load_player_lumps), __4Prop, hideAbilityAdjustMouse, iconAlpha, itemInfoHeight, itemToDestroy, mcerring, mcerror.

## Imports opened
Each new unit imports data or callees that other packages own: `PS`, `globalSystem`, `currentPS`, `trail`, `nmPlayers`, `coreForceSmallCharacters`, `player`, `norrath_player_armor_cache_view_7cb770`, `modelOption`, `_14WoodElfSoldier$m_commanderList`, `leaveCommander__14WoodElfSoldier`, `_textureDrawFloat__FP8_textureffiUiff`.
Caveat: the harness warns the name `trail` is ambiguous (addresses 0x780b00, 0x8d7040, 0x8d7090); the authored function writes at 0x780b00, which is the one the original uses.

## Not closed
- gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb (0x001ce218, 9184 bytes, ~2300 instructions): not attempted beyond inspection. It calls ~30 distinct functions (lumpFindResource 13 times, soundPlaySequence 13 times, soft-double helpers fptodp/dpadd/litodp/dptofp, fmod, sprintf, three RTTI type_info comparisons with __throw_bad_typeid, skill and spell handlers). A verified complete reconstruction needs a paired scenario set for each of those branches; too large for this sitting.
- playerSelectionTask__Fv (0x0027a658, 12200 bytes): same reason; not attempted.
- norrath_item_create: observed (from w9-hud-inventory-items-c notes) to be the same function as `norrath_player_create_item`; needs a one-line alias in the unit that owns it (another package).
- norrath_inventory_message_handler_word_view: the word `invMsgHandle` (0x00486a88), already defined by `inventoryInitNet__Fv`'s unit; needs an alias there (items-player).

## Build-plan entries needed
Not applied to the shared plan. `input_order` left for the lead integrator; `sha256` is of the committed source at the last commit.

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "p_push.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-p-push/p_push.cpp",
  "sha256": "3accb1c8cb21f91c8be3807c0d8a982d9bd8ead1188287035f7951697eda273b",
  "emitted_functions": [
    {
      "name": "P_Push__Fv",
      "compiled_body_bytes": 8
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "spell_draw.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-spell-draw/spell_draw.cpp",
  "sha256": "1d16b680f9ccc9ee6c60fb97a673e6d82fc7dcf3e5e97fe256a144d2d0785591",
  "emitted_functions": [
    {
      "name": "draw__5Spell",
      "compiled_body_bytes": 8
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "model_set_no_z_read.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-model-set-no-z-read/model_set_no_z_read.cpp",
  "sha256": "2e32ab7bb29663f893cb6ea022f95d51f88a5721f1d9d15cb559814bdffab985",
  "emitted_functions": [
    {
      "name": "modelSetNoZRead__Fv",
      "compiled_body_bytes": 20
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "distance_to_plane2.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-distance-to-plane2/distance_to_plane2.cpp",
  "sha256": "c88c1970dc4a8db5d47c8aa6b3ecfb952b6cf3e7eee72a1c4089f8778cb1b87b",
  "emitted_functions": [
    {
      "name": "distanceToPlane2__FP6Point3P6Point2T0",
      "compiled_body_bytes": 48
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w9-render-math-b-common/math_types.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "mat_untransform44.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-mat-untransform44/mat_untransform44.cpp",
  "sha256": "1e18d9e6dd59d92fd2f1fe4a268b712fe602ca45a6b032b38fbe20cbf95175a0",
  "emitted_functions": [
    {
      "name": "matUntransform__FP6Point3P8Matrix44T0",
      "compiled_body_bytes": 188
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "texture_draw_float.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-texture-draw-float/texture_draw_float.cpp",
  "sha256": "8e67f66079a9988d9b670b767199c3c2cb2a47b72cae77fa333855d00a0ceaf2",
  "emitted_functions": [
    {
      "name": "textureDrawFloat__FP8_textureffiUiff",
      "compiled_body_bytes": 52
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "p_open_ps.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-p-open-ps/p_open_ps.cpp",
  "sha256": "e349c3c401a7ef0dcf66f600d2aa086f3377389de653ef810a9c0d6652592c9b",
  "emitted_functions": [
    {
      "name": "P_OpenParticleSystem__Fi",
      "compiled_body_bytes": 68
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "p_close_ps.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-p-close-ps/p_close_ps.cpp",
  "sha256": "6aa93517fe7094c116f354d7e8afaf1f32abafb6e71d8e1b014c4d3efec509f7",
  "emitted_functions": [
    {
      "name": "P_CloseParticleSystem__Fi",
      "compiled_body_bytes": 76
    }
  ]
}
```

```json
{
  "header_closure": [
    "sources/w9-render-math-b-common/math_types.hpp"
  ],
  "kind": "independently_authored_source",
  "object_name": "p_set_ps_matrix.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-p-set-ps-matrix/p_set_ps_matrix.cpp",
  "sha256": "e8b8611e11a30916b349b6f4b86f7158494aba572660c1ce53302e5921630e66",
  "emitted_functions": [
    {
      "name": "P_SetParticleSystemMatrix__FiP8Matrix34",
      "compiled_body_bytes": 176
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "p_set_trail_color.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-p-set-trail-color/p_set_trail_color.cpp",
  "sha256": "2c1029223152b790af69e69880591d0625cdf7ce34b4b0e036fdaed2ff0f6117",
  "emitted_functions": [
    {
      "name": "P_SetTrailColor__Fiiiii",
      "compiled_body_bytes": 76
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "find_commander.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-find-commander-wes/find_commander.cpp",
  "sha256": "ee79671a29963558e260ebbe8e0296e643e045a18f390980b641c1726672d8b5",
  "emitted_functions": [
    {
      "name": "findCommander__14WoodElfSoldier",
      "compiled_body_bytes": 216
    }
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "check_armor_cache_races.o",
  "profile": "authored_cpp",
  "source": "sources/w13-frontend-game-b-player-check-armor-cache-races/check_armor_cache_races.cpp",
  "sha256": "40e36f91d23c2bce5b8bb2bdc498212cfafdf0c2df33f6dcaf92cf75028abbbb",
  "emitted_functions": [
    {
      "name": "playerCheckArmorCacheRaces__Fv",
      "compiled_body_bytes": 176
    }
  ]
}
```

## Authored but never executed
Nothing here ran on the EE or in the game. The only execution is in the private R5900 interpreter and native host contracts; the interpreter is not hardware.

## Unknown
- Meanings of the PS word at +304 (id), of the trail halfword at +1024, of bytes 5 and 6 of an armour-cache entry, of bit 3 and the +960 word of a WoodElfSoldier, and of the last two floats of textureDrawFloat.
- Whether any caller reads the float returned by matUntransform__FP6Point3P8Matrix44T0.
