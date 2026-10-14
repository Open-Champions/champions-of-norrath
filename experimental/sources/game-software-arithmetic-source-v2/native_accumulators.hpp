#pragma once
#include "software_arithmetic.hpp"
namespace norrath_software_arithmetic {
struct Accumulators { Bits hi0, lo0, hi1, lo1; };
// This is native transport, not the arithmetic body. Read/write operands tie
// subsequent arithmetic to the capture, even across GCC volatile-asm motion.
__attribute__((always_inline)) inline Accumulators capture(Bits& left, Bits& right) {
    Accumulators state;
    __asm__ volatile (
        "mfhi %0\n\tmflo %1\n\tmfhi1 %2\n\tmflo1 %3"
        : "=&r"(state.hi0), "=&r"(state.lo0),
          "=&r"(state.hi1), "=&r"(state.lo1), "+r"(left), "+r"(right)
        : : "hi", "lo", "memory");
    return state;
}
// Result is a read/write dependency: its full calculation must precede the
// commit. FIRST inspection must establish that the epilogue cannot clobber it.
__attribute__((always_inline)) inline Bits commit(Accumulators state, Bits result) {
    __asm__ volatile (
        "mthi %1\n\tmtlo %2\n\tmthi1 %3\n\tmtlo1 %4"
        : "+r"(result)
        : "r"(state.hi0), "r"(state.lo0), "r"(state.hi1), "r"(state.lo1)
        : "hi", "lo", "memory");
    return result;
}
// Original zero-denominator paths reach BREAK 0,7 before any MDU operation.
// Incoming banks are restored before that instruction. This does not promise
// the original fault PC, delay-slot BD bit, live GPRs or physical stack frame.
[[noreturn]] __attribute__((always_inline)) inline void zero_denominator(Accumulators state) {
    __asm__ volatile (
        "mthi %0\n\tmtlo %1\n\tmthi1 %2\n\tmtlo1 %3\n\t"
        ".set push\n\t.set noreorder\n\tbreak 0,7\n\t.set pop"
        : : "r"(state.hi0), "r"(state.lo0), "r"(state.hi1), "r"(state.lo1)
        : "hi", "lo", "memory");
    __builtin_unreachable();
}
}
