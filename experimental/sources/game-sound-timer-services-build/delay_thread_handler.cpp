#include "sound_timer_delay.hpp"
using namespace norrath_sound_timer_delay_detail;
extern "C" unsigned long long norrath_sound_delay_thread_handler(
    unsigned long long,unsigned long long,unsigned long long,
    void* semaphore_parameter) {
    // Original physicalA3 is moved to A0 immediately before the genuine call;
    // its callback never explicitly reads incomingT0. Existing int argument
    // narrows to signed32; arbitrary noncanonical upperA3 equality is unproved.
    norrath_sound_interrupt_signal_semaphore(
        signed_word(address(semaphore_parameter)));
    completion_sync();
    completion_enable_interrupts();
    return 0ull;
}
