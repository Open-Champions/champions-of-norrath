// Host check only; not a build input. Mock callees transform vectors by XOR so every lane is traceable.
#include "init.cpp"
#include <math.h>
#include <stdio.h>
#include <string.h>

static unsigned log_kind[8];
static int nlog;
static unsigned char seen_matrix[4];
static float precalc_seen[32];
static unsigned xorbits(float f, unsigned k) { unsigned u; memcpy(&u, &f, 4); u ^= k; return u; }
extern "C" float* __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* v, const unsigned char* m) {
    log_kind[nlog++] = 3;
    seen_matrix[0] = m[0];
    for (int i = 0; i < 3; ++i) { unsigned u = xorbits(v[i], 0x01010101); memcpy(result + i, &u, 4); }
    result[3] = 7.0f;
    return result;
}
extern "C" float* __ml__FRC7VIVect4RC10VIMatrix44(float* result, const float* v, const unsigned char*) {
    log_kind[nlog++] = 4;
    if (v[3] != 0.0f) log_kind[nlog++] = 99;
    for (int i = 0; i < 4; ++i) { unsigned u = xorbits(v[i], 0x02020202); memcpy(result + i, &u, 4); }
    return result;
}
extern "C" void Normalize__7VIVect4(float* v) {
    log_kind[nlog++] = 5;
    for (int i = 0; i < 4; ++i) { unsigned u = xorbits(v[i], 0x04040404); memcpy(v + i, &u, 4); }
}
extern "C" int Precalc__9VICollRay(unsigned char* self) {
    log_kind[nlog++] = 6;
    memcpy(precalc_seen, self, sizeof precalc_seen);
    return -9;
}

int main() {
    alignas(16) unsigned char src[128], dst[128], mat[64];
    for (int i = 0; i < 128; ++i) { src[i] = (unsigned char)(i * 7 + 3); dst[i] = 0xcc; }
    for (int i = 0; i < 64; ++i) mat[i] = (unsigned char)(i + 1);
    const float* s = reinterpret_cast<const float*>(src);
    unsigned want[32];
    for (int i = 0; i < 32; ++i) memcpy(want + i, dst + 4 * i, 4);
    for (int i = 0; i < 3; ++i) {
        want[i] = xorbits(s[i], 0x01010101);
        want[3 + i] = xorbits(s[3 + i], 0x01010101);
        want[14 + i] = xorbits(s[14 + i], 0x06060606);
    }
    memcpy(want + 17, src + 68, 4);
    memcpy(want + 18, src + 72, 4);
    for (int i = 19; i < 22; ++i) memcpy(want + i, src + 4 * i, 4);
    nlog = 0;
    if (Init__9VICollRayRC9VICollRayRC10VIMatrix44(dst, src, mat) != 0) return 1;
    static const unsigned order[] = {3, 3, 4, 5, 6};
    if (nlog != 5) return 2;
    for (int i = 0; i < 5; ++i) if (log_kind[i] != order[i]) return 3;
    if (seen_matrix[0] != 1) return 4;
    for (int i = 0; i < 22; ++i) if (memcmp(dst + 4 * i, want + i, 4) != 0) { { unsigned g; memcpy(&g, dst + 4 * i, 4); printf("lane %d got %08x want %08x src %08x\n", i, g, want[i], *(unsigned*)(src + 4 * i)); } return 5; }
    if (memcmp(dst + 88, "\xcc\xcc\xcc\xcc", 4) != 0 || dst[100] != 0xcc) return 6;
    // Precalc sees the transformed fields, but not the trailing copies.
    if (memcmp(precalc_seen, want, 4 * 17) != 0) return 7;
    return 0;
}
