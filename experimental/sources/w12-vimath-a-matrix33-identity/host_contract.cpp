#include "identity.cpp"
#include <string.h>

static float* zero_arg;
static int zero_calls;
extern "C" void Zero__10VIMatrix33(float* m) {
    zero_arg = m;
    ++zero_calls;
    for (int i = 0; i < 9; ++i) m[i] = 0.0f;  // stands in for the real zeroing
}

int main() {
    float m[16];
    for (int i = 0; i < 16; ++i) m[i] = 5.0f;
    Identity__10VIMatrix33(m);
    if (zero_calls != 1 || zero_arg != m) return 1;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            if (m[3 * r + c] != (r == c ? 1.0f : 0.0f)) return 2 + 3 * r + c;
    for (int i = 9; i < 16; ++i) if (m[i] != 5.0f) return 20;
    return 0;
}
