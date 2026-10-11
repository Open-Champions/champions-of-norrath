#include "sound_rpc.hpp"
using namespace norrath_sound_rpc_detail;

extern "C" void norrath_sound_rpc_free_packet(void* raw_packet) {
    const Address packet=address(raw_packet);
    const unsigned captured_flags=read_word_fresh(packet+16u);
    write_word_ordered(packet+24u,0u);
    write_word_ordered(packet+16u,captured_flags&0xfffffffeu);
}
