#include "player_light_attenuate.cpp"
#include <cstring>
int main() {
    unsigned bits;
    std::memcpy(&bits, &gamePlayerLightAttenuate, 4);
    if (bits != 0u) return 1;
    gamePlayerLightAttenuate = 1.5f;
    std::memcpy(&bits, &gamePlayerLightAttenuate, 4);
    if (bits != 0x3fc00000u) return 2;
    return 0;
}
