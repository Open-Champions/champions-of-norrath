#include "sound_wake_timer.hpp"
using namespace norrath_sound_wake_timer_detail;
extern "C" unsigned long long norrath_sound_interrupt_wake_thread(int thread_id) {
    const WakeKernelResult result=wake_service(-47,signed_word_bits(unsigned(thread_id)));
    // Native BEQ uses full returned S0 and actual POST-SYSCALL A0.
    if(result.status!=result.post_argument_a0)
        return norrath_sound_interrupt_wake_thread_fallback(result.post_argument_a0);
    if(result.status>=256ull)return ~Raw64(0);
    if(read_word_fresh(address(norrath_sound_wake_top_id_word_view))==0u)
        return ~Raw64(0);

    // Captured semaphore precedes the fresh cursor load. Mask511 is a native
    // addressing effect, not proof of a512-element ring owner or capacity.
    const unsigned semaphore=read_word_fresh(
        address(norrath_sound_wake_semaphore_word_view));
    const Address ring=address(norrath_sound_wake_ring_view);
    const unsigned index=read_word_fresh(ring+4u)&511u;
    const Address record=ring+(index<<1);
    write_word_ordered(ring+4u,index+1u);
    write_byte_ordered(record+8u,0u);
    write_byte_ordered(record+9u,unsigned(result.status));
    norrath_sound_interrupt_signal_semaphore(signed_word(semaphore));
    return result.status;
}
