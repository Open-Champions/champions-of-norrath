#include "sound_startup.hpp"
using namespace norrath_sound_startup_detail;

extern "C" void norrath_animation_state_destroy(void* animation_state,int flags) {
    const Address state=address(animation_state);
    const Address model=read_word_fresh(state);
    if(model!=0u){
        write_word_ordered(model+1552u,0u);
        write_word_ordered(state,0u);
    }
    const int initial_count=signed_byte_at(state+5u);
    unsigned captured_byte=read_byte_fresh(state+5u);
    if(initial_count>0){
        Address slot=state+12u;
        int index=0;
        for(;;){
            const Address frame=read_word_fresh(slot);
            if(frame!=0u){
                write_word_ordered(frame+20u,0u);
                write_word_ordered(slot,0u);
                captured_byte=read_byte_fresh(state+5u);
            }
            ++index;
            const bool again=index<signed_byte_value(captured_byte);
            slot+=32u;
            if(!again)break;
        }
    }
    if((unsigned(flags)&1u)!=0u)norrath_animation_state_deallocate(animation_state);
}
