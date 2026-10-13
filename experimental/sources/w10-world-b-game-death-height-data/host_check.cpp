// Host check for gameDeathHeight. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "game_death_height_data.cpp"
int main() {
    unsigned bits; std::memcpy(&bits, &gameDeathHeight, 4);
    if (bits != 0xc7c35000u || sizeof gameDeathHeight != 4) return 1;
    std::puts("gameDeathHeight check passed");
    return 0;
}
