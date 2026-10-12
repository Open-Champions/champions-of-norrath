// Observed: lastCardSlot is a 4-byte word. MC_Init stores -1. The image
// bytes are zero.
// Inferred: none.
// Unknown: which slot numbers are stored later.
extern "C" int lastCardSlot = 0;
