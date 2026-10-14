// Observed: pauseStackPos is one byte. MC_Init stores zero with a byte
// store. Byte loads, including unsigned loads, use this address. The image
// byte is zero. The next symbol starts at the next byte.
// Inferred: none.
// Unknown: the stack depth this byte counts.
extern "C" unsigned char pauseStackPos = 0;
