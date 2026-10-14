extern "C" {
float worldClipFrustum[24];
signed char clipFrustumNormalSign[24];
int worldClipCubeToClipFrustum__FPA1_fi(float* bounds, int mask);
}
int main() {
    // bounds: x0 x1 y0 y1 z0 z1
    float b[6] = {-1, 2, -3, 4, -5, 6};
    // Plane 0 = +x: normal (1,0,0), w = -1.5; near sign byte 0 (min x), far uses max x=2.
    // Sign bytes are 0/1 picks of min/max; the far corner flips the low bit.
    for (int p = 0; p < 6; ++p) {
        worldClipFrustum[p * 4 + 0] = 0; worldClipFrustum[p * 4 + 1] = 0;
        worldClipFrustum[p * 4 + 2] = 0; worldClipFrustum[p * 4 + 3] = -1.0f;
        clipFrustumNormalSign[p * 3 + 0] = 0; clipFrustumNormalSign[p * 3 + 1] = 0; clipFrustumNormalSign[p * 3 + 2] = 0;
    }
    // Plane 0: x - 1.5 with near byte 0 -> far = max x = 2 -> 0.5 >= 0 : set.
    worldClipFrustum[0] = 1; worldClipFrustum[3] = -1.5f;
    // Plane 1: x - 2.5 -> -0.5 : not set.
    worldClipFrustum[4] = 1; worldClipFrustum[7] = -2.5f;
    // Plane 2: -y + 0 with near byte 1 -> far picks index 0 -> y=-3 -> 3 : set.
    worldClipFrustum[8 + 1] = -1; worldClipFrustum[8 + 3] = 0;
    clipFrustumNormalSign[2 * 3 + 1] = 1;
    // Plane 3: z with near byte 1 for z -> far picks z0=-5; w = 4.9 -> -0.1 : not set.
    worldClipFrustum[12 + 2] = 1; worldClipFrustum[12 + 3] = 4.9f;
    clipFrustumNormalSign[3 * 3 + 2] = 1;
    // Plane 4: exactly zero counts as set: z with near byte 0 -> far z1=6; w=-6.
    worldClipFrustum[16 + 2] = 1; worldClipFrustum[16 + 3] = -6.0f;
    // Plane 5: skipped when its bit is preset even though it would not pass.
    worldClipFrustum[20] = 1; worldClipFrustum[23] = -100.0f;
    if (worldClipCubeToClipFrustum__FPA1_fi(b, 0) != (1 | 4 | 16)) return 1;
    if (worldClipCubeToClipFrustum__FPA1_fi(b, 32) != (32 | 1 | 4 | 16)) return 2;
    // Preset bits are preserved, and a preset plane 0 is not re-evaluated.
    worldClipFrustum[3] = -100.0f;
    if (worldClipCubeToClipFrustum__FPA1_fi(b, 1) != (1 | 4 | 16)) return 3;
    if (worldClipCubeToClipFrustum__FPA1_fi(b, 0) != (4 | 16)) return 4;
    // Mixed multi-axis plane checks accumulation: 1*x + 2*y + 3*z + w
    // near sign (0,0,0) -> far corner (x1,y1,z1) = (2,4,6) -> 2+8+18 = 28.
    worldClipFrustum[20] = 1; worldClipFrustum[21] = 2; worldClipFrustum[22] = 3; worldClipFrustum[23] = -28.0f;
    if ((worldClipCubeToClipFrustum__FPA1_fi(b, 0) & 32) == 0) return 5;
    worldClipFrustum[23] = -28.01f;
    if ((worldClipCubeToClipFrustum__FPA1_fi(b, 0) & 32) != 0) return 6;
    return 0;
}
