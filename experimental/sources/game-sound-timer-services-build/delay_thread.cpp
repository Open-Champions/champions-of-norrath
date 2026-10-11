#include "sound_timer_delay.hpp"
using namespace norrath_sound_timer_delay_detail;
extern "C" unsigned long long norrath_sound_delay_thread(
    unsigned long long microseconds) {
    if((read_status()&interrupt_enable_bit)==0ull)
        return signed_word_bits(0x80008008u);
    // Original SP0..31 workspace precedes savedS0 at32 in frame80. Only
    // these three native words are initialized; all other bytes stay unwritten.
    alignas(16) unsigned char descriptor[32];
    const Address descriptor_address=address(descriptor);
    store_word(descriptor_address+4u,1u);
    store_word(descriptor_address+20u,
               address(norrath_sound_delay_thread_name_view));
    store_word(descriptor_address+8u,0u);
    const int semaphore=norrath_sound_create_semaphore(descriptor);
    // Existing canonical CreateSema returns int. The original tests full V0;
    // this source retains its declared signextended32-result proof boundary.
    if(semaphore<0)return signed_word_bits(0x80008003u);
    const Raw64 delay=norrath_sound_timer_usec_to_bus_clock(0ull,microseconds);
    const Raw64 alarm=norrath_sound_set_timer_alarm(
        delay,&norrath_sound_delay_thread_handler,
        pointer(static_cast<unsigned>(semaphore)));
    if(signed_negative(alarm)) {
        norrath_sound_delete_semaphore(semaphore);
        return alarm;
    }
    // Actual WaitSema, not SleepThread. Both real statuses are ignored.
    norrath_player_wait_semaphore(semaphore);
    norrath_sound_delete_semaphore(semaphore);
    return 0ull;
}
