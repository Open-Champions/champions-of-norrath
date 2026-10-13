// Host contract for __tf7AntWave; not an EE input. Callees record their arguments; the type-info nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8Creature[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti7AntWave) return "7AntWave";
    if (p == __ti8Creature) return "8Creature";
    return "?";
}
extern "C" void* __tf8Creature() { calls += "__tf8Creature;"; return __ti8Creature; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + node_name(t) + "," + node_name(base) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    unsigned char* nodes[] = {__ti7AntWave};
    const unsigned sizes[] = {12};
    const int n = 1;
    struct Row { unsigned mask; const char* calls; };
    static const Row rows[] = {
        {0u, "__tf8Creature;si(7AntWave,8Creature);"},
        {1u, ""},
    };
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        const unsigned char marks[4][4] = {{1,0,0,0},{0,0,0,0x80},{0,2,0,0},{0xff,0xff,0xff,0xff}};
        for (int i = 0; i < n; ++i) { std::memset(nodes[i], 0, sizes[i]); if (r.mask >> i & 1) std::memcpy(nodes[i], marks[pat], 4); }
        calls.clear();
        void* got = __tf7AntWave();
        if (got != nodes[0]) { std::fprintf(stderr, "fail: wrong node mask %u\n", r.mask); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: mask %u pat %d got %s want %s\n", r.mask, pat, calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("tf7AntWave ok");
    return failures ? 1 : 0;
}
