// Host check only; not a build input. Mock callees transform vectors by XOR so every lane is traceable.
#include "init.cpp"
#include <initializer_list>
#include <string.h>

static unsigned log_kind[8];
static int nlog, precalc_result;
static unsigned char seen_matrix[8];
static unsigned char precalc_seen[128];
static unsigned xorbits(float f, unsigned k) { unsigned u; memcpy(&u, &f, 4); u ^= k; return u; }
extern "C" float* __ml__FRC7VIVect3RC10VIMatrix44(float* result, const float* v, const unsigned char* m) {
    log_kind[nlog++] = 3;
    memcpy(seen_matrix, m, 4);
    for (int i = 0; i < 3; ++i) { unsigned u = xorbits(v[i], 0x01010101); memcpy(result + i, &u, 4); }
    result[3] = 7.0f;
    return result;
}
extern "C" void SubMatrixMul__C10VIMatrix44RC7VIVect3R7VIVect3(const unsigned char* m, const float* in, float* out) {
    log_kind[nlog++] = 7;
    if (memcmp(seen_matrix, m, 4) != 0) log_kind[nlog++] = 99;
    for (int i = 0; i < 3; ++i) { unsigned u = xorbits(in[i], 0x10101010); memcpy(out + i, &u, 4); }
}
extern "C" void Normalize__7VIVect3(float* v) {
    log_kind[nlog++] = 8;
    for (int i = 0; i < 3; ++i) { unsigned u = xorbits(v[i], 0x08080808); memcpy(v + i, &u, 4); }
}
extern "C" int Precalc__13VIPickSegment(unsigned char* self) {
    log_kind[nlog++] = 6;
    memcpy(precalc_seen, self, 128);
    return precalc_result;
}

int main() {
    for (int result : {-4, -1, 0, 3}) {
        alignas(16) unsigned char src[128], dst[128], mat[64];
        for (int i = 0; i < 128; ++i) { src[i] = (unsigned char)(i * 7 + 3); dst[i] = 0xcc; }
        for (int i = 0; i < 64; ++i) mat[i] = (unsigned char)(i + 1);
        const float* s = reinterpret_cast<const float*>(src);
        unsigned want[32];
        for (int i = 0; i < 32; ++i) memcpy(want + i, dst + 4 * i, 4);
        for (int i = 0; i < 3; ++i) {
            want[i] = xorbits(s[i], 0x01010101);
            want[3 + i] = xorbits(s[3 + i], 0x01010101);
            want[16 + i] = xorbits(s[16 + i], 0x18181818);
        }
        memcpy(reinterpret_cast<unsigned char*>(want) + 76, src + 76, 16);
        unsigned char before[128];
        memcpy(before, want, 128);
        if (result >= 0) {
            memcpy(want + 15, src + 60, 4);
            memcpy(want + 14, src + 56, 4);
        }
        precalc_result = result;
        nlog = 0;
        if (Init__13VIPickSegmentRC13VIPickSegmentRC10VIMatrix44(dst, src, mat) != (result < 0 ? -1 : 0)) return 1;
        static const unsigned order[] = {3, 3, 7, 8, 6};
        if (nlog != 5) return 2;
        for (int i = 0; i < 5; ++i) if (log_kind[i] != order[i]) return 3;
        if (memcmp(dst, want, 128) != 0) return 4;
        if (memcmp(precalc_seen, before, 128) != 0) return 5;
    }
    return 0;
}
