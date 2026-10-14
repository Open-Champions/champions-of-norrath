// Host check for the lump state words. Not an EE plan input.
#include <cstdio>
#include "lump_state.cpp"
int main() {
    int failures = 0;
    if (sizeof(initializedLump) != 4) { std::fprintf(stderr, "fail: initializedLump size\n"); ++failures; }
    if (myLumpBuffer != nullptr || initializedLump != 0) { std::fprintf(stderr, "fail: initial values\n"); ++failures; }
    if (failures) return 1;
    std::printf("lump state storage check passed\n");
    return 0;
}
