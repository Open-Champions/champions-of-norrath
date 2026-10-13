// Host contract for matInvert__FP8Matrix34T0 with a recording stand-in for the
// 4x4 inverse. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" void matInvert__FP8Matrix34T0(Matrix34*, const Matrix34*);
static Matrix44 seen;
static int calls;
static const void* seenOut;
static const void* seenIn;
// Stand-in: records its input and writes the negation of every float, in place.
extern "C" void matInvert__FP8Matrix44T0(Matrix44* out, const Matrix44* in) {
    ++calls; seenOut = out; seenIn = in; seen = *in;
    for (int i = 0; i < 16; ++i) out->m[i] = -in->m[i];
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 in, out;
    for (int i = 0; i < 12; ++i) { in.m[i] = static_cast<float>(i + 1); out.m[i] = 99.0f; }
    matInvert__FP8Matrix34T0(&out, &in);
    expect(calls == 1 && seenOut == seenIn, "one in-place 4x4 call");
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 3; ++c) expect(seen.m[4 * r + c] == in.m[3 * r + c], "rows copied with stride 4");
        expect(seen.m[4 * r + 3] == (r == 3 ? 1.0f : 0.0f), "fourth column 0,0,0,1");
    }
    for (int i = 0; i < 12; ++i) expect(out.m[i] == -in.m[i], "rows copied back with stride 3");
    Matrix34 alias = in;
    matInvert__FP8Matrix34T0(&alias, &alias);
    for (int i = 0; i < 12; ++i) expect(alias.m[i] == -in.m[i], "aliased output");
    if (failures) return 1;
    std::printf("matInvert34 contract passed\n");
    return 0;
}
