// Host contract for matRot__FP8Matrix33is. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off mat_rot33.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include "../w8-object-list-math-common/matrix_types.hpp"

static float cosine_value = 0.8f, sine_value = 0.6f;
static int calls[4], call_count;       // 1 = icos, 2 = isin, in order
static int last_cos_angle, last_sin_angle;
extern "C" float icos__Fi(int angle) { last_cos_angle = angle; calls[call_count++] = 1; return cosine_value; }
extern "C" float isin__Fi(int angle) { last_sin_angle = angle; calls[call_count++] = 2; return sine_value; }
extern "C" void matRot__FP8Matrix33is(Matrix33*, int, short);

static bool same_bits(float a, float b) { return std::memcmp(&a, &b, 4) == 0; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const float start[9] = {1.25f, -2.5f, 3.75f, 0.5f, 7.0f, -1.5f, 2.25f, 9.0f, -4.0f};
    const float c = cosine_value, s = sine_value;

    Matrix33 m;
    std::memcpy(m.m, start, sizeof start);
    matRot__FP8Matrix33is(&m, 0, 1234);
    expect(call_count == 2 && calls[0] == 1 && calls[1] == 2, "cosine is computed before sine");
    expect(last_cos_angle == 1234 && last_sin_angle == 1234, "both receive the angle");
    float want[9];
    std::memcpy(want, start, sizeof start);
    for (int i = 0; i < 3; ++i) {  // axis 0: rows 1 and 2
        want[3 + i] = c * start[3 + i] + s * start[6 + i];
        want[6 + i] = -s * start[3 + i] + c * start[6 + i];
    }
    expect(std::memcmp(m.m, want, sizeof want) == 0, "axis 0 rotates rows 1 and 2, row 0 untouched");

    std::memcpy(m.m, start, sizeof start);
    matRot__FP8Matrix33is(&m, 1, 1234);
    std::memcpy(want, start, sizeof start);
    for (int i = 0; i < 3; ++i) {  // axis 1: rows 0 and 2
        want[i] = c * start[i] - s * start[6 + i];
        want[6 + i] = s * start[i] + c * start[6 + i];
    }
    expect(std::memcmp(m.m, want, sizeof want) == 0, "axis 1 rotates rows 0 and 2 with the sine sign swapped");

    std::memcpy(m.m, start, sizeof start);
    matRot__FP8Matrix33is(&m, 2, 1234);
    std::memcpy(want, start, sizeof start);
    for (int i = 0; i < 3; ++i) {  // axis 2: rows 0 and 1
        want[i] = c * start[i] + s * start[3 + i];
        want[3 + i] = -s * start[i] + c * start[3 + i];
    }
    expect(std::memcmp(m.m, want, sizeof want) == 0, "axis 2 rotates rows 0 and 1");

    for (int axis : {3, -1, 7}) {
        std::memcpy(m.m, start, sizeof start);
        call_count = 0;
        matRot__FP8Matrix33is(&m, axis, 5);
        expect(std::memcmp(m.m, start, sizeof start) == 0, "other axes leave the matrix alone");
        expect(call_count == 2, "trig is still evaluated");
    }

    // The angle is a short: a wider argument is truncated and sign-extended by the callee ABI.
    call_count = 0;
    matRot__FP8Matrix33is(&m, 3, static_cast<short>(40000));
    expect(last_cos_angle == -25536, "angle is passed sign-extended from 16 bits");

    // Rotation by a zero angle (c = 1, s = 0) keeps every element's value.
    cosine_value = 1.0f; sine_value = 0.0f;
    for (int axis = 0; axis < 3; ++axis) {
        std::memcpy(m.m, start, sizeof start);
        matRot__FP8Matrix33is(&m, axis, 0);
        bool equal = true;
        for (int i = 0; i < 9; ++i) equal &= (m.m[i] == start[i]);
        expect(equal, "identity rotation preserves values");
    }
    (void)same_bits;
    if (failures) return 1;
    std::printf("matRot33 contract passed\n");
    return 0;
}
