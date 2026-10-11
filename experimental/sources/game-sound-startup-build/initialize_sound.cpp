#include "sound_startup.hpp"
using namespace norrath_sound_startup_detail;

extern "C" void norrath_sound_initialize(float gain) {
    const Address formats=address(norrath_sound_startup_formats_view);
    norrath_sce_printf(text_at(formats));
    const Address master=address(norrath_sound_master_volume_view);
    if(native_less_than(float_bits(0x3f800000u),gain))
        write_word_ordered(master,0x3f800000u);
    else
        write_float_ordered(master,gain);
    const float fresh_master=read_float_fresh(master);
    if(native_less_than(fresh_master,float_bits(0u)))write_word_ordered(master,0u);
    (void)norrath_sound_remote_initialize();
    norrath_sound_initialize_hardware();
    norrath_sce_printf(text_at(formats+8u));
    const unsigned first_assert_ra=live_return_register();
    for(unsigned channel=0;channel<43u;++channel){
        const Address record=channel_record(channel);
        write_word_ordered(record,0u);
        write_half_ordered(record+4u,0u);
    }
    const Address batch=address(norrath_sound_batch_iop_buffer_word_view);
    if(read_word_fresh(batch)!=0u){
        norrath_sound_assert(text_at(address(norrath_sound_startup_assert_file_view)),1303u,
                             text_at(address(norrath_sound_startup_assert_conditions_view)),signed_word(first_assert_ra));
    }
    const unsigned allocated_batch=norrath_sound_allocate_iop_heap(4096u);
    write_word_ordered(batch,allocated_batch);
    norrath_sce_printf(text_at(formats+16u));
    const unsigned second_assert_ra=live_return_register();
    const Address iop=address(norrath_sound_iop_buffer_word_view);
    if(read_word_fresh(iop)!=0u){
        norrath_sound_assert(text_at(address(norrath_sound_startup_assert_file_view)),1309u,
                             text_at(address(norrath_sound_startup_assert_conditions_view)+16u),signed_word(second_assert_ra));
    }
    const unsigned allocated_iop=norrath_sound_allocate_iop_heap(4096u);
    write_word_ordered(iop,allocated_iop);
    norrath_sce_printf(text_at(formats+24u));
    norrath_sound_initialize_rpc(0);
    norrath_sce_printf(text_at(formats+32u));
    const Address client=address(norrath_sound_rpc_client_view);
    do {
        (void)norrath_sound_bind_rpc(pointer(client),0x163a5,0);
        int countdown=10000;
        --countdown;
        for(;;){
            five_original_wait_nops();
            if(countdown==-1)break;
            --countdown;
        }
    } while(poll_rpc_server_word(client)==0u);
    norrath_sound_restore(false);
    norrath_sound_update_adjustments();
}

/* soundInit__Ff at 0x0024c010 is 428 bytes and is this function.
 * The original name is the same body, not a second copy.
 */
extern "C" void soundInit__Ff(float) __attribute__((alias("norrath_sound_initialize")));
