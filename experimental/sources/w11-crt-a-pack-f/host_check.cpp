// Host contract for __pack_f. Not an EE plan input. Checks the packed bit patterns against an
// independent restatement of the rules and a few hand-worked values.
#include <cstdio>
#include <cstring>
#include "pack_f.cpp"
static unsigned bits(float f) { unsigned b; std::memcpy(&b, &f, 4); return b; }
static unsigned pack(int cls, int sign, int power, unsigned fraction) {
    W11UnpackedSingle r = {cls, sign, power, fraction};
    return bits(__pack_f(&r));
}
int main() {
    int fail = 0;
    // 1.0 : class 3, power 0, fraction 1<<30; 1.5 : fraction 0x60000000; -2.0 : sign 1, power 1
    if (pack(3, 0, 0, 0x40000000u) != 0x3f800000u) ++fail;
    if (pack(3, 0, 0, 0x60000000u) != 0x3fc00000u) ++fail;
    if (pack(3, 1, 1, 0x40000000u) != 0xc0000000u) ++fail;
    if (pack(3, 0, -126, 0x40000000u) != 0x00800000u) ++fail;
    if (pack(3, 0, -127, 0x40000000u) != 0u) ++fail;                 // flushed
    if (pack(3, 1, -127, 0x40000000u) != 0x80000000u) ++fail;        // flushed, keeps sign
    if (pack(3, 0, 127, 0x7fffffffu) != 0x7f7fffffu) ++fail;         // truncation, no rounding
    if (pack(3, 0, 128, 0x40000000u) != 0x7f800000u) ++fail;         // 128: exponent 255, frac bits
    if (pack(3, 0, 128, 0x40000080u) != 0x7f800001u) ++fail;
    if (pack(3, 0, 128, 0x80000000u) != 0x7f800000u) ++fail;         // top bit set: fraction 0
    if (pack(3, 1, 129, 0x40000000u) != 0xff800000u) ++fail;
    if (pack(3, 0, 5000, 0x7fffffffu) != 0x7f800000u) ++fail;
    if (pack(2, 0, 3, 0x40000000u) != 0u) ++fail;                    // class 2 is zero
    if (pack(2, 1, 3, 0x40000000u) != 0x80000000u) ++fail;
    if (pack(3, 0, 3, 0u) != 0u) ++fail;                             // zero fraction
    if (pack(3, 3, 0, 0x40000000u) != 0xbf800000u) ++fail;           // only sign bit 0 used? bit0 of 3 is 1
    if (pack(4, 0, 0, 0x40000000u) != 0x3f800000u) ++fail;           // other classes are not special
    if (pack(0, 0, 0, 0x40000000u) != 0x3f800000u) ++fail;
    if (pack(3, 0, 0, 0x7fffffffu) != 0x3fffffffu) ++fail;
    if (pack(3, 2, 0, 0x40000000u) != 0x3f800000u) ++fail;           // even sign word: positive
    if (fail) { std::printf("pack_f: %d failures\n", fail); return 1; }
    std::printf("pack_f cases passed\n");
    return 0;
}
