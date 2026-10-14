// Host check only; not a build input.
#include "mul.cpp"
int main() {
    const float m[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    const float v[3] = {1, 10, 100};
    float out[3] = {0, 0, 0};
    if (__ml__FRC7VIVect3RC10VIMatrix33(out, v, m) != out) return 1;
    // row vector times matrix: (1+40+700, 2+50+800, 3+60+900)
    return out[0] == 741.0f && out[1] == 852.0f && out[2] == 963.0f ? 0 : 2;
}
