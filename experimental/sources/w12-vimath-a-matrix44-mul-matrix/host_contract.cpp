#include "mul_matrix.cpp"
#include <string.h>

int main() {
    float a[16], b[16], d[16];
    for (int i = 0; i < 16; ++i) { a[i] = 0.5f * (i + 1); b[i] = 0.25f * (16 - i); }
    Mul__10VIMatrix44RC10VIMatrix44T1(d, a, b);
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float want = ((b[c] * a[4 * r] + b[4 + c] * a[4 * r + 1]) + b[8 + c] * a[4 * r + 2]) + b[12 + c] * a[4 * r + 3];
            if (d[4 * r + c] != want) return 1 + 4 * r + c;
        }
    float id[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    float e[16];
    Mul__10VIMatrix44RC10VIMatrix44T1(e, a, id);
    if (memcmp(e, a, sizeof a) != 0) return 20;
    Mul__10VIMatrix44RC10VIMatrix44T1(e, id, b);
    if (memcmp(e, b, sizeof b) != 0) return 21;
    float x[16], y[16];
    memcpy(x, a, sizeof a); memcpy(y, b, sizeof b);
    Mul__10VIMatrix44RC10VIMatrix44T1(x, x, b);   // dest aliases left
    Mul__10VIMatrix44RC10VIMatrix44T1(d, a, b);
    if (memcmp(x, d, sizeof d) != 0) return 22;
    Mul__10VIMatrix44RC10VIMatrix44T1(y, a, y);   // dest aliases right
    if (memcmp(y, d, sizeof d) != 0) return 23;
    return 0;
}
