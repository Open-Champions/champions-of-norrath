# save-and-quests

Package notes for branch `agents/save-and-quests`. Addresses are from the local reference `SLUS_205.65` ELF (not stripped). Every body was compiled with the reviewed `authored_cpp` profile and checked with a host program. None of it ran on the EE or as gameplay. Shared files (build plan, snapshot) were not edited; the entries needed are listed at the end.

`sources/save-and-quests-bytes/bytes.hpp` is a header closure (little-endian word helpers; host checks define `NORRATH_SQ_HOST` and map 32-bit object words to host pointers). It is not its own plan input.

## amx_GetAddr

Observed at `0x138758`, 76 bytes; the reviewed compile emits 128. Words at object+20, +24, +28 are compared as signed values with the address. A value in `[word20, word24)` or at or above `word28` returns 5 and writes nothing. Otherwise the result is base (object word 0) plus the word at base+16 plus the address, stored through the third argument, and 0 is returned. Field names are not established.

## amx_GetString

Observed at `0x138900`, 128 bytes; the reviewed compile emits 136. A first cell of 256 or more (signed) is a packed string: four characters per cell, high byte first, copied through the first zero character. Any other first cell is unpacked: a zero cell gives an empty string, otherwise the low byte of each cell is copied while the following cell is nonzero, then a zero is written. A negative first cell is therefore unpacked. The return is 0.

## scriptSetPostAction__FPc

Observed at `0x001d4060`, 36 bytes; the reviewed compile emits 16. It calls `strcpy` with the existing 200-byte `norrath_script_post_action_view` as destination and the argument as source. The copy is unbounded in the original; the 200-byte size is inferred from the next symbol.

## scriptAddDeferedCall__FPc

Observed at `0x001d3d40`, 108 bytes; the reviewed compile emits 100. While `nextNmDeferedCalls` (int, `0x00486b9c`) is below 64, the string is copied into slot `nextNmDeferedCalls` of bank `(scriptFrame & 1) ^ 1` of `deferedCall` (`0x007d0de8`) and the count is incremented; a full list drops the call. `scriptFrame` is the int at `0x00485088`. Inferred from the strides: banks are 5120 bytes, slots 80 bytes, and `deferedCall` is 10240 bytes (it ends at the game-state table). The copy is unbounded. Opens imports `nextNmDeferedCalls`, `scriptFrame`, `deferedCall`.

## scriptStart__FPc

Observed at `0x001d3e88`, 132 bytes; the reviewed compile emits 188. A null name or null `curScriptPtr` returns -1. `amx_FindPublic(amx, name, &index)` nonzero returns -1 and, when the function-local static `complainFrame.672` (word `0x0048508c`, initially -10000) is below `drawFrameNm`, sets it to `drawFrameNm + 20`. A zero status runs `amx_Exec(amx, &result, index, 0, 0)` and returns the stored word. `result` starts as the zero status, so a run that stores nothing returns 0. The fourth and fifth argument registers are zero; reading them as parameter count and data follows the existing executor's declaration. Opens imports `amx_FindPublic`, `amx_Exec`; uses existing views `norrath_script_current_amx_word_view` and `norrath_script_complain_frame_word_view`.

## scriptGetGameState__FPc

Observed at `0x001d39c0`, 236 bytes; the reviewed compile emits 256. The name is copied (unbounded) into a 256-byte stack buffer and lower-cased with the newlib table (`_ctype_[c + 1] & 1` adds 32). The table `norrath_script_game_state_data_view` (`gameStateData`, `0x007d35e8`) is read as records of a NUL-terminated name plus one signed value byte, ended by a record starting with a zero byte. A match returns the value byte sign-extended. No match appends the lower-cased key, clears its value byte, and returns 0; the byte after the new record is not written. Opens imports `_ctype_`, `strcmp`, `strlen`, `strcpy` (some already defined elsewhere).

## scriptSetGameState__FPci

Observed at `0x001d3ab0`, 248 bytes; the reviewed compile emits 296. The value is clamped to -128..127. The name is lower-cased exactly as in `scriptGetGameState__FPc` and searched in the same record table. The clamped value is stored in the byte after the terminator of the matching record, or of the newly appended key. Returns nothing.

## addQuestToLog__FPcT0

Observed at `0x001c1a60`, 236 bytes; the reviewed compile emits 296. Player records (`player`, `0x00408600`, 23984-byte stride) each hold a quest log: 64-byte entries from record byte 17768 (name text, then description text at entry byte 32) and an entry count word at record byte 21864. The 64-entry limit is observed from the compare and the count position. Walking `nmPlayers` records (re-read after each), a record with count 64 or more ends the whole call, and so does the first record already holding an entry with this name. Otherwise name and description are copied unbounded into the entry at the count and the count increments. Nothing in the record header is named.

