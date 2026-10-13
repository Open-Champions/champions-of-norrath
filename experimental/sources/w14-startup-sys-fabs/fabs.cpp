// Observed fabs at 0x002f7e50, 52 bytes, in the double-precision part of the math library. The image has no
// floating-point hardware for doubles: a double travels as its 64-bit IEEE pattern in one general register, as
// in the soft-double helpers (dpadd, dpmul and the rest). The routine clears bit 63 of that pattern and
// returns it, so every NaN keeps its payload and negative zero becomes positive zero.
// Callers: __ieee754_rem_pio2 (call at 0x002f3e50), which declares it as taking and returning the 64-bit pattern.
extern "C" {
unsigned long long fabs(unsigned long long x) {
    return x & 0x7fffffffffffffffull;
}
}
