// Observed: Normalize__7VIVect3 at 0x0104f2f0 is 112 bytes. The argument (a0) points at
// three floats x, y, z. The squared length is (x*x + y*y) + z*z. If it compares equal
// to 0.0f the function returns 0 and writes nothing. Otherwise it computes
// 1/sqrt(squared length) with rsqrt.s (numerator 1.0f), multiplies each component by
// it, stores the three results back, and returns 1.
// Inferred: the vector is normalized in place and the return value tells whether
// that was possible. A NaN squared length is not equal to zero and therefore takes the
// scaling path. Unknown: the class layout beyond the first 12 bytes.
#if !defined(__mips__)
extern "C" float sqrtf(float);
#endif

extern "C" {
int Normalize__7VIVect3(float* v) {
    float x = v[0], y = v[1], z = v[2];
    float length_squared = x * x + y * y + z * z;
    if (length_squared == 0.0f) return 0;
    float scale;
#if defined(__mips__)
    const float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(scale) : "f"(one), "f"(length_squared));
#else
    scale = 1.0f / sqrtf(length_squared);
#endif
    v[2] = z * scale;
    v[0] = x * scale;
    v[1] = y * scale;
    return 1;
}
}
