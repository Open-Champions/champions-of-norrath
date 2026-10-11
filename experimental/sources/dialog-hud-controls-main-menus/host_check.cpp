#include <cstring>

extern "C" unsigned superMainMenu[20];
extern "C" unsigned raceMainMenu[20];

int main() {
    const unsigned super_want[20] = {
        0x004638e0u, 0u, 0u, 0u, 0x00463968u, 0u, 0u, 0u, 0x00463978u, 3u,
        0x004841ccu, 0u, 0x00463990u, 0u, 0u, 0u, 0u,          0u, 0u, 0u};
    const unsigned race_want[20] = {
        0x004638e0u, 0u, 0u, 0u, 0x004639a0u, 0u, 0u, 0u, 0x004639b8u, 0u,
        0u,          0u, 0x004639c8u, 0u, 0u, 0u, 0u,          0u, 0u, 0u};
    if (sizeof superMainMenu != 80 || sizeof raceMainMenu != 80) return 1;
    if (std::memcmp(superMainMenu, super_want, sizeof super_want)) return 2;
    if (std::memcmp(raceMainMenu, race_want, sizeof race_want)) return 3;
    return 0;
}
