// Observed globalIcons at 0x0042a418, 152 bytes in .data, every byte zero in the image
// (the symbol before it, a 32-byte object, ends exactly where it starts). The init
// routine gameInitPostFrontend__Fv writes four-byte values into it at byte offsets
// taken from a table; the authored caller declares it as a byte array and indexes by
// those offsets.
// Unknown: the meaning of each word, and the word count used (38 words fit).
extern "C" {
alignas(4) unsigned char globalIcons[152] = {};
}
