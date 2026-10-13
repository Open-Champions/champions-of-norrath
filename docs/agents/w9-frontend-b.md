# Front-end shell, player select, memory card and loader (b)

Package `w9-frontend-b`. Names and addresses were read from the pinned
SLUS-20565 image with EE binutils. Nothing here has run on the console or in the
game; every function was compared with the original only in a private
R5900 interpreter (outside Git) that runs the original body and the authored
body from identical memory snapshots, with identical Python stand-ins for every
callee, and compares the call log (arguments, float arguments, stack arguments)
and every changed byte outside the stack. Results below are from that harness
and from the host contracts in each source directory. The compiled bodies are
not byte matches of the original and no such claim is made.

## Functions authored (one commit each)

`coreStartLoading__Fv`, `jumpGateHubReset__Fi`, `jumpGateSave__FPc`,
`netGetLocalObject__FP8Creature`, `netGetLocalPlayerNmSafe__Fi`,
`hudNetWaitMsgReady__Fv`, `multiHudAddHelpMessage__FiPUsT1i`,
`initPreSelect__Fv`, `memcardTitle__FPUs`, `gameDeath__Fv`,
`gameCheckForDeathScreen__Fv`, `keyboardGetNextKey__Fv`,
`keyboardGetNextTranslatedKey__Fv`, `initNamePad__Fv`,
`loadPlayerInfo__FiPiP12playerStruct`, `drawProgress__Fv`, `initStuff__6PhPropi`,
`modifyRamSlotForNetSave__Fv`, `jumpGateTaskDraw__Fv`, `jumpGateTaskRun__Fv`,
`loadRamSlot__Fv`, `gameLoadWorld__FPc`.

Data: `configStrings`, the front-end and death state words (`curSel`,
`configMode`, `inGameConfigShuttingDown`, `emergencyMusicWaitAbort`,
`dialogPacket`, `needEnforce`, `gotPlayerStartThisLevel`,
`hudDisplayNetCamToggle`, `netForceLocalUpdate`, `deathState`, `deathNetHack`,
`needActAlign`) and `gCdFile`.

## Verification summary for the last five functions

- `modifyRamSlotForNetSave__Fv`: 45 scenarios, no differences.
- `jumpGateTaskDraw__Fv`: 400 scenarios (alpha, destination counts, selection
  indices, task-active and jumping flags, random entry names), no differences.
  The ":" suffix-stripping loop has a quirk worth knowing: when stripping makes
  the name shorter than two characters the loop ends with the text found
  before the strip, not a new lookup.
- `jumpGateTaskRun__Fv`: 520 scenarios over every state value 0 to 9, 127, 128
  and 255, random pad words, selection and level indices; float updates agree
  bit for bit (the two step constants are 0x3dcccccc and 0x3d4ccccc, written as
  hexadecimal float literals because they are not the nearest floats to 0.1 and
  0.05).
- `loadRamSlot__Fv`: 180 scenarios (camera and jump-gate byte counts that
  change the alignment loop, player counts -1 to 9, empty and non-empty names).
- `gameLoadWorld__FPc`: 300 scenarios over the real 63-record table with
  random edits of its fields; 711 of 716 instruction slots of the original were
  executed (the rest are padding no-ops).

A correction made during this run: the pad table is at 0x0034beb8 and
levelName at 0x0034b660. An early version of the `jumpGateTaskRun__Fv`
scenario file used wrong addresses, so the pad and level-name branches were not
exercised; it was fixed and rerun with no differences.

## Imports closed

configMode, configStrings, coreStartLoading__Fv, curSel, deathNetHack,
deathState, dialogPacket, drawProgress__Fv, emergencyMusicWaitAbort, gCdFile,
gameCheckForDeathScreen__Fv, gameDeath__Fv, gameLoadWorld__FPc,
gotPlayerStartThisLevel, hudDisplayNetCamToggle, hudNetWaitMsgReady__Fv,
inGameConfigShuttingDown, initNamePad__Fv, initPreSelect__Fv,
initStuff__6PhPropi, jumpGateHubReset__Fi, jumpGateSave__FPc,
jumpGateTaskDraw__Fv, jumpGateTaskRun__Fv, keyboardGetNextKey__Fv,
keyboardGetNextTranslatedKey__Fv, loadPlayerInfo__FiPiP12playerStruct,
loadRamSlot__Fv, memcardTitle__FPUs, modifyRamSlotForNetSave__Fv,
multiHudAddHelpMessage__FiPUsT1i, needActAlign, needEnforce,
netForceLocalUpdate, netGetLocalObject__FP8Creature,
netGetLocalPlayerNmSafe__Fi.

