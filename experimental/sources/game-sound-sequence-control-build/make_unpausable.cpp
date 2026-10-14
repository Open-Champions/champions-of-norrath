#include "sound_sequence_control.hpp"
using namespace norrath_sound_sequence_control_detail;

extern "C" void norrath_sound_make_sequence_unpausable(int sound_id) {
    if(sound_id==-1)return;
    const unsigned id=unsigned(sound_id);
    if((id&0x8000u)==0u)return;
    write_byte_ordered(sequence_record(id&255u)+59u,1u);
}
