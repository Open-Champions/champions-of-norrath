// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "release.cpp"
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
static char trace[1024]; static int tl;
static void put(const char* tag, long a, long b = 0) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld,%ld;", tag, a, b); }
static long off(const void* p) { return static_cast<long>(word_of(p)) - static_cast<long>(W(0)); }
static int releaseResult[8], eraseResult[8], zeroWord[8];
static int doRelease(unsigned char* self, unsigned char* scene, unsigned dict, unsigned raster, unsigned sound, unsigned collide) {
    int sprite = static_cast<int>((off(self) - 0x2000 - 0) / 0x100);
    put("release", off(self), off(scene));
    put("args", static_cast<long>(dict - W(0)), static_cast<long>(raster - W(0)) * 1000000L + static_cast<long>(sound - W(0)) * 1000L + static_cast<long>(collide - W(0)) % 1000);
    if (zeroWord[sprite]) setu(0x1000 + 12 * sprite, 0);
    return releaseResult[sprite];
}
static void doDestroy(unsigned char* self, int flags) { put("destroy", off(self), flags); }
extern "C" int Erase__t6VIPool1ZP8VISpritei(unsigned char* pool, int index) { put("erase", off(pool), index); return eraseResult[index]; }
static int fRelease(unsigned char* a, unsigned char* b, unsigned c, unsigned d, unsigned e, unsigned f) { return doRelease(a, b, c, d, e, f); }
static void fDestroy(unsigned char* a, int f) { doDestroy(a, f); }
extern "C" void* norrath_host_function(unsigned w) { return w == 1 ? (void*)fRelease : w == 2 ? (void*)fDestroy : nullptr; }

static void build(const int* refs, const int* parents) {
    std::memset(arena, 0, sizeof arena);
    unsigned scene = 0x100;
    setu(scene + 996 + 8, W(0x1000));
    setu(scene + 19352, W(0x5000)); setu(scene + 19356, W(0x5100)); setu(scene + 19360, W(0x5200)); setu(scene + 19364, W(0x5300));
    for (int k = 0; k < 4; ++k) {
        unsigned obj = 0x2000 + 0x100 * k;
        setu(0x1000 + 12 * k, W(obj));
        setu(obj + 4, static_cast<unsigned>(refs[k])); setu(obj + 56, static_cast<unsigned>(parents[k]));
        setu(obj + 60, W(0x3000));
    }
    // table shared by all sprites; slot 80: adjustment 0 (so the sprite pointer is recognisable), slot 8: adjustment 0
    seth(0x3000 + 80, 0); setu(0x3000 + 84, 1);
    seth(0x3000 + 8, 0); setu(0x3000 + 12, 2);
}
int main() {
    int refs[4], parents[4];
    // A: single sprite, last reference, everything succeeds
    for (int k = 0; k < 8; ++k) { releaseResult[k] = 0; eraseResult[k] = 0; zeroWord[k] = 0; }
    for (int k = 0; k < 4; ++k) { refs[k] = 1; parents[k] = -1; }
    build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 0) != 0) return 1;
    if (__builtin_strncmp(trace, "release:8192,256;args:", 22) != 0) return 2;
    if (__builtin_strstr(trace, "destroy:8192,3;erase:1252,0;") == nullptr) return 3;
    if (getu(0x2000 + 4) != 0) return 4;
    // B: not the last reference
    refs[0] = 2; build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 0) != 0 || tl != 0 || getu(0x2000 + 4) != 1) return 5;
    refs[0] = 1;
    // C: the sprite's own release fails
    releaseResult[0] = -1; build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 0) != -1) return 7;
    if (__builtin_strstr(trace, "destroy") != nullptr || __builtin_strstr(trace, "erase") != nullptr) return 8;
    releaseResult[0] = 0;
    // D: erase fails
    eraseResult[0] = -4; build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 0) != -1) return 9;
    eraseResult[0] = 0;
    // E: a chain 2 -> 1 -> 0 releases the deepest parent first
    parents[2] = 1; parents[1] = 0;
    build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 2) != 0) return 10;
    {
        const char* r0 = __builtin_strstr(trace, "release:8192,"); const char* r1 = __builtin_strstr(trace, "release:8448,"); const char* r2 = __builtin_strstr(trace, "release:8704,");
        if (!r0 || !r1 || !r2 || !(r0 < r1 && r1 < r2)) return 11;
        const char* e0 = __builtin_strstr(trace, "erase:1252,0;"); const char* e1 = __builtin_strstr(trace, "erase:1252,1;"); const char* e2 = __builtin_strstr(trace, "erase:1252,2;");
        if (!e0 || !e1 || !e2 || !(e0 < e1 && e1 < e2)) return 12;
    }
    // F: the parent is shared (3 references): it is only counted down, the child is still released
    refs[1] = 3; refs[2] = 1; build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 2) != 0) return 13;
    if (__builtin_strstr(trace, "release:8448,") != nullptr) return 14;
    if (__builtin_strstr(trace, "release:8704,") == nullptr) return 15;
    if (getu(0x2000 + 0x100 + 4) != 2) return 16;
    // the parent's failure is ignored: child 2 still releases
    refs[1] = 1; releaseResult[1] = -1; build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 2) != 0) return 17;
    if (__builtin_strstr(trace, "release:8704,") == nullptr) return 18;
    releaseResult[1] = 0;
    // G: the release function clears the pool record: no destructor call, erase still happens
    parents[2] = -1; parents[1] = -1; zeroWord[0] = 1; build(refs, parents); tl = 0; trace[0] = 0;
    if (ReleaseSprite__7VIScenei(arena + 0x100, 0) != 0) return 19;
    if (__builtin_strstr(trace, "destroy") != nullptr || __builtin_strstr(trace, "erase:1252,0;") == nullptr) return 20;
    return 0;
}
