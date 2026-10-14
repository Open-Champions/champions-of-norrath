#include <string.h>
#include "convert_32.cpp"
static unsigned char dst[64];
static int run(int fmt, const unsigned char* src, int count) { int store = fmt; memset(dst, 0xee, sizeof dst);
    return Convert32__10VIESFParseR12VIPixelStorePUcT2i(0, &store, const_cast<unsigned char*>(src), dst, count); }
int main() {
    const unsigned char src[8] = {0xf8, 0x41, 0x9e, 0x80, 0x07, 0xff, 0x08, 0x7f};
    if (run(3, src, 2) != 0 || memcmp(dst, "\xf8\x41\x9e\xff\x07\xff\x08\xff", 8) != 0 || dst[8] != 0xee) return 1;
    if (run(2, src, 2) != 0 || memcmp(dst, src, 8) != 0 || dst[8] != 0xee) return 2;
    if (run(5, src, 2) != 0) return 3;
    unsigned short h0 = (0xf8 >> 3) | ((0x41 >> 3) << 5) | ((0x9e >> 3) << 10) | (1 << 15), h1 = (0x07 >> 3) | ((0xff >> 3) << 5) | ((0x08 >> 3) << 10) | 0;
    if (dst[0] != (h0 & 0xff) || dst[1] != (h0 >> 8) || dst[2] != (h1 & 0xff) || dst[3] != (h1 >> 8) || dst[4] != 0xee) return 4;
    if (run(6, src, 2) != 0 || memcmp(dst, "\x9e\x41\xf8\x80\x08\xff\x07\x7f", 8) != 0 || dst[8] != 0xee) return 5;
    if (run(6, src, 0) != 0 || dst[0] != 0xee) return 6;
    if (run(3, src, -2) != 0 || dst[0] != 0xee) return 7;
    if (run(4, src, 2) != -1 || dst[0] != 0xee) return 8;
    if (run(0, src, 2) != -1) return 9;
    return 0;
}
