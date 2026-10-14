// Host contract for matMakeTrans__FP8Matrix34G6Point3. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matMakeTrans__FP8Matrix34G6Point3(Matrix34*, Point3);
int main() {
    int failures = 0;
    struct { float before[2]; Matrix34 matrix; float after[2]; } box;
    for (float& f : box.matrix.m) f = 7.5f;
    box.before[0] = box.before[1] = box.after[0] = box.after[1] = -3.0f;
    matMakeTrans__FP8Matrix34G6Point3(&box.matrix, Point3{1.5f, -2.25f, 1e10f});
    const float want[12] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1.5f, -2.25f, 1e10f};
    for (int i = 0; i < 12; ++i)
        if (std::memcmp(&box.matrix.m[i], &want[i], 4) != 0) { std::fprintf(stderr, "fail: element %d\n", i); ++failures; }
    if (box.before[0] != -3.0f || box.before[1] != -3.0f || box.after[0] != -3.0f || box.after[1] != -3.0f) {
        std::fprintf(stderr, "fail: wrote outside twelve floats\n"); ++failures;
    }
    if (failures) return 1;
    std::printf("matMakeTrans contract passed\n");
    return 0;
}
