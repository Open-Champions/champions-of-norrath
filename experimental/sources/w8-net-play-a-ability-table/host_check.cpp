#include "ability_table.cpp"
int main() {
    if (sizeof(abilityTable) != 80) return 1;
    const int want[20] = {35, 20, 25, 35, 26, 36, 27, 26, 28, 30, 27, 30, 28, 25, 35, 27, 23, 39, 25, 28};
    for (int i = 0; i < 20; ++i) if (abilityTable[i] != want[i]) return 2;
    return 0;
}
