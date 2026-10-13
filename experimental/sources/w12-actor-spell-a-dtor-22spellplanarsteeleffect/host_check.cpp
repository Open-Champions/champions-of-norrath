// Host contract for _$_22SpellPlanarSteelEffect; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_22SpellPlanarSteelEffect[8];
unsigned char vt_5Spell[8];
static std::string calls;
static unsigned char* current;
static int failures;
static const char* table_name(unsigned word) {
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_22SpellPlanarSteelEffect))) return "22SpellPlanarSteelEffect";
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Spell))) return "5Spell";
    return word == 0x3c3c3c3cu ? "-" : "?";
}
static unsigned word_at(int off) { unsigned w; std::memcpy(&w, current + off, 4); return w; }
static void snap(const char* what, int off, long long arg) {
    char b[96]; std::snprintf(b, sizeof b, "%s@%s(%lld);", what, table_name(word_at(off)), arg); calls += b;
}
void light_destructor(void* s, int f) { snap("light", 20, (static_cast<unsigned char*>(s) - current) * 16 + f); }
void lightDeactivate__FP5Light(void* s) { snap("deact", 20, static_cast<unsigned char*>(s) - current); }
void listDestroy__FP8ListHead(void* s) { snap("list", 20, static_cast<unsigned char*>(s) - current); }
void blockFree__FPv(void* s) { snap("free", 20, s == current); }
int main() {
    alignas(16) unsigned char object[160];
    current = object;
    const int cases[] = {0, 1, 2, 3, 4, 5, static_cast<int>(0x80000001u), 0x7ffffffe};
    const char* want[] = {
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);free@5Spell(1);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);free@5Spell(1);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);free@5Spell(1);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);free@5Spell(1);",
        "deact@22SpellPlanarSteelEffect(48);light@22SpellPlanarSteelEffect(770);list@5Spell(4);",
    };
    for (int i = 0; i < 8; ++i) {
        std::memset(object, 0x3c, sizeof object);
        calls.clear();
        destructor_22SpellPlanarSteelEffect(object, cases[i]);
        if (calls != want[i]) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", cases[i], calls.c_str(), want[i]); ++failures; }
    }
    if (!failures) std::puts("dtor22SpellPlanarSteelEffect ok");
    return failures ? 1 : 0;
}
