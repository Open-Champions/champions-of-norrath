#include "sound_rpc.hpp"
using namespace norrath_sound_rpc_detail;

extern "C" void* norrath_sound_rpc_get_packet(void* pool_state) {
    const Address state=address(pool_state);
    norrath_sound_disable_interrupts();
    const unsigned captured_count=read_word_fresh(state+8u);
    // Original BLEZ delay always reads the base, even if count<=0.
    Address packet=read_word_fresh(state+4u);
    unsigned index=0u;
    if(signed_word(captured_count)>0) {
        for(;;) {
            const unsigned flags=read_word_fresh(packet+16u);
            if((flags&1u)==0u) {
                write_word_ordered(packet+16u,(index<<16)|5u);
                const unsigned old_serial=read_word_fresh(state);
                unsigned packet_serial=old_serial+1u;
                write_word_ordered(state,packet_serial);
                if(packet_serial==1u) {
                    // Preserve the actual double publication and returned1.
                    // Wrapped0 is allowed by the original; no repair is added.
                    write_word_ordered(state,old_serial+2u);
                    packet_serial=1u;
                }
                write_word_ordered(packet+20u,packet);
                write_word_ordered(packet+24u,packet_serial);
                norrath_sound_enable_interrupts();
                return bytes(packet);
            }
            ++index;
            const bool again=signed_word(index)<signed_word(captured_count);
            packet+=64u; // Actual loop-branch delay runs on the final scan too.
            if(!again)break;
        }
    }
    // Original unconditionally EIntr's after DIntr; it does not restore the
    // incoming disabled state or fabricate a response from that return value.
    norrath_sound_enable_interrupts();
    return nullptr;
}
