#include "sound_command_send.hpp"
extern "C" unsigned long long norrath_sound_interrupt_send_command(
    int command,void* packet,int packet_bytes,
    void* source,void* destination,int data_bytes) {
    return norrath_sound_command_send_core(
        command,1,packet,packet_bytes,source,destination,data_bytes);
}
