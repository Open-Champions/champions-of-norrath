# w9-frontend-a: front-end shell, player select, memory card and loader (a)

Sources are under `experimental/sources/w9-frontend-a-*`. Each directory holds one function (or one group of data
objects) in its own commit. Every function was compiled with the reviewed EE toolchain at the `authored_cpp` flags.
Each was compared with the original function from the private executable inside a private R5900 interpreter
(random machine states, every callee replaced by a call-logging hook; the call logs, memory writes and, where the
function returns a value, the return value had to be equal). The interpreter and test scripts are outside git. The
comparison does not show byte equality of the compiled output. The authored objects differ in size and layout from
the originals; only observable behaviour (call order, arguments, memory writes, results) was matched.

## Imports closed

Functions: Ascii2Sjis__FUc, Free__7VISetup, Free__9VIStation, Init__7VISetupi,
Init__9VIStationiPCcPcT3T2T2i, MCA_Busy__Fv, MC_ByteCopy__FPvT0i, MC_CheckPlayerImportOrChange__Fv,
MC_GetNetGameInfo__FPiT0Pc, MC_GetRamPlayerPtr__Fi, MC_GetTempPlayerStorage__Fi, MC_NameToAct__FPc,
MC_SaveLevelEntities__FPc, MC_SetActByLevelName__FPc, Process__7VISetup, Process__9VIStation,
RestartConnect__7VISetup, RunPatcher__7VISetup, _$_7VISetup, _$_9VIStation,
cdCheckForFileInCurrentLayer__FPc, cdIsBusy__Fv, cdLoadAsyncIOP__FPciT0ii, cdLoadAsync__FPcPvT0ii,
checkSave__FP10GameObject.

Data (sizes from the gap to the next symbol where the image gives none): MCA_Result, MCA_DirContents, cardType,
askedForConfigLoad, configLumpLoaded, browserSlot, configFormatted, configFileName, actAlignData, actSendData,
buttonValues, cacheCdDone.

## Imports opened

Every external the new units reference and no unit of this package defines (those are the calls the originals make);
the integrator should diff this against the current unresolved list. Notably specific to this package:
`sAlarmCallback__FiUsPv` (at 0x01150b90, 36 bytes, passed to SetAlarm by RunPatcher), `sLastTickTime` (64-bit word at
0x011ee180, cleared by RunPatcher), `MainLoopBegin__7VISetup`, `MainLoopEnd__7VISetup`,
`StartAmbientSoundtrack__7VISetup`, `VIDnas*` (Init, AuthInstall, GetStatus, Shutdown), the `VIWnd*`/`VIFader`/`VIRaster`
members and `snprintf16__8VIString`, `strlen16__8VIString`, `GetThreadId`, `SetAlarm`, `ReleaseAlarm`, `SleepThread`,
`sceGsSyncV`, `memcmp`, `memcpy`, `strcpy`, `printf`, plus the player-state externals (`player`, `tempPlayerStruct`,
`playerSelectInfo`, `playerStatus`, `playerChangeOldPos`, `playerChangeOldYaw`, `playerStartPos`, `playerStartYaw`,
`killPlayer`, `asyncMode`, `syncState`, `MC_FrontEndImport`, `netNetworkMode`, `nmPlayers`, `gameCam`, `gamePause`,
`saveCheckPointCount`, `particlesPaused`).

## Build-plan entries needed

