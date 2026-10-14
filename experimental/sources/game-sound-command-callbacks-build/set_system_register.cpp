#include "sound_command_callbacks.hpp"
using namespace norrath_sound_command_callbacks_detail;
extern "C" void norrath_sound_command_set_system_register(void* packet,void* parameter) {
    const unsigned index=load_word(address(packet)+16u);
    const Address table=load_word(address(parameter)+28u);
    const unsigned value=load_word(address(packet)+20u);
    store_word(table+(index<<2),value);
}
