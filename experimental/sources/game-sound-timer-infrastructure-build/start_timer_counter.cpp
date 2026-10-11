#include "sound_timer_infrastructure.hpp"
using namespace norrath_sound_timer_infrastructure_detail;
extern "C" unsigned long long norrath_sound_interrupt_start_timer_counter(
    unsigned long long handle) {
    const Address counter=(unsigned(handle)>>10)<<6;
    if(signed_negative(handle))return signed_word_bits(0x80008002u);
    if((handle&0x3ffull)!=signed_word_bits(load_word(counter+8u)))
        return signed_word_bits(0x80008002u);
    const Address state=address(norrath_sound_timer_control_view);
    if(signed_word_bits(load_word(state+28u))==handle)
        return signed_word_bits(0x80000010u);
    if((load_word(counter+12u)&1u)!=0u)return 1ull;

    const Raw64 time=norrath_sound_interrupt_get_timer_system_time();
    const unsigned fresh_flags=load_word(counter+12u);
    store_doubleword(counter+16u,time);
    const unsigned running_flags=fresh_flags|1u;
    store_word(counter+12u,running_flags);
    if((running_flags&2u)!=0u) {
        norrath_sound_timer_insert_alarm(pointer(counter));
        norrath_sound_timer_set_next_comparison(time);
    }
    return 0ull;
}
