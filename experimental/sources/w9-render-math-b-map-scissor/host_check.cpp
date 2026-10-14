// Host contract for the mapScissor words. Not an EE plan input.
#include <cstdio>
extern "C" { extern int mapScissorX; extern int mapScissorY; }
int main() {
    if (mapScissorX != 0 || mapScissorY != 0 || &mapScissorX == &mapScissorY) { std::fprintf(stderr, "fail\n"); return 1; }
    std::printf("mapScissor contract passed\n");
    return 0;
}
