// Observed: Normalize__7VIVect4 at 0x01066378 is 120 bytes. The argument (a0) points at four
// floats x, y, z, w (16-byte aligned: it uses lqc2/sqc2). The squared length is
// ((x*x + y*y) + z*z) + w*w. If it compares equal to 0.0f the function returns 0 and writes
// nothing. Otherwise it computes 1/sqrt(squared length) with rsqrt.s (numerator 1.0f), multiplies
// all four lanes by it with a VU0 macro-mode vmulx (scale broadcast from the scalar), stores the
// 16-byte result back and returns 1.
// Inferred: the vector is normalized in place including w, and the return value tells whether that
// was possible. A NaN squared length is not equal to zero and takes the scaling path. The inline
// assembly reproduces the observed lane multiply so the hardware rounding is the VU0's; the host
// build models it with ordinary float arithmetic, which does not claim VU0 rounding.
// Unknown: the class layout beyond the four floats.
#if !defined(__mips__)
extern "C" float sqrtf(float);
#endif

extern "C" {
int Normalize__7VIVect4(float* v) {
    float x = v[0], y = v[1], z = v[2], w = v[3];
    float length_squared = x * x + y * y + z * z + w * w;
    if (length_squared == 0.0f) return 0;
#if defined(__mips__)
    const float one = 1.0f;
    float scale;
    unsigned scale_bits;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(scale) : "f"(one), "f"(length_squared));
    __asm__ volatile("mfc1 %0, %1" : "=r"(scale_bits) : "f"(scale));
    __asm__ volatile(
        "lqc2 $vf4,0(%0)\n\t"
        "qmtc2 %1,$vf5\n\t"
        "vmulx.xyzw $vf6,$vf4,$vf5x\n\t"
        "sqc2 $vf6,0(%0)"
        :
        : "r"(v), "r"(scale_bits)
        : "memory");
#else
    float scale = 1.0f / sqrtf(length_squared);
    v[0] = x * scale;
    v[1] = y * scale;
    v[2] = z * scale;
    v[3] = w * scale;
#endif
    return 1;
}
}
