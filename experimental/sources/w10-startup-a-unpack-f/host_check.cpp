// Host comparison only. Not an EE build input. Checks the record against hand-computed values.
#include <cstdio>
#include "unpack_f.cpp"
static int failures;
static void expect(unsigned bits, int cls, int sign, int power, unsigned fraction, bool full) {
    W10UnpackedSingle r = {-9, -9, -9, 0xdeadbeefu};
    __unpack_f(&bits, &r);
    bool ok = r.cls == cls && r.sign == sign;
    if (full) ok = ok && r.power == power && r.fraction == fraction;
    else ok = ok && r.power == -9 && r.fraction == 0xdeadbeefu;
    if (!ok) { std::printf("bits %08x wrong: %d %d %d %08x\n", bits, r.cls, r.sign, r.power, r.fraction); ++failures; }
}
int main() {
    expect(0x3f800000u, 3, 0, 0, 0x40000000u, true);
    expect(0xc0000000u, 3, 1, 1, 0x40000000u, true);
    expect(0x3fc00000u, 3, 0, 0, 0x40000000u | (0x400000u << 7), true);
    expect(0x00800000u, 3, 0, -126, 0x40000000u, true);
    expect(0x7f800000u, 3, 0, 128, 0x40000000u, true);
    expect(0x00000000u, 2, 0, 0, 0, false);
    expect(0x80000000u, 2, 1, 0, 0, false);
    expect(0x007fffffu, 2, 0, 0, 0, false);
    expect(0x807fffffu, 2, 1, 0, 0, false);
    if (failures) return 2;
    std::printf("unpack_f cases passed\n");
    return 0;
}
