#include "sound_timer_delay.hpp"
using namespace norrath_sound_timer_delay_detail;
extern "C" unsigned long long norrath_sound_allocate_timer_counter() {
    const int previous_enabled=norrath_sound_disable_interrupts();
    const Raw64 result=norrath_sound_interrupt_allocate_timer_counter();
    if(previous_enabled!=0)norrath_sound_enable_interrupts();
    return result;
}
