/* Observed atan2f at 0x002f1688, 20 bytes. It passes its two arguments unchanged, in order, to
 * __ieee754_atan2f and returns that result unchanged; the original saves and restores
 * the return address around a tail jump. No error-handling path (errno, matherr)
 * is present in the original. Compiled output is not claimed to match the original.
 */
extern "C" {
float __ieee754_atan2f(float x, float y);
float atan2f(float x, float y) { return __ieee754_atan2f(x, y); }
}
