// Observed usprintf__FPUsPcffffffff at 0x001919d0, 76 bytes: the overload of the 16-bit formatter whose
// format is a plain byte string. It widens the format into a 256-unit (512-byte) buffer on its own stack,
// each byte zero-extended to one 16-bit unit (an `lb` tests for the end, an `lbu` supplies the stored
// value), stores a zero unit after the last one, and calls usprintf__FPUsT0ffffffff(dst, buffer, f0..f7)
// with the eight float arguments passed through in order. No length check is made: a format of 256 or
// more bytes overruns the buffer (observed absence of a bound, not a bug claim). It returns nothing.
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dst, unsigned short* format, float f0, float f1,
                                         float f2, float f3, float f4, float f5, float f6, float f7);

extern "C" void usprintf__FPUsPcffffffff(unsigned short* dst, char* format, float f0, float f1, float f2,
                                         float f3, float f4, float f5, float f6, float f7) {
    unsigned short buffer[256];
    unsigned short* out = buffer;
    if (*format != 0) {
        do {
            *out = static_cast<unsigned char>(*format);
            format++;
            out++;
        } while (*format != 0);
    }
    *out = 0;
    usprintf__FPUsT0ffffffff(dst, buffer, f0, f1, f2, f3, f4, f5, f6, f7);
}
