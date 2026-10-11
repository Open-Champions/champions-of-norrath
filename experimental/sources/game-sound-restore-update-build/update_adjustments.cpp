#include "sound_restore_update.hpp"
using namespace norrath_sound_restore_update_detail;

extern "C" void norrath_sound_update_adjustments() {
    const Address volumes=address(norrath_sound_stream_volumes_view);
    const float first=read_float_fresh(volumes);
    const float second=read_float_fresh(volumes+4u);
    const float first_scaled=native_multiply_ordered(first,256.0f);
    const unsigned stereo=read_word_fresh(address(norrath_sound_stereo_enable_word_view));
    const float second_scaled=native_multiply_ordered(second,256.0f);
    const unsigned first_word=native_convert_word(first_scaled);
    const unsigned second_word=native_convert_word(second_scaled);
    const unsigned status=unsigned(norrath_sound_get_stream_play_status(2));
    unsigned third_word=256u;
    if((status&0x80u)!=0u) {
        const float third=read_float_fresh(volumes+8u);
        third_word=native_convert_word(native_multiply_ordered(third,256.0f));
    }
    const Address send=address(norrath_sound_rpc_send_view);
    write_word_ordered(send,stereo);
    write_word_ordered(send+4u,first_word);
    write_word_ordered(send+8u,second_word);
    write_word_ordered(send+12u,third_word);
    rpc_original(0x80,16);
}

