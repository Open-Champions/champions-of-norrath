#pragma once
#include "sound_wake_timer.hpp"
extern "C" {
// Entire actual helper identities remain unfinished source dependencies, with
// real native transport. No software clock, linked original address or answer.
unsigned long long norrath_sound_timer_unlink_alarm(void* counter);
void norrath_sound_timer_insert_alarm(void* counter);
unsigned long long norrath_sound_interrupt_get_timer_system_time();
void norrath_sound_timer_set_next_comparison(unsigned long long time);
// Actual LOCAL OBJECT g_Timer32; external observation only, no source owner.
extern unsigned char norrath_sound_timer_control_view[];
}
namespace norrath_sound_timer_infrastructure_detail {
using namespace norrath_sound_wake_timer_detail;
using norrath_sound_native_register_detail::current_gp_bits;
static_assert(sizeof(Address)==4 && sizeof(Raw64)==8,
              "actual EE32 address and full64 timer lanes");
// Individual actual-width native operations on their true address operands.
// These do not manufacture C++ object layout, status, atomicity or alignment.
__attribute__((always_inline)) inline unsigned load_word(Address at) {
    unsigned value;
    asm volatile("lw %0,0(%1)":"=r"(value):"r"(at):"memory");
    return value;
}
__attribute__((always_inline)) inline void store_word(Address at,unsigned value) {
    asm volatile("sw %0,0(%1)"::"r"(value),"r"(at):"memory");
}
__attribute__((always_inline)) inline Raw64 load_doubleword(Address at) {
    Raw64 value;
    asm volatile("ld %0,0(%1)":"=r"(value):"r"(at):"memory");
    return value;
}
__attribute__((always_inline)) inline void store_doubleword(Address at,Raw64 value) {
    asm volatile("sd %0,0(%1)"::"r"(value),"r"(at):"memory");
}
inline unsigned code_word(NorrathSoundRpcRetryHandler handler) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler));
}
}
