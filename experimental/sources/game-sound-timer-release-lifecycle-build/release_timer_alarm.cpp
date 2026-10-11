#include "sound_timer_release_lifecycle.hpp"
using namespace norrath_sound_timer_release_lifecycle_detail;
extern "C" unsigned long long norrath_sound_release_timer_alarm(Raw64 handle) {
    const Address alarm=alarm_address(handle);
    const int previous_enabled=norrath_sound_disable_interrupts();
    if(signed_negative(handle)) {
        if(previous_enabled!=0)norrath_sound_enable_interrupts();
        return signed_word_bits(0x80008002u);
    }
    const unsigned validation_counter=load_word(alarm+4u);
    if(((static_cast<unsigned>(handle)^validation_counter)&0xffu)!=0u) {
        if(previous_enabled!=0)norrath_sound_enable_interrupts();
        return signed_word_bits(0x80008002u);
    }
    const Raw64 counter=signed_word_bits(load_word(alarm+4u));
    // Ordinary original deliberately ignores this actual raw result and still
    // releases the alarm record; do not borrow interrupt-release behavior.
    static_cast<void>(norrath_sound_free_timer_counter(counter));
    const Address head=address(norrath_sound_timer_free_alarm_head_view);
    const unsigned fresh_head=load_word(head);
    store_word(head,alarm);
    store_word(alarm,fresh_head);
    store_word(alarm+4u,0u);
    if(previous_enabled!=0)norrath_sound_enable_interrupts();
    return 0ull;
}
