// Observed: slotClusters is a 4-byte word. MC_Init stores 715 with a word
// store. One floating-point load uses the same address. The image bytes are
// zero.
// Inferred: none.
// Unknown: what that floating-point load expects from these bits.
extern "C" int slotClusters = 0;