## removeQuestFromLog__FPc

Observed at `0x001be638`, 364 bytes; the reviewed compile emits 432. Same record layout as `addQuestToLog__FPcT0`. For each of `nmPlayers` records, every entry whose name equals the argument has nine bytes copied over both its name and its description from an image constant at `0x00478d90` (observed bytes: `hit`, five zeros, `c`; the last byte is the first byte of the next literal). The constant is embedded as a nine-byte local; its purpose is unknown. The last matching index is kept; later entries are then moved down by `strcpy` of name and description, and the count word decrements. Unlike the add call, no record ends the walk, and a count of zero or less is not scanned.

## AsciiString2Sjis__FPUcPUs

Observed at `0x0024fc10`, 160 bytes; the reviewed compile emits 148. For each source byte it calls `Ascii2Sjis__FUc`, swaps the low two bytes of the result, and stores one halfword. A zero halfword terminates the output. The host check substitutes a stand-in for `Ascii2Sjis__FUc`; the real converter's table is not part of this package. Opens import `Ascii2Sjis__FUc` (and `strlen`).

## textHandler__FPUci

Observed at `0x002ca8d0`, 488 bytes; the reviewed compile emits 436. Symbols (`nm` sizes): `inMsg` `0x008d8938`, 1008 bytes (eight 126-byte slots), `msgTimeout` `0x008d8d28`, 32 bytes (eight words), `outMsg` `0x008d88b8`, 128 bytes, `waiting` `0x00486dac`, 4 bytes. A message shifts slots and words up one place (index 6 to 0), copies `size` bytes into slot 0 (unbounded), and zeroes `msgTimeout[0]`. If slot 0 matches `outMsg` on bytes 0, 1, 2 and on `slot[0] + 1` bytes from byte 3, `outMsg` byte 0 becomes 1, byte 3 becomes 32, bytes 4 and 1 become 0, and `waiting` is cleared. The registrar (`dataHandle` set by the net registration) is not in this function; what the fields mean is unknown (inferred only: received text lines and an outgoing text buffer). Opens imports `inMsg`, `msgTimeout`, `outMsg`, `waiting`, `memcpy`, `memcmp`.

## msg_load__10GameObjectPc and msg_save__10GameObjectPc

Observed at `0x00312e78` and `0x00312e80`, 8 bytes each, weak symbols in the reference. Both return 0 and touch nothing, so the base object has no persistent state. The authored versions are ordinary (strong) definitions.

## msg_load__5LeverPc

Observed at `0x0021c9e0`, 112 bytes. Reads a four-byte value (bit pattern, stored at object+76), a word (object+12), a byte (object+80) and a byte (object+81) from the buffer and returns 10. If the byte at object+81 is nonzero it calls `objectAddToRunList__FP10GameObject` with the object. Mirrors `msg_save__5LeverPc` (`vslots-b-lever-save`). Opens import `objectAddToRunList__FP10GameObject`.

## msg_save__8CreaturePc

Observed at `0x001119d8`, 436 bytes; the reviewed compile emits 576. Buffer order: word at object+244; single at object+560 as an integer; string-pool index of the text at the pointer in object+892. If the single at object+560 is at most zero (NaN skips it): object+36 points to a table of 8-byte entries (text pointer, header pointer); from entry 1 up to the first zero text pointer, the entry whose header pointer equals the word at `object + 264 + (signed byte at object+292) * 32` supplies the text whose pool index is stored, else -1. Then, if bit 0 of the 64-bit flags at object+160 is set, the single at object+564 as an integer and the signed byte at object+76 as a word. Then the byte at object+570, a mask byte (flag bit 24 -> 1, bit 32 -> 2, bit 22 -> 4, bit 23 -> 8), two zero bytes. Returns the byte count. Float-to-integer conversion truncates toward zero. No field is named. Opens import `objectSaveAddToStringPool__FPCc`.

## msg_load__8CreaturePc

