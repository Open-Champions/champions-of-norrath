// Observed usprintf__FPUsT0ffffffff at 0x00190e20, 632 bytes.
// usprintf(dst, format, f0 .. f7): formats into 16-bit units. The eight float
// arguments arrive in the first eight FPU argument registers and are consumed
// in order, one per conversion; the function reads them from an array on its
// own stack and never checks that a ninth conversion has an argument.
//
// The format is read unit by unit. Units other than '%' are copied. A '%' is
// followed by one unit; the pair is always consumed as two units:
//   "%%"  stores one '%' unit;
//   'd'   formats (int)argument with sprintf("%d") into an 80-byte buffer;
//   'c'   stores (int)argument as one unit (low 16 bits);
//   's'   formats sprintf("%s", (int)argument as a char pointer) into a
//         256-byte buffer;
//   'u'   copies the zero-terminated 16-bit string at address (int)argument;
//   'f'   formats the argument widened by fptodp with sprintf("%.2f") into an
//         80-byte buffer;
//   any other unit stores nothing and consumes no argument.
// The three sprintf results are widened to units one signed byte at a time.
// The format strings "%d", "%s" and "%.2f" are at 0x00484480, 0x00484488 and
// 0x00484490. The end of the format is tested only after a unit has been
// stored and the next one read. A format that ends right after a conversion
// therefore stores a zero unit, reads one unit past its own terminator, and
// stops there only if that unit is zero. The host check builds this file with
// sprintf renamed to a stub (-Dsprintf=norrath_host_sprintf) so the double
// passed as 64 bits can be decoded on the host.
extern "C" int sprintf(char* destination, const char* format, ...);
extern "C" unsigned long long fptodp(float value);

static unsigned short* widen(unsigned short* dst, const char* text) {
    while (*text != 0) {
        *dst = static_cast<unsigned short>(static_cast<signed char>(*text));
        dst++;
        text++;
    }
    return dst;
}

extern "C" void usprintf__FPUsT0ffffffff(unsigned short* dst, unsigned short* format, float f0,
                                         float f1, float f2, float f3, float f4, float f5,
                                         float f6, float f7) {
    float arguments[8] = {f0, f1, f2, f3, f4, f5, f6, f7};
    int next = 0;
    char intText[80];
    char stringText[256];
    char floatText[80];
    unsigned c = *format;
    for (;;) {
        if (c != '%') {
            for (;;) {
                *dst = static_cast<unsigned short>(c);
                format++;
                c = *format;
                dst++;
                if (c == 0) {
                    *dst = 0;
                    return;
                }
                if (c == '%') break;
            }
        }
        unsigned spec = format[1];
        if (spec == '%') {
            *dst = '%';
            dst++;
            spec = format[1];
        }
        if (spec == 'd') {
            sprintf(intText, "%d", static_cast<int>(arguments[next++]));
            dst = widen(dst, intText);
            spec = format[1];
        }
        if (spec == 'c') {
            *dst = static_cast<unsigned short>(static_cast<int>(arguments[next++]));
            dst++;
            spec = format[1];
        }
        if (spec == 's') {
            sprintf(stringText, "%s", static_cast<int>(arguments[next++]));
            dst = widen(dst, stringText);
            spec = format[1];
        }
        if (spec == 'u') {
            const unsigned short* text =
                reinterpret_cast<const unsigned short*>(static_cast<int>(arguments[next]));
            next++;
            while (*text != 0) {
                *dst = *text;
                text++;
                dst++;
            }
            spec = format[1];
        }
        if (spec == 'f') {
            sprintf(floatText, "%.2f", fptodp(arguments[next++]));
            dst = widen(dst, floatText);
        }
        format += 2;
        c = *format;
    }
}
