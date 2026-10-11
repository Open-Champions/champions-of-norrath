#include "wielding.hpp"
using namespace norrath_wielding_detail;

extern "C" int norrath_inventory_check_wielding_status(int player_index,const void* description) {
    const Address item=address(description),record=player_record(player_index);
    const Address descriptor=read_word_fresh(item);
    int status=0;
    if(read_signed_byte_fresh(descriptor+29u)==1) {
        const Wide flags=read_wide_fresh(item+8u);
        if((flags&(Wide(1u)<<19))!=0u) {
            status=read_word_fresh(record+80u)==4u?0:1;
        } else {
            const Address name=read_word_fresh(descriptor);
            const char* fragment=text(address(norrath_player_wielding_name_fragment_view));
            if(norrath_factory_strstr(text(name),fragment)==nullptr &&
               read_word_fresh(record+80u)==4u)
                status=1;
        }
    } else {
        const unsigned character_class=read_word_fresh(record+80u);
        if(character_class==1u) {
            const int category=read_signed_byte_fresh(descriptor+30u);
            const unsigned slot=read_word_fresh(descriptor+32u);
            if(category==3 || category==4 || slot==8u || slot==4u)status=1;
        } else if(character_class==3u) {
            const int category=read_signed_byte_fresh(descriptor+30u);
            if(category==2 || category==1 || category==3)status=1;
        } else if(character_class==4u) {
            const int category=read_signed_byte_fresh(descriptor+30u);
            const unsigned slot=read_word_fresh(descriptor+32u);
            if(category==2 || category==1 || category==4 || slot==8u || slot==4u)status=1;
            const Wide flags=read_wide_fresh(item+8u);
            if((flags&(Wide(1u)<<36))!=0u)status=0;
        }
    }

    // The original does every stage even after a failure. A missing effect
    // yields requirement0 and is still compared against the signed playerfield.
    const int level=read_signed_byte_fresh(record+88u);
    if(level<requirement(item,14))status=2;
    const int first_attribute_requirement=requirement(item,15);
    if(signed_word(read_word_fresh(record+144u))<first_attribute_requirement)status=3;
    const int second_attribute_requirement=requirement(item,16);
    if(signed_word(read_word_fresh(record+148u))<second_attribute_requirement)status=4;
    const int third_attribute_requirement=requirement(item,17);
    if(signed_word(read_word_fresh(record+152u))<third_attribute_requirement)status=5;
    const int fourth_attribute_requirement=requirement(item,18);
    if(signed_word(read_word_fresh(record+156u))<fourth_attribute_requirement)status=6;

    const int first_skill_rank=norrath_player_get_skill_rank(player_index,60);
    if(first_skill_rank<requirement(item,39))status=7;
    const int second_skill_rank=norrath_player_get_skill_rank(player_index,61);
    if(second_skill_rank<requirement(item,40))status=8;
    if(requirement(item,19)!=0) {
        // Native makes a second fresh first-match scan; its value is not cached.
        const int required_class=signed_word(unsigned(requirement(item,19))-1u);
        if(read_word_fresh(record+80u)!=unsigned(required_class))status=1;
    }
    return status;
}