Observed at `0x00111b90`, 608 bytes; the reviewed compile emits 824 (plus a 52-byte local helper). Reads what `msg_save__8CreaturePc` wrote: word to object+244; integer word converted to single at object+560; pool lookup of a word stored at object+892; virtual slot at vtable byte 200 on the object (the vtable word is at object+72; a signed halfword adjusts `this`). If the single is at most zero: object+12 |= 0x2009, halfword 10 at object+572, `objectRemoveFromRunList`, a pool lookup of the next word, and (if non-null) a `strcmp` search of the table at object+36 from entry 1; a match runs `animInitState`/`animAdd(…, 4)`/`animSetToEnd` on object+288 and calls the vtable slot at byte 192. Then, for every such object, the type-info function at vtable byte 4 is called and compared with `__tf3Orc` through `__eq__C9type_infoRC9type_info`; equality stores 70 at object+40. A null object calls `__throw_bad_typeid` (observed in the code; unreachable after the earlier dereferences). If flag bit 0 of the 64-bit word at object+160 is set, two words follow (a single at object+564, a low byte at object+76). A byte then goes to object+570, a mask byte sets flag bits 24/32/22/23 (mask 1/2/4/8), two bytes are skipped, and the byte count is returned. The type-info slot is called without `this` in the original (the argument register was not set); the authored call passes nothing. Inferred, not observed: the `__tf3Orc` comparison is a `typeid(*this) == typeid(Orc)` test. Opens imports `objectSaveGetFromStringPool__Fi`, `objectRemoveFromRunList__FP10GameObject`, `animInitState__FP14AnimationState`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animSetToEnd__FP14AnimationState`, `__tf3Orc`, `__eq__C9type_infoRC9type_info`, `__throw_bad_typeid`.

## msg_save__4PropPc

Observed at `0x0014b690`, 372 bytes; the reviewed compile emits 504. Buffer order: three string-pool indexes (`objectSaveAddToStringPool__FPCc`) of the inline texts at object+84, +100 and +164; the 64-bit value at object+232 (if it equals `1 << lilog2(value)` the exponent byte, otherwise a word of -1, the eight value bytes and one zero byte); the matrix at object+272 turned into a quaternion (`matToQuaternion__FP4QuatP8Matrix34`, then `quatNormalize__FP4Quat`) with the first three components times 127 stored as bytes and the fourth times 10000 (observed constant `0x461c4000`) as a halfword; the singles at object+308, +312, +316 as halfwords. Truncating conversions. Returns the byte count (24 or 36). Opens imports `lilog2__Fl`, `matToQuaternion__FP4QuatP8Matrix34`, `quatNormalize__FP4Quat`, `objectSaveAddToStringPool__FPCc`.

## msg_load__4PropPc

Observed at `0x0014b808`, 472 bytes; the reviewed compile emits 648. Reads what `msg_save__4PropPc` wrote. Three pool lookups (`objectSaveGetFromStringPool__Fi`) all happen before the three `strcpy` calls into object+84, +100, +164; object+12 gains bit 12 (0x1000); a signed byte of -1 introduces seven skipped bytes and an eight-byte value (then one more byte), any other byte stores `1 << byte` (count modulo 64) as the 64-bit value at object+232; three signed bytes times the observed constant `0x3c010204` and one signed halfword times `0x38d1b717` make a quaternion for `quatToMat__FP8Matrix34P4Quat` into object+272; three signed halfwords become singles at object+308, +312, +316 (unscaled); then `lumpFindResource__FPcT0(object+84, object+100)` goes to object+76 and `(object+84, object+164)` to object+80. Returns the byte count. Opens imports `quatToMat__FP8Matrix34P4Quat`, `lumpFindResource__FPcT0`.

## msg_save__16CosmeticPropAnimPc

Observed at `0x00155200`, 200 bytes; the reviewed compile emits 308. Buffer order: four string-pool indexes (`objectSaveAddToStringPool__FPCc`) of the text pointers at object+76, +80, +84, +88; the words at object+12, +148, +144; the singles at object+32 and object+28 as truncated integers stored as halfwords. Returns 32. Opens import `objectSaveAddToStringPool__FPCc` (shared with the other save bodies).

## msg_load__16CosmeticPropAnimPc

Observed at `0x0014b1c0`, 356 bytes; the reviewed compile emits 652. Reads what `msg_save__16CosmeticPropAnimPc` wrote. Four pool lookups (`objectSaveGetFromStringPool__Fi`) become pointers at object+76, +80, +84, +88; words go to object+12, +148, +144; two signed halfwords become singles at object+32 and +28 and their low bytes go to object+41 and +40. `lumpFindResource__FPcT0(object+76, object+80)` goes to object+92 and `(object+76, object+84)` to object+96; `(object+76, object+88)` is the header given to `animAdd__FP14AnimationStateP15AnimationHeaderi` on the state at object+100 with count 4 if bit 1 of the word at object+148 is set, else 1. Bit 0 of that word adds `animSetToEnd__FP14AnimationState`. Then `objectAddToSlowRunList__FP10GameObject`. Returns 32. Opens imports `objectAddToSlowRunList__FP10GameObject`, `animAdd`, `animSetToEnd`, `lumpFindResource__FPcT0` (shared with other bodies).

## Data: stateData, dataHandle, scriptInPostAction

Observed from the ELF symbol table: `stateData` `0x006e1c40`, `.bss`, 32 bytes (next symbol 32 bytes later; `subsystem-inits.md` records four 8-byte clears in `padInit__Fv`); `dataHandle` `0x00486db0`, `.sbss`, 4 bytes; `scriptInPostAction` `0x00485097`, 1 byte in `.sdata`, initial value 0. All three are zero-filled objects here. Under the `authored_cpp` profile the last lands in BSS rather than `.sdata` (value unchanged). Layout beyond size is unknown; none is read by anything authored in this package.

## Imports closed (23)

`AsciiString2Sjis__FPUcPUs`, `addQuestToLog__FPcT0`, `amx_GetAddr`, `amx_GetString`, `dataHandle`, `msg_load__10GameObjectPc`, `msg_load__16CosmeticPropAnimPc`, `msg_load__4PropPc`, `msg_load__5LeverPc`, `msg_load__8CreaturePc`, `msg_save__10GameObjectPc`, `msg_save__16CosmeticPropAnimPc`, `msg_save__4PropPc`, `msg_save__8CreaturePc`, `removeQuestFromLog__FPc`, `scriptAddDeferedCall__FPc`, `scriptGetGameState__FPc`, `scriptInPostAction`, `scriptSetGameState__FPci`, `scriptSetPostAction__FPc`, `scriptStart__FPc`, `stateData`, `textHandler__FPUci`.

A private partial build (reviewed `authored_cpp` profile, plan and snapshot copies kept outside git) with all 23 units added compiled 931 units and linked once: unresolved imports went from 950 to 944 (23 closed, 17 opened, none duplicated). No shared file was edited.

## Imports opened (17)

`Ascii2Sjis__FUc`, `__tf3Orc`, `amx_Exec`, `amx_FindPublic`, `deferedCall`, `inMsg`, `lilog2__Fl`, `memcmp`, `msgTimeout`, `nextNmDeferedCalls`, `objectAddToRunList__FP10GameObject`, `objectAddToSlowRunList__FP10GameObject`, `outMsg`, `quatNormalize__FP4Quat`, `quatToMat__FP8Matrix34P4Quat`, `scriptFrame`, `waiting`.

Several other names these bodies call (`strcpy`, `strcmp`, `strlen`, `_ctype_`, `nmPlayers`, `player`, `drawFrameNm`, `objectSaveAddToStringPool__FPCc`, `objectSaveGetFromStringPool__Fi`, `objectRemoveFromRunList__FP10GameObject`, `animInitState__FP14AnimationState`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animSetToEnd__FP14AnimationState`, `matToQuaternion__FP4QuatP8Matrix34`, `lumpFindResource__FPcT0`, `__eq__C9type_infoRC9type_info`, `__throw_bad_typeid`) were already unresolved or defined by other packages. `deferedCall`, `nextNmDeferedCalls`, `scriptFrame` (scriptAddDeferedCall), `inMsg`, `msgTimeout`, `outMsg`, `waiting` (textHandler) and `amx_Exec`, `amx_FindPublic` (scriptStart) have no authored definition anywhere yet; `scriptTask__Fv` (not in this package) is the likely owner of the deferred-call buffers.

