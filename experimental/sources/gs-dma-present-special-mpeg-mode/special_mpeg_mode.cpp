// Observed specialMpegMode at 0x00486a64. Local BSS. The next symbol,
// freeList, is 4 bytes later. vblankHandler reads the first byte as signed.
// The file image is zero.
extern "C" {
signed char specialMpegMode;
}
static_assert(sizeof(specialMpegMode) == 1, "one byte");
