#pragma once
#include "exec_detail.hpp"
namespace norrath_exec_native_frame {
using Address = norrath_exec_detail::Address;
using Register = norrath_exec_detail::Register;
// These are explicitly typed physical reads. The asm output initializes the
// C++ value; no indeterminate C++ object, guessed seed, or helper result is read.
// memory clobbers keep aliasable C++ effects on the proper side of each access.
// Native validity/alignment/fault and final lowering remain separate proofs.
__attribute__((always_inline)) inline Register load_word(Address at) {
    Register value;
    asm volatile("lw %0,0(%1)" : "=r"(value) : "r"(at) : "memory");
    return value;
}
__attribute__((always_inline)) inline Register load_half(Address at) {
    Register value;
    asm volatile("lhu %0,0(%1)" : "=r"(value) : "r"(at) : "memory");
    return value;
}
__attribute__((always_inline)) inline Register load_byte(Address at) {
    Register value;
    asm volatile("lbu %0,0(%1)" : "=r"(value) : "r"(at) : "memory");
    return value;
}
__attribute__((always_inline)) inline void store_word(Address at, Register value) {
    asm volatile("sw %0,0(%1)" : : "r"(value), "r"(at) : "memory");
}
}
