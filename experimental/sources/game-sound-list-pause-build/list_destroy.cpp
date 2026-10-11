#include "list_pause.hpp"
using namespace norrath_sound_list_pause_detail;

extern "C" void norrath_list_destroy(void* head_storage) {
    Address node=read_word_fresh(address(head_storage));
    const Address pool=address(norrath_constructor_list_pool_view);
    // The real branch-delay read is mandatory even for an empty head.
    Address free_list=read_word_fresh(pool);
    if(node==0u)return;
    do {
        const Address captured_next=read_word_fresh(node);
        const Address previous=read_word_fresh(node+4u);
        Address fresh_next=captured_next;
        if(previous!=0u) {
            write_word_ordered(previous,captured_next);
            fresh_next=read_word_fresh(node);
        }
        if(fresh_next!=0u) {
            const Address fresh_previous=read_word_fresh(node+4u);
            write_word_ordered(fresh_next+4u,fresh_previous);
        }
        const Address twin=read_word_fresh(node+8u);
        if(twin!=0u) {
            const Address twin_previous=read_word_fresh(twin+4u);
            if(twin_previous!=0u) {
                const Address twin_next=read_word_fresh(twin);
                write_word_ordered(twin_previous,twin_next);
            }
            const Address fresh_twin_next=read_word_fresh(twin);
            if(fresh_twin_next!=0u) {
                const Address fresh_twin_previous=read_word_fresh(twin+4u);
                write_word_ordered(fresh_twin_next+4u,fresh_twin_previous);
            }
            write_word_ordered(twin,free_list);
            free_list=twin;
        }
        write_word_ordered(node,free_list);
        free_list=node;
        node=captured_next;
    } while(node!=0u);
    write_word_ordered(pool,free_list);
}
