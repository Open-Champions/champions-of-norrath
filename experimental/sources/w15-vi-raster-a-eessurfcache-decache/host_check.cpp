// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <cstdio>
#include <cstring>
static int kind[8], nseq, script[8];
static unsigned char* arg_ptr[8]; static int arg_int[8];
extern "C" {
int Deallocate__13VIEESurfCacheR13VIEESurfAlloc(unsigned char* s, unsigned char* a) { arg_ptr[nseq] = a; arg_int[nseq] = s == arena; kind[nseq] = 1; return script[nseq++]; }
int RemoveFromLRU__13VIEESurfCachei(unsigned char* s, int i) { arg_ptr[nseq] = s; arg_int[nseq] = i; kind[nseq] = 2; return script[nseq++]; }
int Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi(unsigned char* m, int i) { arg_ptr[nseq] = m; arg_int[nseq] = i; kind[nseq] = 3; return script[nseq++]; }
}
#include "decache.cpp"
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    unsigned char surface[32];
    W(64) = arena_base + 0x6000;
    unsigned char* e = arena + 0x6000 + 104 * 2;
    *reinterpret_cast<int*>(surface + 24) = -1; nseq = 0;
    ok = ok && Decache__13VIEESurfCacheR9VISurface(arena, surface) == 0 && nseq == 0 && *reinterpret_cast<int*>(surface + 24) == -1;
    for (int busy = 0; busy < 2; ++busy) {
        const int total = 3 + busy;
        for (int fail = -1; fail < total; ++fail) {
            std::memset(arena + 0x6000, 0, 400);
            *reinterpret_cast<int*>(e + 28) = busy;
            *reinterpret_cast<int*>(surface + 24) = 2; nseq = 0;
            for (int k = 0; k < 8; ++k) script[k] = fail == k ? -3 : k;
            int v = Decache__13VIEESurfCacheR9VISurface(arena, surface);
            int calls = fail < 0 ? total : fail + 1;
            ok = ok && v == (fail < 0 ? 0 : -1) && nseq == calls;
            int k = 0;
            if (busy && k < nseq) { ok = ok && kind[k] == 1 && arg_ptr[k] == e + 20 && arg_int[k]; ++k; }
            if (k < nseq) { ok = ok && kind[k] == 1 && arg_ptr[k] == e + 36 && arg_int[k]; ++k; }
            if (k < nseq) { ok = ok && kind[k] == 2 && arg_int[k] == 2; ++k; }
            if (k < nseq) { ok = ok && kind[k] == 3 && arg_ptr[k] == arena + 56 && arg_int[k] == 2; ++k; }
            const bool removed = nseq == total;   // RemoveFromLRU succeeded only if the Erase call was reached
            ok = ok && *reinterpret_cast<int*>(surface + 24) == (removed || (fail == total - 1) ? -1 : 2);
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
