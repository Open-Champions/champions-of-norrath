#pragma once
#include "skill_progression.hpp"
extern "C" {
// Exact canonical effect-only interfaces already used by complete actions.
void norrath_player_reset_animations(int player_index);
void norrath_inventory_set_animation(int player_index,void* animation_state,int mode);
// Genuine whole originals; no component/frame/transition response is supplied.
void norrath_animation_reset_frame_states(void* animation_state);
void* norrath_animation_get_component(void* animation_state,void* animation_header);
void norrath_animation_add_transition(void* animation_state,void* animation_header,float rate,int flags);
extern const unsigned char norrath_inventory_reset_resource_names_view[];
extern const unsigned char norrath_inventory_animation_lump_names_view[];
extern const unsigned char norrath_inventory_animation_resource_names_view[];
extern unsigned char norrath_inventory_animation_transition_rate_view[];
}
namespace norrath_inventory_animation_detail {
using namespace norrath_skill_progression_detail;
using Wide=unsigned long long;
inline unsigned read_byte_fresh(Address a){return *static_cast<const volatile unsigned char*>(bytes(a));}
inline int read_signed_byte_fresh(Address a){return signed_byte(static_cast<unsigned char>(read_byte_fresh(a)));}
inline void write_byte_ordered(Address a,unsigned value){*static_cast<volatile unsigned char*>(bytes(a))=static_cast<unsigned char>(value);}
inline Wide read_wide_fresh(Address a){const unsigned low=read_word_fresh(a),high=read_word_fresh(a+4u);return Wide(low)|(Wide(high)<<32);}
inline void write_wide_ordered(Address a,Wide value){write_word_ordered(a,unsigned(value));write_word_ordered(a+4u,unsigned(value>>32));}
inline float read_float_fresh(Address a){
    const unsigned bits=read_word_fresh(a);float result;
    unsigned char* p=reinterpret_cast<unsigned char*>(&result);
    p[0]=static_cast<unsigned char>(bits);p[1]=static_cast<unsigned char>(bits>>8);
    p[2]=static_cast<unsigned char>(bits>>16);p[3]=static_cast<unsigned char>(bits>>24);
    return result;
}
// All local stores/loads have their original distinct logical order; these are
// ordinary-RAM byte views, not native atomic LD/LHU/LW bus/fault promises.
inline void copy_name11(Address destination,Address source){
    const unsigned last=read_byte_fresh(source+10u);
    const Wide first=read_wide_fresh(source);
    const unsigned middle=read_half_fresh(source+8u);
    write_wide_ordered(destination,first);
    write_half_ordered(destination+8u,middle);
    write_byte_ordered(destination+10u,last);
}
inline void copy_name12(Address destination,Address source){
    const unsigned last=read_word_fresh(source+8u);
    const Wide first=read_wide_fresh(source);
    write_word_ordered(destination+8u,last);
    write_wide_ordered(destination,first);
}
inline void copy_name13(Address destination,Address source){
    const unsigned last=read_byte_fresh(source+12u);
    const Wide first=read_wide_fresh(source);
    const unsigned middle=read_word_fresh(source+8u);
    write_wide_ordered(destination,first);
    write_word_ordered(destination+8u,middle);
    write_byte_ordered(destination+12u,last);
}
}