Not closed: `creditsText` (see Unknown).

`emergencyMusicWaitAbort` is also defined by two unselected units,
`w8-audio-calls-sound-state` and `simloop-services-sound-music-state`. Select
only one definition when merging plans.

## Imports opened

Symbols referenced by these sources and not defined by them (most are other
packages' functions and data; the integrator should check each is closed by
the merged plan):

`GetSaveFootPrint__6Camera, HUDInit__Fv, Load__6CameraPc, MCA_Cmd__Fiiii, MC_GetAsyncMode__Fv, MC_GetLoadName__Fv, MC_LoadLevelEntities__FPcT0, MC_LoadRamSlot__Fv, MC_ResetAutoCheckPointSave__FPc, MC_SavingOrLoading, Read__10VIKeyboardR12VIConsoleKey, __7Weatheri, _engineCheckStack__FPCci, _listCheckAllIntegrity__FPCci, asyncMode, blockAlloc__Fi, bodyInit__FP5_bodyfffff, bodyUpdateMatrix__FP5_body, boxAdd__FP3BoxT0, boxSetEmpty__FP3Box, browserOverwrite, convertIndiciesToPointers__Fi, coreInterfaceFlare__Fi, coreLoading, coreLoadingStart, coreStartLoadingScreen__Fv, curDest, curPlaySeconds, curPlayer, currActivityScore, currScorePos, cursorTime, cutMode, debugClearStartMenu__Fv, debugStartMenuIsOn__Fv, delta, doJumpEffect__Fi, domeLump, drawFrameNm, eRandState, engineDisplayDisable, enginePopBorderColor__Fv, enginePushBorderColor__FUi, engineRemoveTask__FPFv_v, engineTaskActive__FPc, evalHermite__Ffffff, flareAlpha, fontColor__FUiii, fontEnd__Fv, fontGrid, fontPrintCenteredu__FiiPUsi, fontStart__FP4FontiUii, framedone, gameAutoMapMode, gameCam, gameClear__Fi, gameControlDontResetPause, gameControlPaused, gameDeathHeight, gameDeathStart__Fi, gameDeathTimer, gameDeathTimerSingle, gameDifficulty, gameGetFullLevelName__FPc, gameInitPostFrontend__Fv, gameIsPaused__Fv, gameLevelLoading, gameLineFuse__Fv, gameMaxCamHeight, gameMinCamHeight, gameNetKick__Fi, gameNetReplicateQuestItems, gameOver, gamePause, gamePlayersPresent, gameSetMusicTimer__Fi, getSlotName__FiiPc, groundType, hubNames, hudFadeScreen__Fi, hudFooterUni, hudNoDraw, hudShowMsg__FPUsN20iii, importInfoHeader, inesc, initJumpEffect__Fv, initPosition__Fii, init__3mapi, init__4itemv, inventorySyncQuestItems__Fi, jumpGateDestCount, jumpGateEntries, jumpGateHurtDest, jumpGateLoad__FPc, jumpGateSaveFootPrint__Fv, jumpGateState, jumpGateTime, jumpGateWho, jumping, keepPlayerPositions, lastDest, lastMusicState, lastkey, lasttime, levelFlags, levelName, levelSeeds, lightSetSceneLights__FG6Point3iiiiiiiii, lightStart, list.1073, loadName, loadPlayerLumps__Fv, loc, localPredictedPlayer, lockAlpha, lumpClear__Fi, lumpFindByType__FPvPci, lumpFindResource__FPcT0, lumpFind__FPvPc, lumpLoadGlob__FPc, lumpLoad__FPc, lumpLock__Fi, lumpQuery__FPc, mapScale, mapScissorX, mapScissorY, mapX, mapY, matToQuaternion__FP4QuatP8Matrix34, megatileRot, megatileSeed, megatile_ramkmax, memCheckHeapReal__FPCci, memcpy, modelGetBoundingBox__FP12_modelHeaderlPiN52, modelSetGlobalShadowBrightness__Ff, multiBody, multiHelpMessageOn, multiRelease, multiSide, multiTitle, nameGridCount, nameGridW, netBlankController__Fii, netGetLocalPlayerNm__Fi, netNetworkMode, netPlayerIsConnected__Fi, netPlayerIsLocal__Fi, netSendMessage__FiPUci, netWaitMsgHandler, nmPartners, nmPlayers, nmPlayersAlive, numEnvironments, originalPos, originalYaw, pad, padAcceptButton, padAcceptButtonFontChar, padBackButton, padBackButtonFontChar, padFilter, particlesPaused, partners, player, playerDeallocateArmorCaches__Fv, playerRecalled, playerSelectInfo, playerStartPos, playerStartYaw, progress, quatNormalize__FP4Quat, ramSlot, recallEnter, rectangleDraw__FiiiiiUi, routeClear__Fv, routeMakeLattice__Fv, saveCheckPointCount, scale, scePrintf, scissor, scriptActivatePostAction__Fv, scriptClearAllGameState__Fv, scriptSetGameState__FPci, selectMode, skirmishInit__Fv, smallFont, soundFadeStream__Fi, soundFrame__Fv, soundGetDialogDir__Fv, soundPlaySequence__FP15AnimationHeaderP10GameObjectf, soundSetAmbientData__FPc, soundSetAudioEffect__Fiiiii, soundStartMusic__FPci, sprintf, startScorePos, strcat, strcmp, strcpy, strlen, strncmp, strstr, syncRandState, tempPlayerStruct, textGetString__FPc, textureDrawBorder__FiiiiiUiPcii, thisLevel, timeLastTransition, usprintf__FPUsT0ffffffff, vik, world, worldActivityLevel, worldAllocateSegments__FP12_worldHeader, worldAllocateWaterPatches__Fi, worldBoundingBox, worldDeactivateUntakenDoors__FP12_worldHeader, worldDifficulty, worldSetWaterProperties__FP8_texture, worldTileEntityLightScale`

