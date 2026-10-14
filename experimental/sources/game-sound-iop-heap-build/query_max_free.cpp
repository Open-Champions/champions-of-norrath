#include "sound_iop_heap.hpp"
using namespace norrath_sound_iop_heap_detail;

extern "C" unsigned norrath_sound_query_max_free_memory() {
    if (signed_word(read_aligned_word_fresh(address(norrath_sound_iop_heap_bind_word_view)))<0) return 0u;
    const int status=norrath_sound_call_rpc(
        bytes(address(norrath_sound_iop_heap_client_view)),6,0,
        nullptr,0,bytes(address(norrath_sound_iop_heap_receive_view)),4,nullptr,nullptr);
    // Here the original BGEZL annuls its receive-word load on the negative
    // path. That failure returns the real original unsigned -1 value.
    if (status<0) return 0xffffffffu;
    return read_aligned_word_fresh(address(norrath_sound_iop_heap_receive_view));
}
