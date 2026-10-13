// Host contract, not an EE plan input. Build: c++ -std=c++17 -w host_contract.cpp && ./a.out
#include "mul_vect4_vect3.cpp"
#include <string.h>

int main() {
    float m[16];
    for (int i = 0; i < 16; ++i) m[i] = 0.25f * (i - 5);
    float in[4] = {2.0f, -1.5f, 4.0f, 0.5f};
    float out[4] = {9, 9, 9, 9};
    Mul__C10VIMatrix44RC7VIVect4R7VIVect3(m, in, out);
    for (int i = 0; i < 3; ++i) {
        float want = ((m[i] * 2.0f + m[4 + i] * -1.5f) + m[8 + i] * 4.0f) + m[12 + i] * 0.5f;
        if (out[i] != want) return 1 + i;
    }
    if (out[3] != 9.0f) return 5;
    return 0;
}
