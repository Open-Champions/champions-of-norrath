#include "sound_timer_infrastructure.hpp"
using namespace norrath_sound_timer_infrastructure_detail;
extern "C" unsigned long long norrath_sound_interrupt_allocate_timer_counter() {
    const Address state=address(norrath_sound_timer_control_view);
    const Address counter=load_word(state+20u);
    if(counter==0u)return signed_word_bits(0x80008005u);

    const unsigned used=load_word(state+16u);
    const Raw64 address_part=signed_word_bits(counter<<4);
    const unsigned successor=load_word(counter);
    store_word(counter+40u,0u);
    store_word(state+16u,used+1u);
    store_word(counter+12u,0u);
    store_word(state+20u,successor);
    const unsigned serial=load_word(state+12u);
    store_doubleword(counter+24u,0ull);
    store_word(state+12u,serial+1u);
    const unsigned fresh_serial=load_word(state+12u);
    const unsigned tag=((fresh_serial<<1)&0x3feu)|1u;
    store_word(counter+8u,tag);
    const Raw64 fresh_tag=signed_word_bits(load_word(counter+8u));
    return address_part|fresh_tag;
}
