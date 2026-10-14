#include <cstring>
extern "C" unsigned char nmPlayers[];
extern "C" unsigned char norrath_player_active_count_word_view[];
int main() {
    static const unsigned char want[] = {0x01, 0x00, 0x00, 0x00};
    if (+nmPlayers != +norrath_player_active_count_word_view) return 1;
    if (std::memcmp(nmPlayers, want, sizeof want) != 0) return 2;
    norrath_player_active_count_word_view[0] = 2;
    if (nmPlayers[0] != 2) return 3;
    nmPlayers[0] = 1;
    return 0;
}
