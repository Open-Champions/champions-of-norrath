#pragma once
#include "sound_timer_irq.hpp"
#include "block_alloc.hpp"
extern "C" {
// Full physical64 interfaces derived from these exact originals. Registration
// transports a real codeword; it does not invent an IRQ callback signature.
unsigned long long norrath_sound_timer_initialize(unsigned long long mode);
unsigned long long norrath_sound_timer_initialize_alarm_list();
unsigned long long norrath_sound_add_interrupt_handler2(
    unsigned long long line,unsigned long long callback_codeword,
    unsigned long long priority,unsigned long long parameter);
unsigned long long norrath_sound_enable_interrupt_line(unsigned long long line);
unsigned long long norrath_sound_enable_interrupt_line_native(unsigned long long line);
void norrath_sound_timer_set_count_register(unsigned long long value);
// Exact LOCAL NOTYPE zero-sized originals. Observed writes do not own storage.
extern unsigned char norrath_sound_timer_counter_buffer_view[];
extern unsigned char norrath_sound_timer_alarm_buffer_view[];
}
namespace norrath_sound_timer_initialization_detail {
using namespace norrath_sound_timer_irq_detail;
constexpr Raw64 native_write_count_address=0xffffffffb0001000ull;
__attribute__((always_inline)) inline void ordinary_sync() {
    asm volatile("sync":::"memory");
}
__attribute__((always_inline)) inline Raw64 register_interrupt_service(
    Raw64 line,Raw64 callback_codeword,Raw64 priority,Raw64 parameter) {
    register Raw64 result asm("$2");
    register int operation asm("$3")=16;
    register Raw64 input0 asm("$4")=line;
    register Raw64 input1 asm("$5")=callback_codeword;
    register Raw64 input2 asm("$6")=priority;
    register Raw64 input3 asm("$7")=parameter;
    asm volatile("syscall":"=r"(result),"+r"(operation),"+r"(input0),
                 "+r"(input1),"+r"(input2),"+r"(input3)::
                 "$8","$9","$10","$11","$12","$13","$14",
                 "$15","$24","$25","hi","lo","memory");
    return result;
}
__attribute__((always_inline)) inline Raw64 enable_interrupt_service(Raw64 line) {
    register Raw64 result asm("$2");
    register int operation asm("$3")=20;
    register Raw64 input asm("$4")=line;
    asm volatile("syscall":"=r"(result),"+r"(operation),"+r"(input)::
                 "$5","$6","$7","$8","$9","$10","$11",
                 "$12","$13","$14","$15","$24","$25",
                 "hi","lo","memory");
    return result;
}
// Genuine SYSCALL input/output lanes only. Foreign savedGPR/RA/SP/GP/high128,
// CP0/FPR/FCSR/HI1LO1/IRQ effects are not supplied by these declarations.
}
