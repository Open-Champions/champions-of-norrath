# script-amx

Package notes for the script interpreter. The held draft in `experimental/sources/game-amx-exec-native-transport-source-v4` is unchanged and still stops on `#error`. `norrath_amx_exec` is a separate cell executor under `experimental/sources/script-amx-exec/`.

Observed retail anchors, used only as local reference: `amx_Exec` is the direct-threaded body; `nativeList` is 976 bytes (121 name/function pairs and a zero terminator); `amx_LittleEndian` is a 4-byte zero; `curScriptPtr` is a 4-byte `.sbss` word; `complainFrame.672` is the signed word `-10000`; `gameStateData` starts 8192 bytes before `postAction`; `postAction` is 200 bytes before the next symbol; the two `scriptStart` names on the memory-card load path are `preStart` and `start`. `newWorld` and `newWorldAlways` share `amxNewWorld__FP5__amxPi`.

Inferred from those spans: the game-state table is 8192 bytes and the post-action buffer is 200 bytes. Both are zero-filled. The little-endian word is an explicit zero. Under the reviewed `authored_cpp` profile it lands in BSS; the retail word is in `.sdata`. The value is zero either way. `-fno-zero-initialized-in-bss` is not part of that profile.

The cell executor publishes handler ids `0..130`. Existing relocation treats `table[opcode]` as the cell it writes back, so an opcode cell stays an opcode number. It does not reproduce original handler addresses or the direct-threaded jump. Absolute jump operands are still addresses after relocation. The host contract (`NORRATH_HOST_CONTRACT`) maps those addresses through `norrath_amx_exec_host_base`. Those host symbols are not in the EE object. The EE object keeps the id table in an anonymous-namespace BSS object.

`norrath_amx_exec` calls the function pointer stored at AMX+4. It does not reference `norrath_amx_callback`. On the host, that call goes through `norrath_amx_exec_host_callback` because a host function pointer does not fit in the 4-byte AMX slot. The first PRI is set to zero before the first cell; the original left that register uninitialized. Signed division of the pair whose dividend is the minimum 32-bit value and whose divisor is `-1` returns a defined stand-in. That pair's hardware HI/LO result was not recovered.

`norrath_custom_variables_process` matches the observed tag order, including `waterColor2` before `waterColor`. Failed conversions use zeroed locals; the original left those locals uninitialized. `camMaxHeight` must match at the start of the string.

The native registry names the original `amx*__FP5__amxPi` functions. Existing `norrath_amx_native_*` definitions do not satisfy those names. `script.scr`, the lump format, and the other memory-card keys are not the two script entry keys.

Each EE translation unit below was compiled with the reviewed `authored_cpp` flags. Each has a host test that failed when the behavior was wrong. None of this has been linked into a partial image or run as the game.

## Snapshot files

`tools/check_source_snapshot.py` requires every file under `experimental/sources/` to appear in `experimental/snapshot.json` `files`. These paths are not recorded yet. Host tests are snapshot files only. They are not EE plan inputs.

```
c7cf0e02444eedf985309a5083e5ec1bae2b9cb1b5367c2a36c32e7e9337628d  sources/script-amx-complain-frame/complain_frame.cpp
42c7f4039a4f5cdc44f1c33746bf37ac833b0f95332f7b5253b34c929ca77bc3  sources/script-amx-complain-frame/complain_frame_host.cpp
4a8611d2a19d69c28ddd05ddad4ed01ccd365becb384ccf253880ccd8d6ad88a  sources/script-amx-current-amx/current_amx.cpp
aa1046ce309503eb8d54ecfb587b1dbb346708820f0bda49955ff190f6ad5c55  sources/script-amx-current-amx/current_amx_host.cpp
af764d9d3022a2524909819db9c2c54b179726f72f030542e6c214f6b7eccb12  sources/script-amx-custom-variables/custom_variables.cpp
e7f5852e82e4b54ae16d3ca485aac6825def748534da892a014abad9b67269c1  sources/script-amx-custom-variables/custom_variables_host.cpp
bf98c2664fd9fb9f0cc1fbdad7dfb263d695fb73d86d956b41051b5b5a4b3e44  sources/script-amx-exec/exec.cpp
70424707261e3635b6b5fede3cf0a3addea3a7e88a795c45ac04af0a2d7a8efb  sources/script-amx-exec/exec_host.cpp
0c8aba04707d4f91a4e0cabd897a26878f43e50e5e2f9933bab5df53bcb4e489  sources/script-amx-game-state/game_state.cpp
4a7ecea752afe065af4356d31f6abc22a60a11d8eea2262f15b19551e226d574  sources/script-amx-game-state/game_state_host.cpp
d785940e1d075848b4e5dabca6573b0eaaa3ba38776da689a71a5be967328322  sources/script-amx-little-endian/little_endian.cpp
31c0bc47e0b6068ba4be14dff1cd5667f3349232d8c809231b700396914c53f4  sources/script-amx-little-endian/little_endian_host.cpp
145cd211f56f0e0ee728eae103232902c5d654a3dc71810bc34070d7d29f26aa  sources/script-amx-memory-card-keys/keys.cpp
e617d4eb462a80f30d144e7d943a148bc4d6b5c6b9c1fd6f44b130121d70f011  sources/script-amx-memory-card-keys/keys_host.cpp
a4daf596c59cd27fc7bc85091605497ce448de324e0e379b0a3b65e1bd29923b  sources/script-amx-native-registry/registry.cpp
9f62aba5a36960e52cf212d9a63eb4d27f4b897bf2f83264f15bbd1ba5ca8933  sources/script-amx-native-registry/registry_host.cpp
0424e945a8608c4c12a4978d389e5bfe5ed2efe5d39a46f6feea5b9d0f18571f  sources/script-amx-post-action/post_action.cpp
73c888406f60754402b8bd518694647610a2ec4221d5a877e34a3b494d4374af  sources/script-amx-post-action/post_action_host.cpp
```

