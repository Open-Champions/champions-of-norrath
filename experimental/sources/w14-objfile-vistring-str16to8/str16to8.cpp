// Observed: str16to8__8VIStringPcUiPCUs at 0x0101ac48, 224 bytes. Arguments (dst, limit, src); the result is dst.
//  - If the first source unit is zero, or limit is at most 1, a single zero byte is written at dst and nothing else.
//  - Otherwise units are converted one at a time, at most limit - 1 of them, stopping early after the unit before a zero unit;
//    a zero byte is stored after the last converted byte. A unit below 256 is stored as its low byte. Any other unit is looked up
//    in the 32-entry table of 16-bit values _8VIString$ConversionTable1252 (the entry count is read from the word
//    ConversionTableSize.180, which is 32 in the image): the first entry equal to the unit at index i gives the byte i + 128
//    (i - 128 stored as a signed byte); with no match, or a count that is not positive, the byte 63 ('?') is stored.
// Inferred: a Unicode-to-single-byte conversion with a code-page-1252-style table for the 0x80..0x9f range, truncated to fit a
// destination of limit bytes including the terminator. The table name comes from the symbol.
// Unknown: the table's owner and initialiser (both are imports here), and how units 128..255 should be treated by callers.
extern "C" {
extern const unsigned short _8VIString$ConversionTable1252[] __asm__("_8VIString$ConversionTable1252");
extern const int ConversionTableSize_180 __asm__("ConversionTableSize.180");

char* str16to8__8VIStringPcUiPCUs(char* dst, unsigned limit, const unsigned short* src) {
    char* out = dst;
    if (src[0] == 0 || limit <= 1) {
        *out = 0;
        return dst;
    }
    unsigned count = 1;
    for (;;) {
        unsigned unit = *src++;
        char byte;
        ++count;
        if (unit < 256) {
            byte = static_cast<char>(unit);
        } else {
            byte = '?';
            for (int i = 0; i < ConversionTableSize_180; ++i) {
                if (_8VIString$ConversionTable1252[i] == unit) {
                    byte = static_cast<char>(i - 128);
                    break;
                }
            }
        }
        *out++ = byte;
        if (*src == 0 || count >= limit) break;
    }
    *out = 0;
    return dst;
}
}
