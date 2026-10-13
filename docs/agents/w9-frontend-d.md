# front-end shell, player select, memory card and loader (d)

Package `w9-frontend-d`, branch `agents/w9-frontend-d`. New source is under `experimental/sources/w9-frontend-d-*/`, one directory per symbol group. Function directories also hold a `host_contract.cpp` that includes the unit and checks it with recording stubs; the contracts are not build-plan inputs. Data directories are checked by `host-tests/w9-frontend-d/test_data.py`. Addresses were read from the private reference executable `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`) with `nm` and `objdump`; nothing derived from it is committed.

## Functions authored

| Symbol | Address | Bytes in the image |
| --- | --- | --- |
| `resetWithClose__Fv` | 0x0025b8c0 | 72 |
| `scriptGameStateEntry__FiPi` | 0x001d3f60 | 156 |
| `startProgress__Fv` | 0x0025b968 | 8 |
| `textSend__Fv` | 0x002caab8 | 232 |
| `textureDrawSuperColor__FP8_textureiiiUiiiiicc` | 0x0016e860 | 936 |
| `textureLockCacheBuffer__Fv` | 0x00171138 | 152 |
| `texturePrepareToLockCacheBuffer__Fv` | 0x00172a28 | 12 |
| `textureReleaseCacheBuffer__Fv` | 0x00172a58 | 16 |
| `unModifyRamSlotForNetSave__Fv` | 0x0025a108 | 372 |
| `undoBrowser__Fv` | 0x0025bc88 | 148 |
| `runConfigure__FiPUsi` | 0x00251fe0 | 8032 |
| `runBrowser__FPUs` | 0x00253f40 | 8080 |

Data objects, each a unit of its own: the thirteen state words (`state_words.cpp`), `rumphole`, `savedScene`, `slotInfoHeaders`, `tempPlayerStruct` (`buffers.cpp`), `texStrings` (`tex_strings.cpp`) and `sGameOptions`, `sCdRmode` (`launcher_statics.cpp`). Sizes come from spans to the next symbol or from access widths; contents are zero except `texStrings`, which holds the ten `b_*.tex` names read from the image.

A previous interrupted run of this package had already committed everything except the two large functions, the notes and the plan entries; I re-read the commit messages and the sources, found them consistent, and continued from there.

## Checks performed

- Every unit compiles with the reviewed EE toolchain (`mips64r5900el-ps2-elf-g++` 15.2.0) under the unchanged `authored_cpp` flags, one unit at a time.
- Every `host_contract.cpp` builds with the host C++ compiler (`-std=c++17 -fno-builtin -Wall`, no warnings) and exits 0 (12 contracts). `host-tests/w9-frontend-d/test_data.py` passes (4 tests).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- Private paired runs against the image (a small R5900 interpreter kept outside git, described in the shared memory notes): the original function and the authored function run from identical memory snapshots with identical stub callees, and the call logs (names, arguments, strings read through pointers, float arguments), the return value, every changed byte outside the stack and the function-local statics are compared. For the ten small functions the earlier commit messages record matching runs. For `runConfigure__FiPUsi`: about 120000 randomised scenarios over all 33 states and the tail, 1974 of 2008 original instructions executed; the ones that were not are padding no-ops and the five-instruction unsigned-conversion branch for a value that is clamped to 127 and so cannot reach it. For `runBrowser__FPUs`: about 25000 scenarios over all 16 states, the list rows, the detail panel and every language branch, 1970 of 2020 instructions executed; the rest are padding no-ops and one branch for a play time of zero seconds that cannot be reached (see Unknown). Both comparisons were shown to fail on deliberately mutated sources (a changed constant, text key, size test, transition or plural suffix each produced mismatches).
- Not run: `tools/build_partial.py` (the shared plan and snapshot are off limits). The import comparison below uses the `work/plan9-build/unresolved-imports.json` baseline of the shared checkout.

## Imports closed

