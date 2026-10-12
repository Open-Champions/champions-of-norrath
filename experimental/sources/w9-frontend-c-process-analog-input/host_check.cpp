#include "process_analog_input.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
int main() {
    float a = 1, b = 1;
    processAnalogInput__FiiPfT2(128, 128, &a, &b);  // centered
    if (bits(a) != 0 || bits(b) != 0) return 1;
    a = b = 1;
    processAnalogInput__FiiPfT2(133, 128, &a, &b);  // 5/128 = 0.039 < dead zone
    if (bits(a) != 0 || bits(b) != 0) return 2;
    a = b = 1;
    processAnalogInput__FiiPfT2(255, 128, &a, &b);  // full right
    float m = 127.0f / 128.0f;
    float f = (1.0f - 0.0416666642f / m) * 10.6799994f;
    if (std::fabs(a - m * f) > 1e-4f || b != 0.0f) return 3;
    a = b = 0;
    processAnalogInput__FiiPfT2(0, 128, &a, &b);    // full left, symmetric
    if (std::fabs(a + 1.0f * ((1.0f - 0.0416666642f) * 10.6799994f)) > 1e-3f) return 4;
    processAnalogInput__FiiPfT2(128, 0, &a, &b);    // y axis lands in the second output
    if (b >= 0 || std::fabs(a) > 1e-6f) return 5;
    processAnalogInput__FiiPfT2(128 + 6, 128 + 8, &a, &b);  // 6-8-10 triangle, length 10/128
    float len = 10.0f / 128.0f;
    float g = (1.0f - 0.0416666642f / len) * 10.6799994f;
    if (std::fabs(a - (6.0f / 128.0f) * g) > 1e-4f || std::fabs(b - (8.0f / 128.0f) * g) > 1e-4f) return 6;
    return 0;
}
