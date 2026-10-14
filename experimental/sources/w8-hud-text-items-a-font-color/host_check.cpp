// Host contract for fontColor__FUiii. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstring>
static unsigned char arena[1024];
unsigned char* norrath_host_base = arena;
#include "font_color.cpp"
extern "C" {
unsigned p[4];
unsigned reglistLoopCount;
int nmChars, nmDirectQWords, lastFontColor, lastFontCharAlpha, fontCharAlpha;
}
static unsigned long long q(unsigned off) { unsigned long long v; std::memcpy(&v, arena + off, 8); return v; }
static unsigned w(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }

static int run(int keep) {
    std::memset(arena, 0xAA, sizeof arena);
    unsigned short open = 0xfff0; std::memcpy(arena + 0x20, &open, 2);
    reglistLoopCount = 0x20; p[0] = 0x100; p[1] = 0x1111; p[2] = 0x2222; p[3] = 0x3333;
    nmChars = 5; nmDirectQWords = 7;
    lastFontColor = 1; lastFontCharAlpha = 2; fontCharAlpha = 3;
    fontColor__FUiii(0x80402010u, 77, keep);
    unsigned short now; std::memcpy(&now, arena + 0x20, 2);
    if (now != static_cast<unsigned short>(0xfff0 + 10)) return 1;
    if (nmChars != 0 || nmDirectQWords != 7 + 10 + 3) return 2;
    if (q(0x100) != 0x1000000000000001ull || q(0x108) != 14) return 3;
    if (w(0x110) != 0x80402010u || w(0x114) != 0 || q(0x118) != 1) return 4;
    if (q(0x120) != 0x2400000000008000ull || q(0x128) != 83) return 5;
    if (arena[0xff] != 0xAA || arena[0x130] != 0xAA) return 6;
    if (p[0] != 0x130 || p[1] != 0x1111 || p[2] != 0x100 || p[3] != 0x3333) return 7;
    if (reglistLoopCount != 0x120) return 8;
    if (keep == 0) {
        if (lastFontColor != static_cast<int>(0x80402010u) || lastFontCharAlpha != 77 || fontCharAlpha != 77) return 9;
    } else if (lastFontColor != 1 || lastFontCharAlpha != 2 || fontCharAlpha != 3) return 10;
    return 0;
}
int main() {
    if (int r = run(0)) return r;
    if (int r = run(1)) return 20 + r;
    if (int r = run(-5)) return 40 + r;
    return 0;
}
