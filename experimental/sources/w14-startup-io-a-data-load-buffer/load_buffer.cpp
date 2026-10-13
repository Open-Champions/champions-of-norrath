// Observed: loadBuffer starts at 0x007f8100 in zero-filled storage. It has no recorded size;
// the next symbol, tempBuffer, starts 60032 bytes later. MC_LoadLevelEntities copies saved
// object and floor data into it and passes pointers into it to the object and floor loaders.
// Inferred: the extent is the gap to the next symbol, so 60032 is an upper bound.
// Unknown: the real extent in use, and its required alignment (the address is 256-aligned).
extern "C" {
unsigned char loadBuffer[60032];
}
