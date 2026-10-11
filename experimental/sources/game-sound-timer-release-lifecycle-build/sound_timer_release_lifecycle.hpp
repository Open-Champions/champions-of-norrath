#pragma once
#include "sound_time_conversion.hpp"
extern "C" {
unsigned long long norrath_sound_interrupt_stop_timer_counter(unsigned long long handle);
unsigned long long norrath_sound_stop_timer_counter(unsigned long long handle);
unsigned long long norrath_sound_interrupt_free_timer_counter(unsigned long long handle);
unsigned long long norrath_sound_free_timer_counter(unsigned long long handle);
unsigned long long norrath_sound_interrupt_release_timer_alarm(unsigned long long handle);
unsigned long long norrath_sound_release_timer_alarm(unsigned long long handle);
}
namespace norrath_sound_timer_release_lifecycle_detail {
using namespace norrath_sound_timer_delay_detail;
// Exact original SRL32/SLL32 pointer decoding only. These helpers make no
// allocation, alignment, capacity, pool-membership or mapped-address promise.
inline Address counter_address(Raw64 handle) {
    return (static_cast<unsigned>(handle)>>10)<<6;
}
inline Address alarm_address(Raw64 handle) {
    return (static_cast<unsigned>(handle)>>8)<<4;
}
}
