#pragma once
#include "constructor.hpp"

namespace norrath_sound_native_register_detail {
using Raw64 = unsigned long long;
static_assert(sizeof(Raw64)==8 && sizeof(__builtin_va_list)==4,
              "reviewed EE single-float EABI64 GPR slots and flat cursor");

// Minimal typed operations on the actual live64-bit GP register. These are
// always inline: an adapter call must not become another register/lifetime edge.
__attribute__((always_inline)) inline Raw64 current_gp_bits() {
    Raw64 value;
    asm volatile("daddu %0,$28,$zero":"=r"(value)::"memory");
    return value;
}
__attribute__((always_inline)) inline void install_gp_bits(Raw64 value) {
    // GCC supports local hard-register variables only as ExtendedAsm operands.
    // This variable exists solely to name the actual written GP output; it is
    // not a reservation, cross-call saved value, or replacement GP constant.
    register Raw64 physical_gp asm("$28");
    asm volatile("daddu %0,%1,$zero"
                 : "=&r"(physical_gp) : "r"(value) : "memory");
}
inline Raw64 signed_word_gp_bits(unsigned value) {
    return Raw64(value)|((value&0x80000000u)?0xffffffff00000000ull:0ull);
}

struct SixSavedArgumentRegisters { Raw64 lane[6]; };
// This operates on the physical register-save area supplied by the selected
// native compiler's va_start prologue. It is NOT va_arg traversal, nor a claim
// that missing C++ arguments exist. Every full64-bit lane is read, including
// unprovided lanes. No lane is seeded or converted to a host answer.
//
// Required first-build proof: six native SD A2..T3 spills occur before those
// entry registers are clobbered; cursor addresses that aligned48-byte area;
// all six LDs below occur before any function call or packet store. If not
// established, the variadic entry remains a material native-ABI gap.
__attribute__((always_inline)) inline SixSavedArgumentRegisters
capture_saved_argument_registers(const unsigned char* cursor) {
    SixSavedArgumentRegisters result;
    asm volatile("ld %0,0(%6)\n\tld %1,8(%6)\n\tld %2,16(%6)\n\t"
                 "ld %3,24(%6)\n\tld %4,32(%6)\n\tld %5,40(%6)"
                 :"=&r"(result.lane[0]),"=&r"(result.lane[1]),
                  "=&r"(result.lane[2]),"=&r"(result.lane[3]),
                  "=&r"(result.lane[4]),"=&r"(result.lane[5])
                 :"r"(cursor):"memory");
    return result;
}
}
