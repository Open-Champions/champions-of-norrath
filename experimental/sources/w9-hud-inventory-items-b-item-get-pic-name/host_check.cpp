// Host check: uses the host libc for sprintf, sscanf, strcpy, strstr and strlen.
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstring>

struct PicDescription {
    const char* name;
    unsigned char gap0[14 - sizeof(void*)];
    short half14, half16;
    unsigned char gap1[28 - 18];
    signed char byte28, byte29, byte30;
    unsigned char gap2[48 - 31];
    const char* baseName;
    short globalIndex;
};
struct PicItem {
    const PicDescription* description;
    unsigned char gap[8 - sizeof(void*)];
    long long flags;
    unsigned char gap2[32 - 16];
    unsigned char variant;
};

static int gv;
extern "C" {
unsigned char _4item$prefixTable[40 * 256];
unsigned char _4item$globalItem[16 * 8];
void getPicName__4itemPciPQ24item11description(char*, int, const PicItem*);
int getValue__4itemPQ24item11description(const PicItem*) { return gv; }
}

static PicDescription d;
static PicItem item;
static char out[256];
static const char* run(const char* name, int b28, int b29, int b30, const char* base, int gi, long long flags, int variant, int n, const char* init = "") {
    d = PicDescription{}; d.name = name; d.byte28 = b28; d.byte29 = b29; d.byte30 = b30; d.baseName = base; d.globalIndex = gi;
    d.half14 = 7; d.half16 = 16;
    item = PicItem{}; item.description = &d; item.flags = flags; item.variant = variant;
    std::memset(out, 0, sizeof out); std::strcpy(out, init);
    getPicName__4itemPciPQ24item11description(out, n, &item);
    return out;
}
#define EXPECT(call, want, code) do { const char* r = call; if (std::strcmp(r, want)) { std::printf("fail %d: got '%s' want '%s'\n", code, r, want); return code; } } while (0)

int main() {
    // prefix rows: row r has bytes +6 = r+1, +7 = r+2, +8 = r+3
    for (int r = 0; r < 255; ++r) { _4item$prefixTable[r * 40 + 6] = r + 1; _4item$prefixTable[r * 40 + 7] = r + 2; _4item$prefixTable[r * 40 + 8] = r + 3; }
    _4item$prefixTable[9 * 40 + 6] = 0;
    // globalItem name pointers at +4 of 16-byte rows (host pointers are 8 bytes wide)
    static const char* gnames[] = {"g0pic.tex", "g1pic.tex", "g2pic.tex"};
    for (int i = 0; i < 3; ++i) std::memcpy(_4item$globalItem + 16 * i + 4, &gnames[i], sizeof(char*));
    const long long frac = 5LL << 11;    // ((flags >> 11) + 1) & 0x1f == 6

    // A (byte 29 zero, bit 34 clear): "%d%d.tex" replaces the last character and the extension
    EXPECT(run("sword", 0, 0, 0, "swordpic", 0, frac, 3, 0), "swordpi64.tex", 1);       // digit = row 3 byte +6 = 4
    EXPECT(run("sword", 0, 0, 0, nullptr, 1, frac, 255, 0), "g1pi61.tex", 2);           // no base name: globalItem name; variant 255 -> 1
    EXPECT(run("sword", 0, 0, 4, "aaa", 0, frac, 3, 0), "aa61.tex", 3);                 // description byte 30 == 4 forces 1
    EXPECT(run("sword", 0, 0, 0, "aaa", 0, frac | 0x800000, 3, 0), "aa66.tex", 4);      // flag bit 23: row byte +8
    EXPECT(run("sword", 0, 0, 0, "aaa", 0, frac | 0x800000, 255, 0), "aa61.tex", 5);    // ... not for variant 255
    EXPECT(run("sword+2", 0, 0, 0, "aaa", 0, frac, 3, 0), "aa65.tex", 6);               // (7 + 16) % 18
    EXPECT(run("x", 0, 0, 0, "aaa", 0, frac, 100, 0), "aa6101.tex", 7);                 // digit is printed with %d
    EXPECT(run("x", 0, 0, 0, "aaa", 0, frac, 9, 0), "aa61.tex", 8);                     // row 9 byte +6 is zero -> 1
    EXPECT(run("x", 0, 0, 0, "aaa", 0, frac | (31LL << 11), 3, 0), "aa04.tex", 9);      // ((flags >> 11) + 1) & 0x1f wraps to 0
    // B (byte 29 is 1)
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0, 3, 2), "wem_arm5.tex", 10);               // row 3 byte +7 = 5
    EXPECT(run("tunic", 3, 1, 0, "arm", 0, 0, 3, 2), "arm.tex", 11);                    // byte 28 == 3: nothing more
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0, 3, 6), "bm_arm5.tex", 12);                // n = 6 -> 0
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0, 3, 7), "bf_arm5.tex", 13);                // n = 7 -> 1
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0, 3, 8), "dem_arm5.tex", 14);               // other n unchanged
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0x80000, 3, 3), "ef_arm5.tex", 15);          // bit 19: odd -> 7
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0x80000, 3, 4), "em_arm5.tex", 16);          // bit 19: even -> 6
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0x80000, 3, 6), "em_arm5.tex", 17);
    EXPECT(run("tunic", 0, 1, 0, "arm", 0, 0, 255, 2), "wem_arm1.tex", 18);             // variant 255 -> 1
    gv = 10; EXPECT(run("tunic+1", 0, 1, 0, "arm", 0, 0, 3, 2), "wem_arm2.tex", 19);    // getValue % 4
    gv = -5; EXPECT(run("tunic+1", 0, 1, 0, "arm", 0, 0, 3, 2), "wem_arm-1.tex", 20);   // signed remainder
    gv = 10; EXPECT(run("tunic+1", 0, 1, 0, "arm", 0, 0, 255, 2), "wem_arm1.tex", 21);  // variant 255 forces 1
    // circlet names
    EXPECT(run("circlet12", 0, 1, 0, "arm", 0, 0, 255, 6), "em_cloth1helmet621.tex", 22);
    EXPECT(run("circlet27", 0, 1, 0, "arm", 0, 0, 255, 7), "ef_cloth2helmet671.tex", 23);
    EXPECT(run("circlet35", 0, 1, 0, "arm", 0, 0, 255, 6), "em_cloth3helmet651.tex", 24);
    EXPECT(run("circlet12", 0, 1, 0, "arm", 0, 0, 255, 2), "wem_arm1.tex", 25);         // n not 6 or 7: base name kept
    EXPECT(run("circlet12", 0, 1, 0, "arm", 0, 0x80000, 255, 3), "wef_arm1.tex", 26);   // circlet: n is not adjusted
    // C (anything else): a trailing "1.tex" is rewritten
    EXPECT(run("x", 0, 2, 0, "pic1", 0, frac, 3, 0), "pic6.tex", 27);
    EXPECT(run("x", 0, 2, 0, "pic2", 0, frac, 3, 0), "pic2.tex", 28);
    EXPECT(run("x", 0, 0, 0, "pic1", 0, frac | (1LL << 34), 3, 0), "pic6.tex", 29);     // bit 34 set sends byte 29 == 0 here too
    EXPECT(run("x", 0, -1, 0, "pic1", 0, 0, 3, 0), "pic1.tex", 30);                    // ((0 >> 11) + 1) & 0x1f = 1: unchanged text
    return 0;
}
