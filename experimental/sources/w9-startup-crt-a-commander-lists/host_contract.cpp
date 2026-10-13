#include "commander_lists.cpp"

#include <cstdio>

int main() {
    if (sizeof(WoodElfSoldier_commanderList) != 40 || sizeof(Orc_commanderList) != 80 ||
        sizeof(Wraith_rWraithList) != 240) return 1;
    for (unsigned word : WoodElfSoldier_commanderList) if (word != 0u) return 2;
    for (unsigned word : Orc_commanderList) if (word != 0u) return 3;
    for (unsigned word : Wraith_rWraithList) if (word != 0u) return 4;
    if (static_cast<void*>(WoodElfSoldier_commanderList) == static_cast<void*>(Orc_commanderList) ||
        static_cast<void*>(Orc_commanderList) == static_cast<void*>(Wraith_rWraithList)) return 5;
    std::puts("commander list storage contract passed");
    return 0;
}
