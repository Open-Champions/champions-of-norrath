// Host check for iconName. Not an EE plan input.
#include "icon_name.cpp"
#include <cstring>
int main() {
    static const char* const want[9] = {"ab", "critical", "damage", "eletric", "flaming", "ice", "weight", "gp", "blunt"};
    if (sizeof(iconName) != 9 * sizeof(char*)) return 1;
    for (int i = 0; i < 9; ++i) if (std::strcmp(iconName[i], want[i]) != 0) return 2;
    return 0;
}