## Build-plan entries

Append these to `experimental/build-plan.json` `inputs`. Profile `authored_cpp`, kind `independently_authored_source`, empty `header_closure`. The `input_order` values assume the reconstruction tip still ends at 311. Renumber if that tip has moved. A private copy of the plan was not committed.

```json
[
  {
    "header_closure": [],
    "input_order": 312,
    "kind": "independently_authored_source",
    "object_name": "custom_variables.o",
    "profile": "authored_cpp",
    "sha256": "af764d9d3022a2524909819db9c2c54b179726f72f030542e6c214f6b7eccb12",
    "source": "sources/script-amx-custom-variables/custom_variables.cpp"
  },
  {
    "header_closure": [],
    "input_order": 313,
    "kind": "independently_authored_source",
    "object_name": "current_amx.o",
    "profile": "authored_cpp",
    "sha256": "4a8611d2a19d69c28ddd05ddad4ed01ccd365becb384ccf253880ccd8d6ad88a",
    "source": "sources/script-amx-current-amx/current_amx.cpp"
  },
  {
    "header_closure": [],
    "input_order": 314,
    "kind": "independently_authored_source",
    "object_name": "complain_frame.o",
    "profile": "authored_cpp",
    "sha256": "c7cf0e02444eedf985309a5083e5ec1bae2b9cb1b5367c2a36c32e7e9337628d",
    "source": "sources/script-amx-complain-frame/complain_frame.cpp"
  },
  {
    "header_closure": [],
    "input_order": 315,
    "kind": "independently_authored_source",
    "object_name": "little_endian.o",
    "profile": "authored_cpp",
    "sha256": "d785940e1d075848b4e5dabca6573b0eaaa3ba38776da689a71a5be967328322",
    "source": "sources/script-amx-little-endian/little_endian.cpp"
  },
  {
    "header_closure": [],
    "input_order": 316,
    "kind": "independently_authored_source",
    "object_name": "game_state.o",
    "profile": "authored_cpp",
    "sha256": "0c8aba04707d4f91a4e0cabd897a26878f43e50e5e2f9933bab5df53bcb4e489",
    "source": "sources/script-amx-game-state/game_state.cpp"
  },
  {
    "header_closure": [],
    "input_order": 317,
    "kind": "independently_authored_source",
    "object_name": "post_action.o",
    "profile": "authored_cpp",
    "sha256": "0424e945a8608c4c12a4978d389e5bfe5ed2efe5d39a46f6feea5b9d0f18571f",
    "source": "sources/script-amx-post-action/post_action.cpp"
  },
  {
    "header_closure": [],
    "input_order": 318,
    "kind": "independently_authored_source",
    "object_name": "memory_card_keys.o",
    "profile": "authored_cpp",
    "sha256": "145cd211f56f0e0ee728eae103232902c5d654a3dc71810bc34070d7d29f26aa",
    "source": "sources/script-amx-memory-card-keys/keys.cpp"
  },
  {
    "header_closure": [],
    "input_order": 319,
    "kind": "independently_authored_source",
    "object_name": "native_registry.o",
    "profile": "authored_cpp",
    "sha256": "a4daf596c59cd27fc7bc85091605497ce448de324e0e379b0a3b65e1bd29923b",
    "source": "sources/script-amx-native-registry/registry.cpp"
  },
  {
    "header_closure": [],
    "input_order": 320,
    "kind": "independently_authored_source",
    "object_name": "amx_exec.o",
    "profile": "authored_cpp",
    "sha256": "bf98c2664fd9fb9f0cc1fbdad7dfb263d695fb73d86d956b41051b5b5a4b3e44",
    "source": "sources/script-amx-exec/exec.cpp"
  }
]
```

