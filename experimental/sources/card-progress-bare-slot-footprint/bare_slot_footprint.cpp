// Observed: bareSlotFootPrint is a 4-byte word. MC_Init stores
// slotFootPrint plus 648. The image bytes are zero.
// Inferred: none.
// Unknown: which routine reads it after that store.
extern "C" int bareSlotFootPrint = 0;
