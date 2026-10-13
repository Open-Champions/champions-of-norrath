// Host contract for dropTreasure__4itemG6Point3iffffffPci; not an EE input. Callees are recording stubs. The
// expectation is a second, separately written walk through the same decisions (generator model in 64-bit arithmetic)
// that produces the expected call log for many random parameter sets and generator seeds.
#include "drop_treasure.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <random>
#include <string>

int nmPlayers;
unsigned syncRandState;
float worldDifficulty;
unsigned char globalItem[16 * 8];
static std::string calls;
static float ramp1, ramp2;
static int okCombine, okNamed, meshCounter, allocFail;
static unsigned char arena[584 * 4];
static int arenaUse;
extern "C" {
unsigned long long fptodp(float v) { double d = v; unsigned long long r; std::memcpy(&r, &d, 8); return r; }
unsigned long long dpmul(unsigned long long a, unsigned long long b) { double x, y; std::memcpy(&x, &a, 8); std::memcpy(&y, &b, 8); double z = x * y; unsigned long long r; std::memcpy(&r, &z, 8); return r; }
float dptofp(unsigned long long v) { double d; std::memcpy(&d, &v, 8); return static_cast<float>(d); }
float gameTreasureRamp__Fv(void) { return ramp1; }
float gameItemTreasureRamp__Fv(void) { return ramp2; }
void dropGold__4itemG6Point3i(Point3 p, int amount) { char b[100]; std::snprintf(b, sizeof b, "gold(%g,%g,%g,%d);", p.x, p.y, p.z, amount); calls += b; }
int create__4itemPcPQ24item11descriptionii(const char* n, void* d, int a, int b) {
    const unsigned char* x = static_cast<const unsigned char*>(d);
    calls += std::string("create(") + n + "," + std::to_string(a) + "," + std::to_string(b) + "," + (x[32] == 0xff && x[33] == 0xff ? "filled" : "unfilled") + ");"; return 1; }
void makeRandomPotion__4itemPQ24item11description(void*) { calls += "potion;"; }
int createRandomCombineableItem__4itemPQ24item11descriptionsPc(void*, short level, const char* name) { calls += "comb(" + std::to_string(level) + "," + (name ? name : "null") + ");"; return okCombine; }
int createNamedRandom__4itemPQ24item11descriptioniii(void*, int a, int b, int c) { calls += "named(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ");"; return okNamed; }
int createRandom__4itemPQ24item11descriptioniiii(void*, int a, int b, int c, int d) { calls += "rand(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");"; return okNamed; }
int getProxyMeshId__4itemPQ24item11description(void*) { return meshCounter++ % 8; }
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; if (allocFail) return nullptr; return arena + 584 * (arenaUse++ % 4); }
void phprop_construct(void* self, Point3 p, const char* lump, const char* a, const char* b, long long one, int mode, void*, float s) {
    char buf[200]; std::snprintf(buf, sizeof buf, "ph(%g,%g,%g,%s,%p,%p,%lld,%d,%g);", p.x, p.y, p.z, lump, static_cast<const void*>(a), static_cast<const void*>(b), one, mode, s); calls += buf; }
void initStuff__6PhPropi(void* prop, int mode) {
    float cx, cy; std::memcpy(&cx, static_cast<unsigned char*>(prop) + 252, 4); std::memcpy(&cy, static_cast<unsigned char*>(prop) + 256, 4);
    char buf[100]; std::snprintf(buf, sizeof buf, "init(%g,%g,%d);", cx, cy, mode); calls += buf; }
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return static_cast<float>(a % 11) / 5.0f - 1.0f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return static_cast<float>(a % 7) / 3.0f - 1.0f; }
}

