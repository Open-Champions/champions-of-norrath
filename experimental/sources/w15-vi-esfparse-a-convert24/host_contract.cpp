#include <string.h>
#include "convert_24.cpp"
static unsigned char dst[64];
static int run(int fmt, const unsigned char* src, int count) { int store = fmt; memset(dst, 0xee, sizeof dst);
    return Convert24__10VIESFParseR12VIPixelStorePUcT2i(0, &store, const_cast<unsigned char*>(src), dst, count); }
int main() {
    const unsigned char src[6] = {0xf8, 0x41, 0x9e, 0x07, 0xff, 0x08};
    for (int fmt = 2; fmt <= 3; ++fmt)
        if (run(fmt, src, 2) != 0 || memcmp(dst, "\xf8\x41\x9e\xff\x07\xff\x08\xff", 8) != 0 || dst[8] != 0xee) return 1;
    if (run(4, src, 2) != 0 || memcmp(dst, src, 6) != 0 || dst[6] != 0xee) return 2;
    unsigned short h0 = (0xf8 >> 3) | ((0x41 >> 3) << 5) | ((0x9e >> 3) << 10) | 0x8000, h1 = (0x07 >> 3) | ((0xff >> 3) << 5) | ((0x08 >> 3) << 10) | 0x8000;
    if (run(5, src, 2) != 0 || dst[0] != (h0 & 0xff) || dst[1] != (h0 >> 8) || dst[2] != (h1 & 0xff) || dst[3] != (h1 >> 8) || dst[4] != 0xee) return 3;
    if (run(6, src, 2) != 0 || memcmp(dst, "\x9e\x41\xf8\xff\x08\xff\x07\xff", 8) != 0 || dst[8] != 0xee) return 4;
    if (run(6, src, 0) != 0 || dst[0] != 0xee) return 5;
    if (run(6, src, -1) != 0 || dst[0] != 0xee) return 6;
    if (run(1, src, 2) != -1 || dst[0] != 0xee) return 7;
    if (run(7, src, 2) != -1 || run(0, src, 2) != -1) return 8;
    return 0;
}
