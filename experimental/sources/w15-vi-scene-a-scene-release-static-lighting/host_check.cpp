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
static int removeResult, eraseResult, resourceResult;
extern "C" int Remove__12VIDictionaryR10VIResource(unsigned char* d, unsigned char* r) { put("remove", off(d), off(r)); return removeResult; }
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
extern "C" int ReleaseResources__16VIStaticLightingR8VIRasterR12VIDictionary(unsigned char* l, unsigned char* r, unsigned char* d) { put("res", off(l), off(r) * 100000 + off(d)); return resourceResult; }
extern "C" void _$_t7VIArray1Zi(unsigned char* a, int flags) { put("arrdtor", off(a), flags); }
extern "C" void __builtin_delete(unsigned char* a) { put("delete", off(a)); }
extern "C" int Erase__t6VIPool1ZP16VIStaticLightingi(unsigned char* pool, int index) { put("erase", off(pool), index); return eraseResult; }
int main() {
    const int refs[] = {1, 2, 0, -4};
    const int flagsSet[] = {0, 1};
    const int removes[] = {-3, 0};
    const int erases[] = {-1, 0, 5};
    const int resources[] = {-2, 0, 4};
    for (int index = 0; index < 3; ++index) for (int r : refs) for (int flag : flagsSet) for (int rm : removes) for (int er : erases) for (int rs : resources) {
        std::memset(arena, 0, sizeof arena);
        unsigned scene = 0x100;
        setu(scene + 1068 + 8, W(0x1000));
        setu(scene + 19352, W(0x5000)); setu(scene + 19356, W(0x5100));
        for (int k = 0; k < 3; ++k) { setu(0x1000 + 12 * k, W(0x2000 + 0x100 * k)); setu(0x2000 + 0x100 * k + 4, 100 + k); }
        unsigned obj = 0x2000 + 0x100 * index;
        setu(obj + 4, static_cast<unsigned>(r)); setu(obj, static_cast<unsigned>(flag));
        removeResult = rm; eraseResult = er; resourceResult = rs; tl = 0; trace[0] = 0;
        int got = ReleaseStaticLighting__7VIScenei(arena + scene, index);
        char expect[512]; int el = 0; int want;
        if (r - 1 > 0) want = 0;
        else {
            el += __builtin_snprintf(expect + el, sizeof expect - el, "res:%ld,%ld;", static_cast<long>(obj), 0x5100L * 100000 + 0x5000L);
            want = 0;
            if (rs < 0) want = -1;
            if (want == 0 && flag != 0) { el += __builtin_snprintf(expect + el, sizeof expect - el, "remove:%ld,%ld;", 0x5000L, static_cast<long>(obj)); if (rm < 0) want = -1; }
            if (want == 0) {
                el += __builtin_snprintf(expect + el, sizeof expect - el, "arrdtor:%ld,2;delete:%ld,0;erase:%ld,%d;", static_cast<long>(obj + 8), static_cast<long>(obj), static_cast<long>(scene + 1068), index);
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
