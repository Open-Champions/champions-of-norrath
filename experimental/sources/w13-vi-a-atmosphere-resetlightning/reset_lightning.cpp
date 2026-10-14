// Observed: ResetLightning__12VIAtmosphere at 0x0106b700 is 220 bytes. It calls rand() twice and
// then RecomputeLightningPatches__12VIAtmosphere(atmosphere). With r the first result and u =
// ((131 * r) & 0xff) * 0x1.010100p-8f (the float 0x3b808080, about 1/255): it stores 8.0f at
// +4504; f = u limited to [0x1.999998p-4f, 0x1.999998p-1f] (max with the lower bound, then min with the
// upper one) at +4520; and 6.0f * f at +4508. With v the same expression of the second result
// it stores v * 0x1.333332p-1f + 0x1.333332p-1f at +4516, in the delay slot of the call to
// RecomputeLightningPatches. The three limit/scale floats are the truncated single-precision
// values 0x3dcccccc, 0x3f4ccccc and 0x3f199999, not the rounded 0.1f, 0.8f and 0.6f. The
// literals below are hex floats because the reviewed compiler does not round decimal
// literals to these exact values.
// Inferred: the lightning state: +4504 a constant 8.0f (duration or interval scale), +4520 a
// random brightness in 0.1..0.8, +4508 a length derived from it and +4516 a second random
// strength in 0.6..1.2. The multiplier 131 mod 256 maps rand's low byte to a byte.
// Unknown: which of these fields is read by GetLightning__12VIAtmospherePf, the units, and what
// RecomputeLightningPatches builds.
extern "C" {
int rand(void);
void RecomputeLightningPatches__12VIAtmosphere(void* atmosphere);

void ResetLightning__12VIAtmosphere(unsigned char* atmosphere) {
    const float byteScale = 0x1.010100p-8f;
    float u = static_cast<float>((131 * rand()) & 0xff) * byteScale;
    *reinterpret_cast<float*>(atmosphere + 4504) = 8.0f;
    float level = u > 0x1.999998p-4f ? u : 0x1.999998p-4f;
    level = level < 0x1.999998p-1f ? level : 0x1.999998p-1f;
    *reinterpret_cast<float*>(atmosphere + 4520) = level;
    *reinterpret_cast<float*>(atmosphere + 4508) = level * 6.0f;
    float v = static_cast<float>((131 * rand()) & 0xff) * byteScale;
    *reinterpret_cast<float*>(atmosphere + 4516) = v * 0x1.333332p-1f + 0x1.333332p-1f;
    RecomputeLightningPatches__12VIAtmosphere(atmosphere);
}
}
