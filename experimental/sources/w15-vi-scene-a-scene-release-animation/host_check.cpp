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
static char trace[512]; static int tl;
static void put(const char* tag, long a, long b = 0) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld,%ld;", tag, a, b); }
static long off(const void* p) { return static_cast<long>(word_of(p)) - static_cast<long>(W(0)); }
static int removeResult, eraseResult;
extern "C" int Remove__12VIDictionaryR10VIResource(unsigned char* d, unsigned char* r) { put("remove", off(d), off(r)); return removeResult; }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
extern "C" void _$_13VIHSpriteAnim(unsigned char* a, int flags) { put("dtor", off(a), flags); }
extern "C" int Erase__t6VIPool1ZP13VIHSpriteAnimi(unsigned char* pool, int index) { put("erase", off(pool), index); return eraseResult; }
int main() {
    const int refs[] = {1, 2, 3, 0, -4};
    const int flagsSet[] = {0, 1, 7};
    const int removes[] = {-3, 0, 2};
    const int erases[] = {-1, 0, 5};
    for (int index = 0; index < 3; ++index) for (int r : refs) for (int flag : flagsSet) for (int rm : removes) for (int er : erases) {
        std::memset(arena, 0, sizeof arena);
        unsigned scene = 0x100;
        setu(scene + 1020 + 8, W(0x1000));
        setu(scene + 19352, W(0x5000));
        for (int k = 0; k < 3; ++k) { setu(0x1000 + 12 * k, W(0x2000 + 0x100 * k)); setu(0x2000 + 0x100 * k + 4, 100 + k); setu(0x2000 + 0x100 * k, 0x77); }
        unsigned obj = 0x2000 + 0x100 * index;
        setu(obj + 4, static_cast<unsigned>(r)); setu(obj, static_cast<unsigned>(flag));
        removeResult = rm; eraseResult = er; tl = 0; trace[0] = 0;
        int got = ReleaseAnimation__7VIScenei(arena + scene, index);
        char expect[512]; int el = 0; int want;
        if (r - 1 > 0) want = 0;
        else {
            want = 0;
            if (flag != 0) { el += __builtin_snprintf(expect + el, sizeof expect - el, "remove:%ld,%ld;", 0x5000L, static_cast<long>(obj)); if (rm < 0) want = -1; }
            if (want == 0) {
                el += __builtin_snprintf(expect + el, sizeof expect - el, "dtor:%ld,3;erase:%ld,%d;", static_cast<long>(obj), static_cast<long>(scene + 1020), index);
                want = er < 0 ? -1 : 0;
            }
        }
        expect[el] = 0;
        if (got != want) return 1;
        if (__builtin_strcmp(expect, trace) != 0) return 2;
        if (getu(obj + 4) != static_cast<unsigned>(r - 1)) return 3;
        for (int k = 0; k < 3; ++k) if (k != index && getu(0x2000 + 0x100 * k + 4) != static_cast<unsigned>(100 + k)) return 4;
    }
    return 0;
}
