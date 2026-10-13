// Observed: RecomputeLightningPatches__12VIAtmosphere at 0x0106a020 is 620 bytes in the pinned SLUS-20565 image. The atmosphere is
// this. It fills an array of fourteen 16-byte vectors at this + 3936 (so the w lane of vector i is at this + 3948 + 16*i), one per
// loop pass i = 0..13, calling rand() four times per pass in a fixed order. Constants are the single-float bit patterns shown.
// With P = 0x40490fda (3.14159...), A = 0x3b666666, B = 0x3b23d70a, C = 0x3b808080, and r1..r4 the four rand() results of the pass:
//   h = (float)(128 - ((r1 / 137) & 0xff)) * P * A      -- three angles in a local vector:
//   p = (float)(128 - ((r2 / 137) & 0xff)) * P * B
//   q = (float)(128 - ((r3 / 137) & 0xff)) * P * B      -- stored as (h, p, q)
//   a local zeroed 4x4 matrix M gets RotateHPR__10VIMatrix44RC7VIVect3(M, &(h, p, q));
//   the vector (0.0f, 100.0f, 0.0f, 1.0f) is multiplied by M with Mul__C10VIMatrix44RC7VIVect4R7VIVect4(M, in, out) into vector i;
//   then the w lane of vector i is overwritten with (float)((r4 / 131) & 0xff) * C * 20.0f + 70.0f;
//   M is destroyed with _$_10VIMatrix44(M, 2).
// Divisions are signed. Returns nothing.
// Inferred: builds fourteen pseudo-random points (a 100-unit vector rotated by random heading/pitch/roll) with a random 70..90 value in
// the w lane, as the geometry for lightning bolts; the 0.9 and 0.64-per-256 factors in A and B bound the angles. Not checked here:
// whether the unit is radians (the factor P suggests so), and the role of the w lane.
// Unknown: the member names, the meaning of RotateHPR's argument order, the layout beyond the 224 bytes written, and rand's
// sequence (the libc routine is an import). Nothing here ran on the EE.
extern "C" {
int rand(void);
void RotateHPR__10VIMatrix44RC7VIVect3(float* matrix, const float* angles);
void Mul__C10VIMatrix44RC7VIVect4R7VIVect4(const float* matrix, const float* in, float* out);
void _$_10VIMatrix44(float* matrix, int flags);

void RecomputeLightningPatches__12VIAtmosphere(unsigned char* self) {
    const float pi = __builtin_bit_cast(float, 0x40490fdau);
    const float scaleHeading = __builtin_bit_cast(float, 0x3b666666u);
    const float scaleOther = __builtin_bit_cast(float, 0x3b23d70au);
    const float scaleByte = __builtin_bit_cast(float, 0x3b808080u);
    float* patches = reinterpret_cast<float*>(self + 3936);
    for (int i = 0; i < 14; ++i) {
        alignas(16) float matrix[16] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
        alignas(16) float angles[4];
        alignas(16) float source[4];
        float heading = static_cast<float>(128 - ((rand() / 137) & 0xff)) * pi * scaleHeading;
        float pitch = static_cast<float>(128 - ((rand() / 137) & 0xff));
        int thirdRoll = rand() / 137;
        angles[0] = heading;
        angles[1] = pitch * pi * scaleOther;
        angles[2] = static_cast<float>(128 - (thirdRoll & 0xff)) * pi * scaleOther;
        RotateHPR__10VIMatrix44RC7VIVect3(matrix, angles);
        source[0] = 0.0f;
        source[1] = 100.0f;
        source[2] = 0.0f;
        source[3] = 1.0f;
        float* out = patches + 4 * i;
        Mul__C10VIMatrix44RC7VIVect4R7VIVect4(matrix, source, out);
        out[3] = static_cast<float>((rand() / 131) & 0xff) * scaleByte * 20.0f + 70.0f;
        _$_10VIMatrix44(matrix, 2);
    }
}
}
