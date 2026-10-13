// Host contract for megatileSeed and megatileRot. Not an EE plan input.
#include <cstdio>
extern "C" { extern int megatileSeed; extern int megatileRot; }
int main() {
    if (megatileSeed != 0 || megatileRot != 0 || &megatileSeed == &megatileRot) { std::fprintf(stderr, "fail\n"); return 1; }
    std::printf("megatile seed/rot contract passed\n");
    return 0;
}
