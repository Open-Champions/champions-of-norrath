# memory-card calls

Package notes for `agents/memcard-calls`. Sources are under `experimental/sources/memcard-calls-*/`, one directory per symbol, each with the authored `.cpp` and a `host_contract.cpp` that includes it. The notes file was missing when this package was taken over; the 25 units were already committed (one per commit) and pushed. Per-function observations (addresses, sizes, store order) are recorded as Observed/Inferred/Unknown comments at the top of each source file.

## Checks performed

- All 25 units compile with the reviewed toolchain under the unchanged `authored_cpp` profile, in a private plan appended after the shared plan's 847 inputs (kept outside git). The partial build completed: 870 compilations, one link, 738 functions, 886 unresolved symbols. The build used a throwaway copy of `tools/build_partial.py` that takes each unit's function inventory from its own object, because the shared snapshot has no `emitted_functions` record for these units.
- None of the 25 owned names remains in `unresolved-imports.json`.
- Each `host_contract.cpp` builds with the host C++ compiler and exits 0 (25 of 25).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- No comparison against the private executable was run in this takeover pass, and nothing was executed by the EE.

## Imports closed

`MCA_ClearInfo__Fv`, `MCA_Cmd__Fiiii`, `MCA_Init__Fv`, `MCA_LastDirCount`, `MCA_Port`, `MCA_Slot`, `MCA_Sync__FPi`, `MC_Busy__Fv`, `MC_CheckPointAge`, `MC_Debug`, `MC_GameConfig`, `MC_GetAsyncMode__Fv`, `MC_GetReadyForMainMenu__Fv`, `MC_KickProtect`, `MC_LastModeSuccessful`, `MC_LoadGameBrowser__Fii`, `MC_NewGame__Fi`, `MC_SaveCheckPoint__Fiii`, `MC_SaveConfigSilent__Fii`, `MC_SaveConfig__Fv`, `MC_SaveGameBrowser__Fi`, `MC_SavingOrLoading`, `mcClientID`, `mcHeartBeatRun__Fv`, `mcHeartBeat__Fv`.

## Imports opened

Names these units reference that are still unresolved in the combined build and belong to other packages: `MCA_FileOpen`, `MCA_Formatted`, `MCA_Present`, `MCA_Process__FPi`, `MC_HasThisLevelBeenSaved__Fv`, `MC_ResetRamSlot__Fv`, `MC_SetNewAct__Fi`, `MC_SpawnPoint`, `actCount`, `actLevelCount`, `actionCoolOff`, `allActs`, `asyncChangePlayers__Fv`, `asyncConfigure__Fv`, `asyncLoadGame__Fv`, `asyncMode`, `asyncSaveConfigSilent__Fv`, `asyncSaveConfig__Fv`, `asyncSaveGame__Fv`, `commandQueue`, `curAct`, `curPlaySeconds`, `delayCount`, `disableMC`, `errorCondition`, `flareAlpha`, `flareDelta`, `frontEndRunning`, `frontEndWasRunning`, `fullLaunchOK`, `gameBailFromGameLoop`, `gameGetFullLevelName__FPc`, `gameNoDraw`, `gamePause`, `gameRecallInhibit`, `getLongLevelName__FPc`, `hudSetInfoMessage__FPUsiic`, `interfaceOn`, `netBlankController__Fii`, `newMode__Fii`, `player`, `playersAlive`, `pushPause__Fv`, `ranBrowser`, `ranConfig`, `resetQuickLoad__Fv`, `saveCheckPointCount`, `saveConfigEndGame`, `shopRunning`, `strlen`, `strncmp`, `strstr`, `textGetString__FPc`, `updateRamSlot__Fii`, `whichPad`.

`commandQueue` and `queueEnd`/`queueStart` are referenced by `MCA_Cmd__Fiiii`; the queue is not owned here. The progress words and `strcpy` are left as imports as instructed. Several of the names above may be defined by other packages' units in the integrated plan; this list was taken from the undefined symbols of the 25 objects intersected with the combined unresolved list, so it overstates what is open once those land.

## Build-plan entries needed

Append these 25 entries to `inputs` in `experimental/build-plan.json` (and the matching `files` hashes and snapshot records), with `input_order` continuing from the integrator's current last order (`N` below). `header_closure` is empty for all. The hashes are of the sources at this branch's tip. `default_authored_inputs` rises by 25. The snapshot needs an `emitted_functions` record per unit.

