# Core game loop, engine and scene switching (5/5) — `w8-core-loop-e`

Branch `agents/w8-core-loop-e`. New sources are under `experimental/sources/w8-core-loop-e-*`; each directory holds one
function (or one data object) and, beside it, a `host_check.cpp` that is not a plan input. Addresses and extents come from
the pinned local `SLUS_205.65` symbol table and the instructions, read locally; no listing, byte dump or game data is
committed. Every unit compiles with the reviewed `authored_cpp` profile (GCC 15.2 EE). A fresh compile is not an
original-byte match and nothing here is claimed to match the original bytes. Nothing has run on the EE.

Host checks compile with `c++ -std=c++17`, define `NORRATH_HOST_CONTRACT` where a source reads pointer-valued words (an
`norrath_host_pointer` hook maps 32-bit words into an arena; the EE build never defines it), mock every callee, and return
non-zero on a wrong store, call order, bound or result. Mutants (changed constant, comparison, stride, bit, or removed
store) were run against each function; the survivors that were not equivalent were closed with extra cases, and the one
known equivalent mutants are noted below.

## Functions

| source dir | name | image | original bytes | compiled bytes |
|---|---|---|---|---|
| `w8-core-loop-e-script-clear` | `scriptClear__Fv` | `0x001d4000` | 16 | 28 |
| `w8-core-loop-e-script-super-clear` | `scriptSuperClear__Fv` | `0x001d4048` | 24 | 36 |
| `w8-core-loop-e-set-extra-params` | `setExtraParams__FP8CreaturePc` | `0x001d3970` | 28 | 40 |
| `w8-core-loop-e-script-set-script` | `scriptSetScript__FPv` | `0x001d3e48` | 64 | 84 |
| `w8-core-loop-e-script-clear-all-game-state` | `scriptClearAllGameState__Fv` | `0x001d3f10` | 80 | 76 |
| `w8-core-loop-e-script-task` | `scriptTask__Fv` | `0x001d3db0` | 148 | 240 |
| `w8-core-loop-e-script-run` | `scriptRun__Fv` | `0x001d3ba8` | 244 | 368 |
| `w8-core-loop-e-script-activate-post-action` | `scriptActivatePostAction__Fv` | `0x001d3ca0` | 156 | 256 |
| `w8-core-loop-e-sub-draw-loop` | `subDrawLoop__FPFv_i` | `0x002d0de8` | 228 | 424 |
| `w8-core-loop-e-cat-switch-to-run` | `switchToRun__3Cat` | `0x001eec00` | 396 | 348 |
| `w8-core-loop-e-run-animation` | `runAnimation__Fv` | `0x001d2810` | 304 | 412 |
| `w8-core-loop-e-run-music-ducking` | `runMusicDucking__Fv` | `0x0024b710` | 232 | 372 |
| `w8-core-loop-e-run-sequences` | `runSequences__Fv` | `0x0024d878` | 516 | 724 |
| `w8-core-loop-e-show-coords` | `showCoords__Fv` | `0x0017fb90` | 692 | 752 |
| `w8-core-loop-e-setup-main` | `setup_main__Fv` | `0x01166b78` | 456 | 712 |
| `w8-core-loop-e-station-main` | `station_main__FiPCcPcT2T1T1i` | `0x0116cf48` | 344 | 464 |
| `w8-core-loop-e-start-new-world` | `startNewWorld__Fv` | `0x002cd1e0` | 1244 | 1384 |
| `w8-core-loop-e-run-ambient` | `runAmbient__Fv` | `0x0024da80` | 1052 | 1532 |
| `w8-core-loop-e-run-screaming` | `runScreaming__Fv` | `0x0024b090` | 1244 | 1732 |
| `w8-core-loop-e-run-music` | `runMusic__Fv` | `0x002ce1a0` | 3208 | 4180 |

## Data

