#include "sound_timer_helpers.hpp"
using namespace norrath_sound_timer_helpers_detail;
extern "C" void norrath_sound_timer_set_next_comparison(unsigned long long now) {
    const Address state=address(norrath_sound_timer_control_view);
    const unsigned active=load_word(state+28u);
    const Address head=load_word(state+24u); // actual mandatory branch-delay LW
    if((active&0x80000000u)==0u)return;
    const unsigned mode=load_word(native_mode_register);
    const Raw64 cleared_mode=signed_word_bits(mode)&signed_word_bits(0xfffff7ffu);
    if(head==0u) {
        norrath_sound_timer_set_compare_register(0ull);
        norrath_sound_timer_set_mode_register(cleared_mode);
        return;
    }
    const Raw64 delay=load_doubleword(head+32u);
    const Raw64 start=load_doubleword(head+16u);
    const Raw64 accumulated=load_doubleword(head+24u);
    Address next=load_word(head);
    Raw64 selected_deadline=(delay+start)-accumulated;
    while(next!=0u) {
        const Raw64 next_delay=load_doubleword(next+32u);
        const Raw64 next_start=load_doubleword(next+16u);
        const Raw64 next_accumulated=load_doubleword(next+24u);
        const Raw64 next_deadline=(next_delay+next_start)-next_accumulated;
        if(!(next_deadline<selected_deadline+29491ull))break;
        selected_deadline=next_deadline;
        next=load_word(next);
    }
    const unsigned shift=(mode&3u)<<2; // original mode0/1/2/3 shift0/4/8/12
    if(signed_delta_before_29491(selected_deadline-now)) {
        const Raw64 ticks=29491ull>>shift;
        const unsigned count=load_word(native_count_register); // genuine signedLW
        const Raw64 compare=signed_word_bits(count+unsigned(ticks));
        norrath_sound_timer_set_compare_register(compare);
        norrath_sound_timer_set_mode_register(cleared_mode);
    } else {
        norrath_sound_timer_set_mode_register(cleared_mode);
        const Raw64 compare=signed_word_bits(unsigned(selected_deadline>>shift));
        norrath_sound_timer_set_compare_register(compare);
    }
}
