#include "sound_timer_helpers.hpp"
using namespace norrath_sound_timer_helpers_detail;
extern "C" unsigned long long norrath_sound_timer_unlink_alarm(void* incoming) {
    const Address node=address(incoming);
    const Address previous=load_word(node+4u);
    const Address next=load_word(node);
    if(previous!=0u)store_word(previous,next);
    else store_word(address(norrath_sound_timer_control_view)+24u,next);
    if(next!=0u) {
        const Address fresh_previous=load_word(node+4u);
        store_word(next+4u,fresh_previous);
    }
    store_word(node+4u,0u);
    return signed_word_bits(next);
}
