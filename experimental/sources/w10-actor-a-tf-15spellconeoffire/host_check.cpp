// Host check only; not an EE build input.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti5Spell[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti15SpellConeOfFire) return "15SpellConeOfFire";
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
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s (calls %s)\n", what, calls.c_str()); ++failures; } }
int main() {
    std::memset(__ti15SpellConeOfFire, 0, 12); std::memset(__ti5Spell, 0, 8); calls.clear();
    expect(__tf15SpellConeOfFire() == __ti15SpellConeOfFire, "returns the outer node");
    expect(calls == "user(5Spell,5Spell);si(15SpellConeOfFire,15SpellConeOfFire,5Spell);", "both empty: base user init, then derived si");
    calls.clear();
    expect(__tf15SpellConeOfFire() == __ti15SpellConeOfFire && calls.empty(), "set outer node makes no calls");
    std::memset(__ti15SpellConeOfFire, 0, 12); calls.clear();  // base node stays set
    __tf15SpellConeOfFire();
    expect(calls == "si(15SpellConeOfFire,15SpellConeOfFire,5Spell);", "set base node skips the user init");
    for (int byte = 0; byte < 4; ++byte) {
        std::memset(__ti15SpellConeOfFire, 0, 12); __ti15SpellConeOfFire[byte] = 1; calls.clear();
        expect(__tf15SpellConeOfFire() == __ti15SpellConeOfFire && calls.empty(), "any nonzero byte of the first word skips");
    }
    if (!failures) std::puts("tf15SpellConeOfFire ok");
    return failures ? 1 : 0;
}
