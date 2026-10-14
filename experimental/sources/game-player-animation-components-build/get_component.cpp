#include "animation_components.hpp"
using namespace norrath_animation_components_detail;

extern "C" void* norrath_animation_get_component(void* animation_state,void* animation_header) {
    const Address state=address(animation_state),header=address(animation_header);
    if(state==0u||header==0u)return nullptr;
    const int count=read_signed_byte_fresh(state+4u);
    if(count<=0)return nullptr;
    for(int index=0;index<count;++index) {
        const Address slot=state+8u+unsigned(index)*32u;
        if(read_word_fresh(slot)==header)return pointer(slot);
    }
    return nullptr;
}
