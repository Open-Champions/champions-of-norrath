#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
struct Point3 { float x, y, z; };
extern "C" {
unsigned char bolt[20 * 1844];
int frameNm;
void P_MoveLightning__FiG6Point3T1ii(int handle, Point3* a, Point3* b, int c, int w);
}
int main() {
    unsigned char* trailA = arena_at(0x1000);
    unsigned char* trailB = arena_at(0x3000);
    Point3 a = {1.5f, 2.5f, 3.5f}, b = {-4.0f, 5.0f, -6.0f};
    const int handle = 0x0307;  // record 7
    unsigned char* rec = bolt + 7 * 1844;
    auto arm = [&] {
        std::memset(bolt, 0, sizeof bolt);
        std::memset(arena_at(0x1000), 0xaa, 0x4000);
        sw_(rec + 1840, handle); rec[1836] = 1;
        sw_(rec + 1824, norrath_host_address(trailA));
        sw_(rec + 1828, norrath_host_address(trailB));
        frameNm = 1000;
    };
    unsigned char before[sizeof bolt];
    // Success: stores happen, width byte only when w >= 0.
    arm();
    P_MoveLightning__FiG6Point3T1ii(handle, &a, &b, 0x1ff, 41);
    if (hw(rec + 1832) != 1002) return 1;
    if (lf_(rec + 1792) != 1.5f || lf_(rec + 1796) != 2.5f || lf_(rec + 1800) != 3.5f) return 2;
    if (lf_(rec + 1804) != -4.0f || lf_(rec + 1812) != -6.0f) return 3;
    if (trailA[1037] != 0xff || trailB[1037] != 0xff) return 4;
    if (trailA[1039] != 10 || trailB[1039] != 10) return 5;
    if (trailA[1036] != 0xaa || trailA[1038] != 0xaa || trailA[1040] != 0xaa) return 6;
    // Negative w leaves byte 1039 alone.
    arm();
    P_MoveLightning__FiG6Point3T1ii(handle, &a, &b, 3, -1);
    if (trailA[1037] != 3 || trailA[1039] != 0xaa || trailB[1039] != 0xaa) return 7;
    // Each guard stops everything: no store anywhere.
    for (int g = 0; g != 6; ++g) {
        arm();
        int h = handle;
        if (g == 0) h = 0;
        if (g == 1) h = handle + 0x100;      // same record, word mismatch
        if (g == 2) rec[1836] = 0;
        if (g == 3) sw_(rec + 1824, 0);
        if (g == 4) sw_(rec + 1828, 0);
        if (g == 5) rec[1836] = 0x80;        // signed byte, nonzero: must still run
        std::memcpy(before, bolt, sizeof bolt);
        P_MoveLightning__FiG6Point3T1ii(h == handle + 0x100 ? 0x0307 + 0x100 : h, &a, &b, 9, 9);
        if (g == 5) { if (hw(rec + 1832) != 1002) return 8; continue; }
        if (g == 1) { /* handle 0x407 selects record 7 with a mismatching word */ }
        if (std::memcmp(before, bolt, sizeof bolt) != 0) { std::printf("guard %d\n", g); return 9; }
        if (trailA[1037] != 0xaa || trailB[1037] != 0xaa) return 10;
    }
    std::puts("P_MoveLightning contract passed");
    return 0;
}
