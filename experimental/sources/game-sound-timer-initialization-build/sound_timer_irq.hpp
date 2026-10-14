#pragma once
#include "sound_timer_register.hpp"
extern "C" {
// Actual private IRQ machine entry. No ordinary semantic argument is invented;
// real incoming physical A2 is an explicit foreign register input below.
int norrath_sound_timer_irq_callback();
}
namespace norrath_sound_timer_irq_detail {
using namespace norrath_sound_timer_register_detail;
using Raw64=unsigned long long;
static_assert(sizeof(Raw64)==8 && sizeof(void*)==4 &&
              sizeof(NorrathSoundRpcRetryHandler)==4,"actual EE64 slots/word32");
__attribute__((always_inline)) inline Raw64 incoming_irq_a2_bits() {
    Raw64 value;
    // There is no C++ uninitialized register variable or guessed kernel value.
    // Actual first code must prove this read precedes ANY clobber of entry A2.
    asm volatile("daddu %0,$6,$zero":"=r"(value)::"memory");
    return value;
}
__attribute__((always_inline)) inline void install_node_gp_bits(Raw64 value) {
    // Approved physical GP OUTPUT representation; not the old rejected clobber
    // setter and not a cross-call hard-register reservation or GP constant.
    register Raw64 physical_gp asm("$28");
    asm volatile("daddu %0,%1,$zero"
                 : "=&r"(physical_gp) : "r"(value) : "memory");
}
struct NativeCallbackResult {Raw64 value;Raw64 returned_gp;};
__attribute__((always_inline)) inline NativeCallbackResult invoke_timer_handler(
    NorrathSoundRpcRetryHandler function,Raw64 handle,Raw64 interval,
    Raw64 elapsed,void* parameter,Raw64 captured_irq_a2) {
    // The function's canonical type is FOUR slots. The extra operand is the
    // proven original physical T0 lane, not an invented fifth semantic argument.
    // Each bound register exists only as this ExtendedAsm operand.
    register Raw64 result asm("$2");
    register Raw64 slot_a0 asm("$4")=handle;
    register Raw64 slot_a1 asm("$5")=interval;
    register Raw64 slot_a2 asm("$6")=elapsed;
    register Raw64 slot_a3 asm("$7")=signed_word_bits(address(parameter));
    register Raw64 hidden_t0 asm("$8")=captured_irq_a2;
    register Raw64 physical_gp_output asm("$28");
    register Raw64 target asm("$25")=signed_word_bits(
        static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(function)));
    // This is ONLY the real indirect-call control operation and its explicit
    // delay NOP. All memory loads/algorithm/loops are readable outside it.
    // GP is a real physical OUTPUT; no incoming save or restore is supplied.
    // Status/HI1LO1/high128/IRQ/arbitrary callee saved-state effects are unknown.
    asm volatile(".set push\n\t.set noreorder\n\tjalr %7\n\tnop\n\t.set pop"
        : "=r"(result),"+r"(slot_a0),"+r"(slot_a1),"+r"(slot_a2),
          "+r"(slot_a3),"+r"(hidden_t0),"=&r"(physical_gp_output),"+r"(target)
        :
        : "$3","$9","$10","$11","$12","$13","$14","$15", "$24","$31",
          "$f0","$f1","$f2","$f3","$f4","$f5","$f6","$f7","$f8","$f9",
          "$f10","$f11","$f12","$f13","$f14","$f15","$f16","$f17","$f18","$f19",
          "hi","lo","memory");
    // These are actual outputs, not status defaults. Caller intentionally never
    // restores returned_gp: the original body leaves the callback GP installed.
    return NativeCallbackResult{result,physical_gp_output};
}
__attribute__((always_inline)) inline void memory_sync() {
    asm volatile("sync":::"memory");
}
__attribute__((always_inline)) inline void enable_native_interrupts() {
    asm volatile("ei":::"memory");
}
}
