#include "sound_timer_delay.hpp"
using namespace norrath_sound_timer_delay_detail;
extern "C" unsigned long long norrath_sound_set_timer_handler(
    unsigned long long handle,unsigned long long delay,
    NorrathSoundRpcRetryHandler handler,void* parameter) {
    // All four genuine incoming values are captured before the first foreign
    // operation, as the original saved-register writes do. No missing slot,
    // default status, GP value or callback reply is manufactured.
    const Raw64 captured_handle=handle;
    const Raw64 captured_delay=delay;
    const NorrathSoundRpcRetryHandler captured_handler=handler;
    void* const captured_parameter=parameter;
    const int previous_enabled=norrath_sound_disable_interrupts();
    const Raw64 result=norrath_sound_interrupt_set_timer_handler(
        captured_handle,captured_delay,captured_handler,captured_parameter);
    if(previous_enabled!=0)norrath_sound_enable_interrupts();
    return result;
}