struct Model {
    unsigned s;
    unsigned draw() { s = static_cast<unsigned>((static_cast<unsigned long long>(s) * 1664525ull + 1013904223ull) & 0xffffffffull); return s >> 16; }
};
int main() {
    std::mt19937 rng(123);
    int failures = 0, checked = 0;
    for (int iter = 0; iter < 3000; ++iter) {
        nmPlayers = 1 + rng() % 4;
        worldDifficulty = static_cast<float>(rng() % 12) * 0.75f;
        const unsigned seed = rng();
        const float lift = static_cast<float>(static_cast<int>(rng() % 20)) - 5.0f;
        const float scale = 0.5f + (rng() % 4) * 0.5f;
        const float c = (rng() % 4) * 0.5f + 0.5f, d = (rng() % 6) * 0.2f, e = static_cast<float>(rng() % 100) / 10.0f - 0.5f, f = (rng() % 5) * 0.6f;
        const int heading = static_cast<int>(rng() % 70000) - 5000;
        const char* name = (rng() & 1) ? "nm" : nullptr;
        const int named = rng() % 3 == 0;
        ramp1 = 0.5f + (rng() % 3) * 0.5f; ramp2 = 0.5f + (rng() % 3) * 0.5f;
        okCombine = rng() % 4 != 0; okNamed = rng() % 4 != 0; meshCounter = rng() % 8; allocFail = rng() % 6 == 0; arenaUse = 0;
        syncRandState = seed;
        for (int i = 0; i < 16 * 8; ++i) globalItem[i] = static_cast<unsigned char>(i);
        const Point3 p = {1.5f, -2.5f, 3.25f};
        calls.clear();
        const int meshAtStart = meshCounter;
        dropTreasure__4itemG6Point3iffffffPci(p, heading, lift, scale, c, d, e, f, name, named);
        const unsigned finalState = syncRandState;
        // ---- model: only the decisions and arguments that matter for the log
        Model m{seed};
        std::string want;
        float cc = c, ff = f, dd = d, ee = e;
        if (nmPlayers >= 2) { cc = cc + cc; ff = ff + ff; dd = static_cast<float>(static_cast<double>(dd) * 1.5); }
        if (ee < 0.0f) ee = 0.0f;
        if (9.0f < ee) ee = 9.0f;
        const int idx = static_cast<int>(ee);
        const float t = ee - static_cast<float>(idx);
        // the table rows are data of the image and are read from the source's own table; the model only does the arithmetic
        const float r0 = __builtin_bit_cast(float, tab_bits[idx * 3]), r1 = __builtin_bit_cast(float, tab_bits[idx * 3 + 1]);
        const float r3 = __builtin_bit_cast(float, tab_bits[idx * 3 + 3]), r4 = __builtin_bit_cast(float, tab_bits[idx * 3 + 4]);
        const float aa = r0 * (1.0f - t) + r3 * t, bb = r1 * (1.0f - t) + r4 * t;
        const float u1 = static_cast<float>(m.draw()) / 65536.0f;
        float mult = 1.0f;
        if (u1 < aa) mult = 0.5f;
        if (aa + bb < u1) mult = 3.0f;
        const unsigned pick = m.draw();
        int count = pick >= 61000 ? (pick >= 64000 ? 3 : 2) : 1;
        cc = cc * ramp1;
        for (int i = 0; i < count; ++i) {
            if (static_cast<float>(m.draw()) < dd * 65536.0f) {
                const unsigned x = m.draw();
                char b[100]; std::snprintf(b, sizeof b, "gold(%g,%g,%g,%d);", p.x, p.y, p.z, static_cast<int>(static_cast<float>(x % 40 + 40) * cc * __builtin_bit_cast(float, 0x3d0f5c28u)));
                want += b;
            }
        }
        auto build = [&]() {
            want += "alloc584;";
            if (allocFail) return false;
            return true;
        };
        auto phLog = [&](int first, int second) {
            char buf[200];
            std::snprintf(buf, sizeof buf, "ph(%g,%g,%g,equipmnt.lmp,%p,%p,1,1089,1);", p.x, p.y, p.z + lift,
                          reinterpret_cast<const void*>(*reinterpret_cast<const char* const*>(globalItem + first * 16)),
                          reinterpret_cast<const void*>(*reinterpret_cast<const char* const*>(globalItem + second * 16 + 4)));
            want += buf;
        };
        int mesh = meshAtStart;
        auto nextMesh = [&]() { return mesh++ % 8; };
        const float chance1 = (static_cast<float>(nmPlayers - 1) * __builtin_bit_cast(float, 0x3d4ccccc) + 0.125f) * 65536.0f;
        if (static_cast<float>(m.draw()) < chance1) {
            const unsigned kind = m.draw();
            want += kind < 8000 ? "create(arrows,-1,0,filled);" : "potion;";
            if (build()) { const int a1 = nextMesh(), a2 = nextMesh(); phLog(a1, a2); }
        }
        bool done = false;
        if (static_cast<float>(m.draw()) < __builtin_bit_cast(float, 0x4423d70au)) {
            want += "comb(" + std::to_string(static_cast<short>(static_cast<int>(worldDifficulty))) + "," + (name ? name : "null") + ");";
            if (!okCombine) done = true;
            else if (build()) { const int a1 = nextMesh(), a2 = nextMesh(); phLog(a1, a2); }
        }
        if (!done && static_cast<float>(m.draw()) < ff * 65536.0f) {
            const float ramp = ramp2;
            const float v = static_cast<float>(m.draw()) / 65536.0f;
            float q = (mult * (v * __builtin_bit_cast(float, 0x3ecccccc) + __builtin_bit_cast(float, 0x3f4ccccc))) * (ramp * 200.0f);
            const float w = static_cast<float>(m.draw());
            if (w < __builtin_bit_cast(float, 0x4423d666u)) q *= 3.0f; else if (w < __builtin_bit_cast(float, 0x454ccbffu)) q += q;
            const int level = static_cast<int>(q);
            int kind = 0;
            if (m.draw() >= 30000) kind = m.draw() >= 60000 ? 2 : 1;
            const float roll = static_cast<float>(m.draw());
            const int wd = static_cast<int>(worldDifficulty);
            if (roll < __builtin_bit_cast(float, 0x44f5c28fu) || named) want += "named(" + std::to_string(level * 7) + "," + std::to_string(kind) + "," + std::to_string(wd) + ");";
            else want += "rand(" + std::to_string(level) + "," + std::to_string(kind) + ",-1," + std::to_string(wd) + ");";
            if (okNamed) {
                if (build()) {
                    const int a1 = nextMesh(), a2 = nextMesh(); phLog(a1, a2);
                    const int angle = heading + static_cast<int>(m.draw() & 0x3fff) - 8192;
                    char buf[160];
                    const float cs = static_cast<float>(angle % 11) / 5.0f - 1.0f, sn = static_cast<float>(angle % 7) / 3.0f - 1.0f;
                    std::snprintf(buf, sizeof buf, "cos(%d);sin(%d);init(%g,%g,1089);", angle, angle, cs * 24.0f * scale, sn * 24.0f * scale);
                    want += buf;
                }
            }
        }
        ++checked;
        if (calls != want || finalState != m.s) {
            if (failures < 3) std::fprintf(stderr, "iteration %d differs (state %08x want %08x)\n  got:  %s\n  want: %s\n", iter, finalState, m.s, calls.c_str(), want.c_str());
            ++failures;
        }
    }
    if (!failures) std::printf("dropTreasure ok (%d parameter sets)\n", checked);
    return failures != 0;
}
