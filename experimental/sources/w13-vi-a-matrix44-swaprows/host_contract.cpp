#include "swap_rows.cpp"

int main() {
    float m[16];
    for (int i = 0; i < 16; ++i) m[i] = float(i);
    SwapRows__10VIMatrix44ii(m, 1, 3);
    for (int c = 0; c < 4; ++c) {
        if (m[4 + c] != float(12 + c)) return 1;
        if (m[12 + c] != float(4 + c)) return 2;
        if (m[c] != float(c)) return 3;
        if (m[8 + c] != float(8 + c)) return 4;
    }
    SwapRows__10VIMatrix44ii(m, 2, 2);
    for (int c = 0; c < 4; ++c) if (m[8 + c] != float(8 + c)) return 5;
    return 0;
}
