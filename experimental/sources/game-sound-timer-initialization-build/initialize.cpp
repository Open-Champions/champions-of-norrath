#include "sound_timer_initialization.hpp"
using namespace norrath_sound_timer_initialization_detail;
extern "C" Raw64 norrath_sound_timer_initialize(Raw64 requested_mode) {
    const Address control=address(norrath_sound_timer_control_view);
    const Raw64 current_id=signed_word_bits(load_word(control+8u));
    if(!signed_negative(current_id))return signed_word_bits(0x80008001u);

    store_doubleword(control,0ull);
    const Address counter_base=address(norrath_sound_timer_counter_buffer_view);
    store_word(control+16u,0u);
    norrath_block_memory_set(pointer(counter_base),0,8192u);
    store_word(control+20u,counter_base);
    Address next=counter_base+8192u;
    Address cursor=counter_base+8128u;
    int remaining=127;
    explicit_pause();
    do {
        store_word(cursor,next);
        --remaining;
        cursor-=64u;
        next-=64u;
        explicit_pause();
        explicit_pause();
    } while(remaining>=0);
    // Original clears the final next word in the call's delay slot, before
    // the actual alarm initializer. It does not zero all alarm fields.
    store_word(counter_base+8128u,0u);
    norrath_sound_timer_initialize_alarm_list();

    const unsigned callback_word=static_cast<unsigned>(
        reinterpret_cast<__UINTPTR_TYPE__>(&norrath_sound_timer_irq_callback));
    const Raw64 handler_id=norrath_sound_add_interrupt_handler2(
        11ull,signed_word_bits(callback_word),0ull,0ull);
    if(signed_negative(handler_id))return signed_word_bits(0x80009021u);
    store_word(control+8u,static_cast<unsigned>(handler_id));
    const int interrupts_were_enabled=norrath_sound_disable_interrupts();
    const Raw64 existing_mode=signed_word_bits(load_word(native_mode_register));
    Raw64 mode=(existing_mode&0xfffffffffffffffcull)|requested_mode|0x300ull;
    if((mode&0x80ull)==0ull) {
        // This pure captured-mode update occupies the original COUNT delay;
        // its physical timing/frame alias remains a first-object/native gate.
        mode|=0xc80ull;
        norrath_sound_timer_set_count_register(0ull);
        norrath_sound_timer_set_compare_register(0xffffull);
    }
    norrath_sound_timer_set_mode_register(mode);
    norrath_sound_enable_interrupt_line(11ull);
    if(interrupts_were_enabled!=0)norrath_sound_enable_interrupts();
    return 0ull;
}
