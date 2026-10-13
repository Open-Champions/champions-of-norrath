// Host contract for P_ExtendTrail__FiG6Point3. Not an EE plan input.
#include <cstdio>
extern "C" { unsigned char trail[66560]; }
#include "p_extend_trail.cpp"
#include "../level-anim-load-common/host_check.hpp"
static unsigned char* rec(int slot) { return trail + slot * 1040; }
static void point(unsigned char* b, float x, float y, float z) { sf_(b, x); sf_(b + 4, y); sf_(b + 8, z); }
int main() {
    unsigned char pt[12];
    for (unsigned i = 0; i != sizeof trail; ++i) trail[i] = 0;
    unsigned char* r = rec(3); unsigned char* h = r + 1024;
    sh_(h, 0x0203);                     // handle with slot 3
    sh_(h + 2, 9); h[4] = 10; h[5] = 20;
    point(pt, 10.75f, -3.5f, 70000.0f / 2 + 7);
    P_ExtendTrail__FiG6Point3(0x0203, pt);
    unsigned char* q = r + 20 * 8;
    // 2x truncated toward zero: 21, -7, 70014 -> low 16 bits 0x117e
    if (q[0] != 21 || q[1] != 0 || q[2] != 0xf9 || q[3] != 0xff || q[4] != 0x7e || q[5] != 0x11) { std::printf("point bytes\n"); return 1; }
    if (q[6] != 10 || q[7] != 0) return 2;               // header halfword +2 is 9, plus 1
    if (h[5] != 21 || h[4] != 10 || h[2] != 9) return 3;
    // Wrap of the write index at 128.
    h[5] = 127; h[4] = 5;
    P_ExtendTrail__FiG6Point3(0x0203, pt);
    if (h[5] != 0) return 4;
    // Zero handle, wrong handle and a full ring are no-ops.
    unsigned char keep[sizeof trail]; for (unsigned i = 0; i != sizeof trail; ++i) keep[i] = trail[i];
    P_ExtendTrail__FiG6Point3(0, pt);
    P_ExtendTrail__FiG6Point3(0x0303, pt);              // slot 3 but different sequence
    h[5] = 5; h[4] = 6; for (unsigned i = 0; i != sizeof trail; ++i) keep[i] = trail[i];
    P_ExtendTrail__FiG6Point3(0x0203, pt);             // head + 1 == tail
    if (same("blocked", trail, keep, sizeof trail)) return 5;
    h[5] = 4; h[4] = 6; for (unsigned i = 0; i != sizeof trail; ++i) keep[i] = trail[i];
    P_ExtendTrail__FiG6Point3(0x0203, pt);             // head + 2 == tail
    if (same("blocked 2", trail, keep, sizeof trail)) return 6;
    h[5] = 3; P_ExtendTrail__FiG6Point3(0x0203, pt);   // head + 2 == 5 != 6, allowed
    if (h[5] != 4) return 7;
    // Slot comes from the low byte of the handle only; a mismatching high bit fails the compare.
    sh_(rec(63) + 1024, 0x013f); rec(63)[1024 + 5] = 0;
    P_ExtendTrail__FiG6Point3(0x013f, pt);
    if (rec(63)[1024 + 5] != 1) return 8;
    std::puts("P_ExtendTrail contract passed");
    return 0;
}
