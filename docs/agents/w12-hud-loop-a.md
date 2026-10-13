# w12-hud-loop-a: game loop, HUD, shop, inventory and player-selection functions (A)

Branch `agents/w12-hud-loop-a`. The package list was the 33 unresolved imports named in the task. At the start the shared
partial build (re-run here on `c7243a6d`) had 827 unresolved imports. Original names, addresses and sizes come from the pinned
`SLUS_205.65` symbol table and its instruction bytes; every authored function was compared with the original in a private
R5900 interpreter run (not in git) and has a committed host check. Nothing here ran on the console. No compiled body is
claimed to match the original bytes.

## What was found about the list

Of the 33 names, 8 functions and 3 data objects already had authored sources in the tree from earlier waves
(`w10-front-a-*`, `w11-front-a-*`, `w9-frontend-a-*`, `w9-hud-inventory-items-a-*`, `w10-render-b-*`) that were never
selected into `experimental/build-plan.json`. Their committed host checks (the `w11-front-a-*` directories) pass here.
I did not rewrite them. I re-ran a private paired comparison with the original for all eight functions (details below)
and I list them in the plan entries. The other 22 names (10 functions, 12 data objects, one more data object being
`creditsText`, not authored) are new in this package.

## Observed, inferred, unknown: per function

Every source starts with its own observed / inferred / unknown comment. Short form:

- `fontStringSize__FP4FontPci` (`0x00174390`, 20 bytes). Observed: forwards its three arguments to `_fontStringSize__FP4FontPci`
  (`0x00172ab8`) as a jump. Inferred: int result. Unknown: the width unit and the Font layout.
- `doNetCamToggle__Fv` (`0x001c7330`, 472 bytes). The camera switch and caption for a dead local player in a network game
  (`netNetworkMode`, `nmPlayersAlive`, `cutMode`, `padAcceptButton`, `gameDeathTimerSingle`, `netLocalPad` 88-byte records).
- `dropGold__4itemG6Point3i` (`0x00100a28`, 628 bytes). Up to ten unit-ball rejection draws from `syncRandState`, height probe
  with `worldFindHeight`, then `blockAlloc(192)` and the Gold constructor.
- `helpfulMessagesDraw__Fv` (`0x001c1fb0`, 968 bytes). The info message with its border, then the boss health bar with its
  two lazily built `ResourceMap` textures. Its function-local statics (`messageAlpha`, `bossInitFlag`, `lastBossHealth`, the
  two guards, the two 16-byte maps) are file-local statics here, not at the original addresses.
- `drawCompass__Fv` (`0x001c6a50`, 852 bytes). Compass model drawn twice, rotated toward `drawCompassPoint`, plus its title.
- `drawGold__Fv` (`0x001c45d0`, 1148 bytes, a local function in the image, exported here because `HUDDrawFrame` calls it by
  name). Flying gold amounts (36-byte records in `goldNumber`), the counters and the icon.
- `getDataForAllObjects__FPciPiiT2T2` (`0x00142880`, 1124 bytes, also local in the image). Walks `objectGrid`, `ghostObjectGrid`
  and the off-grid list; see the ambiguity under Unknown.
- `createRandomCombineableItem__4itemPQ24item11descriptionsPc` (`0x00105aa8`, 1204 bytes) and
  `createNamedRandom__4itemPQ24item11descriptioniii` (`0x00103f28`, 1304 bytes). Selection from the item tables, ten rounds in
  the named case, and the roll-field rewrite. The named one has an observed off-by-one in the pick (k = 0 and k = 1 choose
  the same row; the last eligible row can never be chosen); it is reproduced and tested.
- `drawBust__Fv` (`0x001c6da8`, 1412 bytes, local in the image). Bust frame, name plate, optional field meter with the animated
  bar, counter.
- Data objects, each with its address, size and initial bytes in its own file: `arrowLeft`, `arrowRight`, `invHeader`
  (address words), `bustAlpha` (float), `charAnimTest`, `drewHelpfulMessageHelpBar` (ints), `gameFlags` (16 bytes),
  `freeList` (a word), `hairStyleSetting`, `hairColorSetting`, `beardSetting` (16 signed bytes each), `endPos` (16 bytes,
  three floats plus pad), `expandList` (320 bytes), `emergencyResult` (1568 bytes). Sizes of the zero-size BSS symbols are
  the gap to the next symbol in the image and are inferred. Functions that reference each object were found by a scan of
  absolute and gp-relative references and are named in the file.
