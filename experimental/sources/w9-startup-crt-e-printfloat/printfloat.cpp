// Observed printfloat at 0x002d6938, 360 bytes, a local routine of the original that _printf
// calls for the 'e' and 'f' conversions. The argument is one 64-bit register holding
// double-precision bits. In order, with the soft-double routines of the original (dpcmp,
// dpsub, dpmul, dpdiv) and a counter that starts at 0:
//   - a value below zero (dpcmp against zero) becomes zero minus itself and '-' is sent
//     through the function pointer _putchar (0x00444960), loaded at the time of the call;
//   - a value below 0.1 (0x3fb999999999999a) is multiplied by 10.0 and the counter lowered
//     until the value is not below 0.1. A value from 0.1 upward that is not below 1.0 is
//     divided by 10.0 and the counter raised until the value is below 1.0. The tests are the
//     same compares, so each loop test is made once more on entry;
//   - the value is multiplied by 1000000.0 (0x412e848000000000), converted by
//     __fixunsdfdi and then by the local ftoi (0x002d68a0), and kprintf prints it with "0.%d";
//   - a counter of zero or more then prints "e+%d", a negative one prints "e%d", with the
//     counter as the argument (kprintf is a tail call).
// A zero argument never leaves the first loop (0 times 10 is 0); callers skip zero.
// Names: the soft-double compare, multiply, subtract and divide are called through the
// norrath_ names that the neighbouring authored sources define for those routines.
// __fixunsdfdi and ftoi are not yet authored; they are imported under their original names.
extern "C" {
extern int (*_putchar)(int character);
int norrath_softdouble_compare_2fba48(unsigned long long left, unsigned long long right);
unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long left, unsigned long long right);
unsigned long long norrath_factory_double_subtract(unsigned long long left, unsigned long long right);
unsigned long long norrath_player_double_divide(unsigned long long left, unsigned long long right);
unsigned long long __fixunsdfdi(unsigned long long value);
int ftoi(unsigned long long value);
void kprintf(const char* format, ...);

void printfloat(unsigned long long value) {
    int exponent = 0;
    if (norrath_softdouble_compare_2fba48(value, 0ull) < 0) {
        value = norrath_factory_double_subtract(0ull, value);
        _putchar('-');
    }
    if (norrath_softdouble_compare_2fba48(value, 0x3fb999999999999aull) < 0) {
        while (norrath_softdouble_compare_2fba48(value, 0x3fb999999999999aull) < 0) {
            value = norrath_softdouble_multiply_2fb680(value, 0x4024000000000000ull);
            --exponent;
        }
    } else if (norrath_softdouble_compare_2fba48(value, 0x3ff0000000000000ull) >= 0) {
        while (norrath_softdouble_compare_2fba48(value, 0x3ff0000000000000ull) >= 0) {
            value = norrath_player_double_divide(value, 0x4024000000000000ull);
            ++exponent;
        }
    }
    value = norrath_softdouble_multiply_2fb680(value, 0x412e848000000000ull);
    kprintf("0.%d", ftoi(__fixunsdfdi(value)));
    if (exponent >= 0)
        kprintf("e+%d", exponent);
    else
        kprintf("e%d", exponent);
}
}
