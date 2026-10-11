#include "animation_state_helpers.hpp"
using namespace norrath_animation_state_detail;

extern "C" void norrath_animation_append_to_end(void* animation_state,void* animation_header,int flags) {
    if(animation_header==nullptr)return;
    const Address state=address(animation_state);
    const int count=read_signed_byte_fresh(state+4u);
    const int maximum=read_signed_byte_fresh(state+5u);
    const unsigned fresh_count=read_byte_fresh(state+4u);
    const Address previous=state+8u+unsigned(count-1)*32u;
    const Address component=state+8u+unsigned(count)*32u;
    // These genuine reads/CVT and the time load occur even at capacity.
    const Address previous_header=read_word_fresh(previous);
    const int duration_word=signed_word(read_word_fresh(previous_header+4u));
    const float duration=ordered_signed_to_float(duration_word);
    const float previous_time=read_float_fresh(previous+24u);
    if(!(count<maximum))return;
    write_byte_ordered(state+4u,fresh_count+1u);
    const float new_time=ordered_subtract(previous_time,duration);
    const unsigned combined=read_half_fresh(state+6u)|unsigned(flags);
    const Address frame=read_word_fresh(component+4u);
    write_word_ordered(component,address(animation_header));
    write_half_ordered(state+6u,combined);
    write_float_ordered(component+24u,new_time);
    if(frame!=0u){
        write_word_ordered(frame+20u,0u);
        write_word_ordered(component+4u,0u);
    }
    write_half_ordered(component+28u,unsigned(flags));
    write_word_ordered(component+20u,0u);
    write_word_ordered(component+12u,0u);
    write_word_ordered(component+16u,0x3f800000u);
    write_word_ordered(component+8u,0x3f800000u);
}
