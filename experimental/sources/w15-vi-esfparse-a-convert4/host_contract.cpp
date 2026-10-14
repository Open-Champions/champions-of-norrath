#include <string.h>
#include "convert_4.cpp"
static unsigned char parse[40 + 1024]; static unsigned char dst[64];
static int run(int fmt, const unsigned char* src, int count) { int store = fmt; memset(dst, 0xee, sizeof dst);
    return Convert4__10VIESFParseR12VIPixelStorePUcT2i(parse, &store, const_cast<unsigned char*>(src), dst, count); }
static bool pal(int slot, int idx) { const unsigned char* e = parse + 40 + 4 * idx;
    return dst[4 * slot] == e[2] && dst[4 * slot + 1] == e[1] && dst[4 * slot + 2] == e[0] && dst[4 * slot + 3] == e[3]; }
int main() {
    for (int i = 0; i < 1024; ++i) parse[40 + i] = (unsigned char)(i * 7 + (i >> 2));
    const unsigned char src[4] = {0x2d, 0xa1, 0x55, 0x0f};
    if (run(1, src, 5) != 0 || dst[0] != 0xd || dst[1] != 0x2 || dst[2] != 1 || dst[3] != 0xa || dst[4] != 5 || dst[5] != 0xee) return 1;
    if (run(1, src, 4) != 0 || dst[3] != 0xa || dst[4] != 0xee) return 2;
    if (run(1, src, 0) != 0 || dst[0] != 0xee) return 3;
    if (run(0, src, 5) != 0 || memcmp(dst, src, 3) != 0 || dst[3] != 0xee) return 4;
    if (run(0, src, 4) != 0 || memcmp(dst, src, 2) != 0 || dst[2] != 0xee) return 5;
    if (run(6, src, 3) != 0 || !pal(0, 0xd) || !pal(1, 0x2) || !pal(2, 1) || dst[12] != 0xee) return 6;
    if (run(6, src, 0) != 0 || dst[0] != 0xee) return 7;
    if (run(2, src, 3) != -1 || dst[0] != 0xee) return 8;
    if (run(-1, src, 3) != -1) return 9;
    return 0;
}
