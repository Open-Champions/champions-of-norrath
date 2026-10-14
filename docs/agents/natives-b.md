# natives-b

Middle third of the retail `amx*__FP5__amxPi` natives. The 120 names sort in plain byte order. This package is numbers 41 through 80, one function per commit on `agents/natives-b`. Each unit compiles with the reviewed `authored_cpp` profile and has a host test that fails when the cell index, stride, count reload, or rounding is wrong. No pull request was opened.

## Observed

The retail names and extents used below come from the private ELF symbol table, matched to the 120 names in `docs/agents/script-amx.md`. GP-relative addresses use `_gp` `0x4879f0`. Integer-to-float uses `cvt.s.w`. Float-to-int uses `cvt.w.s`, which rounds ties to even. `0.01` is the word `0x3c23d70a`. The scale in `amxGetScaledTime` is `0x3c888888`. The treasure scale in `amxGivePlayerRandomItem` is `0x3c75c28f`.

Player records are the object `player` at `0x408600`, four records of 23984 bytes. These units name the existing `norrath_player_progression_records_view` rather than defining a second `player`. `nmPlayers` is the existing 4-byte object and is read as a signed word. `netNetworkMode` is the existing byte and is read with a signed load. `skills` at `0x34b440` is the existing `norrath_player_skill_catalog_view`; byte 116 is slot 29 of that 4-byte pointer array. `syncRandState` is the existing `norrath_constructor_random_state_view`. `nodes` is the existing `norrath_route_nodes_bytes`; `amxHitPlayers` reads player indexes at byte 5344 (`0x43a3e8`). `levelName` is the existing 322-byte object; the treasure search starts at byte 65 (`0x34b6a1`).

`amxGivePlayerItem` and `amxGivePlayerRandomItem` write `0xFF` at description bytes 32 and 33, zero halfwords at 16 through 31, and `0xFFFF` halfwords at 36, 44, 52, and 60. The sword strings are `shortsword+damageincrease75+weightmodifier-25+requiredlevel5+dexteritybonus5`, `lightsword+damageincrease460+weightmodifier25+requiredlevel21+dexteritybonus10`, and `falchion+damageincrease350+requiredlevel33+dexteritybonus15`. Difficulty 1 stores bytes 18 and 51, difficulty 2 stores 51 and 52, and difficulty 3 stores 36 and 53.

## Inferred

The other description bytes are cleared before `create`. The original left those stack bytes untouched. Calls into the game use C signatures with the retail mangled names. The reviewed EABI assignment of those arguments was not compared instruction by instruction with the retail `jal`. `amxLightEffect` passes the point, cells 3 through 12 as floats, `1`, cell 2 times `0x3c23d70a`, a null object, and zero. `amxHitPlayers` passes the adjusted creature, two zeros, the position pointer, and the damage float. `createRandom` receives the rolled ramp value, the low bit of the next random high half, `-1`, and `worldDifficulty` rounded ties-to-even.

## Snapshot files

`tools/check_source_snapshot.py` requires every file under `experimental/sources/` to appear in `experimental/snapshot.json`. These paths are not recorded yet. Host tests are snapshot files only. They are not EE plan inputs. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests, and that run does not call the snapshot checker.

