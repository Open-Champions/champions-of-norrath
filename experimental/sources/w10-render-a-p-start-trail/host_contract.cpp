// Host contract for P_StartTrail__Fiifiiiii. Not an EE plan input.
#include <cstdio>
extern "C" { unsigned char trail[66560]; int trailSequenceNm = 1; }
#include "p_start_trail.cpp"
#include "../level-anim-load-common/host_check.hpp"
static unsigned char* hdr(int slot) { return trail + slot * 1040 + 1024; }
int main() {
    for (unsigned i = 0; i != sizeof trail; ++i) trail[i] = 0x5c;
    for (int s = 0; s != 64; ++s) { hdr(s)[0] = 0; hdr(s)[1] = 0; }
    // First free slot is 0.
    int h = P_StartTrail__Fiifiiiii(7, -13, 0.5f, 9, 1, 2, 3, 4);
    unsigned char* p = hdr(0);
    if (h != 0x0100 || p[0] != 0x00 || p[1] != 0x01 || trailSequenceNm != 2) { std::printf("handle %x seq %d\n", h, trailSequenceNm); return 1; }
    if (p[2] || p[3] || p[4] || p[5] || p[6] != 7 || p[7] != 9) return 3;
    if (p[8] != 30 || p[9] != 0 || p[10] != 1 || p[11] != 2 || p[12] != 3 || p[13] != 4) return 4;
    if (p[14] != 0x5c || static_cast<signed char>(p[15]) != -4) return 5;   // -13 >> 2 == -4
    if (p[-1] != 0x5c) return 6;
    // Slots 0..4 busy: the next call uses slot 5 and negative sequence wraps in the halfword.
    for (int s = 1; s != 5; ++s) hdr(s)[0] = 1;
    trailSequenceNm = 0x81;
    h = P_StartTrail__Fiifiiiii(0, 0, 3.999f, 0, 0, 0, 0, 0);
    if (h != static_cast<short>(0x8105) || hdr(5)[8] != 239 || trailSequenceNm != 0x82) return 7;
    // All 64 busy: returns zero and writes nothing.
    for (int s = 0; s != 64; ++s) { hdr(s)[0] = 1; hdr(s)[1] = 0; }
    unsigned char keep[sizeof trail]; for (unsigned i = 0; i != sizeof trail; ++i) keep[i] = trail[i];
    h = P_StartTrail__Fiifiiiii(1, 1, 1.0f, 1, 1, 1, 1, 1);
    if (h != 0 || trailSequenceNm != 0x82 || same("full", trail, keep, sizeof trail)) return 8;
    // A halfword with only the high byte nonzero still counts as in use.
    hdr(40)[0] = 0; hdr(40)[1] = 0x80;
    if (P_StartTrail__Fiifiiiii(1, 1, 1.0f, 1, 1, 1, 1, 1) != 0) return 9;
    hdr(40)[1] = 0; hdr(33)[0] = 0; hdr(33)[1] = 0;
    h = P_StartTrail__Fiifiiiii(1, 1, 1.0f, 1, 1, 1, 1, 1);
    if (h != static_cast<short>((0x82 << 8) | 33)) return 10;
    // Large times truncate to 16 bits of the integer.
    hdr(0)[0] = 0; hdr(0)[1] = 0;
    P_StartTrail__Fiifiiiii(1, 1, 2000.0f, 1, 1, 1, 1, 1);        // 120000 = 0x1d4c0
    if (hdr(0)[8] != 0xc0 || hdr(0)[9] != 0xd4) return 11;
    std::puts("P_StartTrail contract passed");
    return 0;
}
