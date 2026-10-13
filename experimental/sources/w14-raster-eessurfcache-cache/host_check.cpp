// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
// Every callee is stubbed; the stubs log their call order and write scripted values into the info and alloc blocks.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "cache.cpp"
#include <cstdio>
#include <cstring>
static char order[64];
static int n, decacheResult, setResult, calcResult, findResult[2], findCalls, allocResult[2], allocCalls, insertResult, downloadResult;
static int infoWord12AtAlloc, blockWordsAtAlloc[8], insertKey, setCalls, downloadCalls;
static unsigned char* setInfo;
static int slotSeen;
extern "C" int Decache__13VIEESurfCacheR9VISurface(unsigned char*, unsigned char*) { order[n++] = 'D'; return decacheResult; }
extern "C" void MoveToLRUEnd__13VIEESurfCachei(unsigned char*, int slot) { order[n++] = 'M'; slotSeen = slot; }
extern "C" void RemoveFromLRU__13VIEESurfCachei(unsigned char*, int slot) { order[n++] = 'R'; slotSeen = slot; }
extern "C" int Set__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoR14VIMaterialPass(unsigned char*, unsigned char*, unsigned char* info, unsigned char*) { order[n++] = 'S'; ++setCalls; setInfo = info; return setResult; }
extern "C" int CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo(unsigned char*, unsigned char*, unsigned char* info) { order[n++] = 'C'; for (int i = 0; i < 24; ++i) reinterpret_cast<int*>(info)[i] = 0x100 + i; return calcResult; }
extern "C" int FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(unsigned char*, unsigned char* alloc, unsigned char*) {
    order[n++] = 'F';
    int* a = reinterpret_cast<int*>(alloc);
    if (findCalls == 0) { a[0] = 2; a[3] = 3; } else { a[0] = 1; a[3] = 2; a[4] = 0x50; a[5] = 0x60; a[6] = 0x70; }
    return findResult[findCalls++];
}
extern "C" int Allocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char*, unsigned char* alloc) {
    order[n++] = 'A';
    int* a = reinterpret_cast<int*>(alloc);
    if (allocCalls == 0) infoWord12AtAlloc = a[3]; else for (int i = 0; i < 8; ++i) blockWordsAtAlloc[i] = a[i];
    return allocResult[allocCalls++];
}
extern "C" int Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo(unsigned char*, const int* key, const unsigned char*) { order[n++] = 'I'; insertKey = *key; return insertResult; }
extern "C" int Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(unsigned char*, unsigned char*, unsigned char*, unsigned*) { order[n++] = 'L'; ++downloadCalls; return downloadResult; }
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void setup() {
    std::memset(arena, 0, sizeof arena);
    // cache at 0x0, table at 0x100 (24-byte records), entries at 0x400 (104 bytes each)
    W(40) = arena_base + 0x100; W(64) = arena_base + 0x400; W(16) = 0;
    for (int i = 0; i < 4; ++i) W(0x100 + 24 * i) = 0x1000 * (i + 1);
    for (int i = 0; i < 6; ++i) W(0x400 + 104 * i + 100) = 50 + i;
    n = 0; std::memset(order, 0, sizeof order); findCalls = 0; allocCalls = 0; setCalls = 0; downloadCalls = 0;
    decacheResult = setResult = calcResult = 0; findResult[0] = findResult[1] = 0; allocResult[0] = allocResult[1] = 0; insertResult = 3; downloadResult = 0;
}
int main() {
    unsigned char* cache = arena;
    alignas(16) static unsigned char surface[128], material[128], pass[128], raster[16];
    unsigned dma = 0;
    bool ok = true;
    // already cached, same version: only the LRU update and an early 0
    setup(); std::memset(surface, 0, sizeof surface); std::memset(material, 0, sizeof material); std::memset(pass, 0, sizeof pass);
    *reinterpret_cast<int*>(surface + 24) = 2; *reinterpret_cast<int*>(pass + 72) = 52;
    ok = ok && Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(cache, surface, material, pass, raster, &dma, 0) == 0 && std::strcmp(order, "M") == 0 && slotSeen == 2;
    // cached, version differs: Set with the entry's info block, then the pass takes the new version
    setup(); *reinterpret_cast<int*>(pass + 72) = 0;
    ok = ok && Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(cache, surface, material, pass, raster, &dma, 1) == 0 && std::strcmp(order, "RS") == 0 &&
         setInfo == arena + 0x400 + 2 * 104 + 20 && *reinterpret_cast<int*>(material + 32) == 1 && *reinterpret_cast<int*>(pass + 72) == 52;
    // not cached: full path with the surface flag, the lock flag and the word at +40 set
    setup(); *reinterpret_cast<int*>(surface + 24) = -1; *reinterpret_cast<int*>(surface + 32) = 1; *reinterpret_cast<int*>(surface + 40) = 1; *reinterpret_cast<int*>(surface + 20) = 0x77;
    *reinterpret_cast<int*>(material + 32) = 0; insertResult = 4;
    int r = Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(cache, surface, material, pass, raster, &dma, 0);
    ok = ok && r == 1 && std::strcmp(order, "DCFAFAIMLS") == 0 && infoWord12AtAlloc == 3 + 0x3000 && insertKey == 0x77 &&
         *reinterpret_cast<int*>(surface + 24) == 4 && *reinterpret_cast<int*>(surface + 32) == 0 && *reinterpret_cast<int*>(material + 32) == 1 &&
         *reinterpret_cast<int*>(pass + 72) == 50 + 4 && slotSeen == 4;
    // the second block's words 4 and 5 (count 2, selector 1) were each adjusted by record 1's first word (0x2000)
    ok = ok && blockWordsAtAlloc[4] == 0x50 + 0x2000 && blockWordsAtAlloc[5] == 0x60 + 0x2000;
    // a non-zero cache word at +16 skips the download; a failing Set returns -1
    setup(); *reinterpret_cast<int*>(surface + 24) = -1; *reinterpret_cast<int*>(surface + 32) = 0; *reinterpret_cast<int*>(surface + 40) = 0; W(16) = 1; setResult = -1;
    ok = ok && Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(cache, surface, material, pass, raster, &dma, 1) == -1 && std::strcmp(order, "CFAIS") == 0 && downloadCalls == 0;
    // a failing first allocation stops the path
    setup(); *reinterpret_cast<int*>(surface + 24) = -1; allocResult[0] = -1;
    ok = ok && Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(cache, surface, material, pass, raster, &dma, 1) == -1 && std::strcmp(order, "CFA") == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
