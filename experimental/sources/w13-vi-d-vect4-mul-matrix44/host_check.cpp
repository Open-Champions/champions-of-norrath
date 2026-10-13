// Host check only; not a build input. Integer-valued inputs make every product and sum exact, so the expected lanes are computed
// independently in double precision.
#include "mul.cpp"
#include <cstdio>

int main() {
    alignas(16) float m[16], v[4], out[4] = {-1, -1, -1, -1};
    bool ok = true;
    for (int seed = 0; seed < 50; ++seed) {
        for (int i = 0; i < 16; ++i) m[i] = static_cast<float>(((i * 7 + seed * 3) % 11) - 5);
        for (int i = 0; i < 4; ++i) v[i] = static_cast<float>(((i * 5 + seed) % 9) - 4);
        float* r = __ml__FRC7VIVect4RC10VIMatrix44(out, v, m);
        if (r != out) ok = false;
        for (int lane = 0; lane < 4; ++lane) {
            double want = 0;
            for (int row = 0; row < 4; ++row) want += static_cast<double>(m[row * 4 + lane]) * v[row];
            if (out[lane] != static_cast<float>(want)) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
