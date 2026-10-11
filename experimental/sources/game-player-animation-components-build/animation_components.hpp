#pragma once
#include "health_regeneration.hpp"
extern "C" {
void norrath_animation_reset_frame_states(void* animation_state);
void* norrath_animation_get_component(void* animation_state,void* animation_header);
void norrath_animation_add_transition(void* animation_state,void* animation_header,float rate,int flags);
extern const unsigned char norrath_animation_transition_threshold_view[];
}
namespace norrath_animation_components_detail {
using namespace norrath_health_regeneration_detail;
inline unsigned read_byte_fresh(Address a) {
    return *static_cast<const volatile unsigned char*>(bytes(a));
}
inline int signed_byte_value(unsigned value) {
    return value<128u?int(value):int(value)-256;
}
inline void write_byte_ordered(Address a,unsigned value) {
    *static_cast<volatile unsigned char*>(bytes(a))=static_cast<unsigned char>(value);
}
inline float native_divide(float left,float right) {
    float result;asm volatile("div.s %0,%1,%2":"=f"(result):"f"(left),"f"(right));return result;
}
}
