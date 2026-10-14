// Observed: matMul__FP8Matrix33N20 at 0x00159150 is 204 bytes. Arguments: dst, a, b, each nine
// floats. A nine-float temporary receives, for row i and column j in 0..2,
//   t[3i+j] = a[3i]*b[j] + a[3i+1]*b[3+j] + a[3i+2]*b[6+j]
// summed left to right; then 36 bytes are copied from the temporary to dst and dst is returned,
// so dst may alias either operand.
// Inferred: a row-major 3x3 product a*b.
// Unknown: the Matrix33 type definition beyond these nine floats.
extern "C" {
float* matMul__FP8Matrix33N20(float* dst, const float* a, const float* b) {
    float t[9];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            t[3 * i + j] = a[3 * i] * b[j] + a[3 * i + 1] * b[3 + j] + a[3 * i + 2] * b[6 + j];
    for (int k = 0; k < 9; ++k) dst[k] = t[k];
    return dst;
}
}
