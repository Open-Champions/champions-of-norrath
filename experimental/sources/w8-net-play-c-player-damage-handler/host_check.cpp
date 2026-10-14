#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/mman.h>
#define sprintf unit_sprintf
#define strcat unit_strcat
#include "player_damage_handler.cpp"
#undef sprintf
#undef strcat
// x86_64 build with -Wl,-pagezero_size,0x10000: the object's pointer words are
// 32 bits, so the animation table lives at 0x20000000.
alignas(8) unsigned char player[5 * 23984];
signed char cutMode;
unsigned char netForceLocalUpdate;
signed char netNetworkMode;
int frameNm;
int cheatInvulnerability;
int g_BloodLevel;
int nmPlayers;
unsigned syncRandState;

static std::string trace;
static int dlg_ret, local_ret, playing_ret;
static void note(const char* fmt, ...) {
    char b[256]; va_list ap; va_start(ap, fmt); vsnprintf(b, sizeof b, fmt, ap); va_end(ap);
    if (!trace.empty()) trace += ' ';
    trace += b;
}
extern "C" int unit_sprintf(char* d, const char* f, ...) { va_list ap; va_start(ap, f); int n = vsnprintf(d, 64, f, ap); va_end(ap); return n; }
extern "C" char* unit_strcat(char* d, const char* s) { return strcat(d, s); }
extern "C" int DLG_Active__Fv() { note("dlg"); return dlg_ret; }
extern "C" int netPlayerIsLocal__Fi(int i) { note("local(%d)", i); return local_ret; }
extern "C" void playerPlaySound__Fii(int i, int s) { note("sound(%d,%d)", i, s); }
extern "C" int playerIsAnimPlaying__6PlayerPc(void*, char* n) { note("playing(%s)", n); return playing_ret; }
extern "C" void* lumpFindResource__FPcT0(char* a, char* b) { note("lump(%s,%s)", a, b); return (void*)0x4321; }
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* st, void* h, float a, float b, int f) {
    unsigned x, y; memcpy(&x, &a, 4); memcpy(&y, &b, 4);
    note("oneshot(+%ld,%lx,%08x,%08x,%d)", (long)((unsigned char*)st - (unsigned char*)0), (unsigned long)h, x, y, f);
}
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float a, int f) {
    note("trans(%lx,%g,%d)", (unsigned long)h, a, f);
}
extern "C" void addEffect__FiG6Point3(int k, const Point3* p) { note("effect(%d,%g,%g,%g)", k, p->x, p->y, p->z); }
extern "C" void* blockAlloc__Fi(int n) { note("alloc(%d)", n); return (void*)0x5555; }
extern "C" void __8GutChunkG6Point3iif(void*, const Point3* p, int a, int b, float c) { note("gut(%g,%g,%g,%d,%d,%g)", p->x, p->y, p->z, a, b, c); }
extern "C" void disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi(
    void*, void* m, void*, const Point3* p, short s, float a, float b, int c) {
    note("disint(%lx,%g,%g,%g,%d,%g,%g,%d)", (unsigned long)m, p->x, p->y, p->z, s, a, b, c);
}
extern "C" void enumerateAlivePlayers__Fv() { note("enum"); }
extern "C" void gameNetKick__Fi(int v) { note("kick(%d)", v); }

