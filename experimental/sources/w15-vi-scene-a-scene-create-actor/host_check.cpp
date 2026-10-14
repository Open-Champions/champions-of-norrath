// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "create.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
static unsigned W(unsigned off) { return word_of(arena + off); }
static void setu(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static unsigned getu(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void seth(unsigned off, short v) { std::memcpy(arena + off, &v, 2); }
extern "C" void* norrath_host_function(unsigned);
static char trace[512]; static int tl;
static void put(const char* tag, long a, long b = 0, long c = 0) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld,%ld,%ld;", tag, a, b, c); }
static long off(const void* p) { return static_cast<long>(word_of(p)) - static_cast<long>(W(0)); }
static int addResultOk, probeResult, makeResult, updateResult;
extern "C" int Add__t6VIList1Z7VIActor(unsigned char* list) { put("add", off(list)); return addResultOk ? static_cast<int>(W(0x4000)) : -1; }
extern "C" int UpdateActor__7VIScenei(unsigned char* scene, int actor) { put("update", off(scene), static_cast<long>(actor) - static_cast<long>(W(0)),
                                                                              static_cast<long>(getu(0x4000 + 96))); return updateResult; }
static int probe(unsigned char* self, unsigned char* scene, unsigned char* vec) { put("probe", off(self), off(scene), off(vec)); return probeResult; }
static int make(unsigned char* self, unsigned char* scene, unsigned raster, unsigned sound, unsigned collide, int zero) {
    put("make", off(self), off(scene), static_cast<long>(raster - W(0)) * 1000000L + static_cast<long>(sound - W(0)) * 1000L + static_cast<long>(collide - W(0)) % 1000 + zero);
    return makeResult;
}
extern "C" void* norrath_host_function(unsigned w) { return w == 1 ? (void*)probe : w == 2 ? (void*)make : nullptr; }
static float fl(unsigned off_) { float f; std::memcpy(&f, arena + off_, 4); return f; }
int main() {
    const int probes[] = {0, 1, 2, -1};
    const int updates[] = {-3, 0, 5};
    for (int sprite = 0; sprite < 3; ++sprite) for (int p : probes) for (int u : updates) for (int ok = 0; ok < 2; ++ok) {
        std::memset(arena, 0, sizeof arena);
        unsigned scene = 0x100;
        setu(scene + 996 + 8, W(0x1000));
        setu(scene + 19356, W(0x5100)); setu(scene + 19360, W(0x5200)); setu(scene + 19364, W(0x5300));
        for (int k = 0; k < 5; ++k) {
            unsigned obj = 0x2000 + 0x100 * k;
            setu(0x1000 + 12 * k, W(obj)); setu(obj + 4, 10 + k); setu(obj + 56, 0xffffffffu); setu(obj + 60, W(0x3000));
        }
        seth(0x3000 + 16, 24); setu(0x3000 + 20, 1);
        seth(0x3000 + 72, -16); setu(0x3000 + 76, 2);
        for (int i = 0; i < 0x200; ++i) arena[0x4000 + i] = 0x5a;
        float a[3] = {1.5f, -2.25f, 3.0f}, b[3] = {0.5f, 8.0f, -7.5f};
        addResultOk = ok; probeResult = p; makeResult = 3; updateResult = u; tl = 0; trace[0] = 0;
        int got = CreateActor__7VISceneRC7VIVect3T1fi(arena + scene, reinterpret_cast<unsigned char*>(a), reinterpret_cast<unsigned char*>(b), 6.25f, sprite);
        if (!ok) {
            if (got != -1) return 1;
            if (__builtin_strcmp(trace, "add:1228,0,0;") != 0) return 2;
            if (arena[0x4000] != 0x5a) return 3;
            continue;
        }
        if (got != static_cast<int>(W(0x4000)) && u >= 0) return 4;
        if (u < 0 && got != -1) return 5;
        if (fl(0x4000 + 68) != 1.5f || fl(0x4000 + 72) != -2.25f || fl(0x4000 + 76) != 3.0f) return 6;
        if (fl(0x4000 + 80) != 0.5f || fl(0x4000 + 84) != 8.0f || fl(0x4000 + 88) != -7.5f) return 7;
        if (fl(0x4000 + 92) != 6.25f) return 8;
        const unsigned zeros[] = {64, 100, 128, 132, 112, 116, 120};
        for (unsigned z : zeros) if (getu(0x4000 + z) != 0) return 9;
        const unsigned minus[] = {104, 108, 152, 156};
        for (unsigned m : minus) if (getu(0x4000 + m) != 0xffffffffu) return 10;
        if (fl(0x4000 + 124) != 1.0f) return 11;
        // untouched bytes of the record
        for (unsigned i = 0; i < 0x200; ++i) {
            bool written = (i >= 64 && i < 68 + 0 + 0) || (i >= 68 && i < 100) || (i >= 100 && i < 140) || i == 152 || (i >= 152 && i < 160);
            if (!written && arena[0x4000 + i] != 0x5a) return 12;
        }
        char expect[512]; int el = 0;
        el += __builtin_snprintf(expect + el, sizeof expect - el, "add:%ld,0,0;", static_cast<long>(scene + 972));
        el += __builtin_snprintf(expect + el, sizeof expect - el, "probe:%ld,%ld,%ld;", static_cast<long>(0x2000 + 0x100 * sprite + 24), static_cast<long>(scene), 0x4000L + 80);
        unsigned wantIndex = sprite;
        if (p == 1) {
            el += __builtin_snprintf(expect + el, sizeof expect - el, "make:%ld,%ld,%ld;", static_cast<long>(0x2000 + 0x100 * sprite - 16), static_cast<long>(scene),
                                     0x5100L * 1000000L + 0x5200L * 1000L + 0x5300L % 1000);
            wantIndex = 3;
        }
        el += __builtin_snprintf(expect + el, sizeof expect - el, "update:%ld,%ld,%u;", static_cast<long>(scene), 0x4000L, wantIndex);
        if (__builtin_strcmp(expect, trace) != 0) return 13;
        if (getu(0x4000 + 96) != wantIndex) return 14;
        // reference counts and the back link
        for (int k = 0; k < 5; ++k) {
            unsigned want = 10 + k;
            if (p == 1) { if (k == sprite) want++; }
            if (k == static_cast<int>(wantIndex)) want++;
            if (getu(0x2000 + 0x100 * k + 4) != want) return 15;
            unsigned wantParent = (p == 1 && k == 3) ? static_cast<unsigned>(sprite) : 0xffffffffu;
            if (getu(0x2000 + 0x100 * k + 56) != wantParent) return 16;
        }
    }
    return 0;
}
