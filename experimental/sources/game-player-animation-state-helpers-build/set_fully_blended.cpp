#include "animation_state_helpers.hpp"
using namespace norrath_animation_state_detail;

extern "C" unsigned norrath_animation_set_to_end_fully_blended(void* animation_state) {
    const Address state=address(animation_state);
    const Address model=read_word_fresh(state);
    if(model!=0u){
        // Opposite to frame-reset's detach order: state pointer first here.
        write_word_ordered(state,0u);
        write_word_ordered(model+1552u,0u);
    }
    const int count=read_signed_byte_fresh(state+4u);
    if(count==0)return 0u;
    const Address component=state+8u+unsigned(count-1)*32u;
    const Address header=read_word_fresh(component);
    write_word_ordered(component+16u,0x3f800000u);
    const int duration_word=signed_word(read_word_fresh(header+4u));
    const float duration=ordered_signed_to_float(duration_word);
    write_word_ordered(component+8u,0x3f800000u);
    write_float_ordered(component+24u,duration);
    return component;
}