static unsigned char* table;  // animation table at a low address
alignas(8) static unsigned char objbuf[2048];
static unsigned char* self_ = objbuf;
static void set32(unsigned char* b, int o, unsigned v) { memcpy(b + o, &v, 4); }
static void setf(unsigned char* b, int o, float v) { memcpy(b + o, &v, 4); }
static unsigned get32(unsigned char* b, int o) { unsigned v; memcpy(&v, b + o, 4); return v; }
static float getf(unsigned char* b, int o) { float v; memcpy(&v, b + o, 4); return v; }
static void reset() {
    memset(player, 0, sizeof player); memset(objbuf, 0, sizeof objbuf);
    cutMode = 0; netForceLocalUpdate = 0; netNetworkMode = 0; frameNm = 777; cheatInvulnerability = 0;
    g_BloodLevel = 0; nmPlayers = 1; syncRandState = 1; dlg_ret = 0; local_ret = 0; playing_ret = 0; trace.clear();
    memset(table, 0, 4096);
    set32(table, 0, 0x20000800);  // first word: pointer to the lump name text
    strcpy((char*)table + 0x800, "LUMP");
    set32(table, 12, 0xaaa0); set32(table, 20, 0xbbb0);
    for (int k = 0; k < 4; ++k) set32(table, 124 + 104 * k, 0x7000 + k);
    set32(self_, 36, 0x20000000);
    set32(self_, 1332, 0x9999);
}
static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }
static int fails;
#define EXPECT(cond, tag) do { if (!(cond)) { printf("FAIL %s (trace: %s)\n", tag, trace.c_str()); ++fails; } } while (0)
#define EXPECT_TRACE(s, tag) do { if (trace != (s)) { printf("FAIL %s\n got: %s\nwant: %s\n", tag, trace.c_str(), (s)); ++fails; } } while (0)

