#include "sound_timer_release_lifecycle.hpp"
using namespace norrath_sound_timer_release_lifecycle_detail;
extern "C" unsigned long long norrath_sound_interrupt_release_timer_alarm(Raw64 handle) {
    const Address alarm=alarm_address(handle);
    if(signed_negative(handle))return signed_word_bits(0x80008002u);
    const unsigned validation_counter=load_word(alarm+4u);
    if(((static_cast<unsigned>(handle)^validation_counter)&0xffu)!=0u)
        return signed_word_bits(0x80008002u);
    // Genuine second fresh field read, distinct from validation.
    const Raw64 counter=signed_word_bits(load_word(alarm+4u));
    const Raw64 result=norrath_sound_interrupt_free_timer_counter(counter);
    if(result!=0ull)return result;
    const Address head=address(norrath_sound_timer_free_alarm_head_view);
    const unsigned fresh_head=load_word(head);
    store_word(head,alarm);
    store_word(alarm,fresh_head);
    store_word(alarm+4u,0u);
    return result;
}
