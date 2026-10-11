#include "sound_timer_initialization.hpp"
using namespace norrath_sound_timer_initialization_detail;
extern "C" Raw64 norrath_sound_enable_interrupt_line_native(Raw64 line) {
    return enable_interrupt_service(line);
}