int main() {
    table = (unsigned char*)mmap((void*)0x20000000, 8192, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
    if ((unsigned long)table != 0x20000000) return 100;
    const unsigned BLEND = 0x3dcccccc;
    char want[512];

    // A: cut mode clears flags and damage and does nothing else
    reset(); cutMode = 1; set32(self_, 744, 0x1); setf(self_, 192, 9.0f);
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("", "A"); EXPECT(get32(self_, 744) == 0 && getf(self_, 192) == 0.0f, "A-state");
    // B: dialog active
    reset(); dlg_ret = 1; set32(self_, 744, 0x1); setf(self_, 192, 9.0f);
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg", "B"); EXPECT(get32(self_, 744) == 0 && getf(self_, 192) == 0.0f, "B-state");
    // B2: bit 0x20000000 bypasses cut mode and dialog checks; nothing happens with no damage
    reset(); cutMode = 1; set32(self_, 744, 0x20000000);
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("", "B2"); EXPECT(get32(self_, 744) == 0, "B2-state");

    // C: tier 1 (0x40000), local player, kind 2: one-shot with the table word for kind 2
    reset(); local_ret = 1; set32(self_, 744, 0x40000); player[23582] = 2; set32(self_, 1400, 5);
    playerDamageHandler__6Player(self_);
    snprintf(want, sizeof want, "dlg local(0) sound(0,12) oneshot(+%ld,7002,%08x,%08x,10)", (long)(self_ + 288 - (unsigned char*)0), BLEND, BLEND);
    EXPECT_TRACE(want, "C");
    EXPECT(getf(self_, 756) == 100.0f && get32(self_, 1400) == 3 && get32(self_, 1088) == 777 && netForceLocalUpdate == 1, "C-state");
    // tier floats and tiers 2,3,3
    float tf[4] = {100, 150, 200, 250}; unsigned bits[4] = {0x40000, 0x80000, 0x100000, 0x200000};
    for (int i = 0; i < 4; ++i) {
        reset(); set32(self_, 744, bits[i] | 0x1000000); playing_ret = 1;
        playerDamageHandler__6Player(self_);
        EXPECT(getf(self_, 756) == tf[i], "tier-float");
    }
    // lowest bit wins when several are set
    reset(); set32(self_, 744, 0x80000 | 0x200000);
    playerDamageHandler__6Player(self_);
    EXPECT(getf(self_, 756) == 150.0f, "tier-order");

    // D: knockdown names by turn, kind 3 selects knockdown2_, other kinds knockdown1_
    struct { int turn; const char* suffix; } cases[] = {
        {30000, "a.anm"}, {-30000, "a.anm"}, {24576, "a.anm"}, {24575, "d.anm"}, {8193, "d.anm"}, {8192, "c.anm"},
        {0, "c.anm"}, {-8191, "c.anm"}, {-8192, "b.anm"}, {-24575, "b.anm"}, {-24576, "a.anm"}, {-20000, "b.anm"}, {20000, "d.anm"}};
    for (auto& c : cases) {
        for (int kind = 0; kind < 5; ++kind) {
            reset(); set32(self_, 744, 0x80000); player[23582] = (unsigned char)kind;
            unsigned short a = 100, b = (unsigned short)(100 + c.turn);
            memcpy(self_ + 56, &a, 2); memcpy(self_ + 748, &b, 2);
            playing_ret = 1;
            playerDamageHandler__6Player(self_);
            snprintf(want, sizeof want, "dlg local(0) sound(0,12) playing(%s%s)", (kind == 2 || kind == 3) ? "knockdown2_" : "knockdown1_", c.suffix);
            EXPECT_TRACE(want, "D");
        }
    }
    // D2: not playing -> lump lookup, slot limit when index >= 5, one-shot flag 2058
    reset(); set32(self_, 744, 0x100000); self_[292] = 6; setf(self_, 280 + 6 * 32, 5.0f); setf(self_, 284 + 6 * 32, 2.0f);
    playerDamageHandler__6Player(self_);
    snprintf(want, sizeof want, "dlg local(0) sound(0,12) playing(knockdown1_c.anm) lump(LUMP,knockdown1_c.anm) oneshot(+%ld,4321,%08x,%08x,2058)", (long)(self_ + 288 - (unsigned char*)0), BLEND, BLEND);
    EXPECT_TRACE(want, "D2");
    EXPECT(getf(self_, 284 + 6 * 32) == __builtin_bit_cast(float, 0xbdaaaaaau), "D2-slot");
    EXPECT(getf(self_, 280 + 6 * 32) == 1.0f, "D2-limit");
    // slot below 5: untouched
    reset(); set32(self_, 744, 0x100000); self_[292] = 4; setf(self_, 280 + 4 * 32, 5.0f);
    playerDamageHandler__6Player(self_);
    EXPECT(getf(self_, 280 + 4 * 32) == 5.0f && getf(self_, 284 + 4 * 32) == 0.0f, "D2-low");
    // state 8: in-range turn keeps the tier, out-of-range turn drops to tier 1 (one-shot, flag 10)
    reset(); set32(self_, 744, 0x100000); set32(self_, 1400, 8); { unsigned short a = 0, b = 5000; memcpy(self_ + 56, &a, 2); memcpy(self_ + 748, &b, 2); }
    playing_ret = 1; playerDamageHandler__6Player(self_);
    EXPECT(trace.find("playing(") != std::string::npos, "state8-in");
    reset(); set32(self_, 744, 0x100000); set32(self_, 1400, 8); { unsigned short a = 0, b = 30000; memcpy(self_ + 56, &a, 2); memcpy(self_ + 748, &b, 2); }
    playing_ret = 1; playerDamageHandler__6Player(self_);
    EXPECT(trace.find("playing(") == std::string::npos && trace.find(",10)") != std::string::npos, "state8-out");
    // the 16-bit difference wraps: 0x0010 - 0xfff0 = 0x0020
    reset(); set32(self_, 744, 0x100000); set32(self_, 1400, 8); { unsigned short a = 0xfff0, b = 0x0010; memcpy(self_ + 56, &a, 2); memcpy(self_ + 748, &b, 2); }
    playing_ret = 1; playerDamageHandler__6Player(self_);
    EXPECT(trace.find("playing(knockdown1_c.anm)") != std::string::npos, "wrap");

    // E: plain damage, alive; chance 0 -> nothing further, generator advances once
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f); syncRandState = 99;
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg", "E"); EXPECT(getf(player, 172) == 15.0f && getf(self_, 192) == 0.0f && get32(self_, 744) == 0, "E-state");
    EXPECT(syncRandState == lcg(99), "E-rand"); EXPECT(get32(self_, 12) == 0x100, "E-flags");
    // E2: chance 0.04 (rec[224] = 0): hit when high half of the next value is below 2621
    unsigned lowSeed = 0, highSeed = 0;
    for (unsigned s = 1; !lowSeed || !highSeed; ++s) { unsigned v = lcg(s) >> 16; if (!lowSeed && v < 2000) lowSeed = s; if (!highSeed && v > 4000) highSeed = s; }
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); syncRandState = lowSeed; player[23582] = 1; local_ret = 1;
    playerDamageHandler__6Player(self_);
    snprintf(want, sizeof want, "dlg local(0) sound(0,12) oneshot(+%ld,7001,%08x,%08x,10)", (long)(self_ + 288 - (unsigned char*)0), BLEND, BLEND);
    EXPECT_TRACE(want, "E2"); EXPECT(get32(self_, 1400) == 3 && get32(self_, 1088) == 777 && netForceLocalUpdate == 1, "E2-state");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); syncRandState = highSeed;
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg", "E3");
    // flag 0x4000000 forces the reaction even for a high value; flag 0x400000 / states 3,7 / 1092==2 suppress it
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); syncRandState = highSeed; set32(self_, 744, 0x4000000);
    playerDamageHandler__6Player(self_); EXPECT(get32(self_, 1400) == 3, "E4");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); syncRandState = lowSeed; set32(self_, 744, 0x400000);
    playerDamageHandler__6Player(self_); EXPECT(get32(self_, 1400) == 0 && trace == "dlg" && get32(self_, 744) == 0, "E5");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); syncRandState = lowSeed; set32(self_, 1400, 7);
    playerDamageHandler__6Player(self_); EXPECT(get32(self_, 1400) == 7 && trace == "dlg", "E6");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); syncRandState = lowSeed; set32(self_, 1092, 2);
    playerDamageHandler__6Player(self_); EXPECT(get32(self_, 1400) == 0 && trace == "dlg", "E7");
    // state 4 returns before any damage is applied; non-positive damage too
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 25.0f); set32(self_, 1400, 4);
    playerDamageHandler__6Player(self_); EXPECT(getf(player, 172) == 25.0f && get32(self_, 744) == 0 && getf(self_, 192) == 10.0f, "E8");
    reset(); setf(self_, 192, 0.0f); setf(player, 172, 25.0f); playerDamageHandler__6Player(self_); EXPECT(getf(player, 172) == 25.0f, "E9");

    // F: cheat invulnerability zeroes the damage unless the 0x20000000 bit is set
    reset(); cheatInvulnerability = 1; setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f);
    playerDamageHandler__6Player(self_); EXPECT(getf(player, 172) == 25.0f, "F");
    reset(); cheatInvulnerability = 1; setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f); set32(self_, 744, 0x20000000);
    playerDamageHandler__6Player(self_); EXPECT(getf(player, 172) == 15.0f, "F2");

    // G: blood effect and gut chunk
    unsigned gutSeed = 0, noGutSeed = 0;
    for (unsigned s = 1; !gutSeed || !noGutSeed; ++s) { unsigned v = (lcg(s) >> 16) & 7; if (!gutSeed && v == 0) gutSeed = s; if (!noGutSeed && v != 0) noGutSeed = s; }
    reset(); g_BloodLevel = 2; setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f);
    set32(self_, 744, 0x4); setf(self_, 16, 1.0f); setf(self_, 20, 2.0f); setf(self_, 24, 3.0f); syncRandState = gutSeed;
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg effect(0,1,2,39) alloc(616) gut(1,2,39,2,1,1)", "G");
    EXPECT(syncRandState == lcg(lcg(gutSeed)), "G-rand");  // blood step, then the hit-chance step
    reset(); g_BloodLevel = 2; setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f);
    set32(self_, 744, 0x1); syncRandState = noGutSeed; playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg effect(0,0,0,36)", "G2");
    reset(); netNetworkMode = 1; setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f);
    set32(self_, 744, 0x8); syncRandState = gutSeed; playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg alloc(616) gut(0,0,36,2,1,1)", "G3");
    reset(); g_BloodLevel = 2; setf(self_, 192, 10.0f); setf(player, 172, 25.0f); setf(player, 224, 1.0f);
    set32(self_, 744, 0x2); syncRandState = gutSeed; playerDamageHandler__6Player(self_);  // bit 0x2 is not in 0xd
    EXPECT_TRACE("dlg", "G4"); EXPECT(syncRandState == lcg(gutSeed), "G4-rand");  // only the hit-chance step ran

    // H: death
    reset(); local_ret = 1; nmPlayers = 2; setf(self_, 192, 10.0f); setf(player, 172, 5.0f); set32(self_, 12, 0x30);
    setf(player, 172 + 23984, 8.0f); player[23582] = 2; set32(player, 84, 0); set32(self_, 744, 0x0);
    playerDamageHandler__6Player(self_);
    EXPECT_TRACE("dlg trans(bbb0,0.5,518) enum local(0) kick(1) sound(1,3)", "H");
    EXPECT(getf(player, 172) == 0.0f && get32(self_, 12) == (0x30u | 0x108u | 1u) && get32(self_, 1400) == 4 && get32(self_, 1088) == 777 &&
           get32(self_, 744) == 0 && netForceLocalUpdate == 1 && getf(self_, 192) == 0.0f, "H-state");
    // class 3 / kind != 2 uses the other table word; class 9 uses the special list
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 5.0f); set32(player, 84, 3); player[23582] = 2;
    playerDamageHandler__6Player(self_); EXPECT(trace.find("trans(aaa0") != std::string::npos, "H2");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 5.0f); set32(player, 84, 9); player[23582] = 2;
    playerDamageHandler__6Player(self_); EXPECT(trace.find("trans(bbb0") != std::string::npos, "H3");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 5.0f); set32(player, 84, 9); player[23582] = 1;
    playerDamageHandler__6Player(self_); EXPECT(trace.find("trans(aaa0") != std::string::npos, "H4");
    int specials[] = {0, 1, 4, 5, 8, 9}; 
    for (int cls = 0; cls < 12; ++cls) {
        bool sp = false; for (int s : specials) sp |= (s == cls);
        reset(); setf(self_, 192, 10.0f); setf(player, 172, 5.0f); set32(player, 84, cls); player[23582] = 2;
        playerDamageHandler__6Player(self_);
        EXPECT(trace.find(sp ? "trans(bbb0" : "trans(aaa0") != std::string::npos, "H-class");
    }
    // single player: no sound for the other player; other dead: no sound; other index flips for player 1
    reset(); nmPlayers = 2; setf(self_, 192, 10.0f); setf(player, 172, 5.0f); setf(player, 172 + 23984, 0.0f);
    playerDamageHandler__6Player(self_); EXPECT(trace.find("sound") == std::string::npos, "H5");
    reset(); nmPlayers = 2; set32(self_, 960, 1); setf(self_, 192, 10.0f); setf(player, 172 + 23984, 5.0f); setf(player, 172, 3.0f);
    playerDamageHandler__6Player(self_); EXPECT(trace.find("sound(0,3)") != std::string::npos, "H6");
    // disintegration when flag 0x10 and health <= 0
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 5.0f); set32(self_, 744, 0x10); setf(self_, 16, 4.0f); setf(self_, 20, 5.0f); setf(self_, 24, 6.0f);
    { short s = 123; memcpy(self_ + 56, &s, 2); }
    playerDamageHandler__6Player(self_);
    EXPECT(trace.rfind("dlg disint(9999,4,5,6,123,1,0,0)", 0) == 0, "I");
    reset(); setf(self_, 192, 10.0f); setf(player, 172, 50.0f); setf(player, 224, 1.0f); set32(self_, 744, 0x10);
    playerDamageHandler__6Player(self_); EXPECT(trace.find("disint") == std::string::npos, "I2");
    printf(fails ? "FAILED\n" : "");
    return fails ? 1 : 0;
}
