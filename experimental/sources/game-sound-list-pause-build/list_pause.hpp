#pragma once
#include "sound_sequence.hpp"
extern "C" {
// Same actual free-list word imported by the complete listOnewayLink source.
extern unsigned char norrath_constructor_list_pool_view[];
// Shared exact network-mode byte; no duplicate state/provider is defined.
extern unsigned char norrath_network_mode_byte_view[];
extern unsigned char norrath_game_pause_word_view[];
extern unsigned char norrath_game_cut_mode_byte_view[];
extern unsigned char norrath_game_core_loading_word_view[];
extern unsigned char norrath_game_frame_number_word_view[];
extern unsigned char norrath_game_hud_no_controller_byte_view[];
extern unsigned char norrath_dialog_active_pointer_word_view[];
int norrath_dialog_is_active();
}
namespace norrath_sound_list_pause_detail {
using namespace norrath_sound_sequence_detail;
}
