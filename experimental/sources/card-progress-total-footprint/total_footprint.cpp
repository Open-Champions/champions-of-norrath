// Observed: totalFootPrint is a 4-byte word. MC_Init stores 0xb6f5c.
// The image bytes are zero. The next symbol is 4 bytes later.
// Inferred: none.
// Unknown: which routine reads the stored total.
extern "C" int totalFootPrint = 0;
