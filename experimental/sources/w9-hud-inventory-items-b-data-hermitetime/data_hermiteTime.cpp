// Observed hermiteTime at 0x00486d34, a declared-size-0 uninitialised object. The next symbol, delta, starts 4 bytes later.
// The 4 bytes are taken from the gap to the next symbol (inferred); their contents at run time are
// unknown, and the object is zero-initialised here.
extern "C" {
extern unsigned char hermiteTime[4];
alignas(4) unsigned char hermiteTime[4];
}
