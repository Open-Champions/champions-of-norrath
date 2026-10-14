#include "sound_alarm_forward_v2.hpp"
using namespace norrath_sound_alarm_forward_v2_detail;
extern "C" unsigned long long norrath_sound_timer_alarm_forward(
    unsigned long long handle,unsigned long long interval,
    unsigned long long elapsed,void* alarm_parameter) {
    const Raw64 entry_t0=incoming_alarm_t0_bits();
    const Address alarm=address(alarm_parameter);
    const unsigned encoded=(alarm<<4)|(unsigned(handle)&0xfeu)|1u;
    const unsigned callback_word=load_word(alarm+8u);
    const unsigned argument_word=load_word(alarm+12u);
    // The unchanged native call operation has no GP save/install/restore. Its
    // last physical operand is real entryT0 here; the inherited parameter name
    // captured_irq_a2 is provenance for the IRQ caller, not a semantic fiftharg.
    const NativeCallbackResult call=invoke_timer_handler(
        handler_at(callback_word),signed_word_bits(encoded),interval,elapsed,
        pointer(argument_word),entry_t0);
    const Raw64 result=call.value;
    if(result!=0ull)return result;

    const Address head=address(norrath_sound_timer_free_alarm_head_view);
    const Address fresh_head=load_word(head);
    store_word(head,alarm);
    store_word(alarm,fresh_head);
    store_word(alarm+4u,0u);
    return ~Raw64(0);
}
