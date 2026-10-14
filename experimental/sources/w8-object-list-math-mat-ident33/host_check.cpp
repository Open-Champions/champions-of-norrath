// Host contract for matIdent__FP8Matrix33. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Matrix33 { float m[9]; };
extern "C" void matIdent__FP8Matrix33(Matrix33*);
int main() {
    struct { float guard_before[2]; Matrix33 matrix; float guard_after[2]; } box;
    for (float& f : box.matrix.m) f = 7.5f;
    box.guard_before[0] = box.guard_before[1] = box.guard_after[0] = box.guard_after[1] = -3.0f;
    matIdent__FP8Matrix33(&box.matrix);
    int failures = 0;
    for (int i = 0; i < 9; ++i) {
        const float want = (i % 4 == 0) ? 1.0f : 0.0f;
        unsigned a, b; std::memcpy(&a, &box.matrix.m[i], 4); std::memcpy(&b, &want, 4);
        if (a != b) { std::fprintf(stderr, "fail: element %d\n", i); ++failures; }
    }
    if (box.guard_before[0] != -3.0f || box.guard_before[1] != -3.0f || box.guard_after[0] != -3.0f || box.guard_after[1] != -3.0f) {
        std::fprintf(stderr, "fail: wrote outside nine floats\n"); ++failures;
    }
    if (failures) return 1;
    std::printf("matIdent33 contract passed\n");
    return 0;
}