`requestToExit`, `resetWithClose__Fv`, `restrictChars`, `rumphole`, `runBrowser__FPUs`, `runConfigure__FiPUsi`, `sCdRmode`, `sGameOptions`, `saveOk`, `savedScene`, `scriptFrame`, `scriptGameStateEntry__FiPi`, `slotInfoHeaders`, `startExitFade`, `startFrame`, `startInhibit`, `startProgress__Fv`, `startScorePos`, `tempPlayerStruct`, `texStrings`, `textBodyLen`, `textSend__Fv`, `textureDrawSuperColor__FP8_textureiiiUiiiiicc`, `textureLockCacheBuffer__Fv`, `texturePrepareToLockCacheBuffer__Fv`, `textureReleaseCacheBuffer__Fv`, `texturesAreDecompressing`, `texturesAreLoading`, `unModifyRamSlotForNetSave__Fv`, `undoBrowser__Fv`, `waiting`, `waitingForStreamSpool`.

Not closed here: `sequence`, `sequenceInProgress` and `sequenceVolume`. They are defined (with the observed initial values) by `w8-audio-calls-sound-state/sound_state.cpp`, which is not a build-plan input at present; defining them again here would be a duplicate once that unit lands, so I left them to it. They close when that unit is in the plan.

## Imports opened

The sixteen units reference these external names that no w9-frontend-d unit defines (128 in all). The 26 below are still in the plan9 baseline's unresolved list, so they are genuinely open until their owners land:

`MCA_Busy__Fv` `MCA_DirContents` `MCA_Result` `_ctype_` `browserSlot` `cdIsBusy__Fv` `cdLoadAsync__FPcPvT0ii` `configFileName` `configFormatted` `evalHermite__Ffffff` `fontPrintWrapCenteredu__FiiPUsiiii` `hudHeaderUni` `memcardTitle__FPUs` `memmove` `outMsg` `popPause__Fi` `quickLoadStamp` `raceStrings` `ramSlot` `sortedDir` `sortedDirCount` `tempBuffer` `texDecodeStarted` `uncompMap` `uncompSpin` `ustrcat__FPUsPc`

The other 102 are not in that baseline list, so other units already in the plan define them; I did not verify each against the current plan:

`GLISTSIZE` `GetSaveFootPrint__6Camera` `MCA_Cmd__Fiiii` `MCA_FileOpen` `MCA_Formatted` `MCA_LastDirCount` `MCA_Port` `MCA_Present` `MCA_Reset__Fv` `MCA_Slot` `MCA_Sync__FPi` `MC_Debug` `MC_GameConfig` `asyncMode` `blocksFree` `browserDelete` `browserOverwrite` `browserState` `configState` `dataHandle` `delayCount` `dirEntryIsQuickLoad__FP14sceMcTblGetDir` `disableMC` `drawAddToList__FPviP8_textureP11_drawRecordi` `drawTexture__FR6PacketP8_textureiiiiii` `engineGetField__Fv` `engineScissory1` `engineScissory2` `engineScreenSlide` `errorCondition` `flareAlpha` `flareDelta` `fontColor__FUiii` `fontEnd__Fv` `fontPrintCenteredu__FiiPUsi` `fontPrint__FiiPci` `fontPrintu__FiiPUsi` `fontStart__FP4FontiUii` `frontEndRunning` `fullLaunchOK` `gameCam` `gameStateData` `generateSortedListAndInfoHeaders__Fv` `getLongLevelName__FPc` `getSlotName__FiiPc` `glistp` `gstart` `hudFooterUni` `hudRunYesNo__FPUsiiii` `hudShowMsg__FPUsN20iii` `iconHeader` `jumpGateSaveFootPrint__Fv` `language` `lastCardPort` `lastCardSlot` `lumpFindResource__FPcT0` `markFileName` `memset` `needToClearCacheBuffer` `netLocalPad` `netNetworkMode` `netSendMessage__FiPUci` `nmPlayers` `originalPos` `originalYaw` `pad` `padAcceptButton` `padAcceptButtonFontChar` `padBackButton` `padBackButtonFontChar` `patchSortedListWithLocalTime__Fv` `progress` `pushPause__Fv` `rectangleDraw__FiiiiiUi` `resetQuickLoad__Fv` `sceMtapPortOpen` `scePadGetSlotMax` `sessionMark` `slotClusters` `slotsCorrupted` `smallFont` `sortedPlayerRace` `sortedTimes` `soundPlaySequence__FP15AnimationHeaderP10GameObjectf` `sprintf` `strcmp` `strlen` `strstr` `supersampleMode` `swap__H1Zi_RX01T0_v` `textGetString__FPc` `textureDrawBorder__FiiiiiUiPcii` `textureUncompBufferLocked` `totalClusters` `uncompCacheBuffer` `undoQuickLoadStamp` `usprintf__FPUsPcffffffff` `usprintf__FPUsT0ffffffff` `ustrcat__FPUsT0` `ustrcpy__FPUsPc` `wasFormatting` `whichPad`

