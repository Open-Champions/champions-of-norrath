// Host contract for P_SetParticleSystemMatrix__FiP8Matrix34. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" {
unsigned char PS[312 * 256];
void P_SetParticleSystemMatrix__FiP8Matrix34(int, const Matrix34*);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Matrix34 m;
    for (int i = 0; i < 12; ++i) m.m[i] = float(i + 1);
    *reinterpret_cast<int*>(PS + 4 * 312 + 304) = 4;
    P_SetParticleSystemMatrix__FiP8Matrix34(4, &m);
    expect(std::memcmp(PS + 4 * 312 + 256, &m, 48) == 0, "matrix stored at +256");
    expect(PS[4 * 312 + 255] == 0 && PS[4 * 312 + 304] == 4, "neighbouring bytes untouched");
    unsigned char before[sizeof PS];
    std::memcpy(before, PS, sizeof PS);
    Matrix34 n; std::memset(&n, 0x7f, sizeof n);
    P_SetParticleSystemMatrix__FiP8Matrix34(0, &n);
    P_SetParticleSystemMatrix__FiP8Matrix34(4 + 256, &n);
    expect(std::memcmp(before, PS, sizeof PS) == 0, "zero or mismatched id changes nothing");
    if (failures) return 1;
    std::printf("P_SetParticleSystemMatrix contract passed\n");
    return 0;
}
