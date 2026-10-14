/* Observed acos at 0x002f15c8, 20 bytes. It passes its argument unchanged to
 * __ieee754_acos and returns that result unchanged; the original saves and restores
 * the return address around a tail jump. No error-handling path (errno, matherr)
 * is present in the original. Compiled output is not claimed to match the original.
 */
extern "C" {
double __ieee754_acos(double x);
double acos(double x) { return __ieee754_acos(x); }
}
