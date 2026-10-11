#include "sound_memory_lifecycle.hpp"
using namespace norrath_sound_memory_lifecycle_detail;

extern "C" void norrath_mem_free_wrapper(void* allocation) {
    // Original signed LB has only a zero/nonzero test; every byte value is
    // preserved, including a nonzero value with its sign bit set.
    if(read_byte_fresh(address(norrath_enable_malloc_wrapper_byte_view))!=0u)
        norrath_mem_free(allocation);
    else
        norrath_player_free_plain(allocation);
}

