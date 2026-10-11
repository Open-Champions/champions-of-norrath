#include "sound_rpc_completion.hpp"
using namespace norrath_sound_rpc_completion_detail;
extern "C" unsigned long long norrath_sound_rpc_retry_bind(
    unsigned long long,unsigned long long,unsigned long long,void* packet) {
    const Raw64 sent=norrath_sound_interrupt_send_command(
        signed_word(0x80000008u),packet,64,nullptr,nullptr,0);
    return sent!=0u?0ull:2048ull;
}
