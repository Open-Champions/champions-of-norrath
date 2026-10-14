#include <string.h>
#include "convert_16.cpp"
static unsigned char dst[64];
static int run(int fmt, const unsigned char* src, int count) { int store = fmt; memset(dst, 0xee, sizeof dst);
    return Convert16__10VIESFParseR12VIPixelStorePUcT2i(0, &store, const_cast<unsigned char*>(src), dst, count); }
int main() {
    alignas(2) const unsigned char src[4] = {0x1f, 0x80, 0xa3, 0x5c};
    // p0 = 0x801f: colors (p<<3)=0xf8, (p>>2)&0xf8=0x00, (p>>7)&0xf8=0x00, alpha bit set
    // p1 = 0x5ca3: colors 0x18, 0x28, 0xb8, alpha bit clear
    if (run(2, src, 2) != 0 || memcmp(dst, "\xf8\x00\x00\x80\x18\x28\xb8\x00", 8) != 0 || dst[8] != 0xee) return 1;
    if (run(3, src, 2) != 0 || memcmp(dst, "\xf8\x00\x00\xff\x18\x28\xb8\xff", 8) != 0 || dst[8] != 0xee) return 2;
    if (run(6, src, 2) != 0 || memcmp(dst, "\x00\x00\xf8\x80\xb8\x28\x18\x00", 8) != 0 || dst[8] != 0xee) return 3;
    if (run(5, src, 2) != 0 || memcmp(dst, src, 4) != 0 || dst[4] != 0xee) return 4;
    if (run(2, src, 0) != 0 || dst[0] != 0xee) return 5;
    if (run(3, src, -4) != 0 || dst[0] != 0xee) return 6;
    if (run(4, src, 2) != -1 || dst[0] != 0xee) return 7;
    if (run(1, src, 2) != -1 || run(0, src, 2) != -1 || run(7, src, 2) != -1) return 8;
    return 0;
}
