// Host contract for msg_poke__5Chesti; not an EE input. Pointer words are offsets from a host base
// (W10_ACTOR_D_HOST). Every callee is a logging stub with scripted results.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[1];
extern "C" { unsigned char globalItem[0x6a0]; int frameNm; int levelFlags; }
#include "msg_poke.cpp"
static std::string calls; static unsigned char* objectUnderTest;
static int itemCount, potionCount, randomCount, rollIndex, allocBudget, createRandomResult;
static int rolls[16]; static float tagFloat;
static std::string f(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static std::string d(void* description) {
    const unsigned char* p = static_cast<const unsigned char*>(description);
    unsigned short h[4]; std::memcpy(&h[0], p + 36, 2); std::memcpy(&h[1], p + 44, 2); std::memcpy(&h[2], p + 52, 2); std::memcpy(&h[3], p + 60, 2);
    unsigned zero = 0; for (unsigned i = 16; i < 32; ++i) zero |= p[i];
    return "D(" + std::to_string(h[0] == 0xffff && h[1] == 0xffff && h[2] == 0xffff && h[3] == 0xffff) + std::to_string(p[32] == 0xff && p[33] == 0xff) + std::to_string(zero == 0) + ")";
}
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return w10_actor_d_host_base + 5; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) { calls += "seq(" + std::to_string(static_cast<unsigned char*>(h) - w10_actor_d_host_base) + "," + std::to_string(o == objectUnderTest) + "," + f(v) + ");"; }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int fl) { calls += "animAdd(" + std::to_string(static_cast<unsigned char*>(s) - objectUnderTest) + "," + std::to_string(static_cast<unsigned char*>(h) - w10_actor_d_host_base) + "," + std::to_string(fl) + ");"; }
void objectAddToRunList__FP10GameObject(void*) { calls += "run;"; }
static char tagNames[8][8];
char* findTagString__10GameObjectPci(void*, const char* n, int i) {
    calls += std::string("fs(") + n + "," + std::to_string(i) + ");";
    int limit = std::strcmp(n, "item") == 0 ? itemCount : std::strcmp(n, "random") == 0 ? randomCount : 0;
    if (i >= limit) return nullptr;
    std::snprintf(tagNames[i], 8, "n%d", i); return tagNames[i];
}
int findTagInt__10GameObjectPci(void*, const char* n, int i) { calls += std::string("fi(") + n + "," + std::to_string(i) + ");"; return potionCount; }
float findTagFloat__10GameObjectPci(void*, const char* n, int i) { calls += std::string("ff(") + n + "," + std::to_string(i) + ");"; return tagFloat; }
int create__4itemPcPQ24item11descriptionii(char* n, void* desc, int a, int b) { calls += std::string("create(") + n + "," + d(desc) + "," + std::to_string(a) + "," + std::to_string(b) + ");"; return 1; }
void makeRandomPotion__4itemPQ24item11description(void* desc) { calls += "potion(" + d(desc) + ");"; }
void* createRandom__4itemPQ24item11descriptioniiii(void* desc, int a, int b, int c, int e) {
    calls += "crand(" + d(desc) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(e) + ");"; return createRandomResult ? desc : nullptr;
}
int getProxyMeshId__4itemPQ24item11description(void*) { calls += "pmi;"; return 3; }
float gameItemTreasureRamp__Fv() { calls += "ramp;"; return 2.0f; }
int eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++ % 16]; }
static unsigned char props[4][600]; static int propCount;
void* blockAlloc__Fi(int n) { calls += "alloc(" + std::to_string(n) + ");"; if (allocBudget-- <= 0) return nullptr; return props[propCount++ % 4]; }
void phProp_construct(void* s, const Point3* p, char* a, char* b, char* c, int dd, int e, void* desc, float fl) {
    calls += "prop(" + f(p->x) + "," + f(p->y) + "," + f(p->z) + "," + a + "," + std::to_string(reinterpret_cast<unsigned long>(b) & 0xffffffff) + "," + std::to_string(reinterpret_cast<unsigned long>(c) & 0xffffffff) + "," + std::to_string(dd) + "," + std::to_string(e) + "," + d(desc) + "," + f(fl) + ");";
    (void)s;
}
void initStuff__6PhPropi(void* p, int v) { float x, y; std::memcpy(&x, static_cast<unsigned char*>(p) + 252, 4); std::memcpy(&y, static_cast<unsigned char*>(p) + 256, 4); calls += "init(" + f(x) + "," + f(y) + "," + std::to_string(v) + ");"; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
void hudGivePlayerGold__FiG6Point3i(int pl, const Point3* p, int amount) { calls += "gold(" + std::to_string(pl) + "," + f(p->x) + "," + f(p->y) + "," + f(p->z) + "," + std::to_string(amount) + ");"; }
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
template <typename T> static T get(unsigned o) { T v; std::memcpy(&v, objectUnderTest + o, sizeof v); return v; }
template <typename T> static void put(unsigned o, T v) { std::memcpy(objectUnderTest + o, &v, sizeof v); }
int main() {
    alignas(16) static unsigned char arena[8192];
    w10_actor_d_host_base = arena;
    unsigned char* object = arena + 2048; objectUnderTest = object;
    auto setup = [&](int w132, short w76) {
        std::memset(arena, 0, sizeof arena); put<unsigned>(36, 300u); put<unsigned>(0, 0);
        unsigned w28 = 777; std::memcpy(arena + 300 + 28, &w28, 4);
        put<int>(132, w132); put<short>(76, w76); put<short>(56, 100); put<unsigned>(12, 0xffffffffu);
        put<float>(16, 1.0f); put<float>(20, 2.0f); put<float>(24, 3.0f);
        frameNm = 4321; levelFlags = 0; calls.clear(); itemCount = potionCount = randomCount = 0; rollIndex = 0; allocBudget = 100; createRandomResult = 1; tagFloat = 4.0f;
        for (int i = 0; i < 16; ++i) rolls[i] = 0x1000 + i;
        unsigned rec0 = 5000, rec1 = 6000; std::memcpy(globalItem + 16 * 3, &rec0, 4); std::memcpy(globalItem + 16 * 3 + 4, &rec1, 4);
    };
    // already opened: pay out
    setup(1, 25); put<unsigned>(84, 0); put<int>(0, 0); calls.clear();
    msg_poke__5Chesti(object, 6);
    expect(calls == "lump(fx.lmp,game_pu_coin.anm);seq(5,1,1);gold(6,1,2,3,25);" && get<int>(84) == 1 && get<short>(76) == 0, "second poke pays out");
    setup(0, 5); msg_poke__5Chesti(object, 0);
    expect(calls == "lump(fx.lmp,door_wdchst_open.anm);seq(5,1,1);animAdd(92,777,516);run;fs(item,0);fi(potions,0);fs(random,0);" && get<int>(132) == 1 && get<int>(80) == 4321 && get<unsigned>(12) == 0xffffffffu, "open with nothing inside, +76 positive keeps the flag");
    setup(0, 0); msg_poke__5Chesti(object, 0);
    expect(get<unsigned>(12) == 0xffefffffu, "+76 not positive clears bit 0x100000");
    setup(0, -3); msg_poke__5Chesti(object, 0);
    expect(get<unsigned>(12) == 0xffefffffu, "+76 negative clears bit 0x100000");
    // two named items, the second allocation fails
    setup(0, 5); itemCount = 2; rolls[0] = 0x2000; allocBudget = 1; msg_poke__5Chesti(object, 0);
    expect(calls.find("fs(item,0);fs(item,0);create(n0,D(111),-1,0);alloc(584);pmi;pmi;prop(1,2,15,equipmnt.lmp,5000,6000,1,1089,D(111),1);rand;cos(") != std::string::npos, "first item spawns");
    expect(calls.find("init(12,-6,1089);fs(item,1);fs(item,1);create(n1,D(111),-1,0);alloc(584);fs(item,2);") != std::string::npos, "launch speed is 24 times the trig value; failed allocation skips the launch");
    // potions: count re-read every pass
    setup(0, 5); potionCount = 2; allocBudget = 0; msg_poke__5Chesti(object, 0);
    expect(calls.find("fi(potions,0);potion(D(111));alloc(584);fi(potions,0);potion(D(111));alloc(584);fi(potions,0);fs(random,0);") != std::string::npos, "two potions");
    // random: amount arithmetic and doubling
    setup(0, 5); randomCount = 1; allocBudget = 0; rolls[0] = 0; rolls[1] = 2; levelFlags = 0; msg_poke__5Chesti(object, 0);
    expect(calls.find("fs(random,0);ff(random,0);ramp;rand;rand;crand(D(111),") != std::string::npos, "random item call order");
    { // roll 0: ((0*0.1)+0.95)*600 = 570; 4 * 2 * 570 = 4560; category rolls[1]=2 % 3 = 2
      expect(calls.find("crand(D(111),4560,2,-1,1);") != std::string::npos, "random amount, category"); }
    setup(0, 5); randomCount = 1; allocBudget = 0; rolls[0] = 0; rolls[1] = 1; levelFlags = 0x10000; msg_poke__5Chesti(object, 0);
    expect(calls.find("crand(D(111),9120,1,-1,1);") != std::string::npos, "bit 0x10000 doubles");
    setup(0, 5); randomCount = 1; allocBudget = 0; rolls[0] = 0; rolls[1] = 3; levelFlags = 0x18000; msg_poke__5Chesti(object, 0);
    expect(calls.find("crand(D(111),18240,0,-1,1);") != std::string::npos, "both bits double twice");
    setup(0, 5); randomCount = 2; allocBudget = 0; createRandomResult = 0; msg_poke__5Chesti(object, 0);
    expect(calls.find("alloc(") == std::string::npos && calls.find("fs(random,1);") != std::string::npos && calls.find("fs(random,2);") != std::string::npos, "a zero createRandom skips the spawn");
    return failures ? 1 : 0;
}
