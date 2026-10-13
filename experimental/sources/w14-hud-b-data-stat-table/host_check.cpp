// Host check for the statTable object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_stat_table.cpp"

#include <cstring>

int main() {
    if (sizeof(statTable) != 4 * sizeof(const char*)) return 1;
    const char* const expected[4] = {"selectStatStrength", "selectStatIntel", "selectStatDexterity", "selectStatStamina"};
    for (int i = 0; i < 4; ++i)
        if (std::strcmp(statTable[i], expected[i]) != 0) return 2;
    return 0;
}
