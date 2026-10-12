#include <cstdio>
#include <cstring>
extern "C" int amxPlayersLoaded__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2 + 16] = {};

int main() {
    std::memset(nmPlayers, 0, 4);
    std::memset(norrath_player_progression_records_view, 0, sizeof norrath_player_progression_records_view);
    if (amxPlayersLoaded__FP5__amxPi(nullptr, nullptr) != 0) return 1;
    nmPlayers[0] = 2;
    if (amxPlayersLoaded__FP5__amxPi(nullptr, nullptr) != 0) return 2;
    static int marker = 1;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872 + 23984) = &marker;
    if (amxPlayersLoaded__FP5__amxPi(nullptr, nullptr) != 1) return 3;
    std::puts("amxPlayersLoaded passed");
    return 0;
}
