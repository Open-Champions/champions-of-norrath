// Host contract for quatMul__FP4QuatN20. Not an EE plan input.
#include <cmath>
#include <cstdio>
struct Quat { float x, y, z, w; };
extern "C" void quatMul__FP4QuatN20(Quat*, const Quat*, const Quat*);
static int near(const Quat& q, float x, float y, float z, float w) {
    return std::fabs(q.x - x) < 1e-6f && std::fabs(q.y - y) < 1e-6f &&
           std::fabs(q.z - z) < 1e-6f && std::fabs(q.w - w) < 1e-6f;
}
int main() {
    Quat one{0, 0, 0, 1}, i{1, 0, 0, 0}, j{0, 1, 0, 0}, k{0, 0, 1, 0}, r{};
    quatMul__FP4QuatN20(&r, &one, &i);
    if (!near(r, 1, 0, 0, 0)) return 1;
    quatMul__FP4QuatN20(&r, &i, &j);          // i*j = k
    if (!near(r, 0, 0, 1, 0)) return 2;
    quatMul__FP4QuatN20(&r, &j, &i);          // j*i = -k
    if (!near(r, 0, 0, -1, 0)) return 3;
    quatMul__FP4QuatN20(&r, &i, &i);          // i*i = -1
    if (!near(r, 0, 0, 0, -1)) return 4;
    quatMul__FP4QuatN20(&r, &j, &k);          // j*k = i
    if (!near(r, 1, 0, 0, 0)) return 5;
    Quat a{1, 2, 3, 4}, b{5, 6, 7, 8};
    quatMul__FP4QuatN20(&r, &a, &b);          // known Hamilton product
    if (!near(r, 24, 48, 48, -6)) return 6;
    quatMul__FP4QuatN20(&a, &a, &b);          // destination aliases left operand
    if (!near(a, 24, 48, 48, -6)) return 7;
    Quat lhs{1, 2, 3, 4}, c{5, 6, 7, 8};
    quatMul__FP4QuatN20(&c, &lhs, &c);        // destination aliases right operand
    if (!near(c, 24, 48, 48, -6)) return 8;
    std::printf("quatMul contract passed\n");
    return 0;
}
