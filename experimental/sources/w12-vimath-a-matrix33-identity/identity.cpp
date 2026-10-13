// Observed: Identity__10VIMatrix33 at 0x0108ae40 is 56 bytes. It calls
// Zero__10VIMatrix33(this), then stores 1.0f at offsets 32, 0 and 16 of the object (in
// that order). No other bytes are written. The function does not set a return value of
// its own: what the caller sees in v0 is whatever Zero__10VIMatrix33 left there (the
// original Zero leaves this+12 in v0).
// Inferred: the matrix is nine consecutive floats (Zero stores nine floats at offsets
// 0 to 32 and the three stores above are the diagonal of a 3x3 with 12-byte rows). The
// authored version returns void, so callers that read v0 would not see the original's
// incidental value.
// Unknown: whether the original declared a return type.
extern "C" {
void Zero__10VIMatrix33(float* matrix);

void Identity__10VIMatrix33(float* matrix) {
    Zero__10VIMatrix33(matrix);
    matrix[8] = 1.0f;
    matrix[0] = 1.0f;
    matrix[4] = 1.0f;
}
}
