/* Observed fabsf at 0x002f1368, 40 bytes. It moves the float through a stack
 * slot, clears bit 31 of the 32-bit pattern and returns the result as a float.
 * The sign bit is cleared on every input, so a NaN keeps its payload and a
 * negative zero becomes positive zero. Compiled output is not claimed to match
 * the original (the original goes through memory).
 */
extern "C" float fabsf(float x) {
    const unsigned bits = __builtin_bit_cast(unsigned, x) & 0x7fffffffu;
    return __builtin_bit_cast(float, bits);
}
