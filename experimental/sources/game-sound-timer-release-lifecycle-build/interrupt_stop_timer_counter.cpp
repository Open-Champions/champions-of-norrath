#include "sound_timer_release_lifecycle.hpp"
using namespace norrath_sound_timer_release_lifecycle_detail;
extern "C" unsigned long long norrath_sound_interrupt_stop_timer_counter(Raw64 handle) {
    const Address counter=counter_address(handle);
    if(signed_negative(handle))return signed_word_bits(0x80008002u);
    const Raw64 tag=signed_word_bits(load_word(counter+8u));
    if((handle&0x3ffull)!=tag)return signed_word_bits(0x80008002u);
    const Address state=address(norrath_sound_timer_control_view);
    const Raw64 current=signed_word_bits(load_word(state+28u));
    if(current==handle)return signed_word_bits(0x80000010u);
    const unsigned flags=load_word(counter+12u);
    if((flags&1u)==0u)return 0ull;
    const Raw64 now=norrath_sound_interrupt_get_timer_system_time();
    const unsigned fresh_flags=load_word(counter+12u);
    const Raw64 start=load_doubleword(counter+16u);
    const Raw64 accumulated=load_doubleword(counter+24u);
    const Raw64 stopped_accumulated=accumulated+(now-start);
    store_doubleword(counter+24u,stopped_accumulated);
    store_word(counter+12u,fresh_flags&0xfffffffeu);
    if((fresh_flags&2u)!=0u) {
        norrath_sound_timer_unlink_alarm(pointer(counter));
        norrath_sound_timer_set_next_comparison(now);
    }
    return 1ull;
}
