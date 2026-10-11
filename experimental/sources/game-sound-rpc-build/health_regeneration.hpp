#pragma once
#include "skill_progression.hpp"
extern "C" {
void norrath_player_regenerate_health(void* actor);
extern const unsigned char norrath_player_health_coefficients_view[];
}
namespace norrath_health_regeneration_detail {
using namespace norrath_skill_progression_detail;
inline float float_from_word(unsigned word) {
    float value;
    unsigned char* p=reinterpret_cast<unsigned char*>(&value);
    p[0]=static_cast<unsigned char>(word);p[1]=static_cast<unsigned char>(word>>8);
    p[2]=static_cast<unsigned char>(word>>16);p[3]=static_cast<unsigned char>(word>>24);
    return value;
}
inline float read_float_fresh(Address a) {return float_from_word(read_word_fresh(a));}
inline void write_float_ordered(Address a,float value) {
    const unsigned char* input=reinterpret_cast<const unsigned char*>(&value);
    volatile unsigned char* output=bytes(a);
    output[0]=input[0];output[1]=input[1];output[2]=input[2];output[3]=input[3];
}
inline int read_signed_byte_fresh(Address a) {
    const unsigned value=*static_cast<const volatile unsigned char*>(bytes(a));
    return value<128u?int(value):int(value)-256;
}
// Typed genuine scalar EE operations retain individual native instructions.
// They do not implement a host arithmetic or general FCSR/exception model.
inline float native_add(float left,float right) {
    float result;asm volatile("add.s %0,%1,%2":"=f"(result):"f"(left),"f"(right));return result;
}
inline float native_subtract(float left,float right) {
    float result;asm volatile("sub.s %0,%1,%2":"=f"(result):"f"(left),"f"(right));return result;
}
inline float native_multiply(float left,float right) {
    float result;asm volatile("mul.s %0,%1,%2":"=f"(result):"f"(left),"f"(right));return result;
}
inline float native_signed_to_float(int integer) {
    const float input=float_from_word(unsigned(integer));
    float result;asm volatile("cvt.s.w %0,%1":"=f"(result):"f"(input));return result;
}
}