## Build-plan entries needed

Append in this order to `experimental/build-plan.json` `inputs` (profile `authored_cpp`, kind `independently_authored_source`). `input_order` assumes the tip still ends at 909 (the vehicle-actors view); renumber if it moved, and add 23 to `default_authored_inputs`. `files` also needs `sources/save-and-quests-bytes/bytes.hpp` (hash below) and each source hash. Compiled body bytes per unit (for `snapshot.json` `emitted_functions`, as the driver reported them):

```
sources/save-and-quests-amx-get-addr/amx_get_addr.cpp: amx_GetAddr 128
sources/save-and-quests-amx-get-string/amx_get_string.cpp: amx_GetString 136
sources/save-and-quests-set-post-action/set_post_action.cpp: scriptSetPostAction__FPc 16
sources/save-and-quests-add-defered-call/add_defered_call.cpp: scriptAddDeferedCall__FPc 100
sources/save-and-quests-script-start/script_start.cpp: scriptStart__FPc 188
sources/save-and-quests-get-game-state/get_game_state.cpp: scriptGetGameState__FPc 256
sources/save-and-quests-set-game-state/set_game_state.cpp: scriptSetGameState__FPci 296
sources/save-and-quests-add-quest/add_quest.cpp: addQuestToLog__FPcT0 296
sources/save-and-quests-remove-quest/remove_quest.cpp: removeQuestFromLog__FPc 432
sources/save-and-quests-ascii-string-sjis/ascii_string_sjis.cpp: AsciiString2Sjis__FPUcPUs 148
sources/save-and-quests-text-handler/text_handler.cpp: textHandler__FPUci 436
sources/save-and-quests-game-object-load/game_object_load.cpp: msg_load__10GameObjectPc 8
sources/save-and-quests-game-object-save/game_object_save.cpp: msg_save__10GameObjectPc 8
sources/save-and-quests-lever-load/lever_load.cpp: msg_load__5LeverPc 136
sources/save-and-quests-creature-save/creature_save.cpp: msg_save__8CreaturePc 576
sources/save-and-quests-creature-load/creature_load.cpp: _ZN12_GLOBAL__N_1L9call_slotEPhj 52, msg_load__8CreaturePc 824
sources/save-and-quests-prop-save/prop_save.cpp: msg_save__4PropPc 504
sources/save-and-quests-prop-load/prop_load.cpp: msg_load__4PropPc 648
sources/save-and-quests-cosmetic-save/cosmetic_save.cpp: msg_save__16CosmeticPropAnimPc 308
sources/save-and-quests-cosmetic-load/cosmetic_load.cpp: msg_load__16CosmeticPropAnimPc 652
sources/save-and-quests-state-data/state_data.cpp: 
sources/save-and-quests-data-handle/data_handle.cpp: 
sources/save-and-quests-in-post-action/in_post_action.cpp: 
```

