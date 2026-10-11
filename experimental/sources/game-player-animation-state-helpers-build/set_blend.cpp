#include "animation_state_helpers.hpp"
using namespace norrath_animation_state_detail;

extern "C" void* norrath_animation_set_blend(void* animation_state,void* animation_header,float blend) {
    if(animation_state==nullptr || animation_header==nullptr)return nullptr;
    const Address state=address(animation_state);
    const int count=read_signed_byte_fresh(state+4u);
    if(count<=0)return nullptr;
    Address component=0u;
    for(int index=0;index<count;++index){
        const Address current=state+8u+unsigned(index)*32u;
        if(read_word_fresh(current)==address(animation_header)){
            component=current;
            break;
        }
    }
    if(component==0u)return nullptr;
    write_float_ordered(component+8u,blend);
    write_word_ordered(component+16u,0x3f800000u);
    write_word_ordered(component+20u,0u);
    write_word_ordered(component+12u,0u);
    return pointer(component);
}
