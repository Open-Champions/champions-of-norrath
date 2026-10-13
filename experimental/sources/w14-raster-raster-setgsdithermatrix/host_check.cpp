// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "set_dither.cpp"
#include <cstdio>
#include <cstring>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    unsigned char raster[0x4600] = {};
    unsigned pointer = arena_base + 0x200;
    memset(arena + 0x200, 0xaa, 96);
    *reinterpret_cast<int*>(raster + 17820) = 0;
    // same mode as cached: nothing written
    bool ok = SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(raster, 0, &pointer) == 0 && pointer == arena_base + 0x200 && arena[0x200] == 0xaa;
    ok = ok && SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(raster, 1, &pointer) == 0 && pointer == arena_base + 0x240 && *reinterpret_cast<int*>(raster + 17820) == 1;
    const unsigned char* p = arena + 0x200;
    ok = ok && p[0] == 3 && p[1] == 0 && p[2] == 0xaa && p[3] == 0x10 && W(0x204) == 0 && W(0x208) == 0xaaaaaaaau && W(0x20c) == 0xaaaaaaaau;
    ok = ok && W(0x210) == 0 && W(0x214) == 0 && W(0x218) == 0x10000000u && W(0x21c) == 0x50000002u;
    unsigned long long q[6];
    memcpy(q, p + 32, sizeof q);
    ok = ok && q[0] == 0x1000000000008001ull && q[1] == 14 && q[2] == 0x6071243571603524ull && q[3] == 0x44 && p[64] == 0xaa;
    // mode 2 uses the other matrix
    ok = ok && SetGSDitherMatrix__8VIRasteriPP10_sceDmaTag(raster, 2, &pointer) == 0 && pointer == arena_base + 0x280;
    memcpy(q, arena + 0x240 + 48, 16);
    ok = ok && q[0] == 0x2031312020313120ull && q[1] == 0x44;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
