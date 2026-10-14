// Observed isin__Fi at 0x15a100, 148 bytes. The argument is shifted by
// -16384 (a quarter turn) and sign-extended from 16 bits, so the sine is
// computed as the cosine of the shifted angle. The rest matches icos__Fi:
// bit 15 of (shifted << 1) ^ shifted selects the -1 sign and wraps the
// value to 15 bits, then the float is scaled by norrath_isin_scale and run
// through x - cubic*x^3 + quintic*x^5 - septic*x^7 and 1 - 2*sine^2.
// Inferred: this reconstruction name is that behavior; the four
// coefficients are defined by the math package from the .lit4 pool.
// Unknown: nothing about the shift is proven beyond the 148 bytes; callers
// pass int16 yaw values and use the float directly.
extern "C" {
extern float norrath_isin_scale;
extern float norrath_isin_cubic;
extern float norrath_isin_quintic;
extern float norrath_isin_septic;
} // extern "C"

namespace {
int wrap15(int value) {
    const unsigned shifted = static_cast<unsigned>(value) << 17;
    return static_cast<int>(shifted) >> 17;
}
} // namespace

extern "C" float isin__Fi(int angle) {
    const int biased = angle - 16384;
    int reduced = static_cast<int>(static_cast<unsigned>(biased) << 16) >> 16;
    float sign = 1.0f;
    if ((((reduced << 1) ^ reduced) & 0x8000) != 0) {
        sign = -1.0f;
        reduced = wrap15(reduced);
    }
    float x = static_cast<float>(reduced) * norrath_isin_scale;
    const float square = x * x;
    const float cube = x * square;
    const float fifth = cube * square;
    float sine = x - norrath_isin_cubic * cube;
    const float seventh = fifth * square;
    sine = sine + norrath_isin_quintic * fifth;
    sine = sine - norrath_isin_septic * seventh;
    const float doubled = sine + sine;
    const float cosine = 1.0f - doubled * sine;
    return sign * cosine;
}
