#include "invert.cpp"
#include <math.h>
#include <string.h>

static int swaps, destroyed, last_flags;
static void* destroyed_arg;
static int swap_log[16][3];
extern "C" int SwapRows__10VIMatrix44ii(float* m, int a, int b) {
    swap_log[swaps][0] = m == nullptr; swap_log[swaps][1] = a; swap_log[swaps][2] = b;
    ++swaps;
    for (int i = 0; i < 4; ++i) { float t = m[4 * a + i]; m[4 * a + i] = m[4 * b + i]; m[4 * b + i] = t; }
    return 0;
}
extern "C" void _$_10VIMatrix44(void* m, int flags) { ++destroyed; destroyed_arg = m; last_flags = flags; }

static void reset() { swaps = 0; destroyed = 0; }

int main() {
    alignas(16) float a[16] = {2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4, 0, 1, 2, 3, 1};
    alignas(16) float inv[16];
    for (int i = 0; i < 16; ++i) inv[i] = 99.0f;
    reset();
    if (Invert__10VIMatrix44RC10VIMatrix44(inv, a) != 0) return 1;
    if (swaps != 8 || destroyed != 1 || last_flags != 2) return 2;
    // swaps come in pairs for each column, local first then destination, with the pivot row
    for (int k = 0; k < 4; ++k) if (swap_log[2 * k][1] != k || swap_log[2 * k + 1][1] != k) return 3;
    // a * inv is the identity (exact for this matrix)
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float s = 0;
            for (int k = 0; k < 4; ++k) s += a[4 * r + k] * inv[4 * k + c];
            if (fabsf(s - (r == c ? 1.0f : 0.0f)) > 1e-5f) return 10 + 4 * r + c;
        }
    // a matrix needing pivoting
    alignas(16) float p[16] = {0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 5, 0, 0, 3, 0};
    alignas(16) float pinv[16];
    reset();
    if (Invert__10VIMatrix44RC10VIMatrix44(pinv, p) != 0) return 30;
    if (swap_log[0][2] != 1) return 31;   // column 0 pivots to row 1
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float s = 0;
            for (int k = 0; k < 4; ++k) s += p[4 * r + k] * pinv[4 * k + c];
            if (fabsf(s - (r == c ? 1.0f : 0.0f)) > 1e-5f) return 40 + 4 * r + c;
        }
    // ties keep the earlier row
    alignas(16) float t[16] = {1, 0, 0, 0, -1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    alignas(16) float tinv[16];
    reset();
    if (Invert__10VIMatrix44RC10VIMatrix44(tinv, t) != 0 || swap_log[0][2] != 0) return 60;
    // singular
    alignas(16) float s[16] = {1, 2, 3, 4, 2, 4, 6, 8, 0, 0, 1, 0, 0, 0, 0, 1};
    alignas(16) float sinv[16];
    reset();
    if (Invert__10VIMatrix44RC10VIMatrix44(sinv, s) != -1) return 61;
    if (destroyed != 1 || last_flags != 2) return 62;
    // source equal to destination
    alignas(16) float same[16]; memcpy(same, a, sizeof a);
    reset();
    if (Invert__10VIMatrix44RC10VIMatrix44(same, same) != 0) return 63;
    if (memcmp(same, inv, sizeof inv) != 0) return 64;
    return 0;
}
