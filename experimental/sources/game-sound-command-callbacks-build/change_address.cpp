#include "sound_command_callbacks.hpp"
using namespace norrath_sound_command_callbacks_detail;
extern "C" void norrath_sound_command_change_address(void* packet,void* parameter) {
    const unsigned destination=load_word(address(packet)+16u);
    store_word(address(parameter)+8u,destination);
}
