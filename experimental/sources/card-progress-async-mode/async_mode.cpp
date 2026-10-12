// Observed: asyncMode is a 4-byte word. newMode and MC_Init load and store
// it with word instructions. The image bytes are zero. Two other local
// symbols are not this word.
// Inferred: none.
// Unknown: the meaning of each stored mode.
extern "C" int asyncMode = 0;
