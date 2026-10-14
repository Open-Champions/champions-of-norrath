// Observed: Precalc__9VICollRay at 0x0109c528 is 232 bytes in the pinned SLUS-20565 image. With the ray as this and its floats
// indexed from +0: v = r[3..5] - r[0..2] is stored at r[6..8] and copied to r[9..11]; Magnitude__C7VIVect3(&r[9]) is called and its
// result is stored at r[12]. If the result equals 0.0f, r[13] is set to 0.0f and r[9..11] stay as the unscaled difference.
// Otherwise r[13] = 1.0f / length and r[9..11] are multiplied by r[13]. The routine returns zero.
// Inferred: r[0..2] is the ray origin, r[3..5] its end point, r[6..8] the segment vector, r[9..11] the unit direction, r[12] the
// length and r[13] the reciprocal length. The callers of this routine in Init__9VICollRayRC7VIVect3T1 (w13-vi-c) read +36 afterwards.
// Unknown: the class size, and Magnitude__C7VIVect3's body (an import). Nothing here ran on the EE.
extern "C" {
float Magnitude__C7VIVect3(const float* v);

int Precalc__9VICollRay(float* r) {
    float dx = r[3] - r[0];
    r[6] = dx;
    float dy = r[4] - r[1];
    r[7] = dy;
    float dz = r[5] - r[2];
    r[8] = dz;
    r[9] = dx;
    r[10] = r[7];
    r[11] = r[8];
    float length = Magnitude__C7VIVect3(r + 9);
    r[12] = length;
    if (length == 0.0f) {
        r[13] = 0.0f;
        return 0;
    }
    float inverse = 1.0f / length;
    r[13] = inverse;
    r[9] = r[9] * inverse;
    r[10] = r[10] * inverse;
    r[11] = r[11] * inverse;
    return 0;
}
}
