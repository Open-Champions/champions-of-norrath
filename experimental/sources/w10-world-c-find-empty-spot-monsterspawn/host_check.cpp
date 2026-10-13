// Host contract for objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off -DNORRATH_HOST_ARENA host_check.cpp find_empty_spot_monsterspawn.cpp
#include <cmath>
#include <cstdio>
#include <cstring>
#include <functional>
#include <vector>

struct ListedObject { unsigned char before_flags[12]; unsigned flags; float x, y; unsigned char gap[4]; float edge; };
extern "C" float* objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(float*, const float*, int, int, int, int, char*);
extern "C" {
unsigned eRandState, syncRandState;
unsigned char* world = reinterpret_cast<unsigned char*>(0x1234);
}
static std::vector<ListedObject> objects;
extern "C" const ListedObject* norrath_host_listed_object(unsigned address) { return &objects[address - 1]; }

struct Probe { float p[3]; float radius; };
static std::vector<Probe> forbidden_calls, height_calls;
static std::vector<std::vector<unsigned>> box_script;
static int box_calls, trig_calls, last_angle;
static float low_seen[3], high_seen[3];
static std::function<float(int, const float*)> forbidden_fn, height_fn;
extern "C" float isin__Fi(int angle) { ++trig_calls; last_angle = angle; return 0.25f * (float)(angle % 7 + 1); }
extern "C" float icos__Fi(int angle) { ++trig_calls; return -0.5f * (float)(angle % 5 + 1); }
extern "C" float worldTestForbidden__FP12_worldHeaderG6Point3f(unsigned char* w, const float* p, float r) {
    if (w != world) return 99.0f;
    forbidden_calls.push_back({{p[0], p[1], p[2]}, r});
    return forbidden_fn(int(forbidden_calls.size()) - 1, p);
}
extern "C" float worldFindHeight__FP12_worldHeaderG6Point3f(unsigned char* w, const float* p, float r) {
    if (w != world) return 1e9f;
    height_calls.push_back({{p[0], p[1], p[2]}, r});
    return height_fn(int(height_calls.size()) - 1, p);
}
extern "C" int objectFindInBox__FG6Point3T0PP10GameObjecti(const float* low, const float* high, unsigned* out, int limit) {
    for (int i = 0; i != 3; ++i) { low_seen[i] = low[i]; high_seen[i] = high[i]; }
    const auto& ids = box_script[box_calls < int(box_script.size()) ? box_calls : int(box_script.size()) - 1];
    ++box_calls;
    unsigned n = 0;
    for (unsigned id : ids) out[n++] = id;
    out[n] = 0;
    return limit == 128 ? int(n) : -1;
}

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::printf("fail: %s\n", m); ++failures; } }

// Independent description of the random draws.
static unsigned lcg(unsigned s) { return s * 1664525u + 1013904223u; }
static void sphere(unsigned& state, float v[3]) {
    for (;;) {
        for (int i = 0; i != 3; ++i) { state = lcg(state); v[i] = float(state >> 16) / 65536.0f * 2.0f - 1.0f; }
        if (!(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] > 1.0f)) return;
    }
}
static float draw_range(unsigned& e, int range) {
    e = lcg(e);
    return (float(e >> 16) / 65536.0f * float(range)) * 2.0f - float(range);
}

