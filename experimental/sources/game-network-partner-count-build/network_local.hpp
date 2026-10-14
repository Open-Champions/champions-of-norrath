#pragma once
#include "skill_progression.hpp"
extern "C" {
int norrath_net_player_is_local(int player_index);
// Actual LOCAL nmPartners4 and unsized network flag base remain unowned views.
extern unsigned char norrath_network_partner_count_word_view[];
extern unsigned char norrath_network_player_local_flags_view[];
}
