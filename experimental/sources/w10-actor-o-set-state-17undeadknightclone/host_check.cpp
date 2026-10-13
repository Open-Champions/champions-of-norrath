// Host contract for setState__17UndeadKnightClonei; not an EE input. 32-bit words are offsets from a
// host base; callees log. The authored function was also compared with the original in a private R5900
// run (600 random objects over states 0..17 and out-of-range values; every original word reached
// except padding).
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "set_state.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
int frameNm;
static std::string calls;
static int procResult, procNew = -1000;
static unsigned roll;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string fbits(float f) { unsigned u; std::memcpy(&u, &f, 4); char b[16]; std::snprintf(b, sizeof b, "%x", u); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
int setStateProcessing__8CreaturePi(void*, int* s) { calls += "proc" + num(*s) + ";"; if (procNew != -1000) *s = procNew; return procResult; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) { calls += "smart" + num(range) + ";"; float p[3] = {1, 2, 3}; std::memcpy(r, p, 12); }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void*, void* h, float t, int f) {
    calls += "trans(" + num(off(h) - 0x2000) + "," + fbits(t) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void*, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(h) - 0x2000) + "," + fbits(a) + "," + fbits(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
}
unsigned w10_actor_o_host_call(unsigned fw, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "v" + num(fw) + "(" + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");"; return 0;
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static short get16(unsigned o) { short v; std::memcpy(&v, obj + o, 2); return v; }
static void reset(short current) {
    std::memset(arena, 0, sizeof arena); w10_actor_o_host_base = arena; calls.clear();
    procResult = 0; procNew = -1000; roll = 0; frameNm = 100;
    put32(36, 0x300); put32(72, 0x800); putA(0x800 + 184, 0); putA(0x800 + 188, 0x77); putA(0x800 + 192, 8); putA(0x800 + 196, 0x78);
    for (unsigned i = 0; i < 200; ++i) putA(0x300 + 4 * i, 0x2000 + 4 * i);       // R[offset] = 0x2000 + offset
    std::memcpy(obj + 572, &current, 2);
    put32(808, 1000);
}
static int check(const char* label, const std::string& want) {
    if (calls == want) return 0;
    std::printf("fail %s\n  want %s\n  got  %s\n", label, want.c_str(), calls.c_str()); return 1;
}
int main() {
    int bad = 0;
    auto p = [](int n) { return "proc" + num(n) + ";"; };
    const std::string fast = "3dcccccc";
    const std::string tr3 = "," + fast + ",3);", shot = "," + fast + "," + fast + ",2056);";
    reset(0); roll = 250; setState__17UndeadKnightClonei(obj, 0);
    bad |= check("0", p(0) + "rand;trans(84" + tr3);
    if (get32(256) != 1000 + 250 % 120) { std::puts("fail 0 wait"); bad = 1; }
    reset(0); setState__17UndeadKnightClonei(obj, 1);
    bad |= check("1", p(1) + "reset;smart200;trans(108," + fast + ",2051);sync;");
    { float x, y, z; std::memcpy(&x, obj + 272, 4); std::memcpy(&y, obj + 276, 4); std::memcpy(&z, obj + 280, 4); if (x != 1 || y != 2 || z != 3) { std::puts("fail 1 destination"); bad = 1; } }
    reset(0); setState__17UndeadKnightClonei(obj, 3); bad |= check("3", p(3) + "reset;trans(108," + fast + ",2051);sync;");
    reset(0); setState__17UndeadKnightClonei(obj, 5); bad |= check("5", p(5) + "trans(60" + tr3);
    reset(0); setState__17UndeadKnightClonei(obj, 13); bad |= check("13", p(13) + "shot(116" + shot);
    struct Row { int state, shot; };
    for (Row row : {Row{14, 28}, Row{15, 36}, Row{16, 44}, Row{17, 52}}) {
        reset(0); roll = 47; setState__17UndeadKnightClonei(obj, row.state);
        bad |= check("14..17", p(row.state) + "trans(60" + tr3 + "shot(" + num(row.shot) + shot + "rand;");
        if (get32(256) != 47 % 20) { std::puts("fail wait"); bad = 1; }
    }
    for (int s : {8, 9}) {
        reset(4); setState__17UndeadKnightClonei(obj, s); bad |= check("8/9", p(s));
        if (get16(572) != 4) { std::puts("fail 8/9 state"); bad = 1; }
    }
    reset(0); put32(12, 0x10000); frameNm = 50; setState__17UndeadKnightClonei(obj, 10);
    bad |= check("10", p(10) + "trans(124," + fast + ",6);v120(8,0,0);");
    if (get32(256) != 53 || get32(12) != 0x1400du) { std::puts("fail 10 stores"); bad = 1; }
    for (int s : {2, 4, 6, 7, 11, 12, 18, 100, -1}) {
        reset(5); setState__17UndeadKnightClonei(obj, s);
        bad |= check("default", p(s) + "v119(0,1,3);");
        if (get16(572) != s) { std::puts("fail default state"); bad = 1; }
    }
    reset(5); procResult = 1; setState__17UndeadKnightClonei(obj, 1); bad |= check("proc", p(1)); if (get16(572) != 5) { std::puts("fail proc state"); bad = 1; }
    reset(5); procNew = 5; setState__17UndeadKnightClonei(obj, 100); bad |= check("changed", p(100) + "trans(60" + tr3); if (get16(572) != 5) { std::puts("fail changed state"); bad = 1; }
    return bad;
}
