// Host contract for setState__11SpiderQueeni; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900
// run (states -7..22 with random object state, 370 of 379 original words reached, the rest padding).
#define W10_ACTOR_N_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "set_state.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int procResult, procNew = -1000, atanResult;
static unsigned rollValue;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rollValue; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float t, int f) { calls += "trans(" + num(off(h) - 0x2000) + "," + num(t) + "," + num(f) + ");"; }
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(h) - 0x2000) + "," + num(a) + "," + num(b) + "," + num(f) + ");";
}
void animSetToEnd__FP14AnimationState(void* s) { calls += std::string("toend") + (s == obj + 288 ? "" : "?") + ";"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + name + ");";
}
float icos__Fi(int a) { calls += "cos" + num(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + num(a) + ";"; return 0.25f; }
int iatan2__Fff(float y, float x) { calls += "atan(" + num(y) + "," + num(x) + ");"; return atanResult; }
void reset__3Web(void* w) { calls += "web" + num(off(w) - 0x1000) + ";"; }
}
unsigned w10_actor_n_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static float getf(unsigned o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void put16(unsigned o, int v) { short s = static_cast<short>(v); std::memcpy(obj + o, &s, 2); }
static void reset(short current) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; rollValue = 1000; atanResult = 0;
    put32(36, 0x300); put32(72, 0x800); putA(0x800 + 192, 8); putA(0x800 + 196, 0x78);
    for (unsigned i = 0; i < 200; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    put32(808, 5000);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    put16(56, 9);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    auto p = [](int n) { return "proc" + num(n) + ";"; };
    // every state sets the float at +176 to 120 except when the first step ends the call
    reset(0); setState__11SpiderQueeni(obj, 2); bad |= check("2", p(2)); if (getf(176) != 120.0f || get16(572) != 2) { std::puts("fail 2 stores"); bad = 1; }
    reset(0); procResult = 1; setState__11SpiderQueeni(obj, 2); if (getf(176) != 0.0f || get16(572) != 0) { std::puts("fail proc stores"); bad = 1; }
    reset(0); setState__11SpiderQueeni(obj, 0); bad |= check("0", p(0) + "rand;trans(92,0.3,3);");
    if (get32(256) != 5000u + 1000u % 300u + 120u) { std::puts("fail 0 word"); bad = 1; }
    reset(0); rollValue = 0xfffffffeu; setState__11SpiderQueeni(obj, 0); if (get32(256) != 5000u + 0xfffffffeu % 300u + 120u) { std::puts("fail 0 unsigned"); bad = 1; }
    reset(0); setState__11SpiderQueeni(obj, 1); bad |= check("1", p(1) + "reset;smart200;trans(148,0.3,3);");
    reset(0); setState__11SpiderQueeni(obj, 3); bad |= check("3", p(3) + "reset;trans(148,0.3,3);"); if (getf(176) != 120.0f) { std::puts("fail 3 float"); bad = 1; }
    reset(0); setState__11SpiderQueeni(obj, 5); bad |= check("5", p(5) + "trans(92,0.1,3);");
    reset(0); setState__11SpiderQueeni(obj, 6); bad |= check("6", p(6) + "shot(36,0.1,0.1,8);"); if (get32(964) != 6) { std::puts("fail 6 word"); bad = 1; }
    reset(0); setState__11SpiderQueeni(obj, 9); bad |= check("9", p(9) + "trans(92,0.3,3);");
    reset(0); setState__11SpiderQueeni(obj, 14); bad |= check("14", p(14) + "trans(164,0.1,3);");
    reset(0); setState__11SpiderQueeni(obj, 19); bad |= check("19", p(19) + "trans(92,0.1,3);shot(140,0.1,0.1,8);");
    reset(0); setState__11SpiderQueeni(obj, 20); bad |= check("20", p(20) + "shot(100,0.1,0.1,8);"); if (get32(960) != 0) { std::puts("fail 20 word"); bad = 1; }
    reset(0); put32(960, 4); setState__11SpiderQueeni(obj, 15); bad |= check("15", p(15) + "shot(52,0.01,0.1,8);"); if (get32(964) != 15 || get32(960) != 0) { std::puts("fail 15 words"); bad = 1; }
    reset(0); put32(960, 4); setState__11SpiderQueeni(obj, 16); bad |= check("16", p(16) + "shot(108,0.1,0.1,8);"); if (get32(964) != 16 || get32(960) != 0) { std::puts("fail 16 words"); bad = 1; }
    reset(0); setState__11SpiderQueeni(obj, 18); bad |= check("18", p(18) + "shot(132,0.01,0.1,8);"); if (get32(964) != 18) { std::puts("fail 18 word"); bad = 1; }
    reset(0); put32(256, 77); setState__11SpiderQueeni(obj, 13); bad |= check("13", p(13) + "web968;trans(164,0.1,3);shot(172,0.1,0.1,8);");
    if (get32(256) != 0 || get32(964) != 13) { std::puts("fail 13 words"); bad = 1; }
    // 10: drop point is the position offset by 60 * (cos, sin, 0); bit 0 of the flag word clears; bit 1 ends the animation
    for (unsigned flags : {0u, 3u}) {
        reset(0); put32(12, flags | 0xf0); setState__11SpiderQueeni(obj, 10);
        bad |= check("10", p(10) + "cos9;sin9;drop(34:20:6,9,36,1,6,1,6,1,venomsac);trans(44,0.3,6);" + (flags & 2 ? "toend;" : "") + "v120(" + num(0x1000 + 8) + ",0);");
        if (getf(28) != 65.0f || get32(12) != ((flags | 0xf0) & ~1u)) { std::puts("fail 10 stores"); bad = 1; }
    }
    // 17: the sign of the 16-bit heading difference picks the one-shot
    for (int h : {0, 1, 100, 0x7fff, 0x8000, 0xffff, 0x10001}) {
        reset(0); put32(576, 0x500); putA(0x500 + 12, 0x600); float t[3] = {10, 12, 0}; std::memcpy(arena + 0x600 + 16, t, 12);
        atanResult = h; put32(960, 5); setState__11SpiderQueeni(obj, 17);
        const short diff = static_cast<short>(static_cast<unsigned short>(h) - 9);
        bad |= check("17", p(17) + "atan(7,6);shot(" + (diff > 0 ? "116" : "124") + ",0.1,0.1,8);");
        if (get32(960) != 0 || get32(964) != 17) { std::puts("fail 17 words"); bad = 1; }
    }
    reset(0); put32(960, 5); setState__11SpiderQueeni(obj, 17); bad |= check("17 without record", p(17)); if (get32(964) != 17 || get32(960) != 0) { std::puts("fail 17 without record"); bad = 1; }
    // 8: keeps the state 9 and 20 (and 6 or 18 with bit 3 of +294); otherwise the angle at +748 picks the one-shot
    for (int s : {9, 20}) { reset(s); setState__11SpiderQueeni(obj, 8); bad |= check("8 keep", p(8)); if (get16(572) != s) { std::puts("fail 8 keep"); bad = 1; } }
    for (int s : {6, 18}) { reset(s); put16(294, 8); setState__11SpiderQueeni(obj, 8); bad |= check("8 keep bit", p(8)); if (get16(572) != s) { std::puts("fail 8 keep bit"); bad = 1; } }
    struct A { int angle; const char* header; } angles[] = {{0, "60"}, {8191, "60"}, {-8191, "60"}, {8192, "68"}, {-8192, "68"}, {8193, "84"}, {24575, "84"}, {24576, "68"},
                                                           {-8193, "76"}, {-24575, "76"}, {-24576, "68"}, {-24577, "68"}, {30000, "68"}};
    for (auto a : angles) {
        reset(6); put16(748, a.angle); setState__11SpiderQueeni(obj, 8);
        bad |= check("8 angle", p(8) + "shot(" + a.header + ",0.1,0.1,16);");
        if (get16(572) != 8) { std::printf("fail 8 state %d\n", a.angle); bad = 1; }
    }
    // 8: the record's block loses its word at +756 when bit 5 of its word at +12 is set
    for (unsigned flags : {0u, 0x20u}) {
        reset(0); put32(576, 0x500); putA(0x500 + 12, 0x600); putA(0x600 + 12, flags); putA(0x600 + 756, 0x55);
        setState__11SpiderQueeni(obj, 8);
        unsigned w; std::memcpy(&w, arena + 0x600 + 756, 4);
        if (w != (flags ? 0u : 0x55u)) { std::puts("fail 8 record word"); bad = 1; }
    }
    // the Creature step may change n
    reset(5); procNew = 14; setState__11SpiderQueeni(obj, 100); bad |= check("changed", p(100) + "trans(164,0.1,3);"); if (get16(572) != 14) { std::puts("fail changed state"); bad = 1; }
    for (int s : {21, 100, -1}) { reset(5); setState__11SpiderQueeni(obj, s); bad |= check("none", p(s)); if (get16(572) != static_cast<short>(s)) { std::puts("fail none state"); bad = 1; } }
    return bad;
}