```
124c3ae09c453ce473d237bef6cec45942c78438a20e5caed1483fb2cddf37d3  sources/natives-b-get-save-inhibit/get_save_inhibit.cpp
a6d4793ecccfd074abe99b9827e882c7b8ccc935ed836cc57546d9408af97947  sources/natives-b-get-save-inhibit/get_save_inhibit_host.cpp
98b9b0dfa87836679ea9544f7f543ad49c9c4ce3bb84091365489bc92ce24a19  sources/natives-b-get-scaled-time/get_scaled_time.cpp
ca0252fb5271ff38645c68ee2eeb852bb5e030151c0518535bfa7eef3f9b4d4b  sources/natives-b-get-scaled-time/get_scaled_time_host.cpp
6abdeba25be424b26d0591b401cf35f7ccbc84eb90c23269b0e47405fc68e0cb  sources/natives-b-get-v-position/get_v_position.cpp
253d6753a0623308d96c3d9cc1756531f261aa5210ef8694c84f5317f5f29d2e  sources/natives-b-get-v-position/get_v_position_host.cpp
5974fac40e614f3144111c17dc810299daa30c28f9e9a38907a26bc852b9cc17  sources/natives-b-getv/getv.cpp
f08324418c4417806de479e7c27ee3baca6dc912fae16f4d6e95a96f9e590a22  sources/natives-b-getv/getv_host.cpp
2b93b1fdc71eccac6a38f0e3f6621f401140178c88a091eaea52e3afbfb7327f  sources/natives-b-give-player-exp/give_player_exp.cpp
859b215ec1d6f4c9157a1648a46ce7555fd8a820f59f0979606144d3667c6699  sources/natives-b-give-player-exp/give_player_exp_host.cpp
ee49c603b1580a273bc9364d8fc6834f9a0def883aad9d97ca2ae68da7975367  sources/natives-b-give-player-gold/give_player_gold.cpp
881bb8c9cf278714d7fe341af82f0934369b9408d4a1a78873ef3319f4fe9446  sources/natives-b-give-player-gold/give_player_gold_host.cpp
f7bfc4366d32826158e576e0ecf46df021aa45e2ea18445a00033b233d42694a  sources/natives-b-give-player-item/give_player_item.cpp
27e2d8b7f639d67b674d80f1621162bdd89b43878875d856f73a0a322ae436ec  sources/natives-b-give-player-item/give_player_item_host.cpp
5733625b16d84789f6bfee623727dca7e81549ed112bba51d33366abef313212  sources/natives-b-give-player-random-item/give_player_random_item.cpp
19bd0755956b654c077bfcfe3050315c2c26e5862bd45e980179489007c04f51  sources/natives-b-give-player-random-item/give_player_random_item_host.cpp
ea6f0fa9ba795c1eeab4b9c475dde6c59da6d2ab41c19893eb7514cab72a0dcc  sources/natives-b-heal-player/heal_player.cpp
790539f2bc938c303486809abb1d403cd921429da377f65689fa2ed2fb42e96e  sources/natives-b-heal-player/heal_player_host.cpp
859a1cc6a6f74aa4811123b5b6184d5cbe607779298b076158ac6fb312b4a125  sources/natives-b-hide-monster/hide_monster.cpp
d3a8399d756ead9d6076ac5cabad44745b5b80e63afed17e897e8564d007db51  sources/natives-b-hide-monster/hide_monster_host.cpp
d900f5b7d8654bf03a055e18bd43fe09cc2b68b060e46d0649bace10e1041d15  sources/natives-b-hide-players/hide_players.cpp
2414b7c1e4d89604f13ce7319f93108bac0d69aa34c1bb1d4e445cb48155d460  sources/natives-b-hide-players/hide_players_host.cpp
1f08e9c08f864cfc1eae9c1d4efb508c9179c785c08dec8bf63f6b646581b5f7  sources/natives-b-hit-players/hit_players.cpp
76f205743b5fc75c665646dfc7bd879e0c61924cd7c2fa53efd1728c0ca51878  sources/natives-b-hit-players/hit_players_host.cpp
9a7f76887f574b2c1b055a70a64a277d40c5f882cf0362d626c3c5494e06a8e7  sources/natives-b-hold-vehicle/hold_vehicle.cpp
de8209e5830f9ea3d1b2eed1dae5365b2d50de8e25cdaba61913f9a09c3e5730  sources/natives-b-hold-vehicle/hold_vehicle_host.cpp
1b7d3f19f8d1568ed7c919630439d60dbe74cac8003673cac366d120828fd852  sources/natives-b-horn-omen/horn_omen.cpp
f8741ab6fa135810b4683ffcbd245f2be34757be9cc63c248db84276c850606f  sources/natives-b-horn-omen/horn_omen_host.cpp
530846a4c274af968870244083952fcaa57e49cc56170e59be8c029bd14fd180  sources/natives-b-hud-fade-screen/hud_fade_screen.cpp
66c98f8c8e8e10fb43519483b0df6a96cd6ee4911cc8cd09a4fbc06d5ad4c8f0  sources/natives-b-hud-fade-screen/hud_fade_screen_host.cpp
0eae2cc6128fd797707a6605482f69a6780a7cbf8074e059bc3d9a43b8699256  sources/natives-b-hud-reset-fade-screen/hud_reset_fade_screen.cpp
fe051d19cf56da01865a3cde8a3e19fc69d8544cbad5dc091868577314cddf9b  sources/natives-b-hud-reset-fade-screen/hud_reset_fade_screen_host.cpp
862dc5d78edc5728b4ccb818ae0e43695a45dca458f68619f9029529704145c2  sources/natives-b-leave-game/leave_game.cpp
3f9c94aac5a920b8d3c8e6a42172853163f0f9543673e687d25fee880fa0efd3  sources/natives-b-leave-game/leave_game_host.cpp
eced1c88e3ebb3a0d03c7914f50ba4ee23647a2df6ebc4fed7ab2321cb315067  sources/natives-b-level-loading/level_loading.cpp
e3bba649a28ebbec286f959ded06814768f4d5a158a74a60cbb794bcefd1a8d8  sources/natives-b-level-loading/level_loading_host.cpp
0844fa2a4c46e6f91587a94e4493ea9cdec67b095772255115665d6ed907c657  sources/natives-b-light-effect/light_effect.cpp
6faa2d90e3473391fc9ae2a653ef5b1eb1b6df758adb28cfc3b30ec45fa84a42  sources/natives-b-light-effect/light_effect_host.cpp
d16b5d27764376ffc7af41ce02f5e7ff6f63196202be56a37f522015fc0ffd8d  sources/natives-b-load-check-point/load_check_point.cpp
6f352e00cbb91b2b17ce0fb3eb9157121b68462a1ddb51580651b95e0c9caee1  sources/natives-b-load-check-point/load_check_point_host.cpp
b3e6f1d3cb91483560aad3849c547ddae7494b6cfa82e899216d533e23e6ec16  sources/natives-b-move-monster/move_monster.cpp
2da540250432ba05b5f9046d379566e85c347eb49709586c1e38c75e5021cb6e  sources/natives-b-move-monster/move_monster_host.cpp
8c2e8b15b94c0c894b3207a9fa5252c0f9b8d573e23f30ea59af51eaf326d2d9  sources/natives-b-network-mode/network_mode.cpp
092765781de2c341ac414fb4f9a65cee4a61b1f09caae4b6d9a8f2acb10f7e87  sources/natives-b-network-mode/network_mode_host.cpp
8bbeba9c0adebbbdfbce30a9fd67022d21f0362628c8c2bc5fbfc6d2ea95c79f  sources/natives-b-new-world/new_world.cpp
d1d4af2e94dfc694bdc52cbf6788c2156b49e6d06c2ac7dbf5d6f6e61825c45b  sources/natives-b-new-world/new_world_host.cpp
12187dbc7481126692362d754d53e9b9ce01a62605be5c9f6aada8e7888de2f0  sources/natives-b-new-world-parameter/new_world_parameter.cpp
9ea8e03d0de6fe9a98c88bfa805934a1c33774caaf5881ce2ceb8e5081fb0ba3  sources/natives-b-new-world-parameter/new_world_parameter_host.cpp
ed081a866851a956469ae8421ab1a6232621a882b67725fd0f09c1cf0808e47c  sources/natives-b-new-world-post/new_world_post.cpp
1334ff854898ebc8451188f5ed78a3b0043a6c1c5d6200f7976d5e873a5173e3  sources/natives-b-new-world-post/new_world_post_host.cpp
29b421895edd5b6cdbaa4de6bb80c279c4eae42fe3f0ecd6bb17da19ba656d87  sources/natives-b-npc-anim-at-end/npc_anim_at_end.cpp
b032f127dc2651dbffdf9b4e7bd73d0b2ba96da5a1a5cfaba4b2e9753452cd4a  sources/natives-b-npc-anim-at-end/npc_anim_at_end_host.cpp
8193fd09f05a03fc6cb010d64523aa51f1fb6c4ad426c3dfb28811eac1d161c2  sources/natives-b-object-in-player-trigger/object_in_player_trigger.cpp
85c9cdff74961f4012f6e193fe47b7440c86180ca7ac0a7a9d01755bc91ddc16  sources/natives-b-object-in-player-trigger/object_in_player_trigger_host.cpp
822cc773960c8c1ce1067b297d7f89c32d9b35454460c4d7d0c0d50bb0dccaa2  sources/natives-b-particle-morph/particle_morph.cpp
ec12286f86cd7ed8eb1c39df559eb8af68a6dc577985f39d86033162d6fde4c4  sources/natives-b-particle-morph/particle_morph_host.cpp
fbec533e661d1695b9536d3cc4d4af9d3f4d3fe101624b53ca70b9a6f14d14ee  sources/natives-b-pause-game-control/pause_game_control.cpp
5dfb75d124f06a9e4cc7f7c5ed496f10b8d6158942e812198003cbe85dc09905  sources/natives-b-pause-game-control/pause_game_control_host.cpp
796e72022b4a0df8e3d2842caf3ceec8529830e81f320e4279a9075ba5ecdb7a  sources/natives-b-planar-steel/planar_steel.cpp
112c9b61d8504e7020cc34418b85aaedb8a283e44653b87da7b45705a61b4ad5  sources/natives-b-planar-steel/planar_steel_host.cpp
35ccb6743bf6c54f641bc02191425dceb4d7453e3bf17e9efeacef19c28f3b84  sources/natives-b-play-got-item-sound/play_got_item_sound.cpp
2a8241f3d0eec73c255f5e18483e4f495b3f0c2fc2b7dc519d700c97146722cd  sources/natives-b-play-got-item-sound/play_got_item_sound_host.cpp
c58e7e7a38d20a4ca0d57008ee48e20f327279fb57ed1628515b9ffd3722a002  sources/natives-b-player-has-item/player_has_item.cpp
ca81b5091a1ab49803bb781f6020e974a22fd66eaf63878861b6c89cd24495c0  sources/natives-b-player-has-item/player_has_item_host.cpp
d67bfdf5bbf171912586a739c3d0d11d6acbcbb780db83d63082cc7bf449c4cc  sources/natives-b-player-sel-running/player_sel_running.cpp
7123bbb38e381b7c658ca75d528b716035a8128e03a824c1a1645a62b81e3373  sources/natives-b-player-sel-running/player_sel_running_host.cpp
5586dcbc052723c5f29e8d6303cd4d9b08f591f113376db682ac7303327a7d33  sources/natives-b-players-alive/players_alive.cpp
023ba38c87055b3a191e2dacb4862153136399a678a5d1bf88ba2302eea91b73  sources/natives-b-players-alive/players_alive_host.cpp
b7c537e1ee2dec85b547a98830271886ffe2990fe8e64174e56dc8c1053de767  sources/natives-b-players-loaded/players_loaded.cpp
4a2f949d1fc28b2f37394f9c6336cc00df218611af69d138f42cdc910cf6cfa3  sources/natives-b-players-loaded/players_loaded_host.cpp
eb0424104343fa162e2b8150dd70f2bdb8231a915d4cb3b49b6ce00c25b1652b  sources/natives-b-preload-resource/preload_resource.cpp
30cbd52c45085e1379f4bdf5266048363139373cf0b3804dab195829887fbfef  sources/natives-b-preload-resource/preload_resource_host.cpp
2fb0cee7a1c43618a88bd95679210d06e81341d154905a0a86610f5dfc90eb78  sources/natives-b-print/print.cpp
7f7d0569456dcbf0e31839a05ed8d4ff1fd5bd5aae98d810d5a6eb69ec685f70  sources/natives-b-print/print_host.cpp
60823e922cf5f75a8e735461ecd48dacf93bbeced84381d82e45274a48572248  sources/natives-b-print-val/print_val.cpp
aedd09a4383911038ead91ecf89b49434fe4419f58dc309cf599ac4400db4b96  sources/natives-b-print-val/print_val_host.cpp
80e6099b40dfcc46bca4e6328531e06b066ddbdfc9e8cdca7a95b6a466fe4dce  sources/natives-b-prop-anim-at-end/prop_anim_at_end.cpp
b2b2d8aa90196521c0ee84bbc13ffff0cb78749a0e06514dffa7ed6cd89eb9bf  sources/natives-b-prop-anim-at-end/prop_anim_at_end_host.cpp
5a192e0e70281d9af394c0dcdd209bc2853b0065a0fa544481bac96edd722424  sources/natives-b-put-npc-in-vehicle/put_npc_in_vehicle.cpp
8572ed19c86256fbe273d3fb93e7e4f2f99a08ad5c11281c785c74aa0d1239df  sources/natives-b-put-npc-in-vehicle/put_npc_in_vehicle_host.cpp
```

