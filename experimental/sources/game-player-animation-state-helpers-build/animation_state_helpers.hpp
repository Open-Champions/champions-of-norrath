#pragma once
#include "health_regeneration.hpp"
#include "creature_animation.hpp"
extern "C" {
// FullBlend exactly matches the existing frozen Creature caller declaration.
unsigned norrath_animation_set_to_end_fully_blended(void* animation_state);
unsigned norrath_animation_header_initialize(void* animation_header);
void norrath_animation_append_to_end(void* animation_state,void* animation_header,int flags);
void* norrath_animation_set_blend(void* animation_state,void* animation_header,float blend);
}
namespace norrath_animation_state_detail {
using namespace norrath_health_regeneration_detail;
inline unsigned read_byte_fresh(Address a){return *static_cast<const volatile unsigned char*>(bytes(a));}
inline void write_byte_ordered(Address a,unsigned value){*static_cast<volatile unsigned char*>(bytes(a))=static_cast<unsigned char>(value);}
// Compiler-only ordering barriers retain the actual memory/native-operation
// boundaries. They emit no original body/byte payload, output or hardware fence.
// Reused typed volatile CVT.S.W/SUB.S keep the real operands and FCSR boundary;
// they do not establish native exceptions/rounding/fault or all-input execution.
inline float ordered_signed_to_float(int value){
    asm volatile("":::"memory");
    const float result=native_signed_to_float(value);
    asm volatile("":::"memory");
    return result;
}
inline float ordered_subtract(float left,float right){
    asm volatile("":::"memory");
    const float result=native_subtract(left,right);
    asm volatile("":::"memory");
    return result;
}
}
