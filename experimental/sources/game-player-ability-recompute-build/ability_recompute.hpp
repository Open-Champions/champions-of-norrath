#pragma once
#include "health_regeneration.hpp"
extern "C" {
void norrath_player_recompute_ability_modifiers();
int norrath_player_recompute_paper_doll_modifiers(int player_index);
// Actual sized count/cheat words and anonymous coefficient observations remain
// external data views. No array capacity, class allocation or owner is inferred.
extern unsigned char norrath_player_active_count_word_view[];
extern unsigned char norrath_player_ability_cheat_mode_word_view[];
extern const unsigned char norrath_player_ability_coefficients_view[];
}
namespace norrath_ability_recompute_detail {
using namespace norrath_health_regeneration_detail;
inline int first_item_modifier(Address item,int id) {
    for(unsigned effect=0;effect<4u;++effect) {
        const Address field=item+36u+effect*8u;
        if(read_signed_half_fresh(field)==id)return read_signed_half_fresh(field+2u);
    }
    return 0;
}
inline int half_truncated(int value) {
    const unsigned biased=unsigned(value)+(unsigned(value)>>31);
    return signed_word((biased>>1)|((biased&0x80000000u)!=0u?0x80000000u:0u));
}
inline int wrapped_product(int value,unsigned factor) {
    return signed_word(unsigned(value)*factor);
}
inline void write_byte_ordered(Address out,unsigned value) {
    *static_cast<volatile unsigned char*>(bytes(out))=static_cast<unsigned char>(value);
}
}
