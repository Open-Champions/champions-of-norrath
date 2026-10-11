# script-amx

Package notes for the script interpreter. The held draft in `experimental/sources/game-amx-exec-native-transport-source-v4` is unchanged and still stops on `#error`. `norrath_amx_exec` is a separate cell executor under `experimental/sources/script-amx-exec/`.

Observed retail anchors, used only as local reference: `amx_Exec` is the direct-threaded body; `nativeList` is 976 bytes (121 name/function pairs and a zero terminator); `amx_LittleEndian` is a 4-byte zero; `curScriptPtr` is a 4-byte `.sbss` word; `complainFrame.672` is the signed word `-10000`; `gameStateData` starts 8192 bytes before `postAction`; `postAction` is 200 bytes before the next symbol; the two `scriptStart` names on the memory-card load path are `preStart` and `start`. `newWorld` and `newWorldAlways` share `amxNewWorld__FP5__amxPi`.

Inferred from those spans: the game-state table is 8192 bytes and the post-action buffer is 200 bytes. Both are zero-filled. The little-endian word is an explicit zero. Under the reviewed `authored_cpp` profile it lands in BSS; the retail word is in `.sdata`. The value is zero either way. `-fno-zero-initialized-in-bss` is not part of that profile.

The cell executor publishes handler ids `0..130`. Existing relocation treats `table[opcode]` as the cell it writes back, so an opcode cell stays an opcode number. It does not reproduce original handler addresses or the direct-threaded jump. Absolute jump operands are still addresses after relocation. The host contract (`NORRATH_HOST_CONTRACT`) maps those addresses through `norrath_amx_exec_host_base`. Those host symbols are not in the EE object. The EE object keeps the id table in an anonymous-namespace BSS object.

`norrath_amx_exec` calls the function pointer stored at AMX+4. It does not reference `norrath_amx_callback`. On the host, that call goes through `norrath_amx_exec_host_callback` because a host function pointer does not fit in the 4-byte AMX slot. The first PRI is set to zero before the first cell; the original left that register uninitialized. Signed division of the pair whose dividend is the minimum 32-bit value and whose divisor is `-1` returns a defined stand-in. That pair's hardware HI/LO result was not recovered.

`norrath_custom_variables_process` matches the observed tag order, including `waterColor2` before `waterColor`. Failed conversions use zeroed locals; the original left those locals uninitialized. `camMaxHeight` must match at the start of the string.

The native registry names the original `amx*__FP5__amxPi` functions. Existing `norrath_amx_native_*` definitions do not satisfy those names. `script.scr`, the lump format, and the other memory-card keys are not the two script entry keys.

`scriptInit__Fv` at `0x001d4010` is 52 bytes. It calls `engineAddTask__FPFv_viPci(scriptTask__Fv, 125, text, 0)` and then stores a zero word at `curScriptPtr`. That word is `norrath_script_current_amx_word_view`: same address `0x00486b94` and the same four-byte extent, so this function writes the existing object. The task text at `0x00485090` has no ELF symbol. The bytes are `Script` and its terminating zero. The following byte is `scriptInPostAction` and is not included. `scriptTask__Fv` at `0x001d3db0` is 148 bytes and stays an import.

Each EE translation unit below was compiled with the reviewed `authored_cpp` flags. Each has a host test that failed when the behavior was wrong. None of this has been linked into a partial image or run as the game.

## Snapshot files

`tools/check_source_snapshot.py` requires every file under `experimental/sources/` to appear in `experimental/snapshot.json` `files`. The earlier script-amx paths are already recorded. These two are not. The host test is a snapshot file only. It is not an EE plan input.

```
fd7991b049e7cae9a2505644f2f929aaea850cb40b62fdab7f7e330ba0035608  sources/script-amx-script-init/script_init.cpp
0cd4095f0eb43d120811a9b8c15c09bd36e546bb3bcb8d84be6f2311ec6def2f  sources/script-amx-script-init/script_init_host.cpp
```

## Build-plan entries

The nine earlier script-amx units are already selected in `experimental/build-plan.json`. Append this one. Profile `authored_cpp`, kind `independently_authored_source`, empty `header_closure`. `input_order` 423 assumes the reconstruction tip still ends at 422. Renumber if that tip has moved. No shared plan file was edited.

