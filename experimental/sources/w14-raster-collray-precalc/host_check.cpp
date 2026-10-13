// Host check only; not a build input. Magnitude is stubbed with the Euclidean length of the vector it is given.
#include "precalc.cpp"
#include <cmath>
#include <cstdio>
static const float* seen;
extern "C" float Magnitude__C7VIVect3(const float* v) { seen = v; return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
int main() {
    float r[16] = {1, 1, 1, 4, 5, 1, -1, -1, -1, -1, -1, -1, -1, -1, 99, 99};
    int res = Precalc__9VICollRay(r);
    bool ok = res == 0 && seen == r + 9 && r[6] == 3 && r[7] == 4 && r[8] == 0 && r[12] == 5.0f && r[13] == 1.0f / 5.0f &&
              r[9] == 3.0f * (1.0f / 5.0f) && r[10] == 4.0f * (1.0f / 5.0f) && r[11] == 0.0f && r[14] == 99.0f;
    float z[16] = {2, 2, 2, 2, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1};
    ok = ok && Precalc__9VICollRay(z) == 0 && z[12] == 0.0f && z[13] == 0.0f && z[9] == 0.0f && z[10] == 0.0f && z[11] == 0.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
