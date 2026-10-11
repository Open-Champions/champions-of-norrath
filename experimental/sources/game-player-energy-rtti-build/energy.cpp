#include "energy_rtti.hpp"
using namespace norrath_health_regeneration_detail;

extern "C" void norrath_player_regenerate_energy(void* actor) {
    const Address a=address(actor);
    if((read_word_fresh(a+12u)&0x20000u)!=0u)return;
    const int player_index=signed_word(read_word_fresh(a+960u));
    const Address record=player_record(player_index);
    const float zero=0.0f;
    if(read_float_fresh(record+172u)<=zero)return;
    const float initial_energy=read_float_fresh(record+196u);
    float maximum=read_float_fresh(record+200u);
    float remaining;
    if(initial_energy<maximum) {
        const Address coefficients=address(norrath_player_energy_coefficients_view);
        const float base=read_float_fresh(coefficients);
        float regeneration=native_multiply(maximum,base);
        const int feat_rank=norrath_player_get_feat_rank(player_index,512ll);
        const float feat=native_signed_to_float(feat_rank);
        const int additional=read_signed_byte_fresh(a+1125u);
        if(zero<feat) {
            const float multiplier=read_float_fresh(coefficients+4u);
            const float offset=read_float_fresh(coefficients+8u);
            regeneration=native_multiply(regeneration,
                native_add(native_multiply(feat,multiplier),offset));
        }
        const unsigned player_class=read_word_fresh(record+80u);
        const float extra=native_signed_to_float(additional);
        regeneration=native_add(regeneration,native_multiply(extra,base));
        // Class zero clears the result after the preceding genuine operations.
        if(player_class==0u)regeneration=zero;
        const float equipment_multiplier=read_float_fresh(coefficients+12u);
        const float unit=1.0f;
        Address slot=record+21896u;
        for(int index=0;index<16;++index,slot+=4u) {
            const Address item=read_word_fresh(slot);
            if(item==0u)continue;
            if(read_word_fresh(item)==0u)continue;
            int modifier=0;
            for(int effect=0;effect<4;++effect) {
                const Address field=item+36u+unsigned(effect)*8u;
                if(read_signed_half_fresh(field)==12) {
                    modifier=read_signed_half_fresh(field+2u);
                    break;
                }
            }
            const float value=native_signed_to_float(modifier);
            const float factor=native_add(native_multiply(value,equipment_multiplier),unit);
            regeneration=native_multiply(regeneration,factor);
        }
        const void* wanted=norrath_spell_blessed_type_info();
        void* spell=norrath_creature_find_running_spell(actor,wanted);
        if(spell!=nullptr) {
            const int amount=norrath_spell_blessed_regeneration(spell);
            const float multiplier=read_float_fresh(coefficients+16u);
            const float value=native_signed_to_float(amount);
            const float product=native_multiply(regeneration,native_multiply(value,multiplier));
            regeneration=native_add(regeneration,product);
        }
        // This index is fresh after the spell callbacks, while record stays captured.
        const int fresh_index=signed_word(read_word_fresh(a+960u));
        const int skill_rank=norrath_player_get_skill_rank(fresh_index,19);
        if(skill_rank!=0) {
            const Address descriptor=read_word_fresh(address(norrath_player_skill_catalog_view)+76u);
            const int amount=norrath_skill_regeneration_amount(pointer(descriptor),skill_rank);
            const float multiplier=read_float_fresh(coefficients+20u);
            const float value=native_signed_to_float(amount);
            const float product=native_multiply(regeneration,native_multiply(value,multiplier));
            regeneration=native_add(regeneration,product);
        }
        const float current=read_float_fresh(record+196u);
        maximum=read_float_fresh(record+200u);
        const float updated=native_add(current,regeneration);
        const bool over=maximum<updated;
        write_float_ordered(record+196u,updated);
        if(over)write_float_ordered(record+196u,maximum);
        remaining=read_float_fresh(record+204u);
    } else remaining=read_float_fresh(record+204u);
    if(!(zero<remaining))return;
    const float current=read_float_fresh(record+196u);
    const float rate=read_float_fresh(record+188u);
    const float updated=native_add(current,rate);
    const bool over=maximum<updated;
    write_float_ordered(record+196u,updated);
    if(over) {
        write_float_ordered(record+196u,maximum);
        remaining=zero;
        write_float_ordered(record+204u,zero);
    }
    const float leftover=native_subtract(remaining,rate);
    const bool below_zero=leftover<zero;
    write_float_ordered(record+204u,leftover);
    if(below_zero)write_float_ordered(record+204u,zero);
}
