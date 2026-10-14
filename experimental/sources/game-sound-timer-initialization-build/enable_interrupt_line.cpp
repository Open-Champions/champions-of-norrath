#include "sound_timer_initialization.hpp"
using namespace norrath_sound_timer_initialization_detail;
extern "C" Raw64 norrath_sound_enable_interrupt_line(Raw64 line) {
    const Raw64 prior_enabled=read_status()&0x10000ull;
    if(prior_enabled!=0ull)norrath_sound_disable_interrupts();
    const Raw64 result=norrath_sound_enable_interrupt_line_native(line);
    ordinary_sync();
    if(prior_enabled!=0ull)norrath_sound_enable_interrupts();
    return result;
}
