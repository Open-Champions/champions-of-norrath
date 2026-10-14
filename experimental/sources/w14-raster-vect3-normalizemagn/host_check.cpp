// Host check only; not a build input.
#include "normalize_magn.cpp"
#include <cstdio>
int main() {
    float v[4] = {3.0f, 4.0f, 0.0f, 9.0f};
    float m = NormalizeMagn__7VIVect3(v);
    bool ok = m == 5.0f && v[0] == 3.0f * (1.0f / 5.0f) && v[1] == 4.0f * (1.0f / 5.0f) && v[2] == 0.0f && v[3] == 9.0f;
    float z[3] = {0.0f, 0.0f, 0.0f};
    ok = ok && NormalizeMagn__7VIVect3(z) == 0.0f && z[0] == 0.0f && z[1] == 0.0f && z[2] == 0.0f;
    float w[3] = {0.0f, -2.0f, 0.0f};
    ok = ok && NormalizeMagn__7VIVect3(w) == 2.0f && w[1] == -1.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
