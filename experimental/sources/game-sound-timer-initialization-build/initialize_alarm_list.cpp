#include "sound_timer_initialization.hpp"
using namespace norrath_sound_timer_initialization_detail;
extern "C" Raw64 norrath_sound_timer_initialize_alarm_list() {
    const Address base=address(norrath_sound_timer_alarm_buffer_view);
    const Address head=address(norrath_sound_timer_free_alarm_head_view);
    store_word(head,base);
    Address next=base+1024u;
    Address cursor=base+1008u;
    int remaining=63;
    explicit_pause();
    do {
        store_word(cursor,next);
        --remaining;
        cursor-=16u;
        next-=16u;
        explicit_pause();
        explicit_pause();
    } while(remaining>=0);
    store_word(base+1008u,0u);
    return 0ull;
}
