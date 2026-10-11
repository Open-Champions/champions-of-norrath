// Observed: _vfprintf_r at 0x306eac passes one 64-bit GPR (ld into a0) to isinf
// at 0x30c470 and branches on a zero result. The body clears the sign, folds a
// nonzero low half into bit 0, and returns 1 only when that word equals
// 0x7ff00000. Both infinities therefore return 1. Finite values and NaNs return 0.
// Inferred: this is the formatter import norrath_factory_double_isinf.
// Unknown: no other call of this body was found; exception flags were not executed.
static_assert(__FLT_MANT_DIG__ == 24, "binary32 mantissa");
static_assert(sizeof(unsigned) == 4 && sizeof(unsigned long long) == 8, "GPR widths");

extern "C" int norrath_factory_double_isinf(unsigned long long bits) {
    const unsigned low = static_cast<unsigned>(bits);
    const unsigned high = static_cast<unsigned>(bits >> 32);
    const unsigned low_nonzero = (low | (0u - low)) >> 31;
    const unsigned combined = (high & 0x7fffffffu) | low_nonzero;
    const unsigned difference = 0x7ff00000u - combined;
    const unsigned difference_nonzero = (difference | (0u - difference)) >> 31;
    return static_cast<int>(1u - difference_nonzero);
}
