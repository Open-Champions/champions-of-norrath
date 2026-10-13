// Host contract for __tf10WeaponRack; not an EE input. Includes the source.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti10GameObject[12];
static std::string calls;
extern "C" {
void* __tf10GameObject() { calls += "tf10GameObject;"; return __ti10GameObject; }
void* __rtti_si(void* t, const char* n, const void* b) {
    calls += std::string("si:") + (t == __ti10WeaponRack ? "own" : "bad") + ":" + n + ":" + (b == __ti10GameObject ? "base" : "bad") + ";";
    return t;
}
}
int main() {
    for (unsigned first : {0u, 1u, 0x1234u, 0xffffffffu}) {
        std::memcpy(__ti10WeaponRack, &first, 4); calls.clear();
        void* r = __tf10WeaponRack();
        std::string want = first == 0 ? "tf10GameObject;si:own:10WeaponRack:base;" : "";
        if (r != __ti10WeaponRack || calls != want) { std::printf("fail first=%u got %s\n", first, calls.c_str()); return 1; }
    }
    return 0;
}
