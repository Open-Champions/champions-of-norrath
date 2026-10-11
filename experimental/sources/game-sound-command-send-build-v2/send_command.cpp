#include "sound_command_send.hpp"
using namespace norrath_sound_command_send_detail;
extern "C" int norrath_sound_send_command(
    int command,void* packet,int packet_bytes,
    void* source,void* destination,int data_bytes) {
    const Raw64 status=norrath_sound_command_send_core(
        command,0,packet,packet_bytes,source,destination,data_bytes);
    // The already frozen ordinary interface promises only signed low32.
    // The original native wrapper transports whole V0; raw upper-bit behavior
    // is deliberately not promised by this existing int source interface.
    return signed_word(unsigned(status));
}
