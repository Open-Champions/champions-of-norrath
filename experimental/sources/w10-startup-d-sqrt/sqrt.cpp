// Observed sqrt at 0x002f1658, 20 bytes. It forwards its double argument unchanged to
// __ieee754_sqrt (0x002f4230) with a tail jump and returns that result; there is no domain
// check or error handling in this image.
// Nothing here was executed on the EE.
extern "C" {
double __ieee754_sqrt(double x);

double sqrt(double x) { return __ieee754_sqrt(x); }
}
