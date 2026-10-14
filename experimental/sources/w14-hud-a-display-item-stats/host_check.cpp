// Build as x86_64 with statics below 4 GiB (cell and base records hold 32-bit pointer words):
//   c++ -arch x86_64 -std=c++17 -Wl,-no_pie -Wl,-pagezero_size,0x1000 host_check.cpp
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" int host_sprintf(char*, const char*, ...);
#define sprintf host_sprintf
#include "display_item_stats.cpp"
#undef sprintf

signed char language;
Font* smallFont = reinterpret_cast<Font*>(0x10);
int itemInfoHeight[4];
void* tinyIcon[9];
unsigned char player[4 * 23984];

static std::vector<std::string> ev;
static float weightValue = 2.5f;
static int featValue = 0;
static int lowRange = 3, highRange = 9;
static unsigned short textPool[512][96];
static int textNext;

static std::string narrow(const unsigned short* s) {
    std::string r;
    while (*s) r += static_cast<char>(*s++);
    return r;
}

extern "C" {
int host_sprintf(char* out, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n;
    if (std::strcmp(fmt, "%.1f") == 0) {
        unsigned long long bits = va_arg(ap, unsigned long long);
        double d;
        std::memcpy(&d, &bits, 8);
        n = std::snprintf(out, 64, fmt, d);
    } else if (std::strcmp(fmt, "%d-%d") == 0) {
        int a = va_arg(ap, int);
        int b = va_arg(ap, int);
        n = std::snprintf(out, 64, fmt, a, b);
    } else {
        n = std::snprintf(out, 64, fmt, va_arg(ap, int));
    }
    va_end(ap);
    return n;
}
unsigned long long fptodp(float f) {
    double d = f;
    unsigned long long r;
    std::memcpy(&r, &d, 8);
    return r;
}
Texture* lumpFindResource__FPcT0(const char* lump, const char* name) {
    ev.push_back(std::string("lump:") + name);
    return reinterpret_cast<Texture*>(0x100);
}
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(Texture* t, int x, int y, int m, unsigned c, int, int, int, int, char, char) {
    char b[96];
    std::snprintf(b, sizeof b, "draw:%d,%d,%d,%x,%d", (int)(reinterpret_cast<unsigned long>(t) >> 8), x, y, c, m);
    ev.push_back(b);
}
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int a, unsigned c, int, int, int) {
    char b[96];
    std::snprintf(b, sizeof b, "border:%d,%d,%d,%d,%d,%x", x, y, w, h, a, c);
    ev.push_back(b);
}
void fontStart__FP4FontiUii(Font*, int s, unsigned c, int f) {
    char b[64];
    std::snprintf(b, sizeof b, "start:%d,%x,%d", s, c, f);
    ev.push_back(b);
}
void fontScissor__Fiiii(int, int, int, int) {}
void fontEnd__Fv() { ev.push_back("end"); }
void fontColor__FUiii(unsigned c, int, int) {
    char b[32];
    std::snprintf(b, sizeof b, "color:%x", c);
    ev.push_back(b);
}
void fontPrint__FiiPci(int x, int y, char* t, int) {
    char b[96];
    std::snprintf(b, sizeof b, "print:%d,%d,%s", x, y, t);
    ev.push_back(b);
}
void fontPrintCentered__FiiPci(int x, int y, char* t, int) {
    char b[96];
    std::snprintf(b, sizeof b, "pcent:%d,%d,%s", x, y, t);
    ev.push_back(b);
}
void fontPrintu__FiiPUsi(int x, int y, unsigned short* t, int) {
    ev.push_back("printu:" + std::to_string(x) + "," + std::to_string(y) + "," + narrow(t));
}
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int) {
    ev.push_back("pcentu:" + std::to_string(x) + "," + std::to_string(y) + "," + narrow(t));
}
int fontStringSize__FP4FontPci(Font*, char* t, int) { return static_cast<int>(std::strlen(t)) * 5; }
int fontStringSizeu__FP4FontPUsi(Font*, unsigned short* t, int) { return static_cast<int>(narrow(t).size()) * 6; }
unsigned short* textGetString__FPc(const char* key) {
    unsigned short* p = textPool[textNext++ % 512];
    int i = 0;
    for (; key[i]; ++i) p[i] = static_cast<unsigned short>(key[i]);
    p[i] = 0;
    return p;
}
void usprintf__FPUsT0ffffffff(unsigned short* out, unsigned short* fmt, float a, float b, float c, float, float, float, float, float) {
    std::string s = narrow(fmt);
    char n[64];
    std::snprintf(n, sizeof n, "[%g,%g,%g]", a, b, c);
    s += n;
    int i = 0;
    for (; i < static_cast<int>(s.size()); ++i) out[i] = static_cast<unsigned short>(s[i]);
    out[i] = 0;
}
void ustrcat__FPUsT0(unsigned short* a, unsigned short* b) {
    std::string s = narrow(a) + narrow(b);
    int i = 0;
    for (; i < static_cast<int>(s.size()); ++i) a[i] = static_cast<unsigned short>(s[i]);
    a[i] = 0;
}
int playerGetFeatRank__Fix(int, long long) { return featValue; }
float getWeight__4itemPQ24item11description(const void*) { return weightValue; }
int getArmorBonus__4itemPQ24item11description(const void*) { return 17; }
int getAttackBonus__4itemPQ24item11description(const void*) { return 6; }
void getDamageRangeAll__4itemPQ24item11descriptionPiT2i(const void*, int* lo, int* hi, int) {
    *lo = lowRange;
    *hi = highRange;
}
}