| source dir | name | image | extent / value |
|---|---|---|---|
| `w8-core-loop-e-data-status-ptr` | `statusPtr` | `0x00486af4` .sbss | one word, zero |
| `w8-core-loop-e-data-sent-end-msg` | `sentEndMsg` | `0x00486b60` .sbss | one word, zero |
| `w8-core-loop-e-data-swooping` | `swooping` | `0x00486b70` .sbss | one word, zero |
| `w8-core-loop-e-data-super-gamestate-init` | `superGamestateInit` | `0x00486dc0` .bss | 8 bytes (name pointer, state word), zero |
| `w8-core-loop-e-data-start-entrance-name` | `startEntranceName` | `0x0042a3f8` .data | 32 bytes, all zero in the file |
| `w8-core-loop-e-data-tripple-buffer-mode` | `trippleBufferMode` | `0x004843e0` .sdata | one word, file value 1 |
| `w8-core-loop-e-data-wait-for-sound` | `waitForSound` | `0x0048501c` .sdata | one byte, file value 0 |
| `w8-core-loop-e-data-world-difficulty` | `worldDifficulty` | `0x00484cac` .sdata | one float, file word `0x3f800000` (1.0) |
| `w8-core-loop-e-data-retval` | `retval` | `0x008e8100` .bss | 64 bytes (distance to `sceCdCbfunc`), zero |
| `w8-core-loop-e-data-stack-15` | `stack.15` | `0x008ddcc0` .bss | 1024 bytes (distance to `topSema`; `InitThread` passes this address and size 1024 as a thread stack), zero; label given with `__asm__("stack.15")` |

## Observed

- Script deferred-call table: `deferedCall` (`0x007d0de8`) is two banks of 64 records of 80 bytes, the bank chosen by the
  low bit of `scriptFrame` (gp-10600). `nmDeferedCalls` (gp-3672) is the count for the current frame;
  `nextNmDeferedCalls` (gp-3668) collects the next frame's count. `scriptTask` runs each record through `scriptStart`;
  `scriptRun` does the same lookup inline with `amx_FindPublic` / `amx_Exec`. A failed lookup moves `complainFrame`
  (the word at `0x0048508c`) to `drawFrameNm + 20` only when it is below `drawFrameNm`. The image value of
  `complainFrame` is -10000. `postAction` (`0x007d55e8`) is a pending-action text; `scriptActivatePostAction` copies it
  to a local buffer, clears its first byte, runs it, and brackets the run with `scriptInPostAction`.
- `scriptSetScript` stores its argument in `curScriptPtr` (null included) and, for a non-null script, calls the AMX
  initializer with the script plus 96 and then the native registration.
- `scriptClearAllGameState` walks packed strings in `gameStateData` (`0x007d35e8`) and zeroes the byte after each
  string's terminator.
- `startNewWorld` splits the requested level name at the first `_` into `levelName+81` (before) and `levelName+161`
  (after), builds `levelName+242`, calls `gameLoadWorld`, places and constructs `Player` objects, and finishes with
  `recallEnter`, `coreLevelCommentTimer` and `coreHeadsetResetTimer`. In the instruction stream the entity-saved
  branch stores a zero word at `engineTaskActive`'s result, which is zero on that path (the address 0); this is kept
  through an opaque pointer, not "fixed". `core.cc` and the line numbers 285, 302, 349 and 351 are the strings and
  arguments of the `_engineCheckStack` calls.
- `runAnimation`, `runMusicDucking`, `runSequences`, `runAmbient`, `runScreaming`, `runMusic` read the 32-bit constants
  listed in each file's header as bit patterns (the values are in the image); none is a computed constant.
- `setup_main` and `station_main` run in the `0x0116....` overlay region; their callees (`VISetup`, `VIStation`,
  `ezNetCnf`, `unet_shutdown`) are other packages' symbols.
- `subDrawLoop` is `simpleDrawLoop` without the exit cleanup (no `gameClear`, stream resets, or `lumpClear`).
- `switchToRun__3Cat` resets the animation state at `Cat+288`, re-adds four animations, restores the saved 32 bytes of
  state at `+424` and writes 1.0, -1/30 (`0xbd088888`) and 1.0 at `+432`, `+436`, `+440`, byte `5` at `+292`.
- `runMusic` zeroes 32 bytes at `0x004296c0` (inside the object `list.1073`) on every call, before reading them;
  the activity ring is therefore zero whenever it is averaged. It then runs a per-frame state machine over
  `lastMusicState` -1, 0, 1, 2 with thresholds 1.742 (`0x3fdef96e`) and 1.072 (`0x3f893719`) and 0.25.

## Inferred (not established)

- The ambient/screaming/music functions drive background audio streams and adaptive music; the names of fields in the
  80-byte dialog directory entries, 452-byte stream records and 680-byte ambient groups are offsets only.
