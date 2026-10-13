// Host contract for worldFindPointHeight__FP12_worldHeaderG6Point3. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA; stored pointers are arena offsets.
#include <cmath>
#include <cstdio>
#include <cstring>
alignas(16) static unsigned char arena[0x8000];
unsigned char* norrath_height_arena = arena;
struct _worldHeader;
struct Point3 { float x, y, z; };
extern "C" float worldFindPointHeight__FP12_worldHeaderG6Point3(_worldHeader*, Point3);
static void put32(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static void put16(unsigned off, int v) { short s = short(v); std::memcpy(arena + off, &s, 2); }
int main() {
    // world at 0: origin (0,0), 2x2 cells (each 160 wide), table at 0x100, parts at 0x200.
    put32(8, 0); put32(12, 0); put32(16, 2); put32(20, 2);
    put32(24, 0x100); put32(32, 0x200);
    put32(0x100 + 4 * 0, 0x300);                         // cell (0,0): list at 0x300
    put32(0x100 + 4 * 1, 0x320);                         // cell (1,0): empty list
    put32(0x100 + 4 * 2, 0x320); put32(0x100 + 4 * 3, 0x320);
    put16(0x300, 1); put16(0x302, 0); put16(0x304, -1);  // parts 1 then 0
    put16(0x320, -1);
    // part 0: box 0..100, grid at 0x400, z = 10, orientation identity
    // part 1: skipped through bit 1
    for (int i = 0; i < 2; ++i) {
        unsigned p = 0x200 + 36 * i;
        put16(p + 0, 0); put16(p + 2, 0); put16(p + 4, 100); put16(p + 6, 100);
        put32(p + 12, 0x400 + 0x100 * i); put16(p + 16, i ? 2 : 0);
        put32(p + 20, 0); put32(p + 24, 0); put32(p + 28, 10);
        put16(p + 32, 32767); put16(p + 34, 0);
        unsigned g = 0x400 + 0x100 * i;
        put32(g, 0); put32(g + 4, 0); put32(g + 8, 4); put32(g + 12, 4);
        for (int k = 0; k < 16; ++k) put16(g + 20 + 2 * k, (50 + k) * 16);
    }
    auto w = reinterpret_cast<_worldHeader*>(arena);
    Point3 p = {20.0f, 41.0f, 0.0f};
    float h = worldFindPointHeight__FP12_worldHeaderG6Point3(w, p);
    if (h != 0.0f) return 1;                              // x' = 4.99 floors to column 4 of 4, outside the grid
    Point3 q = {9.0f, 3.0f, 0.0f};
    h = worldFindPointHeight__FP12_worldHeaderG6Point3(w, q);
    if (h != 10.0f + 50.0f + 2.0f) return 2;              // x' = 2.2 -> column 2, y' = 0.75 -> row 0: cell value 50+2
    Point3 outside = {-5.0f, 3.0f, 0.0f};
    if (worldFindPointHeight__FP12_worldHeaderG6Point3(w, outside) != 0.0f) return 3;
    std::printf("world find point height contract passed\n");
    return 0;
}