- Earlier-wave units listed in the plan entries (not rewritten): `MC_BuildPackedPlayer__FP11playerEntryi`,
  `MC_ConvertTempPlayerToReal__Fi`, `MC_SetRamNmPlayers__Fi`, `MC_NetPack__Fv`, `MC_NetUnPack__Fv`, `MC_LoadLevelEntities__FPcT0`
  (the `w11-front-a` source, not the `w10-front-a` one), `MC_CheckPlayerImportOrChange__Fv`, `coreStartLoadingScreen__Fv`.

## Verification run

- Each new function was compiled with the reviewed EE g++ and `authored_cpp` flags and its host check compiled with
  `c++ -std=c++17 -Wall -Wextra` (flags in each file) and exited 0.
- Each host check was run against hand-made source mutations (constants, strides, offsets, comparison boundaries, call
  arguments); the surviving mutants were either equivalent or led to extra cases, which were added (for example the equal
  boss-health case and the amount clamp in `createNamedRandom`).
- A private paired run (interpreter outside git, same harness as earlier packages) compared original and authored
  functions from identical memory, with identical stand-in callees, over every changed byte outside the stack and the
  call logs. Scenario counts and word coverage of the original: `doNetCamToggle` 300 / 118 of 118; `dropGold` 250 / 158 of
  158; `helpfulMessagesDraw` 400 / 240 of 242; `drawCompass` 300 / 213 of 213; `drawGold` 300 / 286 of 287;
  `getDataForAllObjects` 60 / 282 of 282; `createRandomCombineableItem` 400 / 299 of 301; `createNamedRandom` 400 / 324 of
  326; `drawBust` 400 / 316 of 353 (the rest are float-to-unsigned branches above 2^31 that the alpha range cannot reach);
  `fontStringSize` 5 scenarios, mutation caught. All reported zero mismatches; the uncovered words are nops and delay slots
  unless stated. The same harness re-ran the earlier-wave functions: `MC_SetRamNmPlayers` 12, `MC_ConvertTempPlayerToReal` 9,
  `MC_BuildPackedPlayer` 10, `MC_NetPack` 16 (133 of 135), `MC_NetUnPack` 46 (202 of 213),
  `coreStartLoadingScreen` 60 (390 of 391), `MC_LoadLevelEntities` 60 (161 of 162), `MC_CheckPlayerImportOrChange` 120
  (661 of 714; the 100-slot default initialisation the source omits on purpose is not reachable as a difference): all
  zero mismatches. In `MC_CheckPlayerImportOrChange` the original inlines the player-record copies and the "None" copy
  where the source calls `memcpy` / `strcpy`; those calls were not logged for comparison, which is the only difference
  from the original that the harness is told to ignore.
- A private partial build (copy of the plan and snapshot in a directory outside git, plus the 32 entries below) compiled
  5145 inputs, linked once with 3933 function bodies, 817 unresolved imports and 0 COMMON. The base was 5113 inputs, 3909
  function bodies and 827 unresolved imports.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, all OK.

## Imports closed

`MC_BuildPackedPlayer__FP11playerEntryi`, `MC_CheckPlayerImportOrChange__Fv`, `MC_ConvertTempPlayerToReal__Fi`,
`MC_LoadLevelEntities__FPcT0`, `MC_NetPack__Fv`, `MC_NetUnPack__Fv`, `MC_SetRamNmPlayers__Fi`, `arrowLeft`, `arrowRight`,
`beardSetting`, `bustAlpha`, `charAnimTest`, `coreStartLoadingScreen__Fv`,
`createNamedRandom__4itemPQ24item11descriptioniii`, `createRandomCombineableItem__4itemPQ24item11descriptionsPc`,
`doNetCamToggle__Fv`, `drawBust__Fv`, `drawCompass__Fv`, `drawGold__Fv`, `drewHelpfulMessageHelpBar`,
`dropGold__4itemG6Point3i`, `emergencyResult`, `endPos`, `expandList`, `fontStringSize__FP4FontPci`, `freeList`, `gameFlags`,
`getDataForAllObjects__FPciPiiT2T2`, `hairColorSetting`, `hairStyleSetting`, `helpfulMessagesDraw__Fv`, `invHeader`
(32 names; 827 to 817 unresolved with the opened ones below). Not closed: `creditsText` (see Unknown).