- `runAnimation` looks like a lip-sync drift correction against a stream position; this is a guess from the arithmetic.
- `deferedCall` records are 80-byte names; the 64-record bank size follows from the 5120-byte bank stride.
- `statusPtr`, `sentEndMsg`, `swooping` are declared as plain words; only word accesses were seen.
- The `Player` constructor's last two arguments, and the `Cat` layout beyond the offsets used, are unknown.

## Verification run

- Each source compiled with the reviewed EE g++ and the `authored_cpp` flags; the emitted global functions are exactly the
  one named (helpers are `always_inline`, so no extra local functions are emitted).
- Each `host_check.cpp` compiled with `c++ -std=c++17 -Wall` and exited 0; mutation spot checks failed a numbered case
  per function. Equivalent mutants noted: removing the `amount < desired` clamp in the downward step of `runMusicDucking` is not
  observable (the upward step re-clamps), and swapping the two tags tried per entry in `runMusic`'s first search
  (`_env_` / `mx_theme`) returns the same entry.
- A private link check (not a build): all 30 objects were scanned against the `work/plan8-build` baseline object set.
  30 of the 31 package names resolved and none of the defined symbols duplicates a baseline definition.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Imports closed (names)

`retval`, `runAmbient__Fv`, `runAnimation__Fv`, `runMusicDucking__Fv`, `runMusic__Fv`, `runScreaming__Fv`,
`runSequences__Fv`, `scriptActivatePostAction__Fv`, `scriptClearAllGameState__Fv`, `scriptClear__Fv`, `scriptRun__Fv`,
`scriptSetScript__FPv`, `scriptSuperClear__Fv`, `scriptTask__Fv`, `sentEndMsg`, `setExtraParams__FP8CreaturePc`,
`setup_main__Fv`, `showCoords__Fv`, `stack.15`, `startEntranceName`, `startNewWorld__Fv`,
`station_main__FiPCcPcT2T1T1i`, `statusPtr`, `subDrawLoop__FPFv_i`, `superGamestateInit`, `switchToRun__3Cat`,
`swooping`, `trippleBufferMode`, `waitForSound`, `worldDifficulty` — 30 of the 31 assigned names.

Not closed: `srcfile` (`0x00445200`, 1856 bytes in `.data`). Observed: it is machine code, not data. `InitAlarm`
(0x002dddc8, see `docs/STARTUP_INITSYS.md`) copies these 1856 bytes to `0x80076000`, and the next symbols
are `eenull`, `SysEntry`, `g_Timer` and `tlbsrc`, so it is the alarm kernel-patch body. Publishing it would be a
byte-for-byte transcription of copied code, so it was not authored. It belongs with the `kernel-patches` package,
which authored the other patch bodies as assembly and compared them byte for byte; it needs their decision on whether
and how to reproduce a 464-instruction body independently.

## Imports opened

Measured against the `work/plan8-build` baseline (13:46 on the integration day); several may already be present in the
current shared list or be defined by other wave-8 packages, so treat this as the maximum:

Functions: `Free__7VISetup`, `Free__9VIStation`, `Init__7VISetupi`, `Init__9VIStationiPCcPcT3T2T2i`,
`MC_CheckPlayerImportOrChange__Fv`, `MC_SaveLevelEntities__FPc`, `MC_SetActByLevelName__FPc`, `Process__7VISetup`,
`Process__9VIStation`, `RestartConnect__7VISetup`, `RunPatcher__7VISetup`, `_$_7VISetup`, `_$_9VIStation`,
`__6PlayerG6Point3sii`, `__7VISetup`, `__9VIStation`, `__builtin_new`, `animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi`,
`animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`, `animFindComponentWithFlags__FP14AnimationStatei`,
`animGetVolumeScale__Fii`, `cdCheckForFileInCurrentLayer__FPc`, `cdIsBusy__Fv`, `cdLoadAsyncIOP__FPciT0ii`, `fptodp`,
`gameLoadWorld__FPc`, `getMegatileOriginalPosition__FG6Point3`, `initEzNetCnf`, `objectFindEmptySpot__FG6Point3iiPc`,
`playerSelectionEnforce__Fv`, `runStreamQueing__Fv`, `sceSifMExitRpc`, `soundAllFlags__Fi`, `soundDisable__Ff`,
`soundGetDialogDir__Fv`, `soundGetGlobalDialogDir__Fv`, `soundStartDialogStream__Fiii`, `soundStopSound__Fi`,
`unet_shutdown__Fv`, `worldFindHeight__FP12_worldHeaderG6Point3f`.

