// Host check only; not a build input.
#include "submatrixmul.cpp"
#include <cstdio>
int main() {
    float m[16];
    for (int i = 0; i < 16; ++i) m[i] = 0.25f * (i - 5);
    float in[3] = {2.0f, -1.5f, 4.0f};
    float out[4] = {9, 9, 9, 9};
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(m, in, out);
    bool ok = out[3] == 9.0f;
    for (int i = 0; i < 3; ++i) {
        float want = ((m[i] * 2.0f + m[4 + i] * -1.5f) + m[8 + i] * 4.0f) + m[12 + i] * 0.0f;
        ok = ok && out[i] == want;
    }
    float a[3] = {1, 2, 3}, expect[3];
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(m, a, expect);
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(m, a, a);
    ok = ok && a[0] == expect[0] && a[1] == expect[1] && a[2] == expect[2];
    float shifted[16];
    for (int i = 0; i < 16; ++i) shifted[i] = m[i];
    shifted[12] += 100.0f; shifted[13] -= 50.0f; shifted[14] += 7.0f;  // translation row must not matter
    float other[3];
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(shifted, a, other);
    SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(m, a, expect);
    ok = ok && other[0] == expect[0] && other[1] == expect[1] && other[2] == expect[2];
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
