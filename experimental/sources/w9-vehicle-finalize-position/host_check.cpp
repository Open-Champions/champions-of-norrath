// Host contract for finalizePosition__7Vehicle. Not an EE plan input. The
// pointer arrays (+1052.., +2056..) have 4-byte target strides, so 8-byte host
// pointers would overlap; every scenario here therefore uses one contact
// (n = 1). The two-contact loops are covered by the private paired run.
#include "finalize_position.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

int frameNm;

static int failures = 0;
static int order[64], steps;
static float ray_script[4];
static int ray_calls, impulse_calls, impulse_result, wheel_final_calls, resolve_calls;
static float ray_distance[4];
static Point3 impulse_normal, impulse_velocity;
static float impulse_factor;
static void* wheel_seen[4];

extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void*, const Point3* in) { order[steps++] = 1; *out = {in->x * 2, in->y * 2, in->z * 2}; }
extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void*, const Point3* in) { order[steps++] = 2; *out = {in->x * 3, in->y * 3, in->z * 3}; }
extern "C" void objectUpdateInGrid__FP10GameObject(void*) { order[steps++] = 3; }
extern "C" float rayHit__7VehicleP6Point3T1fT1(unsigned char*, const Point3*, const Point3*, float d, Point3* hit) {
    order[steps++] = 4; ray_distance[ray_calls] = d; *hit = {0.25f + ray_calls, 1.5f, 2.5f}; return ray_script[ray_calls++];
}
extern "C" void getParticleVelocity__7VehicleP6Point3(Point3* r, unsigned char*, const Point3* p) { order[steps++] = 5; *r = {p->x - 1, p->y - 2, p->z - 3}; }
extern "C" int impulse__7VehicleP6Point3N21fii(unsigned char*, const Point3*, const Point3* n, const Point3* v, float f, int a, int b) {
    order[steps++] = 6; ++impulse_calls; impulse_normal = *n; impulse_velocity = *v; impulse_factor = f;
    if (a != 0 || b != 0) ++failures;
    return impulse_result;
}
extern "C" void FinalizePosition__5Wheel(void* w) { order[steps++] = 7; wheel_seen[wheel_final_calls++] = w; }
extern "C" void resolveCreatureCollisions__7Vehicle(unsigned char*) { order[steps++] = 8; ++resolve_calls; }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool close(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

static unsigned char v[2400];
static float q_data[2][8];
static unsigned char wheel_obj[2][8];

static void setup(float f0, float f1) {
    std::memset(v, 0, sizeof v);
    steps = ray_calls = impulse_calls = wheel_final_calls = resolve_calls = 0;
    ray_script[0] = f0; ray_script[1] = f1;
    *reinterpret_cast<int*>(v + 1000) = 1;
    at(v, 16) = 10; at(v, 20) = 20; at(v, 24) = 30;
    for (int i = 0; i < 6; ++i) at(v, 1236 + 4 * i) = float(i + 1);
    at(v, 1140) = 1; at(v, 1144) = 2; at(v, 1148) = 3;
    at(v, 676) = 0.75f; at(v, 680) = 0.5f;
    at(v, 1188) = 3; at(v, 1192) = 4; at(v, 1196) = 0;
    q_data[0][0] = 2.0f; q_data[1][0] = 0.5f;
    unsigned char* q0 = reinterpret_cast<unsigned char*>(q_data[0]);
    unsigned char* q1 = reinterpret_cast<unsigned char*>(q_data[1]);
    std::memcpy(v + 2056, &q0, sizeof q0); (void)q1;
    unsigned char* w0 = wheel_obj[0]; unsigned char* w1 = wheel_obj[1];
    std::memcpy(v + 1052, &w0, sizeof w0); (void)w1;
    frameNm = 4242;
}

int main() {
    // The contact hits at 0.25.
    setup(0.25f, 0.0f);
    expect(finalizePosition__7Vehicle(v) == 0, "returns 0");
    expect(close(at(v, 1332), 2 + 10) && close(at(v, 1336), 4 + 20) && close(at(v, 1340), 6 + 30), "wheel point = transform + position");
    expect(at(v, 1428) == -1 && at(v, 1432) == -2 && at(v, 1436) == -3, "direction is the negated third row");
    expect(ray_calls == 1 && close(ray_distance[0], 2.0f * 39), "ray distance is float(*q) * 39");
    expect(close(at(v, 676), 0.75f), "ray hit: stored 1 - fraction");
    expect(at(v, 708) == 0.75f, "previous value moved to +708");
    expect(at(v, 1740) == 0.25f && at(v, 1744) == 1.5f && at(v, 1748) == 2.5f, "hit point stored at +1740");
    expect(wheel_final_calls == 1 && wheel_seen[0] == wheel_obj[0] && resolve_calls == 1, "wheel finalised, creatures resolved");
    expect(close(at(v, 840), 5.0f) && *reinterpret_cast<int*>(v + 2160) == 4242, "speed and frame number");
    expect(close(at(v, 1440), at(v, 1332) + (-1) * 0.25f * 78.0f), "slot: wheel point + direction * (1 - A) * float(*q) * 39");
    expect(close(at(v, 1444), at(v, 1336) + (-2) * 0.25f * 78.0f), "slot y");
    expect(close(at(v, 1536), 3 * (at(v, 1440) - 10)), "un-rotated offset from the position");
    expect(close(at(v, 1836), at(v, 1440) - 1), "particle velocity stored at +1836");
    expect(impulse_calls == 0, "no impulse for a positive fraction");
    // A miss leaves the stored value cleared and the old one moved.
    setup(-9999.0f, 0.0f);
    finalizePosition__7Vehicle(v);
    expect(at(v, 676) == 0.0f && at(v, 708) == 0.75f && at(v, 1740) == 0.0f, "miss: cleared, previous kept at +708, no hit stored");
    expect(close(at(v, 1440), at(v, 1332) + (-1) * 1.0f * 78.0f), "miss: factor is 1");

    // Negative fraction: impulse runs; a non-zero result returns 1 at once.
    setup(-0.5f, 0.0f); impulse_result = 5;
    expect(finalizePosition__7Vehicle(v) == 1, "impulse non-zero returns 1");
    expect(impulse_calls == 1 && impulse_factor == 0.5f && impulse_normal.x == 0.25f, "impulse arguments");
    expect(ray_calls == 1 && resolve_calls == 0 && *reinterpret_cast<int*>(v + 2160) == 0, "early return skips the tail");
    setup(-0.5f, 0.0f); impulse_result = 0;
    expect(finalizePosition__7Vehicle(v) == 0, "impulse zero continues");
    expect(at(v, 676) == 1.0f, "negative fraction counts as 0: stored 1.0");
    // Fraction of 1.0 or more is ignored.
    setup(1.0f, 7.0f);
    finalizePosition__7Vehicle(v);
    expect(at(v, 676) == 0.0f && impulse_calls == 0, "fraction >= 1 leaves the value cleared");
    // No contacts.
    setup(0, 0); *reinterpret_cast<int*>(v + 1000) = 0;
    finalizePosition__7Vehicle(v);
    expect(ray_calls == 0 && resolve_calls == 1 && order[0] == 3, "zero contacts: grid update then resolve");
    return failures ? 1 : 0;
}
