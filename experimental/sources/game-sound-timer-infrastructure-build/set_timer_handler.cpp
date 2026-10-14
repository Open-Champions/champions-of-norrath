#include "sound_timer_infrastructure.hpp"
using namespace norrath_sound_timer_infrastructure_detail;
extern "C" unsigned long long norrath_sound_interrupt_set_timer_handler(
    unsigned long long handle,unsigned long long delay,
    NorrathSoundRpcRetryHandler handler,void* parameter) {
    const Address counter=(unsigned(handle)>>10)<<6;
    if(signed_negative(handle))return signed_word_bits(0x80008002u);
    if((handle&0x3ffull)!=signed_word_bits(load_word(counter+8u)))
        return signed_word_bits(0x80008002u);
    const Address state=address(norrath_sound_timer_control_view);
    if(signed_word_bits(load_word(state+28u))==handle)
        return signed_word_bits(0x80000010u);

    unsigned flags=load_word(counter+12u);
    if((flags&2u)!=0u) {
        norrath_sound_timer_unlink_alarm(pointer(counter));
        flags=load_word(counter+12u);
    }
    store_word(counter+40u,code_word(handler));
    if(handler==nullptr) {
        store_word(counter+12u,flags&~2u);
    } else {
        store_doubleword(counter+32u,delay);
        store_word(counter+12u,flags|2u);
        const Raw64 live_gp=current_gp_bits();
        store_word(counter+44u,unsigned(live_gp));
        const unsigned fresh_flags=load_word(counter+12u);
        // Original BEQ delay writes parameter48 on BOTH freshflag paths.
        store_word(counter+48u,address(parameter));
        if((fresh_flags&1u)!=0u)
            norrath_sound_timer_insert_alarm(pointer(counter));
    }
    const Raw64 time=norrath_sound_interrupt_get_timer_system_time();
    norrath_sound_timer_set_next_comparison(time);
    return 0ull;
}
