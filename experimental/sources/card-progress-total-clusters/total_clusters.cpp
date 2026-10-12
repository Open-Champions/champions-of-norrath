// Observed: totalClusters is a 4-byte word. MC_Init stores 849 with a word
// store. Three floating-point loads use the same address. The image bytes
// are zero.
// Inferred: none.
// Unknown: what those floating-point loads expect from these bits.
extern "C" int totalClusters = 0;
