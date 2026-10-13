// Host contract for initPosition__Fii. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include <cstdio>
#include <cstring>
#include "init_position_hermite.cpp"
Point3 hvecs[12], hpos[12], p1, p2, v1, v2;
short pitch1, pitch2;
int src, dst;
int main() {
    for (int i = 0; i != 12; ++i) {
        hvecs[i] = {1.f + i, -2.f * i, 0.25f * i - 1};
        hpos[i] = {100.f + i, 200.f - i, 300.f + 2 * i};
    }
    pitch1 = pitch2 = 7; src = dst = -9;
    initPosition__Fii(3, 8);
    if (v1.x != 4.f * 15 || v1.y != -6.f * 15 || v1.z != (0.75f - 1) * 15) return 1;
    if (v2.x != 9.f * 15 || v2.y != -16.f * 15 || v2.z != (2.f - 1) * 15) return 2;
    if (p1.x != 103.f || p1.y != 197.f || p1.z != 306.f + 61.f) return 3;
    if (p2.x != 108.f || p2.y != 192.f || p2.z != 316.f + 61.f) return 4;
    if (pitch1 != 1812 || pitch2 != 1812 || src != 3 || dst != 8) return 5;
    // Source tables are not modified.
    if (hvecs[3].x != 4.f || hpos[8].z != 316.f) return 7;
    // Same index twice, and index zero.
    initPosition__Fii(0, 0);
    if (v1.x != 15.f || v2.x != 15.f || p1.z != 300.f + 61.f || p2.z != 300.f + 61.f || src != 0 || dst != 0) return 8;
    initPosition__Fii(11, 2);
    if (src != 11 || dst != 2 || p1.x != 111.f || p2.x != 102.f) return 9;
    std::puts("initPosition (hermite) check passed");
    return 0;
}
