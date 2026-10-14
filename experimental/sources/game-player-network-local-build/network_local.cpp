#include "network_local.hpp"
using namespace norrath_skill_progression_detail;

extern "C" int norrath_net_player_is_local(int player_index) {
    const int partners=signed_word(read_word_fresh(address(norrath_network_partner_count_word_view)));
    if(player_index>=partners)return 1;
    // Actual original low32 multiplication; negative indices remain admitted.
    const Address field=address(norrath_network_player_local_flags_view)+unsigned(player_index)*17632u;
    const unsigned value=*static_cast<const volatile unsigned char*>(bytes(field));
    return value<128u?int(value):int(value)-256;
}
