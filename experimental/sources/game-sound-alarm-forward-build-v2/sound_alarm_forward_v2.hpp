#pragma once
#include "sound_timer_irq.hpp"
namespace norrath_sound_alarm_forward_v2_detail {
using namespace norrath_sound_timer_irq_detail;
__attribute__((always_inline)) inline Raw64 incoming_alarm_t0_bits() {
    Raw64 value;
    // Real physical entryT0, not a C++ formal parameter/uninitialized variable
    // or a fabricated IRQ value. First code MUST capture before anyT0 clobber.
    asm volatile("daddu %0,$8,$zero":"=r"(value)::"memory");
    return value;
}
}
