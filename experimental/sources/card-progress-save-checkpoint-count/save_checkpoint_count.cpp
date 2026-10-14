// Observed: saveCheckPointCount is a 4-byte word. MC_Init stores zero.
// The image bytes are zero.
// Inferred: none.
// Unknown: which routine increments the count.
extern "C" int saveCheckPointCount = 0;
