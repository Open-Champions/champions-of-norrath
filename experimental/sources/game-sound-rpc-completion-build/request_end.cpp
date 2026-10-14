#include "sound_rpc_completion.hpp"
using namespace norrath_sound_rpc_completion_detail;
extern "C" void norrath_sound_rpc_request_end(void* request,void*) {
    const Address incoming=address(request);
    const unsigned command=read_word_fresh(incoming+32u);
    Address client;
    if(command==0x8000000au) {
        client=read_word_fresh(incoming+28u);
        const unsigned callback_predicate=read_word_fresh(client+28u);
        if(callback_predicate!=0u) {
            // Original signed LW context precedes the live full64 GP capture.
            const unsigned callback_gp_word=read_word_fresh(client+24u);
            const Raw64 saved_gp=current_gp_bits();
            install_gp_bits(signed_word_gp_bits(callback_gp_word));
            // The original reloads the callback after installing its GP.
            const Address fresh_function=read_word_fresh(client+28u);
            const Address argument=read_word_fresh(client+32u);
            completion_handler(fresh_function)(bytes(argument));
            // Preserve the original post-callback GP read, then restore BEFORE
            // reloading client from the incoming packet (which may alias).
            (void)current_gp_bits();
            install_gp_bits(saved_gp);
            client=read_word_fresh(incoming+28u);
        }
    } else {
        // Mandatory even for unrecognized commands above or below command10.
        client=read_word_fresh(incoming+28u);
        if(command==0x80000009u) {
            write_word_ordered(client+36u,read_word_fresh(incoming+36u));
            write_word_ordered(client+20u,read_word_fresh(incoming+40u));
        }
    }
    const unsigned semaphore=read_word_fresh(client+8u);
    if(signed_word(semaphore)>=0)
        norrath_sound_interrupt_signal_semaphore(signed_word(semaphore));
    // Real FreePacket effects occur before the final client0 clear. Do not use
    // an earlier packet pointer or invent a null guard around either access.
    const Address packet=read_word_fresh(client);
    norrath_sound_rpc_free_packet(bytes(packet));
    write_word_ordered(client,0u);
}
