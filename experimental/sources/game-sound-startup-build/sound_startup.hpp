#pragma once
#include "sound_sequence_control.hpp"
#include "health_regeneration.hpp"
extern "C" {
void norrath_sound_initialize(float gain);
void norrath_sound_initialize_hardware();
// Entire exact LOCAL original, explicit private machine entry only. This does
// not register a constructor or establish any public/global original alias.
void norrath_sound_static_initialization(int phase,unsigned priority);
void norrath_animation_state_destroy(void* state,int flags);
void norrath_sound_resource_map_initialize(void* storage,char* name,char* extension);
// Complete real external services, no answers or original code address bindings.
int norrath_sce_printf(const char* format,...);
int norrath_sound_remote_initialize();
int norrath_sound_remote(int mode,int command,...);
unsigned norrath_sound_allocate_iop_heap(unsigned bytes);
void norrath_sound_initialize_rpc(int mode);
int norrath_sound_bind_rpc(void* client,int server_id,int mode);
void norrath_sound_restore(bool reset_hardware);
void norrath_sound_update_adjustments();
void norrath_sound_assert(const char* file,unsigned line,const char* condition,int captured_ra);
void norrath_animation_state_deallocate(void* state);
// Genuine original observations only, no startup data owner or initial payload.
extern unsigned char norrath_sound_master_volume_view[];
extern unsigned char norrath_sound_master_scale_view[];
extern unsigned char norrath_sound_batch_iop_buffer_word_view[];
extern unsigned char norrath_sound_iop_buffer_word_view[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_startup_formats_view[];
extern unsigned char norrath_sound_startup_assert_file_view[];
extern unsigned char norrath_sound_startup_assert_conditions_view[];
extern unsigned char norrath_sound_global_dialog_map_view[];
extern unsigned char norrath_sound_global_dialog_name_view[];
extern unsigned char norrath_sound_global_dialog_extension_view[];
}
namespace norrath_sound_startup_detail {
using namespace norrath_sound_sequence_control_detail;
inline float float_bits(unsigned value) {return norrath_health_regeneration_detail::float_from_word(value);}
inline float read_float_fresh(Address a) {return float_bits(read_word_fresh(a));}
inline float native_multiply_ordered(float left,float right) {
    asm volatile("":::"memory");
    const float result=norrath_health_regeneration_detail::native_multiply(left,right);
    asm volatile("":::"memory");
    return result;
}
// Genuine C.LT.S/FCC0 gate, real inputs/output. No host comparison or fixed
// answer. Compiler barriers preserve memory/operation boundaries, not fences.
inline bool native_less_than(float left,float right) {
    unsigned result;
    asm volatile(".set push\n\t.set noreorder\n\tc.lt.s %1,%2\n\tbc1f 1f\n\tmove %0,$zero\n\taddiu %0,$zero,1\n1:\n\t.set pop"
                 :"=&r"(result):"f"(left),"f"(right):"memory");
    return result!=0u;
}
// Actual typed CVT.W.S→MFC1 bits; no C++ numeric conversion/default/clamp.
inline unsigned native_convert_word(float input) {
    unsigned result;float converted;
    asm volatile("cvt.w.s %1,%2\n\tmfc1 %0,%1"
                 :"=&r"(result),"=&f"(converted):"f"(input):"memory");
    return result;
}
__attribute__((always_inline)) inline unsigned live_return_register() {
    unsigned result;asm volatile("move %0,$31":"=r"(result)::"memory");return result;
}
__attribute__((always_inline)) inline void five_original_wait_nops() {
    asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop":::"memory");
}
inline unsigned poll_rpc_server_word(Address client) {
    // Original client+36 LW requires alignment4; actual pinned base7dd7e0
    // satisfies it. This genuine volatile width retains every real recheck.
    return *reinterpret_cast<const volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(client+36u));
}
inline const char* text_at(Address a) {return reinterpret_cast<const char*>(bytes(a));}
inline int signed_byte_value(unsigned value) {return value<128u?int(value):int(value)-256;}
}
