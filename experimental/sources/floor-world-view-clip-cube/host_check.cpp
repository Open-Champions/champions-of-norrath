#include <cstdio>
#include <cstring>

extern "C" float worldFrustum[24];
extern "C" signed char frustumNormalSign[24];
extern "C" int worldClipCubeToFrustum__FPA1_fi(float* bounds, int mask);

namespace {
void set_plane(int plane, float x, float y, float z, float w) {
    worldFrustum[plane * 4 + 0] = x;
    worldFrustum[plane * 4 + 1] = y;
    worldFrustum[plane * 4 + 2] = z;
    worldFrustum[plane * 4 + 3] = w;
}
}

int main() {
    if (sizeof worldFrustum != 96u || sizeof frustumNormalSign != 24u) return 1;
    for (float value : worldFrustum) {
        if (value != 0.0f) return 2;
    }
    for (signed char value : frustumNormalSign) {
        if (value != 0) return 3;
    }
    std::memset(frustumNormalSign, 0, sizeof frustumNormalSign);
    for (int plane = 0; plane < 6; ++plane) set_plane(plane, 0.0f, 0.0f, 0.0f, 1.0f);
    float inside[6] = {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};
    if (worldClipCubeToFrustum__FPA1_fi(inside, 0) != 0x3f) return 4;

    set_plane(1, 1.0f, 0.0f, 0.0f, -5.0f);
    if (worldClipCubeToFrustum__FPA1_fi(inside, 0) != 0) return 5;
    if (worldClipCubeToFrustum__FPA1_fi(inside, 0x2) != 0x3f) return 6;

    for (int plane = 0; plane < 6; ++plane) set_plane(plane, 0.0f, 0.0f, 0.0f, 1.0f);
    set_plane(2, 1.0f, 0.0f, 0.0f, 0.0f);
    frustumNormalSign[6] = 1;
    float straddling[6] = {-1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};
    if (worldClipCubeToFrustum__FPA1_fi(straddling, 0) != 0x3b) return 7;
    frustumNormalSign[6] = 0;
    if (worldClipCubeToFrustum__FPA1_fi(straddling, 0) != 0) return 8;

    float nan_bounds[6] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};
    unsigned nan_bits = 0x7fc00000u;
    std::memcpy(&nan_bounds[1], &nan_bits, 4);
    for (int plane = 0; plane < 6; ++plane) set_plane(plane, 0.0f, 0.0f, 0.0f, 1.0f);
    set_plane(0, 1.0f, 0.0f, 0.0f, 0.0f);
    std::memset(frustumNormalSign, 0, sizeof frustumNormalSign);
    for (int plane = 1; plane < 6; ++plane) frustumNormalSign[plane * 3] = 1;
    const int with_nan = worldClipCubeToFrustum__FPA1_fi(nan_bounds, 0);
    if (with_nan != 0x3e) return 9;
    return 0;
}
