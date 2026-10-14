// Host contract for setState__11VampireLordi; not an EE input. 32-bit words are offsets from a host
// base, table calls go through w10_actor_p_host_call, and callees log. The authored function was also
// compared with the original in a private R5900 run (2500 randomized scenarios over every state).
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "set_state.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static const unsigned BLOCK = 0x2000, TABLE = 0x2800;
static std::string calls;
static unsigned draw = 7;
static int forced = -1;       // setStateProcessing result: -1 leave alone, otherwise replace the state with this value
static int processed;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
static unsigned lastOffsetFn;
unsigned w10_actor_p_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "slot(" + num(fn) + "," + num(off(adjusted) - 0x1000) + "," + num(n) + "," + num(a1) + ");";
    return 0;
}
extern "C" {
int setStateProcessing__8CreaturePi(void* s, int* state) { calls += "proc(" + num(*state) + ");"; if (forced >= 0) *state = forced; return processed; }
unsigned eRand__10GameObject(void*) { calls += "rand;"; return draw; }
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) {
    calls += "trans(" + num(off(s) - 0x1000) + ",H" + num(off(h) - BLOCK) + "," + num(r) + "," + num(f) + ");";
}
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot(" + num(off(s) - 0x1000) + ",H" + num(off(h) - BLOCK) + "," + num(a) + "," + num(b) + "," + num(f) + ");";
}
void animGlobalSync__FP14AnimationState(void*) { calls += "sync;"; }
void resetRouting__8Creature(void*) { calls += "reset;"; }
void getSmartRandomDestination__8Creaturei(void* r, void*, int range) {
    calls += "dest(" + num(range) + ");";
    float v[3] = {1, 2, 3}; std::memcpy(r, v, 12);
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void*, float v) { calls += "sound(H" + num(off(h) - BLOCK) + "," + num(v) + ");"; }
void dropTreasure__8CreatureG6Point3iffffffPc(void*, Point3 p, int a, float b, float c, float d, float e, float f, float g, char* name) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + num(g) + "," + name + ");";
}
void convertToLord__11VampireLord(void*) { calls += "convert;"; }
unsigned long long litodp(int v) { double d = v; unsigned long long u; std::memcpy(&u, &d, 8); return u; }
unsigned long long dpadd(unsigned long long a, unsigned long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); x += y; unsigned long long u; std::memcpy(&u, &x, 8); return u; }
int dptoli(unsigned long long a) { double x; std::memcpy(&x, &a, 8); return static_cast<int>(x); }
}
static void put32(unsigned at, unsigned v) { std::memcpy(arena + at, &v, 4); }
static void put16(unsigned at, unsigned short v) { std::memcpy(arena + at, &v, 2); }
static unsigned get32(unsigned at) { unsigned v; std::memcpy(&v, arena + at, 4); return v; }
static unsigned short get16(unsigned at) { unsigned short v; std::memcpy(&v, arena + at, 2); return v; }
static unsigned long long get64(unsigned at) { unsigned long long v; std::memcpy(&v, arena + at, 8); return v; }
static void reset() {
    std::memset(arena, 0, sizeof arena); calls.clear(); w10_actor_p_host_base = arena; forced = -1; processed = 0; draw = 7;
    put32(0x1000 + 36, BLOCK); put32(0x1000 + 72, TABLE); put32(0x1000 + 808, 1000);
    for (unsigned i = 0; i < 0x400; i += 4) put32(BLOCK + i, BLOCK + i);      // header pointer == its own offset, easy to name
    { unsigned long long f = 0x00000000000003ffull; std::memcpy(arena + 0x1000 + 160, &f, 8); }
    put32(TABLE + 192 + 4, 0x1357);
}
static int bad;
static void expect(const char* name, bool ok) { if (!ok) { std::printf("fail: %s\n  calls %s\n", name, calls.c_str()); bad = 1; } }
int main() {
    reset(); setState__11VampireLordi(obj, 0);
    expect("state 0", calls == "proc(0);rand;trans(288,H52,0.1,3);" && get32(0x1000 + 256) == 1000 + 7 + 90 && get16(0x1000 + 572) == 0 && get64(0x1000 + 160) == 0x1ffull);
    reset(); setState__11VampireLordi(obj, 1);
    expect("state 1", calls == "proc(1);reset;dest(300);trans(288,H156,0.1,2051);" && get32(0x1000 + 272) == 0x3f800000 && get32(0x1000 + 276) == 0x40000000 && get16(0x1000 + 572) == 1);
    reset(); setState__11VampireLordi(obj, 21);
    expect("state 21", calls == "proc(21);sound(H196,1);trans(288,H148,0.4,6);" && get32(0x1000 + 256) == 1090 && get32(0x1000 + 968) == 0);
    reset(); setState__11VampireLordi(obj, 10);
    expect("state 10", calls == "proc(10);sound(H196,1);drop(0:0:0,0,36,2,1.6,0.6,1.4,0.25,vampirefang);trans(288,H148,0.1,6);rand;slot(4951,0,1,1187);" &&
           get32(0x1000 + 256) == 1000 + 7 + 180 && get16(0x1000 + 572) == 10);
    reset(); put32(0x1000 + 964, 1); setState__11VampireLordi(obj, 10);
    expect("state 10 no convert", calls.find("convert") == std::string::npos);
    reset(); put32(0x1000 + 964, 1); setState__11VampireLordi(obj, 6);
    expect("state 6 converts", calls == "proc(6);trans(288,H28,0.1,3);convert;" && get32(0x1000 + 256) == 20);
    reset(); put32(0x1000 + 964, 1); setState__11VampireLordi(obj, 7);
    expect("state 7 no convert", calls.find("convert") == std::string::npos && get32(0x1000 + 256) == 1240);
    reset(); put32(0x1000 + 980, 1); setState__11VampireLordi(obj, 14);
    expect("state 14 second hit", calls == "proc(14);rand;shot(288,H108,0.2,0.2,2056);" && get32(0x1000 + 988) == 1000 + 7 % 3 * 60 + 240 && get32(0x1000 + 980) == 0);
    reset(); put32(0x1000 + 980, 0); setState__11VampireLordi(obj, 15);
    expect("state 15 start", calls == "proc(15);trans(288,H60,0.1,2051);shot(288,H116,0.2,0.2,2056);");
    reset(); put32(0x1000 + 980, 4); put32(0x1000 + 992, 5); setState__11VampireLordi(obj, 17);
    expect("state 17 later", calls == "proc(17);shot(288,H100,0.2,0.2,2056);" && get32(0x1000 + 980) == 3 && get32(0x1000 + 992) == 0);
    reset(); put16(0x1000 + 572, 6); put16(0x1000 + 294, 8); setState__11VampireLordi(obj, 8);
    expect("state 8 keeps 6", calls == "proc(8);" && get16(0x1000 + 572) == 6);
    reset(); put16(0x1000 + 572, 4); setState__11VampireLordi(obj, 9);
    expect("state 9 reverts", calls == "proc(9);" && get16(0x1000 + 572) == 4);
    reset(); put16(0x1000 + 572, 0xfffe); setState__11VampireLordi(obj, 8);
    expect("state 8 sign", get16(0x1000 + 572) == 0xfffe);
    reset(); setState__11VampireLordi(obj, 23);
    expect("state 23", calls == "proc(23);" && get16(0x1000 + 572) == 23);
    reset(); processed = 1; put16(0x1000 + 572, 99); setState__11VampireLordi(obj, 3);
    expect("handled elsewhere", calls == "proc(3);" && get16(0x1000 + 572) == 99 && get64(0x1000 + 160) == 0x3ffull);
    reset(); forced = 5; setState__11VampireLordi(obj, 0);
    expect("state replaced", calls == "proc(0);trans(288,H60,0.1,3);" || calls == "proc(0);trans(288,H28,0.1,3);");
    reset(); put16(0x1000 + 572, 3); setState__11VampireLordi(obj, 3);
    expect("state 3 repeat", calls == "proc(3);" && get16(0x1000 + 572) == 3);
    reset(); put32(0x1000 + 960, 1); setState__11VampireLordi(obj, 3);
    expect("state 3 960", calls == "proc(3);trans(288,H132,0.1,2051);sync;");
    reset(); setState__11VampireLordi(obj, 20);
    expect("state 20", calls == "proc(20);trans(288,H28,0.3,3);" && get64(0x1000 + 160) == 0x3ffull);
    return bad;
}
