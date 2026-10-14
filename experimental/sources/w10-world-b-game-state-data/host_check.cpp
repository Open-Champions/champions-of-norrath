// Host check for gameStateData. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
#include <cstdio>
#include "game_state_data.cpp"
int main() {
    if (sizeof gameStateData != 8192) return 1;
    for (unsigned i = 0; i != sizeof gameStateData; ++i) if (gameStateData[i] != 0) return 2;
    std::puts("gameStateData check passed");
    return 0;
}