## Imports closed

These 40 definitions close the middle slice. Names are the retail mangled symbols.

- `amxGetSaveInhibit__FP5__amxPi`
- `amxGetScaledTime__FP5__amxPi`
- `amxGetVPosition__FP5__amxPi`
- `amxGetv__FP5__amxPi`
- `amxGivePlayerExp__FP5__amxPi`
- `amxGivePlayerGold__FP5__amxPi`
- `amxGivePlayerItem__FP5__amxPi`
- `amxGivePlayerRandomItem__FP5__amxPi`
- `amxHealPlayer__FP5__amxPi`
- `amxHideMonster__FP5__amxPi`
- `amxHidePlayers__FP5__amxPi`
- `amxHitPlayers__FP5__amxPi`
- `amxHoldVehicle__FP5__amxPi`
- `amxHornOmen__FP5__amxPi`
- `amxHudFadeScreen__FP5__amxPi`
- `amxHudResetFadeScreen__FP5__amxPi`
- `amxLeaveGame__FP5__amxPi`
- `amxLevelLoading__FP5__amxPi`
- `amxLightEffect__FP5__amxPi`
- `amxLoadCheckPoint__FP5__amxPi`
- `amxMoveMonster__FP5__amxPi`
- `amxNetworkMode__FP5__amxPi`
- `amxNewWorld__FP5__amxPi`
- `amxNewWorldParameter__FP5__amxPi`
- `amxNewWorldPost__FP5__amxPi`
- `amxNPCAnimAtEnd__FP5__amxPi`
- `amxObjectInPlayerTrigger__FP5__amxPi`
- `amxParticleMorph__FP5__amxPi`
- `amxPauseGameControl__FP5__amxPi`
- `amxPlanarSteel__FP5__amxPi`
- `amxPlayGotItemSound__FP5__amxPi`
- `amxPlayerHasItem__FP5__amxPi`
- `amxPlayerSelRunning__FP5__amxPi`
- `amxPlayersAlive__FP5__amxPi`
- `amxPlayersLoaded__FP5__amxPi`
- `amxPreloadResource__FP5__amxPi`
- `amxPrint__FP5__amxPi`
- `amxPrintVal__FP5__amxPi`
- `amxPropAnimAtEnd__FP5__amxPi`
- `amxPutNPCInVehicle__FP5__amxPi`

