// Host check only; not a build input.
#include "begin.cpp"
#include <cstdio>
#include <cstring>
static const void* end_arg;
extern "C" int End__Ct6VIPool1ZP15VIParticleMotif(const void* pool) { end_arg = pool; return 0x7fffffff; }
int main() {
    alignas(8) unsigned char pool[64];
    const int firsts[] = {0, 1, 7, -2, 0x12345678};
    for (int first : firsts) {
        std::memset(pool, 0x5c, sizeof pool);
        std::memcpy(pool + 16, &first, 4);
        end_arg = 0;
        if (Begin__Ct6VIPool1ZP15VIParticleMotif(pool) != first || end_arg != 0) { std::puts("FAIL"); return 1; }
    }
    int none = -1;
    std::memcpy(pool + 16, &none, 4);
    if (Begin__Ct6VIPool1ZP15VIParticleMotif(pool) != 0x7fffffff || end_arg != pool) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
