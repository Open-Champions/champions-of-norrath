#include "sound_wake_timer.hpp"
using namespace norrath_sound_wake_timer_detail;
extern "C" unsigned long long norrath_sound_interrupt_wake_thread_fallback(
    unsigned long long physical_argument_a0) {
    // Whole genuine _iWakeupThread16 has selector-52, distinct from root-47.
    return wake_service(-52,physical_argument_a0).status;
}
