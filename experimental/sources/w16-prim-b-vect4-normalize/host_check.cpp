#include "normalize.cpp"
#include <math.h>

int main() {
    alignas(16) float zero[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    if (Normalize__7VIVect4(zero) != 0) return 1;
    if (zero[0] != 0.0f || zero[1] != 0.0f || zero[2] != 0.0f || zero[3] != 0.0f) return 2;
    alignas(16) float a[4] = {3.0f, 0.0f, 4.0f, 0.0f};
    if (Normalize__7VIVect4(a) != 1) return 3;
    float s = 1.0f / sqrtf(25.0f);
    if (a[0] != 3.0f * s || a[1] != 0.0f || a[2] != 4.0f * s || a[3] != 0.0f) return 4;
    alignas(16) float b[4] = {-1.0f, 2.0f, -2.0f, 4.0f};
    if (Normalize__7VIVect4(b) != 1) return 5;
    float t = 1.0f / sqrtf(25.0f);
    if (b[0] != -1.0f * t || b[1] != 2.0f * t || b[2] != -2.0f * t || b[3] != 4.0f * t) return 6;
    alignas(16) float c[4] = {0.0f, 0.0f, 0.0f, 2.0f};   // w alone counts toward the length
    if (Normalize__7VIVect4(c) != 1 || c[3] != 1.0f) return 7;
    alignas(16) float e[4] = {1e-30f, 0.0f, 0.0f, 0.0f};  // squared length underflows to zero
    if (Normalize__7VIVect4(e) != 0 || e[0] != 1e-30f) return 8;
    return 0;
}
