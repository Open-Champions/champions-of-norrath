#include "sound_interrupt_control.hpp"
using namespace norrath_sound_interrupt_control_detail;
extern "C" int norrath_sound_enable_interrupts() {
    const Raw64 previous=read_status();
    enable_interrupts();
    return (previous&interrupt_enable_bit)!=0 ? 1 : 0;
}
