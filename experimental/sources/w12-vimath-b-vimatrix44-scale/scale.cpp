// Observed: Scale__10VIMatrix44f at 0x0109ac98 is 60 bytes. With (this, s in f12) it calls
// Identity__10VIMatrix44(this), then stores s into floats 0, 5 and 10 of the matrix (byte
// offsets 0, 20, 40). Nothing is returned.
// Inferred: a uniform scale matrix over a 16-float row-major matrix; the layout order is not
// proven beyond the three stores.
// Unknown: whether callers depend on any returned register.
extern "C" {
void Identity__10VIMatrix44(void* self);

void Scale__10VIMatrix44f(void* self, float scale) {
    float* m = static_cast<float*>(self);
    Identity__10VIMatrix44(self);
    m[0] = scale;
    m[10] = scale;
    m[5] = scale;
}
}
