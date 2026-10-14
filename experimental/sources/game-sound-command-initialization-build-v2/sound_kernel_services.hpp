#pragma once

using NorrathSoundKernelRaw64 = unsigned long long;
static_assert(sizeof(NorrathSoundKernelRaw64)==8 && sizeof(void*)==4 &&
              sizeof(int)==4,"fixed EE native64 result and32-bit arguments");
extern "C" {
NorrathSoundKernelRaw64 norrath_sound_interrupt_signal_semaphore(int semaphore);
NorrathSoundKernelRaw64 norrath_sound_set_dma(void* descriptors,int count);
NorrathSoundKernelRaw64 norrath_sound_interrupt_set_dma(void* descriptors,int count);
}

namespace norrath_sound_kernel_services_detail {
// These are actual EE syscall boundaries: operation in physical V1, complete
// returned V0, and the ordinary32 argument registers. There is no host kernel
// response or original code address. The call-volatile integer registers and
// primary HI/LO are conservative possible effects, not a native clobber proof.
// Foreign kernel high/128-bit, saved-GPR, GP, CP0/FPR/FCSR, cache and interrupt
// behavior remains a required native-runtime boundary.
__attribute__((always_inline)) inline NorrathSoundKernelRaw64
signal_service(int semaphore) {
    register NorrathSoundKernelRaw64 result asm("$2");
    register int operation asm("$3")=-67;
    register int input asm("$4")=semaphore;
    asm volatile("syscall" : "=r"(result),"+r"(operation),"+r"(input) :
                 : "$5","$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25",
                   "hi","lo","memory");
    return result;
}
__attribute__((always_inline)) inline NorrathSoundKernelRaw64
dma_service(int number,void* descriptors,int count) {
    register NorrathSoundKernelRaw64 result asm("$2");
    register int operation asm("$3")=number;
    register void* input0 asm("$4")=descriptors;
    register int input1 asm("$5")=count;
    asm volatile("syscall" : "=r"(result),"+r"(operation),
                 "+r"(input0),"+r"(input1) :
                 : "$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25",
                   "hi","lo","memory");
    return result;
}
}
