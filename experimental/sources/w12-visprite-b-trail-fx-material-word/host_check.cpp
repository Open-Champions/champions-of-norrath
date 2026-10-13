// Host check only; not a build input. Checks the width (static_assert in the source) and the initial value from the image.
#include "material.cpp"
#include <cstdio>
int main() {
    if (norrath_trail_material != -1) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
