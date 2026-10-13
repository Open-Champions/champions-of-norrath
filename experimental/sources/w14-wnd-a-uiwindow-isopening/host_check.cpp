// Host check only; not a build input.
#include "isopening.cpp"
#include <cstdio>
#include <initializer_list>
#include <limits>

int main() {
    bool ok = true;
    const float nan = std::numeric_limits<float>::quiet_NaN();
    for (float a : {1.0f, 0.0f, 0.5f, 2.0f, -1.0f, nan}) {
        for (float b : {1.0f, 0.0f, 0.5f, 2.0f, nan}) {
            float f[16] = {};
            f[10] = a; f[11] = b;
            ok = ok && IsOpening__C10VIUIWindow(f) == ((a < 1.0f && b == 1.0f) ? 1 : 0);
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
