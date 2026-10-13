// Observed: Zero__10VIMatrix33 at 0x0108ae78 is 52 bytes of straight-line code. It stores
// 0.0f (moved from the zero register) at word offsets 0..8 of the object (byte offsets 0 to
// 32) and reads nothing. The original leaves this+12 in v0 as an incidental address
// register; it sets no value of its own.
// Inferred: nine consecutive floats are a 3x3 matrix in 12-byte rows, matching the diagonal
// stores of Identity__10VIMatrix33. The authored version returns void.
// Unknown: whether the original declared a return type.
extern "C" {
void Zero__10VIMatrix33(float* matrix) {
    for (int i = 0; i < 9; ++i) matrix[i] = 0.0f;
}
}
