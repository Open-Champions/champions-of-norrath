#include "sound_timer_irq.hpp"
using namespace norrath_sound_timer_irq_detail;
extern "C" int norrath_sound_timer_irq_callback() {
    const Raw64 irq_context=incoming_irq_a2_bits();
    const Address state=address(norrath_sound_timer_control_view);
    const unsigned entry_mode=load_word(native_mode_register);
    if((entry_mode&0x400u)!=0u) {
        Address current=load_word(state+24u);
        if(current!=0u) {
            explicit_pause(); // original initial NOP before the first iteration
            do {
                unsigned count=load_word(native_count_register);
                const unsigned mode=load_word(native_mode_register);
                const Raw64 delay=load_doubleword(current+32u);
                const Raw64 start=load_doubleword(current+16u);
                const Raw64 accumulated=load_doubleword(current+24u);
                const Raw64 deadline=(delay+start)-accumulated;
                if((mode&0x800u)!=0u) {
                    const Raw64 old_upper=load_doubleword(state);
                    store_doubleword(state,old_upper+1ull);
                    // The original tests bit800 but genuinely clears bit400.
                    norrath_sound_timer_set_mode_register(
                        signed_word_bits(mode)&signed_word_bits(0xfffffbffu));
                    count=load_word(native_count_register);
                }
                const Raw64 upper=load_doubleword(state);
                const Raw64 now=((upper<<16)|Raw64(count))<<((mode&3u)<<2);
                if(now<deadline)break;

                const Raw64 captured_successor=norrath_sound_timer_unlink_alarm(pointer(current));
                const Raw64 fresh_accumulated=load_doubleword(current+24u);
                const unsigned first_tag=load_word(current+8u);
                const unsigned handle_prefix=current<<4;
                const Raw64 fresh_start=load_doubleword(current+16u);
                store_word(state+28u,handle_prefix|first_tag);
                const Raw64 elapsed=(now+fresh_accumulated)-fresh_start;
                const unsigned node_gp=load_word(current+44u);
                install_node_gp_bits(signed_word_bits(node_gp));
                const unsigned fresh_tag=load_word(current+8u);
                const unsigned function_word=load_word(current+40u);
                const Raw64 interval=load_doubleword(current+32u);
                const unsigned parameter_word=load_word(current+48u);
                const NativeCallbackResult callback=invoke_timer_handler(
                    handler_at(function_word),
                    signed_word_bits(handle_prefix)|signed_word_bits(fresh_tag),
                    interval,elapsed,pointer(parameter_word),irq_context);
                const Raw64 result=callback.value;
                // Returned GP is intentionally NOT restored. The actual native
                // register output remains whatever the real callback supplied.
                if(result==0ull) {
                    const unsigned flags=load_word(current+12u);
                    store_word(current+12u,flags&0xfffffffdu);
                } else if(result==~Raw64(0)) {
                    const Address old_free_head=load_word(state+20u);
                    store_word(state+20u,current);
                    store_word(current,old_free_head);
                    store_word(current+8u,0u);
                    store_word(current+12u,0u);
                } else {
                    const Raw64 fresh_delay=load_doubleword(current+32u);
                    const Raw64 increment=result>14744ull?result:14745ull;
                    store_doubleword(current+32u,fresh_delay+increment);
                    norrath_sound_timer_insert_alarm(pointer(current));
                }
                // Genuine Unlink returns signed-LW next pointer bits; preserve
                // its captured word32 pointer, rather than reloading the head.
                current=static_cast<unsigned>(captured_successor);
            } while(current!=0u);
        }
    }

    store_word(state+28u,0xffffffffu);
    unsigned count=load_word(native_count_register);
    const unsigned mode=load_word(native_mode_register);
    if((mode&0x800u)!=0u) {
        const Raw64 old_upper=load_doubleword(state);
        store_doubleword(state,old_upper+1ull);
        norrath_sound_timer_set_mode_register(
            signed_word_bits(mode)&signed_word_bits(0xfffffbffu));
        count=load_word(native_count_register);
    }
    const Raw64 upper=load_doubleword(state);
    const Raw64 now=((upper<<16)|Raw64(count))<<((mode&3u)<<2);
    norrath_sound_timer_set_next_comparison(now);

    // The original mandatorily reads count before the last fresh mode, and
    // again after a final MODE call. Their values are unused, not their effects.
    static_cast<void>(load_word(native_count_register));
    const unsigned final_mode=load_word(native_mode_register);
    if((final_mode&0x800u)!=0u) {
        const Raw64 old_upper=load_doubleword(state);
        store_doubleword(state,old_upper+1ull);
        norrath_sound_timer_set_mode_register(
            signed_word_bits(final_mode)&signed_word_bits(0xfffffbffu));
        static_cast<void>(load_word(native_count_register));
    }
    static_cast<void>(load_doubleword(state));
    memory_sync();
    enable_native_interrupts();
    return 0;
}
