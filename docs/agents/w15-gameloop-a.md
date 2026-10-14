# w15-gameloop-a: game loop, HUD, stats, name pad, item and player helpers

Branch `agents/w15-gameloop-a`. Private reference executable `work/private/SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses and sizes are that image's own symbol
values. Nothing from the image (bytes, disassembly, decompiler output) is committed. Nothing here ran on the EE or in the game.

## What was done

26 units are authored under `experimental/sources/w15-gameloop-a-*`, one commit each: the name pad (`doNamePad__Fv`,
`drawNamePad__Fv`, `switchToNamePad__Fv`, `resetName__Fv`), the selection-screen helpers (`moveCamera__Fv`, `allDone__Fi`,
`initPostSelect__Fv`, `getHairStyle__Fi`), the stats screen (`statsFrame__Fv`, `printStat__FiPUsiiii`, `displayResistances__Fi`,
`hudRunSlider__FiiiiiUifPii`, `boxDraw__FiiiiiUii`, `msgInventoryAdjustAbility__Fii`), player and world helpers
(`playerRecomputeDamage__6PlayerPQ24item11descriptioni`, `lightGetAmbient__FPs`, `npcLookAtCamera__Fv`,
`objectProcessDeleteList__Fv`, `doEntityDispatch__FP8MTObjDefiP13delayedEntityPib`), three VI/realm routines
(`Add__12VIDictionaryR10VIResourceUi14VIResourceTypei`, `FindTyped__12VIDictionaryUi14VIResourceTypeRi`,
`__Q212RealmService15RealmGatewayAPIUiPCcs`) and the particle definitions `coldSmoke`/`coldSpeck`, `harm1`/`harm2`,
`poisonPoof`, `blueSpark`.

## Method and checks

- Each function was read privately, rewritten in my own words and compiled alone with the reviewed `authored_cpp` flags.
  No byte match to the original is claimed.
- Every unit has a `host_check.cpp` (not a build input) that fails on a wrong branch, offset, constant or call order.
- Each function was also run against the original in a private R5900 interpreter (outside git): identical memory
  snapshots, identical stub callees, equal call logs and equal changed bytes. Scenario counts, covered original words and
  hand mutations that fail are in each commit message. Runs were checked for `HALT`.
- Float constants that the reviewed compiler would round one ulp low from a decimal literal are written as bit patterns.
- Paired runs found two defects in source carried over from other packages (the `blueSpark` first float argument, and the
  `doEntityDispatch` argument list read as a pointer instead of an inline list); both are corrected in my copies.
  The copies supersede the older units: **select the w15-gameloop-a unit or the original, never both**
  (`doEntityDispatch` vs `megatile-do-entity-dispatch`, `objectProcessDeleteList__Fv` vs `norrath_object_process_delete_list`,
  and the particle units vs the nm-players, poison-arrow, harm-touch and cold-arrow static-init units).

## Imports closed (by authored units here)

`allDone__Fi`, `boxDraw__FiiiiiUii`, `_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`, `displayResistances__Fi`, `doEntityDispatch__FP8MTObjDefiP13delayedEntityPib`, `doNamePad__Fv`, `drawNamePad__Fv`, `getHairStyle__Fi`, `_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2`, `hudRunSlider__FiiiiiUifPii`, `initPostSelect__Fv`, `lightGetAmbient__FPs`, `moveCamera__Fv`, `msgInventoryAdjustAbility__Fii`, `_GLOBAL_$I$nmPlayers`, `npcLookAtCamera__Fv`, `objectProcessDeleteList__Fv`, `playerRecomputeDamage__6PlayerPQ24item11descriptioni`, `_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi`, `printStat__FiPUsiiii`, `__Q212RealmService15RealmGatewayAPIUiPCcs`, `resetName__Fv`, `statsFrame__Fv`, `switchToNamePad__Fv`, `Add__12VIDictionaryR10VIResourceUi14VIResourceTypei`, `FindTyped__12VIDictionaryUi14VIResourceTypeRi`.

## Imports of the package list that are NOT closed by this branch

Defined by units that already exist in the repository but are missing from `experimental/build-plan.json`, so they still
show as unresolved (the integrator should select them; I did not duplicate them):

- `hideAbilityAdjustMouse` (`w14-hud-a-hide-ability-adjust-mouse`), `hudQuickSpellOn` (`w14-hud-a-hud-quick-spell-on`),
  `norrath_item_create_random` (`w9-hud-inventory-items-c-create-random`), `specialparse__FPPciiG6Point3Pc`
  (`megatile-specialparse`), `megatileGenerate__FP12_worldHeader` is referenced by `sony-cd-world-init` but I found no
  definition of it.

Not authored, in the order I would take them:

- `statsRunSkillTree__Fi` (0x001bf1a8, 5556 bytes): read, not finished. It builds five 576-byte local tables from
  rodata, draws the points-remaining line and the skill tree, and handles pad input. Too long for a paired result in this
  sitting.
- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` (0x001ce218, 9184 bytes), `routePlot__FR10RouteQueue`
  (0x002c0a10, 19812 bytes), `megatileGenerate__FP12_worldHeader` (0x0025e3a8, 19680 bytes) and `msg_run__6Player`
  (0x00197878, 27496 bytes): not started; each is several thousand instructions.
