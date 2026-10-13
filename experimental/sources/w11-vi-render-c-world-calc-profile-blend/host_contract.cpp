#define NORRATH_HOST_CONTRACT 1
#include "calc_profile_blend.cpp"

#include <cmath>
#include <cstring>

alignas(8) static unsigned char arena[1 << 16];
extern "C" char* norrath_host_translate(unsigned address) { return reinterpret_cast<char*>(arena) + address; }
static void put(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }

static VITerrainProfileBlend out;

static void call(float x, float y, float z) {
    VIVect3 p = {x, y, z};
    std::memset(&out, 0xff, sizeof out);
    CalcProfileBlend__7VIWorldRC7VIVect3R21VITerrainProfileBlend(reinterpret_cast<VIWorld*>(arena + 0x100), &p, &out);
}

int main() {
    // cell 4, n4 = 2 (nodes 2 units apart), n2 = 2 rows of zones, n3 = 3 columns: a 6-wide, 4-high grid of bytes
    const unsigned world = 0x100, table = 0x2000;
    std::memset(arena, 0, sizeof arena);
    put(world + 24, 2); put(world + 28, 3); put(world + 48, 2); putf(world + 32, 4.0f);
    putf(world + 36, 10.0f); putf(world + 40, 99.0f); putf(world + 44, -6.0f);
    put(world + 84, table);
    for (int i = 0; i < 24; ++i) arena[table + i] = static_cast<unsigned char>(100 + i);
    // Grid nodes sit at x = 9 + 2c and z = -7 + 2r (the origin is offset by half a zone and
    // half a cell); node (c = 3, r = 2) is at x = 15, z = -3 and its four neighbours follow.
    call(15.0f, 0.0f, -3.0f);
    if (out.profile[0] != 100 + 2 * 6 + 3 || out.profile[1] != 100 + 2 * 6 + 4 || out.profile[2] != 100 + 3 * 6 + 3 ||
        out.profile[3] != 100 + 3 * 6 + 4) return 1;
    if (out.weight[0] != 1.0f || out.weight[1] != 0.0f || out.weight[2] != 0.0f || out.weight[3] != 0.0f) return 2;
    // a quarter of the way along x and half way along z from node (c = 2, r = 1)
    call(13.5f, 0.0f, -4.0f);
    if (out.profile[0] != 100 + 1 * 6 + 2 || out.profile[1] != 100 + 1 * 6 + 3 || out.profile[2] != 100 + 2 * 6 + 2 ||
        out.profile[3] != 100 + 2 * 6 + 3) return 3;
    if (std::fabs(out.weight[0] - 0.375f) > 1e-6f || std::fabs(out.weight[1] - 0.125f) > 1e-6f ||
        std::fabs(out.weight[2] - 0.375f) > 1e-6f || std::fabs(out.weight[3] - 0.125f) > 1e-6f) return 4;
    // far outside the grid on the low side: clamps to node (0, 0) with zero fractions
    call(-100.0f, 0.0f, -100.0f);
    if (out.profile[0] != 100 || out.profile[1] != 101 || out.profile[2] != 106 || out.profile[3] != 107) return 5;
    if (out.weight[0] != 1.0f) return 6;
    // far outside on the high side: last row and column; every neighbour collapses onto it and the
    // fractions clamp to one
    call(1000.0f, 0.0f, 1000.0f);
    if (out.profile[0] != 123 || out.profile[1] != 123 || out.profile[2] != 123 || out.profile[3] != 123) return 7;
    if (out.weight[3] != 1.0f || out.weight[0] != 0.0f) return 8;
    // last column only (row 1): i1 stays, i2 and i3 move one row
    call(1000.0f, 0.0f, -5.0f);
    if (out.profile[0] != 111 || out.profile[1] != 111 || out.profile[2] != 117 || out.profile[3] != 117) return 9;
    if (out.weight[1] != 1.0f) return 10;
    // the weights always sum to one
    for (float x = 8.0f; x < 26.0f; x += 0.7f)
        for (float z = -9.0f; z < 6.0f; z += 0.9f) {
            call(x, 0.0f, z);
            float s = out.weight[0] + out.weight[1] + out.weight[2] + out.weight[3];
            if (std::fabs(s - 1.0f) > 1e-5f) return 11;
        }
    return 0;
}
