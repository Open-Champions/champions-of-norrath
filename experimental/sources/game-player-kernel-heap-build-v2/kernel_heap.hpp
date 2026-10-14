#pragma once
#include "allocator_support.hpp"
extern "C" {
NorrathFormatUnsigned64 norrath_player_heap_end_bits();
unsigned char* norrath_player_errno_pointer();
extern unsigned char norrath_player_heap_pointer_word[];
extern unsigned char norrath_factory_impure_pointer_word[];
}
namespace norrath_kernel_heap_detail {
using namespace norrath_allocator_support_detail;
// These are native EE instruction boundaries, not simulated kernel answers.
inline Word read_status() {
    Word status;
    asm volatile("mfc0 %0,$12" : "=r"(status) : : "memory");
    return status;
}
inline void disable_interrupts_and_synchronize() {
    asm volatile("di\n\tsync.p" : : : "memory");
}
inline void enable_interrupts() {
    asm volatile("ei" : : : "memory");
}
// EE syscall number is V1, result is the complete V0 and semaphore ID is A0.
// Declare integer call-volatile GPRs and primary HI/LO as possible trap effects.
// Compiler must preserve live heap/status values across the boundary.
// Actual trap behavior and callee-saved preservation require a native kernel;
// this is not a host substitute or a promise of native clobber equivalence.
inline Wide syscall_without_argument(Word number) {
    register Wide result asm("$2");
    register Word operation asm("$3")=number;
    asm volatile("syscall" : "=r"(result),"+r"(operation) :
                 : "$4","$5","$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25","hi","lo","memory");
    return result;
}
inline Wide syscall_with_argument(Word number,int argument) {
    register Wide result asm("$2");
    register Word operation asm("$3")=number;
    register int input asm("$4")=argument;
    asm volatile("syscall" : "=r"(result),"+r"(operation),"+r"(input) :
                 : "$5","$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25","hi","lo","memory");
    return result;
}
}
