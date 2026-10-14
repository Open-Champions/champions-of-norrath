#include "inventory_actions.hpp"
using namespace norrath_inventory_actions_detail;

extern "C" void norrath_inventory_switch_dual_wield(int player_index,const void* description) {
    const Address record=player_record(player_index),selected=address(description);
    const Address first=read_word_fresh(record+21960u);
    if(selected==first) {
        const Address second=read_word_fresh(record+21968u);
        const int handler=signed_word(read_word_fresh(address(norrath_inventory_message_handler_word_view)));
        // Native frame reserves400 local bytes before register saves; only
        // the first8 are an observed packet. Remaining bytes stay uninitialized.
        // This is no claimed callback capacity, packet layout or native frame ABI.
        alignas(8) unsigned char native_local_workspace[400];
        const Address packet=address(native_local_workspace);
        write_half_ordered(packet+2u,unsigned(player_index));
        write_word_ordered(record+21960u,0u);
        write_word_ordered(record+21896u,0u);
        write_byte_ordered(packet+1u,0u);
        write_byte_ordered(packet,1u);
        write_word_ordered(packet+4u,second);
        norrath_net_send_message(handler,native_local_workspace,8);
        // These happen after the real send/callback, even if it changes state.
        write_word_ordered(record+21900u,0u);
        write_word_ordered(record+21968u,0u);
    } else if(selected==read_word_fresh(record+21968u)) {
        write_byte_ordered(record+23582u,0u);
        write_word_ordered(record+21968u,0u);
        write_word_ordered(record+21900u,0u);
        norrath_player_reset_animations(player_index);
        const Address animation_state=address(norrath_inventory_animation_state_base_view)+unsigned(player_index)*264u;
        norrath_inventory_set_animation(player_index,pointer(animation_state),0);
    }
}
