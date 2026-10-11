#include "paper_doll.hpp"
using namespace norrath_paper_doll_detail;

extern "C" int norrath_player_recompute_paper_doll_modifiers(int player_index) {
    const Address record=player_record(player_index);
    Address actor=read_word_fresh(record+21872u);
    if(actor==0u)return 0;
    // Native clears all five actor modifiers in descending address order.
    for(unsigned remaining=5u;remaining!=0u;--remaining)
        write_word_ordered(actor+88u+(remaining-1u)*4u,0u);
    const int weapon_index=read_signed_byte_fresh(record+23572u);
    const Address selected_weapon=read_word_fresh(record+21960u+unsigned(weapon_index)*4u);
    unsigned armor_total=0u;
    for(unsigned slot=0;slot<16u;++slot) {
        if(selected_weapon!=0u && slot==3u) {
            const Address weapon_description=read_word_fresh(selected_weapon);
            if(read_signed_byte_fresh(weapon_description+30u)==4)continue;
        }
        const Address slot_address=record+21896u+slot*4u;
        const Address item=read_word_fresh(slot_address);
        if(item==0u)continue;
        const Address description=read_word_fresh(item);
        if(read_signed_byte_fresh(description+29u)==1) {
            armor_total+=unsigned(norrath_item_get_armor_bonus(pointer(item)));
            actor=read_word_fresh(record+21872u);
        }
        const float percent=read_float_fresh(address(norrath_paper_doll_percent_coefficient_view));
        for(unsigned modifier=0;modifier<5u;++modifier) {
            const int value=first_effect_value(item,int(modifier));
            const float converted=native_signed_to_float(value);
            const Address output=actor+88u+modifier*4u;
            const float prior=read_float_fresh(output);
            const float scaled=native_multiply(converted,percent);
            write_float_ordered(output,native_add(prior,scaled));
        }
        // Native rereads the current slot/descriptor after all five stores.
        // The later scans still use the previously captured item address.
        const Address current_item=read_word_fresh(slot_address);
        const Address current_description=read_word_fresh(current_item);
        if(read_signed_byte_fresh(current_description+29u)!=1)
            armor_total+=unsigned(first_effect_value(item,13));
        const int health_effect=first_effect_value(item,5);
        const float health_added=native_signed_to_float(health_effect);
        const float maximum_health=read_float_fresh(record+176u);
        write_float_ordered(record+176u,native_add(maximum_health,health_added));
        const int energy_effect=first_effect_value(item,6);
        const float energy_added=native_signed_to_float(energy_effect);
        const float maximum_energy=read_float_fresh(record+200u);
        write_float_ordered(record+200u,native_add(maximum_energy,energy_added));
    }
    const int attribute=signed_word(read_word_fresh(record+152u));
    return signed_word(armor_total+unsigned(attribute/2));
}
