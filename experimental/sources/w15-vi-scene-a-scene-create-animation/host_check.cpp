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
static void put(const char* tag, long v) { tl += __builtin_snprintf(trace + tl, sizeof trace - tl, "%s:%ld;", tag, v); }
static unsigned nextBlock; static int addResult; static unsigned replaceWith;
extern "C" unsigned __builtin_new(unsigned size) { put("new", size); return nextBlock; }
extern "C" void __13VIHSpriteAnim(unsigned char* a) { put("ctor", word_of(a) - W(0)); }
extern "C" int Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim(unsigned char* pool, unsigned* element) {
    put("add", word_of(pool) - W(0)); put("elem", *element == 0 ? -1 : static_cast<long>(*element - W(0)));
    if (replaceWith) *element = replaceWith;
    return addResult;
}
extern "C" void* norrath_host_function(unsigned) { return nullptr; }
int main() {
    const int adds[] = {-5, -1, 0, 4};
    const int flags[] = {0, 1, 5};
    for (int k : flags) {
        for (int a : adds) {
            for (int replace = 0; replace < 2; ++replace) {
                std::memset(arena, 0, sizeof arena);
                setu(0x2000 + 4, 10); setu(0x2200 + 4, 20);
                nextBlock = W(0x2000); addResult = a; replaceWith = replace ? W(0x2200) : 0; tl = 0; trace[0] = 0;
                int got = CreateAnimation__7VIScenei(arena + 0x100, k);
                if (got != (a < 0 ? -1 : a)) return 1;
                unsigned first = getu(0x2000 + 4), second = getu(0x2200 + 4);
                unsigned want1 = 10, want2 = 20;
                if (a >= 0 && k != 0) { if (replace) want2 = 21; else want1 = 11; }
                if (first != want1 || second != want2) return 2;
                const char* base = "new:56;ctor:8192;add:1276;elem:8192;";
                if (__builtin_strncmp(trace, base, __builtin_strlen(base)) != 0) return 3;
            }
        }
    }
    // failed allocation: constructor is skipped and the null pointer still goes to Add
    std::memset(arena, 0, sizeof arena);
    nextBlock = 0; addResult = 2; replaceWith = 0; tl = 0; trace[0] = 0;
    if (CreateAnimation__7VIScenei(arena + 0x100, 0) != 2) return 4;
    if (__builtin_strcmp(trace, "new:56;add:1276;elem:-1;") != 0) return 5;
    return 0;
}
