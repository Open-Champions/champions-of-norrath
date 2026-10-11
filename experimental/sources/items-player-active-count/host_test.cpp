#include <cstring>
extern "C" unsigned char norrath_player_active_count_word_view[];
int main() {
    static const unsigned char want[] = {0x01, 0x00, 0x00, 0x00};
    return std::memcmp(norrath_player_active_count_word_view, want, sizeof want) == 0 ? 0 : 1;
}