Points the integrator should check:

- `usprintf` is called with `char*`/pointer-valued floats: `runBrowser__FPUs` passes plural suffix strings as `(float)pointer` (observed: `cvt.s.w` of the address, `usprintf` converts back). That only works while the strings sit below 2^24; the authored literals land in `.rodata`, which a relinked image must keep under that address.
- `getSlotName__FiiPc` is declared returning `char*`; the executable uses its result both as a string and, converted to float, as a `usprintf` argument. Its definition is not in the plan baseline.
- `MCA_Cmd__Fiiii` is declared here with `long` arguments (second to fourth) because the callers pass numbers and pointers in the same slots; the other packages declare it with `const void*`/`int`. The register-level call is the same on the EE.
- The local statics of both big functions keep the executable's names in the symbol table (`oldCard.1178`, `curCard.1179`, `cdPos.1180`, `iconBytes.1181`, `fakeCardDisplay.1182`, `st.1183`, `cdDone.1184`, `originalSlotCount.1185`, `newSlot.1189`, `relativeSlot.1190`, `startSlot.1191`, `newStartSlot.1192`, `newBrowserState.1193`, `t.1194`). The authored statics are internal to each function; nothing else in the image names them.
- The float step 0x3e199999 (a bit pattern slightly below 0.15) at gp-22940 is written as the hex float `0x1.333332p-3f` in `runBrowser__FPUs`.

## Build-plan entries needed

Append these 16 entries to `inputs` in `experimental/build-plan.json`, in this order, starting at the integrator's next order (2318 is the shared plan's current maximum plus one; renumber if another package takes it first). `default_authored_inputs` rises by 16. The matching `files` hashes and snapshot records (with `emitted_functions` for the function units, none for the data units) are the integrator's. Object names do not collide with the shared plan as of this writing.

