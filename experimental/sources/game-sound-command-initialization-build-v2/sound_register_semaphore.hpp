#pragma once
#include "sound_rpc_binding.hpp"
#include "sound_kernel_services.hpp"
extern "C" {
// Same actual FlushCache identity and void interface used by Remote.
void norrath_sound_flush_cache(int mode);
// Exact LOCAL soft_reg8debc0 minimum word observation only. No array extent,
// initializer, whole indexed capacity or source storage is defined here.
extern unsigned char norrath_sound_system_register_words_view[];
}
namespace norrath_sound_register_semaphore_detail {
using Raw64=NorrathSoundKernelRaw64;
using norrath_constructor_detail::Address;
using norrath_constructor_detail::address;
using norrath_constructor_detail::signed_word;
// A single genuine native operation with fixed architectural selector/result
// and typed input operands. These helpers are always inline; the actual first
// compiler must inspect every selector, argument and native effect boundary.
// Possible call-volatile GPR/primary HI-LO changes are declared conservatively;
// saved-GPR, GP, high128, RA/SP, CP0, FPR/FCSR and foreign kernel behavior are
// NOT proved by these declarations. No host status is supplied.
template<class Input>
__attribute__((always_inline)) inline Raw64 one_input_service(int selector,Input value) {
    register Raw64 result asm("$2");
    register int operation asm("$3")=selector;
    register Input input asm("$4")=value;
    asm volatile("syscall" : "=r"(result),"+r"(operation),"+r"(input) :
                 : "$5","$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25",
                   "hi","lo","memory");
    return result;
}
__attribute__((always_inline)) inline Raw64 set_register_service(int number,unsigned value) {
    register Raw64 result asm("$2");
    register int operation asm("$3")=121;
    register int input0 asm("$4")=number;
    register unsigned input1 asm("$5")=value;
    asm volatile("syscall" : "=r"(result),"+r"(operation),
                 "+r"(input0),"+r"(input1) :
                 : "$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25",
                   "hi","lo","memory");
    return result;
}
}