Two deserve a note. `list.1073` is a 3780-byte table of 63 sixty-byte level
records (62 named records and a terminator whose fields act as defaults) that
`gameLoadWorld__FPc` indexes; it is declared with an assembler label because
the symbol name is not a C identifier, and its contents are not authored here.
`loc`, `scale` and `scissor` are read-only float tables at 0x0046cc90,
0x0046ccc0 and 0x0046ccf0 that `gameLoadWorld__FPc` reads.

## Build-plan entries needed

Append to `inputs` (with `input_order` continuing from the current last value,
`kind` `independently_authored_source`) and to `files`. Hashes are of the
files at the last commit. The sources are self-contained (no header closure)
and compile with the existing `authored_cpp` profile.

```json
[
 {
  "source": "sources/w9-frontend-b-cd-file/cd_file.cpp",
  "sha256": "17af56826f2c2e764782ac27d38d5bd4588cfa4dd78ebe0d19b90c5a9d97f36f",
  "object_name": "cd_file.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": []
 },
 {
  "source": "sources/w9-frontend-b-config-strings/config_strings.cpp",
  "sha256": "98ce2b64f32ca7488a8ade3de1a062ddbe8f15460efb5dc5537dc346ec167a39",
  "object_name": "config_strings.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": []
 },
 {
  "source": "sources/w9-frontend-b-core-start-loading/core_start_loading.cpp",
  "sha256": "5a838a8f3cfd98caaad70008740c0179f23783e9e94c3e1a8ef77275790c267c",
  "object_name": "core_start_loading.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "coreStartLoading__Fv",
    "compiled_body_bytes": 32
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-draw-progress/draw_progress.cpp",
  "sha256": "1aebdb96a0e0b6d43c95f7c8e8fa336f61c4cf4b896b46fd3a06721e793babb4",
  "object_name": "draw_progress.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "drawProgress__Fv",
    "compiled_body_bytes": 404
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-game-check-for-death-screen/game_check_for_death_screen.cpp",
  "sha256": "3767d912821d178e083e446fbbae2da0a98a7747df32c01ab364fa9abccc04e1",
  "object_name": "game_check_for_death_screen.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "gameCheckForDeathScreen__Fv",
    "compiled_body_bytes": 576
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-game-death/game_death.cpp",
  "sha256": "13745954a1117024c44fefbd165232f393a5871e12dc84eb916efb65ea2e7a0d",
  "object_name": "game_death.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "gameDeath__Fv",
    "compiled_body_bytes": 216
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-game-load-world/game_load_world.cpp",
  "sha256": "e33c692a96b51da28f95544f0cd35e06547f6ca4a9b3ed7a5fd80f32651d0d40",
  "object_name": "game_load_world.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "gameLoadWorld__FPc",
    "compiled_body_bytes": 3080
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-hud-net-wait-msg-ready/hud_net_wait_msg_ready.cpp",
  "sha256": "dbe5ab40bd8f1b07ff1ef1d7be951cf3f106a2d5e16516c5e172bca96de8c8ca",
  "object_name": "hud_net_wait_msg_ready.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "hudNetWaitMsgReady__Fv",
    "compiled_body_bytes": 128
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-init-name-pad/init_name_pad.cpp",
  "sha256": "00845f8d96638ad3baf43e67777bd842039cac47aa3ad23e41db32c08c3487ab",
  "object_name": "init_name_pad.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "initNamePad__Fv",
    "compiled_body_bytes": 248
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-init-pre-select/init_pre_select.cpp",
  "sha256": "4d86cc776e4d9b3b68212dc3441f3d32bac4a04c8ce6445946bd4e42461bd406",
  "object_name": "init_pre_select.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "initPreSelect__Fv",
    "compiled_body_bytes": 80
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-jump-gate-hub-reset/jump_gate_hub_reset.cpp",
  "sha256": "545fdd397f780a6c121a3144c4d5d83b9d5432a460c153ba3f480f95514e210e",
  "object_name": "jump_gate_hub_reset.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "jumpGateHubReset__Fi",
    "compiled_body_bytes": 68
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-jump-gate-save/jump_gate_save.cpp",
  "sha256": "f29ebcf7c6a6705f16bd9534be2fa54da2dcb7d7c7b339027e363365bf731c69",
  "object_name": "jump_gate_save.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "jumpGateSave__FPc",
    "compiled_body_bytes": 56
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-jump-gate-task-draw/jump_gate_task_draw.cpp",
  "sha256": "fb1e5c676acf6493409e99957a4bfe9f3cd8187192997a4000bd2426101e5304",
  "object_name": "jump_gate_task_draw.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "jumpGateTaskDraw__Fv",
    "compiled_body_bytes": 1072
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-jump-gate-task-run/jump_gate_task_run.cpp",
  "sha256": "04e1d124638e0b9ab09b6add090767a41fb530120d273c1782932a6ea92ece8b",
  "object_name": "jump_gate_task_run.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "jumpGateTaskRun__Fv",
    "compiled_body_bytes": 1468
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-keyboard-get-next-key/keyboard_get_next_key.cpp",
  "sha256": "b069e0f7a3bb5de8c0b4c6e39d253976380ec9395fedd718eec7100c4958cba3",
  "object_name": "keyboard_get_next_key.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "keyboardGetNextKey__Fv",
    "compiled_body_bytes": 416
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-keyboard-get-next-translated-key/keyboard_get_next_translated_key.cpp",
  "sha256": "920b741bc9dd72a6da580bc49ca0e4922fb494a88faa0da9d86b67bf6712753c",
  "object_name": "keyboard_get_next_translated_key.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "keyboardGetNextTranslatedKey__Fv",
    "compiled_body_bytes": 312
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-load-player-info/load_player_info.cpp",
  "sha256": "5c40ac4153e8207ee5e6b026a1679ec6b8a16d451f0a46ce72de1a0bfd5454c4",
  "object_name": "load_player_info.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "loadPlayerInfo__FiPiP12playerStruct",
    "compiled_body_bytes": 312
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-load-ram-slot/load_ram_slot.cpp",
  "sha256": "212177d472f78a7c90ecd21f73050e4a1bc7cf6472f167cdb11767fbb51f4764",
  "object_name": "load_ram_slot.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "loadRamSlot__Fv",
    "compiled_body_bytes": 976
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-memcard-title/memcard_title.cpp",
  "sha256": "e9d52391538b42b93aba7e2ffc72b1d0325be93f49e82be34209c48efac6d85c",
  "object_name": "memcard_title.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "memcardTitle__FPUs",
    "compiled_body_bytes": 128
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-modify-ram-slot-for-net-save/modify_ram_slot_for_net_save.cpp",
  "sha256": "24d83560766b12cae1b5c220f26b847662aa80e9ce3eb29750bd87ed667ead30",
  "object_name": "modify_ram_slot_for_net_save.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "modifyRamSlotForNetSave__Fv",
    "compiled_body_bytes": 656
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-multi-hud-add-help-message/multi_hud_add_help_message.cpp",
  "sha256": "9490940164f57beb1bcb3e9ad4a4a1965b3d92837b97723560560ee21da9398c",
  "object_name": "multi_hud_add_help_message.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "multiHudAddHelpMessage__FiPUsT1i",
    "compiled_body_bytes": 176
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-net-get-local-object/net_get_local_object.cpp",
  "sha256": "0c57c390933ac6078de4af3169de85475d8a731b865de80459741b0b2d0dd7f1",
  "object_name": "net_get_local_object.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "netGetLocalObject__FP8Creature",
    "compiled_body_bytes": 112
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-net-get-local-player-nm-safe/net_get_local_player_nm_safe.cpp",
  "sha256": "9740f22a493c5d025306e4120ee527924b6272c063ee6cc3f484a10df17afbcc",
  "object_name": "net_get_local_player_nm_safe.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "netGetLocalPlayerNmSafe__Fi",
    "compiled_body_bytes": 80
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-ph-prop-init-stuff/init_stuff.cpp",
  "sha256": "fac229b35b2c7fe1063ee1ac5ef187ab97ec3ff90cbbdc50d776238da36de3ff",
  "object_name": "init_stuff.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": [
   {
    "name": "initStuff__6PhPropi",
    "compiled_body_bytes": 692
   }
  ]
 },
 {
  "source": "sources/w9-frontend-b-state-words/state_words.cpp",
  "sha256": "ddb3a6489b73e397242d71d66fe804375b41c17a452f4d1b177d630f6ea02be0",
  "object_name": "state_words.o",
  "profile": "authored_cpp",
  "header_closure": [],
  "emitted_functions": []
 }
]
```

