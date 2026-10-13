#define NORRATH_HOST_CONTRACT 1
#include <initializer_list>
#include <string.h>
#include <stdint.h>
alignas(16) static unsigned char arena[0x10000];
static unsigned char scratch[1024];
extern "C" unsigned char* norrath_host_address(int value) { return arena + value; }
extern "C" unsigned char* norrath_host_load_pointer(const void* field) { uint32_t v; memcpy(&v, field, 4); return arena + v; }
extern "C" void norrath_host_store_pointer(void* field, unsigned char* value) { uint32_t v = static_cast<uint32_t>(value - arena); memcpy(field, &v, 4); }
extern "C" volatile unsigned char* norrath_host_scratch_byte(unsigned address) { return &scratch[address - 0x70000000u]; }
extern "C" { unsigned char SurfCache[16]; }

static void setw(unsigned char* p, int v) { memcpy(p, &v, 4); }
static int getw(const unsigned char* p) { int v; memcpy(&v, p, 4); return v; }

static int cacheCalls, cacheResult, dither[4], ditherCount, beginResult, mtlResult, mtlCalls, mtlArg, cdpResult, cdpCalls, cdpPacket, uploads;
static void* cachePass[4]; static void* cacheSurface[4]; static int cacheFlag[4];
static unsigned char* gMaterial;
extern "C" int Cache__13VIEESurfCacheR9VISurfaceR10VIMaterialR14VIMaterialPassR8VIRasterPP10_sceDmaTagi(void* cache, void* surface, void* material, void* pass, void*, void*, int flag) {
    if (cache != SurfCache || material != gMaterial) return 99;
    cachePass[cacheCalls] = pass; cacheSurface[cacheCalls] = surface; cacheFlag[cacheCalls] = flag;
    return cacheCalls++ == 1 ? cacheResult : 0;
}
extern "C" int SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(void*, int enable, void*) { dither[ditherCount++] = enable; return 0; }
extern "C" int Begin__Ct6VIPool1Z19VIRasterMaterialLRU(void*) { return beginResult; }
extern "C" int MoveToLast__t6VIPool1Z19VIRasterMaterialLRUi(void*, int index) { ++mtlCalls; mtlArg = index; return mtlResult; }
extern "C" int ConstructDMAPacket__10VIMaterialR8VIRasterP7VIQWord(void*, void*, unsigned int packet) { ++cdpCalls; cdpPacket = static_cast<int>(packet); return cdpResult; }
extern "C" int UploadPerMaterial__8VIRasterPP10_sceDmaTag(void*, void*) { ++uploads; return 0; }
#include "add_material.cpp"

