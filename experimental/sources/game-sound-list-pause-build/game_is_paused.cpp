#include "list_pause.hpp"
using namespace norrath_sound_list_pause_detail;

extern "C" int norrath_game_is_paused() {
    const unsigned network_mode=read_byte_fresh(address(norrath_network_mode_byte_view));
    if(network_mode!=0u)
        return signed_word(read_word_fresh(address(norrath_game_pause_word_view)));
    const unsigned cut_mode=read_byte_fresh(address(norrath_game_cut_mode_byte_view));
    // Original branch-delay LW reads coreLoading even when cutMode is nonzero.
    const unsigned captured_loading=read_word_fresh(address(norrath_game_core_loading_word_view));
    if(cut_mode!=0u)return signed_word(captured_loading);
    const int frame_number=signed_word(read_word_fresh(address(norrath_game_frame_number_word_view)));
    if(frame_number<60)return 0;
    if(read_word_fresh(address(norrath_game_pause_word_view))!=0u)return 1;
    if(read_word_fresh(address(norrath_game_core_loading_word_view))!=0u)return 1;
    if(read_byte_fresh(address(norrath_game_hud_no_controller_byte_view))==0u)return 0;
    if(norrath_dialog_is_active()!=0)return 0;
    // Actual post-dialog load is cutMode again, with no cached result.
    if(read_byte_fresh(address(norrath_game_cut_mode_byte_view))!=0u)return 0;
    return 1;
}
