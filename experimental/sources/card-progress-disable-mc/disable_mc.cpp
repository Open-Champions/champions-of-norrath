// Observed: disableMC is a 4-byte word. MC_Init stores its argument with a
// word store and later reloads it with a word load. The image bytes are zero.
// Inferred: none.
// Unknown: which other routine changes it after startup.
extern "C" int disableMC = 0;