Data: `ambientGroup`, `currActivityScore`, `currScorePos`, `deferedCall`, `desiredMusicDuckAmount`, `dialogLumpName`,
`gotPlayerStartThisLevel`, `lastMusicState`, `megatileRot`, `megatileSeed`, `nextNmDeferedCalls`, `nmAmbientGroups`,
`nmDeferedCalls`, `numEnvironments`, `playerStartPos`, `playerStartYaw`, `recallPos`, `scriptFrame`, `startScorePos`,
`timeLastTransition`, `validLocalPlayer`.

One name is not an original symbol: `norrath_music_activity_ring_view` is the view name for the 32 bytes at
`0x004296c0` inside `list.1073`; the integrator must define it (32 bytes, zero) or alias it to that range.
`norrath_script_game_state_data_view`, `norrath_script_post_action_view`, `norrath_script_current_amx_word_view`,
`norrath_script_complain_frame_word_view`, `norrath_sound_sequence_records_view`, `norrath_sound_channel_records_view`,
`norrath_amx_initialize`, `norrath_amx_register_game_natives`, `norrath_amx_find_public`, `norrath_amx_exec` are
existing views/providers of other packages and were used by name.

## Build-plan entries needed

Append in this order (inputs have no cross-dependencies; `input_order` is for the integrator to assign). Every entry uses the
`authored_cpp` profile, an empty `header_closure`, and the sha256 of the committed file. Object names do not collide
with existing plan objects. `host_check.cpp` files are not inputs. The `files` map needs one `sources/<dir>/<file>.cpp`
hash per entry (the `sha256` field).

