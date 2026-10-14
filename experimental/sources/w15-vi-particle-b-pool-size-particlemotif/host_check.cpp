// Host check only; not a build input.
#include "size.cpp"
#include <cstdio>
int main() {
    alignas(8) int pool[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    const int vals[] = {0, 1, 5, -1, 0x7fffffff};
    for (int v : vals) {
        pool[0] = v;
        pool[1] = 99;
        if (Size__Ct6VIPool1ZP15VIParticleMotif(pool) != v || pool[1] != 99) { std::puts("FAIL"); return 1; }
    }
    std::puts("ok");
    return 0;
}
