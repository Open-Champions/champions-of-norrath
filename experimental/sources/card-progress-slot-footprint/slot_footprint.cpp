// Observed: slotFootPrint is a 4-byte word. MC_Init clears it, keeps the
// largest act sum, then stores 0xb29c8. The image bytes are zero.
// Inferred: none.
// Unknown: which later routine reads the stored footprint.
extern "C" int slotFootPrint = 0;
