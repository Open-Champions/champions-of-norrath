// Observed __muldi3 at 0x002f81c0, 112 bytes: the low 64 bits of a 64-bit product. The
// original forms it from 32-bit pieces (an unsigned low-by-low product and two signed
// cross products added into the high word) and so also leaves values in the multiply
// unit's HI/LO registers; only the returned value is authored here.
extern "C" {
typedef unsigned long long Bits;

Bits __muldi3(Bits left, Bits right) {
    const unsigned left_low = static_cast<unsigned>(left);
    const unsigned right_low = static_cast<unsigned>(right);
    const unsigned left_high = static_cast<unsigned>(left >> 32);
    const unsigned right_high = static_cast<unsigned>(right >> 32);
    const Bits low_product = static_cast<Bits>(left_low) * right_low;
    const unsigned high = static_cast<unsigned>(low_product >> 32) + left_low * right_high + left_high * right_low;
    return (static_cast<Bits>(high) << 32) | static_cast<unsigned>(low_product);
}
}
