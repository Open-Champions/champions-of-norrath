// Observed: Distance__C7VIVect3RC7VIVect3 at 0x0104f3d0, 80 bytes. It reads the three
// floats at each of the two vectors (this and the argument), forms the componentwise
// differences this - other, squares and sums them in the order x, y, z, and returns the
// square root (the hardware sqrt.s) in the float result register. Nothing is written.
// Inferred: the Euclidean distance between two points. The host build uses the compiler
// square root, which does not claim the hardware rounding.
// Unknown: member names.
extern "C" {
float Distance__C7VIVect3RC7VIVect3(const float* self, const float* other) {
    float dx = self[0] - other[0];
    float dy = self[1] - other[1];
    float dz = self[2] - other[2];
    float sum = dx * dx + dy * dy + dz * dz;
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(sum));
    return result;
#else
    return __builtin_sqrtf(sum);
#endif
}
}
