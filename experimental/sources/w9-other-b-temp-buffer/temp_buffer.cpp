// Observed: tempBuffer starts at 0x00806b80 in zero-filled storage; the next symbol, ramSlot, starts 95936
// bytes later and loadBuffer precedes it. Only asyncChangePlayers__Fv and runConfigure__FiPUsi take its
// address; the former passes it to loadPlayerInfo__FiPiP12playerStruct.
// Inferred: a scratch buffer. The size is the gap to the next symbol, so it is an upper bound.
// Unknown: the real extent in use.

extern "C" {
unsigned char tempBuffer[95936];
}
static_assert(sizeof(tempBuffer) == 95936, "tempBuffer");
