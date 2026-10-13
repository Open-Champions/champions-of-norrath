#include "mul_vect4.cpp"
#include <string.h>

int main() {
    float m[16];
    for (int i = 0; i < 16; ++i) m[i] = 0.5f * (i + 1);
    float in[4] = {1.0f, -2.0f, 3.0f, 0.25f};
    float out[4] = {9, 9, 9, 9};
    Mul__C10VIMatrix44RC7VIVect4R7VIVect4(m, in, out);
    for (int i = 0; i < 4; ++i) {
        float want = ((m[i] * 1.0f + m[4 + i] * -2.0f) + m[8 + i] * 3.0f) + m[12 + i] * 0.25f;
        if (out[i] != want) return 1 + i;
    }
    // each input lane selects its own row: unit vectors return the rows
    for (int k = 0; k < 4; ++k) {
        float e[4] = {0, 0, 0, 0}; e[k] = 1.0f;
        float r[4];
        Mul__C10VIMatrix44RC7VIVect4R7VIVect4(m, e, r);
        for (int i = 0; i < 4; ++i) if (r[i] != m[4 * k + i]) return 10 + k;
    }
    // output aliasing the input
    float a[4] = {2, 3, 5, 7};
    float expect[4];
    Mul__C10VIMatrix44RC7VIVect4R7VIVect4(m, a, expect);
    Mul__C10VIMatrix44RC7VIVect4R7VIVect4(m, a, a);
    if (memcmp(a, expect, sizeof a) != 0) return 20;
    return 0;
}
