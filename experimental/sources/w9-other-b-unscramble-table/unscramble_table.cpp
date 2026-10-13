// Observed: unscrambleTable starts at 0x006e9080 in zero-filled storage; the next symbol, decodeQ, starts 256
// bytes later. makeUnscrambleTable__Fv, texDecompressThread__FPv and vqDecompress__FP8_texturei take its
// address.
// Inferred: a 256-entry byte table, from the gap and the existing authored makeUnscrambleTable.
// Unknown: the table contents (filled at run time).

extern "C" {
unsigned char unscrambleTable[256];
}
static_assert(sizeof(unscrambleTable) == 256, "unscrambleTable");
