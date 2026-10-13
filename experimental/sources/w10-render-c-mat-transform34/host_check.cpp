// Host contract for matTransform__FP6Point3P8Matrix34T0. Not an EE plan input.
#include <cmath>
#include <cstdio>
struct Point3 { float x, y, z; };
struct Matrix34 { float m[12]; };
extern "C" void matTransform__FP6Point3P8Matrix34T0(Point3*, const Matrix34*, const Point3*);
int main() {
    Matrix34 id{{1, 0, 0, 0, 1, 0, 0, 0, 1, 10, 20, 30}};
    Point3 in{1, 2, 3}, out{};
    matTransform__FP6Point3P8Matrix34T0(&out, &id, &in);
    if (out.x != 11 || out.y != 22 || out.z != 33) return 1;
    // distinguishes row from column use: m[3] multiplies y into out.x
    Matrix34 m{{1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 200, 300}};
    matTransform__FP6Point3P8Matrix34T0(&out, &m, &in);
    if (out.x != 1 + 8 + 21 + 100 || out.y != 2 + 10 + 24 + 200 || out.z != 3 + 12 + 27 + 300) return 2;
    matTransform__FP6Point3P8Matrix34T0(&in, &m, &in);   // aliased
    if (in.x != 130 || in.y != 236 || in.z != 342) return 3;
    std::printf("matTransform34 contract passed\n");
    return 0;
}
