#include "sound_rpc_completion.hpp"
using namespace norrath_sound_rpc_completion_detail;
extern "C" unsigned long long norrath_sound_rpc_retry_rdata(
    unsigned long long,unsigned long long,unsigned long long,void* raw_packet) {
    const Address packet=address(raw_packet);
    const Address source=read_word_fresh(packet+36u);
    const Address destination=read_word_fresh(packet+40u);
    const unsigned data_bytes=read_word_fresh(packet+44u);
    const Raw64 sent=norrath_sound_interrupt_send_command(
        signed_word(0x80000008u),bytes(packet),64,bytes(source),
        bytes(destination),signed_word(data_bytes));
    return sent!=0u?0ull:2048ull;
}
