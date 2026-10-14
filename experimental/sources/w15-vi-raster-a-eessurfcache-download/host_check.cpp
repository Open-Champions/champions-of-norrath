// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <cstdio>
#include <initializer_list>
extern "C" { int norrath_eessurfcache_key __asm__("_13VIEESurfCache$Key"); }
static int calls, result; static unsigned char *s1, *s2, *s3, *s4;
extern "C" int ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(unsigned char* a, unsigned char* b, unsigned char* c, unsigned char* d) {
    ++calls; s1 = a; s2 = b; s3 = c; s4 = d; return result;
}
#include "download.cpp"
int main() {
    bool ok = true;
    unsigned char surface[8], wp[8];
    unsigned char* self = arena;
    unsigned char* info = arena + 0x100;
    for (int r : {0, 5, -1}) {
        norrath_eessurfcache_key = 41; calls = 0; result = r;
        *reinterpret_cast<int*>(self + 28) = 7; *reinterpret_cast<int*>(info + 80) = 3;
        int v = Download__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag(self, surface, info, wp);
        ok = ok && calls == 1 && s1 == self && s2 == surface && s3 == info && s4 == wp;
        if (r < 0) ok = ok && v == -1 && norrath_eessurfcache_key == 41 && *reinterpret_cast<int*>(self + 28) == 7 && *reinterpret_cast<int*>(info + 80) == 3;
        else ok = ok && v == 0 && norrath_eessurfcache_key == 42 && *reinterpret_cast<int*>(self + 28) == 8 && *reinterpret_cast<int*>(info + 80) == 41;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
