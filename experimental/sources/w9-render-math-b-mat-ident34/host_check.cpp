// Host contract for matIdent__FP8Matrix34. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Matrix34 { float m[12]; };
extern "C" void matIdent__FP8Matrix34(Matrix34*);
int main() {
    struct { float before[2]; Matrix34 matrix; float after[2]; } box;
    for (float& f : box.matrix.m) f = 7.5f;
    box.before[0] = box.before[1] = box.after[0] = box.after[1] = -3.0f;
    matIdent__FP8Matrix34(&box.matrix);
    int failures = 0;
    for (int i = 0; i < 12; ++i) {
        const float want = (i == 0 || i == 4 || i == 8) ? 1.0f : 0.0f;
        unsigned a, b; std::memcpy(&a, &box.matrix.m[i], 4); std::memcpy(&b, &want, 4);
        if (a != b) { std::fprintf(stderr, "fail: element %d\n", i); ++failures; }
    }
    if (box.before[0] != -3.0f || box.before[1] != -3.0f || box.after[0] != -3.0f || box.after[1] != -3.0f) {
        std::fprintf(stderr, "fail: wrote outside twelve floats\n"); ++failures;
    }
    if (failures) return 1;
    std::printf("matIdent34 contract passed\n");
    return 0;
}
