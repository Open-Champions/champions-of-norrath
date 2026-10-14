// Observed: _vfprintf_r at 0x306ef4 passes the same saved 64-bit GPR to isnan
// at 0x2f0db0. The body returns 1 when the sign-cleared high word, with bit 0
// set if the low word is nonzero, is strictly above 0x7ff00000. That is an
// all-ones exponent and a nonzero fraction. Infinities and finite values return 0.
// Inferred: this is the formatter import norrath_factory_double_isnan.
// Unknown: quiet versus signaling is not distinguished; the body was not executed.
static_assert(sizeof(unsigned) == 4 && sizeof(unsigned long long) == 8, "GPR widths");

extern "C" int norrath_factory_double_isnan(unsigned long long bits) {
    const unsigned low = static_cast<unsigned>(bits);
    const unsigned high = static_cast<unsigned>(bits >> 32);
    const unsigned low_nonzero = (low | (0u - low)) >> 31;
    const unsigned combined = (high & 0x7fffffffu) | low_nonzero;
    const unsigned difference = 0x7ff00000u - combined;
    return static_cast<int>(difference >> 31);
}
