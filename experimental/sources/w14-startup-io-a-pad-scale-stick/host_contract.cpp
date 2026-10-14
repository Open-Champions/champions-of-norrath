#include "scale_stick.cpp"
#include <cstdint>
#include <cstring>

int main() {
    uint32_t bits;
    const float step = ScaleStick__5VIPadUc(0, 168);
    std::memcpy(&bits, &step, 4);
    if (bits != 0x3c3a2e8bu) return 1;
    for (int v = 0; v < 256; ++v) {
        float r = ScaleStick__5VIPadUc(0, static_cast<unsigned char>(v));
        float want;
        if (v < 88) want = static_cast<float>(88 - v) * -step;
        else if (v >= 168) want = static_cast<float>(v - 167) * step;
        else want = 0.0f;
        if (r != want) return 2;
        if (v >= 88 && v < 168 && r != 0.0f) return 3;
        if (v < 88 && !(r < 0.0f)) return 4;
        if (v >= 168 && !(r > 0.0f)) return 5;
    }
    if (!(ScaleStick__5VIPadUc(0, 0) < -0.98f) || !(ScaleStick__5VIPadUc(0, 255) > 0.98f)) return 6;
    return 0;
}
