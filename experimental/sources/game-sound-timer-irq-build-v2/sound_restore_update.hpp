#pragma once
#include "sound_sequence_control.hpp"
#include "health_regeneration.hpp"

extern "C" {
void norrath_sound_restore(bool reset_hardware);
void norrath_sound_update_adjustments();
int norrath_sound_get_stream_play_status(int stream_index);
void norrath_sound_initialize_hardware();
unsigned norrath_sound_query_max_free_memory();
unsigned norrath_sound_allocate_iop_heap(unsigned bytes);
int norrath_sce_printf(const char* format,...);
void norrath_sound_assert(const char* file,unsigned line,const char* condition,int captured_ra);
int norrath_sound_call_rpc(void* client,int command,int mode,void* send,int send_bytes,
                          void* receive,int receive_bytes,void* end_function,void* end_parameter);

// Address views of the actual original objects; no backing allocations or
// original payload are supplied by these independently authored routines.
extern unsigned char norrath_sound_stream_buffer_sizes_view[];
extern unsigned char norrath_sound_stream_volumes_view[];
extern unsigned char norrath_sound_stereo_enable_word_view[];
extern unsigned char norrath_sound_stream_heap_words_view[];
extern unsigned char norrath_sound_stream_records_view[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];
extern unsigned char norrath_sound_restore_formats_view[];
extern unsigned char norrath_sound_startup_assert_file_view[];
extern unsigned char norrath_sound_restore_allocation_condition_view[];
extern unsigned char norrath_sound_restore_slot_condition_view[];
extern unsigned char norrath_sound_restore_stream_count_condition_view[];
}

namespace norrath_sound_restore_update_detail {
using namespace norrath_sound_sequence_control_detail;
inline const char* text_at(Address a) {return reinterpret_cast<const char*>(bytes(a));}
inline Address stream_record(int index) {
    return address(norrath_sound_stream_records_view)+unsigned(index)*452u;
}
inline float float_bits(unsigned word) {
    return norrath_health_regeneration_detail::float_from_word(word);
}
inline float read_float_fresh(Address a) {return float_bits(read_word_fresh(a));}
// Only the genuine typed native operation and its actual operands are adapted.
// The memory clobber preserves source capture boundaries, not a hardware fence.
inline float native_multiply_ordered(float left,float right) {
    float result;
    asm volatile("mul.s %0,%1,%2":"=f"(result):"f"(left),"f"(right):"memory");
    return result;
}
inline unsigned native_convert_word(float input) {
    unsigned result;float converted;
    asm volatile("cvt.w.s %1,%2\n\tmfc1 %0,%1"
                 :"=&r"(result),"=&f"(converted):"f"(input):"memory");
    return result;
}
// This is the live previous-call return lane observed at the assertion site.
// It is neither a constant original PC nor the enclosing caller's saved RA.
__attribute__((always_inline)) inline unsigned live_return_register() {
    unsigned result;
    asm volatile("move %0,$31":"=r"(result)::"memory");
    return result;
}
__attribute__((always_inline)) inline void assert_original(unsigned line,const unsigned char* condition,unsigned captured_ra) {
    norrath_sound_assert(text_at(address(norrath_sound_startup_assert_file_view)),line,
                         text_at(address(condition)),signed_word(captured_ra));
}
inline void rpc_original(int command,int send_bytes) {
    norrath_sound_call_rpc(bytes(address(norrath_sound_rpc_client_view)),command,0,
                          bytes(address(norrath_sound_rpc_send_view)),send_bytes,
                          bytes(address(norrath_sound_rpc_receive_view)),64,nullptr,nullptr);
}
}