## Imports closed

- `norrath_custom_variables_process`
- `norrath_script_current_amx_word_view`
- `norrath_script_complain_frame_word_view`
- `norrath_amx_little_endian_word_view`
- `norrath_script_game_state_data_view`
- `norrath_script_post_action_view`
- `norrath_mc_pre_start_key_bytes`
- `norrath_mc_start_key_bytes`
- `norrath_amx_game_native_registry_view`
- `norrath_amx_exec`

## Imports opened

`norrath_amx_exec` references `norrath_copy_bytes` and `norrath_memory_compare`. Both already exist in other packages. It also references `norrath_amx_little_endian_word_view`, which this branch defines.

`norrath_custom_variables_process` references `norrath_factory_strstr`, `norrath_player_sscanf`, `SetMaxHeight__6Cameraf`, `SetPitchAtLevel__6Cameraii`, `DisableZooming__6Camera`, `SetDistanceAtLevel__6Cameraif`, `fxUpdateDust__Fiiiiiiii`, `gameCam`, `levelFlags`, `worldWaterAlpha`, `worldWaterAlpha2`, `worldWaterColor`, `worldWaterColor2`, `worldWaterBumpAmount`, `worldWaterBumpRadius`, `worldWaterBumpTimer`, `worldWaterWavyness`, and `worldWaterWavyness2`. `gameCam` is the camera pointer stored in that word.

`norrath_amx_game_native_registry_view` references these unresolved functions. `amxNewWorld__FP5__amxPi` is used twice:

