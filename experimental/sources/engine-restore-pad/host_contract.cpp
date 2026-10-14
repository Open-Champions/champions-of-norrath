#include <cstdint>
#include <cstring>

#include "pad.cpp"
#include "../engine-restore-skip-intro/skip_intro.cpp"
#include "../engine-restore-display-disable/engine_display_disable.cpp"

int main() {
    if (engineDisplayDisable != 3) return 1;
    if (skipIntro != 0) return 2;
    if (sizeof pad != 0x160) return 3;
    for (unsigned i = 0; i < sizeof pad; ++i) if (pad[i] != 0) return 4;
    *reinterpret_cast<unsigned*>(pad + 8) = 16398u;
    if (*reinterpret_cast<unsigned*>(pad + 8) != 16398u) return 5;
    *reinterpret_cast<unsigned*>(pad + 268) = 120u;
    if (*reinterpret_cast<unsigned*>(pad + 268) != 120u) return 6;
    if (pad[0] != 0 || pad[351] != 0) return 7;
    engineDisplayDisable = 4;
    skipIntro = 1;
    if (engineDisplayDisable != 4 || skipIntro != 1) return 8;
    return 0;
}
