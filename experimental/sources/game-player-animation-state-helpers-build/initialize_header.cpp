#include "animation_state_helpers.hpp"
using namespace norrath_animation_state_detail;

extern "C" unsigned norrath_animation_header_initialize(void* animation_header) {
    const Address header=address(animation_header);
    const unsigned offset8=read_word_fresh(header+8u);
    const unsigned offset16=read_word_fresh(header+16u);
    const unsigned offset12=read_word_fresh(header+12u);
    const unsigned chain_offset=read_word_fresh(header+52u);
    write_word_ordered(header+8u,offset8+header);
    write_word_ordered(header+16u,offset16+header);
    write_word_ordered(header+12u,offset12+header);
    if(chain_offset!=0u){
        Address entry=header+chain_offset;
        const unsigned first_probe=read_word_fresh(entry);
        write_word_ordered(header+52u,entry);
        if(first_probe!=0u){
            // Pointer publication can alias the list. The first entry is read
            // again and written even if this fresh value has become zero.
            unsigned value=read_word_fresh(entry);
            for(;;){
                write_word_ordered(entry,value+header);
                entry+=8u;
                value=read_word_fresh(entry);
                if(value==0u)break;
            }
        }
    }
    const unsigned tail_offset=read_word_fresh(header+56u);
    const unsigned relocated_tail=tail_offset+header;
    if(tail_offset!=0u)write_word_ordered(header+56u,relocated_tail);
    return relocated_tail;
}
