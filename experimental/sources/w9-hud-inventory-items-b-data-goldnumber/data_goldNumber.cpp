// Observed goldNumber at 0x007d0588, a declared-size-0 uninitialised object. The next symbol, hudBox, starts 0x120 bytes later; hudGivePlayerGold__FiG6Point3i walks eight 36-byte entries from this address.
// The 288 bytes are taken from the gap to the next symbol (inferred); their contents at run time are
// unknown, and the object is zero-initialised here.
extern "C" {
extern unsigned char goldNumber[288];
alignas(4) unsigned char goldNumber[288];
}
