// Host check only; not a build input.
#include <cstring>
static int dealloc_ret, offsets_ret, alloc_ret, dma_ret, order[8], n;
extern "C" int Deallocate__9VISurface(unsigned char*) { order[n++] = 1; return dealloc_ret; }
extern "C" int CalcSurfaceOffsets__9VISurface(unsigned char*) { order[n++] = 2; return offsets_ret; }
extern "C" int Allocate__9VISurface(unsigned char*) { order[n++] = 3; return alloc_ret; }
extern "C" int SetDMAHeaders__9VISurface(unsigned char*) { order[n++] = 4; return dma_ret; }
#include "init.cpp"
#include <cstdio>
static int& I(unsigned char* s, int off) { return *reinterpret_cast<int*>(s + off); }
static int run(unsigned char* s, int depth, int d, int o, int a, int m) {
    std::memset(s, 0xee, 0x80); n = 0; dealloc_ret = d; offsets_ret = o; alloc_ret = a; dma_ret = m;
    return Init__9VISurfaceR8VIRasterii10VIBitDepthi(s, 0, 64, 32, depth, 77);
}
int main() {
    alignas(4) unsigned char s[0x80];
    bool ok = true;
    const int fmt[5] = {0, 1, 5, 2, 2}, pal[5] = {16, 256, 0, 0, 0};
    for (int d = 0; d < 5; ++d) {
        ok = ok && run(s, d, 0, 0, 0, 0) == 0 && n == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4
            && I(s, 8) == 64 && I(s, 12) == 32 && I(s, 64) == 77 && I(s, 16) == fmt[d] && I(s, 40) == pal[d] && I(s, 52) == 2;
    }
    ok = ok && run(s, 1, -1, 0, 0, 0) == -1 && n == 1 && I(s, 8) == (int)0xeeeeeeee;
    ok = ok && run(s, 5, 0, 0, 0, 0) == -1 && n == 1 && I(s, 8) == 64 && I(s, 64) == 77 && I(s, 16) == (int)0xeeeeeeee;
    ok = ok && run(s, -1, 0, 0, 0, 0) == -1 && n == 1;
    ok = ok && run(s, 2, 0, -1, 0, 0) == -1 && n == 2;
    ok = ok && run(s, 2, 0, 0, -1, 0) == -1 && n == 3;
    ok = ok && run(s, 2, 0, 0, 0, -1) == -1 && n == 4;
    ok = ok && run(s, 2, 5, 5, 5, 5) == 0 && n == 4;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