static unsigned char cell[96];
static unsigned char baseRec[64];
static char nameText[64];
static int bad;
#define CHECK(c) do { if (!(c)) { std::printf("fail line %d\n", __LINE__); bad++; } } while (0)

static bool has(const char* s) {
    for (auto& e : ev) if (e == s) return true;
    return false;
}
static void put16(unsigned char* p, int v) { short s = static_cast<short>(v); std::memcpy(p, &s, 2); }
static void setup(const char* name, int kind8, int slot, unsigned long long flags) {
    std::memset(cell, 0, sizeof cell);
    std::memset(baseRec, 0, sizeof baseRec);
    std::strcpy(nameText, name);
    unsigned nameWord = static_cast<unsigned>(reinterpret_cast<unsigned long>(nameText));
    std::memcpy(baseRec, &nameWord, 4);
    baseRec[28] = static_cast<unsigned char>(kind8);
    baseRec[29] = static_cast<unsigned char>(slot);
    unsigned baseWord = static_cast<unsigned>(reinterpret_cast<unsigned long>(baseRec));
    std::memcpy(cell, &baseWord, 4);
    std::memcpy(cell + 8, &flags, 8);
    for (int i = 0; i < 4; ++i) put16(cell + 36 + 8 * i, -1);
    ev.clear();
}
static void entry(int i, int kind, int v1, int v2 = 0, int v3 = 0) {
    put16(cell + 36 + 8 * i, kind);
    put16(cell + 38 + 8 * i, v1);
    put16(cell + 40 + 8 * i, v2);
    put16(cell + 42 + 8 * i, v3);
}

int main() {
    for (int i = 0; i < 9; ++i) tinyIcon[i] = reinterpret_cast<void*>(0x100 * (i + 1));
    language = 1;
    // slot 0 sword: two filled power-up slots, a class requirement, a strength requirement, a cloud kind
    std::memset(player, 0, sizeof player);
    itemInfoHeight[0] = 0;
    setup("sword", 0, 0, 0);
    entry(0, 19, 2);
    entry(1, 15, 10);
    entry(2, 33, 6, 2);
    int pw = 5;   // strength 5 < 10
    std::memcpy(player + 144, &pw, 4);
    int cls = 2;
    std::memcpy(player + 80, &cls, 4);
    displayItemStats__FiiiPQ24item11descriptioniii(0, 100, 50, cell, 0x40, 0x20, 5);
    const std::vector<std::string> expected = {
        "lump:powerupfull.tex", "draw:1,175,52,80404040,6", "lump:powerupfull.tex", "draw:1,187,52,80404040,6",
        "lump:powerupfull.tex", "draw:1,175,64,80404040,6", "lump:powerupemt.tex", "draw:1,187,64,80404040,6",
        "draw:7,112,73,80404040,6", "start:6,80202020,-1", "pcent:136,70,2.5", "end",
        "draw:3,67,50,80404040,6", "start:6,80202020,-1", "print:90,46,3-9", "end",
        "start:6,80202020,-1", "pcentu:102,90,AttackBonus[6,0,0]", "color:80202020", "color:80050a40",
        "pcentu:102,110,ReqStrength[10,0,0]", "color:80202020", "pcentu:102,130,ClericOnly[0,0,0]", "color:80202020",
        "pcentu:102,150,LargePoisonCloud[6,0,0]", "end", "border:-9,-60,223,137,6,80000000"};
    if (ev != expected) {
        for (auto& e : ev) std::printf("  %s\n", e.c_str());
        bad++;
    }
    CHECK(itemInfoHeight[0] == 6);
    // slot 1: kind 13 is not printed, armour bonus printed
    std::memset(player, 0, sizeof player);
    itemInfoHeight[1] = 0;
    setup("mail", 0, 1, 0);
    entry(0, 13, 4);
    displayItemStats__FiiiPQ24item11descriptioniii(1, 100, 50, cell, 0x40, 0x20, 2);
    CHECK(has("print:89,47,17"));
    bool sawArmor = false;
    for (auto& e : ev) if (e.find("ArmorBonusNew") != std::string::npos) sawArmor = true;
    CHECK(!sawArmor);
    CHECK(itemInfoHeight[1] == 2);   // armour and weight only
    // wide box in language 4 and border growth
    language = 4;
    itemInfoHeight[0] = 0;
    setup("potion of x", 0, 0, 0x100);
    entry(0, 5, 20);
    displayItemStats__FiiiPQ24item11descriptioniii(0, 200, 100, cell, 1, 2, 3);
    // 0x100 flag adds 21 to y, language 4 is the 299 wide box at x - 150; potion skips the power-up row
    CHECK(!has("lump:powerupfull.tex") && !has("lump:powerupemt.tex"));
    CHECK(ev.back() == "border:50,33,299,97,6,80000000");
    if (!bad) std::puts("displayItemStats passed");
    return bad;
}
