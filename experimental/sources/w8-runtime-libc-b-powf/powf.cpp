// Observed powf at 0x002f16d0, 20 bytes. It is a wrapper with no argument
// checks or error reporting: it passes both floats to __ieee754_powf
// (0x002f4f80) and returns that result.
extern "C" float __ieee754_powf(float base, float exponent);

extern "C" float powf(float base, float exponent) {
    return __ieee754_powf(base, exponent);
}
