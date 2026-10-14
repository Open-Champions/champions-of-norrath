// Observed bitoff at 0x0047c9d0: 256 bytes of read-only data (a local object of the GSM codec's normalisation routine).
// Entry i is 8 minus the number of significant bits of i: 8 for 0, 7 for 1, 6 for 2..3, 5 for 4..7, 4 for 8..15, 3 for
// 16..31, 2 for 32..63, 1 for 64..127 and 0 for 128..255. gsm_norm (0x002ed9e0) indexes it with one byte of its argument.
// Unknown: nothing beyond the 256 values; nothing was executed on the EE.
#define W15_BITOFF_X2(v) v, v
#define W15_BITOFF_X4(v) W15_BITOFF_X2(v), W15_BITOFF_X2(v)
#define W15_BITOFF_X8(v) W15_BITOFF_X4(v), W15_BITOFF_X4(v)
#define W15_BITOFF_X16(v) W15_BITOFF_X8(v), W15_BITOFF_X8(v)
#define W15_BITOFF_X32(v) W15_BITOFF_X16(v), W15_BITOFF_X16(v)
#define W15_BITOFF_X64(v) W15_BITOFF_X32(v), W15_BITOFF_X32(v)
#define W15_BITOFF_X128(v) W15_BITOFF_X64(v), W15_BITOFF_X64(v)
extern "C" {
extern const unsigned char bitoff[256];
const unsigned char bitoff[256] = {
    8, 7, W15_BITOFF_X2(6), W15_BITOFF_X4(5), W15_BITOFF_X8(4), W15_BITOFF_X16(3), W15_BITOFF_X32(2), W15_BITOFF_X64(1),
    W15_BITOFF_X128(0)};
}