```json
{"header_closure":[],"kind":"independently_authored_source","object_name":"switch_to_run.o","profile":"authored_cpp","sha256":"f8259179c802008c4ddd5419fc15df3677a296230217b2473aa2012780f20e3d","source":"sources/w8-core-loop-e-cat-switch-to-run/switch_to_run.cpp","emitted_functions":[{"name":"switchToRun__3Cat","compiled_body_bytes":348}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"retval.o","profile":"authored_cpp","sha256":"53247057efa0679f830868055336d2a6e9e8f86216eb658744a6abd3a46e0aa5","source":"sources/w8-core-loop-e-data-retval/retval.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"sent_end_msg.o","profile":"authored_cpp","sha256":"639f1cc55b8fa86c7d60e0e0b5ff4d020e34a6aca6c88c2ce033164945943256","source":"sources/w8-core-loop-e-data-sent-end-msg/sent_end_msg.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"stack_15.o","profile":"authored_cpp","sha256":"3881afcf5cd1d466a0d3ceda898b56bc92946c6443d319bee14588290cdac4a3","source":"sources/w8-core-loop-e-data-stack-15/stack_15.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"start_entrance_name.o","profile":"authored_cpp","sha256":"0ebeed7997281286a6e5a1490d6a5436b148c90fdee9c38e83f049d8d610b55b","source":"sources/w8-core-loop-e-data-start-entrance-name/start_entrance_name.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"status_ptr.o","profile":"authored_cpp","sha256":"7e4f4ed694f748d82ef209365287574dab68e5539389cdf4b680777ef3ad52ac","source":"sources/w8-core-loop-e-data-status-ptr/status_ptr.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"super_gamestate_init.o","profile":"authored_cpp","sha256":"0975709ac43ad35de3374013e61f65308a0bead2d76d23501cf386d6b325470d","source":"sources/w8-core-loop-e-data-super-gamestate-init/super_gamestate_init.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"swooping.o","profile":"authored_cpp","sha256":"2842d14ef488f574d9c550d6fafc30970e9aa7506d42e61d5e156b20aceb8500","source":"sources/w8-core-loop-e-data-swooping/swooping.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"tripple_buffer_mode.o","profile":"authored_cpp","sha256":"350e68bda58b77bf4a53c13d3e346ae1894419a5f472192d96d756fe76983206","source":"sources/w8-core-loop-e-data-tripple-buffer-mode/tripple_buffer_mode.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"wait_for_sound.o","profile":"authored_cpp","sha256":"7137e2265894932a8d22a7ec659e11385ff349076b41c21adcf8330debf0d58c","source":"sources/w8-core-loop-e-data-wait-for-sound/wait_for_sound.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"world_difficulty.o","profile":"authored_cpp","sha256":"fdded875b7f97ad59db91ab2a999e62c9137112c70895ef3e6bd02184c10a938","source":"sources/w8-core-loop-e-data-world-difficulty/world_difficulty.cpp"}
{"header_closure":[],"kind":"independently_authored_source","object_name":"run_ambient.o","profile":"authored_cpp","sha256":"174b1559eaba2ebbb5d3e443babe9d7bc6849a3296043fa1952fe729b154d8b9","source":"sources/w8-core-loop-e-run-ambient/run_ambient.cpp","emitted_functions":[{"name":"runAmbient__Fv","compiled_body_bytes":1532}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"run_animation.o","profile":"authored_cpp","sha256":"a7487b943bc552b8982cfd0aa7d772ed036e2edcfa3be5cee8e3554690d95ba1","source":"sources/w8-core-loop-e-run-animation/run_animation.cpp","emitted_functions":[{"name":"runAnimation__Fv","compiled_body_bytes":412}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"run_music.o","profile":"authored_cpp","sha256":"e9aa95b68c68442e409e20716b72ce482a0565da6705ea3444b7d7fdb62b8512","source":"sources/w8-core-loop-e-run-music/run_music.cpp","emitted_functions":[{"name":"runMusic__Fv","compiled_body_bytes":4180}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"run_music_ducking.o","profile":"authored_cpp","sha256":"429f2d0f03ac007416c5e070c91002732eada706fe85da9bffe8e5a92a80709f","source":"sources/w8-core-loop-e-run-music-ducking/run_music_ducking.cpp","emitted_functions":[{"name":"runMusicDucking__Fv","compiled_body_bytes":372}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"run_screaming.o","profile":"authored_cpp","sha256":"c4dde6b8143d50dd8e2cfb23e5234723401c213191d1761a39bf891a54c476bb","source":"sources/w8-core-loop-e-run-screaming/run_screaming.cpp","emitted_functions":[{"name":"runScreaming__Fv","compiled_body_bytes":1732}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"run_sequences.o","profile":"authored_cpp","sha256":"31e7c9ef7ae38ef9b9eb75d2923d9fb866c9c3d5b60112b275c0ae7cab319d1d","source":"sources/w8-core-loop-e-run-sequences/run_sequences.cpp","emitted_functions":[{"name":"runSequences__Fv","compiled_body_bytes":724}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_activate_post_action.o","profile":"authored_cpp","sha256":"d850fc6e1aba0f43f5a4bad309fdea412f3594cc9017533633c0c399d3c88964","source":"sources/w8-core-loop-e-script-activate-post-action/script_activate_post_action.cpp","emitted_functions":[{"name":"scriptActivatePostAction__Fv","compiled_body_bytes":256}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_clear.o","profile":"authored_cpp","sha256":"0942953b6ef4373986d129c1ec4c2cff7b45c8574e1931125756c71b43604af7","source":"sources/w8-core-loop-e-script-clear/script_clear.cpp","emitted_functions":[{"name":"scriptClear__Fv","compiled_body_bytes":28}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_clear_all_game_state.o","profile":"authored_cpp","sha256":"933e09597b1e2df9d9f1ea98b0f49b4bbed07d808dc1f98b9bcab06f1a6bcddd","source":"sources/w8-core-loop-e-script-clear-all-game-state/script_clear_all_game_state.cpp","emitted_functions":[{"name":"scriptClearAllGameState__Fv","compiled_body_bytes":76}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_run.o","profile":"authored_cpp","sha256":"1c09d05118d8241369a75d791cd0fe7454aa726a22daf28c1acd6899f1b4a9da","source":"sources/w8-core-loop-e-script-run/script_run.cpp","emitted_functions":[{"name":"scriptRun__Fv","compiled_body_bytes":368}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_set_script.o","profile":"authored_cpp","sha256":"4ad713ace1edb21665b940112e933d60cbcf7ee0a7d36e8d62971ae195d79f26","source":"sources/w8-core-loop-e-script-set-script/script_set_script.cpp","emitted_functions":[{"name":"scriptSetScript__FPv","compiled_body_bytes":84}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_super_clear.o","profile":"authored_cpp","sha256":"6d8d17a6e557584849a490193d19c9a5e6410f5dd91dc2df52a3a2cca1ee5bae","source":"sources/w8-core-loop-e-script-super-clear/script_super_clear.cpp","emitted_functions":[{"name":"scriptSuperClear__Fv","compiled_body_bytes":36}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"script_task.o","profile":"authored_cpp","sha256":"a4971d19a18267068a36b2476c76bf119a8f89633b3f0841b30f7b86041ca5c2","source":"sources/w8-core-loop-e-script-task/script_task.cpp","emitted_functions":[{"name":"scriptTask__Fv","compiled_body_bytes":240}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"set_extra_params.o","profile":"authored_cpp","sha256":"e65ee8a705c20c2036a06539e3d7860a22c2051f39c14cc7600e19d734bb5cab","source":"sources/w8-core-loop-e-set-extra-params/set_extra_params.cpp","emitted_functions":[{"name":"setExtraParams__FP8CreaturePc","compiled_body_bytes":40}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"setup_main.o","profile":"authored_cpp","sha256":"a45afd7e682eaac972b291b1954b8523b17e7fe01b40347fd7be56c4a7d048b2","source":"sources/w8-core-loop-e-setup-main/setup_main.cpp","emitted_functions":[{"name":"setup_main__Fv","compiled_body_bytes":712}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"show_coords.o","profile":"authored_cpp","sha256":"eeeb6fd404f16c4ed7df7e6243d147684f37ab9e9b5054b8f8f32064aa39a047","source":"sources/w8-core-loop-e-show-coords/show_coords.cpp","emitted_functions":[{"name":"showCoords__Fv","compiled_body_bytes":752}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"start_new_world.o","profile":"authored_cpp","sha256":"961cd9768142a6427757128a161676eaaa6b477e1114cb8912bacbbe5c0a47de","source":"sources/w8-core-loop-e-start-new-world/start_new_world.cpp","emitted_functions":[{"name":"startNewWorld__Fv","compiled_body_bytes":1384}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"station_main.o","profile":"authored_cpp","sha256":"63242cbb42aaaf0eed1c658df83d66d2b6a20dbf1674ac1199721b344166d29a","source":"sources/w8-core-loop-e-station-main/station_main.cpp","emitted_functions":[{"name":"station_main__FiPCcPcT2T1T1i","compiled_body_bytes":464}]}
{"header_closure":[],"kind":"independently_authored_source","object_name":"sub_draw_loop.o","profile":"authored_cpp","sha256":"e478c7c35736b04f77a94e7f783d91784f8192c65e5e7fedaf14f8640e9e9e49","source":"sources/w8-core-loop-e-sub-draw-loop/sub_draw_loop.cpp","emitted_functions":[{"name":"subDrawLoop__FPFv_i","compiled_body_bytes":424}]}
```

