// Observed: matMul__FP8Matrix34P8Matrix33T0 at 0x00156230 is 276 bytes. The mangled name types the
// arguments (Matrix34*, Matrix33*, Matrix33*); the body treats the first as a twelve-float
// destination. A twelve-float temporary receives
//   t[3i+j] = a[3i]*b[j] + a[3i+1]*b[3+j] + a[3i+2]*b[6+j]   (i, j in 0..2, summed left to right)
//   t[9..11] = b[9..11]
// then 48 bytes are copied to dst and dst is returned, so dst may alias an operand.
// Observed: the second source is read at floats 9 to 11, past a nine-float Matrix33, so the object
// passed there has at least twelve floats.
// Inferred: a rotation product whose second operand's translation part is carried through.
// Unknown: the exact Matrix34 definition; the name Matrix33 for b is the mangled one.
extern "C" {
float* matMul__FP8Matrix34P8Matrix33T0(float* dst, const float* a, const float* b) {
    float t[12];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            t[3 * i + j] = a[3 * i] * b[j] + a[3 * i + 1] * b[3 + j] + a[3 * i + 2] * b[6 + j];
    t[9] = b[9];
    t[10] = b[10];
    t[11] = b[11];
    for (int k = 0; k < 12; ++k) dst[k] = t[k];
    return dst;
}
}
