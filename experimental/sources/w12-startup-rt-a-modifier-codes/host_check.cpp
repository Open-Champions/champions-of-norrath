// Host check for ModifierCodes. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
#include "modifier_codes.cpp"
int main() {
    if (sizeof(ModifierCodes) != 8) return 1;
    for (int i = 0; i < 8; ++i)
        if (ModifierCodes[i] != 0xe0 + i) return 2;
    return 0;
}
