// Observed: __ml__FRC7VIVect4RC10VIMatrix44 at 0x010d0b58 is 108 bytes in the pinned SLUS-20565 image. Arguments: a result
// buffer (the hidden return pointer for a four-float value), the vector and the matrix. A local four-float buffer is first filled
// with 0.0. The four 16-byte rows of the matrix (offsets 0, 16, 32, 48) and the vector are loaded into VU0 registers with lqc2, then
// the macro-mode sequence vmulax x, vmadday y, vmaddaz z, vmaddw w computes, per lane,
//   ((row0*x + row1*y) + row2*z) + row3*w
// and sqc2 stores the 16 bytes to the local buffer. The four floats are copied to the result buffer, which is returned.
// Inferred: the matrix acts on a row vector (v * M), as for the existing Mul__C10VIMatrix44 routine; the by-value return follows
// from the buffer being returned in v0 (the pre-fill is a default constructor of the temporary).
// Unknown: whether the original source used inline assembly or a library routine. The host build models the same sum with ordinary
// float arithmetic, which does not claim VU0 rounding. Nothing here ran on the EE.
extern "C" {
float* __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* vector, const float* matrix) {
    alignas(16) float temp[4] = {0.0f, 0.0f, 0.0f, 0.0f};
#if defined(__mips__)
    __asm__ volatile(
        "lqc2 $vf4,0(%0)\n\t"
        "lqc2 $vf5,16(%0)\n\t"
        "lqc2 $vf6,32(%0)\n\t"
        "lqc2 $vf7,48(%0)\n\t"
        "lqc2 $vf8,0(%1)\n\t"
        "vmulax.xyzw $ACC,$vf4,$vf8x\n\t"
        "vmadday.xyzw $ACC,$vf5,$vf8y\n\t"
        "vmaddaz.xyzw $ACC,$vf6,$vf8z\n\t"
        "vmaddw.xyzw $vf9,$vf7,$vf8w\n\t"
        "sqc2 $vf9,0(%2)"
        :
        : "r"(matrix), "r"(vector), "r"(temp)
        : "memory");
#else
    for (int i = 0; i < 4; ++i) {
        float acc = matrix[i] * vector[0];
        acc = acc + matrix[4 + i] * vector[1];
        acc = acc + matrix[8 + i] * vector[2];
        temp[i] = acc + matrix[12 + i] * vector[3];
    }
#endif
    result[0] = temp[0];
    result[1] = temp[1];
    result[2] = temp[2];
    result[3] = temp[3];
    return result;
}
}
