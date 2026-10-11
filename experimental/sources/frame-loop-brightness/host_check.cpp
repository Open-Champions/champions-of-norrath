#include <cstdio>
#include <cstring>

#include "brightness.cpp"

int main() {
    const unsigned char expected[4] = {0x00, 0x00, 0x00, 0x3f};
    unsigned char bytes[4];
    std::memcpy(bytes, &engineBrightnessCorrection, 4);
    if (std::memcmp(bytes, expected, 4) != 0 || engineBrightnessCorrection != 0x3f000000u) {
        std::fprintf(stderr, "fail: engineBrightnessCorrection bytes\n");
        return 1;
    }
    bytes[3] ^= 1u;
    unsigned changed;
    std::memcpy(&changed, bytes, 4);
    if (changed == engineBrightnessCorrection) return 1;
    std::printf("brightness checks passed\n");
    return 0;
}
