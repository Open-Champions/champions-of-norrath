#include "storage.hpp"
#include <cstdio>
int main() {
    static const unsigned char expect[1] = { 0x00 };
    for (unsigned i = 0; i != 1; ++i) {
        if (norrath_game_cut_mode_byte_view[i] != expect[i]) return 1;
    }
    norrath_game_cut_mode_byte_view[0] = static_cast<unsigned char>(norrath_game_cut_mode_byte_view[0] ^ 0x5au);
    if (norrath_game_cut_mode_byte_view[0] == expect[0]) return 2;
    norrath_game_cut_mode_byte_view[0] = expect[0];
    if (norrath_game_cut_mode_byte_view[0] != expect[0]) return 3;
    std::puts("Define the cut-mode byte.");
    return 0;
}