## Imports opened

Undefined symbols in these EE objects. Data names already defined by other packages stay unresolved only until those units are linked: `gameDifficulty`, `levelName`, `netNetworkMode`, `nmPlayers`, `norrath_constructor_random_state_view`, `norrath_player_progression_records_view`, `norrath_player_skill_catalog_view`, and `norrath_route_nodes_bytes`. `strcpy`, `strncmp`, and `strstr` are libc. The remaining names are still open.

- `AddNPCDriver__7VehicleP3NPC`
- `DLG_NewWorld`
- `SetHold__7Vehiclei`
- `__11LightEffectG6Point3ffffffffffffP10GameObjecti`
- `__19ParticleMorphEffectP8CreatureT1`
- `__19SpellHornOmenEffectiP8CreatureP5Skill`
- `__22SpellPlanarSteelEffectiP8CreatureP5Skill`
- `__8ItemPropG6Point3PQ24item11descriptioni`
- `__tf7Trigger`
- `activePlayer`
- `addItemToInventory__FiPQ24item11descriptioni`
- `addSpell__8CreatureP5Spell`
- `amx_GetAddr`
- `amx_GetString`
- `blockAlloc__Fi`
- `createRandom__4itemPQ24item11descriptioniiii`
- `create__4itemPcPQ24item11descriptionii`
- `currentRunningObject`
- `doNewWorld__FPc`
- `equipItem__FiPQ24item11descriptioni`
- `gameBailFromGameLoop`
- `gameControlDontResetPause`
- `gameControlPaused`
- `gameDeathStart__Fi`
- `gameDifficulty`
- `gameHidePlayers`
- `gameItemTreasureRamp__Fv`
- `gameLevelLoading`
- `gameNetKick__Fi`
- `gameRecallInhibit`
- `getWeight__4itemPQ24item11description`
- `givePlayerExperience__Fii`
- `hudFadeScreen__Fi`
- `hudResetFadeScreen__Fi`
- `hudSetInfoMessage__FPUsiic`
- `iatan2__Fff`
- `levelName`
- `lumpFindResource__FPcT0`
- `lumpLoad__FPc`
- `netGetLocalPlayerNm__Fi`
- `netNetworkMode`
- `netPlayerIsLocal__Fi`
- `nmPlayers`
- `nmPlayersAlive`
- `norrath_constructor_random_state_view`
- `norrath_player_progression_records_view`
- `norrath_player_skill_catalog_view`
- `norrath_route_nodes_bytes`
- `objectFindObjectByClass__FPcPP10GameObjecti`
- `objectFindObject__FPc`
- `objectFindObject__FPcPP10GameObjecti`
- `objectRemoveFromGrid__FP10GameObject`
- `objectUpdateInGrid__FP10GameObject`
- `playerGetFeatRank__Fix`
- `playerPlaySound__Fii`
- `playerRecomputeWeight__Fi`
- `playerSelectionRunning__Fv`
- `queryInventoryItem__FPci`
- `scriptGetGameState__FPc`
- `scriptInPostAction`
- `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`
- `strcpy`
- `strncmp`
- `strstr`
- `textGetString__FPc`
- `worldDifficulty`