```json
[
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 910, "kind": "independently_authored_source", "object_name": "amx_get_addr.o", "profile": "authored_cpp", "sha256": "89594d4051da58fdefc79f05562bd747f85604f98d09bd3161b2b4111e97b313", "source": "sources/save-and-quests-amx-get-addr/amx_get_addr.cpp"},
{"header_closure": [], "input_order": 911, "kind": "independently_authored_source", "object_name": "amx_get_string.o", "profile": "authored_cpp", "sha256": "ee5f475cb38c289e06475af77a59707f3a2d09ffba490e4602a080a83f9b41b9", "source": "sources/save-and-quests-amx-get-string/amx_get_string.cpp"},
{"header_closure": [], "input_order": 912, "kind": "independently_authored_source", "object_name": "set_post_action.o", "profile": "authored_cpp", "sha256": "e8f36a2fad2523f4adde90c0d487d966fd055b2ec7fd08670decab485c983757", "source": "sources/save-and-quests-set-post-action/set_post_action.cpp"},
{"header_closure": [], "input_order": 913, "kind": "independently_authored_source", "object_name": "add_defered_call.o", "profile": "authored_cpp", "sha256": "abd827398e803a5c4fe661a762c093795578444c591c806d69a36fa73a864f06", "source": "sources/save-and-quests-add-defered-call/add_defered_call.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 914, "kind": "independently_authored_source", "object_name": "script_start.o", "profile": "authored_cpp", "sha256": "4fec6a7bafad43a874304c5b973f773b157d60ac8c48ea1a3b8b40218c95e0c1", "source": "sources/save-and-quests-script-start/script_start.cpp"},
{"header_closure": [], "input_order": 915, "kind": "independently_authored_source", "object_name": "get_game_state.o", "profile": "authored_cpp", "sha256": "1fc1723b2592068cf1ecb10f975360bec581c3e51e83aee0df3b213623667de1", "source": "sources/save-and-quests-get-game-state/get_game_state.cpp"},
{"header_closure": [], "input_order": 916, "kind": "independently_authored_source", "object_name": "set_game_state.o", "profile": "authored_cpp", "sha256": "abf955b1e052c52b867ef65fb6265aa341d46c3a235c972f1d9f7350ecad5314", "source": "sources/save-and-quests-set-game-state/set_game_state.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 917, "kind": "independently_authored_source", "object_name": "add_quest.o", "profile": "authored_cpp", "sha256": "039733aabbad82350024006d779fa0421724fa6d54ff39e1533af2ff7314cc7f", "source": "sources/save-and-quests-add-quest/add_quest.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 918, "kind": "independently_authored_source", "object_name": "remove_quest.o", "profile": "authored_cpp", "sha256": "a4e79d765342aae77dfb4832623d5d3829a5adadddb3edb823b62469478e39ef", "source": "sources/save-and-quests-remove-quest/remove_quest.cpp"},
{"header_closure": [], "input_order": 919, "kind": "independently_authored_source", "object_name": "ascii_string_sjis.o", "profile": "authored_cpp", "sha256": "eb5fb9c5873312ed948fb7ef3877ddb2e51235f28b7269c0aff4fb496d2dce99", "source": "sources/save-and-quests-ascii-string-sjis/ascii_string_sjis.cpp"},
{"header_closure": [], "input_order": 920, "kind": "independently_authored_source", "object_name": "text_handler.o", "profile": "authored_cpp", "sha256": "5ee90f2807c4f38da187bc9d6493460293566530bba768d5869e02809e0c1585", "source": "sources/save-and-quests-text-handler/text_handler.cpp"},
{"header_closure": [], "input_order": 921, "kind": "independently_authored_source", "object_name": "game_object_load.o", "profile": "authored_cpp", "sha256": "8c095b9a7542316e73a1eeefdc2c6b724570148e87b764bd18f315adcfdf1e21", "source": "sources/save-and-quests-game-object-load/game_object_load.cpp"},
{"header_closure": [], "input_order": 922, "kind": "independently_authored_source", "object_name": "game_object_save.o", "profile": "authored_cpp", "sha256": "2c7dc828fbd998c670ce84619b8e7e93fa0366b887690e4ead427f4201517adc", "source": "sources/save-and-quests-game-object-save/game_object_save.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 923, "kind": "independently_authored_source", "object_name": "lever_load.o", "profile": "authored_cpp", "sha256": "f92b53782c03e8bf450e7b41c36d751d508193eeb0abe33e55afc7b575aa66f0", "source": "sources/save-and-quests-lever-load/lever_load.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 924, "kind": "independently_authored_source", "object_name": "creature_save.o", "profile": "authored_cpp", "sha256": "01b80406fe4e3a85e2d5b952a2ffed1af02be13896546646d2d8a46b6b5b46b5", "source": "sources/save-and-quests-creature-save/creature_save.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 925, "kind": "independently_authored_source", "object_name": "creature_load.o", "profile": "authored_cpp", "sha256": "ba05211077291f65e81686775058f5e617bea64849a214b3dc5d62163b6b78fa", "source": "sources/save-and-quests-creature-load/creature_load.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 926, "kind": "independently_authored_source", "object_name": "prop_save.o", "profile": "authored_cpp", "sha256": "b03b0fc5f7533a8804fcf3b6c3f8a08d8ce6fa41779b5eaef2a9c4f4393c7128", "source": "sources/save-and-quests-prop-save/prop_save.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 927, "kind": "independently_authored_source", "object_name": "prop_load.o", "profile": "authored_cpp", "sha256": "1bd9b016495cbbb06b023a424b06e64ea55235c548771d1076bb1b4d93bc14a1", "source": "sources/save-and-quests-prop-load/prop_load.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 928, "kind": "independently_authored_source", "object_name": "cosmetic_save.o", "profile": "authored_cpp", "sha256": "40ea1cf182d9d35ca86efd0866741673002b40b12d3abc0d9054a18da95bf745", "source": "sources/save-and-quests-cosmetic-save/cosmetic_save.cpp"},
{"header_closure": ["sources/save-and-quests-bytes/bytes.hpp"], "input_order": 929, "kind": "independently_authored_source", "object_name": "cosmetic_load.o", "profile": "authored_cpp", "sha256": "d22d228ddf39cd6c5f016550126834df439ac367c6be066836425b0b6f31633b", "source": "sources/save-and-quests-cosmetic-load/cosmetic_load.cpp"},
{"header_closure": [], "input_order": 930, "kind": "independently_authored_source", "object_name": "state_data.o", "profile": "authored_cpp", "sha256": "3646a3e21270235486b84ee2eba05970975127ee7ed61c973368fcff25294ad6", "source": "sources/save-and-quests-state-data/state_data.cpp"},
{"header_closure": [], "input_order": 931, "kind": "independently_authored_source", "object_name": "data_handle.o", "profile": "authored_cpp", "sha256": "5a58ddb2994dd4c41733f6a896cc8353f28659402e0df71244fd547f0bc7e232", "source": "sources/save-and-quests-data-handle/data_handle.cpp"},
{"header_closure": [], "input_order": 932, "kind": "independently_authored_source", "object_name": "in_post_action.o", "profile": "authored_cpp", "sha256": "c4f7c7b0f654233bdbcec0e1529535f08a40a1063cc5d48dc78ecff49fd70eb7", "source": "sources/save-and-quests-in-post-action/in_post_action.cpp"}
]
```