Several directories reuse source file names (`init.cpp`, `process.cpp`, `free.cpp`, `destroy.cpp` for both VISetup and
VIStation), so the `object_name` values below are made unique. `input_order` is left for the integrator. Data-only
units have no `emitted_functions`.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "ascii2sjis.o", "profile": "authored_cpp", "sha256": "d07341de42bfab0c6833a4cdb3414070be2a604bdd83bbeab9da1a03e759f7e5", "source": "sources/w9-frontend-a-ascii2sjis/ascii2sjis.cpp", "emitted_functions": [{"name": "Ascii2Sjis__FUc", "compiled_body_bytes": 292}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "cd_check_file.o", "profile": "authored_cpp", "sha256": "0917294c1950238bc4e60d01dae76f7a134540cf4ef57920a1fbe501d568e217", "source": "sources/w9-frontend-a-cd-check-file/cd_check_file.cpp", "emitted_functions": [{"name": "cdCheckForFileInCurrentLayer__FPc", "compiled_body_bytes": 224}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "cd_is_busy.o", "profile": "authored_cpp", "sha256": "696fd4c398e670e769e15ea714c66362595957385ace2fb669a28ceba2d4fe5e", "source": "sources/w9-frontend-a-cd-is-busy/cd_is_busy.cpp", "emitted_functions": [{"name": "cdIsBusy__Fv", "compiled_body_bytes": 36}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "cd_load_async.o", "profile": "authored_cpp", "sha256": "c167234d7bc7c8037f4ca229b7fd99e5905a7b39561d8da5b3052f7fdd743949", "source": "sources/w9-frontend-a-cd-load-async/cd_load_async.cpp", "emitted_functions": [{"name": "cdLoadAsync__FPcPvT0ii", "compiled_body_bytes": 252}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "cd_load_async_iop.o", "profile": "authored_cpp", "sha256": "aaece1ce7daec7398306308b2353d32e82d689f37a710b85936c36bfc1bc69a0", "source": "sources/w9-frontend-a-cd-load-async-iop/cd_load_async_iop.cpp", "emitted_functions": [{"name": "cdLoadAsyncIOP__FPciT0ii", "compiled_body_bytes": 240}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "check_save.o", "profile": "authored_cpp", "sha256": "7ab0b50a67fd1d0ce947c4fccd08e3f34e9c838224324876e5061018744ac002", "source": "sources/w9-frontend-a-check-save/check_save.cpp", "emitted_functions": [{"name": "checkSave__FP10GameObject", "compiled_body_bytes": 328}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "data_act_align.o", "profile": "authored_cpp", "sha256": "f78559d8714cfc2b1aec3fe751f7f9e7a374c3007228a5792ff422b779fbe552", "source": "sources/w9-frontend-a-data-act-align/act_align.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "data_button_values.o", "profile": "authored_cpp", "sha256": "8219f7f8152285bc6ab48ee6e4e836ad34713567c8c527d95485e4d334109406", "source": "sources/w9-frontend-a-data-button-values/button_values.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "data_cache_cd_done.o", "profile": "authored_cpp", "sha256": "f77bacce12e5bc6f9fd81c1ecc1b2f3907bb5d9b7fe455299876f832dbdf98b3", "source": "sources/w9-frontend-a-data-cache-cd-done/cache_cd_done.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "data_memcard_state.o", "profile": "authored_cpp", "sha256": "2d3fd9045b4094226ea0cf588dc5cdcea18dbce1c5f65cfe4505ed6e8504b9e9", "source": "sources/w9-frontend-a-data-memcard-state/memcard_state.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_byte_copy.o", "profile": "authored_cpp", "sha256": "2a3b76a753bb626df05b44d58a23c6455d9ffd49e4601706d4134a76437e8c88", "source": "sources/w9-frontend-a-mc-byte-copy/mc_byte_copy.cpp", "emitted_functions": [{"name": "MC_ByteCopy__FPvT0i", "compiled_body_bytes": 48}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_check_player_import_or_change.o", "profile": "authored_cpp", "sha256": "232ae541045e730b711e7bcaa8e8459ac700829ca57e3c9de780394b28ce2b33", "source": "sources/w9-frontend-a-mc-check-player-import-or-change/check_player_import_or_change.cpp", "emitted_functions": [{"name": "_ZN12_GLOBAL__N_1L14remove_flaggedEi", "compiled_body_bytes": 192}, {"name": "MC_CheckPlayerImportOrChange__Fv", "compiled_body_bytes": 2248}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_get_net_game_info.o", "profile": "authored_cpp", "sha256": "806804bdff0b2839cef764d905f9a7ab0da2d36f0c553ec7c149218ae62103ea", "source": "sources/w9-frontend-a-mc-get-net-game-info/mc_get_net_game_info.cpp", "emitted_functions": [{"name": "MC_GetNetGameInfo__FPiT0Pc", "compiled_body_bytes": 176}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_get_ram_player_ptr.o", "profile": "authored_cpp", "sha256": "567922b12f7544150550985abee8894dcce778fa330cef5cd7825187c409bb8a", "source": "sources/w9-frontend-a-mc-get-ram-player-ptr/mc_get_ram_player_ptr.cpp", "emitted_functions": [{"name": "MC_GetRamPlayerPtr__Fi", "compiled_body_bytes": 112}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_get_temp_player_storage.o", "profile": "authored_cpp", "sha256": "2371ef46bab022506349ee625086967b1e9724835223bea672b793e2d5d39443", "source": "sources/w9-frontend-a-mc-get-temp-player-storage/mc_get_temp_player_storage.cpp", "emitted_functions": [{"name": "MC_GetTempPlayerStorage__Fi", "compiled_body_bytes": 24}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_name_to_act.o", "profile": "authored_cpp", "sha256": "cbd3522c9b93eee4557222e68a90b6192ae58bdceb73615998f7334511fb8f24", "source": "sources/w9-frontend-a-mc-name-to-act/mc_name_to_act.cpp", "emitted_functions": [{"name": "MC_NameToAct__FPc", "compiled_body_bytes": 272}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_save_level_entities.o", "profile": "authored_cpp", "sha256": "9acf254f90d49496959ceb3c7fe28e20b5fdadeb7d32e256c79ed7e948740ed0", "source": "sources/w9-frontend-a-mc-save-level-entities/mc_save_level_entities.cpp", "emitted_functions": [{"name": "MC_SaveLevelEntities__FPc", "compiled_body_bytes": 416}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mc_set_act_by_level_name.o", "profile": "authored_cpp", "sha256": "37f810bf79d484fd5daefcd2f4d72fa980129682603b7ddc029709573776159e", "source": "sources/w9-frontend-a-mc-set-act-by-level-name/mc_set_act_by_level_name.cpp", "emitted_functions": [{"name": "MC_SetActByLevelName__FPc", "compiled_body_bytes": 352}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "mca_busy.o", "profile": "authored_cpp", "sha256": "99c60a31a20642c5d3a84bce6183a7abe88795cfa002dc4d34756fc3c9c53c58", "source": "sources/w9-frontend-a-mca-busy/mca_busy.cpp", "emitted_functions": [{"name": "MCA_Busy__Fv", "compiled_body_bytes": 104}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_setup_destroy.o", "profile": "authored_cpp", "sha256": "102e2f146cc98e40061c6bcd7040a11da59d41999ca4e101be095f365e018bef", "source": "sources/w9-frontend-a-vi-setup-destroy/destroy.cpp", "emitted_functions": [{"name": "_$_7VISetup", "compiled_body_bytes": 348}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_setup_free.o", "profile": "authored_cpp", "sha256": "64f98d6d350fc41104ad3e051545635808236caddfbaf30b5bb35d1b85b7e2c4", "source": "sources/w9-frontend-a-vi-setup-free/free.cpp", "emitted_functions": [{"name": "Free__7VISetup", "compiled_body_bytes": 188}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_setup_init.o", "profile": "authored_cpp", "sha256": "2adfb33faed7ca5f97991ce8ec951f7b9456f7b3163c977169d08ac00993c16a", "source": "sources/w9-frontend-a-vi-setup-init/init.cpp", "emitted_functions": [{"name": "Init__7VISetupi", "compiled_body_bytes": 900}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_setup_process.o", "profile": "authored_cpp", "sha256": "0a094449c9b8610a1191582c0ae29f8bced6c9490fd2d15dc511a8c35b80c90e", "source": "sources/w9-frontend-a-vi-setup-process/process.cpp", "emitted_functions": [{"name": "Process__7VISetup", "compiled_body_bytes": 880}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_setup_restart_connect.o", "profile": "authored_cpp", "sha256": "849e263e56f95d47a43a17e55ebeb0fdcf3161384e6c001ad332679f8be20c80", "source": "sources/w9-frontend-a-vi-setup-restart-connect/restart_connect.cpp", "emitted_functions": [{"name": "RestartConnect__7VISetup", "compiled_body_bytes": 112}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_setup_run_patcher.o", "profile": "authored_cpp", "sha256": "370df7545a39f70814c338ceef760c64f84934b0bddb2320148fb7d3a53c04ab", "source": "sources/w9-frontend-a-vi-setup-run-patcher/run_patcher.cpp", "emitted_functions": [{"name": "RunPatcher__7VISetup", "compiled_body_bytes": 1220}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_station_destroy.o", "profile": "authored_cpp", "sha256": "2f3a211b9ef07e9cee495dfbc03159762fb845511171b1b2828bda7022ad0767", "source": "sources/w9-frontend-a-vi-station-destroy/destroy.cpp", "emitted_functions": [{"name": "_$_9VIStation", "compiled_body_bytes": 304}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_station_free.o", "profile": "authored_cpp", "sha256": "e0a1cd6ff23330139de2ac259fd366047f9158439d7c1c197d295a48916d227c", "source": "sources/w9-frontend-a-vi-station-free/free.cpp", "emitted_functions": [{"name": "Free__9VIStation", "compiled_body_bytes": 188}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_station_init.o", "profile": "authored_cpp", "sha256": "95e6093e18cff6325765353f338d67bed880485c38fff8579776b975e4103501", "source": "sources/w9-frontend-a-vi-station-init/init.cpp", "emitted_functions": [{"name": "Init__9VIStationiPCcPcT3T2T2i", "compiled_body_bytes": 1192}]}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "vi_station_process.o", "profile": "authored_cpp", "sha256": "f329ee8d3f929ebec235949ea2eb782e937ee718db0d0d6b49f566caa419b7a6", "source": "sources/w9-frontend-a-vi-station-process/process.cpp", "emitted_functions": [{"name": "Process__9VIStation", "compiled_body_bytes": 884}]}
```

## Observed, inferred, unknown

Observed and inferred are stated at the top of each source file. In short:

- Observed: all call orders, arguments, constants, offsets, strings and loop bounds in the sources, taken from the
  original instruction stream. Delay-slot effects were followed where they change values; for example in
  `RunPatcher__7VISetup` the value tested after `MainLoopEnd` is `-1` if that call returned negative, otherwise the
  value `MainLoopBegin` had returned (the original copies the register in a delay slot before the call).
- Inferred: the meaning of the 84-byte pad snapshot copy, the 16-frame wait counter in `Process__9VIStation`,
  the status block of `RunPatcher__7VISetup` being the DNAS status (code, sub-code, progress), and that the flagged
  inventory items in `MC_CheckPlayerImportOrChange__Fv` are shared between players.
- Unknown: member types of the VISetup/VIStation objects; the meaning of the words at 0xb468, 0x17a30 and 0xbd98 of
  VIStation and 0x153c0 to 0x153e0 of VISetup; the units of VITimer::GetTime; the meaning of the item flag bit 39,
  of the 4100-byte block at player byte 17768 and of playerSelectInfo bytes 32 to 37; the values of `cardType`.

## Authored but never executed

None of these ran on the console or in the whole game. Only the private interpreter comparison (and, for the
older commits, the committed host contracts) exercised them. Specific limits:

- `Process__9VIStation`, `Init__9VIStationiPCcPcT3T2T2i`, `RunPatcher__7VISetup` and
  `MC_CheckPlayerImportOrChange__Fv` have no committed host contract; their only test is the private comparison.
  Callees were hooks, so nothing below them was run.
- Not covered by the comparisons: the 8-byte-aligned variants of inline struct copies (the original has an aligned and
  an unaligned copy loop; the comparison states took one), and in `MC_CheckPlayerImportOrChange__Fv` a few paths
  (item removal in the front-end-import path in some runs, copy-loop variants).
- `MC_CheckPlayerImportOrChange__Fv` reads `playerChangeOldPos[-1]` and `playerChangeOldYaw[-1]` (as the original does)
  when a pad is active but no active pad index is below the old player count; the authored code does it with pointer
  arithmetic, which is undefined behaviour in the language.
- The authored `MC_CheckPlayerImportOrChange__Fv` calls `memcpy`/`strcpy` where the original inlines the copies and
  omits the original's inline default initialisation of its 100 collected-item slots (always overwritten before use).
- `Process__9VIStation` keeps its 16-frame counter in an anonymous-namespace static; the original's is a local static
  at 0x0118e608. Placement of data is not claimed.
- Compiled sizes differ from the originals (for example RunPatcher 1220 bytes against 1408).

## Blockers

None for the listed symbols. `sAlarmCallback__FiUsPv`, `MainLoopBegin__7VISetup` and `MainLoopEnd__7VISetup` belong to
other packages or remain unowned.
