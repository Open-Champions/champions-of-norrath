// Host contract for Ice msg_run; not an EE input. Words are offsets from a host base (see words.hpp);
// the table's function word 1 selects the deleter stub.
#define W10_ACTOR_H_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w10h_host_base;
static void deleter(void*, int);
void* w10h_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(&deleter) : nullptr; }
extern "C" unsigned char vt_PhIce[8];
extern "C" unsigned char vaporParticle[128] = {};
#include "msg_run.cpp"
unsigned char vt_PhIce[8];

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char props[4][600] __attribute__((aligned(16)));
static std::vector<int> rolls; static size_t rollIndex; static int gameState; static std::string calls;
static int allocIndex; static bool nullFirst;
struct Ctor { void* prop; unsigned char point[12]; char *a, *b, *c; long flags; int d; void* desc; float scale; };
static std::vector<Ctor> ctors; static std::vector<std::pair<void*, int>> inits;
struct Part { void* def; void* pos; float v[3]; float extra; }; static std::vector<Part> parts;
static void* delSelf; static int delFlag; static std::string stateName; static char *lumpA, *lumpB; static unsigned char resource[32];
extern "C" {
int scriptGetGameState__FPc(char* n) { calls += "S"; stateName = n; return gameState; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += "L"; lumpA = a; lumpB = b; return resource; }
void* blockAlloc__Fi(int size) { calls += "A"; if (size != 592) std::puts("fail: alloc size"); if (nullFirst && allocIndex == 0) { ++allocIndex; return nullptr; } return props[allocIndex++ % 4]; }
void initStuff__6PhPropi(void* p, int v) { calls += "I"; inits.push_back({p, v}); }
int eRand__10GameObject(void*) { calls += "r"; return rolls.at(rollIndex++ % rolls.size()); }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, void* p, void* v, float e) { calls += "P"; Part q; q.def = d; q.pos = p; std::memcpy(q.v, v, 12); q.extra = e; parts.push_back(q); }
void phprop_construct(void* p, const void* pos, char* a, char* b, char* c, long f, int d, void* desc, float s) __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
void phprop_construct(void* p, const void* pos, char* a, char* b, char* c, long f, int d, void* desc, float s) {
    calls += "C"; Ctor q; q.prop = p; std::memcpy(q.point, pos, 12); q.a = a; q.b = b; q.c = c; q.flags = f; q.d = d; q.desc = desc; q.scale = s; ctors.push_back(q); }
}
static void deleter(void* s, int f) { calls += "D"; delSelf = s; delFlag = f; }
static float comp(int r) { float v = static_cast<float>(r) * 0x1.0p-16f; v = v + v; return v - 1.0f; }
static float unit(int r) { return static_cast<float>(r) * 0x1.0p-16f; }
int main() {
    int bad = 0;
    w10h_host_base = arena;
    for (int variant = 0; variant < 6; ++variant) {
        unsigned char* object = arena + 0x1000;
        std::memset(arena, 0x22, sizeof arena); std::memset(props, 0x77, sizeof props);
        calls.clear(); ctors.clear(); inits.clear(); parts.clear(); allocIndex = 0; delSelf = nullptr; delFlag = -1;
        gameState = variant == 0 ? 0 : 3; nullFirst = variant == 5;
        resource[18] = variant == 1 ? 0 : (variant == 2 ? 0x80 : (variant == 3 ? 2 : 3));
        // draws per prop: [3 for props][rejected 3 + accepted 3][1 speed]
        static const int pattern[] = {50000, 20000, 65536,  60000, 60000, 60000,  40000, 30000, 36000,  45000};
        rolls.assign(pattern, pattern + 10); rollIndex = 0;
        unsigned t = 0x100, fnWord = 1; short adj = 24; std::memcpy(object + 72, &t, 4); std::memcpy(arena + t + 8, &adj, 2); std::memcpy(arena + t + 12, &fnWord, 4);
        unsigned w76 = 0x300, w88 = 0x380, w92 = 0x3c0; std::memcpy(object + 76, &w76, 4); std::memcpy(object + 88, &w88, 4); std::memcpy(object + 92, &w92, 4);
        for (int i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0x30 + i);
        msg_run__3Ice(object);
        if (stateName != "iceSheetBroken") { std::puts("fail: state name"); bad = 1; }
        if (variant == 0) { if (calls != "S" || delSelf) { std::puts("fail: state zero returns at once"); bad = 1; } continue; }
        if (lumpA != reinterpret_cast<char*>(arena + w76) || lumpB != reinterpret_cast<char*>(arena + w88)) { std::puts("fail: lump arguments"); bad = 1; }
        const int count = static_cast<signed char>(resource[18]);
        const int props_wanted = count > 0 ? (count + 1) / 2 : 0;
        if (static_cast<int>(ctors.size()) != props_wanted - (nullFirst && props_wanted > 0 ? 1 : 0)) { std::printf("fail: constructed %zu of %d (variant %d)\n", ctors.size(), props_wanted, variant); bad = 1; }
        if (delSelf != object + 24 || delFlag != 3) { std::puts("fail: delete call"); bad = 1; }
        if (static_cast<int>(parts.size()) != props_wanted) { std::puts("fail: one particle per iteration"); bad = 1; }
        for (size_t i = 0; i < ctors.size(); ++i) {
            const Ctor& c = ctors[i];
            const int k = 2 * static_cast<int>(i + (nullFirst ? 1 : 0));
            if (std::memcmp(c.point, object + 16, 12) != 0 || c.a != reinterpret_cast<char*>(arena + w76) || c.b != reinterpret_cast<char*>(arena + w88) ||
                c.c != reinterpret_cast<char*>(arena + w92) || c.flags != (1L << k) || c.d != 1556 || c.desc != nullptr || c.scale != 1.0f) { std::puts("fail: constructor arguments"); bad = 1; }
            unsigned char* p = static_cast<unsigned char*>(c.prop);
            unsigned w; float f;
            std::memcpy(&w, p + 72, 4); if (w != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_PhIce))) { std::puts("fail: table word"); bad = 1; }
            std::memcpy(&w, p + 584, 4); if (w != 0) bad = 1;
            std::memcpy(&w, p + 588, 4); if (w != 0) bad = 1;
            std::memcpy(&w, p + 564, 4); if (w != 20) { std::puts("fail: 564"); bad = 1; }
            if (i == 0 && !nullFirst) {
                std::memcpy(&f, p + 252, 4); if (f != (unit(50000) - 0.5f) * 5.0f) { std::puts("fail: +252"); bad = 1; }
                std::memcpy(&f, p + 256, 4); if (f != -100.0f - unit(20000) * 200.0f) { std::puts("fail: +256"); bad = 1; }
                std::memcpy(&f, p + 48, 4); if (f != unit(65536) * 200.0f + 100.0f) { std::puts("fail: +48"); bad = 1; }
            }
        }
        if (!parts.empty() && !(nullFirst)) {
            // first prop: rolls 0-2 go to the prop fields, then the rejected draw, the accepted one and the speed
            const Part& q = parts[0];
            float x = comp(40000), y = comp(30000), z = comp(36000);
            float len2 = (x * x + y * y) + z * z; float s = 1.0f / __builtin_sqrtf(len2);
            float speed = unit(45000) * 10.0f * 35.0f;
            float vy = y * s * speed, vx = x * s * speed * 0x1.47ae14p-8f;
            if (vy > 0) vy = -vy;
            if (std::fabs(q.v[0] - vx) > 1e-6f || std::fabs(q.v[1] - vy) > 1e-5f || q.v[2] != 0.0f || q.extra != 0.0f || q.def != vaporParticle) { std::puts("fail: particle"); bad = 1; }
            if (q.v[1] > 0) { std::puts("fail: y not positive"); bad = 1; }
            if (q.pos != static_cast<unsigned char*>(ctors[0].prop) + 16) { std::puts("fail: particle position"); bad = 1; }
        }
    }
    if (!bad) std::puts("ice msg_run ok");
    return bad;
}
