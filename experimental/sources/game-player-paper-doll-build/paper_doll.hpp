#pragma once
#include "health_regeneration.hpp"
extern "C" {
int norrath_player_recompute_paper_doll_modifiers(int player_index);
int norrath_item_get_armor_bonus(const void* description);
// Same genuine architectural CVT.W.S/MFC1 provider already integrated.
unsigned ee_cvt_w_s_word(float value);
extern const unsigned char norrath_paper_doll_percent_coefficient_view[];
extern const unsigned char norrath_item_armor_percent_coefficient_view[];
}
namespace norrath_paper_doll_detail {
using namespace norrath_health_regeneration_detail;
inline int first_effect_value(Address item,int identifier) {
    for(unsigned index=0;index<4u;++index) {
        const Address effect=item+36u+8u*index;
        if(read_signed_half_fresh(effect)==identifier)
            return read_signed_half_fresh(effect+2u);
    }
    return 0;
}
}
