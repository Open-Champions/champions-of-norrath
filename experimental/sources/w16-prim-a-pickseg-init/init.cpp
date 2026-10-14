// Observed: Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44 at 0x010bf4e0 is 400 bytes.
// Arguments: the destination segment D (this), a source segment S and a matrix M. Offsets are
// bytes. It runs, in order:
//   1. __ml__FRC7VIVect3RC10VIMatrix44(tmp, S+0, M); D[0..8] = tmp x, y, z (fourth lane dropped)
//   2. the same call with S+12; D[12..20] = tmp x, y, z
//   3. SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(M, S+64, D+64), then Normalize__7VIVect3(D+64)
//   4. the 16 bytes S[76..91] are copied to D[76..91] (unaligned doubleword pairs)
//   5. Precalc__13VIPickSegment(D); a negative result returns -1 and leaves D[56], D[60] as they
//      were; otherwise D[60] = S[60] (word), D[56] = S[56] (float) and the function returns 0.
// The two vector temporaries start as 0.0 in all four lanes.
// Inferred: D[0..8] and D[12..20] are the segment's end points moved by M, D+64 is a direction
// carried by the matrix without translation, then made unit length. Unknown: field names, what
// Precalc derives, what S[56], S[60] and S[76..91] hold.
extern "C" {
float* __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* vector, const unsigned char* matrix);
void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const unsigned char* matrix, const float* in, float* out);
void Normalize__7VIVect3(float* self);
int Precalc__13VIPickSegment(unsigned char* self);

int Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44(unsigned char* self, const unsigned char* source,
                                                        const unsigned char* matrix) {
    alignas(16) float point[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float other[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float result[4];
    float* out = reinterpret_cast<float*>(self);
    const float* in = reinterpret_cast<const float*>(source);

    __ml__FRC7VIVect3RC10VIMatrix44(result, in, matrix);
    point[0] = result[0];
    point[1] = result[1];
    point[2] = result[2];
    point[3] = result[3];

    __ml__FRC7VIVect3RC10VIMatrix44(result, in + 3, matrix);
    other[0] = result[0];
    other[1] = result[1];
    other[2] = result[2];
    other[3] = result[3];
    out[0] = point[0];
    out[1] = point[1];
    out[2] = point[2];
    out[3] = other[0];
    out[4] = other[1];
    out[5] = other[2];

    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(matrix, in + 16, out + 16);
    Normalize__7VIVect3(out + 16);

    for (int i = 0; i < 16; ++i)
        self[76 + i] = source[76 + i];

    if (Precalc__13VIPickSegment(self) < 0)
        return -1;
    *reinterpret_cast<unsigned*>(self + 60) = *reinterpret_cast<const unsigned*>(source + 60);
    out[14] = in[14];
    return 0;
}
}
