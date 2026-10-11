#include <cstdio>
#include <cstring>
extern "C" int amxGivePlayerGold__FP5__amxPi(void*, int*);
extern "C" unsigned char nmPlayers[4] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2] = {};

static unsigned word_at(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8)
        | (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

int main() {
    std::memset(nmPlayers, 0, 4);
    std::memset(norrath_player_progression_records_view, 0, sizeof norrath_player_progression_records_view);
    int cells[] = {4, 5};
    if (amxGivePlayerGold__FP5__amxPi(nullptr, cells) != 0) return 1;
    nmPlayers[0] = 0xff;
    nmPlayers[1] = nmPlayers[2] = nmPlayers[3] = 0xff;
    if (amxGivePlayerGold__FP5__amxPi(nullptr, cells) != 0) return 2;
    if (word_at(norrath_player_progression_records_view + 232) != 0) return 3;
    nmPlayers[0] = 2;
    nmPlayers[1] = nmPlayers[2] = nmPlayers[3] = 0;
    norrath_player_progression_records_view[232] = 10;
    norrath_player_progression_records_view[232 + 23984] = 1;
    norrath_player_progression_records_view[236] = 7;
    if (amxGivePlayerGold__FP5__amxPi(nullptr, cells) != 0) return 4;
    if (word_at(norrath_player_progression_records_view + 232) != 12) return 5;
    if (word_at(norrath_player_progression_records_view + 232 + 23984) != 3) return 6;
    if (norrath_player_progression_records_view[236] != 7) return 7;
    nmPlayers[0] = 1;
    std::memset(norrath_player_progression_records_view + 232, 0xff, 4);
    cells[1] = 1;
    if (amxGivePlayerGold__FP5__amxPi(nullptr, cells) != 0) return 8;
    if (word_at(norrath_player_progression_records_view + 232) != 0) return 9;
    std::puts("amxGivePlayerGold passed");
    return 0;
}
