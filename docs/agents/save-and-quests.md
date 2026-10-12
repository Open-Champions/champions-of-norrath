# save-and-quests

Package notes for branch `agents/save-and-quests`. Addresses are from the local reference `SLUS_205.65` ELF (not stripped). Every body was compiled with the reviewed `authored_cpp` profile and checked with a host program. None of it ran on the EE or as gameplay. Shared files (build plan, snapshot) were not edited; the entries needed are listed at the end.

`sources/save-and-quests-bytes/bytes.hpp` is a header closure (little-endian word helpers; host checks define `NORRATH_SQ_HOST` and map 32-bit object words to host pointers). It is not its own plan input.

## amx_GetAddr

Observed at `0x138758`, 76 bytes; the reviewed compile emits 128. Words at object+20, +24, +28 are compared as signed values with the address. A value in `[word20, word24)` or at or above `word28` returns 5 and writes nothing. Otherwise the result is base (object word 0) plus the word at base+16 plus the address, stored through the third argument, and 0 is returned. Field names are not established.

## amx_GetString

Observed at `0x138900`, 128 bytes; the reviewed compile emits 136. A first cell of 256 or more (signed) is a packed string: four characters per cell, high byte first, copied through the first zero character. Any other first cell is unpacked: a zero cell gives an empty string, otherwise the low byte of each cell is copied while the following cell is nonzero, then a zero is written. A negative first cell is therefore unpacked. The return is 0.
