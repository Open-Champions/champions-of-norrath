#include "animation_components.hpp"
using namespace norrath_animation_components_detail;

extern "C" void norrath_animation_reset_frame_states(void* animation_state) {
    const Address state=address(animation_state);
    const Address initial=read_word_fresh(state);
    if(initial!=0u) {
        write_word_ordered(initial+1552u,0u);
        write_word_ordered(state,0u);
    }
    const int initial_count=read_signed_byte_fresh(state+4u);
    // Original BLEZ delay reads this unsigned byte even for a nonpositive count.
    unsigned captured_count=read_byte_fresh(state+4u);
    if(initial_count<=0)return;
    int index=0;
    Address slot=state+12u;
    do {
        const Address frame=read_word_fresh(slot);
        if(frame!=0u) {
            write_word_ordered(frame+20u,0u);
            write_word_ordered(slot,0u);
            captured_count=read_byte_fresh(state+4u);
        }
        // Null frame entries retain the preceding captured count, without read.
        index=signed_word(unsigned(index)+1u);
        slot+=32u;
    } while(index<signed_byte_value(captured_count));
}
