#include "norrath_views.cpp"
#include <cstdio>
#include <cstring>

int main() {
    int bad = sizeof(norrath_got_player_start_this_level_bytes) != 1 || sizeof(norrath_keep_player_positions_bytes) != 1 ||
              sizeof(norrath_restore_broadcast_mask_word) != 4;
    bad |= norrath_got_player_start_this_level_bytes[0] || norrath_keep_player_positions_bytes[0];
    for (int i = 0; i < 4; ++i) bad |= norrath_restore_broadcast_mask_word[i];
    unsigned bits;
    std::memcpy(&bits, &norrath_menu_local_t_004844e8, 4);
    bad |= bits != 0x3f800000u || norrath_menu_word_00484514 != 116;
    std::printf(bad ? "FAIL\n" : "ok: sizes and initial values\n");
    return bad;
}
