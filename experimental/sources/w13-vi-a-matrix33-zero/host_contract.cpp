#include "zero.cpp"

int main() {
    float m[12];
    for (int i = 0; i < 12; ++i) m[i] = 7.0f;
    Zero__10VIMatrix33(m);
    for (int i = 0; i < 9; ++i) if (m[i] != 0.0f) return 1 + i;
    for (int i = 9; i < 12; ++i) if (m[i] != 7.0f) return 20;
    return 0;
}
