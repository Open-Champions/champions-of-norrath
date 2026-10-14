// Host comparison only. Not an EE build input.
#include <cstdio>
#include "dst.cpp"
int main() {
    static_assert(sizeof(dst) == 4, "word");
    if (dst != 0) { std::puts("dst not zero"); return 2; }
    dst = 7;
    std::puts("dst definition ok");
    return 0;
}
