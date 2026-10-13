// Host contract for worldToMap__3mapP6Point2PfT2. Not an EE plan input.
#include "world_to_map.cpp"

#include <cstdio>
#include <cstring>

float _3map$mapOffset[2];

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

int main() {
    unsigned kbits = 0x3d088888u;
    float k;
    std::memcpy(&k, &kbits, 4);
    const float pts[][2] = {{0.0f, 0.0f}, {120.0f, -40.0f}, {-1000.5f, 77.25f}, {3.0f, 1e6f}};
    const float offs[][2] = {{0.0f, 0.0f}, {100.0f, 50.0f}, {-12.5f, 7.0f}};
    for (const auto& o : offs) {
        for (const auto& p : pts) {
            _3map$mapOffset[0] = o[0];
            _3map$mapOffset[1] = o[1];
            float x = -1.0f, y = -1.0f;
            float in[2] = {p[0], p[1]};
            worldToMap__3mapP6Point2PfT2(in, &x, &y);
            volatile float t0 = p[0] * 0.25f;
            volatile float t1 = p[1] * 0.25f;
            volatile float u0 = t0 * k;
            volatile float u1 = t1 * k;
            expect(x == o[0] + u0, "x = offset + p.x * 0.25 * k");
            expect(y == o[1] + u1, "y = offset + p.y * 0.25 * k");
            expect(in[0] == p[0] && in[1] == p[1], "input untouched");
        }
    }
    // The scale constant itself, observed from the output of a unit point.
    _3map$mapOffset[0] = 0.0f; _3map$mapOffset[1] = 0.0f;
    float x, y, in[2] = {4.0f, 8.0f};
    worldToMap__3mapP6Point2PfT2(in, &x, &y);
    expect(x == k && y == 2.0f * k, "scale constant bits 0x3d088888");
    return failures ? 1 : 0;
}
