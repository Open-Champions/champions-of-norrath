// Host contract for distanceToPlane2__FP6Point3P6Point2T0. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
struct Point3 { float x, y, z; };
struct Point2 { float x, y; };
extern "C" float distanceToPlane2__FP6Point3P6Point2T0(const Point3*, const Point2*, const Point3*);
static bool same(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Point3 o = {1.0f, 2.0f, 100.0f}, p = {4.0f, 8.0f, -50.0f};
    Point2 n = {0.5f, 0.25f};
    expect(same(distanceToPlane2__FP6Point3P6Point2T0(&o, &n, &p), 0.5f * 3.0f + 0.25f * 6.0f), "x and y terms");
    Point2 nx = {1.0f, 0.0f}, ny = {0.0f, 1.0f};
    expect(same(distanceToPlane2__FP6Point3P6Point2T0(&o, &nx, &p), 3.0f), "x axis only");
    expect(same(distanceToPlane2__FP6Point3P6Point2T0(&o, &ny, &p), 6.0f), "y axis only");
    expect(same(distanceToPlane2__FP6Point3P6Point2T0(&p, &n, &o), -(0.5f * 3.0f + 0.25f * 6.0f)), "swapped points negate");
    Point3 q = o; q.z = 9999.0f;
    expect(same(distanceToPlane2__FP6Point3P6Point2T0(&o, &n, &q), 0.0f), "z ignored");
    if (failures) return 1;
    std::printf("distanceToPlane2 contract passed\n");
    return 0;
}
