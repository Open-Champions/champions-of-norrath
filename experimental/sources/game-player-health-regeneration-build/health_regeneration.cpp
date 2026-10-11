#include "health_regeneration.hpp"
using namespace norrath_health_regeneration_detail;

extern "C" void norrath_player_regenerate_health(void* actor) {
    const Address a=address(actor);
    if((read_word_fresh(a+12u)&0x20000u)!=0u)return;
    const int player_index=signed_word(read_word_fresh(a+960u));
    const Address record=player_record(player_index);
    const float zero=0.0f;
    const float initial_health=read_float_fresh(record+172u);
    if(initial_health<=zero)return;
    float maximum=read_float_fresh(record+176u);
    float remaining;
    if(initial_health<maximum) {
        const Address coefficients=address(norrath_player_health_coefficients_view);
        const float base=read_float_fresh(coefficients);
        float regeneration=native_multiply(maximum,base);
        const int rank=norrath_player_get_feat_rank(player_index,16ll);
        const float feat=native_signed_to_float(rank);
        // Original delay slot reads this signed byte before either feat branch.
        const int additional=read_signed_byte_fresh(a+1127u);
        if(zero<feat) {
            const float multiplier=read_float_fresh(coefficients+4u);
            const float offset=read_float_fresh(coefficients+8u);
            const float factor=native_add(native_multiply(feat,multiplier),offset);
            regeneration=native_multiply(regeneration,factor);
        }
        const float equipment_multiplier=read_float_fresh(coefficients+12u);
        const float extra=native_signed_to_float(additional);
        regeneration=native_add(regeneration,native_multiply(extra,base));
        const float unit=1.0f;
        Address slot=record+21896u;
        for(int index=0;index<16;++index,slot+=4u) {
            const Address item=read_word_fresh(slot);
            if(item==0u)continue;
            if(read_word_fresh(item)==0u)continue;
            int modifier=0;
            for(int effect=0;effect<4;++effect) {
                const Address field=item+36u+unsigned(effect)*8u;
                if(read_signed_half_fresh(field)==11) {
                    modifier=read_signed_half_fresh(field+2u);
                    break;
                }
            }
            const float value=native_signed_to_float(modifier);
            const float factor=native_add(native_multiply(value,equipment_multiplier),unit);
            regeneration=native_multiply(regeneration,factor);
        }
        const float current=read_float_fresh(record+172u);
        maximum=read_float_fresh(record+176u);
        const float updated=native_add(current,regeneration);
        const bool over=maximum<updated;
        // Native branch delay stores the sum even when it will be overwritten.
        write_float_ordered(record+172u,updated);
        if(over)write_float_ordered(record+172u,maximum);
        remaining=read_float_fresh(record+180u);
    } else remaining=read_float_fresh(record+180u);
    if(!(zero<remaining))return;
    const float current=read_float_fresh(record+172u);
    const float rate=read_float_fresh(record+184u);
    const float updated=native_add(current,rate);
    const bool over=maximum<updated;
    write_float_ordered(record+172u,updated);
    if(over) {
        write_float_ordered(record+172u,maximum);
        remaining=zero;
        write_float_ordered(record+180u,zero);
    }
    const float leftover=native_subtract(remaining,rate);
    const bool below_zero=leftover<zero;
    write_float_ordered(record+180u,leftover);
    if(below_zero)write_float_ordered(record+180u,zero);
}
