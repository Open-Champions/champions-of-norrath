// Host contract for __tf17VehicleBodySphere; not an EE input. Callees record their arguments.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti17VehicleBodySphere) return "17VehicleBodySphere";
    if (p == __ti10GameObject) return "10GameObject";
    return "?";
}
extern "C" void* __tf10GameObject() { calls += "tfGO;"; return nullptr; }
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
int main() {
    unsigned char* nodes[] = {__ti17VehicleBodySphere};
    const unsigned sizes[] = {12};
    int n = 1;
    struct Row { unsigned mask; const char* calls; };
    static const Row rows[] = {
        {0u, "tfGO;si(17VehicleBodySphere,17VehicleBodySphere,10GameObject);"},
        {1u, ""},
    };
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        const unsigned char marks[4][4] = {{1,0,0,0},{0,0,0,0x80},{0,2,0,0},{0xff,0xff,0xff,0xff}};
        for (int i = 0; i < n; ++i) { std::memset(nodes[i], 0, sizes[i]); if (r.mask >> i & 1) std::memcpy(nodes[i], marks[pat], 4); }
        calls.clear();
        void* got = __tf17VehicleBodySphere();
        if (got != nodes[0]) { std::fprintf(stderr, "fail: wrong node mask %u\n", r.mask); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: mask %u pat %d got %s want %s\n", r.mask, pat, calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("tf17VehicleBodySphere ok");
    return failures ? 1 : 0;
}
