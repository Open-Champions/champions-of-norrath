// Host check only; not a build input. Links the storage with the existing download routine.
// The DMA constructor is a stub that always succeeds; the download routine is not re-authored here.
#include "surface_cache_key.cpp"
static unsigned char arena[256];
static int calls;
extern "C" int ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(unsigned char*, unsigned char*, unsigned char*, unsigned char*) {
    ++calls;
    return 0;
}
#include "../w15-vi-raster-a-eessurfcache-download/download.cpp"
int main() {
    if (norrath_eessurfcache_key != 0) return 1;
    unsigned char surface[8], wp[8];
    unsigned char* self = arena;
    unsigned char* info = arena + 128;
    *reinterpret_cast<int*>(self + 28) = 0;
    if (Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(self, surface, info, wp) != 0) return 2;
    if (*reinterpret_cast<int*>(info + 80) != 0) return 3;
    if (norrath_eessurfcache_key != 1) return 4;
    if (*reinterpret_cast<int*>(self + 28) != 1) return 5;
    if (calls != 1) return 6;
    return 0;
}
