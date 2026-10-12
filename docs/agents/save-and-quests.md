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
