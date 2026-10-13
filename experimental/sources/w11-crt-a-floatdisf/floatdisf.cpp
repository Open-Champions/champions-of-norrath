// Observed __floatdisf at 0x002f9af0, 232 bytes, pinned SLUS_205.65. Signed 64-bit integer to
// float, through the soft-double entry points litodp (0x002fba98), dpmul (0x002fb680),
// dpadd (0x002fb5c0) and dptofp (0x002fbc20, double to float).
//   1. Double-rounding guard: when the value lies outside (-2^53, 2^53) and any of its low
//      eleven bits is set, bit 11 (0x800) is ORed in, so the later rounding to double cannot
//      hide a sticky bit.
//   2. The guarded value is converted exactly as __floatdidf does (high half through litodp and
//      two multiplications by 2^16, low half through litodp with 2^32 added when negative, then
//      one dpadd).
//   3. dptofp of that double is returned.
// Unknown: the bodies of the four callees. Nothing was executed on the EE.
extern "C" {
unsigned long long litodp(int value);
unsigned long long dpmul(unsigned long long left, unsigned long long right);
unsigned long long dpadd(unsigned long long left, unsigned long long right);
float dptofp(unsigned long long value);

float __floatdisf(long long value) {
    unsigned long long bits = static_cast<unsigned long long>(value);
    if (bits + 0x1fffffffffffffull > 0x3ffffffffffffeull && (bits & 0x7ff) != 0) bits |= 0x800;
    const long long guarded = static_cast<long long>(bits);
    const unsigned long long scale16 = 0x40f0000000000000ull;
    unsigned long long scaled = dpmul(dpmul(litodp(static_cast<int>(guarded >> 32)), scale16), scale16);
    const int low_bits = static_cast<int>(guarded);
    unsigned long long low = litodp(low_bits);
    if (low_bits < 0) low = dpadd(low, 0x41f0000000000000ull);
    return dptofp(dpadd(scaled, low));
}
}
