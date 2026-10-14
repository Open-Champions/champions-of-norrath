#include "sound_remote_backend.hpp"
using namespace norrath_sound_remote_backend_detail;
using namespace norrath_sound_native_register_detail;

extern "C" int norrath_sound_remote(int mode,int command,...) {
    // va_start is used only for this compiler's native register-save area.
    // The typed native LD adapter snapshots six complete physical64-bit slots;
    // it never calls va_arg or invents values for absent source arguments.
    __builtin_va_list saved_register_cursor;
    __builtin_va_start(saved_register_cursor,command);
    const SixSavedArgumentRegisters captured_registers=
        capture_saved_argument_registers(static_cast<const unsigned char*>(saved_register_cursor));
    __builtin_va_end(saved_register_cursor);

    const Address packet=address(norrath_sound_remote_packet_view);
    const Address client=address(norrath_sound_remote_rpc_client_view);
    write_word_ordered(packet,packet);
    for(unsigned slot=0;slot<6u;++slot)
        write_word_ordered(packet+4u+slot*4u,unsigned(captured_registers.lane[slot]));

    unsigned end_function=0u;
    int rpc_mode=0;
    if(mode==0) {
        rpc_mode=1;
        end_function=read_word_fresh(address(norrath_sound_remote_end_function_word_view));
    }

    unsigned previous_interrupt_function=0u;
    if(unsigned(command)==0x8160u) {
        const unsigned selector=read_word_fresh(packet+4u);
        // Original branch-delay read precedes either callback state path.
        const unsigned interrupt_argument=read_word_fresh(packet+12u);
        const unsigned callback=read_word_fresh(packet+8u);
        const unsigned state_offset=selector==0u?0u:4u;
        const Address function_slot=address(norrath_sound_remote_interrupt_functions_view)+state_offset;
        previous_interrupt_function=read_word_fresh(function_slot);
        write_word_ordered(function_slot,callback);
        write_word_ordered(address(norrath_sound_remote_interrupt_arguments_view)+state_offset,
                           interrupt_argument);
        const unsigned callback_gp=unsigned(current_gp_bits());
        write_word_ordered(address(norrath_sound_remote_interrupt_gps_view)+state_offset,callback_gp);
    } else if(unsigned(command)==0x8170u) {
        const unsigned interrupt_argument=read_word_fresh(packet+8u);
        const unsigned callback=read_word_fresh(packet+4u);
        const Address function_slot=address(norrath_sound_remote_interrupt_functions_view)+8u;
        previous_interrupt_function=read_word_fresh(function_slot);
        write_word_ordered(function_slot,callback);
        write_word_ordered(address(norrath_sound_remote_interrupt_arguments_view)+8u,interrupt_argument);
        const unsigned callback_gp=unsigned(current_gp_bits());
        write_word_ordered(address(norrath_sound_remote_interrupt_gps_view)+8u,callback_gp);
    }

    // The original private stack result is genuinely initialized here. This
    // does not initialize the unwritten portion of the external64-byte packet.
    volatile unsigned result_word=0u;
    if(unsigned(command)==0x8130u || unsigned(command)==0x81a0u ||
       unsigned(command)==0x81b0u) {
        const Raw64 saved_gp=install_callback_gp_if_active(end_function);
        const unsigned end_parameter=read_word_fresh(packet);
        const unsigned command_field=read_word_fresh(packet+4u);
        const Address send=read_word_fresh(packet+8u);
        (void)norrath_sound_call_rpc(bytes(client),signed_word(unsigned(command)|command_field),
             rpc_mode,bytes(send),64,bytes(packet),64,
             bytes(end_function),bytes(end_parameter));
        // Original common exit always reads packet0. If active, it captures
        // and restores GP, then rereads packet0; that second word is the result.
        const unsigned first_packet_word=read_word_fresh(packet);
        (void)restore_callback_gp_if_active(end_function,saved_gp);
        result_word=end_function!=0u?read_word_fresh(packet):first_packet_word;
    } else if(unsigned(command)==0x8140u) {
        const Raw64 saved_gp=install_callback_gp_if_active(end_function);
        const Address receive=read_word_fresh(packet+8u);
        const unsigned command_field=read_word_fresh(packet+4u);
        (void)norrath_sound_call_rpc(bytes(client),signed_word(command_field|0x8140u),
             rpc_mode,bytes(packet),64,bytes(receive),64,
             bytes(end_function),bytes(receive));
        // The actual returnedGP is captured/restored on the active path,
        // but this command's result remains the initialized private result.
        (void)restore_callback_gp_if_active(end_function,saved_gp);
    } else if(unsigned(command)==0x81c0u || unsigned(command)==0x81d0u) {
        const Address send=read_word_fresh(packet+4u);
        const unsigned count_half=read_half_fresh(packet+8u);
        write_half_ordered(send+2u,count_half);
        if(unsigned(command)==0x81d0u) {
            const unsigned extra_field=read_word_fresh(packet+20u);
            write_word_ordered(send+4u,extra_field);
        }
        Address receive=read_word_fresh(packet+12u);
        unsigned receive_bytes=read_word_fresh(packet+16u); // mandatory delay read
        if(receive==0u) {
            receive=address(const_cast<unsigned*>(&result_word));
            receive_bytes=4u;
        }
        const Raw64 saved_gp=install_callback_gp_if_active(end_function);
        const unsigned send_count=read_word_fresh(packet+8u);
        const Address fresh_send=read_word_fresh(packet+4u);
        const unsigned send_bytes=(send_count+1u)*8u;
        (void)norrath_sound_call_rpc(bytes(client),command,rpc_mode,bytes(fresh_send),
             signed_word(send_bytes),bytes(receive),signed_word(receive_bytes),
             bytes(end_function),bytes(packet));
        (void)restore_callback_gp_if_active(end_function,saved_gp);
        const Address fresh_receive=read_word_fresh(packet+12u);
        if(fresh_receive!=0u)result_word=read_word_fresh(fresh_receive);
    } else {
        const Raw64 saved_gp=install_callback_gp_if_active(end_function);
        if(unsigned(command)-0x9000u<0xf1u) {
            const Address send=read_word_fresh(packet+4u);
            const unsigned send_bytes=read_word_fresh(packet+8u);
            (void)norrath_sound_call_rpc(bytes(client),command,rpc_mode,bytes(send),
                 signed_word(send_bytes),bytes(packet),16,
                 bytes(end_function),bytes(packet));
        } else {
            (void)norrath_sound_call_rpc(bytes(client),command,rpc_mode,bytes(packet),64,
                 bytes(packet),16,bytes(end_function),bytes(packet));
        }
        const unsigned first_packet_word=read_word_fresh(packet);
        (void)restore_callback_gp_if_active(end_function,saved_gp);
        result_word=end_function!=0u?read_word_fresh(packet):first_packet_word;
    }

    if(unsigned(command)==0x8160u || unsigned(command)==0x8170u)
        result_word=previous_interrupt_function;
    return signed_word(result_word);
}
