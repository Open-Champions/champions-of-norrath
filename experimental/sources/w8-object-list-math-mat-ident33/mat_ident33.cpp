// Observed matIdent__FP8Matrix33 at 0x159090, extent 48. Writes nine floats:
// 1.0 at word offsets 0, 4 and 8 of the diagonal (byte offsets 0, 16, 32) and
// 0.0 elsewhere. Stores happen in a fixed order but no reader can tell.
struct Matrix33 {
    float m[9];
};

extern "C" void matIdent__FP8Matrix33(Matrix33* matrix) {
    matrix->m[0] = 1.0f;
    matrix->m[1] = 0.0f;
    matrix->m[2] = 0.0f;
    matrix->m[3] = 0.0f;
    matrix->m[4] = 1.0f;
    matrix->m[5] = 0.0f;
    matrix->m[6] = 0.0f;
    matrix->m[7] = 0.0f;
    matrix->m[8] = 1.0f;
}
