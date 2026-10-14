// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int nseq, kind[40], aint[40]; static unsigned char* aptr[40];
static int find_script[4], nfind, dealloc_ret, remove_ret, erase_ret;
static int key_value = 6; static unsigned char surfaces[16][32];
extern "C" {
int FindFree__13VIEESurfCacheR13VIEESurfAlloc(unsigned char*, unsigned char*) { kind[nseq++] = 0; return find_script[nfind++]; }
int Deallocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char*, unsigned char* a) { aptr[nseq] = a; kind[nseq++] = 1; return dealloc_ret; }
int RemoveFromLRU__13VIEESurfCachei(unsigned char*, int i) { aint[nseq] = i; kind[nseq++] = 2; return remove_ret; }
const int* Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char*, int i) { aint[nseq] = i; kind[nseq++] = 3; return &key_value; }
unsigned char* Surface__8VIRasteri(unsigned char*, int i) { aint[nseq] = i; kind[nseq++] = 4; return surfaces[i]; }
int Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char*, int i) { aint[nseq] = i; kind[nseq++] = 5; return erase_ret; }
}
#include "find_lru.cpp"
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    W(64) = arena_base + 0x6000;
    unsigned char* e3 = arena + 0x6000 + 104 * 3;
    for (int busy = 0; busy < 2; ++busy) {
        // immediate hit
        std::memset(surfaces, 0, sizeof surfaces); nseq = nfind = 0; find_script[0] = 7; W(8) = 3;
        ok = ok && FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(arena, nullptr, nullptr) == 7 && nseq == 1;
        // empty list: -1
        nseq = nfind = 0; find_script[0] = 0; W(8) = 0xffffffffu;
        ok = ok && FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(arena, nullptr, nullptr) == -1 && nseq == 1;
        // one eviction then found
        std::memset(arena + 0x6000, 0, 800); *reinterpret_cast<int*>(e3 + 28) = busy;
        nseq = nfind = 0; find_script[0] = 0; find_script[1] = 2; W(8) = 3; dealloc_ret = 0; remove_ret = 0; erase_ret = 0;
        int v = FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(arena, nullptr, nullptr);
        int expect_calls = 1 + busy + 1 + 1 + 1 + 1 + 1 + 1;
        ok = ok && v == 2 && nseq == expect_calls && surfaces[6][24] == 0xff && surfaces[6][27] == 0xff;
        int k = 1;
        if (busy) { ok = ok && kind[k] == 1 && aptr[k] == e3 + 20; ++k; }
        ok = ok && kind[k] == 1 && aptr[k] == e3 + 36 && kind[k + 1] == 2 && aint[k + 1] == 3 && kind[k + 2] == 3 && kind[k + 3] == 4 && aint[k + 3] == 6 && kind[k + 4] == 5 && aint[k + 4] == 3;
        // errors
        for (int which = 0; which < 3; ++which) {
            nseq = nfind = 0; find_script[0] = 0; W(8) = 3; dealloc_ret = which == 0 ? -1 : 0; remove_ret = which == 1 ? -1 : 0; erase_ret = which == 2 ? -1 : 0;
            ok = ok && FindLRU__13VIEESurfCacheR13VIEESurfAllocR8VIRaster(arena, nullptr, nullptr) == -1;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
