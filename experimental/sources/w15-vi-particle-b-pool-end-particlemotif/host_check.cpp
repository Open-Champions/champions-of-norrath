// Host check only; not a build input.
#include "end.cpp"
#include <cstdio>
int main() {
    unsigned char pool[64] = {0};
    if (End__Ct6VIPool1ZP15VIParticleMotif(pool) != 0x7fffffff || End__Ct6VIPool1ZP15VIParticleMotif(0) != 0x7fffffff) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
