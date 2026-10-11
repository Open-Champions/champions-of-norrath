#include "sound_rpc_callbacks.hpp"
using namespace norrath_sound_rpc_callbacks_detail;
extern "C" void* norrath_sound_rpc_get_free_packet_indexed(void* pool_state,int index) {
    const Address state=address(pool_state);
    if(index>=0) {
        const unsigned captured_count=read_word_fresh(state+32u);
        if(index<signed_word(captured_count)) {
            const Address base=read_word_fresh(state+28u);
            return bytes(base+(unsigned(index)<<6));
        }
    }
    return norrath_sound_rpc_get_free_packet(pool_state);
}
