#include "font_printu_inner.cpp"

FontPacketState p;
unsigned char* glistp;
unsigned char* gstart;
int GLISTSIZE;
int engineFullFrameMode;
int nmChars;
int nmDirectQWords;
int scanMask;
unsigned char* reglistLoopCount;

static int prints;
static int px, py, pcount;
static const unsigned short* ptext;
static int masks_seen[4];
static unsigned long long block_at_print[4];
static int dwords_at_print[4];
extern "C" void subfontPrintu__FiiPUsi(int x, int y, const unsigned short* text, int count) {
    px = x; py = y; ptext = text; pcount = count;
    masks_seen[prints] = scanMask;
    dwords_at_print[prints] = nmDirectQWords;
    ++prints;
}

static unsigned long long get64(const unsigned char* where) {
    return *reinterpret_cast<const unsigned long long*>(where);
}

int main() {
    alignas(16) static unsigned char list[4096];
    alignas(16) static unsigned char block[256];
    static unsigned short loops[2] = {7, 0};
    static const unsigned short text[3] = {72, 105, 0};
    gstart = list;
    // Too little room: exactly the threshold is enough, one unit less is not.
    glistp = list + 8 * 100;
    GLISTSIZE = 6099;
    engineFullFrameMode = 0;
    _fontPrintu__FiiPUsi(1, 2, text, 3);
    if (prints != 0) return 1;
    GLISTSIZE = 6100;
    _fontPrintu__FiiPUsi(-4, 300, text, 2);
    if (prints != 1 || px != -4 || py != 300 || ptext != text || pcount != 2) return 2;
    if (p.cursor || scanMask || nmDirectQWords) return 3;   // untouched in the half-frame mode
    // Full-frame mode: two prints bracketed by scan-mask blocks, then a restore block.
    prints = 0;
    engineFullFrameMode = 1;
    p.cursor = block;
    reglistLoopCount = reinterpret_cast<unsigned char*>(&loops[0]);
    nmChars = 5;
    nmDirectQWords = 20;
    _fontPrintu__FiiPUsi(10, 20, text, 3);
    if (prints != 2) return 4;
    // The first print sees block one: mask 2, loop count 7 + 10, nmDirectQWords 20 + 10 + 3.
    if (masks_seen[0] != 2 || dwords_at_print[0] != 33) return 5;
    if (loops[0] != 17) return 6;
    if (masks_seen[1] != 3 || dwords_at_print[1] != 36) return 7;
    if (nmChars != 0 || nmDirectQWords != 39 || scanMask != 0) return 8;
    if (p.cursor != block + 144 || p.first_tag != block + 96) return 9;
    if (reglistLoopCount != block + 128) return 10;
    for (int i = 0; i < 3; ++i) {
        const unsigned char* b = block + 48 * i;
        if (get64(b) != 0x1000000000000001ULL || get64(b + 8) != 0xe) return 11;
        if (get64(b + 24) != 0x22 || get64(b + 32) != 0x2400000000008000ULL || get64(b + 40) != 0x53) return 12;
    }
    if (get64(block + 16) != 2 || get64(block + 64) != 3 || get64(block + 112) != 0) return 13;
    if (px != 10 || py != 20 || ptext != text || pcount != 3) return 14;
    return 0;
}
