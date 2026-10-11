#include "sound_sequence_control.hpp"
using namespace norrath_sound_sequence_control_detail;

extern "C" void norrath_sound_stop(int sound_id) {
    if(sound_id==-1)return;
    const unsigned id=unsigned(sound_id);
    if((id&0x8000u)!=0u){
        const unsigned index=id&255u;
        const unsigned generation=(id>>8)&127u;
        const Address sequence=sequence_record(index);
        if(read_half_fresh(sequence+56u)==generation)
            write_byte_ordered(sequence+4u,0u);
        // Even a stale sequence generation still performs this complete scan.
        for(int channel=0;channel<43;++channel){
            const Address record=channel_record(unsigned(channel));
            if((read_half_fresh(record+4u)&4u)==0u)continue;
            if(read_signed_half_fresh(record+6u)!=int(index))continue;
            if(read_signed_half_fresh(record+8u)!=int(generation))continue;
            norrath_sound_stop(channel);
        }
        return;
    }
    // Original negative/unbounded IDs retain wrapped map/address behavior.
    const Address mapping=address(norrath_sound_channel_voice_map_view)+id*2u;
    const int bank=signed_byte_at(mapping);
    const int voice=signed_byte_at(mapping+1u);
    const Address key_on=address(norrath_sound_key_on_words_view)+unsigned(bank)*4u;
    const Address key_off=address(norrath_sound_key_off_words_view)+unsigned(bank)*4u;
    const Wide bit=native_shifted_one_word(voice);
    const Wide captured_on=native_signed_word_bits(read_word_fresh(key_on));
    const Wide captured_off=native_signed_word_bits(read_word_fresh(key_off));
    const Wide cleared_on=captured_on&~bit;
    write_half_ordered(channel_record(id)+4u,0u);
    const Wide marked_off=captured_off|bit;
    write_word_ordered(key_off,unsigned(marked_off));
    write_word_ordered(key_on,unsigned(cleared_on));
}
