#pragma once
#include "sound_timer_helpers.hpp"
extern "C" {
// Genuine complete SetT2 core: two full physical64 inputs. The native SW
// consumes low32 of value, while the register address retains sign extension.
void norrath_sound_timer_write_register(unsigned long long native_address,
                                        unsigned long long raw_word);
}
namespace norrath_sound_timer_register_detail {
using namespace norrath_sound_timer_helpers_detail;
constexpr Raw64 interrupt_enable_bit=0x10000ull;
constexpr Raw64 native_write_mode_address=0xffffffffb0001010ull;
constexpr Raw64 native_write_compare_address=0xffffffffb0001020ull;
// Each primitive is one real architectural instruction; no hidden body/loop,
// host status/exception response, original code address or owned device state.
__attribute__((always_inline)) inline Raw64 read_status() {
    Raw64 value;
    asm volatile("mfc0 %0,$12":"=r"(value)::"memory");
    return value;
}
__attribute__((always_inline)) inline void disable_interrupts() {
    asm volatile("di":::"memory");
}
__attribute__((always_inline)) inline void pipeline_sync() {
    asm volatile("sync.p":::"memory");
}
__attribute__((always_inline)) inline void explicit_pause() {
    asm volatile("nop":::"memory");
}
__attribute__((always_inline)) inline void write_status(Raw64 value) {
    asm volatile("mtc0 %0,$12"::"r"(value):"memory");
}
__attribute__((always_inline)) inline void store_native_word(Raw64 at,Raw64 value) {
    asm volatile("sw %0,0(%1)"::"r"(value),"r"(at):"memory");
}
__attribute__((always_inline)) inline Raw64 live_return_register_bits() {
    Raw64 value;
    asm volatile("daddu %0,$31,$zero":"=r"(value)::"memory");
    return value;
}
__attribute__((always_inline)) inline void write_error_epc(Raw64 value) {
    asm volatile("mtc0 %0,$30"::"r"(value):"memory");
}
__attribute__((always_inline)) inline void exception_return() {
    // Real ERET transfers control through architectural ErrorEPC/status. GCC
    // does not model this exit in C++ CFG. This candidate is NOT safe to admit
    // unless its actual whole first object is frame0/noSP/noRAsave/nohelpers.
    // No subsequent C++ effect or synthetic exception result is supplied.
    asm volatile("eret":::"memory");
}
}
