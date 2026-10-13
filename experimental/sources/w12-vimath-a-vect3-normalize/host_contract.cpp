#include "normalize.cpp"

int main() {
    float zero[3] = {0.0f, 0.0f, 0.0f};
    if (Normalize__7VIVect3(zero) != 0) return 1;
    if (zero[0] != 0.0f || zero[1] != 0.0f || zero[2] != 0.0f) return 2;
    float a[3] = {3.0f, 0.0f, 4.0f};
    if (Normalize__7VIVect3(a) != 1) return 3;
    float s = 1.0f / sqrtf(25.0f);
    if (a[0] != 3.0f * s || a[1] != 0.0f || a[2] != 4.0f * s) return 4;
    float b[3] = {-1.0f, 2.0f, -2.0f};
    if (Normalize__7VIVect3(b) != 1) return 5;
    float t = 1.0f / sqrtf(9.0f);
    if (b[0] != -1.0f * t || b[1] != 2.0f * t || b[2] != -2.0f * t) return 6;
    float e[3] = {1e-30f, 0.0f, 0.0f};  // squared length underflows to zero
    if (Normalize__7VIVect3(e) != 0 || e[0] != 1e-30f) return 7;
    return 0;
}
