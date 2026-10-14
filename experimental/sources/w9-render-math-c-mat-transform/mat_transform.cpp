// Observed: the function at 0x00159880 (symbol matTransform__FP6Point3P8Matrix33T0, 160
// bytes) is what the Wave Maker constructor calls right after getWaveletMat (jal
// 0x151438 follows jal 0x151420). This file defines the placeholder name
// norrath_mat_transform that the authored Wave Maker constructor uses.
// Reads three input floats first (copying them to a local, so output may alias input)
// and computes, with the matrix as nine floats a[0..8]:
//   out[0] = a[0]*x + a[3]*y + a[6]*z
//   out[1] = a[1]*x + a[4]*y + a[7]*z
//   out[2] = a[2]*x + a[5]*y + a[8]*z
// each sum evaluated left to right. Nothing is returned.
// Inferred: Matrix33 is nine contiguous floats. Unknown: which column/row convention
// the engine's name implies.
extern "C" void norrath_mat_transform(float* out, const float* a, const float* in) {
    const float x = in[0], y = in[1], z = in[2];
    out[0] = a[0] * x + a[3] * y + a[6] * z;
    out[1] = a[1] * x + a[4] * y + a[7] * z;
    out[2] = a[2] * x + a[5] * y + a[8] * z;
}
