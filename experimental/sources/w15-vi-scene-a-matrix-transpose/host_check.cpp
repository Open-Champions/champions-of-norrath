// Host check only; not a build input.
#include "transpose.cpp"
#include <cstring>
int main() {
    alignas(16) float src[16], dst[16];
    for (int i = 0; i < 16; ++i) { src[i] = static_cast<float>(i + 1) * 1.5f; dst[i] = -1.0f; }
    Transpose__10VIMatrix44RC10VIMatrix44(reinterpret_cast<unsigned char*>(dst), reinterpret_cast<unsigned char*>(src));
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            if (dst[c * 4 + r] != src[r * 4 + c]) return 1;
    for (int i = 0; i < 16; ++i) if (src[i] != static_cast<float>(i + 1) * 1.5f) return 2;
    // bit patterns, including a NaN payload and negative zero, are copied untouched
    unsigned bits[16];
    for (int i = 0; i < 16; ++i) bits[i] = 0x7fc00000u + static_cast<unsigned>(i) * 0x1234567u;
    bits[5] = 0x80000000u;
    unsigned out[16] = {0};
    Transpose__10VIMatrix44RC10VIMatrix44(reinterpret_cast<unsigned char*>(out), reinterpret_cast<unsigned char*>(bits));
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            if (out[c * 4 + r] != bits[r * 4 + c]) return 3;
    return 0;
}
