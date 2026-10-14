// Host check only; not a build input.
#include "assign.cpp"
#include <cstdio>
int main() {
    float src[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 77};
    float dst[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 55};
    bool ok = __as__10VIMatrix33RC10VIMatrix33(dst, src) == dst;
    for (int i = 0; i < 9; ++i) ok = ok && dst[i] == src[i];
    ok = ok && dst[9] == 55.0f;
    ok = ok && __as__10VIMatrix33RC10VIMatrix33(dst, dst) == dst && dst[4] == 5.0f;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
