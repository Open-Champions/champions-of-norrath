#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned char player[24000] = {};
extern "C" int nmPlayers = 0;
extern "C" int activePlayer = 0;
extern "C" int amxSetBestActivePlayer__FP5__amxPi(void*, int*);

static unsigned char header[16];

static void set_header(unsigned char* value) {
    std::memcpy(player + 21872, &value, sizeof value);
}

int main() {
    std::memset(player, 0, sizeof player);
    int params[1] = {0};
    nmPlayers = 2;
    activePlayer = 9;
    if (amxSetBestActivePlayer__FP5__amxPi(nullptr, params) != 1) return 1;
    if (activePlayer != 1) return 2;
    nmPlayers = 1;
    if (amxSetBestActivePlayer__FP5__amxPi(nullptr, params) != 0 || activePlayer != 0) return 3;
    std::memset(header, 0, sizeof header);
    set_header(header);
    nmPlayers = 2;
    if (amxSetBestActivePlayer__FP5__amxPi(nullptr, params) != 0 || activePlayer != 0) return 4;
    int flags = 8;
    std::memcpy(header + 12, &flags, 4);
    if (amxSetBestActivePlayer__FP5__amxPi(nullptr, params) != 1 || activePlayer != 1) return 5;
    nmPlayers = 3;
    if (amxSetBestActivePlayer__FP5__amxPi(nullptr, params) != 0 || activePlayer != 0) return 6;
    flags = 0x18;
    std::memcpy(header + 12, &flags, 4);
    nmPlayers = 2;
    if (amxSetBestActivePlayer__FP5__amxPi(nullptr, params) != 1) return 7;
    std::printf("set best active player passed\n");
    return 0;
}