```json
[
 {"header_closure": [], "input_order": 2318, "kind": "independently_authored_source", "object_name": "buffers.o", "profile": "authored_cpp", "sha256": "004db45b5d72662807c8ca653d69b984a0bea4f21e81a66ca5271f4ff78d8b72", "source": "sources/w9-frontend-d-buffers/buffers.cpp"},
 {"header_closure": [], "input_order": 2319, "kind": "independently_authored_source", "object_name": "launcher_statics.o", "profile": "authored_cpp", "sha256": "a2ecd2d532e55612c3eb1f2f56e0f3ade00ef51d082675d154da8b14e42d9f9d", "source": "sources/w9-frontend-d-launcher-statics/launcher_statics.cpp"},
 {"header_closure": [], "input_order": 2320, "kind": "independently_authored_source", "object_name": "reset_with_close.o", "profile": "authored_cpp", "sha256": "1cec612f3943671b3e744a08aebb62ee2092968c874f3eaa628533ded68c4be8", "source": "sources/w9-frontend-d-reset-with-close/reset_with_close.cpp", "emitted_functions": [{"name": "resetWithClose__Fv", "compiled_body_bytes": 88}]},
 {"header_closure": [], "input_order": 2321, "kind": "independently_authored_source", "object_name": "run_browser.o", "profile": "authored_cpp", "sha256": "a874b7348df1099ce0d00b38013b6607ce725e228699e4aa62c369df2fa34313", "source": "sources/w9-frontend-d-run-browser/run_browser.cpp", "emitted_functions": [{"name": "_ZN12_GLOBAL__N_1L13sayAcceptBackEPtPKc", "compiled_body_bytes": 112}, {"name": "runBrowser__FPUs", "compiled_body_bytes": 8872}]},
 {"header_closure": [], "input_order": 2322, "kind": "independently_authored_source", "object_name": "run_configure.o", "profile": "authored_cpp", "sha256": "289ae1792eb9ac84903d17bdbf360e1e289e6149e87da0a269bf344757f16b92", "source": "sources/w9-frontend-d-run-configure/run_configure.cpp", "emitted_functions": [{"name": "_ZN12_GLOBAL__N_1L14deleteMatchingEPKcb", "compiled_body_bytes": 244}, {"name": "_ZN12_GLOBAL__N_1L16footerAcceptBackEPKc", "compiled_body_bytes": 100}, {"name": "runConfigure__FiPUsi", "compiled_body_bytes": 8416}]},
 {"header_closure": [], "input_order": 2323, "kind": "independently_authored_source", "object_name": "script_game_state_entry.o", "profile": "authored_cpp", "sha256": "e380060cc9ac5282ee1d7e27ef89af4699a72fa57c95004804bfd28d9a77517c", "source": "sources/w9-frontend-d-script-game-state-entry/script_game_state_entry.cpp", "emitted_functions": [{"name": "scriptGameStateEntry__FiPi", "compiled_body_bytes": 180}]},
 {"header_closure": [], "input_order": 2324, "kind": "independently_authored_source", "object_name": "start_progress.o", "profile": "authored_cpp", "sha256": "f7b2c779c7008fff862f849b75bca28b919685f5d2de6b4bd70228af1ae3c9c0", "source": "sources/w9-frontend-d-start-progress/start_progress.cpp", "emitted_functions": [{"name": "startProgress__Fv", "compiled_body_bytes": 12}]},
 {"header_closure": [], "input_order": 2325, "kind": "independently_authored_source", "object_name": "state_words.o", "profile": "authored_cpp", "sha256": "8c96d4294d89b450ad449ec89c97b2e96129371f96f64578c4aa420a378b076b", "source": "sources/w9-frontend-d-state-words/state_words.cpp"},
 {"header_closure": [], "input_order": 2326, "kind": "independently_authored_source", "object_name": "tex_strings.o", "profile": "authored_cpp", "sha256": "cf8cb4b28751c89066d93ef74ba8aceafba628f15f546eb1819289b10f3e2e7e", "source": "sources/w9-frontend-d-tex-strings/tex_strings.cpp"},
 {"header_closure": [], "input_order": 2327, "kind": "independently_authored_source", "object_name": "text_send.o", "profile": "authored_cpp", "sha256": "a8305b897965df081612155ffd5d804a742e636b8a6306c985bc642d61bfaa18", "source": "sources/w9-frontend-d-text-send/text_send.cpp", "emitted_functions": [{"name": "textSend__Fv", "compiled_body_bytes": 244}]},
 {"header_closure": [], "input_order": 2328, "kind": "independently_authored_source", "object_name": "texture_draw_super_color.o", "profile": "authored_cpp", "sha256": "16390cc2005b1bc8b4306dec6b1141214ba0de55b51c919c8920cad276b127fc", "source": "sources/w9-frontend-d-texture-draw-super-color/texture_draw_super_color.cpp", "emitted_functions": [{"name": "textureDrawSuperColor__FP8_textureiiiUiiiiicc", "compiled_body_bytes": 976}]},
 {"header_closure": [], "input_order": 2329, "kind": "independently_authored_source", "object_name": "texture_lock_cache_buffer.o", "profile": "authored_cpp", "sha256": "3b390d14f749833ab4f5e2c9ad085ee787e7a4a1423171de8d5328e810aa97b5", "source": "sources/w9-frontend-d-texture-lock-cache-buffer/texture_lock_cache_buffer.cpp", "emitted_functions": [{"name": "textureLockCacheBuffer__Fv", "compiled_body_bytes": 188}]},
 {"header_closure": [], "input_order": 2330, "kind": "independently_authored_source", "object_name": "texture_prepare_to_lock_cache_buffer.o", "profile": "authored_cpp", "sha256": "460d236767659decbf6c24691bd91b16ede3e9090c1f8796b14ce51bdb64af62", "source": "sources/w9-frontend-d-texture-prepare-to-lock-cache-buffer/texture_prepare_to_lock_cache_buffer.cpp", "emitted_functions": [{"name": "texturePrepareToLockCacheBuffer__Fv", "compiled_body_bytes": 16}]},
 {"header_closure": [], "input_order": 2331, "kind": "independently_authored_source", "object_name": "texture_release_cache_buffer.o", "profile": "authored_cpp", "sha256": "c5846332dcd3533af042feb074962c0f6ff762492ff039e4026fc8da0e8ddd68", "source": "sources/w9-frontend-d-texture-release-cache-buffer/texture_release_cache_buffer.cpp", "emitted_functions": [{"name": "textureReleaseCacheBuffer__Fv", "compiled_body_bytes": 24}]},
 {"header_closure": [], "input_order": 2332, "kind": "independently_authored_source", "object_name": "un_modify_ram_slot_for_net_save.o", "profile": "authored_cpp", "sha256": "b16c6211cc52a4bad35369870c18c82db330374f42bb8e1e00966ff6617142c4", "source": "sources/w9-frontend-d-un-modify-ram-slot-for-net-save/un_modify_ram_slot_for_net_save.cpp", "emitted_functions": [{"name": "unModifyRamSlotForNetSave__Fv", "compiled_body_bytes": 340}]},
 {"header_closure": [], "input_order": 2333, "kind": "independently_authored_source", "object_name": "undo_browser.o", "profile": "authored_cpp", "sha256": "059b77c8a255f551afb8fc175aaf93c716c4a0be8c7b580182b068711a4c9816", "source": "sources/w9-frontend-d-undo-browser/undo_browser.cpp", "emitted_functions": [{"name": "undoBrowser__Fv", "compiled_body_bytes": 104}]}
]
```

