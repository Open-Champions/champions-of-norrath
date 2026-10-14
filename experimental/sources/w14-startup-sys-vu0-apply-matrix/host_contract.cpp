// Host contract for sceVu0ApplyMatrix (host fallback path). Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// DIntr and EIntr are scripted: the interrupt calls bracket the transform, EIntr runs only when DIntr returned
// nonzero, the result equals ((r0*x + r1*y) + r2*z) + r3*w, the matrix is unchanged, and the destination may
// alias the vector. The VU0 instruction sequence itself is covered by the private paired run in the notes.
#include <cstdio>
#include <cstring>
static int dintr_ret, order[8], n;
extern "C" int DIntr() { order[n++] = 1; return dintr_ret; }
extern "C" int EIntr() { order[n++] = 2; return 0; }
#include "vu0_apply_matrix.cpp"
int main() {
    alignas(16) float m[16], v[4], d[4], copy[16];
    for (int i = 0; i < 16; ++i) m[i] = 0.5f * (i + 1) - 3.0f;
    v[0] = 1.5f; v[1] = -2.0f; v[2] = 0.25f; v[3] = 1.0f;
    std::memcpy(copy, m, sizeof m);
    dintr_ret = 1; n = 0;
    sceVu0ApplyMatrix(d, m, v);
    if (n != 2 || order[0] != 1 || order[1] != 2) return 1;
    for (int c = 0; c < 4; ++c) {
        float want = ((m[c] * v[0] + m[4 + c] * v[1]) + m[8 + c] * v[2]) + m[12 + c] * v[3];
        if (d[c] != want) return 2 + c;
    }
    if (std::memcmp(m, copy, sizeof m)) return 6;
    dintr_ret = 0; n = 0;
    float e[4];
    sceVu0ApplyMatrix(e, m, v);
    if (n != 1 || order[0] != 1 || std::memcmp(e, d, sizeof e)) return 7;
    float alias[4] = {v[0], v[1], v[2], v[3]};
    sceVu0ApplyMatrix(alias, m, alias);
    if (std::memcmp(alias, d, sizeof d)) return 8;
    float ident[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    sceVu0ApplyMatrix(e, ident, v);
    if (std::memcmp(e, v, sizeof e)) return 9;
    std::puts("sceVu0ApplyMatrix host contract passed");
    return 0;
}
