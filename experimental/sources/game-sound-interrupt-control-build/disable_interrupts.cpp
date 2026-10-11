#include "sound_interrupt_control.hpp"
using namespace norrath_sound_interrupt_control_detail;
extern "C" int norrath_sound_disable_interrupts() {
    const Raw64 previous=read_status();
    if((previous&interrupt_enable_bit)==0)return 0;
    explicit_pause();
    Raw64 current;
    do {
        disable_interrupts();
        pipeline_sync();
        current=read_status();
        explicit_pause();
        explicit_pause();
    } while((current&interrupt_enable_bit)!=0);
    return 1;
}
