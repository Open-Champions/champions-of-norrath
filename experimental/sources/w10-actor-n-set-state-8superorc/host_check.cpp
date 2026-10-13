// Host contract for setState__8SuperOrci; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900
// run (all states -7..20 with random object state, 235 of 241 original words reached, the rest
// padding).
#define W10_ACTOR_N_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_n_host_base;
#include "set_state.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int procResult, procNew = -1000;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float t, int f) {
    calls += "trans(" + num(off(h) - 0x2000) + "," + num(t) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(h) - 0x2000) + "," + num(a) + "," + num(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + (name ? name : "null") + ");";
}
void soundFadeStream__Fi(int s) { calls += "fade" + num(s) + ";"; }
}
unsigned w10_actor_n_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted)) + "," + num(n) + "," + num(a1) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current) {
    std::memset(arena, 0, sizeof arena); w10_actor_n_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000;
    put32(36, 0x300); put32(72, 0x400); putA(0x400 + 184, 8); putA(0x400 + 188, 0x77); putA(0x400 + 192, 0x10008); putA(0x400 + 196, 0x78);
    for (unsigned i = 0; i < 100; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = 9; std::memcpy(obj + 56, &h, 2);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    auto p = [](int n) { return "proc" + num(n) + ";"; };
    const std::string one = ",0.1,3);", shots = ",0.1,0.1,2056);";
    for (int s : {0, 9}) { reset(0); setState__8SuperOrci(obj, s); bad |= check("0/9", p(s) + "trans(68,0.3,3);"); if (get16(572) != s) { std::puts("fail 0/9 state"); bad = 1; } }
    reset(0); setState__8SuperOrci(obj, 1);
    bad |= check("1", p(1) + "reset;smart200;trans(92,0.3,2051);sync;");
    { float x; std::memcpy(&x, obj + 272, 4); float y; std::memcpy(&y, obj + 276, 4); float z; std::memcpy(&z, obj + 280, 4); if (x != 1 || y != 2 || z != 3) { std::puts("fail 1 destination"); bad = 1; } }
    reset(0); setState__8SuperOrci(obj, 3); bad |= check("3", p(3) + "reset;trans(92,0.3,2051);sync;");
    reset(0); setState__8SuperOrci(obj, 5); bad |= check("5", p(5) + "trans(60" + one);
    reset(0); setState__8SuperOrci(obj, 6); bad |= check("6", p(6) + "trans(60" + one + "shot(28" + shots);
    reset(0); setState__8SuperOrci(obj, 14); bad |= check("14", p(14) + "trans(60" + one + "shot(36" + shots);
    reset(0); setState__8SuperOrci(obj, 15); bad |= check("15", p(15) + "trans(60" + one + "shot(84" + shots);
    reset(0); setState__8SuperOrci(obj, 16); bad |= check("16", p(16) + "trans(60" + one + "shot(52" + shots);
    // 8: state 15 calls the table entry at +184 with 16 and becomes 16; any other state is kept.
    reset(15); setState__8SuperOrci(obj, 8); bad |= check("8/15", p(8) + "v119(4104,1,16);"); if (get16(572) != 16) { std::puts("fail 8/15 state"); bad = 1; }
    reset(7); setState__8SuperOrci(obj, 8); bad |= check("8/7", p(8)); if (get16(572) != 7) { std::puts("fail 8/7 state"); bad = 1; }
    // 10: the table entry at +192 has adjustment 1 (low halfword of 0x10008 is 8).
    reset(0); setState__8SuperOrci(obj, 10);
    bad |= check("10", p(10) + "trans(100,0.1,12);drop(4:5:6,9,36,2,3,1,3,1,null);v120(" + num(0x1000 + 8) + ",0,0);fade0;");
    // 13: distance from the point at +16 to the record's point decides the word at +964.
    for (int far : {0, 1}) {
        reset(0); put32(576, 0x500); putA(0x500 + 12, 0x600);
        float q[2] = {far ? 4.0f + 121.0f : 4.0f + 119.0f, 5.0f}; std::memcpy(arena + 0x600 + 16, q, 8);
        put32(964, 5); put32(960, 3); setState__8SuperOrci(obj, 13);
        bad |= check("13", p(13) + "trans(60" + one + "shot(44" + shots);
        if (get32(964) != static_cast<unsigned>(far) || get32(960) != 0) { std::puts("fail 13 stores"); bad = 1; }
    }
    reset(0); put32(964, 5); put32(960, 3); setState__8SuperOrci(obj, 13);
    if (get32(964) != 5 || get32(960) != 0) { std::puts("fail 13 without record"); bad = 1; }
    // nothing more for 2, 4, 7, 11, 12 and out-of-range states; the state halfword still follows n
    for (int s : {2, 4, 7, 11, 12, 17, 18, 100}) { reset(5); setState__8SuperOrci(obj, s); bad |= check("none", p(s)); if (get16(572) != s) { std::puts("fail none state"); bad = 1; } }
    // a non-zero result from the Creature step ends the call before anything else, including the state store
    reset(5); procResult = 1; setState__8SuperOrci(obj, 1); bad |= check("proc", p(1)); if (get16(572) != 5) { std::puts("fail proc state"); bad = 1; }
    // the Creature step may change n
    reset(5); procNew = 5; setState__8SuperOrci(obj, 100); bad |= check("changed", p(100) + "trans(60" + one); if (get16(572) != 5) { std::puts("fail changed state"); bad = 1; }
    return bad;
}
