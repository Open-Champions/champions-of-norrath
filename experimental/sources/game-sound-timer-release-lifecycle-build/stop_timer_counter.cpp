#include "sound_timer_release_lifecycle.hpp"
using namespace norrath_sound_timer_release_lifecycle_detail;
extern "C" unsigned long long norrath_sound_stop_timer_counter(Raw64 handle) {
    const Raw64 captured_handle=handle;
    const int previous_enabled=norrath_sound_disable_interrupts();
    const Raw64 result=norrath_sound_interrupt_stop_timer_counter(captured_handle);
    if(previous_enabled!=0)norrath_sound_enable_interrupts();
    return result;
}
