#include "sound_rpc_callbacks.hpp"
using namespace norrath_sound_rpc_callbacks_detail;
extern "C" void norrath_sound_rpc_request_rdata(void* request,void* pool_state) {
    const Address incoming=address(request);
    const unsigned flags=read_word_fresh(incoming+16u);
    const Address packet=address((flags&4u)
        ? norrath_sound_rpc_get_free_packet_indexed(pool_state,int(flags>>16))
        : norrath_sound_rpc_get_free_packet(pool_state));
    const unsigned source_packet=read_word_fresh(incoming+20u);
    const unsigned client=read_word_fresh(incoming+28u);
    write_word_ordered(packet+20u,source_packet);
    write_word_ordered(packet+28u,client);
    write_word_ordered(packet+32u,0x8000000cu);
    write_word_ordered(packet+36u,read_word_fresh(incoming+32u));
    write_word_ordered(packet+40u,read_word_fresh(incoming+36u));
    write_word_ordered(packet+44u,read_word_fresh(incoming+40u));

    // Three fresh request reads, including the native last-delay-slot length.
    const unsigned data_bytes=read_word_fresh(incoming+40u);
    const Address source=read_word_fresh(incoming+32u);
    const Address destination=read_word_fresh(incoming+36u);
    const unsigned long long sent=norrath_sound_interrupt_send_command(
        signed_word(0x80000008u),bytes(packet),64,bytes(source),
        bytes(destination),signed_word(data_bytes));
    if(sent==0u)
        norrath_sound_interrupt_set_timer_alarm(
            2048ull,norrath_sound_rpc_retry_rdata,bytes(packet));
}
