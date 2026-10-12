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
