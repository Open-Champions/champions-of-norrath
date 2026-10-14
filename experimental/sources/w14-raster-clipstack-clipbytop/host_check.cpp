// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
// The output vertex pointer is stored as its low 32 bits, which is what the test compares.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "clip_by_top.cpp"
#include <cstdio>
#include <cstdint>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static float& F(unsigned off) { return *reinterpret_cast<float*>(arena + off); }
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
static void setup(int entryCount, int planeCount, const float* planes) {
    W(0) = entryCount; W(8) = arena_base + 0x100; W(32) = arena_base + 0x200; W(40) = arena_base + 0x1000; W(48) = arena_base + 0x1400;
    W(0x100 + 16 * (entryCount - 1)) = planeCount; W(0x100 + 16 * (entryCount - 1) + 4) = 0;
    for (int i = 0; i < planeCount * 4; ++i) F(0x200 + 4 * i) = planes[i];
}
int main() {
    const float square[12] = {0, 0, 0, 10, 0, 0, 10, 10, 0, 0, 10, 0};
    int count = -1; unsigned out = 0;
    // x <= 5 kept: (x - 5) < 0
    const float p1[4] = {1, 0, 0, -5};
    setup(1, 1, p1);
    int r = ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(arena, 4, square, &count, &out);
    const float* o = reinterpret_cast<const float*>(arena + 0x1000);
    // vertices in order: prev (0,10) is kept, cur (0,0) kept -> (0,0); (10,0): crossing -> (5,0); (10,10) removed; (0,10): crossing from (10,10)? prev
    // distance 5 > 0, current -5 < 0 -> intersection (5,10) then (0,10)
    static const float want1[12] = {0, 0, 0, 5, 0, 0, 5, 10, 0, 0, 10, 0};
    bool ok = r == 1 && count == 4 && out == low(o);
    for (int i = 0; i < 12; ++i) ok = ok && o[i] == want1[i];
    // two planes: the second pass reads the first buffer and writes the second
    const float p2[8] = {1, 0, 0, -5, 0, 1, 0, -5};
    setup(1, 2, p2);
    r = ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(arena, 4, square, &count, &out);
    ok = ok && r == 1 && count == 4 && out == low(arena + 0x1400);
    const float* o2 = reinterpret_cast<const float*>(arena + 0x1400);
    float minX = 1e9f, maxX = -1e9f, minY = 1e9f, maxY = -1e9f;
    for (int i = 0; i < count; ++i) { float x = o2[3 * i], y = o2[3 * i + 1]; if (x < minX) minX = x; if (x > maxX) maxX = x; if (y < minY) minY = y; if (y > maxY) maxY = y; }
    ok = ok && minX == 0 && maxX == 5 && minY == 0 && maxY == 5;
    // everything removed
    const float p3[4] = {1, 0, 0, 50};
    setup(1, 1, p3);
    count = 77; out = 77;
    ok = ok && ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(arena, 4, square, &count, &out) == 0 && count == 0 && out == 0;
    // no planes: the input passes through
    setup(2, 0, p3);
    ok = ok && ClipByTop__11VIClipStackiP7VIVect3RiPP7VIVect3(arena, 4, square, &count, &out) == 1 && count == 4 && out == low(square);
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
