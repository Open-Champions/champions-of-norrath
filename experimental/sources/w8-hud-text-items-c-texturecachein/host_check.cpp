#include <cstring>
struct Tex { unsigned char before[4]; unsigned short size; unsigned char middle[18]; short cachedAddress; short cacheSlot; };
struct Rec { Tex* texture; short start; short end; short lockCount; short unobserved; };
Rec cache[16];
int cacheHead, cacheTail, textureBufferSize, textureBufferStartAddress;
extern "C" void textureCacheIn__FP8_texture(Tex*);

static Tex tex[20];
static void reset() {
    memset(cache, 0, sizeof cache); memset(tex, 0, sizeof tex);
    cacheHead = cacheTail = 0; textureBufferSize = 1000; textureBufferStartAddress = 0x12340100;
    for (Tex& t : tex) t.cachedAddress = -1;
}
static bool put(int n, int size) { tex[n].size = size; textureCacheIn__FP8_texture(&tex[n]); return true; }
#define CHECK(c, code) do { if (!(c)) return code; } while (0)

int main() {
    reset();
    put(0, 100);
    CHECK(cache[0].start == 0 && cache[0].end == 100 && cache[0].texture == &tex[0] && cache[0].lockCount == 0, 1);
    CHECK(tex[0].cachedAddress == 0x100 && tex[0].cacheSlot == 0 && cacheHead == 1 && cacheTail == 0, 2);
    put(1, 200); put(2, 600);
    CHECK(cache[1].start == 100 && cache[1].end == 300 && cache[2].start == 300 && cache[2].end == 900, 3);
    CHECK(tex[2].cachedAddress == 0x100 + 300 && tex[2].cacheSlot == 2 && cacheHead == 3, 4);
    // Does not fit after the newest record: restart at 0, evicting what it overlaps.
    put(3, 150);
    CHECK(cache[3].start == 0 && cache[3].end == 150 && cacheHead == 4 && cacheTail == 2, 5);
    CHECK(tex[0].cachedAddress == 0 && tex[1].cachedAddress == 0 && tex[2].cachedAddress == 0x100 + 300, 6);
    // Fits after the newest record without evicting.
    put(4, 100);
    CHECK(cache[4].start == 150 && cache[4].end == 250 && cacheTail == 2 && tex[2].cachedAddress != 0, 7);
    // A locked overlapped record aborts the placement.
    cache[2].lockCount = 1;
    put(5, 500);
    CHECK(cacheHead == 5 && cacheTail == 2 && tex[2].cachedAddress != 0 && tex[5].cachedAddress == -1, 8);
    cache[2].lockCount = 0;
    put(5, 500);
    CHECK(cache[5].start == 250 && cache[5].end == 750 && cacheHead == 6 && cacheTail == 3 && tex[2].cachedAddress == 0, 9);
    // Evicting everything leaves a single new record.
    put(6, 400);
    CHECK(cache[6].start == 0 && cache[6].end == 400 && cacheHead == 7 && cacheTail == 6, 10);
    CHECK(tex[3].cachedAddress == 0 && tex[4].cachedAddress == 0 && tex[5].cachedAddress == 0, 11);
    // Zero size is treated as one.
    reset(); put(0, 0);
    CHECK(cache[0].end == 1, 12);
    // Wrapped ring: newest record starts below the oldest.
    reset();
    put(0, 100); put(1, 200); put(2, 600); put(3, 150);
    CHECK(cacheTail == 2 && cacheHead == 4, 13);
    put(7, 900);
    CHECK(cache[4].start == 0 && cache[4].end == 900 && cacheHead == 5 && cacheTail == 4, 14);
    CHECK(tex[2].cachedAddress == 0 && tex[3].cachedAddress == 0, 15);
    // Wrapped ring with a locked tail record aborts before any placement.
    reset();
    put(0, 100); put(1, 200); put(2, 600); put(3, 150);
    cache[2].lockCount = 2;
    put(7, 900);
    CHECK(cacheHead == 4 && cacheTail == 2 && tex[2].cachedAddress != 0, 16);
    // Full ring: evicts the tail to make room for the head slot.
    reset();
    for (int i = 0; i < 15; i++) { cache[i].texture = &tex[i]; cache[i].start = i * 10; cache[i].end = i * 10 + 10; tex[i].cachedAddress = 5; }
    cacheHead = 15; cacheTail = 0;
    put(16, 10);
    CHECK(cache[15].start == 150 && cache[15].end == 160 && cache[15].texture == &tex[16], 17);
    CHECK(tex[0].cachedAddress == 0 && cacheTail == 1 && cacheHead == 0, 18);
    // Full ring with a locked tail aborts.
    reset();
    for (int i = 0; i < 15; i++) { cache[i].texture = &tex[i]; cache[i].start = i * 10; cache[i].end = i * 10 + 10; tex[i].cachedAddress = 5; }
    cacheHead = 15; cacheTail = 0; cache[0].lockCount = 1;
    put(16, 10);
    CHECK(cacheHead == 15 && cacheTail == 0 && tex[0].cachedAddress == 5 && cache[15].texture == 0, 19);
    return 0;
}