## Snapshot files

Every file under `experimental/sources/save-and-quests-*` needs a `snapshot.json` `files` entry (host checks are snapshot files only, not plan inputs):

```
abd827398e803a5c4fe661a762c093795578444c591c806d69a36fa73a864f06  sources/save-and-quests-add-defered-call/add_defered_call.cpp
0a9e2b5e29929e3815f97236ce23e4a6b888a2dd5f089ac533fb0f83908a5085  sources/save-and-quests-add-defered-call/host_check.cpp
039733aabbad82350024006d779fa0421724fa6d54ff39e1533af2ff7314cc7f  sources/save-and-quests-add-quest/add_quest.cpp
f60484b49fad06a4ab1f6c86ca92c7b19c465c14b49b38e789ad712c13023d8f  sources/save-and-quests-add-quest/host_check.cpp
89594d4051da58fdefc79f05562bd747f85604f98d09bd3161b2b4111e97b313  sources/save-and-quests-amx-get-addr/amx_get_addr.cpp
287b4e9bdb037ea8c55cbde11500c93b0f94f31d95bd04db145a3e1ae86e1d32  sources/save-and-quests-amx-get-addr/host_check.cpp
ee5f475cb38c289e06475af77a59707f3a2d09ffba490e4602a080a83f9b41b9  sources/save-and-quests-amx-get-string/amx_get_string.cpp
fc40e845ac18b538343eaea2592167b6047dcc4317b50b320c5fffe4baab6a99  sources/save-and-quests-amx-get-string/host_check.cpp
eb5fb9c5873312ed948fb7ef3877ddb2e51235f28b7269c0aff4fb496d2dce99  sources/save-and-quests-ascii-string-sjis/ascii_string_sjis.cpp
fd728599547471a51b3209f44cf5610b86fc880b1c2cf6ff8f39ce9c08b54d95  sources/save-and-quests-ascii-string-sjis/host_check.cpp
dc984940fedd822330789ee092575ce3c60a013dbb68924ed1722ddb382a9c30  sources/save-and-quests-bytes/bytes.hpp
d22d228ddf39cd6c5f016550126834df439ac367c6be066836425b0b6f31633b  sources/save-and-quests-cosmetic-load/cosmetic_load.cpp
8c241f4d057d6a6232edb7f4e2c43748f93857b224f693895894bfb1a6855320  sources/save-and-quests-cosmetic-load/host_check.cpp
40ea1cf182d9d35ca86efd0866741673002b40b12d3abc0d9054a18da95bf745  sources/save-and-quests-cosmetic-save/cosmetic_save.cpp
1374f88b06d13b425ba1e8fb7d165723a1083f8cd9d4c120a24e550f0e255c4b  sources/save-and-quests-cosmetic-save/host_check.cpp
ba05211077291f65e81686775058f5e617bea64849a214b3dc5d62163b6b78fa  sources/save-and-quests-creature-load/creature_load.cpp
293fefdc16694e51f11669e70e5248a4e95bfebf890b4e853b2b0091ab14c87f  sources/save-and-quests-creature-load/host_check.cpp
01b80406fe4e3a85e2d5b952a2ffed1af02be13896546646d2d8a46b6b5b46b5  sources/save-and-quests-creature-save/creature_save.cpp
b40169753740c16132254e3ec04662318f0387cdea44d0fcbf5111db5a96d496  sources/save-and-quests-creature-save/host_check.cpp
5a58ddb2994dd4c41733f6a896cc8353f28659402e0df71244fd547f0bc7e232  sources/save-and-quests-data-handle/data_handle.cpp
a898046be80d001dca244c9680c7bb7702d5330ccf417469e7b9baeed370d4b0  sources/save-and-quests-data-handle/host_check.cpp
8c095b9a7542316e73a1eeefdc2c6b724570148e87b764bd18f315adcfdf1e21  sources/save-and-quests-game-object-load/game_object_load.cpp
f6d0c009ea14c006da3635bc8bada0cb9476d5bf7385f745f9fdcd93d3fa50d4  sources/save-and-quests-game-object-load/host_check.cpp
2c7dc828fbd998c670ce84619b8e7e93fa0366b887690e4ead427f4201517adc  sources/save-and-quests-game-object-save/game_object_save.cpp
82c0fa219177a1c7847dd00103625857776c5c1abdf288308bbbb9f7a315f243  sources/save-and-quests-game-object-save/host_check.cpp
1fc1723b2592068cf1ecb10f975360bec581c3e51e83aee0df3b213623667de1  sources/save-and-quests-get-game-state/get_game_state.cpp
d3d8c44f7c0427fae2f6b29d25fbdffe3e815275249a9907336ac9083ee886c6  sources/save-and-quests-get-game-state/host_check.cpp
0bd8c5f304cac2005356c885ac9c806749343ff1f257ed7491dc07d710c4b5c3  sources/save-and-quests-in-post-action/host_check.cpp
c4f7c7b0f654233bdbcec0e1529535f08a40a1063cc5d48dc78ecff49fd70eb7  sources/save-and-quests-in-post-action/in_post_action.cpp
be2fcd3c4e209c3a4e2bfb45c45c4efd77074e6264add58dd1f0a00e6bdb1b5c  sources/save-and-quests-lever-load/host_check.cpp
f92b53782c03e8bf450e7b41c36d751d508193eeb0abe33e55afc7b575aa66f0  sources/save-and-quests-lever-load/lever_load.cpp
506f38b5ae6681adc700a181144cc0a5eb06b9dcaa2fd759103dd7e34fbfb24d  sources/save-and-quests-prop-load/host_check.cpp
1bd9b016495cbbb06b023a424b06e64ea55235c548771d1076bb1b4d93bc14a1  sources/save-and-quests-prop-load/prop_load.cpp
a9b5214b1d5a8f6a2318adf67a66f95282e3325f6dc9179229e6b9d1c76ad0bc  sources/save-and-quests-prop-save/host_check.cpp
b03b0fc5f7533a8804fcf3b6c3f8a08d8ce6fa41779b5eaef2a9c4f4393c7128  sources/save-and-quests-prop-save/prop_save.cpp
8fb2cb4ceccea30407ecdd4b1fcc83fe8fa42baed13d8bc56d6e0a2119fb2403  sources/save-and-quests-remove-quest/host_check.cpp
a4e79d765342aae77dfb4832623d5d3829a5adadddb3edb823b62469478e39ef  sources/save-and-quests-remove-quest/remove_quest.cpp
35cf204da891e502b66dfe5ec5f43aea1839bd34666fdc223f9e150abfcb3895  sources/save-and-quests-script-start/host_check.cpp
4fec6a7bafad43a874304c5b973f773b157d60ac8c48ea1a3b8b40218c95e0c1  sources/save-and-quests-script-start/script_start.cpp
8825fa50b893628491b31d57253502ff522289440dace8ce861589646653668f  sources/save-and-quests-set-game-state/host_check.cpp
abf955b1e052c52b867ef65fb6265aa341d46c3a235c972f1d9f7350ecad5314  sources/save-and-quests-set-game-state/set_game_state.cpp
3c0528f1a0853b60fee716e75c722ed8072cf698f08eeee68c03dbf54764ce56  sources/save-and-quests-set-post-action/host_check.cpp
e8f36a2fad2523f4adde90c0d487d966fd055b2ec7fd08670decab485c983757  sources/save-and-quests-set-post-action/set_post_action.cpp
262c4215a3244cd241f093b60ab181bfa3ff4805c2e1cadfc9b462a3653f6f2d  sources/save-and-quests-state-data/host_check.cpp
3646a3e21270235486b84ee2eba05970975127ee7ed61c973368fcff25294ad6  sources/save-and-quests-state-data/state_data.cpp
85d21deecc595e64c9d86f9009f7c590ee19581f0fe17fe1aedac5cc1c7a6a8e  sources/save-and-quests-text-handler/host_check.cpp
5ee90f2807c4f38da187bc9d6493460293566530bba768d5869e02809e0c1585  sources/save-and-quests-text-handler/text_handler.cpp
```

