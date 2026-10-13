// Host contract for _$_19SpellHornOmenEffect; not an EE input. Includes the source.
#include "dtor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_19SpellHornOmenEffect[8];
unsigned char vt_5Spell[8];
static std::string calls;
static unsigned char* current;
static int failures;
static const char* table_name(unsigned word) {
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_19SpellHornOmenEffect))) return "19SpellHornOmenEffect";
    if (word == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_5Spell))) return "5Spell";
    return word == 0x3c3c3c3cu ? "-" : "?";
}
static unsigned word_at(int off) { unsigned w; std::memcpy(&w, current + off, 4); return w; }
static void snap(const char* what, int off, long long arg) {
    char b[96]; std::snprintf(b, sizeof b, "%s@%s(%lld);", what, table_name(word_at(off)), arg); calls += b;
}
void listDestroy__FP8ListHead(void* s) { snap("list", 20, static_cast<unsigned char*>(s) - current); }
void P_DeleteParticleSystem__Fi(int id) { snap("pd", 20, id); }
void blockFree__FPv(void* s) { snap("free", 20, s == current); }
int main() {
    alignas(16) unsigned char object[160];
    current = object;
    const int cases[] = {0, 1, 2, 3, 4, 5, static_cast<int>(0x80000001u), 0x7ffffffe};
    const char* want[] = {
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);free@5Spell(1);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);free@5Spell(1);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);free@5Spell(1);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);free@5Spell(1);",
        "pd@19SpellHornOmenEffect(2052718593);list@5Spell(4);",
    };
    for (int i = 0; i < 8; ++i) {
        std::memset(object, 0x3c, sizeof object);
        { int id = 0x7a5a0001; std::memcpy(object + 28, &id, 4); }
        calls.clear();
        destructor_19SpellHornOmenEffect(object, cases[i]);
        if (calls != want[i]) { std::fprintf(stderr, "fail: flags %x got %s want %s\n", cases[i], calls.c_str(), want[i]); ++failures; }
    }
    if (!failures) std::puts("dtor19SpellHornOmenEffect ok");
    return failures ? 1 : 0;
}
