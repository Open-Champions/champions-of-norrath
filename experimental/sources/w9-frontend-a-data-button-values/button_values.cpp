// Observed: buttonValues at 0x004293b0 is 56 bytes, fourteen little-endian words in the image:
// 0x20, 0x10, 0x80, 0x40, 0, 0, 0, 0, 4, 1, 8, 2, 0x400, 0.
// Inferred: each nonzero word is a single-bit mask (0x400 included), so the table looks like
// pad button masks indexed by position; that is not confirmed by a reader here.
// Unknown: what the index means and which of the zero entries are unused.

extern "C" {
unsigned buttonValues[14] = {0x20, 0x10, 0x80, 0x40, 0, 0, 0, 0, 4, 1, 8, 2, 0x400, 0};
}
