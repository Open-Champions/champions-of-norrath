// Host contract for quatToMat__FP8Matrix33P4Quat. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off quat_to_mat33.cpp host_check.cpp
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "../w8-object-list-math-common/matrix_types.hpp"
extern "C" void quatToMat__FP8Matrix33P4Quat(Matrix33*, const Quat*);

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix33 m;
    const Quat identity = {0, 0, 0, 1};
    quatToMat__FP8Matrix33P4Quat(&m, &identity);
    const float eye[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    bool same = true;
    for (int i = 0; i < 9; ++i) same &= (m.m[i] == eye[i]);
    expect(same, "identity quaternion gives the identity matrix");

    const float h = std::sqrt(0.5f);
    const Quat quarter_z = {0, 0, h, h};
    quatToMat__FP8Matrix33P4Quat(&m, &quarter_z);
    const float rz[9] = {0, 1, 0, -1, 0, 0, 0, 0, 1};
    bool close = true;
    for (int i = 0; i < 9; ++i) close &= std::fabs(m.m[i] - rz[i]) < 1e-6f;
    expect(close, "90 degrees about z puts +1 at index 1 and -1 at index 3");

    const Quat half_x = {1, 0, 0, 0};
    quatToMat__FP8Matrix33P4Quat(&m, &half_x);
    const float rx[9] = {1, 0, 0, 0, -1, 0, 0, 0, -1};
    same = true;
    for (int i = 0; i < 9; ++i) same &= (m.m[i] == rx[i]);
    expect(same, "180 degrees about x flips y and z");

    // Random quaternions against the textbook double-precision expansion; the
    // sign of a quaternion and its scale do not change the matrix.
    std::srand(7);
    double worst = 0;
    for (int n = 0; n < 5000; ++n) {
        const double k = 0.1 + 3.0 * (std::rand() / double(RAND_MAX));
        const double ux = std::rand() / double(RAND_MAX) - 0.5, uy = std::rand() / double(RAND_MAX) - 0.5;
        const double uz = std::rand() / double(RAND_MAX) - 0.5, uw = std::rand() / double(RAND_MAX) - 0.5;
        const double norm = std::sqrt(ux * ux + uy * uy + uz * uz + uw * uw);
        const double x = ux / norm, y = uy / norm, z = uz / norm, w = uw / norm;
        const double ref[9] = {1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y),
                               2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x),
                               2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y)};
        const Quat q = {float(x * k), float(y * k), float(z * k), float(w * k)};
        quatToMat__FP8Matrix33P4Quat(&m, &q);
        for (int i = 0; i < 9; ++i) worst = std::fmax(worst, std::fabs(m.m[i] - ref[i]));
    }
    std::printf("worst deviation from the reference %.3g\n", worst);
    expect(worst < 3e-6, "matches the reference expansion for scaled quaternions");
    if (failures) return 1;
    std::printf("quatToMat33 contract passed\n");
    return 0;
}
