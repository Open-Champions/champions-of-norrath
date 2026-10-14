#include <cstring>
#include <cstdint>
#define memset unit_memset
#include "player_deallocate_armor_caches.cpp"
#undef memset
extern "C" void* unit_memset(void* d, int v, unsigned n) { return memset(d, v, n); }
int cacheLoadCache;
signed char cacheCdDone;
unsigned eqLumpHeader;
unsigned wLumpHeader;
alignas(8) unsigned char norrath_player_armor_cache_view_7cb770[3520 + 64];
static int waits, wait_until;
static void* freed[64];
static int nfreed;
extern "C" void cdWait__Fv() { if (++waits >= wait_until) cacheCdDone = 1; }
extern "C" void memFree__FPv(void* p) {
    freed[nfreed++] = p;
}
// memset is provided by the host libc here; the unit declares it with
// an unsigned count, which matches the 32-bit target.
static void fill() {
    memset(norrath_player_armor_cache_view_7cb770, 0xaa, sizeof norrath_player_armor_cache_view_7cb770);
}
static unsigned char* el(int g, int s) { return norrath_player_armor_cache_view_7cb770 + g * 88 + s * 352; }
int main() {
    fill();
        // pointers in a mix of slots; others null
    int expect_order[40], nexp = 0;
    for (int g = 0; g < 4; ++g)
        for (int s = 0; s < 10; ++s) {
            unsigned v = ((g + s) % 3 == 0) ? 0x1000u + g * 10 + s : 0u;
                        memset(el(g, s), 0xaa, 88);
            memcpy(el(g, s), &v, 4);
            if (v) expect_order[nexp++] = g * 10 + s;
        }
    cacheLoadCache = 5; cacheCdDone = 0; wait_until = 3;
    eqLumpHeader = 0x9001; wLumpHeader = 0x9002;
    playerDeallocateArmorCaches__Fv();
    if (waits != 3) return 1;
    if (cacheLoadCache != 0) return 2;
    if (nfreed != nexp + 2) return 3;
    for (int i = 0; i < nexp; ++i) if ((unsigned long)freed[i] != 0x1000u + expect_order[i]) return 4;
    if ((unsigned long)freed[nexp] != 0x9001 || (unsigned long)freed[nexp + 1] != 0x9002) return 5;
    if (eqLumpHeader || wLumpHeader) return 6;
    for (int g = 0; g < 4; ++g)
        for (int s = 0; s < 10; ++s)
            for (int i = 0; i < 88; ++i) if (el(g, s)[i]) return 7;
    // bytes between elements of a group (not part of any 88-byte element) are untouched
    for (int g = 0; g < 4; ++g)
        for (int i = 0; i < 3520; ++i) {
            int off = i, inel = 0;
            for (int gg = 0; gg < 4; ++gg) for (int s = 0; s < 10; ++s) {
                int base = gg * 88 + s * 352; if (off >= base && off < base + 88) inel = 1; }
            if (!inel && norrath_player_armor_cache_view_7cb770[i] != 0xaa) return 8;
        }
    for (int i = 3520; i < 3584; ++i) if (norrath_player_armor_cache_view_7cb770[i] != 0xaa) return 9;
    // no cache load pending: no waits even with cacheCdDone clear; null lumps are not freed
    nfreed = 0; waits = 0; cacheLoadCache = 0; cacheCdDone = 0;
    playerDeallocateArmorCaches__Fv();
    if (waits != 0 || nfreed != 0) return 10;
    // load pending but already done: no wait, flag cleared
    cacheLoadCache = 1; cacheCdDone = 1;
    playerDeallocateArmorCaches__Fv();
    if (waits != 0 || cacheLoadCache != 0) return 11;
    return 0;
}