int main() {
    alignas(16) static unsigned char raster[20000];
    const int matAddr = 0x4000, ownerAddr = 0x4400;
    unsigned char* M = arena + matAddr; unsigned char* O = arena + ownerAddr;
    unsigned char* items = arena + 0x100; unsigned char* table = arena + 0x600; unsigned char* tag = arena + 0x800;
    gMaterial = M;
    // Each scenario: {passes, secondPassCacheResult, flags12, word32, word36, victimOwnerIsOther}
    struct Case { int passes; int cacheR; int flags; int w32; int w36; bool owner; int stamp; int itemStamp; int begin; int mtl; int scratchSame; } cases[] = {
        {0, 0, 0x10, 0, -1, false, 5, 0, 2, 3, 0},   // new slot, dither on, no passes
        {2, 0, 0, 0, -1, true, 5, 0, 2, 3, 1},       // two passes, victim owned by another material, color unchanged
        {2, -1, 0, 0, -1, false, 5, 0, 2, 3, 0},     // second Cache call fails: -1
        {1, 0, 0, 0, 1, false, 5, 5, 2, 4, 0},       // reuse slot, same stamp
        {1, 0, 4, 0, 1, false, 6, 5, 2, 4, 0},       // flags bit 2 forces rebuild
        {1, 0, 0, 7, 1, false, 6, 5, 2, 4, 1},       // word32 forces rebuild
        {1, 0, 0, 0, 1, false, 6, 5, 2, 4, 0},       // reuse slot, different stamp
    };
    for (const Case& c : cases) {
        memset(arena, 0, sizeof arena); memset(raster, 0, sizeof raster); memset(scratch, 0, sizeof scratch);
        setw(M + 8, c.passes); setw(M + 12, c.flags); setw(M + 32, c.w32); setw(M + 36, c.w36); setw(M + 16, 0x11223344);
        for (int p = 0; p < 3; ++p) setw(M + 48 + 144 * p, p == 0 ? 1 : 2);
        norrath_host_store_pointer(raster + 19388, table);
        for (int i = 0; i < 4; ++i) norrath_host_store_pointer(table + 12 * i, arena + 0x700 + 16 * i);
        norrath_host_store_pointer(raster + 19460, items);
        setw(raster + 19500, c.stamp);
        for (int i = 0; i < 8; ++i) { setw(items + 24 * i, 0x3000 + i); setw(items + 24 * i + 4, 10 + i); setw(items + 24 * i + 8, -1); setw(items + 24 * i + 12, c.itemStamp); }
        if (c.w36 >= 0) setw(items + 24 * c.w36 + 8, matAddr);
        if (c.owner) { setw(items + 24 * c.begin + 8, ownerAddr); setw(O + 36, c.begin); }
        unsigned char* slot = raster + 19492; norrath_host_store_pointer(slot, tag);
        memset(tag, 0x5a, 32);
        unsigned want224 = c.scratchSame ? 0x11223344u : 0u; memcpy(scratch + 0x224, &want224, 4);
        cacheCalls = 0; cacheResult = c.cacheR; ditherCount = 0; beginResult = c.begin; mtlResult = c.mtl; mtlCalls = 0; cdpResult = 77; cdpCalls = 0; uploads = 0;
        int ret = AddMaterial__8VIRasteriPP10_sceDmaTag(raster, matAddr, slot);
        if (c.cacheR < 0 && c.passes >= 2) { if (ret != -1 || cacheCalls != 2 || ditherCount != 0 || getw(raster + 17824) != 0) return 1; continue; }
        if (ret != 0 || cacheCalls != c.passes) return 2;
        if (ditherCount != 1 || dither[0] != ((c.flags & 0x10) ? 1 : 0)) return 3;
        for (int i = 0; i < cacheCalls; ++i) if (cachePass[i] != M + 48 + 144 * i || cacheFlag[i] != 0 || cacheSurface[i] != arena + 0x700 + 16 * (i == 0 ? 1 : 2)) return 4;
        bool rebuild = c.w32 != 0 || (c.flags & 4) || c.w36 < 0;
        int slotIndex = getw(M + 36);
        if (rebuild) {
            if (slotIndex != c.mtl || mtlArg != c.begin || cdpCalls != 1 || cdpPacket != 0x3000 + c.mtl) return 5;
            if (getw(items + 24 * c.mtl + 4) != 77) return 6;
            if (getw(items + 24 * c.mtl + 8) != matAddr || getw(items + 24 * c.mtl + 12) != c.stamp) return 7;
            if (getw(raster + 19480) != 1) return 8;
            if (c.owner && c.begin != c.mtl && (getw(O + 36) != -1 || getw(items + 24 * c.begin + 8) != -1)) return 9;
        } else {
            if (slotIndex != c.mtl || mtlArg != c.w36 || cdpCalls != 0) return 10;
            if (getw(items + 24 * c.mtl + 12) != c.stamp) return 11;
            if (getw(raster + 19480) != (c.itemStamp == c.stamp ? 0 : 1)) return 12;
        }
        // Tag: byte 3 = 0x30, word 1 = item word 0, field 0 = item word 4; pointer advanced.
        int used = getw(M + 36);
        if (tag[3] != 0x30 || getw(tag + 4) != 0x3000 + used) return 13;
        short qwc; memcpy(&qwc, tag, 2);
        if (qwc != getw(items + 24 * used + 4)) return 14;
        if (norrath_host_load_pointer(slot) != tag + 16) return 15;
        if (uploads != (c.scratchSame ? 0 : 1)) return 16;
        unsigned s224; memcpy(&s224, scratch + 0x224, 4);
        if (s224 != 0x11223344u) return 17;
        if (getw(raster + 17824) != matAddr) return 18;
    }
    // Early out when the raster flag is set.
    memset(raster, 0, sizeof raster); setw(raster + 17804, 1); setw(M + 8, 3);
    cacheCalls = 0;
    if (AddMaterial__8VIRasteriPP10_sceDmaTag(raster, matAddr, raster + 19492) != 0 || cacheCalls != 0 || getw(raster + 17824) != 0) return 19;
    return 0;
}
