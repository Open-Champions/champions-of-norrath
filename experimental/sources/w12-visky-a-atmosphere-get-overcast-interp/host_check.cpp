// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
#include "get_overcast_interp.cpp"
#include <cstring>

static unsigned char object[2048];
static float query(int a, int b, float f) {
    std::memset(object, 0x44, sizeof object);
    std::memcpy(object + 1516, &a, 4);
    std::memcpy(object + 1520, &b, 4);
    std::memcpy(object + 1544, &f, 4);
    unsigned char before[sizeof object];
    std::memcpy(before, object, sizeof before);
    float r = GetOvercastInterp__12VIAtmosphere(object);
    if (std::memcmp(before, object, sizeof before) != 0) return -99.0f;
    return r;
}

int main() {
    if (query(0, 0, 0.25f) != 0.0f) return 1;
    if (query(0, 1, 0.25f) != 0.25f) return 2;
    if (query(1, 0, 0.25f) != 0.75f) return 3;
    if (query(1, 1, 0.25f) != 1.0f) return 4;
    if (query(0, 2, 0.25f) != 1.0f) return 5;
    if (query(0, -1, 0.25f) != 1.0f) return 6;
    if (query(2, 0, 0.25f) != 1.0f) return 7;
    if (query(-1, 0, 0.25f) != 1.0f) return 8;
    if (query(1, 2, 0.25f) != 1.0f) return 9;
    if (query(1, 0, 0.75f) != 0.25f) return 10;
    return 0;
}
