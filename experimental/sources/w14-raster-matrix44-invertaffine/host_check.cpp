// Host check only; not a build input.
#include "invert_affine.cpp"
#include <cmath>
#include <cstdio>
static bool near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }
// row-vector product: out = a * b with a, b 4x4 row-major
static void mul(const float* a, const float* b, float* out) {
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float s = 0.0f;
            for (int k = 0; k < 4; ++k) s += a[4 * r + k] * b[4 * k + c];
            out[4 * r + c] = s;
        }
}
int main() {
    float scale[16] = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, -1, 2, -3, 1};
    float inv[16];
    for (int i = 0; i < 16; ++i) inv[i] = 99.0f;
    InvertAffine__10VIMatrix44RC10VIMatrix44(inv, scale);
    float want[16] = {0.5f, 0, 0, 0, 0, 1.0f / 3.0f, 0, 0, 0, 0, 0.25f, 0, 0.5f, -2.0f / 3.0f, 0.75f, 1};
    bool ok = true;
    for (int i = 0; i < 16; ++i) ok = ok && near(inv[i], want[i]);
    // a rotation-like matrix with a translation: m * inverse must be the identity
    float m[16] = {0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 4, 5, 6, 1}, mi[16], prod[16];
    InvertAffine__10VIMatrix44RC10VIMatrix44(mi, m);
    mul(m, mi, prod);
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) ok = ok && near(prod[4 * r + c], r == c ? 1.0f : 0.0f);
    float g[16] = {1, 2, 3, 0, 0, 1, 4, 0, 5, 6, 0, 0, -2, 7, 1, 1}, gi[16];
    InvertAffine__10VIMatrix44RC10VIMatrix44(gi, g);
    mul(g, gi, prod);
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) ok = ok && near(prod[4 * r + c], r == c ? 1.0f : 0.0f);
    ok = ok && gi[3] == 0.0f && gi[7] == 0.0f && gi[11] == 0.0f && gi[15] == 1.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
