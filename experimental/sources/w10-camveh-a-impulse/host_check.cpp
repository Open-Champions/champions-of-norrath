// Host contract for impulse__7VehicleP6Point3N21fii. Not an EE plan input.
#include "impulse.cpp"

#include <cstdio>
#include <cstring>
#include <vector>

static std::vector<int> order;   // 1 untransform, 2 transform, 3 backup, 4 addImpulse
static Point3 untransform_in, transform_in, add_impulse, add_point;
static const void* untransform_matrix; static const void* transform_matrix;
static unsigned char* backup_arg; static unsigned char* add_arg; static int add_flag;
static Point3 untransform_result = {1, 2, 3}, transform_result = {0.5f, -1, 2};

extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    order.push_back(1); untransform_in = *in; untransform_matrix = m; *out = untransform_result;
}
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    order.push_back(2); transform_in = *in; transform_matrix = m; *out = transform_result;
}
extern "C" void backup__7Vehicle(unsigned char* v) { order.push_back(3); backup_arg = v; }
extern "C" void addImpulse__7VehicleP6Point3T1i(unsigned char* v, const Point3* i, const Point3* p, int flag) {
    order.push_back(4); add_arg = v; add_impulse = *i; add_point = *p; add_flag = flag;
}

static void put(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static bool same(const Point3& a, const Point3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
static Point3 cross3(const Point3& a, const Point3& b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

int main() {
    int failures = 0;
    const float k = 0x1.a41a40p-6f;
    unsigned char v[2304];
    std::memset(v, 0, sizeof v);
    put(v, 16, 1); put(v, 20, 2); put(v, 24, 3);
    put(v, 672, 0.25f); put(v, 1224, 2); put(v, 1228, 4); put(v, 1232, 8);
    const Point3 point = {11, 22, 33};
    const Point3 normal = {0, 0, 1};
    const Point3 vel = {5, 6, -7};
    for (int flat = 0; flat < 2; ++flat) {
        for (int skip = 0; skip < 2; ++skip) {
            order.clear();
            const float e = 0.5f;
            const int r = impulse__7VehicleP6Point3N21fii(v, &point, &normal, &vel, e, skip, flat);
            // v = vel * -(1.5): (-7.5, -9, 10.5); s = 10.5 (or 0 when flat -> still not < 0)
            const float scale = -(e + 1.0f);
            Point3 vv = {vel.x * scale, vel.y * scale, vel.z * scale};
            if (flat) vv.z = 0;
            const float s = (vv.x * normal.x + vv.y * normal.y) + vv.z * normal.z;
            Point3 rr = {(point.x - 1) * k, (point.y - 2) * k, flat ? 0.0f : (point.z - 3) * k};
            const Point3 t = cross3(rr, normal);
            const Point3 l = {untransform_result.x * 2, untransform_result.y * 4, untransform_result.z * 8};
            const Point3 u = cross3(transform_result, rr);
            const float d = 0.25f + ((u.x * normal.x + u.y * normal.y) + u.z * normal.z);
            const float j = s / d;
            const Point3 imp = {normal.x * j, normal.y * j, normal.z * j};
            std::vector<int> want_order = {1, 2};
            if (!skip) want_order.push_back(3);
            want_order.push_back(4);
            if (r != 1 || order != want_order) { std::fprintf(stderr, "fail: flow flat %d skip %d\n", flat, skip); ++failures; }
            if (!same(untransform_in, t) || untransform_matrix != v + 1116 || transform_matrix != v + 1116) { std::fprintf(stderr, "fail: torque input\n"); ++failures; }
            if (!same(transform_in, l)) { std::fprintf(stderr, "fail: inertia scaling\n"); ++failures; }
            if (!same(add_impulse, imp) || !same(add_point, rr) || add_flag != 1 || add_arg != v || (!skip && backup_arg != v)) { std::fprintf(stderr, "fail: add impulse flat %d skip %d\n", flat, skip); ++failures; }
        }
    }
    // approaching the wrong way: s < 0 returns 0 without calling anything
    order.clear();
    const Point3 away = {0, 0, 5};
    if (impulse__7VehicleP6Point3N21fii(v, &point, &normal, &away, 0.5f, 0, 0) != 0 || !order.empty()) { std::fprintf(stderr, "fail: s < 0\n"); ++failures; }
    return failures ? 1 : 0;
}