## Authored but never executed

All 23 symbols compile with the reviewed EE toolchain and were run only as host programs on this machine against stand-ins for their callees. Nothing ran on the EE, in the original process, or as gameplay. The host mapping of 32-bit object words to host pointers (`NORRATH_SQ_HOST`) is a test device, not target behavior. `msg_load__8CreaturePc` exercises the type-info path only through stubs; the real `__tf3Orc`/type-info slot behavior was not run. `scriptStart__FPc` was not run against the real `amx_Exec`.

## Unknown

- Field and flag meaning in every save/load body (offsets and masks only); which object states the Creature, Prop, CosmeticPropAnim and Lever records belong to beyond their symbol names.
- Why `removeQuestFromLog__FPc` overwrites removed quest slots with nine bytes from `0x00478d90` (observed `hit`, five zeros, `c`); the constant is embedded as-is.
- The `deferedCall` slot size (80 bytes) and bank layout are inferred from strides only; the `postAction`/game-state table sizes come from symbol spacing.
- The `textHandler__FPUci` buffers are called by their ELF names; their role (received text lines versus an outgoing text buffer) is inferred, not observed.
- Whether the Creature `typeid(*this) == typeid(Orc)` reading is the original source form; only the call sequence and the stored byte 70 are observed.
- R5900 float-to-integer overflow and NaN behavior of the save conversions (host tests stay within range).
- Nothing in this package registers `textHandler__FPUci` or sets `dataHandle`; those registrars belong to the save-net-anim package.
