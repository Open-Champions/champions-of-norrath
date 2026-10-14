#include "config_strings.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static const char* want[9] = {"c_attack", "c_magic1", "c_magic2", "c_action", "c_block",
                                  "c_health", "c_mana", "c_automap", "c_weapontoggle"};
    if (sizeof configStrings != 36 && sizeof configStrings != 72) return 1;
    if (sizeof configStrings / sizeof configStrings[0] != 9) return 2;
    for (int i = 0; i < 9; ++i) if (std::strcmp(configStrings[i], want[i]) != 0) return 3 + i;
    std::puts("config strings passed");
    return 0;
}
