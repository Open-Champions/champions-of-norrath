// Host contract for rayHit__FP6Point3N30. Not an EE plan input.
#include <cstdio>
struct Point3 { float x, y, z; };
extern "C" float rayHit__FP6Point3N30(const Point3*, const Point3*, const Point3*, const Point3*);
int main() {
    Point3 p0{0, 0, 5}, p2{0, 0, 0}, a{0, 0, -1}, n{0, 0, 1};
    // d = -1, t = 5, p = (0,0,5), e = -((0,0,5).(0,0,-1)) = 5 -> 5 * (5 / 5) = 5
    if (rayHit__FP6Point3N30(&p0, &a, &p2, &n) != 5.0f) return 1;
    Point3 up{0, 0, 1};                      // d = +1 -> -1
    if (rayHit__FP6Point3N30(&p0, &up, &p2, &n) != -1.0f) return 2;
    Point3 below{0, 0, -5};                  // t < 0 -> -1
    if (rayHit__FP6Point3N30(&below, &a, &p2, &n) != -1.0f) return 3;
    // small |e| path: a nearly perpendicular to p = n*t
    Point3 flat{1e-5f, 0, -1e-5f};           // d = -1e-5 < 0, e = 5e-5 < 0.001 -> t*(t*1000)
    if (rayHit__FP6Point3N30(&p0, &flat, &p2, &n) != 5.0f * (5.0f * 0x1.f40002p+9f)) return 4;
    std::printf("rayHit contract passed\n");
    return 0;
}
