#include "sound_restore_update.hpp"
using namespace norrath_sound_restore_update_detail;

extern "C" int norrath_sound_get_stream_play_status(int stream_index) {
    rpc_original(0x50,0);
    const Address receive=address(norrath_sound_rpc_receive_view)+unsigned(stream_index)*16u;
    const Address record=stream_record(stream_index);
    const unsigned first=read_word_fresh(receive);
    const unsigned second=read_word_fresh(receive+4u);
    if(first!=second)
        return signed_word(read_word_fresh(record+32u)|1u);
    if(signed_word(read_word_fresh(record+20u))>0)
        return signed_word(read_word_fresh(record+32u)|1u);
    if(signed_word(read_word_fresh(record+448u))<=0)
        return 0;
    const unsigned flags=read_word_fresh(record+108u);
    if((flags&8u)==0u)
        return signed_word(flags|1u);
    return 0;
}

