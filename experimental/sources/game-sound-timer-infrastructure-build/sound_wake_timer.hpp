#pragma once
#include "sound_rpc_callbacks.hpp"
#include "sound_kernel_services.hpp"

extern "C" {
// Same established public machine interfaces; no synthetic status or timer.
unsigned long long norrath_sound_interrupt_wake_thread_fallback(
    unsigned long long physical_argument_a0);
unsigned long long norrath_sound_interrupt_allocate_timer_counter();
unsigned long long norrath_sound_interrupt_set_timer_handler(
    unsigned long long handle,unsigned long long delay,
    NorrathSoundRpcRetryHandler handler,void* parameter);
unsigned long long norrath_sound_interrupt_start_timer_counter(
    unsigned long long handle);
// Exact genuine private LOCAL AlarmHandler92, with the existing four-slot
// transport. A1 interval and A2 elapsed are passed through to the real handler.
unsigned long long norrath_sound_timer_alarm_forward(
    unsigned long long handle,unsigned long long interval,
    unsigned long long elapsed,void* alarm_parameter);

// Actual observation views only. No backing word, array, capacity or zero seed.
extern unsigned char norrath_sound_wake_top_id_word_view[];
extern unsigned char norrath_sound_wake_semaphore_word_view[];
extern unsigned char norrath_sound_wake_ring_view[];
extern unsigned char norrath_sound_timer_free_alarm_head_view[];
}

namespace norrath_sound_wake_timer_detail {
using namespace norrath_sound_rpc_callbacks_detail;
using Raw64=unsigned long long;
static_assert(sizeof(Raw64)==8 && sizeof(NorrathSoundRpcRetryHandler)==4 &&
              sizeof(void*)==4 && sizeof(int)==4,"actual EE64 lanes/word32");

inline Raw64 signed_word_bits(unsigned word) {
    return Raw64(word)|((word&0x80000000u)!=0u?0xffffffff00000000ull:0ull);
}
inline bool signed_negative(Raw64 bits) {return (bits>>63)!=0u;}
inline NorrathSoundRpcRetryHandler handler_at(Address a) {
    return reinterpret_cast<NorrathSoundRpcRetryHandler>(
        static_cast<__UINTPTR_TYPE__>(a));
}

struct WakeKernelResult {Raw64 status;Raw64 post_argument_a0;};
// This single actual SYSCALL has TWO observed outputs. In particular A0 is a
// read/write raw64 physical register; its post-kernel value is not the int
// supplied to the original call. There is no guessed preservation or answer.
// The fixed local registers are scoped to this operation, not across calls.
// Foreign high128/saved-GPR/GP/CP0/FPR/FCSR/HI1LO1/IRQ effects remain unproved.
__attribute__((always_inline)) inline WakeKernelResult
wake_service(int selector,Raw64 physical_argument_a0) {
    register Raw64 status asm("$2");
    register int operation asm("$3")=selector;
    register Raw64 argument asm("$4")=physical_argument_a0;
    asm volatile("syscall" : "=r"(status),"+r"(operation),"+r"(argument) :
                 : "$5","$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25",
                   "hi","lo","memory");
    return WakeKernelResult{status,argument};
}
}
