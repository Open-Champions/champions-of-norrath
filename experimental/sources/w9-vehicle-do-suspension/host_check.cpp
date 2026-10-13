// Host contract for doSuspension__7Vehicle. Not an EE plan input. The pointer
// array at +2056 (4-byte stride) would overlap with 8-byte host pointers, so
// one wheel is used; the paired run against the original covers more.
#include "do_suspension.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int order[16], steps;
struct Accum { const Point3* point; Point3 force; int flag; };
static Accum accum[8]; static int accum_calls;
static float step_seen; static Point3 state_at_step[4];
static int new_flag;

extern "C" void accumForceAndTorque__7VehicleP6Point3T1i(unsigned char*, const Point3* p, const Point3* f, int flag) { order[steps++] = 1; accum[accum_calls++] = {p, *f, flag}; }
extern "C" void getNewRotAndPos__7VehicleP6Point3T1fii(unsigned char* v, const Point3* a, const Point3* b, float step, int unused, int flag) {
    order[steps++] = 2; step_seen = step; new_flag = flag; (void)unused;
    if (a != reinterpret_cast<Point3*>(v + 2016) || b != reinterpret_cast<Point3*>(v + 2028)) ++failures;
    float* f = reinterpret_cast<float*>(v);
    f[16 / 4] = 99; f[1188 / 4] = 77; f[1200 / 4] = 66; f[1084 / 4] = 55;   // state the routine must put back
    f[1116 / 4] = 11; f[1140 / 4] = 0; f[1144 / 4] = 0; f[1148 / 4] = -2;   // predicted matrix, third row (copied to +1176..)
}
extern "C" void quatToMat__FP8Matrix33P4Quat(void* m, Quat* q) { order[steps++] = 3; (void)m; state_at_step[0] = {q->x, q->y, q->z}; }
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void*, const Point3* in) { order[steps++] = 4; *out = {in->x + 1, in->y + 2, in->z + 3}; }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static int& wd(unsigned char* o, int off) { return *reinterpret_cast<int*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }

static unsigned char v[2600];
static float q[4];

static void setup(float a, float b, int w2144) {
    std::memset(v, 0, sizeof v); steps = accum_calls = 0;
    wd(v, 1000) = 1; wd(v, 2144) = w2144;
    q[0] = 2.0f; q[1] = 3.0f; q[2] = 5.0f; q[3] = 0.5f;
    float* qp = q; std::memcpy(v + 2056, &qp, sizeof qp);
    at(v, 676) = a; at(v, 708) = b;
    at(v, 1428) = 0; at(v, 1432) = 0; at(v, 1436) = 1;
    at(v, 16) = 1; at(v, 20) = 2; at(v, 24) = 3; at(v, 1188) = 4; at(v, 1192) = 5; at(v, 1196) = 6; at(v, 1200) = 7; at(v, 1204) = 8; at(v, 1208) = 9;
    at(v, 1084) = 0.1f; at(v, 1088) = 0.2f; at(v, 1092) = 0.3f; at(v, 1096) = 0.4f;
    at(v, 1440) = 0; at(v, 1444) = 0; at(v, 1448) = 0;
    at(v, 2016) = 5; at(v, 2028) = 5;
}

int main() {
    // A = 0.5, B = 0.25, word 2144 clear: S = 0.5*-3 + ((0.5-0.25)*60) * -(5) because A >= B -> factor q2 (A < B false).
    setup(0.5f, 0.25f, 0);
    doSuspension__7Vehicle(v);
    const float S = 0.5f * -3.0f + ((0.5f - 0.25f) * 60.0f) * -5.0f;
    expect(near(accum[0].force.z, S) && accum[0].flag == 1 && accum[0].point == (Point3*)(v + 1536), "first force: direction * S at the wheel point, flag 1");
    expect(order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4, "order: accumulate, integrate, quatToMat, matTransform");
    expect(near(step_seen, 0x1.111110p-6f) && new_flag == 1, "integration step and flag");
    expect(at(v, 2016) == 0 && at(v, 2028) == 0, "accumulators cleared before the first force");
    // State restored.
    expect(at(v, 16) == 1 && at(v, 1188) == 4 && at(v, 1200) == 7 && near(at(v, 1084), 0.1f) && near(at(v, 1096), 0.4f), "position, velocities and orientation put back");
    expect(near(state_at_step[0].x, 0.1f), "matrix rebuilt from the restored orientation");
    expect(at(v, 644) == 99 && at(v, 1152) == 11, "predicted position and matrix kept at +644 and +1152");
    expect(at(v, 608) == 4 && at(v, 632) == 7 && at(v, 620) == 1 && near(at(v, 1100), 0.1f), "saved copies at +608, +632, +620, +1100");
    // Second phase: g = -(0, 0, -2) = (0, 0, 2); T = A + g.d / (q0 * 39).
    // p = (stub transform (1,2,3)) + predicted position (99, 2, 3) = (100, 4, 6); rest point is zero.
    const float T = 0.5f + (2.0f * 6.0f) / (2.0f * 39.0f);
    const float U = T * -3.0f + ((T - 0.25f) * 60.0f) * -(T < 0.25f ? 5.0f * 0.5f : 5.0f);
    expect(accum_calls == 2 && accum[1].flag == 0, "second force with flag 0");
    expect(near(accum[1].force.z, (U + S) * 0.5f), "force = direction * ((U + S) * 0.5)");
    // A = 0 and T = 0: no force at all, S stays zero.
    setup(0.0f, 0.25f, 0);
    doSuspension__7Vehicle(v);
    expect(accum_calls == 0 && order[0] == 2, "no wheel contact: no forces");
    // Word 2144 set: only the A * -q1 term.
    setup(0.5f, 0.25f, 1);
    doSuspension__7Vehicle(v);
    expect(near(accum[0].force.z, 0.5f * -3.0f), "word 2144 set: spring term only");
    // A < B uses q2 * q3.
    setup(0.25f, 0.5f, 0);
    doSuspension__7Vehicle(v);
    expect(near(accum[0].force.z, 0.25f * -3.0f + ((0.25f - 0.5f) * 60.0f) * -(5.0f * 0.5f)), "A < B: factor q2 * q3");
    return failures ? 1 : 0;
}
