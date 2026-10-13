// Host contract for objectFindEmptySpot__FG6Point3iiPc. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp
#ifndef NORRATH_HOST_ARENA
#define NORRATH_HOST_ARENA
#endif
#include <cmath>
#include <functional>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "../w9-animation-object-c-common/host_check.hpp"
#include "find_empty_spot.cpp"
unsigned syncRandState;
unsigned char* world = reinterpret_cast<unsigned char*>(0x1234);
struct Probe { float p[3]; float radius; };
static std::vector<Probe> forbidden_calls, height_calls;
static std::vector<std::vector<unsigned char*>> box_script;   // objects returned per box call
static int box_calls;
static float low_seen[3], high_seen[3];
static std::function<float(int, const float*)> forbidden_fn, height_fn;
extern "C" float worldTestForbidden__FP12_worldHeaderG6Point3f(unsigned char* w, const float* point, float radius) {
    if (w != world) return 99.0f;
    forbidden_calls.push_back({{point[0], point[1], point[2]}, radius});
    return forbidden_fn(int(forbidden_calls.size()) - 1, point);
}
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char* w, const float* point, float radius) {
    if (w != world) return 1e9f;
    height_calls.push_back({{point[0], point[1], point[2]}, radius});
    return height_fn(int(height_calls.size()) - 1, point);
}
extern "C" int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned* out, int limit) {
    for (int i = 0; i != 3; ++i) { low_seen[i] = low[i]; high_seen[i] = high[i]; }
    const auto& objs = box_script[box_calls < int(box_script.size()) ? box_calls : box_script.size() - 1];
    ++box_calls;
    unsigned n = 0;
    for (auto* o : objs) out[n++] = norrath_host_address(o);
    out[n] = 0;
    return limit == 128 ? int(n) : -1;
}
// Independent draw: three generator steps, each mapped to [-1, 1); reject outside the unit sphere.
static void draw(unsigned& state, float v[3]) {
    for (;;) {
        for (int i = 0; i != 3; ++i) {
            state = state * 1664525u + 1013904223u;
            v[i] = float(state >> 16) / 65536.0f * 2.0f - 1.0f;
        }
        if (!(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] > 1.0f)) return;
    }
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } };
    unsigned char out[12];
    unsigned char centre[12];
    sf_(centre, 100.0f); sf_(centre + 4, -50.0f); sf_(centre + 8, 10.0f);
    char found = 7;
    auto reset = [&]() { forbidden_calls.clear(); height_calls.clear(); box_calls = 0; box_script = {{}}; std::memset(out, 0xee, 12); found = 7; };
    // 1. Everything accepted on the first draw.
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 12.5f; };
    syncRandState = 12345;
    unsigned state = 12345; float v[3]; draw(state, v);
    unsigned char* r = objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(r == out, "returns the result pointer");
    expect(syncRandState == state, "generator state written back after the draw");
    expect(lf_(out) == 100.0f + v[0] * 36.0f && lf_(out + 4) == -50.0f + v[1] * 36.0f && lf_(out + 8) == 12.5f, "result is the offset point with the ground height");
    expect(found == 1 && forbidden_calls.size() == 1 && height_calls.size() == 1 && box_calls == 1, "one attempt");
    expect(forbidden_calls[0].p[2] == 10.0f && forbidden_calls[0].radius == 24.0f && height_calls[0].p[2] == 10.0f, "probes use the centre height and the radius");
    expect(low_seen[0] == lf_(out) - 24.0f && low_seen[2] == 12.5f - 100000.0f && high_seen[1] == lf_(out + 4) + 24.0f && high_seen[2] == 12.5f + 100000.0f, "box around the candidate");
    // 2. Always forbidden: fifty attempts, result stays at the centre, flag 0, state advanced by the draws.
    reset();
    forbidden_fn = [](int, const float*) { return 1.0f; };
    syncRandState = 99;
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(found == 0 && forbidden_calls.size() == 50 && height_calls.empty() && box_calls == 0, "fifty forbidden attempts");
    expect(lf_(out) == 100.0f && lf_(out + 4) == -50.0f && lf_(out + 8) == 10.0f, "result is the centre after total failure");
    // 3. Null flag pointer is allowed.
    reset();
    forbidden_fn = [](int, const float*) { return 1.0f; };
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, nullptr);
    expect(forbidden_calls.size() == 50, "no flag pointer");
    // 4. Too high: height - centre.z must be below 6. Second call is allowed.
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int n, const float*) { return n < 2 ? 16.0f : 15.99f; };
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(height_calls.size() == 3 && found == 1, "a rise of exactly 6.0 is refused and 5.99 is accepted");
    // 5. Failing the height test must not move the result; failing the box test still does (ground height kept).
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 3.0f; };
    unsigned char* blocker = arena_at(0x1000);
    std::memset(blocker, 0, 0x100);
    sf_(blocker + 28, 0.0f);
    box_script = {{blocker}};
    // blocker sits exactly on the candidate each time: place after the first draw.
    syncRandState = 777; unsigned s2 = 777; float w[3]; draw(s2, w);
    sf_(blocker + 16, 100.0f + w[0] * 36.0f); sf_(blocker + 20, -50.0f + w[1] * 36.0f);
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(forbidden_calls.size() > 1 && found == 1, "a blocked first candidate is retried and eventually accepted when the blocker no longer overlaps");
    // 6. Objects with flag bit 0 are ignored; distance is compared with radius^2 + edge^2 (<= blocks).
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 3.0f; };
    syncRandState = 31337; unsigned s3 = 31337; draw(s3, w);
    float cx = 100.0f + w[0] * 36.0f, cy = -50.0f + w[1] * 36.0f;
    sw_(blocker + 12, 1); sf_(blocker + 16, cx); sf_(blocker + 20, cy);
    box_script = {{blocker}};
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(found == 1 && forbidden_calls.size() == 1, "flag bit 0 object is ignored");
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 3.0f; };
    syncRandState = 31337;
    sw_(blocker + 12, 0); sf_(blocker + 28, 10.0f);
    sf_(blocker + 16, cx + 26.0f); sf_(blocker + 20, cy);       // 26 away: 676 <= 576 + 100 blocks exactly
    box_script = {{blocker}, {}};
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(forbidden_calls.size() == 2 && found == 1, "equal squared distance blocks the first candidate");
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 3.0f; };
    syncRandState = 31337;
    sf_(blocker + 16, cx + 27.0f);
    box_script = {{blocker}};
    objectFindEmptySpot__FG6Point3iiPc(out, centre, 36, 24, &found);
    expect(forbidden_calls.size() == 1 && found == 1, "just outside the combined radius is free");
    if (failures) return 1;
    std::printf("objectFindEmptySpot check passed\n");
    return 0;
}
