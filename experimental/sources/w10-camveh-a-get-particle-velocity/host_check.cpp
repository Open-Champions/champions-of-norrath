// Host contract for getParticleVelocity__7VehicleP6Point3. Not an EE plan input.
#include "get_particle_velocity.cpp"

#include <cstdio>
#include <cstring>

static const void* seen_matrix;
static const Point3* seen_in;
static Point3 transformed = {1, 2, 3};
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    seen_matrix = m; seen_in = in; *out = transformed;
}

static void put(unsigned char* o, int off, float a, float b, float c) {
    float v[3] = {a, b, c};
    std::memcpy(o + off, v, 12);
}

int main() {
    int failures = 0;
    unsigned char vehicle[2304];
    std::memset(vehicle, 0, sizeof vehicle);
    put(vehicle, 16, 10, 20, 30);
    put(vehicle, 1188, 100, 200, 300);
    put(vehicle, 1200, 7, 8, 9);
    const Point3 p = {10 + 39.0f, 20 + 78.0f, 30 + 117.0f};  // d is about (1, 2, 3) after scaling
    Point3 out = {-1, -1, -1};
    Point3* r = getParticleVelocity__7VehicleP6Point3(&out, vehicle, &p);
    const float k = 0x1.a41a40p-6f;
    const Point3 d = {(p.x - 10) * k, (p.y - 20) * k, (p.z - 30) * k};
    const Point3 a = transformed;
    const Point3 want = {a.y * d.z - a.z * d.y + 100, a.z * d.x - a.x * d.z + 200, a.x * d.y - a.y * d.x + 300};
    if (r != &out) { std::fprintf(stderr, "fail: return pointer\n"); ++failures; }
    if (out.x != want.x || out.y != want.y || out.z != want.z) { std::fprintf(stderr, "fail: result %g %g %g want %g %g %g\n", out.x, out.y, out.z, want.x, want.y, want.z); ++failures; }
    if (seen_matrix != vehicle + 1116 || seen_in != reinterpret_cast<Point3*>(vehicle + 1200)) { std::fprintf(stderr, "fail: transform args\n"); ++failures; }
    // the cross product terms are not symmetric: a unit rotation about z gives a pure x/y result
    transformed = {0, 0, 1};
    getParticleVelocity__7VehicleP6Point3(&out, vehicle, &p);
    if (out.x != 100 - d.y || out.y != 200 + d.x || out.z != 300) { std::fprintf(stderr, "fail: z axis\n"); ++failures; }
    return failures ? 1 : 0;
}
