// Host contract for msg_trigger__11VampireLordi; not an EE input. 32-bit words are offsets from a host
// base; callees log. The authored function was also compared with the original in a private R5900 run.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_trigger.cpp"

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
char bossMeterName[128];
int bossMeterCurrent, bossMeterMax;
char* batParams[2];
static std::string calls;
static unsigned rolls[6] = {0, 5999, 6000, 11999, 12000, 40000};
static int rollIndex, allocIndex;
static bool failAlloc;
static unsigned char critters[6][8];
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }
extern "C" {
void soundResetStream__Fi(int s) { calls += "reset(" + num(s) + ");"; }
void soundStartMusic__FPci(char* n, int id) { calls += std::string("music(") + n + "," + num(id) + ");"; }
unsigned eRand__10GameObject(void* o) { calls += o == obj ? "rand;" : "badrand;"; return rolls[rollIndex++ % 6]; }
float icos__Fi(int a) { calls += "cos(" + num(a) + ");"; return static_cast<float>(a) / 1000.0f; }
float isin__Fi(int a) { calls += "sin(" + num(a) + ");"; return static_cast<float>(a) / 500.0f; }
void* blockAlloc__Fi(int size) { calls += "alloc(" + num(size) + ");"; if (failAlloc) return nullptr; return critters[allocIndex++ % 6]; }
void critter_construct(void* self, char* name, Point3 p, short yaw, char** args) {
    calls += std::string("critter(") + num(static_cast<unsigned char*>(self) - critters[0]) + "," + name + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," +
             num(yaw) + "," + (args == batParams ? "params" : "other") + ");";
}
void listOnewayLink__FP8ListHeadT0Pv(void* h, void* l, void* d) {
    calls += "link(" + num(off(h) - 0x1000) + "," + num(static_cast<unsigned char*>(l) - critters[0]) + "," + (l == d ? "same" : "diff") + ");";
}
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) {
    calls += "anim(" + num(off(s) - 0x1000) + "," + num(off(h)) + "," + num(r) + "," + num(f) + ");";
}
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
int main() {
    int bad = 0;
    w10_actor_p_host_base = arena;
    // message 1
    std::memset(arena, 0, sizeof arena); calls.clear();
    putf(560, 33.75f); putf(564, 120.5f);
    std::memset(bossMeterName, 'x', sizeof bossMeterName);
    msg_trigger__11VampireLordi(obj, 1);
    if (calls != "reset(0);music(mx_boss_act2_vamplord_l.vag,259);" || std::strcmp(bossMeterName, "VampireLord") != 0 || bossMeterName[12] != 'x' ||
        bossMeterCurrent != 33 || bossMeterMax != 120) { std::puts(calls.c_str()); bad |= 1; }
    // other messages
    for (int m : {2, -1, 3}) {
        std::memset(arena, 0, sizeof arena); calls.clear(); obj[570] = 8;
        msg_trigger__11VampireLordi(obj, m);
        if (!calls.empty() || obj[572] != 0) bad |= 2;
    }
    // message 0, bit 3 clear
    std::memset(arena, 0, sizeof arena); calls.clear(); obj[570] = 0xf7; put32(808, 0x500);
    msg_trigger__11VampireLordi(obj, 0);
    unsigned a, b; std::memcpy(&a, obj + 1020, 4); std::memcpy(&b, obj + 1016, 4);
    if (!calls.empty() || a != 1 || b != 0x500 + 120 || obj[572] != 0) bad |= 4;
    // message 0, bit 3 set
    for (bool fail : {false, true}) {
        std::memset(arena, 0, sizeof arena); calls.clear(); rollIndex = allocIndex = 0; failAlloc = fail;
        obj[570] = 0x08; put32(808, 0x777); put32(968, 99); put32(36, 0x300); { unsigned h = 0x640; std::memcpy(arena + 0x300 + 148, &h, 4); }
        float pt[3] = {10, 20, 30}; std::memcpy(obj + 16, pt, 12);
        unsigned short yaw = 40000; std::memcpy(obj + 56, &yaw, 2);
        putf(32, 8.0f);
        msg_trigger__11VampireLordi(obj, 0);
        std::string want;
        for (int i = 0; i < 6; ++i) {
            unsigned angle = (((40000u + 5461u * i - 16383u) & 0xffffu) + rolls[i] % 12000u - 6000u) & 0xffffu;
            want += "rand;cos(" + num(angle) + ");sin(" + num(angle) + ");alloc(1088);";
            if (!fail) {
                float c = static_cast<float>(static_cast<int>(angle)) / 1000.0f * 240.0f + 10, s = static_cast<float>(static_cast<int>(angle)) / 500.0f * 240.0f + 20;
                want += "critter(" + num(8 * i) + ",bat," + num(c) + ":" + num(s) + ":34," + num(static_cast<short>(angle + 0x801f)) + ",params);link(1028," + num(8 * i) + ",same);";
            }
        }
        want += "anim(288,1600,0.1,3);";
        unsigned w1024, w968; std::memcpy(&w1024, obj + 1024, 4); std::memcpy(&w968, obj + 968, 4);
        if (calls != want || obj[572] != 22 || obj[573] != 0 || w1024 != 0x777 || w968 != 0) { std::printf("fail fail=%d\n want %s\n got  %s\n", fail, want.c_str(), calls.c_str()); bad |= 8; }
    }
    return bad;
}
