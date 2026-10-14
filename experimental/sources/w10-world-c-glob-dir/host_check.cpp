// Host contract for the globDir storage. Not an EE plan input.
#include <cstdio>
#include "glob_dir.cpp"

int main() {
    bool zero = true;
    for (char c : globDir) zero &= c == 0;
    if (!zero || sizeof(globDir) != 2080) { std::fprintf(stderr, "fail: globDir is 2080 zero bytes\n"); return 1; }
    std::printf("globDir contract passed\n");
    return 0;
}
