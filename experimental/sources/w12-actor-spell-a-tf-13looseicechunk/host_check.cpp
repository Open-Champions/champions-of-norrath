// Host contract for __tf13LooseIceChunk; not an EE input. Callees record their arguments.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti6PhProp[12];
unsigned char __ti4Prop[12];
unsigned char __ti10GameObject[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti13LooseIceChunk) return "13LooseIceChunk";
    if (p == __ti6PhProp) return "6PhProp";
    if (p == __ti4Prop) return "4Prop";
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
    unsigned char* nodes[] = {__ti13LooseIceChunk, __ti6PhProp, __ti4Prop};
    const unsigned sizes[] = {12, 12, 12};
    int n = 3;
    struct Row { unsigned mask; const char* calls; };
    static const Row rows[] = {
        {0u, "tfGO;si(4Prop,4Prop,10GameObject);si(6PhProp,6PhProp,4Prop);si(13LooseIceChunk,13LooseIceChunk,6PhProp);"},
        {4u, "si(6PhProp,6PhProp,4Prop);si(13LooseIceChunk,13LooseIceChunk,6PhProp);"},
        {2u, "si(13LooseIceChunk,13LooseIceChunk,6PhProp);"},
        {6u, "si(13LooseIceChunk,13LooseIceChunk,6PhProp);"},
        {1u, ""},
        {5u, ""},
        {3u, ""},
        {7u, ""},
    };
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        const unsigned char marks[4][4] = {{1,0,0,0},{0,0,0,0x80},{0,2,0,0},{0xff,0xff,0xff,0xff}};
        for (int i = 0; i < n; ++i) { std::memset(nodes[i], 0, sizes[i]); if (r.mask >> i & 1) std::memcpy(nodes[i], marks[pat], 4); }
        calls.clear();
        void* got = __tf13LooseIceChunk();
        if (got != nodes[0]) { std::fprintf(stderr, "fail: wrong node mask %u\n", r.mask); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: mask %u pat %d got %s want %s\n", r.mask, pat, calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("tf13LooseIceChunk ok");
    return failures ? 1 : 0;
}
