#include "sound_timer_initialization.hpp"
using namespace norrath_sound_timer_initialization_detail;
extern "C" Raw64 norrath_sound_add_interrupt_handler2(
    Raw64 line,Raw64 callback_codeword,Raw64 priority,Raw64 parameter) {
    return register_interrupt_service(line,callback_codeword,priority,parameter);
}
