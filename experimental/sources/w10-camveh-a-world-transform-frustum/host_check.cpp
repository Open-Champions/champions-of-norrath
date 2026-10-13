// Host contract for worldTransformFrustum__FP8Matrix34. Not an EE plan input.
#include "world_transform_frustum.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

unsigned char halfFrustumMode;
float farClip;
float viewFrustum[18];
float clipViewFrustum[18];
float worldFrustum[24];
float worldClipFrustum[24];
signed char frustumNormalSign[18];
signed char clipFrustumNormalSign[18];

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static void setup(unsigned char mode) {
    halfFrustumMode = mode;
    farClip = 100.0f;
    for (int i = 0; i < 18; ++i) {
        viewFrustum[i] = 0.0f;
        clipViewFrustum[i] = 0.0f;
    }
    // plane i uses the unit vector along axis i % 3, signed by i / 3
    for (int i = 0; i < 6; ++i) {
        viewFrustum[3 * i + i % 3] = i < 3 ? 1.0f : -1.0f;
        clipViewFrustum[3 * i + (i + 1) % 3] = i < 3 ? 2.0f : -2.0f;
    }
    std::memset(worldFrustum, 0, sizeof worldFrustum);
    std::memset(worldClipFrustum, 0, sizeof worldClipFrustum);
    std::memset(frustumNormalSign, 7, sizeof frustumNormalSign);
    std::memset(clipFrustumNormalSign, 7, sizeof clipFrustumNormalSign);
}

int main() {
    // Identity rotation, translation (1, 2, 3): p = -t.
    const float identity[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 2, 3};
    setup(0);
    worldTransformFrustum__FP8Matrix34(identity);
    // plane 0: normal (1,0,0): plane[3] = -(1 * -1) = 1
    expect(worldFrustum[0] == 1 && worldFrustum[1] == 0 && worldFrustum[2] == 0 && worldFrustum[3] == 1.0f, "plane 0");
    expect(worldFrustum[4 + 1] == 1 && worldFrustum[4 + 3] == 2.0f, "plane 1");
    // plane 2: normal (0,0,1): d = 3
    expect(worldFrustum[8 + 2] == 1 && worldFrustum[8 + 3] == 3.0f, "plane 2");
    // plane 3: normal (-1,0,0): d = -1
    expect(worldFrustum[12] == -1 && worldFrustum[12 + 3] == -1.0f, "plane 3 negated normal");
    // planes 4 and 5 carry the far clip and the 18.0 offset in their last float.
    expect(worldFrustum[19] == (-(-1.0f * -2.0f)) + 100.0f, "plane 4 gets farClip added");
    expect(worldFrustum[23] == (-(-1.0f * -3.0f)) - 18.0f, "plane 5 gets 18.0 subtracted");
    // signs
    expect(frustumNormalSign[0] == 1 && frustumNormalSign[1] == 0 && frustumNormalSign[2] == 0, "signs of plane 0");
    expect(frustumNormalSign[9] == 0 && frustumNormalSign[10] == 0 && frustumNormalSign[11] == 0, "signs of plane 3 (negative normal)");
    // clip planes use clipViewFrustum and are not changed by mode.
    expect(worldClipFrustum[1] == 2 && worldClipFrustum[3] == -(2.0f * -2.0f), "clip plane 0");
    expect(worldClipFrustum[19] == -(-2.0f * -3.0f) + 100.0f && worldClipFrustum[23] == -(-2.0f * -1.0f) - 18.0f, "clip plane far/near offsets");
    expect(clipFrustumNormalSign[1] == 1 && clipFrustumNormalSign[0] == 0, "clip signs");

    // Modes: 1 forces plane 0's normal to +x; 2 forces plane 1's to -x; 3 both. Restored afterwards.
    for (unsigned mode = 0; mode < 5; ++mode) {
        setup(static_cast<unsigned char>(mode));
        float before[18]; std::memcpy(before, viewFrustum, sizeof before);
        worldTransformFrustum__FP8Matrix34(identity);
        expect(std::memcmp(before, viewFrustum, sizeof before) == 0, "viewFrustum restored");
        const bool first = mode == 1 || mode == 3, second = mode == 2 || mode == 3;
        // plane 0 is +x already (normal along axis 0): detect through plane 1 instead: plane 1 is normally +y.
        expect((worldFrustum[4] == -1.0f) == second, "mode 2/3 force plane 1 to -x");
        expect((worldFrustum[0] == 1.0f && worldFrustum[1] == 0.0f) == true, "plane 0 stays +x");
        (void)first;
    }
    setup(1); viewFrustum[0] = 0; viewFrustum[1] = 5; viewFrustum[2] = 0;
    worldTransformFrustum__FP8Matrix34(identity);
    expect(worldFrustum[0] == 1.0f && worldFrustum[1] == 0.0f, "mode 1 overrides plane 0's normal");
    setup(0); viewFrustum[0] = 0; viewFrustum[1] = 5; viewFrustum[2] = 0;
    worldTransformFrustum__FP8Matrix34(identity);
    expect(worldFrustum[1] == 5.0f && viewFrustum[1] == 5.0f, "mode 0 leaves it");
    setup(0x81);
    worldTransformFrustum__FP8Matrix34(identity);
    expect(worldFrustum[4] == 0.0f && worldFrustum[5] == 1.0f, "mode 0x81 changes nothing");

    // A non-trivial matrix: rows (0,1,0), (-1,0,0), (0,0,1), translation (0, 0, 0).
    const float turn[12] = {0, 1, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0};
    setup(0);
    worldTransformFrustum__FP8Matrix34(turn);
    // plane 0: v = (1,0,0): (v.m0, v.m3, v.m6) = (0, -1, 0)
    expect(worldFrustum[0] == 0 && worldFrustum[1] == -1 && worldFrustum[2] == 0, "rotated plane 0");
    expect(frustumNormalSign[1] == 0, "sign of negative component");
    return failures ? 1 : 0;
}
