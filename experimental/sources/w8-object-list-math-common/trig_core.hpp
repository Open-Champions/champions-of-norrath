#pragma once
// Shared body of isin__Fi and icos__Fi. The two routines are identical after the
// angle is turned into a signed halfword: isin first subtracts 16384 (a quarter
// turn) and icos does not.
//
// Observed (0x15a070 and 0x15a100): the halfword x is folded into the range
// -16384..16383 by keeping its low 15 bits as a signed value when bits 15 and 14
// differ, and the final product is then negated (a multiplier of -1.0 instead
// of 1.0). The folded value times a scale constant gives z; an odd polynomial
//   z - c3*z^3 + c5*z^5 - c7*z^7
// is evaluated in that order, and the result is 1 - 2*p*p, which is cos(2*p')
// for the half angle p. The four constants come from the float pool at
// 0x47fefc (two identical copies of four words, one per routine): scale
// 0x38490fda (about pi / 32768), then 0x3e2aaa57, 0x3c086b24 and 0x39458017.
// Inferred: the angle unit is 65536 per turn.
static_assert(sizeof(float) == 4 && sizeof(int) == 4, "single float and 32-bit int");

namespace trig_core {
constexpr float scale = __builtin_bit_cast(float, 0x38490fdau);
constexpr float cubic = __builtin_bit_cast(float, 0x3e2aaa57u);
constexpr float quintic = __builtin_bit_cast(float, 0x3c086b24u);
constexpr float septic = __builtin_bit_cast(float, 0x39458017u);

inline float evaluate(int half_word) {
    // half_word is already sign-extended from 16 bits.
    int folded = half_word;
    float sign = 1.0f;
    if (((folded ^ (folded * 2)) & 0x8000) != 0) {
        sign = -1.0f;
        folded = static_cast<short>(static_cast<unsigned short>(folded << 1)) / 2;
    }
    float z = static_cast<float>(folded) * scale;
    const float z2 = z * z;
    const float z3 = z * z2;
    const float cubic_term = z3 * cubic;
    const float z5 = z3 * z2;
    z = z - cubic_term;
    const float quintic_term = z5 * quintic;
    const float z7 = z5 * z2;
    z = z + quintic_term;
    const float septic_term = z7 * septic;
    z = z - septic_term;
    return sign * (1.0f - (z + z) * z);
}
}  // namespace trig_core
