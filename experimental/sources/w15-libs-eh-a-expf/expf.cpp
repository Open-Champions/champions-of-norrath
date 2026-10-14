// Observed expf at 0x002f16a0, 20 bytes. It is a wrapper with no argument
// checks or error reporting: it passes the float to __ieee754_expf
// (0x002f4b58) and returns that result.
extern "C" float __ieee754_expf(float x);

extern "C" float expf(float x) {
    return __ieee754_expf(x);
}
