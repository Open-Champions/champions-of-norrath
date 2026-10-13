// Observed: Mul__C10VIMatrix44RC7VIVect4R7VIVect3 at 0x010d08f0, 96 bytes. It zero-fills a
// 16-byte stack temporary, loads the four matrix rows (a0) and the four floats of the
// input (a1) into VU0 registers with lqc2, runs vmulax/vmadday/vmaddaz/vmaddw, stores the
// four-lane result in the temporary, and writes its first three floats to a2 (stores in
// the order x, z, y). Nothing is returned.
// Inferred: a four-component row vector times the matrix with the fourth lane dropped;
// the sibling Vect4 -> Vect4 and Vect3 -> Vect3 forms use the same sequence. The host
// build models the sum with plain float arithmetic, which does not claim VU0 rounding.
// Unknown: member names and whether the output may alias the input (the input is read
// completely before the output is written).
extern "C" {
void Mul__C10VIMatrix44RC7VIVect4R7VIVect3(const float* matrix, const float* in, float* out) {
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
        : "r"(matrix), "r"(in), "r"(result)
        : "memory");
#else
    for (int i = 0; i < 4; ++i) {
        float acc = matrix[i] * in[0];
        acc = acc + matrix[4 + i] * in[1];
        acc = acc + matrix[8 + i] * in[2];
        result[i] = acc + matrix[12 + i] * in[3];
    }
#endif
    float x = result[0], y = result[1], z = result[2];
    out[0] = x;
    out[2] = z;
    out[1] = y;
}
}
