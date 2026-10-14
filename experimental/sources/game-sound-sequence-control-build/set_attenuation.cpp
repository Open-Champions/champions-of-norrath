#include "sound_sequence_control.hpp"
using namespace norrath_sound_sequence_control_detail;

extern "C" void norrath_sound_set_sequence_attenuation(int sound_id,float near_distance,float far_distance) {
    if(sound_id==-1)return;
    // The actual original has no sequence-bit or generation guard here.
    const Address sequence=sequence_record(unsigned(sound_id)&255u);
    write_float_ordered(sequence+68u,near_distance);
    write_float_ordered(sequence+72u,far_distance);
}