- `norrath_item_create`: no symbol of that name is in the image's symbol table; the authored callers
  (`ctors-lava-wraith-weapon-rack`) use a name of their own, so it cannot be matched to an original function from here.

## Build-plan entries needed

Append these inputs with fresh `input_order` values after the current tip; `default_authored_inputs` grows by 26. Body sizes
come from the reviewed EE g++ `authored_cpp` profile, not from the original image.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_all_done.o",
  "profile": "authored_cpp",
  "sha256": "1fcafeaac256d9e077d079c31bdd6b4fdc42dd4dd0fb8e95cbb1ecba06132654",
  "source": "sources/w15-gameloop-a-all-done/all_done.cpp",
  "emitted_functions": [
   {
    "name": "allDone__Fi",
    "compiled_body_bytes": 284
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_box_draw.o",
  "profile": "authored_cpp",
  "sha256": "938ed967f6a093d00c0447e40e38b2a602f68d6837635af5aaffcb85a8d204d1",
  "source": "sources/w15-gameloop-a-box-draw/box_draw.cpp",
  "emitted_functions": [
   {
    "name": "boxDraw__FiiiiiUii",
    "compiled_body_bytes": 364
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_cold_arrow_particle_defs.o",
  "profile": "authored_cpp",
  "sha256": "be748f7c8015dffebc0947de451f89354937d34b89767c10c8a66a6451fd5366",
  "source": "sources/w15-gameloop-a-cold-arrow-particle-defs/cold_arrow_particle_defs.cpp",
  "emitted_functions": [
   {
    "name": "_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill",
    "compiled_body_bytes": 308
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_display_resistances.o",
  "profile": "authored_cpp",
  "sha256": "adf3491d2fddcac69c379ab966ff5078cb7f36fcf30fcf8cd39b48989c9dac9a",
  "source": "sources/w15-gameloop-a-display-resistances/display_resistances.cpp",
  "emitted_functions": [
   {
    "name": "displayResistances__Fi",
    "compiled_body_bytes": 500
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_do_entity_dispatch.o",
  "profile": "authored_cpp",
  "sha256": "a374aceec7fea92f8307172df6607f1dc6b9523160851ab1c5a896a75a17178e",
  "source": "sources/w15-gameloop-a-do-entity-dispatch/dispatch.cpp",
  "emitted_functions": [
   {
    "name": "doEntityDispatch__FP8MTObjDefiP13delayedEntityPib",
    "compiled_body_bytes": 2020
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_do_name_pad.o",
  "profile": "authored_cpp",
  "sha256": "cf297794ea5dc6dbfa555f6b83974f292a2aa08accd24b85edd53d8cb400fc3d",
  "source": "sources/w15-gameloop-a-do-name-pad/do_name_pad.cpp",
  "emitted_functions": [
   {
    "name": "doNamePad__Fv",
    "compiled_body_bytes": 2204
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_draw_name_pad.o",
  "profile": "authored_cpp",
  "sha256": "cafc599d951d8975479b10d231575af01af71b4dc753d7f02534cb21b9a08946",
  "source": "sources/w15-gameloop-a-draw-name-pad/draw_name_pad.cpp",
  "emitted_functions": [
   {
    "name": "drawNamePad__Fv",
    "compiled_body_bytes": 2764
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_get_hair_style.o",
  "profile": "authored_cpp",
  "sha256": "ab8872407e6c8e0940e736a1fdde40c60c3487da15d3f17209790520a1d615bc",
  "source": "sources/w15-gameloop-a-get-hair-style/get_hair_style.cpp",
  "emitted_functions": [
   {
    "name": "getHairStyle__Fi",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_harm_touch_particle_defs.o",
  "profile": "authored_cpp",
  "sha256": "5ca644e0bce143ab1caf303c93a0daae3f0687e7bc2d71af6764cb0b2ad15a30",
  "source": "sources/w15-gameloop-a-harm-touch-particle-defs/harm_touch_particle_defs.cpp",
  "emitted_functions": [
   {
    "name": "_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2",
    "compiled_body_bytes": 220
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_hud_run_slider.o",
  "profile": "authored_cpp",
  "sha256": "8c03bf58e59c6966845975d43d73a77a7d52b28de6f9b53ee019070e758a64e2",
  "source": "sources/w15-gameloop-a-hud-run-slider/hud_run_slider.cpp",
  "emitted_functions": [
   {
    "name": "hudRunSlider__FiiiiiUifPii",
    "compiled_body_bytes": 432
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_init_post_select.o",
  "profile": "authored_cpp",
  "sha256": "93f6a37f3239187660c055bc80777236f828e77202122f257d6fe03d80c6e3d5",
  "source": "sources/w15-gameloop-a-init-post-select/init_post_select.cpp",
  "emitted_functions": [
   {
    "name": "initPostSelect__Fv",
    "compiled_body_bytes": 100
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_light_get_ambient.o",
  "profile": "authored_cpp",
  "sha256": "cce6d0e6eae75bf63386236fc1f2cd055bd3ae7d3052398d527c53ea6d07a6c5",
  "source": "sources/w15-gameloop-a-light-get-ambient/light_get_ambient.cpp",
  "emitted_functions": [
   {
    "name": "lightGetAmbient__FPs",
    "compiled_body_bytes": 44
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_move_camera.o",
  "profile": "authored_cpp",
  "sha256": "ae4aedb67842ac6b66e3bc0bc53e4b33f0d5802c2ac88e6b14a5ce2754965cb9",
  "source": "sources/w15-gameloop-a-move-camera/move_camera.cpp",
  "emitted_functions": [
   {
    "name": "moveCamera__Fv",
    "compiled_body_bytes": 1868
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_msg_inventory_adjust_ability.o",
  "profile": "authored_cpp",
  "sha256": "1973bac5dd7ec2db8d78d87174d636298efcfe9b4d498ce465103b18f4a2255d",
  "source": "sources/w15-gameloop-a-msg-inventory-adjust-ability/msg_inventory_adjust_ability.cpp",
  "emitted_functions": [
   {
    "name": "msgInventoryAdjustAbility__Fii",
    "compiled_body_bytes": 64
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_nm_players_particle_defs.o",
  "profile": "authored_cpp",
  "sha256": "34ab966df91bd14ce9376bb021383460c240dbc574f1d5f6d65f046e9395e61c",
  "source": "sources/w15-gameloop-a-nm-players-particle-defs/nm_players_particle_defs.cpp",
  "emitted_functions": [
   {
    "name": "_GLOBAL_$I$nmPlayers",
    "compiled_body_bytes": 688
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_npc_look_at_camera.o",
  "profile": "authored_cpp",
  "sha256": "0341a7a470f3bccb50e0e434d6da501304d1407ad432e1bb9113bf020c006ddb",
  "source": "sources/w15-gameloop-a-npc-look-at-camera/npc_look_at_camera.cpp",
  "emitted_functions": [
   {
    "name": "npcLookAtCamera__Fv",
    "compiled_body_bytes": 140
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_object_process_delete_list.o",
  "profile": "authored_cpp",
  "sha256": "041a928f3d26c047f600f96fcd7a0a2fb22837ffc3bc9a998eb9d4f24ee90d04",
  "source": "sources/w15-gameloop-a-object-process-delete-list/object_process_delete_list.cpp",
  "emitted_functions": [
   {
    "name": "objectProcessDeleteList__Fv",
    "compiled_body_bytes": 124
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_player_recompute_damage.o",
  "profile": "authored_cpp",
  "sha256": "cb4c60ac54a3249ba82cfcb6d65e8336b43626b0467e4534639b9a68a7cb88a8",
  "source": "sources/w15-gameloop-a-player-recompute-damage/player_recompute_damage.cpp",
  "emitted_functions": [
   {
    "name": "playerRecomputeDamage__6PlayerPQ24item11descriptioni",
    "compiled_body_bytes": 1852
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_poison_arrow_particle_defs.o",
  "profile": "authored_cpp",
  "sha256": "d52dd68b305a69febefdc7bac4225cdb66a48a0c082ae6df3d5821cf2b46e58b",
  "source": "sources/w15-gameloop-a-poison-arrow-particle-defs/poison_arrow_particle_defs.cpp",
  "emitted_functions": [
   {
    "name": "_GLOBAL_$I$getRankDescription__16SkillPoisonArrowPUsi",
    "compiled_body_bytes": 312
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_print_stat.o",
  "profile": "authored_cpp",
  "sha256": "2d9ecee0494d2f428f796f571bd038a48ab4b2a9a9e56f7e1547af8ba4339109",
  "source": "sources/w15-gameloop-a-print-stat/print_stat.cpp",
  "emitted_functions": [
   {
    "name": "printStat__FiPUsiiii",
    "compiled_body_bytes": 624
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_realm_gateway_api_ctor.o",
  "profile": "authored_cpp",
  "sha256": "74d6ff4f7564948b83247fd9c9d6909f1e3a579bc1253a87520b9c000f399705",
  "source": "sources/w15-gameloop-a-realm-gateway-api-ctor/ctor.cpp",
  "emitted_functions": [
   {
    "name": "__Q212RealmService15RealmGatewayAPIUiPCcs",
    "compiled_body_bytes": 124
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_reset_name.o",
  "profile": "authored_cpp",
  "sha256": "16f17320956f30296e88d126cddb097598217b66adb8f372959929c3b07fcf14",
  "source": "sources/w15-gameloop-a-reset-name/reset_name.cpp",
  "emitted_functions": [
   {
    "name": "resetName__Fv",
    "compiled_body_bytes": 48
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_stats_frame.o",
  "profile": "authored_cpp",
  "sha256": "1c50e4c78c01c50bb7e91b758035364b214f837827c028edf18f299821904516",
  "source": "sources/w15-gameloop-a-stats-frame/stats_frame.cpp",
  "emitted_functions": [
   {
    "name": "statsFrame__Fv",
    "compiled_body_bytes": 516
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_switch_to_name_pad.o",
  "profile": "authored_cpp",
  "sha256": "9bfe5baf6984ef8d26c88bce0b015e68568b14e71092d7a055d58aa4e71b32bd",
  "source": "sources/w15-gameloop-a-switch-to-name-pad/switch_to_name_pad.cpp",
  "emitted_functions": [
   {
    "name": "switchToNamePad__Fv",
    "compiled_body_bytes": 112
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_vidictionary_add.o",
  "profile": "authored_cpp",
  "sha256": "fb590d5e1a0314b37c3410e56e2304a12121e0bf28a0e961b26836d530c8c7e0",
  "source": "sources/w15-gameloop-a-vidictionary-add/add.cpp",
  "emitted_functions": [
   {
    "name": "Add__12VIDictionaryR10VIResourceUi14VIResourceTypei",
    "compiled_body_bytes": 68
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w15_gameloop_a_vidictionary_find_typed.o",
  "profile": "authored_cpp",
  "sha256": "ee47a93092334aee700fc24170b6e4b01d67941c3ff014aacf2b2b13f5e0fe2c",
  "source": "sources/w15-gameloop-a-vidictionary-find-typed/find_typed.cpp",
  "emitted_functions": [
   {
    "name": "FindTyped__12VIDictionaryUi14VIResourceTypeRi",
    "compiled_body_bytes": 172
   }
  ]
 }
]
```

## Imports opened

Symbols the sources reference but do not define (from an `ld -r`-style `nm -u` of my objects minus my own definitions);
other packages own them under these names:

`BuildMatrix__6CamerafffP6Point3`, `End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, `Find__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUi`, `Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry`, `Pop__6Camera`, `SetMode__6Camerai`, `SetPitch__6Camerai`, `SetYaw__6Camerai`, `_$_16ListSafeIterator`, `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `__16ListSafeIteratorP8ListHeadi`, `__Q212RealmService19RealmGatewayAPICorePCcsPQ212RealmService15RealmGatewayAPI`, `__builtin_new`, `__tf18SpellAncestralCall`, `_ctype_`, `_vt$Q212RealmService15RealmGatewayAPI`, `addpmlist__FPc`, `addpmlistptrscap__Fv`, `checkIfSpellIsRunning__8CreatureRC9type_info`, `curPlayer`, `deleteList`, `delta`, `dst`, `engineDisable30fps`, `engineRemoveTask__FPFv_v`, `engineSetDisplayDisable__Fi`, `engineSyncVBlank__Fv`, `evalHermite3D__FfP6Point3N41`, `evalHermite__Ffffff`, `fontColor__FUiii`, `fontEnd__Fv`, `fontGrid`, `fontPrintCentered__FiiPci`, `fontPrintCenteredu__FiiPUsi`, `fontPrintWrapu__FiiPUsiiii`, `fontPrint__FiiPci`, `fontPrintu__FiiPUsi`, `fontScissor__Fiiii`, `fontStart__FP4FontiUii`, `fontStringSize__FP4FontPci`, `fontStringSizeu__FP4FontPUsi`, `frameNm`, `gSkillTree`, `gameBailFromGameLoop`, `gameButtonToIcon__Fi`, `gameCam`, `getAttackBonus__4itemPQ24item11description`, `getDamageRangeAll__4itemPQ24item11descriptionPiT2i`, `getDamageScale__12SkillArcheryi`, `getTweeks__FiPsT1P6Point3`, `hairStyleSetting`, `hermDeltaScale`, `hermiteTime`, `hudFooterUni`, `hudShowMsg__FPUsN20iii`, `iatan2__Fff`, `initPosition__Fii`, `invMsgHandle`, `inventoryDrawPlayer__FiiP14AnimationStatei`, `inventoryLoad__Fv`, `kittencount`, `language`, `lerpShortRot__Fssfff`, `levelName`, `lightInfo`, `lightMats`, `lightPopSceneLights__Fv`, `lightSetSceneLights__FG6Point3iiiiiiiii`, `lockAlpha`, `lumpFindResource__FPcT0`, `lumpQuery__FPc`, `marble`, `matIdent__FP8Matrix34`, `matRot__FP8Matrix33is`, `matRot__FP8Matrix34is`, `matTransform__FP6Point3P8Matrix33T0`, `mediumFont`, `memset`, `monsterSpawn__FPPcG6Point3sb`, `mt_tiledata`, `nameGridCount`, `nameGridDstW`, `nameGridDstX`, `nameGridDstY`, `nameGridPos`, `nameGridSrcW`, `nameGridSrcX`, `nameGridSrcY`, `nameGridW`, `namePos`, `needEnforce`, `netSendMessage__FiPUci`, `next__16ListSafeIterator`, `nmPlayersAlive`, `norrath_player_select_view_486d3c`, `norrath_stats_screen_view_7d0440`, `objectCreateByName__FPcG6Point3sPPc`, `objectIgnoreFlags`, `otherHermiteTime`, `p1`, `p2`, `pad`, `padAcceptButton`, `padAcceptButtonFontChar`, `padBackButton`, `pitch1`, `pitch2`, `player`, `playerGetSkillRank__Fii`, `playerSelRunning`, `playerSelectInfo`, `playerSelectNextName`, `playerSelectPrevName`, `playerSelectSuccessful`, `playerSelectionTask__Fv`, `playersAlive`, `pmlistptrs`, `pmlistptrsused`, `posInAura__18SpellAncestralCallP6Point3PfT2`, `rectangleDraw__FiiiiiUi`, `scene`, `scriptSetGameState__FPci`, `selectMode`, `setNewGridPosition__Fii`, `showNoNamePrompt`, `skills`, `smallFont`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `specialparse__FPPciiG6Point3Pc`, `sprintf`, `src`, `stage`, `statsDrawDelay`, `statsScreen`, `strcasecmp`, `strcat`, `strcmp`, `strcpy`, `strlen`, `strncmp`, `strstr`, `targetObjs`, `targets`, `textGetString__FPc`, `textureDrawColor__FP8_textureiiiUi`, `throbA`, `throbB`, `throbG`, `throbR`, `userFilterFunction`, `usprintf__FPUsPcffffffff`, `usprintf__FPUsT0ffffffff`, `v1`, `v2`, `worldCameraPos`, `worldSetViewMatricies__FP6Point3P8Matrix34`, `yaws`.

Two of them, `norrath_player_select_view_486d3c` and `norrath_stats_screen_view_7d0440`, are view names of my own for
record addresses; the driver cannot bind them to image symbols (`playerSelectInfo`/`statsScreen` appear too). The
integrator should reconcile these.

## Authored but never executed

All 26 units. Only host contracts and the private interpreter ran them; none ran on the EE or inside the game.

## Unknown

- Meaning of many constants (colors, layout offsets, delay values) beyond their use; no field names are claimed.
- Whether the fixed record sizes used by the name pad and stats screen are the true structure sizes.
- Interpreter float rounding is nearest while the EE compiler truncates, so one-ulp constant mutants may survive.
