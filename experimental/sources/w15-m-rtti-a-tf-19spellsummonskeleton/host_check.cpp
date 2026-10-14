// Host contract for __tf19SpellSummonSkeleton; not an EE input. Callees record their arguments; the nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti19SpellSummonSkeleton[12] __attribute__((aligned(4)));
unsigned char __ti5Spell[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti5Spell ? "5Spell" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + (t == __ti19SpellSummonSkeleton ? "19SpellSummonSkeleton" : "?") + "," +
             (base == __ti5Spell ? "5Spell" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; bool base_set; const char* calls; };
    static const Row rows[] = {
        {false, false, "user(5Spell,5Spell);si(19SpellSummonSkeleton,5Spell,19SpellSummonSkeleton);"},
        {false, true, "si(19SpellSummonSkeleton,5Spell,19SpellSummonSkeleton);"},
        {true, false, ""},
        {true, true, ""},
    };
    // Any nonzero first word skips the work for that node; a zero word registers it. Trailing bytes are not read.
    static const unsigned char set_words[2][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}};
    for (const Row& r : rows) for (int pat = 0; pat < 2; ++pat) {
        std::memset(__ti19SpellSummonSkeleton, 0x5a, sizeof __ti19SpellSummonSkeleton);
        std::memset(__ti5Spell, 0x5a, sizeof __ti5Spell);
        if (r.set) std::memcpy(__ti19SpellSummonSkeleton, set_words[pat], 4);
        else std::memset(__ti19SpellSummonSkeleton, 0, 4);
        if (r.base_set) std::memcpy(__ti5Spell, set_words[pat], 4);
        else std::memset(__ti5Spell, 0, 4);
        calls.clear();
        void* got = __tf19SpellSummonSkeleton();
        if (got != __ti19SpellSummonSkeleton) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w15-m-rtti-a-tf-19spellsummonskeleton ok");
    return failures ? 1 : 0;
}
