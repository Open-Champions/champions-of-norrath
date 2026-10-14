#pragma once
#include "exec_detail.hpp"
namespace norrath_exec_transport {
using norrath_exec_detail::Register;
using norrath_exec_detail::SignedRegister;
using norrath_exec_detail::Word;
using norrath_exec_detail::low;
using norrath_exec_detail::extend;
// Complete two native HI/LO banks; every member is an actual 64-bit lane.
// Entry assembly initializes these values from hardware, never a zero seed.
struct alignas(16) Special { Register hi0, lo0, hi1, lo1; };
inline void signed_divide(Special& state, Register numerator, Register denominator) {
    const Word a=low(numerator), b=low(denominator);
    const SignedRegister x=norrath_constructor_detail::signed_word(a);
    const SignedRegister y=norrath_constructor_detail::signed_word(b);
    if(a==0x80000000u && b==0xffffffffu) {
        state.lo0=extend(0x80000000u);
        state.hi0=0ull;
    } else if(b!=0u) {
        // Both operands are widened signed32; this signed64 division/remainder
        // cannot overflow, and its denominator is known nonzero.
        state.lo0=extend(static_cast<Word>(x/y));
        state.hi0=extend(static_cast<Word>(x%y));
    } else {
        state.lo0=x<0ll ? 1ull : extend(0xffffffffu);
        state.hi0=extend(a);
    }
    // HI1 and LO1 are untouched by original DIV.
}
inline void unsigned_divide(Special& state, Register numerator, Register denominator) {
    const Word a=low(numerator), b=low(denominator);
    if(b!=0u) {
        state.lo0=extend(a/b);
        state.hi0=extend(a%b);
    } else {
        state.lo0=extend(0xffffffffu);
        state.hi0=extend(a);
    }
}
inline Register signed_multiply(Special& state, Register left, Register right) {
    const SignedRegister x=norrath_constructor_detail::signed_word(low(left));
    const SignedRegister y=norrath_constructor_detail::signed_word(low(right));
    // The signed32 product fits signed64. Conversion to unsigned64 is defined
    // modulo 2^64, permitting a defined extraction of both result words.
    const Register product=static_cast<Register>(x*y);
    state.lo0=extend(static_cast<Word>(product));
    state.hi0=extend(static_cast<Word>(product>>32));
    return state.lo0; // Original MULT also writes Rd low64; its high64 survives.
}
}
