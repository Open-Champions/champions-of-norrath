// Host contract for segmentLineHit__FP6Point3N50. Not an EE plan input.
#include <cstdio>
struct Point3 { float x, y, z; };
extern "C" int segmentLineHit__FP6Point3N50(const Point3*, const Point3*, const Point3*, const Point3*, const Point3*, Point3*);
int main() {
    Point3 p0{0, 0, 0}, p1{-1, 0, 0}, c{0, 0, 1}, e{0.5f, 0, -1}, out{};
    // w = p3 - e = (0,0,2) -> den = 2 >= 0 -> miss
    Point3 p3{0.5f, 0, 1};
    if (segmentLineHit__FP6Point3N50(&p0, &p1, &c, &p3, &e, &out) != 0) return 1;
    // swap so the segment points down: w = (0,0,-2) from e=(0.5,0,1) towards z=-1
    Point3 e2{0.1f, 0, 1}, p3b{0.1f, 0, -1};
    if (segmentLineHit__FP6Point3N50(&p0, &p1, &c, &p3b, &e2, &out) != 1) return 2;
    if (out.x != 0.1f || out.y != 0 || out.z != 0) return 3;   // s = 0.5, point (0.1,0,0)
    // plane crossing beyond the segment (s > 1)
    Point3 p3c{0.1f, 0, 0.5f};
    if (segmentLineHit__FP6Point3N50(&p0, &p1, &c, &p3c, &e2, &out) != 0) return 4;
    // beyond the span: hit point far past p0 along p0-p1 (proj > len + 0.25)
    Point3 e3{3.0f, 0, 1}, p3d{3.0f, 0, -1};
    if (segmentLineHit__FP6Point3N50(&p0, &p1, &c, &p3d, &e3, &out) != 0) return 5;
    // behind p1 (proj < -0.25)
    Point3 e4{-3.0f, 0, 1}, p3e{-3.0f, 0, -1};
    if (segmentLineHit__FP6Point3N50(&p0, &p1, &c, &p3e, &e4, &out) != 0) return 6;
    // tiny denominator is clamped to -0.01: s = 0.5 instead of 5
    Point3 e5{0.1f, 0, 0.005f}, p3f{0.1f, 0, 0.004f};
    if (segmentLineHit__FP6Point3N50(&p0, &p1, &c, &p3f, &e5, &out) != 1) return 7;
    if (out.z < 0.0044f || out.z > 0.0046f) return 8;
    std::printf("segmentLineHit contract passed\n");
    return 0;
}
