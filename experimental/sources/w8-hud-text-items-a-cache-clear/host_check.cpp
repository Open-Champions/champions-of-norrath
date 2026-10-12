// Host contract for cacheClear__Fv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstdint>
#include <initializer_list>
#include <cstring>
static unsigned char arena[16][64];
unsigned char* norrath_host_base = reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(arena) & ~std::uintptr_t(0xffffffffu));
#include "cache_clear.cpp"
extern "C" {
unsigned char cache[192];
unsigned char dummyTex[64];
int cacheHead, cacheTail;
}
static unsigned word(const void* p) { return NORRATH_WORD(p); }
static void fill() {
    std::memset(arena, 0, sizeof arena);
    std::memset(cache, 0x5a, sizeof cache);
    for (int i = 0; i < 16; ++i) {
        unsigned w = word(arena[i]);
        std::memcpy(cache + i * 12, &w, 4);
        unsigned short mark = 0x7777; std::memcpy(arena[i] + 24, &mark, 2);
        std::memset(dummyTex, 0, sizeof dummyTex);
    }
    unsigned short m = 0x7777; std::memcpy(dummyTex + 24, &m, 2);
}
static unsigned short mark(int i) { unsigned short m; std::memcpy(&m, arena[i] + 24, 2); return m; }
static int after() {
    unsigned want = word(dummyTex);
    for (int i = 0; i < 16; ++i) {
        unsigned w; std::memcpy(&w, cache + i * 12, 4);
        if (w != want) return 1;
        for (int b : {4, 5, 6, 7, 8, 9}) if (cache[i * 12 + b] != 0) return 2;
        for (int b : {10, 11}) if (cache[i * 12 + b] != 0x5a) return 3;
    }
    return cacheHead == 0 && cacheTail == 0 ? 0 : 4;
}
int main() {
    fill(); cacheHead = 3; cacheTail = 6;
    cacheClear__Fv();
    for (int i = 0; i < 16; ++i) if (mark(i) != ((i >= 3 && i <= 6) ? 0 : 0x7777)) return 10 + i;
    if (int r = after()) return 30 + r;
    fill(); cacheHead = 14; cacheTail = 1;   // wraps around the ring
    cacheClear__Fv();
    for (int i = 0; i < 16; ++i) if (mark(i) != ((i == 14 || i == 15 || i == 0 || i == 1) ? 0 : 0x7777)) return 50 + i;
    if (int r = after()) return 70 + r;
    fill(); cacheHead = 5; cacheTail = 5;     // single record
    cacheClear__Fv();
    for (int i = 0; i < 16; ++i) if (mark(i) != (i == 5 ? 0 : 0x7777)) return 90 + i;
    if (int r = after()) return 110 + r;
    // The dummy texture itself is not cleared unless a live record points at it.
    unsigned short dm; std::memcpy(&dm, dummyTex + 24, 2);
    return dm == 0x7777 ? 0 : 120;
}
