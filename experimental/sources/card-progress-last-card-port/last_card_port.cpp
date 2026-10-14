// Observed: lastCardPort is a 4-byte word. MC_Init stores -1. The image
// bytes are zero.
// Inferred: none.
// Unknown: which port numbers are stored later.
extern "C" int lastCardPort = 0;
