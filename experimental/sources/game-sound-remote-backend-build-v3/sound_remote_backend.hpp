#pragma once
#include "sound_startup.hpp"
#include "sound_restore_update.hpp"
#include "sound_native_registers.hpp"

extern "C" {
// Exact existing canonical entries: no alternate eight-argument definition,
// wrapper, alias or original-address provider replaces the variadic symbol.
int norrath_sound_remote_initialize();
int norrath_sound_remote(int mode,int command,...);
// Whole original FlushCache16 syscall effect; genuine unfinished provider.
void norrath_sound_flush_cache(int mode);

// Genuine minimum observations only, with no source storage or initializer.
extern unsigned char norrath_sound_remote_packet_view[];
extern unsigned char norrath_sound_remote_rpc_client_view[];
extern unsigned char norrath_sound_remote_end_function_word_view[];
extern unsigned char norrath_sound_remote_end_gp_word_view[];
extern unsigned char norrath_sound_remote_interrupt_functions_view[];
extern unsigned char norrath_sound_remote_interrupt_arguments_view[];
extern unsigned char norrath_sound_remote_interrupt_gps_view[];
extern unsigned char norrath_sound_remote_bind_failure_format_view[];
}

namespace norrath_sound_remote_backend_detail {
using namespace norrath_sound_sequence_control_detail;
using norrath_sound_native_register_detail::Raw64;
using norrath_sound_native_register_detail::current_gp_bits;
using norrath_sound_native_register_detail::install_gp_bits;
using norrath_sound_native_register_detail::signed_word_gp_bits;
inline const char* text_at(Address a) {
    return reinterpret_cast<const char*>(bytes(a));
}
__attribute__((always_inline)) inline void four_original_wait_nops() {
    asm volatile("nop\n\tnop\n\tnop\n\tnop":::"memory");
}
inline unsigned poll_remote_server_word(Address client) {
    return *reinterpret_cast<const volatile unsigned*>(
        static_cast<__UINTPTR_TYPE__>(client+36u));
}
// Each switch uses the fresh original signed LW context, not baselineGP or a
// compiled function address. The caller must retain the captured full64 GP
// across the actual RPC, then restore it before any later external call.
__attribute__((always_inline)) inline Raw64 install_callback_gp_if_active(unsigned end_function) {
    if(end_function==0u)return 0u;
    const unsigned callback_gp=read_word_fresh(address(norrath_sound_remote_end_gp_word_view));
    const Raw64 saved_gp=current_gp_bits();
    install_gp_bits(signed_word_gp_bits(callback_gp));
    return saved_gp;
}
__attribute__((always_inline)) inline Raw64 restore_callback_gp_if_active(
        unsigned end_function,Raw64 saved_gp) {
    if(end_function==0u)return 0u;
    const Raw64 returned_gp=current_gp_bits();
    install_gp_bits(saved_gp);
    return returned_gp;
}
}
