#include "sound_timer_helpers.hpp"
using namespace norrath_sound_timer_helpers_detail;
extern "C" unsigned long long norrath_sound_interrupt_get_timer_system_time() {
    Raw64 count=load_unsigned_word(native_count_register);
    const unsigned mode=load_word(native_mode_register);
    Raw64 upper=load_doubleword(address(norrath_sound_timer_control_view));
    if((mode&0x800u)!=0u) {
        count=load_unsigned_word(native_count_register);
        ++upper;
    }
    const unsigned mode_bits=mode&3u;
    // Genuine original quirk: shift0/4/8/16, not uniform mode*4.
    const unsigned shift=mode_bits==0u?0u:(2u<<mode_bits);
    return ((upper<<16)|count)<<shift;
}
