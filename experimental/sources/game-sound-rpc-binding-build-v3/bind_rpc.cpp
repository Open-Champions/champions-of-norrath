#include "sound_rpc_binding.hpp"
using namespace norrath_sound_rpc_binding_detail;

extern "C" int norrath_sound_bind_rpc(void* raw_client,int server_id,int mode) {
    const Address client=address(raw_client);
    write_word_ordered(client+16u,0u);
    // Original packet allocator JAL delay publishes this even on failure.
    write_word_ordered(client+36u,0u);
    void* raw_packet=norrath_sound_rpc_get_packet(
        bytes(address(norrath_sound_rpc_packet_pool_state_view)));
    if(raw_packet==nullptr)return -1;
    const Address packet=address(raw_packet);
    const unsigned packet_serial=read_word_fresh(packet+24u);
    write_word_ordered(client,packet);
    write_word_ordered(client+4u,packet_serial);
    write_word_ordered(packet+32u,unsigned(server_id));
    write_word_ordered(packet+20u,packet);
    // Original mode branch delay publishes the client on BOTH paths.
    write_word_ordered(packet+28u,client);

    if((unsigned(mode)&1u)!=0u) {
        write_word_ordered(client+8u,0xffffffffu);
        const int sent=norrath_sound_send_command(signed_word(0x80000009u),
            raw_packet,64,nullptr,nullptr,0);
        if(sent!=0)return 0;
        norrath_sound_rpc_free_packet(raw_packet);
        return -2;
    }

    // Original112-byte frame has local bytes0..31 before its first saved GPR.
    // Only these two words are written; all other kernel workspace bytes stay
    // unwritten. No semaphore descriptor layout or default field is inferred.
    alignas(8) unsigned char semaphore_workspace[32];
    write_word_ordered(address(semaphore_workspace)+8u,0u);
    write_word_ordered(address(semaphore_workspace)+4u,1u);
    const int semaphore=norrath_sound_create_semaphore(semaphore_workspace);
    write_word_ordered(client+8u,unsigned(semaphore));
    if(semaphore<0) {
        norrath_sound_rpc_free_packet(raw_packet);
        return -3;
    }
    const int sent=norrath_sound_send_command(signed_word(0x80000009u),
        raw_packet,64,nullptr,nullptr,0);
    if(sent==0) {
        // Bind's actual failure order differs from CallRPC: free FIRST, then
        // reload client8 after that real call for the genuine deletion.
        norrath_sound_rpc_free_packet(raw_packet);
        norrath_sound_delete_semaphore(signed_word(read_word_fresh(client+8u)));
        return -2;
    }
    norrath_player_wait_semaphore(signed_word(read_word_fresh(client+8u)));
    norrath_sound_delete_semaphore(signed_word(read_word_fresh(client+8u)));
    return 0;
}
