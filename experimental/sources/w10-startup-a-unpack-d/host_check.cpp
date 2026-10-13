// Host comparison only. Not an EE build input. Hand-computed records for each branch.
#include <cstdio>
#include <cstddef>
#include "unpack_d.cpp"
static int failures;
static void expect(unsigned long long bits, int cls, int sign, int power, unsigned long long fraction, bool written) {
    W10UnpackedDouble r;
    r.cls = r.sign = r.power = r.unused = -9;
    r.fraction = 0xdeadbeefdeadbeefull;
    r.unused2 = 0x1122334455667788ull;
    __unpack_d(&bits, &r);
    bool ok = r.cls == cls && r.sign == sign && r.unused == -9 && r.unused2 == 0x1122334455667788ull;
    if (written) ok = ok && r.power == power && r.fraction == fraction;
    else ok = ok && r.power == -9 && r.fraction == 0xdeadbeefdeadbeefull;
    if (!ok) { std::printf("bits %016llx wrong: %d %d %d %016llx\n", bits, r.cls, r.sign, r.power, r.fraction); ++failures; }
}
int main() {
    static_assert(sizeof(W10UnpackedDouble) == 32 && offsetof(W10UnpackedDouble, fraction) == 16, "layout");
    const unsigned long long one = 1ull << 60;
    expect(0x3ff0000000000000ull, 3, 0, 0, one, true);
    expect(0xc000000000000000ull, 3, 1, 1, one, true);
    expect(0x3ff8000000000000ull, 3, 0, 0, one | (0x8000000000000ull << 8), true);
    expect(0x0010000000000000ull, 3, 0, -1022, one, true);
    expect(0x0000000000000000ull, 2, 0, 0, 0, false);
    expect(0x800fffffffffffffull, 2, 1, 0, 0, false);
    expect(0x7ff0000000000000ull, 4, 0, 1023, one, true);
    expect(0xfff0000000000000ull, 4, 1, 1023, one, true);
    expect(0x7ff8000000000000ull, 1, 0, 1023, 0x8000000000000ull, true);
    expect(0x7ff0000000000001ull, 0, 0, 1023, 1ull, true);
    expect(0xfff7ffffffffffffull, 0, 1, 1023, 0x7ffffffffffffull, true);
    if (failures) return 2;
    std::printf("unpack_d cases passed\n");
    return 0;
}
