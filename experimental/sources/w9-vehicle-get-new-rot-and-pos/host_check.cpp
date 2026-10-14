// Host contract for getNewRotAndPos__7VehicleP6Point3T1fii. Not an EE plan input.
// With the identity matrix both computed angles are 0 degrees to rounding, so the
// +920 / +924 limit signs decide whether the spin components are cut to zero.
#include "get_new_rot_and_pos.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int order[8], steps;
static Point3 turn_seen;
static unsigned char* mat_in;
static int word_at_box, grid_calls;

extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    order[steps++] = 1; mat_in = (unsigned char*)m; *out = *in;
}
extern "C" void changeOrientation__7VehicleG6Point3(unsigned char*, const Point3* t) { order[steps++] = 2; turn_seen = *t; }
extern "C" void buildCollisionBox__7Vehicle(unsigned char* v) { order[steps++] = 3; std::memcpy(&word_at_box, v + 2164, 4); }
extern "C" void objectUpdateInGrid__FP10GameObject(void*) { order[steps++] = 4; ++grid_calls; }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool close(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

static void setup(unsigned char* v, float l1, float l2, float wx, float wy) {
    std::memset(v, 0, 2304);
    at(v, 1116) = 1; at(v, 1132) = 1; at(v, 1148 - 0) = 0;
    reinterpret_cast<float*>(v + 1116)[8] = 1;
    at(v, 16) = 10; at(v, 20) = 20; at(v, 24) = 30;
    at(v, 1188) = 1; at(v, 1192) = 2; at(v, 1196) = 3;
    at(v, 1200) = wx; at(v, 1204) = wy; at(v, 1208) = 0.5f;
    at(v, 672) = 2.0f; at(v, 1224) = 3.0f; at(v, 1228) = 4.0f; at(v, 1232) = 5.0f;
    at(v, 920) = l1; at(v, 924) = l2;
    std::memcpy(v + 2164, "\x07\0\0\0", 4);
}

int main() {
    static unsigned char v[2304];
    const Point3 first = {1, 2, 3}, second = {0.5f, 0.25f, 2.0f};
    const float dt = 0.5f;

    setup(v, 5.0f, 5.0f, 1.0f, 1.0f);
    steps = 0; grid_calls = 0;
    getNewRotAndPos__7VehicleP6Point3T1fii(v, &first, &second, dt, 99, 0);
    expect(close(at(v, 1188), 1 + 1 * 2 * dt) && close(at(v, 1192), 2 + 2 * 2 * dt) && close(at(v, 1196), 3 + 3 * 2 * dt), "velocity gains first * (+672) * step");
    expect(close(at(v, 1200), 1 + 0.5f * 3 * dt) && close(at(v, 1204), 1 + 0.25f * 4 * dt) && close(at(v, 1208), 0.5f + 2 * 5 * dt), "spin gains second * per-axis * step");
    expect(close(at(v, 16), 10 + at(v, 1188) * 39 * dt) && close(at(v, 20), 20 + at(v, 1192) * 39 * dt) && close(at(v, 24), 30 + at(v, 1196) * 39 * dt), "position advances by velocity * 39 * step");
    expect(close(turn_seen.x, at(v, 1200) * dt) && close(turn_seen.y, at(v, 1204) * dt) && close(turn_seen.z, at(v, 1208) * dt), "orientation turn is spin * step");
    expect(steps == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4, "call sequence");
    expect(mat_in == v + 1116, "matrix at +1116");
    expect(word_at_box == 0 && grid_calls == 1, "flag 0: +2164 cleared before the box rebuild, grid updated");

    setup(v, 5.0f, 5.0f, 1.0f, 1.0f);
    steps = 0; grid_calls = 0;
    getNewRotAndPos__7VehicleP6Point3T1fii(v, &first, &second, dt, 0, 1);
    expect(steps == 2 && grid_calls == 0 && v[2164] == 7, "non-zero flag skips the box and grid work");

    // Limits of -1: both bounds hold for an angle near 0, so both spin axes are cut.
    setup(v, -1.0f, -1.0f, 1.0f, 1.0f);
    getNewRotAndPos__7VehicleP6Point3T1fii(v, &first, &second, dt, 0, 1);
    expect(at(v, 1200) == 0.0f && at(v, 1204) == 0.0f, "spin cut when the limits are negative");
    setup(v, -1.0f, -1.0f, -4.0f, -4.0f);
    getNewRotAndPos__7VehicleP6Point3T1fii(v, &first, &second, dt, 0, 1);
    expect(at(v, 1200) == 0.0f && at(v, 1204) == 0.0f, "negative spin cut too");
    return failures ? 1 : 0;
}
