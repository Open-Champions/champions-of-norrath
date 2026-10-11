#include "sound_timer_register.hpp"
using namespace norrath_sound_timer_register_detail;
extern "C" void norrath_sound_timer_set_compare_register(unsigned long long raw_word) {
    norrath_sound_timer_write_register(native_write_compare_address,raw_word);
}
