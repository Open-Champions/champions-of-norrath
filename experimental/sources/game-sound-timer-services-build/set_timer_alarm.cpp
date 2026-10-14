#include "sound_timer_delay.hpp"
using namespace norrath_sound_timer_delay_detail;
extern "C" unsigned long long norrath_sound_set_timer_alarm(
    unsigned long long delay,NorrathSoundRpcRetryHandler handler,void* parameter) {
    if(handler==nullptr)return signed_word_bits(0x80000016u);
    const int previous_enabled=norrath_sound_disable_interrupts();
    const Address head=address(norrath_sound_timer_free_alarm_head_view);
    const Address alarm=load_word(head);
    if(alarm!=0u) {
        const unsigned successor=load_word(alarm);
        store_word(head,successor);
    }
    if(alarm==0u) {
        if(previous_enabled!=0)norrath_sound_enable_interrupts();
        return signed_word_bits(0x80008005u);
    }
    const Raw64 counter=norrath_sound_allocate_timer_counter();
    if(signed_negative(counter)) {
        const unsigned fresh_head=load_word(head);
        store_word(head,alarm);
        store_word(alarm,fresh_head);
        store_word(alarm+4u,0u);
        if(previous_enabled!=0)norrath_sound_enable_interrupts();
        return counter;
    }
    // Genuine BGEZL publishes callback only on allocator nonnegative path.
    store_word(alarm+8u,code_word(handler));
    store_word(alarm+12u,address(parameter));
    store_word(alarm+4u,unsigned(counter));
    norrath_sound_set_timer_handler(
        counter,delay,&norrath_sound_timer_alarm_forward,pointer(alarm));
    norrath_sound_start_timer_counter(counter);
    // The original SLL4 also executes in this branch delay before EIntr,
    // and is recomputed afterward on the enabled path. It has no memory or
    // native callback effect; physical placement is a first-object boundary.
    const unsigned shifted_alarm=alarm<<4;
    if(previous_enabled!=0)norrath_sound_enable_interrupts();
    return signed_word_bits(shifted_alarm|(unsigned(counter)&0xfeu)|1u);
}
