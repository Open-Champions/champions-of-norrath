#include "anim_get_volume_scale.cpp"
#include <cstdio>
extern "C" unsigned eRandState = 0;
extern "C" float pitchTab[128] = {};
extern "C" float groupVolumeScale[300] = {};
int main() {
    for (int i = 0; i < 128; ++i) pitchTab[i] = i / 100.0f;
    for (int i = 0; i < 300; ++i) groupVolumeScale[i] = 1.0f;
    groupVolumeScale[7] = 0.5f;
    // no spread: index 64, generator untouched
    eRandState = 123;
    if (animGetVolumeScale__Fii(0, 0) != pitchTab[64] || eRandState != 123) return 1;
    if (animGetVolumeScale__Fii(7, -3) != pitchTab[64] * 0.5f || eRandState != 123) return 2;
    // spread: the offset follows the generator
    eRandState = 0;
    const unsigned state = 0x3c6ef35fu;
    const int offset = static_cast<int>(state >> 16) % 7 - 6;
    if (animGetVolumeScale__Fii(0, 3) != pitchTab[64 + offset] || eRandState != state) return 3;
    // limits: a huge spread can push the index below zero, a product above 1 is limited
    pitchTab[0] = 0.25f;
    eRandState = 0;
    const unsigned s2 = 0x3c6ef35fu;
    const int off2 = static_cast<int>(s2 >> 16) % 2001 - 2000;
    const int want = 64 + off2 < 0 ? 0 : 64 + off2;
    if (animGetVolumeScale__Fii(0, 1000) != pitchTab[want]) return 4;
    pitchTab[64] = 3.0f;
    if (animGetVolumeScale__Fii(0, 0) != 1.0f) return 5;
    pitchTab[64] = -2.0f;
    if (animGetVolumeScale__Fii(0, 0) != 0.0f) return 6;
    pitchTab[64] = 0.8f; groupVolumeScale[9] = 0.5f;
    if (animGetVolumeScale__Fii(9, 0) != 0.8f * 0.5f) return 7;
    std::puts("anim_get_volume_scale passed");
    return 0;
}
