#include "sound_rpc.hpp"
using namespace norrath_sound_rpc_detail;

extern "C" int norrath_sound_call_rpc(void* client,int command,int mode,
                                     void* send,int send_bytes,void* receive,
                                     int receive_bytes,void* end_function,
                                     void* end_parameter) {
    void* raw_packet=norrath_sound_rpc_get_packet(
        bytes(address(norrath_sound_rpc_packet_pool_state_view)));
    if(raw_packet==nullptr)return -1;
    const Address packet=address(raw_packet);
    const Address client_address=address(client);
    // Capture the packet serial before any client publication, even when the
    // genuine dynamic packet/client regions overlap.
    const unsigned packet_serial=read_word_fresh(packet+24u);
    write_word_ordered(client_address+32u,address(end_parameter));
    write_word_ordered(client_address+4u,packet_serial);
    write_word_ordered(client_address,packet);
    write_word_ordered(client_address+28u,address(end_function));
    const unsigned original_gp=static_cast<unsigned>(
        norrath_sound_native_register_detail::current_gp_bits());
    write_word_ordered(client_address+24u,original_gp);

    write_word_ordered(packet+32u,unsigned(command));
    write_word_ordered(packet+36u,unsigned(send_bytes));
    write_word_ordered(packet+40u,address(receive));
    write_word_ordered(packet+44u,unsigned(receive_bytes));
    write_word_ordered(packet+20u,packet);
    const unsigned server_word=read_word_fresh(client_address+36u);
    write_word_ordered(packet+28u,client_address);
    write_word_ordered(packet+52u,server_word);

    if((unsigned(mode)&2u)==0u) {
        if(send==receive) {
            // Original signed maximum, including a nonpositive length; this
            // path still calls the genuine cache service once.
            const int length=send_bytes<receive_bytes?receive_bytes:send_bytes;
            norrath_sound_write_back_data_cache(send,length);
        } else {
            if(send_bytes>0)norrath_sound_write_back_data_cache(send,send_bytes);
            if(receive_bytes>0)
                norrath_sound_write_back_data_cache(receive,receive_bytes);
        }
    }

    if((unsigned(mode)&1u)!=0u) {
        write_word_ordered(packet+48u,end_function!=nullptr?1u:0u);
        const unsigned destination=read_word_fresh(client_address+20u);
        write_word_ordered(client_address+8u,0xffffffffu);
        const int result=norrath_sound_send_command(signed_word(0x8000000au),
            raw_packet,64,send,bytes(destination),send_bytes);
        if(result!=0)return 0;
        norrath_sound_rpc_free_packet(raw_packet);
        return -2;
    }

    // The original192-byte frame has a32-byte local region before its first
    // saved GPR. Only words+8 and+4 are written for the real kernel call. No
    // descriptor layout, default word0, remaining bytes or kernel read extent
    // is inferred. The unused object-representation bytes stay unwritten.
    alignas(8) unsigned char semaphore_workspace[32];
    write_word_ordered(address(semaphore_workspace)+8u,0u);
    write_word_ordered(address(semaphore_workspace)+4u,1u);
    const int semaphore=norrath_sound_create_semaphore(semaphore_workspace);
    write_word_ordered(client_address+8u,unsigned(semaphore));
    if(semaphore<0) {
        norrath_sound_rpc_free_packet(raw_packet);
        return -3;
    }
    write_word_ordered(packet+48u,1u);
    const unsigned destination=read_word_fresh(client_address+20u);
    const int result=norrath_sound_send_command(signed_word(0x8000000au),
        raw_packet,64,send,bytes(destination),send_bytes);
    if(result==0) {
        norrath_sound_delete_semaphore(signed_word(read_word_fresh(client_address+8u)));
        norrath_sound_rpc_free_packet(raw_packet);
        return -2;
    }
    norrath_player_wait_semaphore(signed_word(read_word_fresh(client_address+8u)));
    // This independent reread is after the genuine wait, not the initial ID.
    norrath_sound_delete_semaphore(signed_word(read_word_fresh(client_address+8u)));
    return 0;
}
