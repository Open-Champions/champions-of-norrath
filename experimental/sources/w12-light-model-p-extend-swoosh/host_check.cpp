// Host contract for P_ExtendSwoosh__FiG6Point3T1f. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
extern "C" {
unsigned char trail[66560];
void P_ExtendSwoosh__FiG6Point3T1f(int, const Point3*, const Point3*, float);
}
static short h16(const unsigned char* p) { short v; std::memcpy(&v, p, 2); return v; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    auto rec = [&](int i) { return trail + 1040 * i; };
    auto arm = [&](int i, int handle, int head, int tail, int n) {
        std::memset(trail, 0x11, sizeof trail);
        unsigned char* r = rec(i);
        std::memcpy(r + 1024, &handle, 2);
        unsigned short nn = static_cast<unsigned short>(n);
        std::memcpy(r + 1026, &nn, 2);
        r[1028] = static_cast<unsigned char>(tail); r[1029] = static_cast<unsigned char>(head);
    };
    Point3 a{1.25f, -2.5f, 3.75f}, b{10.0f, 20.0f, -30.0f};
    // handle 0x103 selects record 3 and needs the whole handle in the halfword
    arm(3, 0x103, 5, 0, 7);
    P_ExtendSwoosh__FiG6Point3T1f(0x103, &a, &b, 0.5f);
    unsigned char* r = rec(3);
    expect(r[1029] == 7, "head advanced by two");
    expect(h16(r + 40) == 2 && h16(r + 42) == -5 && h16(r + 44) == 7 && h16(r + 46) == 8, "first entry: doubled a and n + 1");
    expect(h16(r + 48) == 20 && h16(r + 50) == 40 && h16(r + 52) == -60 && h16(r + 54) == 127, "second entry: doubled b and f * 255");
    expect(r[39] == 0x11 && r[56] == 0x11, "neighbouring entries untouched");
    // the whole handle must match the stored halfword
    arm(3, 0x203, 5, 0, 7);
    P_ExtendSwoosh__FiG6Point3T1f(0x103, &a, &b, 0.5f);
    expect(rec(3)[1029] == 5 && rec(3)[40] == 0x11, "stale handle ignored");
    arm(0, 0, 5, 0, 7);
    P_ExtendSwoosh__FiG6Point3T1f(0, &a, &b, 0.5f);
    expect(rec(0)[1029] == 5 && rec(0)[40] == 0x11, "zero handle ignored");
    // ring full: needs two free entries
    arm(3, 0x103, 5, 6, 0);
    P_ExtendSwoosh__FiG6Point3T1f(0x103, &a, &b, 0.5f);
    expect(rec(3)[1029] == 5, "next slot is the tail");
    arm(3, 0x103, 5, 7, 0);
    P_ExtendSwoosh__FiG6Point3T1f(0x103, &a, &b, 0.5f);
    expect(rec(3)[1029] == 5, "slot after next is the tail");
    arm(3, 0x103, 5, 8, 0);
    P_ExtendSwoosh__FiG6Point3T1f(0x103, &a, &b, 0.5f);
    expect(rec(3)[1029] == 7, "three free slots is enough");
    // wrap at 128
    arm(3, 0x103, 127, 100, 0xffff);
    P_ExtendSwoosh__FiG6Point3T1f(0x103, &a, &b, 1.0f);
    r = rec(3);
    expect(r[1029] == 1, "head wraps");
    expect(h16(r + 127 * 8 + 6) == 0, "marker wraps to zero in the first entry");
    expect(h16(r + 0) == 20 && h16(r + 2) == 40 && h16(r + 4) == -60 && h16(r + 6) == 255, "second entry lands in slot 0");
    if (failures) return 1;
    std::printf("P_ExtendSwoosh contract passed\n");
    return 0;
}
