// Host contract for getDynamicLightNumber__FP13PolysetHeader. Not an EE plan input.
// Build with -DNORRATH_HOST_ARENA; pointer words in records are arena addresses.
#include <cstdio>
#include <cstring>
#include "../level-anim-load-common/host_arena.hpp"
#include "../level-anim-load-common/host_check.hpp"
extern "C" {
unsigned char DynamicLightArray[16 * 68];
int drawFrameNm;
unsigned getDynamicLightNumber__FP13PolysetHeader(unsigned char* ph);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char* ph = arena_at(0x100);
    unsigned char* other = arena_at(0x200);
    auto rec = [&](int i) { return DynamicLightArray + i * 68; };
    auto reset = [&] {
        std::memset(DynamicLightArray, 0, sizeof DynamicLightArray);
        std::memset(ph, 0, 64); std::memset(other, 0, 64);
        drawFrameNm = 500;
        for (int i = 0; i < 16; ++i) sw_(rec(i), 100 + i);   // all older than drawFrameNm
    };
    // Already assigned slot 5 (stored as 6): refresh the frame word only.
    reset(); sw_(ph + 4, 6u | 0x80000000u);
    unsigned n = getDynamicLightNumber__FP13PolysetHeader(ph);
    expect(n == 5, "existing slot returned");
    expect(hw(rec(5)) == 500, "existing slot frame refreshed");
    expect(hw(ph + 4) == (6u | 0x80000000u), "existing slot word unchanged");
    expect(hw(rec(0)) == 100 && hw(rec(4) + 4) == 0, "other records unchanged");
    // Unassigned: oldest record wins, the first one on ties.
    reset(); sw_(rec(9), 7); sw_(rec(12), 7);
    n = getDynamicLightNumber__FP13PolysetHeader(ph);
    expect(n == 9, "oldest record chosen, first on tie");
    expect(hw(ph + 4) == 10u, "ph slot word is n + 1");
    expect(hw(rec(9) + 4) == norrath_host_address(ph), "record owner is ph");
    expect(hw(rec(9)) == 500, "record frame refreshed");
    // No record older than drawFrameNm: record 0 is taken.
    reset(); for (int i = 0; i < 16; ++i) sw_(rec(i), 600 + i);
    sw_(ph + 4, 0x80000000u);
    n = getDynamicLightNumber__FP13PolysetHeader(ph);
    expect(n == 0, "falls back to record 0");
    expect(hw(ph + 4) == 0x80000001u, "flag bit kept");
    // Out-of-range stored number counts as unassigned.
    reset(); sw_(ph + 4, 17u); sw_(rec(3), 1);
    n = getDynamicLightNumber__FP13PolysetHeader(ph);
    expect(n == 3 && hw(ph + 4) == 4u, "slot 17 is reassigned");
    // The old owner loses its number but keeps bit 31.
    reset(); sw_(rec(2), 1); sw_(rec(2) + 4, norrath_host_address(other)); sw_(other + 4, 3u | 0x80000000u);
    n = getDynamicLightNumber__FP13PolysetHeader(ph);
    expect(n == 2 && hw(other + 4) == 0x80000000u, "old owner cleared, flag kept");
    // The claimant being its own previous owner is renumbered.
    reset(); sw_(rec(4), 1); sw_(rec(4) + 4, norrath_host_address(ph)); sw_(ph + 4, 0x80000000u | 0x7fffffffu);
    n = getDynamicLightNumber__FP13PolysetHeader(ph);
    expect(n == 4 && hw(ph + 4) == (0x80000000u | 5u), "self owner renumbered");
    if (failures) return 1;
    std::printf("getDynamicLightNumber contract passed\n");
    return 0;
}