## Authored but never executed

All 30 units. The functions have only been run on the host against mocks of their callees (including the `runMusic`
ring zeroing, the LCG steps, and all pointer-valued words through the host arena hook). No unit has run on the EE or
in a game. `startNewWorld`'s null-address store, `runScreaming`'s IOP RPC sequence, `setup_main`/`station_main`'s
overlay calls, and the `stack.15`/`retval` storage are unexecuted and unobserved beyond the instruction forms. The
function-local statics of `runAnimation` (`lastAmountPlayed`, `correction`) and `runScreaming` (`loadDone` and the
`load*` words) are ordinary zero-initialized local objects here; in the image the first pair is `.sdata`, so their
sections differ. `loadDone` keeps the image value 1.

## Unknown

- What `srcfile` should be as authored source (see above).
- The meaning of the many dialog-entry, stream-record and ambient-group fields; of `statusPtr`, `sentEndMsg`,
  `swooping` and `retval`; of `trippleBufferMode`, `waitForSound` and `worldDifficulty` beyond their stored values.
- Why `startNewWorld` stores through a null result, why `runMusic` clears its ring before reading it, and the units of
  the 1.742 / 1.072 / 0.25 thresholds.
- The `Player` constructor's trailing arguments, `Cat` fields outside the used offsets, and the real sizes of the
  `VISetup` (0x153f0 bytes allocated) and `VIStation` (at most 96960 bytes of stack) objects.
- Whether the function-return type of `subDrawLoop__FPFv_i` is void: nothing meaningful is left in the result register.
- The imports listed under "Imports opened" depend on other wave-8 packages; their final status is the integrator's.
