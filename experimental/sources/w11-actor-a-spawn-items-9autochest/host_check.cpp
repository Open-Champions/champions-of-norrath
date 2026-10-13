// Host contract for spawnItems__9AutoChest; not an EE input. 32-bit words are offsets from a host base; the tag
// functions, the item functions and the allocator are stubs that log (descriptions are logged as the bytes the
// function sets). Each scenario lists the exact call log the rule in spawn_items.cpp gives. The authored
// function was also compared with the original in a private R5900 run (310 scenarios). Build with
// -ffp-contract=off. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w11_actor_a_host_base;
#include "spawn_items.cpp"

unsigned char globalItem[16 * 8];
int levelFlags;
static unsigned char arena[0x10000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::vector<unsigned> draws;
static size_t nextDraw;
static int nItem, nRandom, nPotions, allocCalls, failAlloc, meshCalls;
static float tagFloat, ramp;
static std::vector<int> createRandomResults;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0u; }
static std::string dsc(const void* d) {
    const unsigned char* p = static_cast<const unsigned char*>(d);
    short a[4]; std::memcpy(a, p + 36, 8); short b; std::memcpy(&b, p + 60, 2); short c; std::memcpy(&c, p + 16, 2); short e; std::memcpy(&e, p + 30, 2);
    return "[" + num(static_cast<signed char>(p[32])) + "," + num(static_cast<signed char>(p[33])) + "," + num(a[0]) + "," + num(b) + "," + num(c) + "," + num(e) + "]";
}
extern "C" {
char* findTagString__10GameObjectPci(void*, const char* tag, int i) {
    calls += std::string("tag(") + tag + "," + num(i) + ");";
    const int n = !std::strcmp(tag, "item") ? nItem : !std::strcmp(tag, "random") ? nRandom : 0;
    static char name[] = "n"; return i < n ? name : nullptr;
}
int findTagInt__10GameObjectPci(void*, const char* tag, int i) { calls += std::string("int(") + tag + "," + num(i) + ");"; return nPotions; }
float findTagFloat__10GameObjectPci(void*, const char* tag, int i) { calls += std::string("float(") + tag + "," + num(i) + ");"; return tagFloat; }
unsigned eRand__10GameObject(void*) { calls += "r;"; return draws[nextDraw++ % draws.size()]; }
void* blockAlloc__Fi(int n) { ++allocCalls; calls += "alloc" + num(n) + ";"; return allocCalls == failAlloc ? nullptr : arena + 0x6000 + 0x400 * allocCalls; }
float icos__Fi(int a) { calls += "cos(" + num(a) + ");"; return 0.25f * static_cast<float>(a % 7); }
float isin__Fi(int a) { calls += "sin(" + num(a) + ");"; return 0.125f * static_cast<float>(a % 5); }
int getProxyMeshId__4itemPQ24item11description(void*) { calls += "mesh;"; return ++meshCalls; }
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 p, char* a, char* b, char* c, long n, int flags, void* d, float f) {
    calls += "prop(" + num(off(self)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + a + "," + num(off(b)) + "," + num(off(c)) + "," + num(n) + "," + num(flags) + "," + dsc(d) + "," + num(f) + ");";
}
void initStuff__6PhPropi(void* p, int f) { float v[2]; std::memcpy(v, static_cast<unsigned char*>(p) + 252, 8); calls += "init(" + num(off(p)) + "," + num(f) + "," + num(v[0]) + "," + num(v[1]) + ");"; }
void create__4itemPcPQ24item11descriptionii(char* name, void* d, int a, int b) { calls += std::string("create(") + name + "," + dsc(d) + "," + num(a) + "," + num(b) + ");"; }
void makeRandomPotion__4itemPQ24item11description(void* d) { calls += "potion" + dsc(d) + ";"; }
int createRandom__4itemPQ24item11descriptioniiii(void* d, int amount, int cat, int a, int b) {
    calls += "random(" + num(amount) + "," + num(cat) + "," + num(a) + "," + num(b) + ");";
    const int r = createRandomResults.empty() ? 1 : createRandomResults.front(); if (!createRandomResults.empty()) createRandomResults.erase(createRandomResults.begin()); return r;
}
float gameItemTreasureRamp__Fv() { calls += "ramp;"; return ramp; }
void dropTreasure__4itemG6Point3iffffffPci(Point3 p, int h, float a, float b, float c, float d, float e, float f, char* name, int n) {
    calls += "drop(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(h) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + "," + (name ? name : "null") + "," + num(n) + ");";
}
}
static std::string spawnLog(int block, unsigned kickDraw, int firstMesh, const char* descText = "[-1,-1,-1,-1,0,0]") {
    const int angle = 100 + static_cast<int>(kickDraw & 0x3fffu) - 8192;
    const float c = 0.25f * static_cast<float>(angle % 7) * 24.0f, s = 0.125f * static_cast<float>(angle % 5) * 24.0f;
    return "alloc584;mesh;mesh;prop(" + num(0x6000 + 0x400 * block) + ",1:2:15,equipmnt.lmp," + num(0x100 + firstMesh) + "," + num(0x200 + firstMesh + 1) + ",1,1089," + descText + ",1);r;cos(" + num(angle) +
           ");sin(" + num(angle) + ");init(" + num(0x6000 + 0x400 * block) + ",1089," + num(c) + "," + num(s) + ");";
}
static int bad = 0;
static void run(const char* name, int items, int randoms, int potions, unsigned word236, std::vector<unsigned> d, int flags, const std::string& want, int fail = 0, std::vector<int> cr = {}) {
    std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
    for (int i = 0; i < 8; ++i) { unsigned a = 0x100 + i, b = 0x200 + i; std::memcpy(globalItem + 16 * i, &a, 4); std::memcpy(globalItem + 16 * i + 4, &b, 4); }
    float pt[3] = {1, 2, 3}; std::memcpy(obj + 16, pt, 12); *reinterpret_cast<short*>(obj + 56) = 100; std::memcpy(obj + 236, &word236, 4);
    nItem = items; nRandom = randoms; nPotions = potions; draws = d; nextDraw = 0; allocCalls = 0; failAlloc = fail; meshCalls = 0; levelFlags = flags; tagFloat = 2.0f; ramp = 1.5f; createRandomResults = cr;
    spawnItems__9AutoChest(obj);
    if (calls != want) { std::printf("fail: %s\n  want %s\n  got  %s\n", name, want.c_str(), calls.c_str()); bad = 1; }
}
static std::string amountText(unsigned draw, int flags, int* categoryOut, unsigned categoryDraw) {
    float amount = 2.0f * 1.5f;
    float scale = static_cast<float>(static_cast<int>(draw)) * w11a::bits(0x37800000u);
    scale = scale * w11a::bits(0x3dcccccc); scale = scale + w11a::bits(0x3f733333u); scale = scale * 300.0f;
    amount = amount * scale;
    if (flags & 0x10000) amount += amount;
    if (flags & 0x8000) amount += amount;
    *categoryOut = static_cast<int>(categoryDraw % 3u);
    return num(static_cast<int>(amount));
}
int main() {
    const std::string drops = "drop(1:2:3,100,36,2,0,0,2.5,0.75,null,0);drop(1:2:3,100,36,2,0,0,3.5,0.0199999996,null,0);drop(1:2:3,100,36,2,0,0,4.5,0.00499999989,null,0);";
    const std::string idle = "tag(item,0);int(potions,0);tag(random,0);";
    run("nothing", 0, 0, 0, 0, {30000}, 0, idle + drops + "r;");
    run("word 236", 0, 0, 0, 1, {30000}, 0, idle);
    run("arrows", 0, 0, 0, 0, {4999, 5000}, 0, idle + drops + "r;create(arrows,[-1,-1,-1,-1,0,0],-1,0);" + spawnLog(1, 5000, 1));
    run("arrows, allocation fails", 0, 0, 0, 0, {4999, 5000}, 0, idle + drops + "r;create(arrows,[-1,-1,-1,-1,0,0],-1,0);alloc584;", 1);
    run("potion", 0, 0, 0, 0, {10000, 7}, 0, idle + drops + "r;potion[-1,-1,-1,-1,0,0];" + spawnLog(1, 7, 1));
    run("negative draw is below 5000", 0, 0, 0, 0, {0xfffffff0u, 9}, 0, idle + drops + "r;create(arrows,[-1,-1,-1,-1,0,0],-1,0);" + spawnLog(1, 9, 1));
    // One item tag, one potion, one random entry: three spawns (kick draws 3, 4 and 8; scale draw 65536, category draw 5).
    for (int flags : {0, 0x10000, 0x8000, 0x18000}) {
        int cat; const std::string amount = amountText(65536, flags, &cat, 5);
        const std::string want = "tag(item,0);tag(item,0);create(n,[-1,-1,-1,-1,0,0],-1,0);" + spawnLog(1, 3, 1) + "tag(item,1);int(potions,0);potion[-1,-1,-1,-1,0,0];" + spawnLog(2, 4, 3) + "int(potions,0);tag(random,0);" +
                                 "float(random,0);ramp;r;r;random(" + amount + "," + num(cat) + ",-1,1);" + spawnLog(3, 8, 5) + "tag(random,1);";
        run("three spawns", 1, 1, 1, 0, {3, 4, 65536, 5, 8}, flags, want, 0, {1});
    }
    // createRandom failing skips that spawn; allocation failing skips only the kick.
    run("createRandom fails", 0, 1, 0, 0, {65536, 5}, 0, "tag(item,0);int(potions,0);tag(random,0);float(random,0);ramp;r;r;random(" + [] { int c; return amountText(65536, 0, &c, 5); }() + ",2,-1,1);tag(random,1);r;", 0, {0});
    return bad;
}
