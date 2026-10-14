// Observed: Init__12VICollSphereRC12VICollSphereRC10VIMatrix44 at 0x010a0538 is 548 bytes. Arguments:
// the destination sphere probe D (this), a source probe S and a matrix M. Offsets are bytes. It runs,
// in order:
//   1. __ml__FRC7VIVect3RC10VIMatrix44(tmp, S+0, M); D[0..8] = tmp x, y, z (fourth lane dropped)
//   2. the same call with S+12; D[12..20] = tmp x, y, z
//   3. D[88] = S[88] * sqrt(M[0]*M[0] + M[1]*M[1] + M[2]*M[2]) -- the length of M's first row, summed
//      in that order, with the hardware sqrt.s instruction
//   4. __ml__FRC7VIVect4RC10VIMatrix44(tmp, {S[56], S[60], S[64], 0.0}, M); a second four-float
//      vector takes all four result lanes and Normalize__7VIVect4 runs on it; D[56], D[60], D[64] =
//      its x, y, z (fourth lane dropped)
//   5. Precalc__12VICollSphere(D), result ignored
//   6. D[68] = S[68] (float), D[72] = S[72] (word), D[76..84] = S[76..84] (three floats)
// and returns 0. There is no failure path. Vector temporaries start as 0.0 in all four lanes.
// Inferred: the same shape as the ray initialiser, with an extra scalar (a radius) scaled by the
// matrix's x-axis scale; D[0..8] and D[12..20] are two points moved by M. Unknown: field names, what
// Precalc derives, and the meaning of S[68..84].
namespace {
__attribute__((always_inline)) inline float square_root(float value) {
#if defined(__mips__)
    float result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(value));
    return result;
#else
    return __builtin_sqrtf(value);
#endif
}
}

extern "C" {
float* __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
float* __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
void Normalize__7VIVect4(float* self);
int Precalc__12VICollSphere(unsigned char* self);

int Init__12VICollSphereRC12VICollSphereRC10VIMatrix44(unsigned char* self, const unsigned char* source,
                                              const unsigned char* matrix) {
    alignas(16) float point[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float direction[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float result[4];
    float* out = reinterpret_cast<float*>(self);
    const float* in = reinterpret_cast<const float*>(source);

    __ml__FRC7VIVect3RC10VIMatrix44(result, in, matrix);
    point[0] = result[0];
    point[1] = result[1];
    point[2] = result[2];
    point[3] = result[3];
    out[0] = point[0];
    out[1] = point[1];
    out[2] = point[2];

    __ml__FRC7VIVect3RC10VIMatrix44(result, in + 3, matrix);
    point[0] = result[0];
    point[1] = result[1];
    point[2] = result[2];
    point[3] = result[3];
    out[3] = point[0];
    out[4] = point[1];
    out[5] = point[2];

    const float* m = reinterpret_cast<const float*>(matrix);
    out[22] = in[22] * square_root(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);

    point[0] = in[14];
    point[1] = in[15];
    point[2] = in[16];
    point[3] = 0.0f;
    __ml__FRC7VIVect4RC10VIMatrix44(result, point, matrix);
    direction[0] = result[0];
    direction[1] = result[1];
    direction[2] = result[2];
    direction[3] = result[3];
    Normalize__7VIVect4(direction);
    out[14] = direction[0];
    out[15] = direction[1];
    out[16] = direction[2];

    Precalc__12VICollSphere(self);

    out[17] = in[17];
    *reinterpret_cast<unsigned*>(self + 72) = *reinterpret_cast<const unsigned*>(source + 72);
    out[19] = in[19];
    out[20] = in[20];
    out[21] = in[21];
    return 0;
}
}
