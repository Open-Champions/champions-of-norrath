// Host check only; not an EE input. __rtti_user and __rtti_si are recording stubs; the nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti12SpellHealing[12] __attribute__((aligned(4)));
unsigned char __ti5Spell[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti5Spell ? "base" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + (t == __ti12SpellHealing ? "node" : "?") + "," + name + "," + (base == __ti5Spell ? "base" : "?") + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}

int main() {
    int failures = 0;
    struct Row { bool node_set, base_set; const char* want; };
    static const Row rows[] = {
        {false, false, "user(base,5Spell);si(node,12SpellHealing,base);"},
        {false, true, "si(node,12SpellHealing,base);"},
        {true, false, ""},
        {true, true, ""},
    };
    for (const Row& r : rows) {
        std::memset(__ti12SpellHealing, 0, sizeof __ti12SpellHealing);
        std::memset(__ti5Spell, 0, sizeof __ti5Spell);
        if (r.node_set) std::memcpy(__ti12SpellHealing, "\x01\0\0\0", 4);
        if (r.base_set) std::memcpy(__ti5Spell, "\x01\0\0\0", 4);
        calls.clear();
        if (__tf12SpellHealing() != __ti12SpellHealing) { std::fprintf(stderr, "fail: return\n"); ++failures; }
        if (calls != r.want) { std::fprintf(stderr, "fail: calls %s\n", calls.c_str()); ++failures; }
    }
    if (!failures) std::puts("w16-m-data-a-tf-12spellhealing ok");
    return failures ? 1 : 0;
}
