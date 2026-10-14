// Observed: Mul__C10VIMatrix44RC7VIVect3R7VIVect3 at 0x010d09a8 is 136 bytes. It builds
// a 16-byte temporary (x, y, z, 1.0f) from the three floats at a1 and a second 16-byte
// temporary that is zero-filled, loads the four matrix rows (a0) and the first
// temporary with lqc2, runs the same vmulax/vmadday/vmaddaz/vmaddw sequence as the
// four-component multiply, stores the result in the second temporary, and writes its
// first three floats to a2 (stores in the order x, z, y). Nothing is returned.
// Inferred: a point (w = 1) transformed by the matrix as a row vector; the fourth
// result lane is discarded. Unknown: the third argument may alias the input; the
// original reads all three input floats before writing any output, as does this version.
// The host build models the sum with plain float arithmetic.
extern "C" {
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const float* matrix, const float* in, float* out) {
    alignas(16) float source[4] = {in[0], in[1], in[2], 1.0f};
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