A private plan with these entries cannot be validated by `--check-plan`
because the shared snapshot lists the covered inputs; the integrator's
snapshot refresh is required. I compiled every entry with the `authored_cpp`
flags directly and recorded the emitted function sizes above.

## Authored but never executed

All of it, on the console or in the game. Everything was run only against
stand-in callees (host contracts and the private paired harness).
`jumpGateTaskDraw__Fv` and `jumpGateTaskRun__Fv` are scheduled tasks that
nothing in the authored startup chain registers yet; `gameLoadWorld__FPc`
depends on the unauthored `list.1073` table.

## Unknown

- `creditsText` (0x00428a20, 2408 bytes of initialised data) is a table of
  credit lines and string pointers. It is the game's own text, so no
  reconstruction is published; `frontend-shell-run` already declares it as an
  import. It stays open until the owner decides how to carry such data.
- The 63-record `list.1073` contents (per-level numbers and name keys).
- Layout of `gCdFile` beyond its first word (40 bytes reserved), the meaning of
  `curSel`, `configMode`, `dialogPacket` and `needEnforce`, the `levelName`
  fields at +80, +81, +161 and +242, the 112-byte save header apart from the
  offsets used by `loadRamSlot__Fv`, and the layout of `ramSlot`
  beyond the offsets used by `modifyRamSlotForNetSave__Fv` and
  `loadRamSlot__Fv`.
