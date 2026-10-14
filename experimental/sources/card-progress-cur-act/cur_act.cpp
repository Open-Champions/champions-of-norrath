// Observed: curAct is a 4-byte word. MC_Init stores -1. The image bytes
// are zero. The next symbol is 4 bytes later.
// Inferred: none.
// Unknown: which routine publishes a real act index.
extern "C" int curAct = 0;
