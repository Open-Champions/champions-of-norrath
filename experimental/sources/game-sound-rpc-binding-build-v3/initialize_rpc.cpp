#include "sound_rpc_binding.hpp"
using namespace norrath_sound_rpc_binding_detail;

extern "C" void norrath_sound_initialize_rpc(int mode) {
    (void)mode; // The original body does not use incoming A0.
    norrath_sound_disable_interrupts();
    const Address initialized=address(norrath_sound_rpc_initialized_word_view);
    if(read_word_fresh(initialized)!=0u) {
        norrath_sound_enable_interrupts();
        return;
    }
    write_word_ordered(initialized,1u);
    norrath_sound_enable_interrupts();
    norrath_sound_initialize_command();
    norrath_sound_disable_interrupts();

    const Address state=address(norrath_sound_rpc_packet_pool_state_view);
    const Address packets=address(norrath_sound_rpc_packet_buffer_view);
    const Address free_packets=address(norrath_sound_rpc_free_buffer_view);
    const Address free_packets2=address(norrath_sound_rpc_free_buffer2_view);
    // Actual individual publications in original order. These constants are
    // source effects, not a complete zeroed state or backing-array owner.
    write_word_ordered(state+32u,32u);
    write_word_ordered(state,1u);
    write_word_ordered(state+20u,uncached_pointer_word(free_packets));
    write_word_ordered(state+4u,uncached_pointer_word(packets));
    write_word_ordered(state+28u,uncached_pointer_word(free_packets2));
    write_word_ordered(state+8u,32u);
    write_word_ordered(state+12u,0u);
    write_word_ordered(state+16u,0u);
    write_word_ordered(state+24u,32u);
    write_word_ordered(state+36u,0u);
    norrath_sound_add_command_handler(signed_word(0x80000008u),
        norrath_sound_rpc_request_end,bytes(state));
    norrath_sound_add_command_handler(signed_word(0x80000009u),
        norrath_sound_rpc_request_bind,bytes(state));
    norrath_sound_add_command_handler(signed_word(0x8000000au),
        norrath_sound_rpc_request_call,bytes(state));
    norrath_sound_add_command_handler(signed_word(0x8000000cu),
        norrath_sound_rpc_request_rdata,bytes(state));
    norrath_sound_enable_interrupts();

    // Preserve full64 service result comparison; never narrow to a lowword or
    // synthesize a ready result. Both service entries remain genuine imports.
    if(norrath_sound_get_register(signed_word(0x80000002u))!=0ull)return;
    const Address initialization_packet=packets+64u;
    write_word_ordered(initialization_packet+12u,1u);
    norrath_sound_send_command(signed_word(0x80000002u),
        bytes(initialization_packet),16,nullptr,nullptr,0);
    while(norrath_sound_get_system_register(0)==0ull) {
        // Uncapped genuine repeated service read; no response, delay timer or
        // initialization of unwritten packet/header bytes is substituted.
    }
    norrath_sound_set_register(signed_word(0x80000002u),1u);
}
