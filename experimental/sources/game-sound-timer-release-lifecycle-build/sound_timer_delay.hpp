#pragma once
#include "sound_timer_helpers.hpp"
#include "sound_interrupt_control.hpp"
extern "C" {
// Ordinary counterparts are genuine distinct whole original dependencies,
// not aliases for their interrupt variants or original code addresses.
unsigned long long norrath_sound_allocate_timer_counter();
unsigned long long norrath_sound_set_timer_handler(
    unsigned long long handle,unsigned long long delay,
    NorrathSoundRpcRetryHandler handler,void* parameter);
unsigned long long norrath_sound_start_timer_counter(unsigned long long handle);
unsigned long long norrath_sound_timer_usec_to_bus_clock(
    unsigned long long seconds,unsigned long long microseconds);
unsigned long long norrath_sound_set_timer_alarm(
    unsigned long long delay,NorrathSoundRpcRetryHandler handler,void* parameter);
unsigned long long norrath_sound_delay_thread(unsigned long long microseconds);
unsigned long long norrath_sound_delay_thread_handler(
    unsigned long long handle,unsigned long long interval,
    unsigned long long elapsed,void* semaphore_parameter);
// Actual rodata address observation at47b688, not a source string or owner.
extern unsigned char norrath_sound_delay_thread_name_view[];
}
namespace norrath_sound_timer_delay_detail {
using namespace norrath_sound_timer_helpers_detail;
// Individual native operations reuse reviewed typed primitives; no supplied
// status, frame initialization or replacement service is introduced.
using norrath_sound_interrupt_control_detail::read_status;
constexpr Raw64 interrupt_enable_bit=0x10000ull;
__attribute__((always_inline)) inline void completion_sync() {
    asm volatile("sync":::"memory");
}
__attribute__((always_inline)) inline void completion_enable_interrupts() {
    asm volatile("ei":::"memory");
}
}
