// Host check only; not a build input.
#include "isopen.cpp"
#include <cstdio>
#include <initializer_list>
#include <limits>

int main() {
    bool ok = true;
    const float inf = std::numeric_limits<float>::infinity();
    const float nan = std::numeric_limits<float>::quiet_NaN();
    for (float v : {1.0f, 0.0f, 0.5f, 0.999999f, 1.0000001f, 2.0f, -1.0f, inf, nan}) {
        float f[16] = {};
        f[10] = v;
        ok = ok && IsOpen__C10VIUIWindow(f) == (v == 1.0f ? 1 : 0);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
