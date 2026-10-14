// Observed: SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3 at 0x010d0a30 is 128 bytes in the pinned SLUS-20565 image. It is
// Mul__C10VIMatrix44RC7VIVect3R7VIVect3 (see the w12-vimath-a package) with the fourth lane of the source set to 0.0f instead of
// 1.0f: a 16-byte temporary (x, y, z, 0.0f) is built from the three floats at a1 and a zero-filled second temporary receives the
// vmulax/vmadday/vmaddaz/vmaddw sum of the four matrix rows (a0) weighted by x, y, z and w. The first three results are written to
// a2 in the order x, z, y. Nothing is returned.
// Inferred: transforms a direction by the upper 3x3 part of the matrix, ignoring the translation row (w = 0), treating the vector
// as a row vector. The sum is ((x*row0 + y*row1) + z*row2) + 0*row3 lane by lane.
// Unknown: the third argument may alias the input; every input float is read before any output is written, as in the original.
// The host build models the sum with plain float arithmetic; hardware VU0 rounding is not modelled. Nothing here ran on the EE.
extern "C" {
void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const float* matrix, const float* in, float* out) {
    alignas(16) float source[4] = {in[0], in[1], in[2], 0.0f};
    alignas(16) float result[4] = {0.0f, 0.0f, 0.0f, 0.0f};
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
        : "r"(matrix), "r"(source), "r"(result)
        : "memory");
#else
    for (int i = 0; i < 4; ++i) {
        float acc = matrix[i] * source[0];
        acc = acc + matrix[4 + i] * source[1];
        acc = acc + matrix[8 + i] * source[2];
        result[i] = acc + matrix[12 + i] * source[3];
    }
#endif
    float x = result[0], y = result[1], z = result[2];
    out[0] = x;
    out[2] = z;
    out[1] = y;
}
}
