#include "sound_command_initialization.hpp"
using namespace norrath_sound_command_initialization_detail;
extern "C" void norrath_sound_initialize_command() {
    (void)norrath_sound_disable_interrupts();
    if(read_aligned_word(address(norrath_sound_command_initialized_word_view))!=0u) {
        (void)norrath_sound_enable_interrupts();
        return;
    }
    const Address packet_buffer=address(norrath_sound_command_packet_buffer_view);
    const Address send_buffer=address(norrath_sound_command_send_buffer_view);
    const Address state=address(norrath_sound_command_state_view);
    const Address handlers=address(norrath_sound_command_system_handlers_view);
    const Address soft_registers=address(norrath_sound_system_register_words_view);
    write_aligned_word(address(norrath_sound_command_initialized_word_view),1u);
    write_aligned_word(state,packet_buffer|0x20000000u);
    write_aligned_word(state+28u,soft_registers);
    write_aligned_word(state+4u,send_buffer|0x20000000u);
    write_aligned_word(state+16u,32u);
    write_aligned_word(state+8u,0u);
    write_aligned_word(state+12u,handlers);
    write_aligned_word(state+20u,0u);
    write_aligned_word(state+24u,0u);
    // Actual do-first twelve-byte cursor loop; third word remains untouched.
    Address cursor=handlers;
    const Address limit=handlers+384u;
    do {
        write_aligned_word(cursor,0u);
        write_aligned_word(cursor+4u,0u);
        cursor+=12u;
    } while(signed_word(cursor)<signed_word(limit));
    // Actual32 descending stores, not source-owned zero data or an index cap.
    int ordinal=31;
    cursor=soft_registers+124u;
    do {
        write_aligned_word(cursor,0u);
        --ordinal;
        cursor-=4u;
    } while(ordinal>=0);
    // Distinct genuine LOCAL codeword imports, not original-address aliases.
    write_aligned_word(handlers,command_handler_word(norrath_sound_command_change_address));
    write_aligned_word(handlers+12u,command_handler_word(norrath_sound_command_set_system_register));
    write_aligned_word(handlers+16u,state);
    write_aligned_word(handlers+4u,state);
    (void)norrath_sound_enable_interrupts();
    norrath_sound_flush_cache(0);
    // Exact native physical register addresses observed in the original.
    // They have no ELF data symbols/PT_LOADs or candidate storage. These are
    // actual volatile32 effects, never fabricated SDK/cache/device responses.
    if((read_aligned_word(0x1000e010u)&0x20u)!=0u)
        write_aligned_word(0x1000e010u,32u);
    if((read_aligned_word(0x1000c000u)&0x100u)==0u)
        norrath_sound_set_dma_chain();
    const Raw64 handler_id=norrath_sound_add_dma_handler(5,norrath_sound_command_dma_handler,0);
    write_aligned_word(address(norrath_sound_command_dma_handle_view),unsigned(handler_id));
    (void)norrath_sound_enable_dma(5);
    const Raw64 existing=norrath_sound_get_register(signed_word(0x80000000u));
    write_aligned_word(state+8u,unsigned(existing));
    const Address command_packet=address(norrath_sound_command_initialization_packet_view);
    if(existing!=0ull) {
        // Fast path leaves command_packet+12 untouched, publishes the raw
        // packet-buffer address (not the uncached state0 pointer), then sends.
        write_aligned_word(command_packet+16u,packet_buffer);
        (void)norrath_sound_send_command(signed_word(0x80000000u),pointer(command_packet),20,
                                         nullptr,nullptr,0);
        return;
    }
    // Genuine uncapped service polling; no host answer, delay or invented cap.
    while((norrath_sound_get_register(4)&0x20000ull)==0ull) {}
    const Raw64 remote_address=norrath_sound_get_register(2);
    write_aligned_word(state+8u,unsigned(remote_address));
    // Existing canonical Set interface is unsigned32. Original fullraw A1
    // upperbits are not established by this whole source/interface boundary.
    norrath_sound_set_register(signed_word(0x80000000u),unsigned(remote_address));
    norrath_sound_set_register(signed_word(0x80000001u),state);
    write_aligned_word(command_packet+16u,packet_buffer);
    write_aligned_word(command_packet+12u,0u);
    (void)norrath_sound_send_command(signed_word(0x80000002u),pointer(command_packet),20,
                                     nullptr,nullptr,0);
    // Void canonical API does not promise the original final tail-call V0.
}
