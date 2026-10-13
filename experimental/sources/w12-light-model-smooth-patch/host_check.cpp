// Host contract for smoothPatch__FP12_worldHeaderP16WaterPolysetData. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA. The expected cell values were worked out with independent
// float32 arithmetic (weights (n) * 0x3dcccccc, truncating division).
#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" void smoothPatch__FP12_worldHeaderP16WaterPolysetData(unsigned char*, unsigned char*);
static short cell(const unsigned char* grid, int row, int col) {
    short v; std::memcpy(&v, grid + 80 + row * 80 + col * 2, 2); return v;
}
static void fillGrid(unsigned char* grid, short v) {
    for (int i = 0; i < 0x1400; i += 2) std::memcpy(grid + i, &v, 2);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char* world = arena_at(0x100);
    unsigned char* d = arena_at(0x200);
    unsigned char* table = arena_at(0x1000);    // five 60-byte patch records
    unsigned char* hdr = arena_at(0x2000);      // five 0x40-byte headers
    unsigned char* grids = arena_at(0x10000);   // five 0x1400-byte grids
    sw_(world + 36, norrath_host_address(table));
    for (int p = 0; p < 5; ++p) {
        sw_(table + 60 * p, norrath_host_address(hdr + 0x40 * p));
        sw_(hdr + 0x40 * p + 8, norrath_host_address(grids + 0x1400 * p));
    }
    const short values[5] = {7, 100, 300, 200, 400};   // grid 0 is the patch itself
    for (int p = 0; p < 5; ++p) fillGrid(grids + 0x1400 * p, values[p]);
    sw_(d + 8, norrath_host_address(grids));
    // halfwords d+0..d+6 pick patches 1 (left), 2 (right), 3 (top), 4 (bottom)
    sh_(d, 1); sh_(d + 2, 2); sh_(d + 4, 3); sh_(d + 6, 4);
    smoothPatch__FP12_worldHeaderP16WaterPolysetData(world, d);
    struct { int r, c, v; } want[] = {{0, 0, 150}, {15, 0, 99}, {15, 31, 299}, {0, 15, 199}, {31, 15, 399}, {31, 31, 350},
                                      {15, 15, 0}, {5, 5, 149}, {0, 31, 249}, {31, 0, 250}, {9, 9, 29}, {22, 22, 69}};
    for (auto& w : want) {
        if (cell(grids, w.r, w.c) != w.v) { std::printf("cell %d,%d got %d want %d\n", w.r, w.c, cell(grids, w.r, w.c), w.v); ++failures; }
    }
    // second grid cleared, bytes outside both grids untouched
    unsigned char* g = grids;
    bool cleared = true;
    for (int row = 0; row < 32; ++row)
        for (int col = 0; col < 32; ++col) { short v; std::memcpy(&v, g + 2704 + row * 64 + col * 2, 2); cleared = cleared && v == 0; }
    expect(cleared, "second grid cleared");
    short pad; std::memcpy(&pad, g + 2, 2);
    expect(pad == 7, "bytes before the first row untouched");
    std::memcpy(&pad, g + 80 + 31 * 80 + 64, 2);
    expect(pad == 7, "bytes after the last row untouched");
    expect(cell(grids + 0x1400, 3, 3) == 100, "neighbour grids unchanged");
    // no neighbours: everything becomes zero
    fillGrid(grids, 55);
    sh_(d, 0xffff); sh_(d + 2, 0xffff); sh_(d + 4, 0xffff); sh_(d + 6, 0xffff);
    smoothPatch__FP12_worldHeaderP16WaterPolysetData(world, d);
    expect(cell(grids, 0, 0) == 0 && cell(grids, 31, 31) == 0 && cell(grids, 16, 16) == 0, "no neighbours gives zero");
    if (failures) return 1;
    std::printf("smoothPatch contract passed\n");
    return 0;
}
