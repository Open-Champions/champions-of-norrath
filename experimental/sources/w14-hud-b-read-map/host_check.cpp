// Host check for readMap__3mapPUc. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// The expected map is built from the description (a 108 by 108 nibble grid doubled in each
// direction with rounded-up means, last row and column copied), not from the routine's loops.
#include "read_map.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

alignas(8) unsigned char _3map$maptiles[216 * 216 * 2];

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static int grid[108][108];
static int nibble(const unsigned char* tiles, int row, int col) { return tiles[(row * 216 + col) * 2 + 1] >> 4; }

int main() {
    std::vector<unsigned char> source(108 * 54);
    unsigned seed = 12345;
    for (auto& b : source) { seed = seed * 1103515245u + 12345u; b = static_cast<unsigned char>(seed >> 16); }
    for (int r = 0; r < 108; ++r)
        for (int c = 0; c < 108; ++c) {
            unsigned char b = source[r * 54 + c / 2];
            grid[r][c] = (c % 2 == 0) ? (b >> 4) : (b & 15);
        }
    unsigned char before[sizeof _3map$maptiles];
    for (unsigned i = 0; i < sizeof before; ++i) { seed = seed * 1103515245u + 12345u; before[i] = static_cast<unsigned char>(seed >> 16); }
    std::memcpy(_3map$maptiles, before, sizeof before);
    readMap__3mapPUc(source.data());
    // only the high nibble of byte 1 of a tile may differ
    for (unsigned i = 0; i < sizeof before; ++i) {
        unsigned char mask = (i % 2 == 1) ? 0x0f : 0xff;
        CHECK((_3map$maptiles[i] & mask) == (before[i] & mask));
    }
    auto expect = [&](int r, int c) {
        int rr = r / 2, cc = c / 2;
        if (r % 2 == 0 && c % 2 == 0) return grid[rr][cc];
        auto at = [&](int y, int x) { return grid[y > 107 ? 107 : y][x > 107 ? 107 : x]; };
        if (r % 2 == 0) { int a = at(rr, cc), b = c == 215 ? at(rr, cc) : at(rr, cc + 1); return c == 215 ? at(rr, 107) : (a + b + 1) / 2; }
        int up = r == 215 ? rr : rr, down = r == 215 ? rr : rr + 1;
        // an odd tile of an even-row pass is interpolated horizontally first, then rows are blended
        auto horizontal = [&](int y) { return c % 2 == 0 ? at(y, cc) : (c == 215 ? at(y, 107) : (at(y, cc) + at(y, cc + 1) + 1) / 2); };
        if (r == 215) return horizontal(107);
        return (horizontal(up) + horizontal(down) + 1) / 2;
    };
    for (int r = 0; r < 216; ++r)
        for (int c = 0; c < 216; ++c) CHECK(nibble(_3map$maptiles, r, c) == expect(r, c));
    // a constant source gives a constant map
    std::vector<unsigned char> flat(108 * 54, 0x77);
    readMap__3mapPUc(flat.data());
    for (int r = 0; r < 216; ++r)
        for (int c = 0; c < 216; ++c) CHECK(nibble(_3map$maptiles, r, c) == 7);
    // the rounded-up mean: neighbours 0 and 1 give 1, 14 and 15 give 15
    std::vector<unsigned char> ramp(108 * 54, 0x01);
    readMap__3mapPUc(ramp.data());
    CHECK(nibble(_3map$maptiles, 0, 0) == 0 && nibble(_3map$maptiles, 0, 2) == 1 && nibble(_3map$maptiles, 0, 1) == 1);
    std::puts("ok");
    return 0;
}
