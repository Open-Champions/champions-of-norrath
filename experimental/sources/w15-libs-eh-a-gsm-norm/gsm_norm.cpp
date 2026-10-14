// Observed gsm_norm at 0x002ed9e0, 216 bytes, pinned SLUS_205.65: the normalisation shift count of a GSM 06.10
// codec (how far a signed 32-bit quantity can be shifted left before it overflows). The argument is a 64-bit word and
// the logic runs on it directly:
//   * A negative value at or below -1073741824 (0xffffffffc0000000 as the image compares it) returns 0; any other
//     negative value is replaced by its complement.
//   * The value is then tested against masks on its low 32 bits: 0xffff0000 set, then 0xff000000 set returns
//     bitoff[(v >> 24) & 255] - 1, otherwise bitoff[(v >> 16) & 255] + 7; with 0xffff0000 clear, 0x0000ff00 set returns
//     bitoff[(v >> 8) & 255] + 15, otherwise bitoff[v & 255] + 23. bitoff is the 256-byte table at 0x0047c9d0.
// The masks are applied to the 64-bit value, so bits above bit 31 matter to the first test only through the shifts' byte
// extractions (which take 8 bits); values above 32 bits are outside what the callers pass.
// Unknown: nothing beyond the arithmetic; nothing was executed on the EE.
extern "C" {
extern const unsigned char bitoff[256];

int gsm_norm(long long value) {
    if (value < 0) {
        if (value <= -1073741824) return 0;
        value = ~value;
    }
    if (value & 0xffff0000ll) {
        if (value & 0xff000000ll) return bitoff[(value >> 24) & 0xff] - 1;
        return bitoff[(value >> 16) & 0xff] + 7;
    }
    if (value & 0x0000ff00ll) return bitoff[(value >> 8) & 0xff] + 15;
    return bitoff[value & 0xff] + 23;
}
}
