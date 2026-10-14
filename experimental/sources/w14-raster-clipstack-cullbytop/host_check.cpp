// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "cull_by_top.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static float& F(unsigned off) { return *reinterpret_cast<float*>(arena + off); }
int main() {
    // stack object at 0: 2 entries at 0x100, planes at 0x200. Entry 1 is the top: 2 planes starting at plane index 1.
    W(0) = 2; W(8) = arena_base + 0x100; W(32) = arena_base + 0x200;
    W(0x100) = 9; W(0x104) = 0;        // entry 0 is not used
    W(0x110) = 2; W(0x114) = 1;        // top: two planes from plane 1
    float planes[4][4] = {{0, 0, 1, 1000}, {1, 0, 0, -5}, {0, 1, 0, -5}, {0, 0, 1, 1000}};
    for (int p = 0; p < 4; ++p) for (int k = 0; k < 4; ++k) F(0x200 + 16 * p + 4 * k) = planes[p][k];
    float sphere[4] = {0, 0, 0, 1};
    bool ok = CullByTop__11VIClipStackRC8VISphere(arena, sphere) == 0;       // distances -5, -5: inside
    sphere[0] = 7;  ok = ok && CullByTop__11VIClipStackRC8VISphere(arena, sphere) == 1;   // 7-5 = 2 > 1
    sphere[0] = 6;  ok = ok && CullByTop__11VIClipStackRC8VISphere(arena, sphere) == 0;   // 1 is not > 1
    sphere[0] = 0; sphere[1] = 12; ok = ok && CullByTop__11VIClipStackRC8VISphere(arena, sphere) == 1;  // second plane
    sphere[1] = 0; sphere[2] = 50;  ok = ok && CullByTop__11VIClipStackRC8VISphere(arena, sphere) == 0;  // plane 3 (z) is outside the top range
    W(0x110) = 0; sphere[0] = 100; ok = ok && CullByTop__11VIClipStackRC8VISphere(arena, sphere) == 0;   // no planes
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
