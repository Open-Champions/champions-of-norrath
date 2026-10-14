// Observed: SwapRows__10VIMatrix44ii at 0x0109b1d8 is 92 bytes. The object holds rows of
// four floats at a 16-byte stride; the two row numbers (a1, a2) are scaled by 16 and the
// four floats of each row are exchanged. Row a1 is read and written first, then row a2.
// Nothing is returned and no bound check is made on the row numbers.
// Inferred: a 4x4 float matrix helper for the Gauss-Jordan Invert; swapping a row with
// itself is harmless. Unknown: whether the original declared a return type.
extern "C" {
void SwapRows__10VIMatrix44ii(float* matrix, int row_a, int row_b) {
    float* a = matrix + row_a * 4;
    float* b = matrix + row_b * 4;
    for (int i = 0; i < 4; ++i) {
        float t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}
}
