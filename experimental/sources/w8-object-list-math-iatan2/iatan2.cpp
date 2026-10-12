// Observed iatan2__Fff at 0x15a198, extent 52. Calls the single-float atan2 on
// its two arguments in order, multiplies the result by the float constant at
// 0x47ff1c (bytes 83 f9 22 46, 10430.378, which is 65536 / (2 * pi) rounded to
// single), converts to an integer with cvt.w.s and keeps the low 16 bits as a
// signed halfword. The call at 0x2f1688 is libm atan2f; the authored provider
// for that call is norrath_native_atan2f, which this source calls so that no
// new import is opened. Inferred: the result is an angle in 65536-per-turn units.
extern "C" float norrath_native_atan2f(float y, float x);

extern "C" short iatan2__Fff(float y, float x) {
    return static_cast<short>(static_cast<int>(norrath_native_atan2f(y, x) * 10430.378f));
}
