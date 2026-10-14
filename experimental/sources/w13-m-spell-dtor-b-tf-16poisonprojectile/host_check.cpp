// Host contract for __tf16PoisonProjectile; not an EE input. Callees record their arguments; the type-info nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10Projectile[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti16PoisonProjectile) return "16PoisonProjectile";
    if (p == __ti10Projectile) return "10Projectile";
    return "?";
}
extern "C" void* __tf10Projectile() { calls += "__tf10Projectile;"; return __ti10Projectile; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + node_name(t) + "," + node_name(base) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    unsigned char* nodes[] = {__ti16PoisonProjectile};
    const unsigned sizes[] = {12};
    const int n = 1;
    struct Row { unsigned mask; const char* calls; };
    static const Row rows[] = {
        {0u, "__tf10Projectile;si(16PoisonProjectile,10Projectile);"},
        {1u, ""},
    };
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        const unsigned char marks[4][4] = {{1,0,0,0},{0,0,0,0x80},{0,2,0,0},{0xff,0xff,0xff,0xff}};
        for (int i = 0; i < n; ++i) { std::memset(nodes[i], 0, sizes[i]); if (r.mask >> i & 1) std::memcpy(nodes[i], marks[pat], 4); }
        calls.clear();
        void* got = __tf16PoisonProjectile();
        if (got != nodes[0]) { std::fprintf(stderr, "fail: wrong node mask %u\n", r.mask); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: mask %u pat %d got %s want %s\n", r.mask, pat, calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("tf16PoisonProjectile ok");
    return failures ? 1 : 0;
}
