// Observed: RotateZ__10VIMatrix44f at 0x0109adf0 is 100 bytes. With (this, angle in f12) it calls
// sinf(angle), then cosf(angle), then Identity__10VIMatrix44(this), and stores: cosine into
// floats 0 and 5, sine into float 1, and the negated sine into float 4 (byte offsets
// 0, 20, 4, 16). Nothing is returned.
// Inferred: a rotation about the Z axis in a row-major 16-float matrix, angle in radians (the
// arguments go straight to sinf/cosf); which handedness callers expect is not proven here.
// Unknown: whether callers depend on a returned register.
extern "C" {
float sinf(float angle);
float cosf(float angle);
void Identity__10VIMatrix44(void* self);

void RotateZ__10VIMatrix44f(void* self, float angle) {
    float* m = static_cast<float*>(self);
    float s = sinf(angle);
    float c = cosf(angle);
    Identity__10VIMatrix44(self);
    m[0] = c;
    m[5] = c;
    m[1] = s;
    m[4] = -s;
}
}
