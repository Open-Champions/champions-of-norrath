// Observed ustrcat__FPUsT0 at 0x001918d8, 96 bytes.
// It advances the first argument to its zero unit, then copies 16-bit units
// from the second argument until its first zero unit and stores a zero unit.
extern "C" void ustrcat__FPUsT0(unsigned short* dst, unsigned short* src) {
    while (*dst != 0) dst++;
    while (*src != 0) {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = 0;
}
