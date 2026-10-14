#pragma once
#include "inventory_actions.hpp"
extern "C" {
// Genuine full108-byte dependency, independently reconstructed here. No old
// diagnostic implementation, storage or synthetic provider is incorporated.
int norrath_net_get_local_player_number(int ordinal);
// Exact observed data address views, with no object/array capacity or owner.
extern unsigned char norrath_network_mode_byte_view[];
extern unsigned char norrath_network_message_callbacks_view[];
extern unsigned char norrath_network_send_cursor_words_view[];
extern unsigned char norrath_network_send_ring_bytes_view[];
}
namespace norrath_network_send_detail {
using namespace norrath_inventory_actions_detail;
inline Address partner_field(Address first,int player_number) {
    return first+unsigned(player_number)*17632u;
}
inline Address header_destination(Address ring,unsigned raw_cursor) {
    // Original DIV/MFHI uses signed target32 remainder with divisor8191.
    // Negative remainders address bytes before this view; no clamp is added.
    const int remainder=signed_word(raw_cursor)%8191;
    return ring+unsigned(remainder);
}
}
