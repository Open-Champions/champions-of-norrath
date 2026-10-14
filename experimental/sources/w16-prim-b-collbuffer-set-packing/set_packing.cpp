// Observed: SetPacking__12VICollBufferi at 0x0108baa0 is 80 bytes. It calls powf(2.0f, (float)n)
// once (n is the int argument), divides 1.0f by the result, stores that float at offset 16 of the
// object and returns 0.
// Inferred: offset 16 holds a reciprocal scale for packed coordinates (2^-n). Unknown: the unit of n.
extern "C" {
float powf(float x, float y);

int SetPacking__12VICollBufferi(unsigned char* self, int bits) {
    float scale = 1.0f / powf(2.0f, static_cast<float>(bits));
    *reinterpret_cast<float*>(self + 16) = scale;
    return 0;
}
}
