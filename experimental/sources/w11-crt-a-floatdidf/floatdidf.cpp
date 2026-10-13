// Observed __floatdidf at 0x002f9a48, 168 bytes, pinned SLUS_205.65. Signed 64-bit integer to
// double, built from the soft-double entry points (doubles travel as 64-bit values):
// litodp (0x002fba98, int to double), dpmul (0x002fb680) and dpadd (0x002fb5c0).
//   1. high = litodp(value >> 32, arithmetic); scaled = dpmul(dpmul(high, 2^16), 2^16). The
//      scale is applied in two steps of 0x40f0000000000000, each one a multiplication by 2^16.
//   2. low = litodp(low 32 bits taken as a signed int). When that int is negative, 2^32
//      (0x41f0000000000000) is added to it with dpadd so it stands for the unsigned half.
//   3. The result is dpadd(scaled, low).
// Unknown: the bodies of the three callees (only their call shapes are used here).
// Nothing was executed on the EE.
extern "C" {
unsigned long long litodp(int value);
unsigned long long dpmul(unsigned long long left, unsigned long long right);
unsigned long long dpadd(unsigned long long left, unsigned long long right);

unsigned long long __floatdidf(long long value) {
    const unsigned long long scale16 = 0x40f0000000000000ull;
    unsigned long long scaled = dpmul(dpmul(litodp(static_cast<int>(value >> 32)), scale16), scale16);
    const int low_bits = static_cast<int>(value);
    unsigned long long low = litodp(low_bits);
    if (low_bits < 0) low = dpadd(low, 0x41f0000000000000ull);
    return dpadd(scaled, low);
}
}
