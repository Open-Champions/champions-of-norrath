// Host check for animFrame__Fv. Not an EE plan input.
#include "anim_frame.cpp"
#include <cstring>

extern "C" {
int drawFrameNm;
}
extern "C" {
float groupVolumeScale[300];
}
static float* const table = groupVolumeScale;

static unsigned bits(float f) {
    unsigned b;
    std::memcpy(&b, &f, 4);
    return b;
}

int main() {
    unsigned expect[300];
    for (int i = 0; i < 300; ++i) expect[i] = 0x3f7fffffu;
    const int a[] = {5, 18, 40};
    const int b[] = {10, 14, 21, 31, 32, 33, 34, 43};
    for (int i : a) expect[i] = 0x3f599999u;
    for (int i : b) expect[i] = 0x3f666666u;
    expect[12] = 0x3f4cccccu;
    expect[72] = 0x3f266666u;

    // not on a multiple of 16: untouched
    for (int n = 1; n < 16; ++n) {
        for (int i = 0; i < 300; ++i) table[i] = 3.0f;
        drawFrameNm = 32 + n;
        animFrame__Fv();
        for (int i = 0; i < 300; ++i) if (bits(table[i]) != bits(3.0f)) return 1;
    }
    // on a multiple of 16: whole table rewritten, any previous content
    const int frames[] = {0, 16, 4096, -16};
    for (int f : frames) {
        for (int i = 0; i < 300; ++i) table[i] = static_cast<float>(i) * -7.5f;
        drawFrameNm = f;
        animFrame__Fv();
        for (int i = 0; i < 300; ++i) if (bits(table[i]) != expect[i]) return 2;
    }
    // other high bits ignored, only the low four matter
    for (int i = 0; i < 300; ++i) table[i] = 0.0f;
    drawFrameNm = 0x7ffffff0;
    animFrame__Fv();
    if (bits(table[299]) != 0x3f7fffffu || bits(table[72]) != 0x3f266666u) return 3;
    return 0;
}
