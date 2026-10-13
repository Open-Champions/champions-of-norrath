// Host contract for backup__7Vehicle. Not an EE plan input.
#include "backup.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

static int failures = 0;
static int order[8], steps;
static Quat mul_a, mul_b, norm_in;
static unsigned char* matrix_out;
static const void* seen_vehicle;

extern "C" void backupJustForces__7Vehicle(unsigned char* v) { order[steps++] = 1; seen_vehicle = v; }
extern "C" void quatMul__FP4QuatN20(Quat* out, const Quat* a, const Quat* b) {
    order[steps++] = 2; mul_a = *a; mul_b = *b;
    out->x = 10; out->y = 20; out->z = 30; out->w = 40;
}
extern "C" void quatNormalize__FP4Quat(Quat* q) { order[steps++] = 3; norm_in = *q; q->x = 7.0f; }
extern "C" void quatToMat__FP8Matrix33P4Quat(void* m, Quat* q) { order[steps++] = 4; matrix_out = static_cast<unsigned char*>(m); (void)q; }

static float& at(unsigned char* o, int off) { return *reinterpret_cast<float*>(o + off); }
static void expect(bool ok, const char* msg) { if (!ok) { std::fprintf(stderr, "fail: %s\n", msg); ++failures; } }
static bool close(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }

int main() {
    static unsigned char v[2304];
    std::memset(v, 0, sizeof v);
    const float C = 0x1.111110p-6f;
    at(v, 16) = 100; at(v, 20) = 200; at(v, 24) = 300;
    at(v, 1084) = 1; at(v, 1088) = 2; at(v, 1092) = 3; at(v, 1096) = 4;
    at(v, 1188) = 60; at(v, 1192) = -30; at(v, 1196) = 6;
    at(v, 1200) = 12; at(v, 1204) = -24; at(v, 1208) = 36;
    backup__7Vehicle(v);
    expect(seen_vehicle == v, "backupJustForces on the vehicle");
    expect(steps == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4, "call order");
    expect(close(at(v, 16), 100 - 60 * 39 * C) && close(at(v, 20), 200 + 30 * 39 * C) && close(at(v, 24), 300 - 6 * 39 * C), "position backed up by velocity * 39 / 60");
    expect(close(mul_b.x, 12 * -C * 0.5f) && close(mul_b.y, -24 * -C * 0.5f) && close(mul_b.z, 36 * -C * 0.5f) && mul_b.w == 0.0f, "spin quaternion");
    expect(mul_a.x == 1 && mul_a.y == 2 && mul_a.z == 3 && mul_a.w == 4, "product uses the old orientation");
    expect(norm_in.x == 11 && norm_in.y == 22 && norm_in.z == 33 && norm_in.w == 44, "orientation gains the product before normalising");
    expect(at(v, 1084) == 7.0f && matrix_out == v + 1116, "matrix rebuilt from the orientation at +1084");
    return failures ? 1 : 0;
}
