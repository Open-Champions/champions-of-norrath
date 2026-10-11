#include "sound_timer_release_lifecycle.hpp"
using namespace norrath_sound_timer_release_lifecycle_detail;
extern "C" unsigned long long norrath_sound_interrupt_free_timer_counter(Raw64 handle) {
    const Address counter=counter_address(handle);
    if(signed_negative(handle))return signed_word_bits(0x80008002u);
    const Raw64 tag=signed_word_bits(load_word(counter+8u));
    if((handle&0x3ffull)!=tag)return signed_word_bits(0x80008002u);
    const Address state=address(norrath_sound_timer_control_view);
    const Raw64 current=signed_word_bits(load_word(state+28u));
    if(current==handle)return signed_word_bits(0x80000010u);
    const unsigned flags=load_word(counter+12u);
    if((flags&2u)!=0u)norrath_sound_timer_unlink_alarm(pointer(counter));
    store_word(counter+8u,0u);
    store_word(counter+12u,0u);
    const unsigned fresh_head=load_word(state+20u);
    const unsigned fresh_count=load_word(state+16u);
    store_word(counter,fresh_head);
    const unsigned decremented_count=fresh_count-1u;
    store_word(state+20u,counter);
    store_word(state+16u,decremented_count);
    return 0ull;
}
