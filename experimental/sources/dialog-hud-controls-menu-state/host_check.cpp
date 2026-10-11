#include <cstring>

extern "C" unsigned menu;
extern "C" unsigned menuNm;
extern "C" unsigned char startMenuState;
extern "C" unsigned char useRaceMenu;
extern "C" unsigned menuPlayerNm;

int main() {
    const unsigned char zero[4] = {};
    if (std::memcmp(&menu, zero, 4) || std::memcmp(&menuNm, zero, 4)) return 1;
    if (std::memcmp(&menuPlayerNm, zero, 4)) return 2;
    if (sizeof startMenuState != 1 || sizeof useRaceMenu != 1) return 3;
    if (startMenuState != 0 || useRaceMenu != 0) return 4;
    menu = 0x11u;
    menuNm = 0x22u;
    menuPlayerNm = 0x33u;
    startMenuState = 1;
    useRaceMenu = 1;
    if (menu != 0x11u || menuNm != 0x22u || menuPlayerNm != 0x33u) return 5;
    if (startMenuState != 1 || useRaceMenu != 1) return 6;
    return 0;
}
