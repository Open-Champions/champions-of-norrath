// Observed: strcat16__8VIStringPUsUiPCc at 0x0101ae58 is 176 bytes. Arguments: destination of
// 16-bit units (a0), a limit (a1, unsigned) and a source of 8-bit characters (a2). The
// destination's current length is measured, then source characters are appended while the source
// character is non-zero and (byte offset of the write cursor from the start) + 2 < limit. Each
// character is sign-extended from 8 bits into the 16-bit unit (0xE9 becomes 0xFFE9); no code-page
// table is used. A zero unit is always written at the final cursor. The return value is the
// original destination pointer.
// Inferred: appends a narrow string to a wide one with plain sign extension, unlike the table-driven
// str8to16.
// Unknown: whether the sign extension is intended; it is what the original does.
extern "C" {
unsigned short* strcat16__8VIStringPUsUiPCc(unsigned short* dst, unsigned int limit, const char* src) {
    unsigned short* out = dst;
    while (*out != 0) ++out;
    while (*src != 0 && static_cast<unsigned int>(reinterpret_cast<unsigned char*>(out) - reinterpret_cast<unsigned char*>(dst)) + 2 < limit) {
        *out++ = static_cast<unsigned short>(static_cast<signed char>(*src++));
    }
    *out = 0;
    return dst;
}
}
