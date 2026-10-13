// Host contract for the lightGrid object. Not an EE plan input.
#include <cstdio>
#include "light_grid.cpp"
int main() {
    static_assert(sizeof(lightGrid) == 32400u, "lightGrid extent");
    for (unsigned i = 0; i < sizeof lightGrid; ++i)
        if (lightGrid[i] != 0) { std::fprintf(stderr, "fail: byte %u not zero\n", i); return 1; }
    std::printf("lightGrid contract passed\n");
    return 0;
}