```json
{
  "header_closure": [],
  "input_order": 423,
  "kind": "independently_authored_source",
  "object_name": "script_init.o",
  "profile": "authored_cpp",
  "sha256": "fd7991b049e7cae9a2505644f2f929aaea850cb40b62fdab7f7e330ba0035608",
  "source": "sources/script-amx-script-init/script_init.cpp"
}
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
- `scriptInit__Fv`

## Imports opened

`scriptInit__Fv` references `engineAddTask__FPFv_viPci`, `scriptTask__Fv`, and `norrath_script_current_amx_word_view`. The current-AMX word is defined in this package. The scheduler and `scriptTask__Fv` stay unresolved.

`norrath_amx_exec` references `norrath_copy_bytes` and `norrath_memory_compare`. Both already exist in other packages. It also references `norrath_amx_little_endian_word_view`, which this branch defines.

`norrath_custom_variables_process` references `norrath_factory_strstr`, `norrath_player_sscanf`, `SetMaxHeight__6Cameraf`, `SetPitchAtLevel__6Cameraii`, `DisableZooming__6Camera`, `SetDistanceAtLevel__6Cameraif`, `fxUpdateDust__Fiiiiiiii`, `gameCam`, `levelFlags`, `worldWaterAlpha`, `worldWaterAlpha2`, `worldWaterColor`, `worldWaterColor2`, `worldWaterBumpAmount`, `worldWaterBumpRadius`, `worldWaterBumpTimer`, `worldWaterWavyness`, and `worldWaterWavyness2`. `gameCam` is the camera pointer stored in that word.

`norrath_amx_game_native_registry_view` references these unresolved functions. `amxNewWorld__FP5__amxPi` is used twice:

`amxCallScript__FP5__amxPi`, `amxPrint__FP5__amxPi`, `amxPrintVal__FP5__amxPi`, `amxSound__FP5__amxPi`, `amxSoundSequence__FP5__amxPi`, `amxLightEffect__FP5__amxPi`, `amxSimpleNewWorld__FP5__amxPi`, `amxNewWorld__FP5__amxPi`, `amxNewWorldPost__FP5__amxPi`, `amxActivateShop__FP5__amxPi`, `amxShopRunning__FP5__amxPi`, `amxGetScaledTime__FP5__amxPi`, `amxSetFarClip__FP5__amxPi`, `amxStartDialog__FP5__amxPi`, `amxChangeSwoop__FP5__amxPi`, `amxStartWithSwoopDone__FP5__amxPi`, `amxDialogRunning__FP5__amxPi`, `amxDeleteMonster__FP5__amxPi`, `amxHideMonster__FP5__amxPi`, `amxShowMonster__FP5__amxPi`, `amxMoveMonster__FP5__amxPi`, `amxFollowPlayer__FP5__amxPi`, `amxStopFollowingPlayer__FP5__amxPi`, `amxFollowPath__FP5__amxPi`, `amxStopFollowingPath__FP5__amxPi`, `amxFriendly__FP5__amxPi`, `amxUnFriendly__FP5__amxPi`, `amxStartPropAnim__FP5__amxPi`, `amxStopPropAnim__FP5__amxPi`, `amxPropAnimAtEnd__FP5__amxPi`, `amxStartNPCAnim__FP5__amxPi`, `amxNPCAnimAtEnd__FP5__amxPi`, `amxAddExpandingPoolUnder__FP5__amxPi`, `amxPlayerHasItem__FP5__amxPi`, `amxRemovePlayerItem__FP5__amxPi`, `amxGivePlayerItem__FP5__amxPi`, `amxGivePlayerRandomItem__FP5__amxPi`, `amxGivePlayerExp__FP5__amxPi`, `amxGivePlayerGold__FP5__amxPi`, `amxTakePlayerGold__FP5__amxPi`, `amxCheckPlayerGold__FP5__amxPi`, `amxPlayGotItemSound__FP5__amxPi`, `amxSetInfo__FP5__amxPi`, `amxAddHelpMessage__FP5__amxPi`, `amxAddQuest__FP5__amxPi`, `amxRemoveQuest__FP5__amxPi`, `amxHornOmen__FP5__amxPi`, `amxPlanarSteel__FP5__amxPi`, `amxParticleMorph__FP5__amxPi`, `amxHealPlayer__FP5__amxPi`, `amxLeaveGame__FP5__amxPi`, `amxEndGame__FP5__amxPi`, `amxEndGauntlet__FP5__amxPi`, `amxClearRecall__FP5__amxPi`, `amxStartMusic__FP5__amxPi`, `amxStartAudio__FP5__amxPi`, `amxEndMusic__FP5__amxPi`, `amxSaveGame__FP5__amxPi`, `amxSaveEndConfig__FP5__amxPi`, `amxSaveDone__FP5__amxPi`, `amxExitToMainMenu__FP5__amxPi`, `amxReapEnemies__FP5__amxPi`, `amxGetv__FP5__amxPi`, `amxGetNumPlayers__FP5__amxPi`, `amxSetv__FP5__amxPi`, `amxAddv__FP5__amxPi`, `amxGetRand__FP5__amxPi`, `amxActivePlayerIsDwarf__FP5__amxPi`, `amxActivePlayerIsElf__FP5__amxPi`, `amxGamePaused__FP5__amxPi`, `amxPauseGameControl__FP5__amxPi`, `amxLevelLoading__FP5__amxPi`, `amxSetParticle__FP5__amxPi`, `amxAddParticleOnObject__FP5__amxPi`, `amxSetNoCollide__FP5__amxPi`, `amxShakeCamera__FP5__amxPi`, `amxDeleteObjects__FP5__amxPi`, `amxHidePlayers__FP5__amxPi`, `amxCheckPlayerAlive__FP5__amxPi`, `amxSetBestActivePlayer__FP5__amxPi`, `amxDecayPlayerLights__FP5__amxPi`, `amxCheckIfAnyMonstersRemain__FP5__amxPi`, `amxForceMutexRelease__FP5__amxPi`, `amxAddDeferedCall__FP5__amxPi`, `amxCutscene__FP5__amxPi`, `amxSetPostAction__FP5__amxPi`, `amxNewWorldParameter__FP5__amxPi`, `amxPutNPCInVehicle__FP5__amxPi`, `amxPutPlayersInVehicle__FP5__amxPi`, `amxEntityRideVehicle__FP5__amxPi`, `amxControlVehicle__FP5__amxPi`, `amxPlayersLoaded__FP5__amxPi`, `amxHoldVehicle__FP5__amxPi`, `amxGetPlayerVPosition__FP5__amxPi`, `amxGetVPosition__FP5__amxPi`, `amxGetPlayerVDir__FP5__amxPi`, `amxSetupPlayerSel__FP5__amxPi`, `amxStartPlayerSel__FP5__amxPi`, `amxPlayerSelRunning__FP5__amxPi`, `amxObjectInPlayerTrigger__FP5__amxPi`, `amxSetEntrance__FP5__amxPi`, `amxHudResetFadeScreen__FP5__amxPi`, `amxHudFadeScreen__FP5__amxPi`, `amxClosestPlayerDist__FP5__amxPi`, `amxHitPlayers__FP5__amxPi`, `amxShowCompass__FP5__amxPi`, `amxShowBust__FP5__amxPi`, `amxGetHitPoints__FP5__amxPi`, `amxSetHitPoints__FP5__amxPi`, `amxSaveCheckPoint__FP5__amxPi`, `amxLoadCheckPoint__FP5__amxPi`, `amxTrigger__FP5__amxPi`, `amxAutoPropSetState__FP5__amxPi`, `amxPreloadResource__FP5__amxPi`, `amxSetSaveInhibit__FP5__amxPi`, `amxGetSaveInhibit__FP5__amxPi`, `amxCountQuestItems__FP5__amxPi`, `amxPlayersAlive__FP5__amxPi`, `amxCountLiveObjects__FP5__amxPi`, `amxNetworkMode__FP5__amxPi`.

## Authored but never executed

The host test for `scriptInit__Fv` checked the task registration, the text `Script`, the integer 125, the zero fourth argument, and that the current AMX word is cleared after the scheduler call. That function was compiled for the EE and not run. The host tests executed the entry checks, a public lookup, parameter copy, halt, add, the adjusted signed division of `-7` by `3`, relative jump slot 52, sign-byte slot 93, endian slot 29, bad opcode 0, callback slots that return or fail, and copy slot 117. The EE objects were compiled and not run. No script from a retail image was executed. The held direct-threaded draft was not executed. `scriptTask__Fv` was not reconstructed.

Slots implemented in `norrath_amx_exec` and not run by the host test: 2–8, 10, 13, 14, 17–22, 24–28, 30, 32–51, 53–72, 74–77, 79–92, 94–116, 118, 119, and 121–130.

## Unknown

The retail disc revision is whatever the local reference ELF is; this package does not identify another build. Original handler addresses, the physical frame, HI/LO banks, 128-bit spills, and interrupt or fault behavior inside `amx_Exec` are unknown. The minimum-negative divided by `-1` hardware result is unknown. The original uninitialized PRI and the original uninitialized `cvProcess` conversion locals are unknown values, so this source does not invent them. The C++ type of `gameStateData` beyond the 8192-byte span is unknown. Whether `postAction`'s 200 bytes include padding is unknown; the gap to the next symbol is 200. `script.scr` and the remaining memory-card keys were left unresolved on purpose. How the original `amx*` natives correspond to `norrath_amx_native_*` names is unknown. What the integer 125 selects inside `engineAddTask__FPFv_viPci` is unknown.
