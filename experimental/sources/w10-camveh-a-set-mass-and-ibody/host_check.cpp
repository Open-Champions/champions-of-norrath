// Host contract for setMassAndIBody__7Vehicle. Not an EE plan input.
#include "set_mass_and_ibody.cpp"

#include <cstdio>
#include <cstring>

static float get(const unsigned char* o, int off) { float v; std::memcpy(&v, o + off, 4); return v; }
static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }

int main() {
    int failures = 0;
    const float k1 = 0x1.a41a40p-6f, k2 = 0x1.555554p-4f;
    const float cases[][4] = {{10000.f, 120.f, 120.f, 250.f}, {2.f, 1.f, 2.f, 3.f}, {0.5f, 40.f, 7.f, 3.25f}};
    for (const auto& c : cases) {
        unsigned char o[2304];
        std::memset(o, 0x6b, sizeof o);
        unsigned char before[sizeof o];
        put(o, 884, c[0]); put(o, 888, c[1]); put(o, 892, c[2]); put(o, 896, c[3]);
        std::memcpy(before, o, sizeof o);
        setMassAndIBody__7Vehicle(o);
        const float x2 = (c[1] * k1) * (c[1] * k1), y2 = (c[2] * k1) * (c[2] * k1), z2 = (c[3] * k1) * (c[3] * k1);
        const float a = c[0] * (x2 + y2) * k2, b = c[0] * (x2 + z2) * k2, d = c[0] * (z2 + y2) * k2;
        const struct { int off; float v; } want[] = {
            {668, c[0]}, {672, 1.0f / c[0]}, {1216, a}, {1220, b}, {1212, d},
            {1228, 1.0f / a}, {1232, 1.0f / b}, {1224, 1.0f / d}};
        unsigned char expect[sizeof o];
        std::memcpy(expect, before, sizeof o);
        for (const auto& w : want) {
            if (get(o, w.off) != w.v) { std::fprintf(stderr, "fail: offset %d\n", w.off); ++failures; }
            put(expect, w.off, w.v);
        }
        if (std::memcmp(expect, o, sizeof o) != 0) { std::fprintf(stderr, "fail: stray write\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
