#include "process_analog_input.cpp"
#include <cmath>
#include <cstring>
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
static float fb(unsigned u) { float f; std::memcpy(&f, &u, 4); return f; }
int main() {
    const float dead = fb(0x3ea3d70au), gain = fb(0x3fbc3c3cu);
    float a = 1, b = 1;
    processAnalogInput__FiiPfT2(128, 128, &a, &b);  // centered
    if (bits(a) != 0 || bits(b) != 0) return 1;
    a = b = 1;
    processAnalogInput__FiiPfT2(128 + 40, 128, &a, &b);  // 40/128 = 0.3125 < 0.32
    if (bits(a) != 0 || bits(b) != 0) return 2;
    processAnalogInput__FiiPfT2(128 + 41, 128, &a, &b);  // 41/128 = 0.3203 >= 0.32
    if (!(a > 0.0f && a < 0.01f) || b != 0.0f) return 3;
    processAnalogInput__FiiPfT2(255, 128, &a, &b);  // full right
    float m = 127.0f / 128.0f;
    if (std::fabs(a - m * ((1.0f - dead / m) * gain)) > 1e-5f || b != 0.0f) return 4;
    processAnalogInput__FiiPfT2(0, 128, &a, &b);  // full left, symmetric
    if (a >= 0 || std::fabs(a + 1.0f * ((1.0f - dead) * gain)) > 1e-5f) return 5;
    processAnalogInput__FiiPfT2(128, 0, &a, &b);  // y axis lands in the second output
    if (b >= 0 || std::fabs(a) > 1e-6f) return 6;
    processAnalogInput__FiiPfT2(128 + 60, 128 + 80, &a, &b);  // 3-4-5 triangle, length 100/128
    float len = 100.0f / 128.0f;
    float g = (1.0f - dead / len) * gain;
    if (std::fabs(a - (60.0f / 128.0f) * g) > 1e-5f || std::fabs(b - (80.0f / 128.0f) * g) > 1e-5f) return 7;
    return 0;
}
