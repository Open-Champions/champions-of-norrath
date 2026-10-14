// Observed: Transpose__10VIMatrix44RC10VIMatrix44 at 0x0109b180, 88 bytes, returns nothing. With D the matrix being written and S the matrix argument
// (both 16 consecutive 32-bit floats, row-major), it copies S[row*4 + column] to D[column*4 + row] for all sixteen elements, one
// 32-bit float load/store each, with no test for D and S being the same object.
// Inferred: a plain 4x4 transpose of a matrix that is not the destination; with D == S the lower triangle would be overwritten by values already moved.
// Unknown: whether any caller passes the same matrix twice.
extern "C" {
void Transpose__10VIMatrix44RC10VIMatrix44(unsigned char* self, const unsigned char* source) {
    float* to = reinterpret_cast<float*>(self);
    const float* from = reinterpret_cast<const float*>(source);
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            to[column * 4 + row] = from[row * 4 + column];
        }
    }
}
}
