// Host contract for matToQuaternion__FP4QuatP8Matrix34. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off mat_to_quaternion.cpp \
//        ../w8-object-list-math-quat-to-mat33/quat_to_mat33.cpp host_check.cpp
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../w8-object-list-math-common/matrix_types.hpp"
extern "C" void matToQuaternion__FP4QuatP8Matrix34(Quat*, const Matrix34*);
extern "C" void quatToMat__FP8Matrix33P4Quat(Matrix33*, const Quat*);

static Matrix34 widen(const Matrix33& a) {
    Matrix34 b;
    std::memcpy(b.m, a.m, sizeof a.m);
    b.m[9] = 5.0f; b.m[10] = 6.0f; b.m[11] = 7.0f;
    return b;
}

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Quat q;

    Matrix34 eye = {{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0}};
    matToQuaternion__FP4QuatP8Matrix34(&q, &eye);
    expect(q.x == 0 && q.y == 0 && q.z == 0 && q.w == 1, "identity gives (0,0,0,1)");

    Matrix34 flip_x = {{1, 0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0}};   // trace -1, i = 0
    matToQuaternion__FP4QuatP8Matrix34(&q, &flip_x);
    expect(q.x == 1 && q.y == 0 && q.z == 0 && q.w == 0, "half turn about x");
    Matrix34 flip_y = {{-1, 0, 0, 0, 1, 0, 0, 0, -1, 0, 0, 0}};   // i = 1
    matToQuaternion__FP4QuatP8Matrix34(&q, &flip_y);
    expect(q.x == 0 && q.y == 1 && q.z == 0 && q.w == 0, "half turn about y");
    Matrix34 flip_z = {{-1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 0, 0}};   // i = 2
    matToQuaternion__FP4QuatP8Matrix34(&q, &flip_z);
    expect(q.x == 0 && q.y == 0 && q.z == 1 && q.w == 0, "half turn about z");

    // Element placement: a quarter turn about z in this file's layout (m1 = +1, m3 = -1).
    Matrix34 quarter_z = {{0, 1, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0}};
    matToQuaternion__FP4QuatP8Matrix34(&q, &quarter_z);
    const float h = std::sqrt(0.5f);
    expect(std::fabs(q.x) < 1e-6f && std::fabs(q.y) < 1e-6f && std::fabs(q.z - h) < 1e-6f && std::fabs(q.w - h) < 1e-6f, "quarter turn about z");

    // Round trip through quatToMat for random rotations, all four branches.
    std::srand(11);
    int branch_hits[4] = {0, 0, 0, 0};
    double worst = 0;
    for (int n = 0; n < 20000; ++n) {
        double u[4], norm = 0;
        for (double& v : u) { v = std::rand() / double(RAND_MAX) - 0.5; norm += v * v; }
        norm = std::sqrt(norm);
        const Quat in = {float(u[0] / norm), float(u[1] / norm), float(u[2] / norm), float(u[3] / norm)};
        Matrix33 m3;
        quatToMat__FP8Matrix33P4Quat(&m3, &in);
        const float trace = m3.m[0] + m3.m[4] + m3.m[8];
        int branch = 0;
        if (!(0.0f < trace)) branch = (m3.m[0] < m3.m[4]) ? ((m3.m[4] < m3.m[8]) ? 3 : 2) : ((m3.m[0] < m3.m[8]) ? 3 : 1);
        ++branch_hits[branch];
        const Matrix34 m4 = widen(m3);
        matToQuaternion__FP4QuatP8Matrix34(&q, &m4);
        const double dot = in.x * q.x + in.y * q.y + in.z * q.z + in.w * q.w;
        const double sign = dot < 0 ? -1.0 : 1.0;
        worst = std::fmax(worst, std::fabs(q.x * sign - in.x));
        worst = std::fmax(worst, std::fabs(q.y * sign - in.y));
        worst = std::fmax(worst, std::fabs(q.z * sign - in.z));
        worst = std::fmax(worst, std::fabs(q.w * sign - in.w));
    }
    std::printf("branches hit %d %d %d %d, worst component error %.3g\n", branch_hits[0], branch_hits[1], branch_hits[2], branch_hits[3], worst);
    expect(branch_hits[0] > 0 && branch_hits[1] > 0 && branch_hits[2] > 0 && branch_hits[3] > 0, "every branch is exercised");
    expect(worst < 2e-3, "round trip recovers the quaternion up to sign");
    if (failures) return 1;
    std::printf("matToQuaternion contract passed\n");
    return 0;
}
