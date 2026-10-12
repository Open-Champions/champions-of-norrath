// Host check for the string pool words. Not an EE plan input.
#include <cstdio>
extern "C" { extern char *poolStart, *poolEnd, *poolOffset; }
int main() {
    if (poolStart || poolEnd || poolOffset) { std::fprintf(stderr, "fail: pool words start nonzero\n"); return 1; }
    std::printf("string pool state check passed\n");
    return 0;
}
