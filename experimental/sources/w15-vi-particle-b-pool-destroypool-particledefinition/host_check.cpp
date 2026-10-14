// Host check only; not a build input.
#include "destroypool.cpp"
#include <cstdio>
static int log_n, log_v[16], begin_first, steps;
extern "C" int Begin__Ct6VIPool1ZP20VIParticleDefinition(const void*) { log_v[log_n++] = -1000; return begin_first; }
extern "C" int Next__Ct6VIPool1ZP20VIParticleDefinitioni(const void*, int i) { log_v[log_n++] = i; return ++steps >= 4 ? 0x7fffffff : i + 2; }
int main() {
    int dummy;
    begin_first = 0x7fffffff; log_n = steps = 0;
    if (DestroyPool__t6VIPool1ZP20VIParticleDefinition(&dummy) != 0 || log_n != 1) { std::puts("FAIL"); return 1; }
    begin_first = 3; log_n = steps = 0;
    if (DestroyPool__t6VIPool1ZP20VIParticleDefinition(&dummy) != 0 || log_n != 5 || log_v[1] != 3 || log_v[2] != 5 || log_v[3] != 7 || log_v[4] != 9) { std::puts("FAIL"); return 1; }
    std::puts("ok");
    return 0;
}
