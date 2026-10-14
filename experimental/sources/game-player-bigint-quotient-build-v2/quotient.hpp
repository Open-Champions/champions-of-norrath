#pragma once
#include "number_parser.hpp"
extern "C" int norrath_player_bigint_quotient(unsigned char*,unsigned char*);
namespace norrath_bigint_quotient_detail {
using namespace norrath_player_number_detail;
inline Word arithmetic_right16(Word value){return(value>>16)|(value&0x80000000u?0xffff0000u:0u);}
// Genuine primary HI/LO DIVU, zero-divisor BREAK0,7, then MFLO.
// No memory access, software response, raw instruction word, or host division.
inline Word native_divide_or_break(Word numerator,Word denominator) {
    Word quotient;
    asm volatile(
        ".set push\n\t.set noreorder\n\t"
        "divu $0,%1,%2\n\t"
        "bnez %2,1f\n\tnop\n\t"
        "break 0,7\n"
        "1:\n\tmflo %0\n\t.set pop"
        : "=&r"(quotient) : "r"(numerator),"r"(denominator) : "hi","lo");
    return quotient;
}
}
