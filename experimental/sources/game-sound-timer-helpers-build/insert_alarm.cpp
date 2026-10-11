#include "sound_timer_helpers.hpp"
using namespace norrath_sound_timer_helpers_detail;
extern "C" void norrath_sound_timer_insert_alarm(void* incoming) {
    const Address node=address(incoming);
    const Address state=address(norrath_sound_timer_control_view);
    const Raw64 delay=load_doubleword(node+32u);
    const Raw64 start=load_doubleword(node+16u);
    const Raw64 accumulated=load_doubleword(node+24u);
    Address current=load_word(state+24u);
    const Raw64 deadline=(delay+start)-accumulated;
    Address previous=0u;
    while(current!=0u) {
        const Raw64 current_delay=load_doubleword(current+32u);
        const Raw64 current_start=load_doubleword(current+16u);
        const Raw64 current_accumulated=load_doubleword(current+24u);
        const Raw64 current_deadline=(current_delay+current_start)-current_accumulated;
        if(deadline<current_deadline)break;
        previous=current;
        current=load_word(previous);
    }
    store_word(node+4u,previous);
    store_word(node,current);
    if(current!=0u)store_word(current+4u,node);
    if(previous!=0u)store_word(previous,node);
    else store_word(state+24u,node);
}
