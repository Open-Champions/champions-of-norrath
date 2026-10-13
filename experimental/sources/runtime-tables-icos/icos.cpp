// Observed icos__Fi at 0x15a070, 144 bytes. The int argument is first
// sign-extended from 16 bits, so one full turn is 65536 units. When bit 15
// of (reduced << 1) ^ reduced is set, the angle is in the far half: the
// sign becomes -1 and the angle is wrapped to 15 bits. The wrapped value is
// converted to float and scaled by norrath_icos_scale, then evaluated as
// x - cubic*x^3 + quintic*x^5 - septic*x^7 in that order, and the result is
// 1 - 2*sine^2 times the sign, which is the cosine half-angle identity.
// Inferred: this reconstruction name is that behavior; the four
// coefficients are defined by the math package from the .lit4 pool.
// Unknown: nothing about the reduction is proven beyond the 144 bytes;
// callers pass int16 yaw values and use the float directly.
extern "C" {
extern float norrath_icos_scale;
extern float norrath_icos_cubic;
extern float norrath_icos_quintic;
extern float norrath_icos_septic;
} // extern "C"

namespace {
int wrap15(int value) {
    const unsigned shifted = static_cast<unsigned>(value) << 17;
    return static_cast<int>(shifted) >> 17;
}
} // namespace

extern "C" float icos__Fi(int angle) {
    int reduced = static_cast<int>(static_cast<unsigned>(angle) << 16) >> 16;
    float sign = 1.0f;
    if ((((reduced << 1) ^ reduced) & 0x8000) != 0) {
        sign = -1.0f;
        reduced = wrap15(reduced);
    }
    float x = static_cast<float>(reduced) * norrath_icos_scale;
    const float square = x * x;
    const float cube = x * square;
    const float fifth = cube * square;
    float sine = x - norrath_icos_cubic * cube;
    const float seventh = fifth * square;
    sine = sine + norrath_icos_quintic * fifth;
    sine = sine - norrath_icos_septic * seventh;
    const float doubled = sine + sine;
    const float cosine = 1.0f - doubled * sine;
    return sign * cosine;
}
