// Observed: str8to16__8VIStringPUsUiPCc at 0x0101ad28 is 144 bytes. Arguments: destination of
// 16-bit units (a0), a limit (a1, unsigned) and a source of 8-bit characters (a2). Each source
// byte is read unsigned. Bytes 0x80..0x9f are replaced by the 16-bit entry at index (byte - 0x80)
// of _8VIString$ConversionTable1252; every other byte is stored as its own value. Copying continues
// while the source byte is non-zero and 2 * (units written + 1) < limit; a zero unit is always
// written at the final cursor, even for limits 0..2. The return value is the original destination.
// Inferred: widens a Windows-1252 style narrow string; the limit is the destination size in bytes.
// Unknown: whether callers pass byte sizes consistently.
extern "C" {
extern const unsigned short _8VIString$ConversionTable1252[32];

unsigned short* str8to16__8VIStringPUsUiPCc(unsigned short* dst, unsigned int limit, const char* src) {
    unsigned short* out = dst;
    unsigned int bytes = 2;
    while (*reinterpret_cast<const unsigned char*>(src) != 0 && bytes < limit) {
        unsigned int c = *reinterpret_cast<const unsigned char*>(src++);
        if (c - 0x80u < 0x20u) {
            *out++ = _8VIString$ConversionTable1252[c - 0x80u];
        } else {
            *out++ = static_cast<unsigned short>(c);
        }
        bytes += 2;
    }
    *out = 0;
    return dst;
}
}
