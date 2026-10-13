// Host check only; not a build input. Checks the width (static_assert in the source) and the initial value from the image.
#include "inactive.cpp"
#include <cstdio>
int main() {
    for (unsigned w : norrath_trail_inactive) if (w != 0) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
