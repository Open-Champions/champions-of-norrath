#include "sound_sequence_control.hpp"
using namespace norrath_sound_sequence_control_detail;

extern "C" void norrath_sound_make_persistent(int sound_id) {
    if(sound_id==-1)return;
    const unsigned id=unsigned(sound_id);
    if((id&0x8000u)!=0u){
        const Address sequence=sequence_record(id&255u);
        write_byte_ordered(sequence+60u,1u);
        const unsigned captured_flags=read_half_fresh(sequence+36u);
        write_word_ordered(sequence+16u,0x3f800000u);
        write_word_ordered(sequence+24u,0x3f800000u);
        write_word_ordered(sequence+20u,0u);
        write_half_ordered(sequence+36u,(captured_flags|1u)&0xfffbu);
        write_word_ordered(sequence+28u,0u);
        return;
    }
    if(sound_id<0)return;
    const Address channel=channel_record(id);
    const unsigned flags=read_half_fresh(channel+4u);
    write_half_ordered(channel+4u,flags|1u);
}