```json
[
 {
  "header_closure": [],
  "input_order": "N+0",
  "kind": "independently_authored_source",
  "object_name": "busy.o",
  "profile": "authored_cpp",
  "sha256": "346e48c8ce513a4b65339aefccfb545439da4eef5e7f32616056067cbb3d52d0",
  "source": "sources/memcard-calls-busy/busy.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+1",
  "kind": "independently_authored_source",
  "object_name": "checkpoint_age.o",
  "profile": "authored_cpp",
  "sha256": "b73708c86d8db62d26e4ba6785b7ef039490f4c16145903271e817ff30c62cd2",
  "source": "sources/memcard-calls-checkpoint-age/checkpoint_age.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+2",
  "kind": "independently_authored_source",
  "object_name": "client_id.o",
  "profile": "authored_cpp",
  "sha256": "414fdd33d2335ebfe7fccd81e81026b5ae34faa6a909029530c3da12d4a3ee2a",
  "source": "sources/memcard-calls-client-id/client_id.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+3",
  "kind": "independently_authored_source",
  "object_name": "debug.o",
  "profile": "authored_cpp",
  "sha256": "0b1d795445317be091a348599a736f9879637ec8aef8d4501ffa9e174a7d30d0",
  "source": "sources/memcard-calls-debug/debug.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+4",
  "kind": "independently_authored_source",
  "object_name": "game_config.o",
  "profile": "authored_cpp",
  "sha256": "3fb2c99891488c3437e4911eb613280aae3d8845536978e40f968093a2d7b282",
  "source": "sources/memcard-calls-game-config/game_config.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+5",
  "kind": "independently_authored_source",
  "object_name": "get_async_mode.o",
  "profile": "authored_cpp",
  "sha256": "cc81ed0546562a715882a0ed5ab91d0207b34cdb83f8766984fbeceb1f6a7547",
  "source": "sources/memcard-calls-get-async-mode/get_async_mode.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+6",
  "kind": "independently_authored_source",
  "object_name": "heartbeat_run.o",
  "profile": "authored_cpp",
  "sha256": "829f0be95204a161442a275f97ff461ad03e6d0f0e51ba68b51c2c1bb5af8ba4",
  "source": "sources/memcard-calls-heartbeat-run/heartbeat_run.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+7",
  "kind": "independently_authored_source",
  "object_name": "heartbeat.o",
  "profile": "authored_cpp",
  "sha256": "1c462ccfa55cca39ca645912c022bc0820f7e3193bb9c9648063ea556dd18e28",
  "source": "sources/memcard-calls-heartbeat/heartbeat.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+8",
  "kind": "independently_authored_source",
  "object_name": "kick_protect.o",
  "profile": "authored_cpp",
  "sha256": "7dc3243177a41fe7771ee7381773256455889eebe65b015c36a242b173f95d59",
  "source": "sources/memcard-calls-kick-protect/kick_protect.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+9",
  "kind": "independently_authored_source",
  "object_name": "last_dir_count.o",
  "profile": "authored_cpp",
  "sha256": "ef00b984c14454585949d7781e043ccbf26dbb2bef541c397306d9c808047b1e",
  "source": "sources/memcard-calls-last-dir-count/last_dir_count.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+10",
  "kind": "independently_authored_source",
  "object_name": "last_mode.o",
  "profile": "authored_cpp",
  "sha256": "7b9b1cb249fef329d5fe3d5d869cac7fcc6dbc584496b9e41d3573eefdf4601f",
  "source": "sources/memcard-calls-last-mode/last_mode.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+11",
  "kind": "independently_authored_source",
  "object_name": "load_browser.o",
  "profile": "authored_cpp",
  "sha256": "c7055557342e66347f1f8e25446f313dbdf4dd7aa937ad73149259a592b6a1f6",
  "source": "sources/memcard-calls-load-browser/load_browser.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+12",
  "kind": "independently_authored_source",
  "object_name": "clear_info.o",
  "profile": "authored_cpp",
  "sha256": "eb2d67e685029111fe2699638eaa043763ff30fafdab35ec77815e227cac3615",
  "source": "sources/memcard-calls-mca-clear-info/clear_info.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+13",
  "kind": "independently_authored_source",
  "object_name": "mca_cmd.o",
  "profile": "authored_cpp",
  "sha256": "cee25c006eba5e76e124ee35c1d88c483a2b45f1c85a89f2169580a7ad0187e3",
  "source": "sources/memcard-calls-mca-cmd/mca_cmd.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+14",
  "kind": "independently_authored_source",
  "object_name": "mca_init.o",
  "profile": "authored_cpp",
  "sha256": "850c67940f874913ab260757ae8df9d3f255218bfeb650ed5b21ef395696814b",
  "source": "sources/memcard-calls-mca-init/mca_init.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+15",
  "kind": "independently_authored_source",
  "object_name": "mca_sync.o",
  "profile": "authored_cpp",
  "sha256": "f23247d0284dbe5492ecb2c4a13593f7e7140fdef14d41eb295ef3de2b001feb",
  "source": "sources/memcard-calls-mca-sync/mca_sync.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+16",
  "kind": "independently_authored_source",
  "object_name": "new_game.o",
  "profile": "authored_cpp",
  "sha256": "950b7386dde99809c071ff3f68764377d2efe1fc4c0cde4905e85fa6dc5842f8",
  "source": "sources/memcard-calls-new-game/new_game.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+17",
  "kind": "independently_authored_source",
  "object_name": "port.o",
  "profile": "authored_cpp",
  "sha256": "fe5da6455ff961bd7519bed93e810faaad741522702ac8ef09f0b66a74ca3279",
  "source": "sources/memcard-calls-port/port.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+18",
  "kind": "independently_authored_source",
  "object_name": "ready_menu.o",
  "profile": "authored_cpp",
  "sha256": "f868751fac82b3ec60d350726e551ad05c916727e582e7c348d57dc66c04e9c0",
  "source": "sources/memcard-calls-ready-menu/ready_menu.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+19",
  "kind": "independently_authored_source",
  "object_name": "save_browser.o",
  "profile": "authored_cpp",
  "sha256": "1adc1bcc95eab1fe6d9366d15a5b384ecae6d66e1af3445745c042a42171b825",
  "source": "sources/memcard-calls-save-browser/save_browser.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+20",
  "kind": "independently_authored_source",
  "object_name": "save_checkpoint.o",
  "profile": "authored_cpp",
  "sha256": "1a54e9fd1a67090b9659a64431eae8d8664aef4441681d683c76798ed2d55ec9",
  "source": "sources/memcard-calls-save-checkpoint/save_checkpoint.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+21",
  "kind": "independently_authored_source",
  "object_name": "save_config_silent.o",
  "profile": "authored_cpp",
  "sha256": "b07bfc61d69bb179021cec7241f5363df1c481b4bbb822eee5ab33ac102d6442",
  "source": "sources/memcard-calls-save-config-silent/save_config_silent.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+22",
  "kind": "independently_authored_source",
  "object_name": "save_config.o",
  "profile": "authored_cpp",
  "sha256": "5ec8329b5f1aa230d039e25b7b6dcb10abba13dafb8e76c8f61780ab7656852e",
  "source": "sources/memcard-calls-save-config/save_config.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+23",
  "kind": "independently_authored_source",
  "object_name": "saving.o",
  "profile": "authored_cpp",
  "sha256": "4cecd3cd86e00003a88fbe281bbd8c768de318cb99b3babd1bbe407c05990e45",
  "source": "sources/memcard-calls-saving/saving.cpp"
 },
 {
  "header_closure": [],
  "input_order": "N+24",
  "kind": "independently_authored_source",
  "object_name": "slot.o",
  "profile": "authored_cpp",
  "sha256": "bbef7090e708fbfe3071ea7c31620fb7478d6c70929fa130e21877e5fdcdfc14",
  "source": "sources/memcard-calls-slot/slot.cpp"
 }
]
```

## Authored but never executed

Every function is checked only by its host contract and by compiling and linking the object. None was run on the EE, and none was compared byte for byte against the private image in this pass. The data objects are zero-initialised storage whose sizes were taken from the layout evidence in each file's comment.

## Unknown

Whether a reader consumes a command-queue slot during the full-queue countdown in `MCA_Cmd__Fiiii`. Whether the asynchronous mode handlers and `MCA_Process__FPi` write the state bytes in the order the browsers assume. See the Unknown lines in each source header for the rest.
