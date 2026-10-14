#include "sound_rpc_callbacks.hpp"
using namespace norrath_sound_rpc_callbacks_detail;
extern "C" void norrath_sound_rpc_request_bind(void* request,void* pool_state) {
    const Address incoming=address(request);
    const Address packet=address(norrath_sound_rpc_get_free_packet(pool_state));
    const unsigned client=read_word_fresh(incoming+28u);
    const unsigned source_packet=read_word_fresh(incoming+20u);
    write_word_ordered(packet+28u,client);
    write_word_ordered(packet+20u,source_packet);
    write_word_ordered(packet+32u,0x80000009u);
    const unsigned server_id=read_word_fresh(incoming+32u);
    const Address server=address(norrath_sound_rpc_find_server(server_id,pool_state));
    if(server) {
        write_word_ordered(packet+36u,server);
        // Fresh after publishing the server pointer, including possible aliases.
        const unsigned receive_buffer=read_word_fresh(server+8u);
        write_word_ordered(packet+40u,receive_buffer);
    } else {
        write_word_ordered(packet+36u,0u);
        write_word_ordered(packet+40u,0u);
    }
    const unsigned long long sent=norrath_sound_interrupt_send_command(
        signed_word(0x80000008u),bytes(packet),64,nullptr,nullptr,0);
    if(sent==0u)
        norrath_sound_interrupt_set_timer_alarm(
            2048ull,norrath_sound_rpc_retry_bind,bytes(packet));
}
