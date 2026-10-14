#include "sound_command_callbacks.hpp"
using namespace norrath_sound_command_callbacks_detail;
extern "C" int norrath_sound_command_dma_handler() {
    // This is the observed original local workspace BEFORE its saved state,
    // not an inferred packet capacity. It is deliberately not zero-initialized.
    alignas(16) unsigned char snapshot[112];
    const Address state=address(norrath_sound_command_state_view);
    const Address packet=load_word(state);
    const unsigned length=load_byte(packet);
    if(length==0)return 0;

    // LBU bounds the actual value to1..255. Original signed MOVN rounding is
    // (length+15)>>4 on this whole reachable domain, with no added size guard.
    const unsigned quadwords=(length+15u)>>4;
    store_byte(packet,0);
    Address from=packet;
    Address to=address(snapshot);
    unsigned remaining=quadwords;
    do {
        transfer_quadword(from,to);
        from+=16u;
        to+=16u;
        --remaining;
    } while(remaining!=0);
    // The uncapped path can write beyond112 for original lengths>112.
    // It remains authored, with saved-state/frame/alias/lifetime proof held.
    norrath_sound_interrupt_set_dma_chain();

    const unsigned first_command=load_word(address(snapshot)+8u);
    if(signed_word(first_command)<0) {
        const unsigned command=load_word(address(snapshot)+8u)&0x7fffffffu;
        const int count=signed_word(load_word(state+16u));
        if(signed_word(command)<count) {
            const unsigned offset=command*12u;
            const Address first_table=load_word(state+12u);
            const Address entry=first_table+offset;
            const unsigned handler_word=load_word(entry);
            if(handler_word!=0) {
                const unsigned handler_gp=load_word(entry+8u);
                const Raw64 saved_gp=current_gp_bits();
                install_gp_bits(signed_word_gp_bits(handler_gp));
                const Address fresh_table=load_word(state+12u);
                const Address parameter=load_word(fresh_table+offset+4u);
                command_handler(handler_word)(snapshot,pointer(parameter));
                const Raw64 returned_gp=current_gp_bits();
                (void)returned_gp;
                install_gp_bits(saved_gp);
            }
        }
    } else {
        const unsigned command=load_word(address(snapshot)+8u);
        const int count=signed_word(load_word(state+24u));
        if(signed_word(command)<count) {
            const unsigned offset=command*12u;
            const Address first_table=load_word(state+20u);
            const Address entry=first_table+offset;
            const unsigned handler_word=load_word(entry);
            if(handler_word!=0) {
                const unsigned handler_gp=load_word(entry+8u);
                const Raw64 saved_gp=current_gp_bits();
                install_gp_bits(signed_word_gp_bits(handler_gp));
                const Address fresh_table=load_word(state+20u);
                const Address parameter=load_word(fresh_table+offset+4u);
                command_handler(handler_word)(snapshot,pointer(parameter));
                const Raw64 returned_gp=current_gp_bits();
                (void)returned_gp;
                install_gp_bits(saved_gp);
            }
        }
    }
    finish_interrupt();
    return 0;
}
