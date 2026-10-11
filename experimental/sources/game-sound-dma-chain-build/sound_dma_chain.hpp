#pragma once
#include "sound_kernel_services.hpp"

extern "C" void norrath_sound_interrupt_set_dma_chain();

namespace norrath_sound_dma_chain_detail {
// The real no-argument EE operation uses V1=-120 and an opaque V0 result.
// Its existing dispatcher has a void interface and ignores that result.
// The compiler sees possible volatile-register and memory effects; this
// does not establish foreign kernel clobbers, high128, GP or IRQ behavior.
__attribute__((always_inline)) inline void start_chain_service() {
    register NorrathSoundKernelRaw64 result asm("$2");
    register int operation asm("$3")=-120;
    asm volatile("syscall" : "=r"(result),"+r"(operation) :
                 : "$4","$5","$6","$7","$8","$9","$10","$11",
                   "$12","$13","$14","$15","$24","$25",
                   "hi","lo","memory");
}
}
