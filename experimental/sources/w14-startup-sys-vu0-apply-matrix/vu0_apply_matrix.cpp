/* Observed sceVu0ApplyMatrix at 0x01348500 (overlay range, 124 bytes) of the VU0 helper library.
 * Arguments: a destination vector (16 bytes), a matrix (four 16-byte rows) and a source vector. DIntr
 * runs first. The four matrix rows and the vector are then loaded into VU0 registers, and
 *   destination = ((row0 * x + row1 * y) + row2 * z) + row3 * w
 * is formed with the multiply-to-accumulator, two multiply-add-to-accumulator and a final multiply-add
 * sequence (vmulax, vmadday, vmaddaz, vmaddw), then stored as one quadword. The routine ends by a tail
 * call to EIntr when DIntr returned nonzero. Nothing is returned.
 * The vector is loaded before the destination is written, so the destination may alias the vector.
 * Inferred: DIntr's nonzero result means interrupts were enabled beforehand (same use as in
 * sceVu0CopyMatrix); the product is the row-vector transform v * M.
 * Unknown: alignment is assumed 16 bytes (lqc2/sqc2 ignore the low four address bits, which the host
 * fallback cannot reproduce); the host fallback models the sum with plain float arithmetic.
 */
extern "C" {
int DIntr();
int EIntr();

void sceVu0ApplyMatrix(float* destination, const float* matrix, const float* vector) {
    const int was_enabled = DIntr();
#if defined(__mips__)
    __asm__ volatile(
        "lqc2 $vf4,0(%1)\n\t"
        "lqc2 $vf5,16(%1)\n\t"
        "lqc2 $vf6,32(%1)\n\t"
        "lqc2 $vf7,48(%1)\n\t"
        "lqc2 $vf8,0(%2)\n\t"
        "vmulax.xyzw $ACC,$vf4,$vf8x\n\t"
        "vmadday.xyzw $ACC,$vf5,$vf8y\n\t"
        "vmaddaz.xyzw $ACC,$vf6,$vf8z\n\t"
        "vmaddw.xyzw $vf9,$vf7,$vf8w\n\t"
        "sqc2 $vf9,0(%0)"
        :
        : "r"(destination), "r"(matrix), "r"(vector)
        : "memory");
#else
    float m[16], v[4], out[4];
    for (int i = 0; i < 16; ++i) m[i] = matrix[i];
    for (int i = 0; i < 4; ++i) v[i] = vector[i];
    for (int i = 0; i < 4; ++i) {
        float acc = m[i] * v[0];
        acc = acc + m[4 + i] * v[1];
        acc = acc + m[8 + i] * v[2];
        out[i] = acc + m[12 + i] * v[3];
    }
    for (int i = 0; i < 4; ++i) destination[i] = out[i];
#endif
    if (was_enabled) EIntr();
}
}
