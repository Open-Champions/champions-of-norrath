#include "sound_timer_register.hpp"
using namespace norrath_sound_timer_register_detail;
extern "C" void norrath_sound_timer_write_register(unsigned long long native_address,
                                                   unsigned long long raw_word) {
    const Raw64 previous_enabled=read_status()&interrupt_enable_bit;
    if(previous_enabled!=0ull) {
        explicit_pause();
        Raw64 still_enabled;
        do {
            disable_interrupts();
            pipeline_sync();
            still_enabled=read_status()&interrupt_enable_bit;
            explicit_pause();
            explicit_pause();
            explicit_pause();
            // Original branch-delay NOP executes on both outcomes. Exact
            // generated placement/native branch timing is a first-object gate.
            explicit_pause();
        } while(still_enabled!=0ull);
    }
    Raw64 status=read_status();
    pipeline_sync();
    status|=6ull;
    status^=2ull;
    status|=previous_enabled;
    write_status(status);
    pipeline_sync();
    store_native_word(native_address,raw_word);
    const Raw64 live_ra=live_return_register_bits();
    write_error_epc(live_ra);
    pipeline_sync();
    exception_return();
    // Genuine trailing JR/return exists in the original complete112 extent.
    // GCC sees ordinary fallthrough; hardware ERET normally exits before it.
    // Physical frame/clobber/exception behavior remains explicitly unproved.
}
