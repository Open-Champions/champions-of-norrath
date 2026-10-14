// Host check for vt_SpellRoot. Not an EE plan input; it includes the definition file.
#include "vt_SpellRoot.cpp"
int main() {
    if (sizeof vt_SpellRoot != 0x30) return 1;
    for (unsigned char byte : vt_SpellRoot) if (byte != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(vt_SpellRoot) % 4u != 0) return 3;
    return 0;
}
