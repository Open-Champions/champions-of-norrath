// Observed matIdent__FP8Matrix34 at 0x001590c0, extent 60. Writes twelve floats
// (byte offsets 0..44): 1.0 at offsets 0, 16 and 32, and 0.0 at the other nine.
// The shape matches the Matrix34 layout used by matMakeTransYaw (three rows of
// three plus a translation row). Returns nothing.
// Inferred: identity rotation with zero translation. Unknown: nothing further.
struct Matrix34 {
    float m[12];
};

extern "C" void matIdent__FP8Matrix34(Matrix34* matrix) {
    matrix->m[11] = 0.0f;
    matrix->m[8] = 1.0f;
    matrix->m[0] = 1.0f;
    matrix->m[1] = 0.0f;
    matrix->m[2] = 0.0f;
    matrix->m[3] = 0.0f;
    matrix->m[4] = 1.0f;
    matrix->m[5] = 0.0f;
    matrix->m[6] = 0.0f;
    matrix->m[7] = 0.0f;
    matrix->m[9] = 0.0f;
    matrix->m[10] = 0.0f;
}
