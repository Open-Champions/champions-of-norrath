// Observed: Init__9VICollRayRC9VICollRayRC10VIMatrix44 at 0x0109bff0 is 468 bytes. Arguments: the
// destination ray D (this), a source ray S and a matrix M. Offsets are bytes. It runs, in order:
//   1. __ml__FRC7VIVect3RC10VIMatrix44(tmp, S+0, M); D[0..8] = tmp x, y, z (the fourth lane is dropped)
//   2. the same call with S+12; D[12..20] = tmp x, y, z
//   3. __ml__FRC7VIVect4RC10VIMatrix44(tmp, {S[56], S[60], S[64], 0.0}, M); a second four-float
//      vector takes all four result lanes and Normalize__7VIVect4 runs on it; D[56], D[60], D[64] =
//      its x, y, z (the fourth lane is dropped)
//   4. Precalc__9VICollRay(D), result ignored
//   5. D[68] = S[68] (float), D[72] = S[72] (word), D[76..84] = S[76..84] (three floats)
// and returns 0. There is no failure path. All three vector temporaries are initialised to 0.0 in
// all four lanes first.
// Inferred: D[0..8] and D[12..20] are the ray's two end points moved by M, D[56..64] its direction
// (rotated by M as a w = 0 vector, then unit length); the rest is copied through. Unknown: the
// real field names, what Precalc derives, and the meaning of S[68..84].
extern "C" {
float* __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
float* __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
void Normalize__7VIVect4(float* self);
int Precalc__9VICollRay(unsigned char* self);

int Init__9VICollRayRC9VICollRayRC10VIMatrix44(unsigned char* self, const unsigned char* source,
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

    Precalc__9VICollRay(self);

    out[17] = in[17];
    *reinterpret_cast<unsigned*>(self + 72) = *reinterpret_cast<const unsigned*>(source + 72);
    out[19] = in[19];
    out[20] = in[20];
    out[21] = in[21];
    return 0;
}
}
