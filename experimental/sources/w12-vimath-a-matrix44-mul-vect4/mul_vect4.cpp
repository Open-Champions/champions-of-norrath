// Observed: Mul__C10VIMatrix44RC7VIVect4R7VIVect4 at 0x010d08c0 is 48 bytes. It loads
// the four 16-byte rows of the matrix (a0, offsets 0, 16, 32, 48) and the four floats
// of the input vector (a1) into VU0 registers with lqc2, then runs the macro-mode
// sequence vmulax x, vmadday y, vmaddaz z, vmaddw w, so each output lane is
//   ((row0*x + row1*y) + row2*z) + row3*w
// per lane, and stores the 16-byte result at a2 with sqc2. It returns nothing that is
// set here. The input is read completely before the output is written.
// Inferred: the matrix acts on a row vector (v' = v * M). The four rows may hold any
// values; no divide or clamp is applied. The inline assembly reproduces the observed
// instruction sequence so the hardware rounding is the VU0's. The host build models
// the same sum with ordinary float arithmetic, which does not claim VU0 rounding.
// Unknown: whether the original source used inline assembly or a library routine.
extern "C" {
void Mul__C10VIMatrix44RC7VIVect4R7VIVect4(const float* matrix, const float* in, float* out) {
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
        : "r"(matrix), "r"(in), "r"(out)
        : "memory");
#else
    float x = in[0], y = in[1], z = in[2], w = in[3];
    float r[4];
    for (int i = 0; i < 4; ++i) {
        float acc = matrix[i] * x;
        acc = acc + matrix[4 + i] * y;
        acc = acc + matrix[8 + i] * z;
        r[i] = acc + matrix[12 + i] * w;
    }
    for (int i = 0; i < 4; ++i) out[i] = r[i];
#endif
}
}
