// Observed: matTranspose__FP8Matrix33T0 at 0x00159cd8 is 168 bytes. Arguments: dst, src, each nine
// floats. A nine-float temporary receives t[3i+k] = src[i+3k] for i, k in 0..2; 36 bytes are then
// copied to dst and dst is returned, so dst may equal src.
// Inferred: the transpose of a row-major 3x3 matrix.
// Unknown: the Matrix33 type definition beyond these nine floats.
extern "C" {
float* matTranspose__FP8Matrix33T0(float* dst, const float* src) {
    float t[9];
    for (int i = 0; i < 3; ++i)
        for (int k = 0; k < 3; ++k) t[3 * i + k] = src[i + 3 * k];
    for (int k = 0; k < 9; ++k) dst[k] = t[k];
    return dst;
}
}
