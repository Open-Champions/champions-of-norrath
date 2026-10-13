// Host contract for __tf14SpellHarmTouch; not an EE input. Includes the source.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5Spell[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti14SpellHarmTouch) return "14SpellHarmTouch";
    if (p == __ti5Spell) return "5Spell";
    return "?";
}
void* __tf5Spell() { calls += "__tf5Spell;"; return nullptr; }
void __rtti_si(void* t, const char* name, const void* b) {
    calls += std::string("si(") + node_name(t) + "," + name + "," + node_name(b) + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti14SpellHarmTouch, 0, 12); calls.clear();
    expect(__tf14SpellHarmTouch() == __ti14SpellHarmTouch, "returns the node");
    expect(calls == "__tf5Spell;si(14SpellHarmTouch,14SpellHarmTouch,5Spell);", "empty node: base function, then si");
    calls.clear();
    expect(__tf14SpellHarmTouch() == __ti14SpellHarmTouch && calls.empty(), "set node makes no calls");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti14SpellHarmTouch, 0, 12); __ti14SpellHarmTouch[byte] = 1; calls.clear();
        expect(__tf14SpellHarmTouch() == __ti14SpellHarmTouch && calls.empty(), "any nonzero byte of the first word skips");
    }
    std::memset(__ti14SpellHarmTouch, 0, 12); __ti14SpellHarmTouch[4] = 9; __ti14SpellHarmTouch[11] = 4; calls.clear();
    __tf14SpellHarmTouch();
    expect(!calls.empty(), "only the first word gates initialization");
    if (!failures) std::puts("tf14SpellHarmTouch ok");
    return failures ? 1 : 0;
}
