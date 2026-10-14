// Observed powf at 0x2f16d0, 20 bytes: stack-prologue words around a jump
// to __ieee754_powf at 0x2f4f80, the 2016-byte mathematical body. The
// wrapper passes both floats through unchanged and returns its result.
// Inferred: this reconstruction name is that tail call; the target body is
// the already defined norrath_math_ieee754_powf, which reconstructs the
// same 2016-byte routine. The original __ieee754_powf symbol itself is not
// defined by any package.
// Unknown: the 2016-byte body is another unit's reconstruction; this unit
// only forwards to it.
extern "C" float norrath_math_ieee754_powf(float x, float y);

extern "C" float powf(float x, float y) {
    return norrath_math_ieee754_powf(x, y);
}
