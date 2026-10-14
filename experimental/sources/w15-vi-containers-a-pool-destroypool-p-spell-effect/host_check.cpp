// Host check only; not a build input. A scripted pool of live indices 3, 5, 9 with end value 0x7fffffff.
#include "destroypool.cpp"
#include <string>
static std::string trace;
static const void* the_pool;
static int ends;
extern "C" int Begin__Ct6VIPool1ZP13VISpellEffect(const void* p) { trace += p == the_pool ? "B;" : "B?;"; return 3; }
extern "C" int End__Ct6VIPool1ZP13VISpellEffect(const void* p) { ++ends; trace += p == the_pool ? "E;" : "E?;"; return 0x7fffffff; }
extern "C" int Next__Ct6VIPool1ZP13VISpellEffecti(const void* p, int it) {
    trace += (p == the_pool ? "N" : "N?") + std::to_string(it) + ";";
    return it == 3 ? 5 : it == 5 ? 9 : 0x7fffffff;
}
int main() {
    unsigned pool[8] = {};
    the_pool = pool;
    if (DestroyPool__t6VIPool1ZP13VISpellEffect(pool) != 0) return 1;
    if (trace != "B;E;N3;E;N5;E;N9;E;") return 2;
    for (unsigned w : pool) if (w != 0) return 3;
    return 0;
}
