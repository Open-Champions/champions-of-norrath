// Host contract for __ti5Spell. Not an EE plan input.
#include <cstdio>
#include "ti_spell.cpp"
int main() {
    if (sizeof(__ti5Spell) != 8 || (reinterpret_cast<__UINTPTR_TYPE__>(__ti5Spell) & 3) != 0) return 1;
    for (unsigned i = 0; i < 8; ++i) if (__ti5Spell[i] != 0) return 2;
    std::puts("__ti5Spell ok");
    return 0;
}
