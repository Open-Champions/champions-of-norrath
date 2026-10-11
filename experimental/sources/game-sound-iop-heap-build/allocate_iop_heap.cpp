#include "sound_iop_heap.hpp"
using namespace norrath_sound_iop_heap_detail;

extern "C" unsigned norrath_sound_allocate_iop_heap(unsigned requested_bytes) {
    if (signed_word(read_aligned_word_fresh(address(norrath_sound_iop_heap_bind_word_view)))<0) return 0u;
    write_aligned_word(address(norrath_sound_iop_heap_send_view),requested_bytes);
    const int status=norrath_sound_call_rpc(
        bytes(address(norrath_sound_iop_heap_client_view)),1,0,
        bytes(address(norrath_sound_iop_heap_send_view)),4,
        bytes(address(norrath_sound_iop_heap_receive_view)),4,nullptr,nullptr);
    // The original ordinary BGEZ delay slot reads this word even if the real
    // RPC failed; preserve that fresh read before selecting the actual result.
    const unsigned received=read_aligned_word_fresh(address(norrath_sound_iop_heap_receive_view));
    return status<0?0u:received;
}
