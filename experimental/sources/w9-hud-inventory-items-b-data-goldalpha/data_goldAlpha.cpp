// Observed goldAlpha at 0x007d08e0, a declared-size-0 uninitialised object. The next symbol, playerGettinIt, starts 0x10 bytes later.
// The 16 bytes are taken from the gap to the next symbol (inferred); their contents at run time are
// unknown, and the object is zero-initialised here.
extern "C" {
extern unsigned char goldAlpha[16];
alignas(4) unsigned char goldAlpha[16];
}