`amxCallScript__FP5__amxPi`, `amxPrint__FP5__amxPi`, `amxPrintVal__FP5__amxPi`, `amxSound__FP5__amxPi`, `amxSoundSequence__FP5__amxPi`, `amxLightEffect__FP5__amxPi`, `amxSimpleNewWorld__FP5__amxPi`, `amxNewWorld__FP5__amxPi`, `amxNewWorldPost__FP5__amxPi`, `amxActivateShop__FP5__amxPi`, `amxShopRunning__FP5__amxPi`, `amxGetScaledTime__FP5__amxPi`, `amxSetFarClip__FP5__amxPi`, `amxStartDialog__FP5__amxPi`, `amxChangeSwoop__FP5__amxPi`, `amxStartWithSwoopDone__FP5__amxPi`, `amxDialogRunning__FP5__amxPi`, `amxDeleteMonster__FP5__amxPi`, `amxHideMonster__FP5__amxPi`, `amxShowMonster__FP5__amxPi`, `amxMoveMonster__FP5__amxPi`, `amxFollowPlayer__FP5__amxPi`, `amxStopFollowingPlayer__FP5__amxPi`, `amxFollowPath__FP5__amxPi`, `amxStopFollowingPath__FP5__amxPi`, `amxFriendly__FP5__amxPi`, `amxUnFriendly__FP5__amxPi`, `amxStartPropAnim__FP5__amxPi`, `amxStopPropAnim__FP5__amxPi`, `amxPropAnimAtEnd__FP5__amxPi`, `amxStartNPCAnim__FP5__amxPi`, `amxNPCAnimAtEnd__FP5__amxPi`, `amxAddExpandingPoolUnder__FP5__amxPi`, `amxPlayerHasItem__FP5__amxPi`, `amxRemovePlayerItem__FP5__amxPi`, `amxGivePlayerItem__FP5__amxPi`, `amxGivePlayerRandomItem__FP5__amxPi`, `amxGivePlayerExp__FP5__amxPi`, `amxGivePlayerGold__FP5__amxPi`, `amxTakePlayerGold__FP5__amxPi`, `amxCheckPlayerGold__FP5__amxPi`, `amxPlayGotItemSound__FP5__amxPi`, `amxSetInfo__FP5__amxPi`, `amxAddHelpMessage__FP5__amxPi`, `amxAddQuest__FP5__amxPi`, `amxRemoveQuest__FP5__amxPi`, `amxHornOmen__FP5__amxPi`, `amxPlanarSteel__FP5__amxPi`, `amxParticleMorph__FP5__amxPi`, `amxHealPlayer__FP5__amxPi`, `amxLeaveGame__FP5__amxPi`, `amxEndGame__FP5__amxPi`, `amxEndGauntlet__FP5__amxPi`, `amxClearRecall__FP5__amxPi`, `amxStartMusic__FP5__amxPi`, `amxStartAudio__FP5__amxPi`, `amxEndMusic__FP5__amxPi`, `amxSaveGame__FP5__amxPi`, `amxSaveEndConfig__FP5__amxPi`, `amxSaveDone__FP5__amxPi`, `amxExitToMainMenu__FP5__amxPi`, `amxReapEnemies__FP5__amxPi`, `amxGetv__FP5__amxPi`, `amxGetNumPlayers__FP5__amxPi`, `amxSetv__FP5__amxPi`, `amxAddv__FP5__amxPi`, `amxGetRand__FP5__amxPi`, `amxActivePlayerIsDwarf__FP5__amxPi`, `amxActivePlayerIsElf__FP5__amxPi`, `amxGamePaused__FP5__amxPi`, `amxPauseGameControl__FP5__amxPi`, `amxLevelLoading__FP5__amxPi`, `amxSetParticle__FP5__amxPi`, `amxAddParticleOnObject__FP5__amxPi`, `amxSetNoCollide__FP5__amxPi`, `amxShakeCamera__FP5__amxPi`, `amxDeleteObjects__FP5__amxPi`, `amxHidePlayers__FP5__amxPi`, `amxCheckPlayerAlive__FP5__amxPi`, `amxSetBestActivePlayer__FP5__amxPi`, `amxDecayPlayerLights__FP5__amxPi`, `amxCheckIfAnyMonstersRemain__FP5__amxPi`, `amxForceMutexRelease__FP5__amxPi`, `amxAddDeferedCall__FP5__amxPi`, `amxCutscene__FP5__amxPi`, `amxSetPostAction__FP5__amxPi`, `amxNewWorldParameter__FP5__amxPi`, `amxPutNPCInVehicle__FP5__amxPi`, `amxPutPlayersInVehicle__FP5__amxPi`, `amxEntityRideVehicle__FP5__amxPi`, `amxControlVehicle__FP5__amxPi`, `amxPlayersLoaded__FP5__amxPi`, `amxHoldVehicle__FP5__amxPi`, `amxGetPlayerVPosition__FP5__amxPi`, `amxGetVPosition__FP5__amxPi`, `amxGetPlayerVDir__FP5__amxPi`, `amxSetupPlayerSel__FP5__amxPi`, `amxStartPlayerSel__FP5__amxPi`, `amxPlayerSelRunning__FP5__amxPi`, `amxObjectInPlayerTrigger__FP5__amxPi`, `amxSetEntrance__FP5__amxPi`, `amxHudResetFadeScreen__FP5__amxPi`, `amxHudFadeScreen__FP5__amxPi`, `amxClosestPlayerDist__FP5__amxPi`, `amxHitPlayers__FP5__amxPi`, `amxShowCompass__FP5__amxPi`, `amxShowBust__FP5__amxPi`, `amxGetHitPoints__FP5__amxPi`, `amxSetHitPoints__FP5__amxPi`, `amxSaveCheckPoint__FP5__amxPi`, `amxLoadCheckPoint__FP5__amxPi`, `amxTrigger__FP5__amxPi`, `amxAutoPropSetState__FP5__amxPi`, `amxPreloadResource__FP5__amxPi`, `amxSetSaveInhibit__FP5__amxPi`, `amxGetSaveInhibit__FP5__amxPi`, `amxCountQuestItems__FP5__amxPi`, `amxPlayersAlive__FP5__amxPi`, `amxCountLiveObjects__FP5__amxPi`, `amxNetworkMode__FP5__amxPi`.

## Authored but never executed

The host tests executed the entry checks, a public lookup, parameter copy, halt, add, the adjusted signed division of `-7` by `3`, relative jump slot 52, sign-byte slot 93, endian slot 29, bad opcode 0, callback slots that return or fail, and copy slot 117. The EE objects were compiled and not run. No script from a retail image was executed. The held direct-threaded draft was not executed.

Slots implemented in `norrath_amx_exec` and not run by the host test: 2–8, 10, 13, 14, 17–22, 24–28, 30, 32–51, 53–72, 74–77, 79–92, 94–116, 118, 119, and 121–130.

## Unknown

The retail disc revision is whatever the local reference ELF is; this package does not identify another build. Original handler addresses, the physical frame, HI/LO banks, 128-bit spills, and interrupt or fault behavior inside `amx_Exec` are unknown. The minimum-negative divided by `-1` hardware result is unknown. The original uninitialized PRI and the original uninitialized `cvProcess` conversion locals are unknown values, so this source does not invent them. The C++ type of `gameStateData` beyond the 8192-byte span is unknown. Whether `postAction`'s 200 bytes include padding is unknown; the gap to the next symbol is 200. `script.scr` and the remaining memory-card keys were left unresolved on purpose. How the original `amx*` natives correspond to `norrath_amx_native_*` names is unknown.
