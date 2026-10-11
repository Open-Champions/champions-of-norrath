#include "sound_restore_update.hpp"
using namespace norrath_sound_restore_update_detail;

extern "C" void norrath_sound_restore(bool reset_hardware) {
    int stream_index=0;
    int allocation_count=0;
    const Address sizes=address(norrath_sound_stream_buffer_sizes_view);
    if(reset_hardware) {
        write_word_ordered(sizes,0x32000u);
        norrath_sound_initialize_hardware();
    }

    for(;;) {
        const unsigned maximum=norrath_sound_query_max_free_memory();
        norrath_sce_printf(text_at(address(norrath_sound_restore_formats_view)),
                          signed_word(maximum)>>10);
        if(signed_word(maximum)<16384)break;

        unsigned combined=read_word_fresh(sizes+unsigned(stream_index)*4u)+4096u;
        int next_stream=stream_index+1;
        int group_count=1;
        for(int candidate_index=next_stream;candidate_index<3;++candidate_index) {
            const unsigned candidate=combined+read_word_fresh(sizes+unsigned(candidate_index)*4u)+4096u;
            if(signed_word(maximum)<signed_word(candidate))break;
            combined=candidate;
            ++group_count;
        }
        const unsigned allocated_bytes=signed_word(combined)<signed_word(maximum)?combined:maximum;
        norrath_sce_printf(text_at(address(norrath_sound_restore_formats_view)+16u),
                          signed_word(combined),signed_word(maximum));
        unsigned allocation=norrath_sound_allocate_iop_heap(allocated_bytes);
        // Capture immediately after the genuine call, before source read/write
        // helpers can be outlined and temporarily change the live RA lane.
        unsigned previous_real_call_return=live_return_register();
        if(allocation==0u) {
            assert_original(1230u,norrath_sound_restore_allocation_condition_view,previous_real_call_return);
            previous_real_call_return=live_return_register();
        }
        const Address slot=address(norrath_sound_stream_heap_words_view)+unsigned(allocation_count)*4u;
        if(read_word_fresh(slot)!=0u) {
            assert_original(1231u,norrath_sound_restore_slot_condition_view,previous_real_call_return);
            previous_real_call_return=live_return_register();
        }
        write_word_ordered(slot,allocation);
        ++allocation_count;

        if(group_count==1) {
            const Address record=stream_record(stream_index);
            stream_index=next_stream;
            write_word_ordered(record+44u,allocation+2048u);
            write_word_ordered(record+48u,allocated_bytes-4096u);
        } else {
            // The original reads sizes by the index inside this allocation
            // group. It does not substitute the current global stream index.
            for(int group_index=0;group_index<group_count;++group_index) {
                const unsigned length=read_word_fresh(sizes+unsigned(group_index)*4u);
                const Address record=stream_record(stream_index);
                stream_index=next_stream;
                const unsigned buffer=allocation+2048u;
                allocation=allocation+length+4096u;
                write_word_ordered(record+44u,buffer);
                next_stream=stream_index+1;
                write_word_ordered(record+48u,length);
            }
        }
        if(stream_index>=4)
            assert_original(1248u,norrath_sound_restore_stream_count_condition_view,previous_real_call_return);
        if(stream_index>=3)break;
    }

    for(int remaining=stream_index;remaining<3;++remaining) {
        const Address record=stream_record(remaining);
        write_word_ordered(record+44u,0u);
        write_word_ordered(record+48u,0u);
    }
    norrath_sce_printf(text_at(address(norrath_sound_restore_formats_view)+128u),allocation_count);
    for(int index=0;index<3;++index) {
        const Address record=stream_record(index);
        const unsigned reported_buffer=read_word_fresh(record+44u);
        const unsigned reported_length=read_word_fresh(record+48u);
        norrath_sce_printf(text_at(address(norrath_sound_restore_formats_view)+152u),
                          index,signed_word(reported_buffer),signed_word(reported_length));
        write_word_ordered(record+16u,0u);
        // Both fields are fresh after the real print call; all twelve packet
        // bytes are captured before any write into the shared send buffer.
        const unsigned packet_length=read_word_fresh(record+48u);
        const unsigned packet_buffer=read_word_fresh(record+44u);
        const Address send=address(norrath_sound_rpc_send_view);
        write_word_ordered(send,unsigned(index));
        write_word_ordered(send+4u,packet_buffer);
        write_word_ordered(send+8u,packet_length);
        write_byte_ordered(record,0u);
        rpc_original(0x30,12);
    }
    // The zero-length request still passes the actual send-buffer pointer.
    rpc_original(0x20,0);
}

/* soundRestore__Fb at 0x0024bc38 is 984 bytes and is this function.
 * The original name is the same body, not a second copy.
 */
extern "C" void soundRestore__Fb(bool) __attribute__((alias("norrath_sound_restore")));

