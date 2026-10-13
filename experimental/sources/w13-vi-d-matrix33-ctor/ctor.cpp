// Observed: __10VIMatrix33 at 0x0108ace0 is 64 bytes in the pinned SLUS-20565 image. With the object as this it stores float 0.0
// into nine consecutive floats (+0..+32, three rows of three at a 12-byte stride) and then jumps to Zero__10VIMatrix33 with the
// object unchanged. It reads nothing from the object.
// Inferred: a 3x3 float matrix stored row by row; the constructor zeroes it and then calls Zero, which in its own body also
// stores zeros (not authored here).
// Unknown: whether the original source wrote the loop explicitly or an inlined member initializer produced it; Zero__10VIMatrix33
// is an import. Nothing here ran on the EE.
extern "C" {
void Zero__10VIMatrix33(void* self);

void __10VIMatrix33(void* self) {
    float* m = static_cast<float*>(self);
    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col) m[row * 3 + col] = 0.0f;
    Zero__10VIMatrix33(self);
}
}
