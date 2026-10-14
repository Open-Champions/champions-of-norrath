// Host contract for addImpulse__7VehicleP6Point3T1i; not an EE input. matUntransform is a recording stub
// that returns a fixed linear function of its input and checks the matrix pointer.
#include "add_impulse.cpp"
#include <cstdio>
#include <cstring>
static unsigned char vehicle[2400];
static int calls; static Point3 seen_in; static const void* seen_matrix;
extern "C" void matUntransform__FP6Point3P8Matrix33T0(Point3* out, const void* m, const Point3* in) {
    ++calls; seen_in = *in; seen_matrix = m;
    out->x = in->x * 2.0f + 1.0f; out->y = in->y * -3.0f; out->z = in->z + in->x;
}
static float& F(int off) { return *reinterpret_cast<float*>(vehicle + off); }
int main() {
    int failures = 0;
    struct C { Point3 imp, pt; float k; } cases[] = {
        {{1, 2, 3}, {4, -5, 6}, 0.5f}, {{0, 0, 0}, {1, 1, 1}, 2.0f}, {{-7.5f, 0.25f, 9}, {0, 0, 0}, -1.0f}, {{1e3f, -1e-3f, 3}, {2, 3, 5}, 0.0f}};
    for (const C& c : cases) {
        std::memset(vehicle, 0, sizeof vehicle);
        F(672) = c.k;
        const Point3 v0 = {1, 2, 3}, a0 = {-4, 5, 0.5f}, g = {2, 0.5f, -1};
        std::memcpy(vehicle + 1188, &v0, 12); std::memcpy(vehicle + 1200, &a0, 12); std::memcpy(vehicle + 1224, &g, 12);
        calls = 0;
        addImpulse__7VehicleP6Point3T1i(vehicle, &c.imp, &c.pt, 99);
        const Point3 cr = {c.pt.y * c.imp.z - c.pt.z * c.imp.y, c.pt.z * c.imp.x - c.pt.x * c.imp.z, c.pt.x * c.imp.y - c.pt.y * c.imp.x};
        const Point3 wantV = {v0.x + c.imp.x * c.k, v0.y + c.imp.y * c.k, v0.z + c.imp.z * c.k};
        const Point3 loc = {(cr.x * 2.0f + 1.0f) * g.x, (cr.y * -3.0f) * g.y, (cr.z + cr.x) * g.z};
        const Point3 wantA = {a0.x + loc.x, a0.y + loc.y, a0.z + loc.z};
        Point3 gotV, gotA; std::memcpy(&gotV, vehicle + 1188, 12); std::memcpy(&gotA, vehicle + 1200, 12);
        int flag; std::memcpy(&flag, vehicle + 2144, 4);
        if (calls != 1 || seen_matrix != vehicle + 1116 || std::memcmp(&seen_in, &cr, 12) != 0 || std::memcmp(&gotV, &wantV, 12) != 0 ||
            std::memcmp(&gotA, &wantA, 12) != 0 || flag != 1) { std::fprintf(stderr, "case differs\n"); ++failures; }
    }
    if (!failures) std::puts("addImpulse ok");
    return failures != 0;
}
