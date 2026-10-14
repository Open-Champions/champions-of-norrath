#pragma once
#include "sound_sequence.hpp"
extern "C" {
void norrath_sound_stop(int sound_id);
void norrath_sound_make_persistent(int sound_id);
void norrath_sound_make_sequence_unpausable(int sound_id);
void norrath_sound_set_sequence_attenuation(int sound_id,float near_distance,float far_distance);
// Actual unowned observations, never source state/initializers/capacity grants.
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char norrath_sound_channel_voice_map_view[];
extern unsigned char norrath_sound_key_on_words_view[];
extern unsigned char norrath_sound_key_off_words_view[];
}
namespace norrath_sound_sequence_control_detail {
using namespace norrath_sound_sequence_detail;
inline int signed_byte_at(Address a) {
    const unsigned value=read_byte_fresh(a);
    return value<128u?int(value):int(value)-256;
}
inline Address sequence_record(unsigned index) {
    return address(norrath_sound_sequence_records_view)+index*76u;
}
inline Address channel_record(unsigned index) {
    return address(norrath_sound_channel_records_view)+index*20u;
}
inline Wide native_signed_word_bits(unsigned value) {
    return Wide(value)|((value&0x80000000u)?0xffffffff00000000ull:0ull);
}
// Actual SLLV uses only the low5 count bits and sign-extends its32-bit result
// into the64-bit GPR. No signed host shift/overflow or artificial24-channel cap.
inline Wide native_shifted_one_word(int count) {
    return native_signed_word_bits(1u<<(unsigned(count)&31u));
}
}
