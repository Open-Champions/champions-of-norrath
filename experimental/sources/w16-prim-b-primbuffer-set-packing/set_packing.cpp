// Observed: SetPacking__12VIPrimBufferiii at 0x01102b88 is 188 bytes. For each of its three int
// arguments, in argument order, it calls powf(2.0f, (float)n), divides 1.0f by the result and
// stores the float at offset 48, 52 and 56 of the object. It returns 0.
// Inferred: the three floats are reciprocal scales (2^-n) for the three packed vertex components.
// Unknown: which components they scale (positions, normals or texture coordinates).
extern "C" {
float powf(float x, float y);

int SetPacking__12VIPrimBufferiii(unsigned char* self, int a, int b, int c) {
    float* out = reinterpret_cast<float*>(self + 48);
    out[0] = 1.0f / powf(2.0f, static_cast<float>(a));
    out[1] = 1.0f / powf(2.0f, static_cast<float>(b));
    out[2] = 1.0f / powf(2.0f, static_cast<float>(c));
    return 0;
}
}
