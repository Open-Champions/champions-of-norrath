// Observed nameTags at 0x008d5dc0, a declared-size-0 uninitialised object. The next symbol, lightInfo, starts 0x80 bytes later.
// The 128 bytes are taken from the gap to the next symbol (inferred); their contents at run time are
// unknown, and the object is zero-initialised here.
extern "C" {
extern unsigned char nameTags[128];
alignas(4) unsigned char nameTags[128];
}
