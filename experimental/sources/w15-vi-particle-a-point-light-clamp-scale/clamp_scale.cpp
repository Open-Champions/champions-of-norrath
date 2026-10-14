// Observed: ClampScale__12VIPointLightR10VIMatrix44 at 0x010c3790 is 444 bytes (the same instruction sequence as the other two ClampScale routines for VIPointLight,
// VIPointSprite and VIStreamAudioSprite). Argument: the sprite and a 4x4 float matrix M. For each of the three rows R0 = M[0..2], R1 = M[4..6]
// and R2 = M[8..10] it forms the squared length (components squared and summed in order) and the reciprocal square root
// s = 1.0f / sqrt(squared length) (rsqrt.s). When s < 0.25f (the row is longer than 4) the three components of that row are replaced by
// (component * s) * 4.0f; other rows are untouched. M[3], M[7], M[11] and the fourth row are neither read nor written. It returns 0.
// Inferred: this limits each axis scale of the matrix to at most 4.0; the object argument is not read.
// Unknown: why the limit is 4.0 and what the caller does with the clamped matrix.
extern "C" {
static void clamp_row(float* row, float s) {
    if (s < 0.25f) {
        row[0] = row[0] * s * 4.0f;
        row[1] = row[1] * s * 4.0f;
        row[2] = row[2] * s * 4.0f;
    }
}

int ClampScale__12VIPointLightR10VIMatrix44(void*, float* m) {
    float s0 = 1.0f / __builtin_sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
    float s1 = 1.0f / __builtin_sqrtf(m[4] * m[4] + m[5] * m[5] + m[6] * m[6]);
    float s2 = 1.0f / __builtin_sqrtf(m[8] * m[8] + m[9] * m[9] + m[10] * m[10]);
    clamp_row(m, s0);
    clamp_row(m + 4, s1);
    clamp_row(m + 8, s2);
    return 0;
}
}
