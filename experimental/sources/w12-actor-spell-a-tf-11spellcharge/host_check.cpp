// Host contract for __tf11SpellCharge; not an EE input. Callees record their arguments.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5Spell[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti11SpellCharge) return "11SpellCharge";
    if (p == __ti5Spell) return "5Spell";
    return "?";
}
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + node_name(t) + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
extern "C" void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
int main() {
    unsigned char* nodes[] = {__ti11SpellCharge, __ti5Spell};
    const unsigned sizes[] = {12, 8};
    int n = 2;
    struct Row { unsigned mask; const char* calls; };
    static const Row rows[] = {
        {0u, "user(5Spell,5Spell);si(11SpellCharge,11SpellCharge,5Spell);"},
        {2u, "si(11SpellCharge,11SpellCharge,5Spell);"},
        {1u, ""},
        {3u, ""},
    };
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        const unsigned char marks[4][4] = {{1,0,0,0},{0,0,0,0x80},{0,2,0,0},{0xff,0xff,0xff,0xff}};
        for (int i = 0; i < n; ++i) { std::memset(nodes[i], 0, sizes[i]); if (r.mask >> i & 1) std::memcpy(nodes[i], marks[pat], 4); }
        calls.clear();
        void* got = __tf11SpellCharge();
        if (got != nodes[0]) { std::fprintf(stderr, "fail: wrong node mask %u\n", r.mask); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: mask %u pat %d got %s want %s\n", r.mask, pat, calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("tf11SpellCharge ok");
    return failures ? 1 : 0;
}
