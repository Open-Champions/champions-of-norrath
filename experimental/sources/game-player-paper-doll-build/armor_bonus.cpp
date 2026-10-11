#include "paper_doll.hpp"
using namespace norrath_paper_doll_detail;

extern "C" int norrath_item_get_armor_bonus(const void* description) {
    const Address item=address(description);
    const Address definition=read_word_fresh(item);
    const int base=read_signed_half_fresh(definition+24u);
    if(first_effect_value(item,13)==0)return base;
    const float base_float=native_signed_to_float(base);
    // Native scans13 a second time after capturing/converting the base value.
    const int percent_effect=first_effect_value(item,13);
    const float effect_float=native_signed_to_float(percent_effect);
    const float percent=read_float_fresh(address(norrath_item_armor_percent_coefficient_view));
    const float scaled=native_multiply(effect_float,percent);
    const float adjustment=native_multiply(base_float,scaled);
    const unsigned native_integer=ee_cvt_w_s_word(adjustment);
    return signed_word(unsigned(base)+native_integer);
}
