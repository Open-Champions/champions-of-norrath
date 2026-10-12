#include <cstring>
#include <initializer_list>

extern "C" {
struct FontPacketState {
    unsigned char* cursor;
    unsigned char* start;
    unsigned char* first_tag;
};
FontPacketState p;
unsigned char* glistp;
void* curFont;
int drawBucket;
int restrictChars;
int fontCharAlpha;
int nmChars;
unsigned lastFontColor;
int scissored;
unsigned char* vifDirectQWordCount;
unsigned char* reglistLoopCount;
int nmDirectQWords;
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags);
}

alignas(16) static unsigned char buf[1024];

static unsigned long long q(const unsigned char* at) {
    unsigned long long v;
    std::memcpy(&v, at, 8);
    return v;
}
static unsigned w(const unsigned char* at) {
    unsigned v;
    std::memcpy(&v, at, 4);
    return v;
}

static int run(int offset) {
    std::memset(buf, 0xff, sizeof buf);
    glistp = buf + offset;
    p = FontPacketState{};
    nmChars = 99;
    scissored = 99;
    int marker;
    fontStart__FP4FontiUii(&marker, 7, 0x80402010u, 3);
    if (curFont != &marker || drawBucket != 7 || fontCharAlpha != 3) return 1;
    if (lastFontColor != 0x80402010u || restrictChars != 100000 || nmChars != 0 || scissored != 0) return 2;
    if (p.start != buf + offset) return 3;
    if (w(buf + offset + 8) != 0x11000000u) return 4;
    // Zero padding from S+12 up to the DIRECT word, which sits at 12 mod 16.
    unsigned char* direct = buf + offset + 12;
    while ((reinterpret_cast<unsigned long>(direct) & 15) != 12) {
        if (*direct != 0) return 5;
        ++direct;
    }
    if (vifDirectQWordCount != direct || w(direct) != 0x50000000u) return 6;
    unsigned char* tag = direct + 4;
    if ((reinterpret_cast<unsigned long>(tag) & 15) != 0) return 7;
    if (p.first_tag != tag) return 8;
    if (q(tag) != 0x1000000000000007ULL || q(tag + 8) != 0xe) return 9;
    static const unsigned long long pairs[7][2] = {
        {0, 0x3f}, {0, 6}, {1, 0x14}, {0x80402010ULL, 1}, {0x44, 0x42}, {0x156, 0}, {0x3001d, 0x47}};
    for (int i = 0; i < 7; ++i) {
        if (q(tag + 16 + 16 * i) != pairs[i][0] || q(tag + 24 + 16 * i) != pairs[i][1]) return 10 + i;
    }
    if (reglistLoopCount != tag + 128) return 20;
    if (q(tag + 128) != 0x2400000000008000ULL || q(tag + 136) != 0x53) return 21;
    if (p.cursor != tag + 144) return 22;
    if (nmDirectQWords != 9) return 23;
    // Nothing beyond the second tag is written.
    for (int i = 144; i < 160; ++i) if (tag[i] != 0xff) return 24;
    // The eight bytes at S+0 are left for the DMA tag.
    for (int i = 0; i < 8; ++i) if (buf[offset + i] != 0xff) return 25;
    return 0;
}

int main() {
    for (int offset : {0, 4, 8, 12, 16, 64}) {
        int r = run(offset);
        if (r) return r + 100 * (offset / 4 + 1);
    }
    return 0;
}
