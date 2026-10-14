#include "sound_rpc_callbacks.hpp"
using namespace norrath_sound_rpc_callbacks_detail;
extern "C" void* norrath_sound_rpc_find_server(unsigned server_id,void* pool_state) {
    const Address state=address(pool_state);
    Address queue=read_word_fresh(state+40u);
    while(queue) {
        Address server=read_word_fresh(queue+8u);
        while(server) {
            const unsigned candidate_id=read_word_fresh(server);
            if(candidate_id==server_id)return bytes(server);
            server=read_word_fresh(server+56u);
        }
        queue=read_word_fresh(queue+20u);
    }
    return nullptr;
}
