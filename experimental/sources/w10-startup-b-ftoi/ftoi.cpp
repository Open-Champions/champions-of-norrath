/* Observed ftoi at 0x002d68a0, 148 bytes (a local routine; printfloat calls it). The
 * argument is one 64-bit register read as IEEE double-precision bits; the sign bit
 * is ignored. With e the 11-bit exponent field minus 1075:
 *   - e below -53 (a magnitude below one half) returns 0;
 *   - e of 13 or more returns 9999;
 *   - otherwise the fraction field with the implicit leading one added is a 53-bit
 *     integer. For e of 0 or more it is shifted left by e. For e below 0, with
 *     n = -e, it is shifted right by (n - 2) taken modulo 64, which keeps two
 *     fractional bits; when those two bits are both set the remaining integer is
 *     incremented, otherwise it is truncated.
 *   - the result is the low 32 bits of that value, as a signed number.
 * The two-bit rule means a fraction of three quarters or more rounds up and
 * anything lower truncates. For n = 1 the shift is 63 and the result is 0.
 * Inferred: the routine converts a double near the range printfloat produces to an
 * int with limited rounding; the bound 13 caps how far left it shifts.
 * Unknown: why the rounding point is three quarters. Compiled output is not claimed
 * to match the original.
 */
extern "C" int ftoi(unsigned long long bits) {
    const long long exponent = static_cast<long long>((bits << 1) >> 53) - 1075;
    if (exponent < -53) return 0;
    if (exponent >= 13) return 9999;
    unsigned long long value = (bits & 0x000fffffffffffffull) | 0x0010000000000000ull;
    if (exponent >= 0) {
        value <<= exponent;
    } else {
        value >>= static_cast<unsigned>(-exponent - 2) & 63u;
        value = ((value & 3u) == 3u) ? (value >> 2) + 1 : (value >> 2);
    }
    return static_cast<int>(static_cast<unsigned>(value));
}