## Imports opened

These 22 names are new in the private link: `ChangeNetCam__6Camerai`, `blackoutTexCopy`, `commaString__FPc`,
`floorPutSaveInfo__FPc`, `foregrndTexCopy`, `getPackChecksum__Fv`, `loadBuffer`, `loadFrames`, `loadStack`,
`loadThreadMain__FPv`, `loadThreadParam`, `matUntransform__FP6Point3P8Matrix44T0`, `mcDisableStartupScript`,
`megatileEntitize__FPci`, `modelSetNoZRead__Fv`, `objGet__FP10GameObjectPc`, `objectPutSaveData__FPci`,
`padChangeIndex__Fii`, `playerGoldIconx`, `readMap__3mapPUc`, `scrollmapTexCopy`, `titleloadTexCopy`. Their owners are
other packages (camera, fonts/text, loading thread, save/object code, model/matrix, player-selection pad code); none was authored here.

## Build-plan entries needed

32 entries, profile `authored_cpp`, listed below in the order of the generator
`docs/agents/w12-hud-loop-a-plan-entries.py` (gitignored directory; added with `git add -f`). Run it with
`--toolchain /Users/case/norrath-agents/repo/work/toolchains/ee` to print the same lines, and with `--private-root DIR`
(outside git) to write a private plan and snapshot copy that contains them; `input_order` is assigned by the integrator.
Ten functions and 14 data objects are new sources (`w12-hud-loop-a-*`); eight are earlier-wave sources
(`w10-front-a-mc-build-packed-player`, `w9-frontend-a-mc-check-player-import-or-change`,
`w10-front-a-mc-convert-temp-player-to-real`, `w11-front-a-mc-load-level-entities`, `w10-front-a-mc-net-pack`,
`w10-front-a-mc-net-un-pack`, `w10-front-a-mc-set-ram-nm-players`, `w10-front-a-core-start-loading-screen`). The anonymous
namespace helpers some units emit (`_ZN12_GLOBAL__N_1...`) are local to their unit.

Do not also select these overlapping sources, which define the same names: `w10-front-a-mc-load-level-entities`
(use the `w11-front-a` one), `w9-hud-inventory-items-a-appearance-settings` (defines `hairStyleSetting`,
`hairColorSetting`, `beardSetting` and also `skinSetting`, which is not in this package), and
`w10-render-b-data-inventory-textures` (defines `invHeader`). The `w11-front-a-views` units (already selected) define
address-matched `norrath_front_a_view_*` objects at the addresses of `arrowLeft`, `arrowRight`, `charAnimTest`,
`bustAlpha`, `emergencyResult`, `beardSetting` and `endPos`; they are different symbol names, so they do not collide at
link, but they describe the same storage as the named objects here. The integrator decides which kind stays when a
layout is built.

