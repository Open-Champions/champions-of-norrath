// Observed msgTimeout at 0x008d8d28, a declared-size-0 uninitialised object. The next symbol, sQueue, starts 0x20 bytes later.
// The 32 bytes are taken from the gap to the next symbol (inferred); their contents at run time are
// unknown, and the object is zero-initialised here.
extern "C" {
extern unsigned char msgTimeout[32];
alignas(4) unsigned char msgTimeout[32];
}
