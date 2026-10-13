// Host contract for msg_trigger__9MummyKingi; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_J_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include "../w10-actor-j-common/access.hpp"
unsigned char* w10_actor_j_host_base;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
char bossMeterName[128];
int bossMeterCurrent;
void soundResetStream__Fi(int s) { calls += "reset" + num(s) + ";"; }
void soundStartMusic__FPci(char* n, int v) { calls += std::string("music(") + n + "," + num(v) + ");"; }
void fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(void* m, void* m2, void* a, Point3 p, short s, float f, float g, int h) {
    calls += "dust(" + num(off(m)) + "," + num(off(m2)) + "," + num(off(a) - 0x1000) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(s) + "," + num(f) + "," + num(g) + "," + num(h) + ");";
}
void fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi(void* m, void* m2, void* a, Point3 p, short s, float f, float g, int h) {
    calls += "sparkles(" + num(off(m)) + "," + num(off(m2)) + "," + num(off(a) - 0x1000) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(s) + "," + num(f) + "," + num(g) + "," + num(h) + ");";
}
}
#include "msg_trigger.cpp"
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void setup(unsigned w960) {
    std::memset(arena, 0, sizeof arena); w10_actor_j_host_base = arena; calls.clear();
    std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -9;
    put32(36, 0x300); putA(0x300 + 12, 0x400); putA(0x300 + 116, 0x500);
    float pt[3] = {4, 5, 6}; std::memcpy(obj + 16, pt, 12);
    short h = 7; std::memcpy(obj + 56, &h, 2); float f = 2.5f; std::memcpy(obj + 224, &f, 4);
    put32(960, w960);
}
int main() {
    int bad = 0;
    setup(0); put32(160, 0xffffffffu); put32(164, 0xffffffffu); put32(12, 0xffffffffu); float v = 41.75f; std::memcpy(obj + 560, &v, 4);
    msg_trigger__9MummyKingi(obj, 1);
    unsigned lo, hi, w12; std::memcpy(&lo, obj + 160, 4); std::memcpy(&hi, obj + 164, 4); std::memcpy(&w12, obj + 12, 4);
    if (calls != "reset0;music(mx_boss_act3_mumking_l.vag,259);" || lo != 0xfbffffffu || hi != 0xffffffffu || w12 != 0xfeffffffu ||
        std::memcmp(bossMeterName, "MummyKing", 10) != 0 || bossMeterName[10] != 'x' || bossMeterCurrent != 41) { std::printf("fail 1\n  %s\n", calls.c_str()); bad = 1; }
    setup(9); msg_trigger__9MummyKingi(obj, 0);
    unsigned w960; std::memcpy(&w960, obj + 960, 4);
    if (calls != "dust(1024,1280,288,4:5:6,7,2.5,0,0);" || w960 != 0) { std::printf("fail 0a\n  %s\n", calls.c_str()); bad = 1; }
    setup(0); msg_trigger__9MummyKingi(obj, 0); std::memcpy(&w960, obj + 960, 4);
    if (calls != "sparkles(1024,1280,288,4:5:6,7,2.5,0,0);" || w960 != 14) { std::printf("fail 0b\n  %s\n", calls.c_str()); bad = 1; }
    for (int other : {2, -1, 99}) {
        setup(5); msg_trigger__9MummyKingi(obj, other); std::memcpy(&w960, obj + 960, 4);
        if (!calls.empty() || w960 != 5 || bossMeterCurrent != -9) { std::printf("fail other %d\n", other); bad = 1; }
    }
    return bad;
}