```
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "fontStringSize__FP4FontPci"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_font_string_size.o", "profile": "authored_cpp", "sha256": "4fb2c154c9ed926ae5eee1536fb13e7fbde5f3b961ec5deb29628103b0ce5f10", "source": "sources/w12-hud-loop-a-font-string-size/font_string_size.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 636, "name": "doNetCamToggle__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_do_net_cam_toggle.o", "profile": "authored_cpp", "sha256": "ec6dfe0ea1c600d2ae6a17150858abc8c9402c41a232e0ebb49a58442ebe1230", "source": "sources/w12-hud-loop-a-do-net-cam-toggle/do_net_cam_toggle.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 552, "name": "dropGold__4itemG6Point3i"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_drop_gold.o", "profile": "authored_cpp", "sha256": "7c01f4084286d72af0dcef2595bb22667559b21145f41750779364c597b94f5b", "source": "sources/w12-hud-loop-a-drop-gold/drop_gold.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1160, "name": "helpfulMessagesDraw__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_helpful_messages_draw.o", "profile": "authored_cpp", "sha256": "1a84e5f07fefd1f609fba5400a83c9e71ce42f91191516c403e21c4f09f843a2", "source": "sources/w12-hud-loop-a-helpful-messages-draw/helpful_messages_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 788, "name": "drawCompass__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_draw_compass.o", "profile": "authored_cpp", "sha256": "bb03b34e34fbe272653702a2a24a0343a33e4eaa4e23ee1813503993382f6a64", "source": "sources/w12-hud-loop-a-draw-compass/draw_compass.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1388, "name": "drawGold__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_draw_gold.o", "profile": "authored_cpp", "sha256": "507a3e3eca845c2dc765e85f35123d6bef6d93adf48d8493253eecca8d30fd90", "source": "sources/w12-hud-loop-a-draw-gold/draw_gold.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 396, "name": "_ZN12_GLOBAL__N_1L9walk_listERNS_4WalkEPh"}, {"compiled_body_bytes": 364, "name": "getDataForAllObjects__FPciPiiT2T2"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_get_data_for_all_objects.o", "profile": "authored_cpp", "sha256": "e23f3495fa6e488cb592b5e4d13a1754b46577b6f1a5adb507ffc828097c97de", "source": "sources/w12-hud-loop-a-get-data-for-all-objects/get_data_for_all_objects.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "_ZN12_GLOBAL__N_1L8load_u64EPKh"}, {"compiled_body_bytes": 256, "name": "_ZN12_GLOBAL__N_1L8eligibleEPKhPKcii"}, {"compiled_body_bytes": 696, "name": "createRandomCombineableItem__4itemPQ24item11descriptionsPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_create_random_combineable_item.o", "profile": "authored_cpp", "sha256": "64ac82c55e18e80154494323af5109ccbdb850e31b3e7456adb398582ad6b0ba", "source": "sources/w12-hud-loop-a-create-random-combineable-item/create_random_combineable_item.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "_ZN12_GLOBAL__N_1L8load_u64EPKh"}, {"compiled_body_bytes": 160, "name": "_ZN12_GLOBAL__N_1L8eligibleEPKhiib"}, {"compiled_body_bytes": 1272, "name": "createNamedRandom__4itemPQ24item11descriptioniii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_create_named_random.o", "profile": "authored_cpp", "sha256": "ba86cf1302678dc621cb675179c3fe2070269820c669756b27839fbb35cdd14b", "source": "sources/w12-hud-loop-a-create-named-random/create_named_random.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1820, "name": "drawBust__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_draw_bust.o", "profile": "authored_cpp", "sha256": "52d72237459a793e8d4834920cf41751d6f7823b482435246154f8f77bf792cd", "source": "sources/w12-hud-loop-a-draw-bust/draw_bust.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_arrow_left.o", "profile": "authored_cpp", "sha256": "c8a559de8eb8e9775bef41f79242903dd7a2b69ad61313f2e6409cc5793b2757", "source": "sources/w12-hud-loop-a-data-arrow-left/data_arrow_left.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_arrow_right.o", "profile": "authored_cpp", "sha256": "2fcf32ef5918c59ea6fc5056a4f198031d8353145d55fbab55544eaf4cc27df6", "source": "sources/w12-hud-loop-a-data-arrow-right/data_arrow_right.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_beard_setting.o", "profile": "authored_cpp", "sha256": "04206430d31c5eb2491d650d1a170253fbc4b6f251926dea7eb905f56e9fbde7", "source": "sources/w12-hud-loop-a-data-beard-setting/data_beard_setting.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_bust_alpha.o", "profile": "authored_cpp", "sha256": "e39971b0580b4630d63f786e3f3156cf4d0056516c2b755b2fa55c1bfbef0fc9", "source": "sources/w12-hud-loop-a-data-bust-alpha/data_bust_alpha.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_char_anim_test.o", "profile": "authored_cpp", "sha256": "3defb62bc92a09d06640c01ee4451865b77c53d22bb907757db9605974c59c8b", "source": "sources/w12-hud-loop-a-data-char-anim-test/data_char_anim_test.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_drew_helpful_message_help_bar.o", "profile": "authored_cpp", "sha256": "1c163923ed05da766b95c14110a0aee6c53125da0d3cdcbecd22a23f9b87d711", "source": "sources/w12-hud-loop-a-data-drew-helpful-message-help-bar/data_drew_helpful_message_help_bar.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_emergency_result.o", "profile": "authored_cpp", "sha256": "5554bb584f92f862fe96b4a9a96b4e2ab05fbfd13954ac299c3f6312d57fcc66", "source": "sources/w12-hud-loop-a-data-emergency-result/data_emergency_result.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_end_pos.o", "profile": "authored_cpp", "sha256": "5605cb515dbe9a9752824775a3d7837d1faf651a1100cd23cc13e5df32de1d2c", "source": "sources/w12-hud-loop-a-data-end-pos/data_end_pos.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_expand_list.o", "profile": "authored_cpp", "sha256": "261b9eb7b7c5b3e7b017764aa39f956f2b0fdebc16359b3bccad258296783771", "source": "sources/w12-hud-loop-a-data-expand-list/data_expand_list.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_free_list.o", "profile": "authored_cpp", "sha256": "bc57aece019d6e7719aec2676600ae76763449cbb662ae02b6df420e1fcea8b5", "source": "sources/w12-hud-loop-a-data-free-list/data_free_list.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_game_flags.o", "profile": "authored_cpp", "sha256": "60cc02a83d69981516ece8b7c09ee748135dda9234592ae57d8adbd38a770783", "source": "sources/w12-hud-loop-a-data-game-flags/data_game_flags.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_hair_color_setting.o", "profile": "authored_cpp", "sha256": "bdc8bf7aa0df9251451f59c1d94d1fe18c852e31f8f3b50a08c1385b942547ef", "source": "sources/w12-hud-loop-a-data-hair-color-setting/data_hair_color_setting.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_hair_style_setting.o", "profile": "authored_cpp", "sha256": "0deda377b57870b78501322b725b32b2fc8d9f8c06a92641ee7b750d70e474fe", "source": "sources/w12-hud-loop-a-data-hair-style-setting/data_hair_style_setting.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_hla_data_inv_header.o", "profile": "authored_cpp", "sha256": "7db13ed314e044e6817c345a68b6ddd23d35568895d6c417cb5077338304a701", "source": "sources/w12-hud-loop-a-data-inv-header/data_inv_header.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "MC_BuildPackedPlayer__FP11playerEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10_front_a_mc_build_packed_player.o", "profile": "authored_cpp", "sha256": "b76eda68f2819d15051a1e41b9d551ee332e7a57bbb41d5e21218832befcdf57", "source": "sources/w10-front-a-mc-build-packed-player/mc_build_packed_player.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "_ZN12_GLOBAL__N_1L14remove_flaggedEi"}, {"compiled_body_bytes": 2248, "name": "MC_CheckPlayerImportOrChange__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w9_frontend_a_mc_check_player_import_or_change.o", "profile": "authored_cpp", "sha256": "232ae541045e730b711e7bcaa8e8459ac700829ca57e3c9de780394b28ce2b33", "source": "sources/w9-frontend-a-mc-check-player-import-or-change/check_player_import_or_change.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "MC_ConvertTempPlayerToReal__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10_front_a_mc_convert_temp_player_to_real.o", "profile": "authored_cpp", "sha256": "eaa32379cec41b516348f17f2b4c0ce05ca48c965e2f7cda07d18eb07f33d55f", "source": "sources/w10-front-a-mc-convert-temp-player-to-real/mc_convert_temp_player_to_real.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 812, "name": "MC_LoadLevelEntities__FPcT0"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11_front_a_mc_load_level_entities.o", "profile": "authored_cpp", "sha256": "54c5a5cdffa7e72aa22166eb19faae0b11234abb0b85d36efb7f84d41d6987da", "source": "sources/w11-front-a-mc-load-level-entities/mc_load_level_entities.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 524, "name": "MC_NetPack__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10_front_a_mc_net_pack.o", "profile": "authored_cpp", "sha256": "648cd7f877ccd63e4c377227ad5f386c5bd7b9b8c617cd21b7d84b298f993878", "source": "sources/w10-front-a-mc-net-pack/mc_net_pack.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 808, "name": "MC_NetUnPack__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10_front_a_mc_net_un_pack.o", "profile": "authored_cpp", "sha256": "29c9db525a6091d36e4bdfcd5320ee0fbc4da05ecc8e80e1b5daf0e07a0ac7ef", "source": "sources/w10-front-a-mc-net-un-pack/mc_net_un_pack.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "MC_SetRamNmPlayers__Fi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10_front_a_mc_set_ram_nm_players.o", "profile": "authored_cpp", "sha256": "f74276bd8ee26825c1122d5f4c6d0dd71d2405a2b4566273d26dfdd82e9045e6", "source": "sources/w10-front-a-mc-set-ram-nm-players/mc_set_ram_nm_players.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1504, "name": "coreStartLoadingScreen__Fv"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10_front_a_core_start_loading_screen.o", "profile": "authored_cpp", "sha256": "e67708e4c2ceea440101158c9485bb1f77564fa81cf135efdf923f4de39d57f1", "source": "sources/w10-front-a-core-start-loading-screen/core_start_loading_screen.cpp"}
```

