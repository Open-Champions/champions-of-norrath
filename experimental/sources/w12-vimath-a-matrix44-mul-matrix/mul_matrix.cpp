// Observed: Mul__10VIMatrix44RC10VIMatrix44T1 at 0x010d0848 is 120 bytes. Arguments:
// the destination (a0) and two source matrices a1 and a2, each 16 floats as four
// 16-byte rows. The four rows of a2 are loaded into VU0 registers; for each row r of
// a1 the sequence vmulax/vmadday/vmaddaz/vmaddw yields
//   ((a2.row0*r.x + a2.row1*r.y) + a2.row2*r.z) + a2.row3*r.w
// which is stored as the matching row of the destination. Rows of a1 are loaded one
// at a time as the results are produced, and the destination row is stored before the
// next-but-one row of a1 is loaded. Nothing is returned.
// Inferred: dest = a1 * a2 for row vectors. Every row of a1 is loaded before the store
// that could overwrite it (row 3 is loaded before the row 1 store), so dest may equal
// a1 or a2; the inline assembly keeps the observed instruction order so that holds here
// too. The host build copies both sources first and models the sums with plain floats.
// Unknown: whether the original source used inline assembly or a library routine.
extern "C" {
void Mul__10VIMatrix44RC10VIMatrix44T1(float* dest, const float* left, const float* right) {
#if defined(__mips__)
    __asm__ volatile(
        "lqc2 $vf8,0(%1)\n\t"
        "lqc2 $vf4,0(%2)\n\t"
        "lqc2 $vf5,16(%2)\n\t"
        "lqc2 $vf6,32(%2)\n\t"
        "lqc2 $vf7,48(%2)\n\t"
        "lqc2 $vf9,16(%1)\n\t"
        "vmulax.xyzw $ACC,$vf4,$vf8x\n\t"
        "vmadday.xyzw $ACC,$vf5,$vf8y\n\t"
        "vmaddaz.xyzw $ACC,$vf6,$vf8z\n\t"
        "vmaddw.xyzw $vf12,$vf7,$vf8w\n\t"
        "lqc2 $vf10,32(%1)\n\t"
        "vmulax.xyzw $ACC,$vf4,$vf9x\n\t"
        "vmadday.xyzw $ACC,$vf5,$vf9y\n\t"
        "vmaddaz.xyzw $ACC,$vf6,$vf9z\n\t"
        "vmaddw.xyzw $vf13,$vf7,$vf9w\n\t"
        "sqc2 $vf12,0(%0)\n\t"
        "lqc2 $vf11,48(%1)\n\t"
        "vmulax.xyzw $ACC,$vf4,$vf10x\n\t"
        "vmadday.xyzw $ACC,$vf5,$vf10y\n\t"
        "vmaddaz.xyzw $ACC,$vf6,$vf10z\n\t"
        "vmaddw.xyzw $vf14,$vf7,$vf10w\n\t"
        "sqc2 $vf13,16(%0)\n\t"
        "vmulax.xyzw $ACC,$vf4,$vf11x\n\t"
        "vmadday.xyzw $ACC,$vf5,$vf11y\n\t"
        "vmaddaz.xyzw $ACC,$vf6,$vf11z\n\t"
        "vmaddw.xyzw $vf15,$vf7,$vf11w\n\t"
        "sqc2 $vf14,32(%0)\n\t"
        "sqc2 $vf15,48(%0)"
        :
        : "r"(dest), "r"(left), "r"(right)
        : "memory");
#else
    float l[16], rt[16], out[16];
    for (int i = 0; i < 16; ++i) { l[i] = left[i]; rt[i] = right[i]; }
    for (int row = 0; row < 4; ++row)
        for (int i = 0; i < 4; ++i) {
            float acc = rt[i] * l[4 * row];
            acc = acc + rt[4 + i] * l[4 * row + 1];
            acc = acc + rt[8 + i] * l[4 * row + 2];
            out[4 * row + i] = acc + rt[12 + i] * l[4 * row + 3];
        }
    for (int i = 0; i < 16; ++i) dest[i] = out[i];
#endif
}
}
