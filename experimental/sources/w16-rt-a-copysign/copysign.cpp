// Observed copysign at 0x002f8138, 68 bytes. A double is one 64-bit value in an integer
// register. The result keeps the low word of the first argument unchanged; its high word
// is the first argument's high word with the sign bit cleared, ORed with the sign bit of
// the second argument's high word.
extern "C" {
typedef unsigned long long Dp;

Dp copysign(Dp magnitude, Dp sign_source) {
    const unsigned magnitude_high = static_cast<unsigned>(magnitude >> 32);
    const unsigned sign_high = static_cast<unsigned>(sign_source >> 32);
    const unsigned high = (magnitude_high & 0x7fffffffu) | (sign_high & 0x80000000u);
    return (magnitude & 0xffffffffull) | (static_cast<Dp>(high) << 32);
}
}
