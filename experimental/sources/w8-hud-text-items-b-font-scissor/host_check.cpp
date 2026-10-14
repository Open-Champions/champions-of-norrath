#include <cstring>

extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
FontPacketState p;
int nmChars;
int scissored;
unsigned char* reglistLoopCount;
int nmDirectQWords;
void fontScissor__Fiiii(int x0, int y0, int x1, int y1);
}

alignas(16) static unsigned char buf[512];

static unsigned long long q(const unsigned char* at) {
    unsigned long long v;
    std::memcpy(&v, at, 8);
    return v;
}

// Prepares an open run like the one fontStart leaves: a glyph tag at buf + 64
// whose loop count word is 5, the cursor at buf + 208.
static void open_run(int chars) {
    std::memset(buf, 0xee, sizeof buf);
    unsigned long long old_tag = 0x2400000000008000ULL | 5;
    std::memcpy(buf + 64, &old_tag, 8);
    reglistLoopCount = buf + 64;
    p.cursor = buf + 208;
    p.start = buf + 8;
    p.first_tag = buf + 16;
    nmChars = chars;
    nmDirectQWords = 20;
    scissored = 0;
}

static unsigned long long rect(int x0, int y0, int x1, int y1) {
    return (unsigned long long)x0 | ((unsigned long long)x1 << 16) | ((unsigned long long)y0 << 32) |
           ((unsigned long long)y1 << 48);
}

int main() {
    // Ordinary rectangle with 3 glyphs pending.
    open_run(3);
    fontScissor__Fiiii(10, 20, 300, 200);
    if (q(buf + 64) != (0x2400000000008000ULL | 11)) return 1;     // 5 + 2 * 3
    unsigned char* c = buf + 208;
    if (q(c) != 0x1000000000000001ULL || q(c + 8) != 0xe) return 2;
    if (q(c + 16) != rect(20, 20, 599, 200) || q(c + 24) != 0x40) return 3;
    if (q(c + 32) != 0x2400000000008000ULL || q(c + 40) != 0x53) return 4;
    if (p.cursor != c + 48 || p.first_tag != c || p.start != buf + 8) return 5;
    if (reglistLoopCount != c + 32) return 6;
    if (scissored != 1 || nmChars != 0 || nmDirectQWords != 20 + 6 + 3) return 7;
    if (c[48] != 0xee) return 8;

    // Clamping: x0 and y0 raised to 0, x1 lowered to 640, y1 lowered to 447.
    open_run(0);
    fontScissor__Fiiii(-5, -3, 1000, 500);
    c = buf + 208;
    if (q(c + 16) != rect(0, 0, 1279, 447)) return 9;
    if (q(buf + 64) != (0x2400000000008000ULL | 5)) return 10;
    if (nmDirectQWords != 23) return 11;

    // Bounds exactly at the limits are kept; 641 and 448 are clamped.
    open_run(1);
    fontScissor__Fiiii(0, 0, 640, 447);
    if (q(buf + 208 + 16) != rect(0, 0, 1279, 447)) return 12;
    open_run(1);
    fontScissor__Fiiii(1, 2, 641, 448);
    if (q(buf + 208 + 16) != rect(2, 2, 1279, 447)) return 13;
    open_run(1);
    fontScissor__Fiiii(639, 446, 639, 446);
    if (q(buf + 208 + 16) != rect(1278, 446, 1277, 446)) return 14;
    return 0;
}
