// Host check for globalIcons. Not an EE plan input.
#include "global_icons.cpp"
int main() {
    static_assert(sizeof(globalIcons) == 152, "size");
    for (unsigned char b : globalIcons) if (b != 0) return 1;
    if (reinterpret_cast<__UINTPTR_TYPE__>(globalIcons) % 4 != 0) return 2;
    return 0;
}
