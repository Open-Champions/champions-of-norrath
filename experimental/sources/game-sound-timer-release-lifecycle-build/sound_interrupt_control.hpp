#pragma once
#include "sound_rpc.hpp"
namespace norrath_sound_interrupt_control_detail {
using Raw64=unsigned long long;
static_assert(sizeof(Raw64)==8 && sizeof(int)==4,"reviewed EE GPR64/word32");
// These are individual architectural operations on the actual CPU. They do
// not supply a host status value or redirect to an original function address.
__attribute__((always_inline)) inline Raw64 read_status() {
    Raw64 status;
    asm volatile("mfc0 %0,$12" : "=r"(status) : : "memory");
    return status;
}
__attribute__((always_inline)) inline void disable_interrupts() {
    asm volatile("di" : : : "memory");
}
__attribute__((always_inline)) inline void enable_interrupts() {
    asm volatile("ei" : : : "memory");
}
__attribute__((always_inline)) inline void pipeline_sync() {
    asm volatile("sync.p" : : : "memory");
}
__attribute__((always_inline)) inline void explicit_pause() {
    asm volatile("nop" : : : "memory");
}
constexpr Raw64 interrupt_enable_bit=0x10000ull;
}
