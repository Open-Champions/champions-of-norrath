#include "load_slot.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof loadSlot == 1, "one byte");
    if (loadSlot != 0) return 1;
    loadSlot = -1;
    if (loadSlot != -1) return 2;
    std::puts("load slot passed");
    return 0;
}
