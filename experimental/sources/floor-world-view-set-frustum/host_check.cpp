#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" float tanf(float radians);
extern "C" float viewFrustum[18];
extern "C" float clipViewFrustum[18];
extern "C" float worldScrz;
extern "C" void worldSetFrustum__Fffff(float, float, float, float);

namespace {
int calls = 0;
float seen[3];
const float answers[3] = {2.0f, 4.0f, 0.5f};

unsigned bits_of(float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    return bits;
}
float bits_float(unsigned bits) {
    float value = 0.0f;
    std::memcpy(&value, &bits, 4);
    return value;
}
void expect_normalized(const float* plane, float x, float y, float z, int code) {
    const float length_squared = (x * x + y * y) + z * z;
    const float scale = 1.0f / std::sqrt(length_squared);
    const float expected[3] = {x * scale, y * scale, z * scale};
    for (int i = 0; i < 3; ++i) {
        if (bits_of(plane[i]) != bits_of(expected[i])) {
            std::fprintf(stderr, "plane %d component %d got %08x want %08x\n",
                         code, i, bits_of(plane[i]), bits_of(expected[i]));
            std::exit(code);
        }
    }
}
}

extern "C" float tanf(float radians) {
    if (calls >= 3) std::exit(20);
    seen[calls] = radians;
    return answers[calls++];
}

int main() {
    if (bits_of(worldScrz) != 0x44800000u) return 1;
    const float scale = bits_float(0x3c8efa34u);
    worldSetFrustum__Fffff(50.0f, 0.5f, 80.0f, 0.25f);
    if (calls != 3) return 2;
    const float first_argument = (50.0f * scale) * 0.5f;
    const float second_argument = ((50.0f + 1.0f) * scale) * 0.5f;
    const float third_argument = (80.0f * scale) * 0.5f;
    if (bits_of(seen[0]) != bits_of(first_argument)) return 3;
    if (bits_of(seen[1]) != bits_of(second_argument)) return 4;
    if (bits_of(seen[2]) != bits_of(third_argument)) return 5;
    if (bits_of(worldScrz) != bits_of(bits_float(0x43eec000u) / 2.0f)) return 6;
    expect_normalized(viewFrustum + 0, 1.0f, 0.0f, 2.0f, 7);
    expect_normalized(viewFrustum + 3, -1.0f, 0.0f, 2.0f, 8);
    expect_normalized(viewFrustum + 6, 0.0f, 1.0f, 1.0f, 9);
    expect_normalized(viewFrustum + 9, 0.0f, -1.0f, 1.0f, 10);
    expect_normalized(viewFrustum + 12, 0.0f, 0.0f, -1.0f, 11);
    expect_normalized(viewFrustum + 15, 0.0f, 0.0f, 1.0f, 12);
    expect_normalized(clipViewFrustum + 0, 1.0f, 0.0f, 0.25f, 13);
    expect_normalized(clipViewFrustum + 3, -1.0f, 0.0f, 0.25f, 14);
    expect_normalized(clipViewFrustum + 6, 0.0f, 1.0f, 0.5f, 15);
    expect_normalized(clipViewFrustum + 9, 0.0f, -1.0f, 0.5f, 16);
    expect_normalized(clipViewFrustum + 12, 0.0f, 0.0f, -1.0f, 17);
    expect_normalized(clipViewFrustum + 15, 0.0f, 0.0f, 1.0f, 18);
    return 0;
}