## Authored but never executed

All of it. Nothing ran on the console. The private paired runs used stand-in callees for everything below the unit's
own code, so no callee behaviour was exercised. Specific limits:

- Pointer words in records (item and player records, list nodes, the filter function address, texture and row pointers)
  are read as 32-bit words and used as addresses on the target; the host checks map them through an arena or a table
  (`NORRATH_HOST_CONTRACT`), so the 64-bit host never dereferences a target address.
- `getDataForAllObjects`, `createRandomCombineableItem`, `createNamedRandom` and `drawBust` iterate loops whose end
  depends on table contents that only the original image holds; the paired runs used small synthetic tables.
- `drawGold`, `drawBust`, `helpfulMessagesDraw` and `drawCompass` convert floats to integers and unsigned bytes with the
  compiler's own conversion; the host checks pin values where the result is not a round number, but the EE float rounding
  was exercised only by the interpreter.
- `dropGold`'s rejection loop terminates for the roll sequences tried; a roll sequence that never lands in the unit ball
  would loop here exactly as in the original.
- The data objects are zero-filled; their runtime contents are set by the units named in each file.

## Unknown

- `creditsText` (`0x00428a20`, 2408 bytes) is a table of 301 records of an int style and a text pointer into the image's
  string pool. Authoring it means reproducing the credits strings, which are game text; earlier packages
  (`w8-frontend-menu-a`, `w10-front-a`, `w11-front-a`) left it unauthored for the same reason. It stays an unresolved
  import, pending a maintainer decision on whether the text may be published.
- `offTheGrid`: the image has two local symbols with that name (`0x00486848` and `0x00486890`). `getDataForAllObjects` uses
  the one at `0x00486890`. Another planned unit already supplies a symbol `offTheGrid` by name, so the name alone cannot say
  which one the link should pick; the paired run bound it to `0x00486890` explicitly.
- Meanings: the flag bits 0x4000, 0x3000, 0x2000 in the object flag word and the filter argument at object + 52; category 2,
  flag bit 35 and flag 0x10000 of the item rows; the two constants 12.0 and 6.0 in `dropGold`; the constant triple in
  `drawCompass`; which language numbers use the second `drawBust` / boss bar layout; what `drawBustMax` counts; why the
  compass model is drawn twice; the unit of `fontStringSize`.
- Layouts: the `goldNumber` field names, the texture header beyond the two halfwords, the item description beyond the
  words used, and the true extents of the zero-size BSS symbols (`beardSetting`, `hairStyleSetting`, `hairColorSetting`,
  `endPos`, `expandList`, `emergencyResult`).
- Which statics of the original were local (`drawGold`, `drawBust`, `getDataForAllObjects`, the HUD message statics) and
  whether exporting them under their image names matches how the final image was linked.
