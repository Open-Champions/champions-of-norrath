#include "network_send.hpp"
using namespace norrath_network_send_detail;

extern "C" int norrath_net_get_local_player_number(int ordinal) {
    const unsigned mode=read_byte_fresh(address(norrath_network_mode_byte_view));
    // The original branch-delay LW captures this count even when mode is zero.
    const int count=signed_word(read_word_fresh(address(norrath_network_partner_count_word_view)));
    if(mode==0u)return ordinal;
    Address flag=address(norrath_network_player_local_flags_view);
    unsigned remaining=unsigned(ordinal);
    int index=0;
    while(index<count) {
        const unsigned local=read_byte_fresh(flag);
        flag+=17632u;
        if(local!=0u) {
            --remaining;
            if(remaining==0xffffffffu)return index;
        }
        index=signed_word(unsigned(index)+1u);
    }
    return -1;
}
