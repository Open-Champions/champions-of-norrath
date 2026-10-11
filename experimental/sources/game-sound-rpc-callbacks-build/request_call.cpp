#include "sound_rpc_callbacks.hpp"
using namespace norrath_sound_rpc_callbacks_detail;
extern "C" void norrath_sound_rpc_request_call(void* request,void*) {
    const Address incoming=address(request);
    const Address server=read_word_fresh(incoming+52u);
    const Address queue=read_word_fresh(server+64u);
    const unsigned head=read_word_fresh(queue+12u);
    if(head) {
        // The native non-null-head path dereferences even a null tail.
        const Address tail=read_word_fresh(queue+16u);
        write_word_ordered(tail+60u,server);
    } else {
        write_word_ordered(queue+12u,server);
    }
    write_word_ordered(queue+16u,server);

    const unsigned source_packet=read_word_fresh(incoming+20u);
    const unsigned client=read_word_fresh(incoming+28u);
    write_word_ordered(server+32u,source_packet);
    write_word_ordered(server+28u,client);
    write_word_ordered(server+36u,read_word_fresh(incoming+32u));
    write_word_ordered(server+12u,read_word_fresh(incoming+36u));
    write_word_ordered(server+40u,read_word_fresh(incoming+40u));
    write_word_ordered(server+44u,read_word_fresh(incoming+44u));
    write_word_ordered(server+48u,read_word_fresh(incoming+48u));
    write_word_ordered(server+52u,read_word_fresh(incoming+16u));
    const unsigned thread=read_word_fresh(queue);
    if(signed_word(thread)>=0) {
        const unsigned busy=read_word_fresh(queue+4u);
        if(busy==0u)norrath_sound_interrupt_wake_thread(signed_word(thread));
    }
}
