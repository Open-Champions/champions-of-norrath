#include "sound_wake_timer.hpp"
using namespace norrath_sound_wake_timer_detail;
extern "C" unsigned long long norrath_sound_interrupt_set_timer_alarm(
    unsigned long long delay,NorrathSoundRpcRetryHandler handler,void* parameter) {
    if(handler==nullptr)return signed_word_bits(0x80000016u);
    const Address head=address(norrath_sound_timer_free_alarm_head_view);
    const Address alarm=read_word_fresh(head);
    if(alarm!=0u) {
        const unsigned successor=read_word_fresh(alarm);
        write_word_ordered(head,successor);
    }
    if(alarm==0u)return signed_word_bits(0x80008005u);

    const Raw64 counter=norrath_sound_interrupt_allocate_timer_counter();
    // Original BGEZL tests the whole allocator V0; no low32/success assumption.
    if(signed_negative(counter)) {
        const unsigned fresh_head=read_word_fresh(head);
        write_word_ordered(head,alarm);
        write_word_ordered(alarm,fresh_head);
        write_word_ordered(alarm+4u,0u);
        return counter;
    }

    write_word_ordered(alarm+8u,
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler)));
    write_word_ordered(alarm+12u,address(parameter));
    write_word_ordered(alarm+4u,unsigned(counter));
    norrath_sound_interrupt_set_timer_handler(
        counter,delay,&norrath_sound_timer_alarm_forward,pointer(alarm));
    norrath_sound_interrupt_start_timer_counter(counter);
    // Actual SLL word then OR/ANDI yields a signed32 synthetic alarm handle
    // transported in full64 V0. This is neither a host timer nor an allocation.
    const unsigned encoded=(alarm<<4)|(unsigned(counter)&0xfeu)|1u;
    return signed_word_bits(encoded);
}