## Build-plan entries needed

Append these to `experimental/build-plan.json` `inputs`. Profile `authored_cpp`, kind `independently_authored_source`, empty `header_closure`. `input_order` assumes the reconstruction tip still ends at 505. Renumber if that tip has moved. `experimental/build-plan.json` and `experimental/snapshot.json` were not edited.

```json
[
  {
    "header_closure": [],
    "input_order": 506,
    "kind": "independently_authored_source",
    "object_name": "get_save_inhibit.o",
    "profile": "authored_cpp",
    "sha256": "124c3ae09c453ce473d237bef6cec45942c78438a20e5caed1483fb2cddf37d3",
    "source": "natives-b-get-save-inhibit/get_save_inhibit.cpp"
  },
  {
    "header_closure": [],
    "input_order": 507,
    "kind": "independently_authored_source",
    "object_name": "get_scaled_time.o",
    "profile": "authored_cpp",
    "sha256": "98b9b0dfa87836679ea9544f7f543ad49c9c4ce3bb84091365489bc92ce24a19",
    "source": "natives-b-get-scaled-time/get_scaled_time.cpp"
  },
  {
    "header_closure": [],
    "input_order": 508,
    "kind": "independently_authored_source",
    "object_name": "get_v_position.o",
    "profile": "authored_cpp",
    "sha256": "6abdeba25be424b26d0591b401cf35f7ccbc84eb90c23269b0e47405fc68e0cb",
    "source": "natives-b-get-v-position/get_v_position.cpp"
  },
  {
    "header_closure": [],
    "input_order": 509,
    "kind": "independently_authored_source",
    "object_name": "getv.o",
    "profile": "authored_cpp",
    "sha256": "5974fac40e614f3144111c17dc810299daa30c28f9e9a38907a26bc852b9cc17",
    "source": "natives-b-getv/getv.cpp"
  },
  {
    "header_closure": [],
    "input_order": 510,
    "kind": "independently_authored_source",
    "object_name": "give_player_exp.o",
    "profile": "authored_cpp",
    "sha256": "2b93b1fdc71eccac6a38f0e3f6621f401140178c88a091eaea52e3afbfb7327f",
    "source": "natives-b-give-player-exp/give_player_exp.cpp"
  },
  {
    "header_closure": [],
    "input_order": 511,
    "kind": "independently_authored_source",
    "object_name": "give_player_gold.o",
    "profile": "authored_cpp",
    "sha256": "ee49c603b1580a273bc9364d8fc6834f9a0def883aad9d97ca2ae68da7975367",
    "source": "natives-b-give-player-gold/give_player_gold.cpp"
  },
  {
    "header_closure": [],
    "input_order": 512,
    "kind": "independently_authored_source",
    "object_name": "give_player_item.o",
    "profile": "authored_cpp",
    "sha256": "f7bfc4366d32826158e576e0ecf46df021aa45e2ea18445a00033b233d42694a",
    "source": "natives-b-give-player-item/give_player_item.cpp"
  },
  {
    "header_closure": [],
    "input_order": 513,
    "kind": "independently_authored_source",
    "object_name": "give_player_random_item.o",
    "profile": "authored_cpp",
    "sha256": "5733625b16d84789f6bfee623727dca7e81549ed112bba51d33366abef313212",
    "source": "natives-b-give-player-random-item/give_player_random_item.cpp"
  },
  {
    "header_closure": [],
    "input_order": 514,
    "kind": "independently_authored_source",
    "object_name": "heal_player.o",
    "profile": "authored_cpp",
    "sha256": "ea6f0fa9ba795c1eeab4b9c475dde6c59da6d2ab41c19893eb7514cab72a0dcc",
    "source": "natives-b-heal-player/heal_player.cpp"
  },
  {
    "header_closure": [],
    "input_order": 515,
    "kind": "independently_authored_source",
    "object_name": "hide_monster.o",
    "profile": "authored_cpp",
    "sha256": "859a1cc6a6f74aa4811123b5b6184d5cbe607779298b076158ac6fb312b4a125",
    "source": "natives-b-hide-monster/hide_monster.cpp"
  },
  {
    "header_closure": [],
    "input_order": 516,
    "kind": "independently_authored_source",
    "object_name": "hide_players.o",
    "profile": "authored_cpp",
    "sha256": "d900f5b7d8654bf03a055e18bd43fe09cc2b68b060e46d0649bace10e1041d15",
    "source": "natives-b-hide-players/hide_players.cpp"
  },
  {
    "header_closure": [],
    "input_order": 517,
    "kind": "independently_authored_source",
    "object_name": "hit_players.o",
    "profile": "authored_cpp",
    "sha256": "1f08e9c08f864cfc1eae9c1d4efb508c9179c785c08dec8bf63f6b646581b5f7",
    "source": "natives-b-hit-players/hit_players.cpp"
  },
  {
    "header_closure": [],
    "input_order": 518,
    "kind": "independently_authored_source",
    "object_name": "hold_vehicle.o",
    "profile": "authored_cpp",
    "sha256": "9a7f76887f574b2c1b055a70a64a277d40c5f882cf0362d626c3c5494e06a8e7",
    "source": "natives-b-hold-vehicle/hold_vehicle.cpp"
  },
  {
    "header_closure": [],
    "input_order": 519,
    "kind": "independently_authored_source",
    "object_name": "horn_omen.o",
    "profile": "authored_cpp",
    "sha256": "1b7d3f19f8d1568ed7c919630439d60dbe74cac8003673cac366d120828fd852",
    "source": "natives-b-horn-omen/horn_omen.cpp"
  },
  {
    "header_closure": [],
    "input_order": 520,
    "kind": "independently_authored_source",
    "object_name": "hud_fade_screen.o",
    "profile": "authored_cpp",
    "sha256": "530846a4c274af968870244083952fcaa57e49cc56170e59be8c029bd14fd180",
    "source": "natives-b-hud-fade-screen/hud_fade_screen.cpp"
  },
  {
    "header_closure": [],
    "input_order": 521,
    "kind": "independently_authored_source",
    "object_name": "hud_reset_fade_screen.o",
    "profile": "authored_cpp",
    "sha256": "0eae2cc6128fd797707a6605482f69a6780a7cbf8074e059bc3d9a43b8699256",
    "source": "natives-b-hud-reset-fade-screen/hud_reset_fade_screen.cpp"
  },
  {
    "header_closure": [],
    "input_order": 522,
    "kind": "independently_authored_source",
    "object_name": "leave_game.o",
    "profile": "authored_cpp",
    "sha256": "862dc5d78edc5728b4ccb818ae0e43695a45dca458f68619f9029529704145c2",
    "source": "natives-b-leave-game/leave_game.cpp"
  },
  {
    "header_closure": [],
    "input_order": 523,
    "kind": "independently_authored_source",
    "object_name": "level_loading.o",
    "profile": "authored_cpp",
    "sha256": "eced1c88e3ebb3a0d03c7914f50ba4ee23647a2df6ebc4fed7ab2321cb315067",
    "source": "natives-b-level-loading/level_loading.cpp"
  },
  {
    "header_closure": [],
    "input_order": 524,
    "kind": "independently_authored_source",
    "object_name": "light_effect.o",
    "profile": "authored_cpp",
    "sha256": "0844fa2a4c46e6f91587a94e4493ea9cdec67b095772255115665d6ed907c657",
    "source": "natives-b-light-effect/light_effect.cpp"
  },
  {
    "header_closure": [],
    "input_order": 525,
    "kind": "independently_authored_source",
    "object_name": "load_check_point.o",
    "profile": "authored_cpp",
    "sha256": "d16b5d27764376ffc7af41ce02f5e7ff6f63196202be56a37f522015fc0ffd8d",
    "source": "natives-b-load-check-point/load_check_point.cpp"
  },
  {
    "header_closure": [],
    "input_order": 526,
    "kind": "independently_authored_source",
    "object_name": "move_monster.o",
    "profile": "authored_cpp",
    "sha256": "b3e6f1d3cb91483560aad3849c547ddae7494b6cfa82e899216d533e23e6ec16",
    "source": "natives-b-move-monster/move_monster.cpp"
  },
  {
    "header_closure": [],
    "input_order": 527,
    "kind": "independently_authored_source",
    "object_name": "network_mode.o",
    "profile": "authored_cpp",
    "sha256": "8c2e8b15b94c0c894b3207a9fa5252c0f9b8d573e23f30ea59af51eaf326d2d9",
    "source": "natives-b-network-mode/network_mode.cpp"
  },
  {
    "header_closure": [],
    "input_order": 528,
    "kind": "independently_authored_source",
    "object_name": "new_world.o",
    "profile": "authored_cpp",
    "sha256": "8bbeba9c0adebbbdfbce30a9fd67022d21f0362628c8c2bc5fbfc6d2ea95c79f",
    "source": "natives-b-new-world/new_world.cpp"
  },
  {
    "header_closure": [],
    "input_order": 529,
    "kind": "independently_authored_source",
    "object_name": "new_world_parameter.o",
    "profile": "authored_cpp",
    "sha256": "12187dbc7481126692362d754d53e9b9ce01a62605be5c9f6aada8e7888de2f0",
    "source": "natives-b-new-world-parameter/new_world_parameter.cpp"
  },
  {
    "header_closure": [],
    "input_order": 530,
    "kind": "independently_authored_source",
    "object_name": "new_world_post.o",
    "profile": "authored_cpp",
    "sha256": "ed081a866851a956469ae8421ab1a6232621a882b67725fd0f09c1cf0808e47c",
    "source": "natives-b-new-world-post/new_world_post.cpp"
  },
  {
    "header_closure": [],
    "input_order": 531,
    "kind": "independently_authored_source",
    "object_name": "npc_anim_at_end.o",
    "profile": "authored_cpp",
    "sha256": "29b421895edd5b6cdbaa4de6bb80c279c4eae42fe3f0ecd6bb17da19ba656d87",
    "source": "natives-b-npc-anim-at-end/npc_anim_at_end.cpp"
  },
  {
    "header_closure": [],
    "input_order": 532,
    "kind": "independently_authored_source",
    "object_name": "object_in_player_trigger.o",
    "profile": "authored_cpp",
    "sha256": "8193fd09f05a03fc6cb010d64523aa51f1fb6c4ad426c3dfb28811eac1d161c2",
    "source": "natives-b-object-in-player-trigger/object_in_player_trigger.cpp"
  },
  {
    "header_closure": [],
    "input_order": 533,
    "kind": "independently_authored_source",
    "object_name": "particle_morph.o",
    "profile": "authored_cpp",
    "sha256": "822cc773960c8c1ce1067b297d7f89c32d9b35454460c4d7d0c0d50bb0dccaa2",
    "source": "natives-b-particle-morph/particle_morph.cpp"
  },
  {
    "header_closure": [],
    "input_order": 534,
    "kind": "independently_authored_source",
    "object_name": "pause_game_control.o",
    "profile": "authored_cpp",
    "sha256": "fbec533e661d1695b9536d3cc4d4af9d3f4d3fe101624b53ca70b9a6f14d14ee",
    "source": "natives-b-pause-game-control/pause_game_control.cpp"
  },
  {
    "header_closure": [],
    "input_order": 535,
    "kind": "independently_authored_source",
    "object_name": "planar_steel.o",
    "profile": "authored_cpp",
    "sha256": "796e72022b4a0df8e3d2842caf3ceec8529830e81f320e4279a9075ba5ecdb7a",
    "source": "natives-b-planar-steel/planar_steel.cpp"
  },
  {
    "header_closure": [],
    "input_order": 536,
    "kind": "independently_authored_source",
    "object_name": "play_got_item_sound.o",
    "profile": "authored_cpp",
    "sha256": "35ccb6743bf6c54f641bc02191425dceb4d7453e3bf17e9efeacef19c28f3b84",
    "source": "natives-b-play-got-item-sound/play_got_item_sound.cpp"
  },
  {
    "header_closure": [],
    "input_order": 537,
    "kind": "independently_authored_source",
    "object_name": "player_has_item.o",
    "profile": "authored_cpp",
    "sha256": "c58e7e7a38d20a4ca0d57008ee48e20f327279fb57ed1628515b9ffd3722a002",
    "source": "natives-b-player-has-item/player_has_item.cpp"
  },
  {
    "header_closure": [],
    "input_order": 538,
    "kind": "independently_authored_source",
    "object_name": "player_sel_running.o",
    "profile": "authored_cpp",
    "sha256": "d67bfdf5bbf171912586a739c3d0d11d6acbcbb780db83d63082cc7bf449c4cc",
    "source": "natives-b-player-sel-running/player_sel_running.cpp"
  },
  {
    "header_closure": [],
    "input_order": 539,
    "kind": "independently_authored_source",
    "object_name": "players_alive.o",
    "profile": "authored_cpp",
    "sha256": "5586dcbc052723c5f29e8d6303cd4d9b08f591f113376db682ac7303327a7d33",
    "source": "natives-b-players-alive/players_alive.cpp"
  },
  {
    "header_closure": [],
    "input_order": 540,
    "kind": "independently_authored_source",
    "object_name": "players_loaded.o",
    "profile": "authored_cpp",
    "sha256": "b7c537e1ee2dec85b547a98830271886ffe2990fe8e64174e56dc8c1053de767",
    "source": "natives-b-players-loaded/players_loaded.cpp"
  },
  {
    "header_closure": [],
    "input_order": 541,
    "kind": "independently_authored_source",
    "object_name": "preload_resource.o",
    "profile": "authored_cpp",
    "sha256": "eb0424104343fa162e2b8150dd70f2bdb8231a915d4cb3b49b6ce00c25b1652b",
    "source": "natives-b-preload-resource/preload_resource.cpp"
  },
  {
    "header_closure": [],
    "input_order": 542,
    "kind": "independently_authored_source",
    "object_name": "print.o",
    "profile": "authored_cpp",
    "sha256": "2fb0cee7a1c43618a88bd95679210d06e81341d154905a0a86610f5dfc90eb78",
    "source": "natives-b-print/print.cpp"
  },
  {
    "header_closure": [],
    "input_order": 543,
    "kind": "independently_authored_source",
    "object_name": "print_val.o",
    "profile": "authored_cpp",
    "sha256": "60823e922cf5f75a8e735461ecd48dacf93bbeced84381d82e45274a48572248",
    "source": "natives-b-print-val/print_val.cpp"
  },
  {
    "header_closure": [],
    "input_order": 544,
    "kind": "independently_authored_source",
    "object_name": "prop_anim_at_end.o",
    "profile": "authored_cpp",
    "sha256": "80e6099b40dfcc46bca4e6328531e06b066ddbdfc9e8cdca7a95b6a466fe4dce",
    "source": "natives-b-prop-anim-at-end/prop_anim_at_end.cpp"
  },
  {
    "header_closure": [],
    "input_order": 545,
    "kind": "independently_authored_source",
    "object_name": "put_npc_in_vehicle.o",
    "profile": "authored_cpp",
    "sha256": "5a192e0e70281d9af394c0dcdd209bc2853b0065a0fa544481bac96edd722424",
    "source": "natives-b-put-npc-in-vehicle/put_npc_in_vehicle.cpp"
  }
]
```

## Authored but never executed

All 40 EE objects above were compiled with the reviewed toolchain and checked by a host test. None has been linked into the shared partial build, loaded by the game, or executed on the EE. A fresh partial build and its `unresolved-imports.json` were not run. The closed names are the ELF slice, not a link map.

## Unknown

The bytes of an item description outside the stores listed above are whatever was on the retail stack. Zeroing them is local to this reconstruction. Whether `create` reads those bytes was not recovered. The retail register layout of `LightEffect`, `ItemProp`, `ParticleMorphEffect`, the hit virtual call, and the fifth `createRandom` argument was not reproduced beyond the C call. What writes the player-index words at `nodes+5344`, and why the treasure search starts at `levelName+65`, was not recovered. No alias was added from `player` to the progression view, from `skills` to the catalog view, or from `syncRandState` to the random-state view.
