#include "animation_components.hpp"
using namespace norrath_animation_components_detail;

extern "C" void norrath_animation_add_transition(void* animation_state,void* animation_header,float rate,int flags) {
    const Address header=address(animation_header);
    if(header==0u)return;
    const Address state=address(animation_state);
    int count=read_signed_byte_fresh(state+4u);
    const int capacity=read_signed_byte_fresh(state+5u);
    if(!(count<capacity)) {
        count=signed_word(unsigned(count)-1u);
        const Address old_slot=state+12u+unsigned(count)*32u;
        const Address old_frame=read_word_fresh(old_slot);
        if(old_frame!=0u) {
            write_word_ordered(old_frame+20u,0u);
            write_word_ordered(old_slot,0u);
        }
    }
    if(count==0)rate=0.0f;
    const Address component=state+8u+unsigned(count)*32u;
    write_byte_ordered(state+4u,unsigned(count)+1u);
    const unsigned old_flags=read_half_fresh(state+6u);
    const Address old_frame=read_word_fresh(component+4u);
    write_word_ordered(component,header);
    write_half_ordered(state+6u,old_flags|unsigned(flags));
    write_word_ordered(component+24u,0u);
    write_half_ordered(component+30u,0u);
    if(old_frame!=0u) {
        write_word_ordered(old_frame+20u,0u);
        write_word_ordered(component+4u,0u);
    }
    const float threshold=read_float_fresh(address(norrath_animation_transition_threshold_view));
    write_half_ordered(component+28u,unsigned(flags));
    const bool below_threshold=rate<threshold;
    float step=1.0f;
    // The native branch delay always clears this before MUL/DIV or final stores.
    write_word_ordered(component+8u,0u);
    if(!below_threshold)step=native_divide(1.0f,native_multiply(rate,60.0f));
    write_float_ordered(component+12u,step);
    write_float_ordered(component+16u,1.0f);
    write_word_ordered(component+20u,0u);
}
