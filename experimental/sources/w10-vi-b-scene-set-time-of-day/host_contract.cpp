#include "set_time_of_day.cpp"

#include <cstring>

static int check(float input, float expected) {
    alignas(4) unsigned char object[8200];
    std::memset(object, 0x5a, sizeof object);
    if (SetTimeOfDay__7VIScenef(reinterpret_cast<VIScene*>(object), input) != 0) return 1;
    float stored;
    std::memcpy(&stored, object + 8160, 4);
    if (std::memcmp(&stored, &expected, 4) != 0) return 2;
    for (int i = 0; i < 8200; ++i) {
        if ((i < 8160 || i >= 8164) && object[i] != 0x5a) return 3;
    }
    return 0;
}

int main() {
    unsigned top_bits = 0x41bffffau;
    float top;
    std::memcpy(&top, &top_bits, 4);
    if (int e = check(12.0f, 12.0f)) return e;
    if (int e = check(0.0f, 0.0f)) return 10 + e;
    if (int e = check(-5.0f, 0.0f)) return 20 + e;
    if (int e = check(24.0f, top)) return 30 + e;
    if (int e = check(1000.0f, top)) return 40 + e;
    if (int e = check(top, top)) return 50 + e;
    if (int e = check(23.5f, 23.5f)) return 60 + e;
    return 0;
}