## Authored but never executed

Everything. No unit has run on the EE or an emulator. The paired runs execute the original and the authored machine code on a small interpreter with scripted callees, which checks equivalence of effects, not behaviour in the game. In particular nothing here has run with a real memory card, real pad state, or the real `usprintf`/`hudShowMsg`/font routines, and the `asm volatile` no-op loop in state 4 of `runConfigure__FiPUsi` (two passes of four no-ops, observed in the instruction stream between two comparisons) is reproduced for fidelity only.

## Deliberate differences from the original

- Both large functions are written as one `switch` over the state word with a shared tail; the compiled code is larger (`runConfigure__FiPUsi` 8416 against 8032 bytes, `runBrowser__FPUs` 8872 against 8080). No instruction-for-instruction match is claimed.
- `runConfigure__FiPUsi` ignores its second parameter, as the original does.
- `runBrowser__FPUs` omits the play-time branch for zero seconds (the original computes the minutes, forces a zero to one and then still tests for zero); the compiler used for the executable did not remove it, a current compiler would.
- Several tests (`language` against 0..4, the `asyncMode` re-read in the load branch) are written once where the instruction stream repeats them with the same result.
- Strings the executable keeps in `.data` (`"*"`, `"slot"`, `"elf.ico"`, the `%d.`/`%d:%s%d%s` formats and the plural suffix table, ":  1 (") are literals in the authored code; the pointer values therefore differ from the image.

## Unknown

- The meaning of the states of both functions beyond what the code does. The names used in the comments (card chooser, error screen) describe behaviour only. In `runConfigure__FiPUsi` the states 12 to 20 look like a recovery path from their transitions only.
- What the commands passed to `MCA_Cmd__Fiiii` mean (0, 1, 2, 3, 5, 6, 7, 8, 9, 10, 11 appear) beyond the names the w8 notes suggest; the numbers are used as observed.
- What the strings `"*"`, `"slot"` and `"elf.ico"` (data after `fakeCardDisplay`) are for: they are the directory glob, the substring that marks save files and the disc file whose contents are written as `icon.ico`.
- The meaning of pad word bit 0x80 in `runBrowser__FPUs` (a delete request in asyncMode 2), the masks 0x101000 and 0x204000 beyond up/down use, and the value 125.0f passed as a float argument to the "MemcardSelectDeleteBack" text.
- Whether `sortedTimes` holds BCD date-time records (assumed from the arithmetic) and which field is the month, day and year.
- Whether the three words `blocksFree`, `totalClusters` and `slotClusters` are clusters or blocks; they are only compared and printed.
- The play-time branch noted above and the remaining unclaimed padding in both functions.
- `sequence`, `sequenceInProgress` and `sequenceVolume`: left to the audio package as described under Imports closed.
