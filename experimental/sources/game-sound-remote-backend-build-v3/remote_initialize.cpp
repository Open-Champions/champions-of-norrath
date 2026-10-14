#include "sound_remote_backend.hpp"
using namespace norrath_sound_remote_backend_detail;

extern "C" int norrath_sound_remote_initialize() {
    norrath_sound_initialize_rpc(0);
    const Address client=address(norrath_sound_remote_rpc_client_view);
    do {
        const int bind_result=norrath_sound_bind_rpc(bytes(client),signed_word(0x80000701u),0);
        if(bind_result<0) {
            norrath_sce_printf(text_at(address(norrath_sound_remote_bind_failure_format_view)));
            return -1;
        }
        int countdown=10000;
        do {
            --countdown;
            four_original_wait_nops();
        } while(countdown!=-1);
    } while(poll_remote_server_word(client)==0u);
    write_word_ordered(address(norrath_sound_remote_end_function_word_view),0u);
    write_word_ordered(address(norrath_sound_remote_end_gp_word_view),0u);
    norrath_sound_flush_cache(0);
    return 0;
}
