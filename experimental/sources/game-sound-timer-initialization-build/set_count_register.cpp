#include "sound_timer_initialization.hpp"
using namespace norrath_sound_timer_initialization_detail;
extern "C" void norrath_sound_timer_set_count_register(Raw64 value) {
    norrath_sound_timer_write_register(native_write_count_address,value);
}
