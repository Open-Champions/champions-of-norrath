#include "get_game_options.cpp"

#include <cstdio>

char sGameOptions[64];

int main() {
    if (VIGetGameOptions__Fv() != static_cast<void*>(sGameOptions)) return 1;
    if (VIGetGameOptions__Fv() != VIGetGameOptions__Fv()) return 2;
    std::puts("vi get game options contract passed");
    return 0;
}