int main() {
    const float centre[3] = {100.0f, -50.0f, 10.0f};
    float out[3];
    char found = 7;
    auto reset = [&]() {
        forbidden_calls.clear(); height_calls.clear(); box_calls = 0; box_script = {{}}; trig_calls = 0;
        std::memset(out, 0, sizeof out); found = 7; objects.clear();
    };
    // 1. Accepted on the first attempt.
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 12.5f; };
    eRandState = 777; syncRandState = 12345;
    unsigned e = 777, s = 12345; float u[3];
    const float m = draw_range(e, 40);
    sphere(s, u);
    const float sx = 0.25f * (float)(3 % 7 + 1) * m, sy = -0.5f * (float)(3 % 5 + 1) * m;
    float* r = objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(out, centre, 36, 40, 3, 24, &found);
    expect(r == out, "returns the result pointer");
    expect(eRandState == e && syncRandState == s, "both generator states advanced and written back");
    expect(last_angle == 3 && trig_calls == 2, "angle passed to isin and icos");
    const float cx = (100.0f + sx) + u[0] * 36.0f, cy = (-50.0f + sy) + u[1] * 36.0f;
    expect(out[0] == cx && out[1] == cy && out[2] == 10.0f, "candidate: shift plus scaled unit point, z stays at the centre");
    expect(found == 1 && forbidden_calls.size() == 1 && height_calls.size() == 1 && box_calls == 1, "one attempt");
    expect(forbidden_calls[0].radius == 24.0f && forbidden_calls[0].p[0] == cx && height_calls[0].p[1] == cy && height_calls[0].p[2] == 10.0f,
           "probes use the candidate and the radius");
    expect(low_seen[0] == cx - 24.0f && low_seen[2] == 10.0f - 100000.0f && high_seen[1] == cy + 24.0f && high_seen[2] == 10.0f + 100000.0f,
           "box around the candidate, z extent 100000");
    // 2. Always forbidden: 50 attempts; the result is the last candidate; the generators advance 50 times.
    reset();
    forbidden_fn = [](int, const float*) { return 1.0f; };
    eRandState = 5; syncRandState = 99;
    e = 5; s = 99;
    float last[3] = {0, 0, 0};
    for (int i = 0; i != 50; ++i) {
        const float mm = draw_range(e, 40); sphere(s, u);
        const float ssx = 0.25f * (float)(3 % 7 + 1) * mm, ssy = -0.5f * (float)(3 % 5 + 1) * mm;
        last[0] = (100.0f + ssx) + u[0] * 36.0f; last[1] = (-50.0f + ssy) + u[1] * 36.0f; last[2] = 10.0f;
    }
    objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(out, centre, 36, 40, 3, 24, &found);
    expect(found == 0 && forbidden_calls.size() == 50 && height_calls.empty() && box_calls == 0, "fifty forbidden attempts, flag 0");
    expect(eRandState == e && syncRandState == s, "generator states after fifty attempts");
    expect(out[0] == last[0] && out[1] == last[1] && out[2] == last[2], "result is the last candidate tried");
    // 3. A null flag pointer is allowed.
    reset();
    forbidden_fn = [](int, const float*) { return 1.0f; };
    objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(out, centre, 36, 40, 3, 24, nullptr);
    expect(forbidden_calls.size() == 50, "null flag pointer: still fifty attempts");
    // 4. Height test: 16.0 (= centre + 6.0) is rejected, 15.99 accepted; the height is not stored.
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int n, const float*) { return n == 0 ? 16.0f : 15.99f; };
    objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(out, centre, 36, 40, 3, 24, &found);
    expect(found == 1 && height_calls.size() == 2 && box_calls == 1 && forbidden_calls.size() == 2, "height exactly 6.0 above is spent, just below is accepted");
    expect(out[2] == 10.0f && low_seen[2] == 10.0f - 100000.0f, "ground height does not change the candidate");
    // 5. Blocking objects. Learn the first candidate with an empty box, then place an object relative to it.
    reset();
    forbidden_fn = [](int, const float*) { return 0.0f; };
    height_fn = [](int, const float*) { return 11.0f; };
    auto run = [&]() {
        forbidden_calls.clear(); height_calls.clear(); box_calls = 0;
        eRandState = 31; syncRandState = 32;
        objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(out, centre, 36, 40, 3, 24, &found);
    };
    box_script = {{}};
    run();
    const float px = forbidden_calls[0].p[0], py = forbidden_calls[0].p[1];
    expect(forbidden_calls.size() == 1, "empty box accepts the first candidate");
    objects.push_back({});
    objects[0].edge = 8.0f; objects[0].y = py; objects[0].flags = 0;
    float ox = px + 32.0f;
    while (ox - px > 32.0f) ox = std::nextafter(ox, 0.0f);
    while (ox - px < 32.0f) ox = std::nextafter(ox, 1e9f);
    expect(ox - px == 32.0f, "test setup: object exactly 32.0 from the candidate");
    box_script = {{1}, {}};
    objects[0].x = ox;
    run();
    expect(found == 1 && forbidden_calls.size() == 2 && box_calls == 2, "distance 32 = radius 24 + edge 8 blocks (not above)");
    objects[0].x = std::nextafter(ox, 1e9f);
    run();
    expect(found == 1 && forbidden_calls.size() == 1 && box_calls == 1, "a hair farther does not block");
    objects[0].x = px; objects[0].flags = 1;
    run();
    expect(found == 1 && forbidden_calls.size() == 1, "flag bit 0 exempts an object standing on the candidate");
    objects[0].flags = 2;
    run();
    expect(found == 1 && forbidden_calls.size() == 2, "other flag bits do not exempt it");
    objects[0].flags = 0; objects[0].x = px; objects[0].edge = -24.0f;
    run();
    expect(forbidden_calls.size() == 2, "reach 0 with the object on the candidate: 0 <= 0 blocks");
    box_script = {{1}};
    objects[0].edge = 1.0e6f;
    run();
    expect(found == 0 && forbidden_calls.size() == 50 && box_calls == 50, "a huge object reaches every candidate: fifty attempts, flag 0");
    if (failures) return 1;
    std::printf("objectFindEmptySpotMonsterspawn contract passed\n");
    return 0;
}
