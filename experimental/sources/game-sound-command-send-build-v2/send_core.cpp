#include "sound_command_send.hpp"
using namespace norrath_sound_command_send_detail;
extern "C" unsigned long long norrath_sound_command_send_core(
    int command,int flags,void* raw_packet,int packet_bytes,
    void* source,void* destination,int data_bytes) {
    // Original ADDIU(word) then SLTIU97 accepts exactly16 through112.
    if(unsigned(packet_bytes)-16u>=97u)return 0ull;

    const Address packet=address(raw_packet);
    // Original frame128's first saved GPR is32: two16-byte descriptor slots.
    // No whole-array initialization; the second slot remains unwritten when
    // no separate data descriptor is created.
    alignas(8) unsigned char descriptor_workspace[32];
    const Address descriptors=address(descriptor_workspace);
    unsigned descriptor_count=0u;
    if(data_bytes>0) {
        const unsigned captured_first_byte=read_byte_fresh(packet);
        write_word_ordered(packet+4u,address(destination));
        descriptor_count=1u;
        write_word_ordered(descriptors,address(source));
        write_word_ordered(packet,captured_first_byte|(unsigned(data_bytes)<<8));
        write_word_ordered(descriptors+4u,address(destination));
        write_word_ordered(descriptors+8u,unsigned(data_bytes));
        write_word_ordered(descriptors+12u,0u);
        if((unsigned(flags)&4u)!=0u)
            norrath_sound_write_back_data_cache(source,data_bytes);
    } else {
        const unsigned captured_first_byte=read_byte_fresh(packet);
        write_word_ordered(packet+4u,0u);
        write_word_ordered(packet,captured_first_byte);
    }

    // Fresh AFTER the optional real source cache call. This captured field
    // precedes all command-descriptor/packet publications, even if they alias.
    const unsigned dma_destination=read_word_fresh(
        address(norrath_sound_command_dma_destination_word_view));
    const Address command_descriptor=descriptors+(descriptor_count<<4);
    write_word_ordered(command_descriptor,packet);
    write_word_ordered(command_descriptor+4u,dma_destination);
    write_word_ordered(command_descriptor+8u,unsigned(packet_bytes));
    write_word_ordered(packet+8u,unsigned(command));
    write_byte_ordered(packet,unsigned(packet_bytes));
    write_word_ordered(command_descriptor+12u,68u);
    // Original cache-call delay increments the captured count BEFORE callee entry.
    ++descriptor_count;
    norrath_sound_write_back_data_cache(raw_packet,packet_bytes);
    if((unsigned(flags)&1u)!=0u)
        return norrath_sound_interrupt_set_dma(descriptor_workspace,int(descriptor_count));
    return norrath_sound_set_dma(descriptor_workspace,int(descriptor_count));
}
