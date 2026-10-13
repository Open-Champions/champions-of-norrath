#include "mul_vect3.cpp"
#include <string.h>

int main() {
    float m[16];
    for (int i = 0; i < 16; ++i) m[i] = 0.25f * (i - 5);
    float in[3] = {2.0f, -1.5f, 4.0f};
    float out[4] = {9, 9, 9, 9};
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m, in, out);
    for (int i = 0; i < 3; ++i) {
        float want = ((m[i] * 2.0f + m[4 + i] * -1.5f) + m[8 + i] * 4.0f) + m[12 + i] * 1.0f;
        if (out[i] != want) return 1 + i;
    }
    if (out[3] != 9.0f) return 5;  // fourth float is not written
    float a[3] = {1, 2, 3}, expect[3];
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m, a, expect);
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(m, a, a);
    if (memcmp(a, expect, sizeof a) != 0) return 6;
    return 0;
}
