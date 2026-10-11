#pragma once
#include "sound_command_initialization.hpp"
extern "C" {
// Genuine isceSifSetDChain16, SYS CALL selector-120; this entire provider is
// still unfinished. The original dispatcher ignores its opaque result.
void norrath_sound_interrupt_set_dma_chain();
}
namespace norrath_sound_command_callbacks_detail {
using norrath_constructor_detail::Address;
using norrath_constructor_detail::address;
using norrath_constructor_detail::pointer;
using norrath_constructor_detail::signed_word;
using norrath_sound_native_register_detail::Raw64;
using norrath_sound_native_register_detail::current_gp_bits;
using norrath_sound_native_register_detail::install_gp_bits;
using norrath_sound_native_register_detail::signed_word_gp_bits;
static_assert(sizeof(Address)==4 && sizeof(Raw64)==8 &&
              sizeof(NorrathSoundCommandHandler)==4,"actual EE32 addresses/64 GP");
// Minimal real-width native loads/stores operate on their real input address.
// They also avoid interpreting a native-written byte workspace as C++ uint
// objects. No status, host memory answer, owner or kernel effect is supplied.
__attribute__((always_inline)) inline unsigned load_word(Address at) {
    unsigned value;
    asm volatile("lw %0,0(%1)":"=r"(value):"r"(at):"memory");
    return value;
}
__attribute__((always_inline)) inline void store_word(Address at,unsigned value) {
    asm volatile("sw %0,0(%1)"::"r"(value),"r"(at):"memory");
}
__attribute__((always_inline)) inline unsigned load_byte(Address at) {
    unsigned value;
    asm volatile("lbu %0,0(%1)":"=r"(value):"r"(at):"memory");
    return value;
}
__attribute__((always_inline)) inline void store_byte(Address at,unsigned value) {
    asm volatile("sb %0,0(%1)"::"r"(value),"r"(at):"memory");
}
// One native128-bit transfer, not a Raw64 replacement. SAME physical $8 value
// flows directly from LQ to SQ; both actual address operands are explicit.
// GCC's GPR clobber covers its tracked lane; native high128 lifetime/alignment
// and generated address/inline/stack order remain mandatory first-build gates.
// No loop, response, control flow, original opcode word or body is hidden here.
__attribute__((always_inline)) inline void transfer_quadword(Address from,Address to) {
    asm volatile("lq $8,0(%0)\n\tsq $8,0(%1)"
                 ::"r"(from),"r"(to):"$8","memory");
}
__attribute__((always_inline)) inline void finish_interrupt() {
    // These are the two real final architectural operations on every nonzero
    // packet path, even if no handler is called. They do not read a host flag.
    asm volatile("sync":::"memory");
    asm volatile("ei":::"memory");
}
inline NorrathSoundCommandHandler command_handler(unsigned word) {
    return reinterpret_cast<NorrathSoundCommandHandler>(
        static_cast<__UINTPTR_TYPE__>(word));
}
}
