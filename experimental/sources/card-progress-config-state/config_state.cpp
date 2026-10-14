// Observed: configState is a 4-byte word. MC_Init stores -1. The image
// bytes are zero.
// Inferred: none.
// Unknown: the state numbers stored after startup.
extern "C" int configState = 0;
