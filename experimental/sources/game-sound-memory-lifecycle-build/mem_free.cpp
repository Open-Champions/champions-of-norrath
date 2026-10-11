#include "sound_memory_lifecycle.hpp"
using namespace norrath_sound_memory_lifecycle_detail;

extern "C" void norrath_mem_free(void* allocation) {
    const Address header=address(allocation)-28u;
    // No null guard or guessed block validation is added before the real check.
    norrath_mem_check_heap_real(text_at(address(norrath_heap_check_file_view)),583);
    const Address free_bytes=address(norrath_heap_free_bytes_view);
    const unsigned old_free_bytes=read_word_fresh(free_bytes);
    const unsigned old_size=read_word_fresh(header+4u);
    const Address next=read_word_fresh(header+12u);
    write_word_ordered(address(norrath_heap_next_fit_view),header);
    write_word_ordered(free_bytes,old_free_bytes+old_size+28u);
    write_half_ordered(header+2u,0u);
    write_word_ordered(header+8u,0u);

    const unsigned next_kind=read_half_fresh(next+2u);
    if(next_kind==0u && header<next) {
        const Address next_next=read_word_fresh(next+12u);
        const unsigned next_size=read_word_fresh(next+4u);
        write_word_ordered(next_next+16u,header);
        write_half_ordered(next+2u,2u);
        // The native code rereads the header link after publishing next_kind.
        const Address fresh_next=read_word_fresh(header+12u);
        write_word_ordered(header+4u,old_size+next_size+28u);
        const Address fresh_next_next=read_word_fresh(fresh_next+12u);
        write_word_ordered(header+12u,fresh_next_next);
    }

    const Address previous=read_word_fresh(header+16u);
    const unsigned previous_kind=read_half_fresh(previous+2u);
    if(previous_kind==0u && previous<header) {
        const Address captured_next=read_word_fresh(header+12u);
        const unsigned captured_size=read_word_fresh(header+4u);
        write_word_ordered(previous+12u,captured_next);
        const unsigned previous_size=read_word_fresh(previous+4u);
        const Address fresh_next=read_word_fresh(header+12u);
        const Address fresh_previous=read_word_fresh(header+16u);
        write_half_ordered(header+2u,2u);
        write_word_ordered(fresh_next+16u,fresh_previous);
        write_word_ordered(previous+4u,previous_size+captured_size+28u);
        const Address final_previous=read_word_fresh(header+16u);
        write_word_ordered(address(norrath_heap_next_fit_view),final_previous);
    }
}

