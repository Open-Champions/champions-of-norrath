#include "cut_mode.cpp"
#include <cstdio>
static_assert(sizeof(cutMode) == 1, "cutMode is one byte");
int main() {
    if (cutMode != 0) return 1;
    cutMode = 0x80;
    if (cutMode != 0x80) return 2;
    std::puts("cutMode contract passed");
    return 0;
}
