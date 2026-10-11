#include "sound_rpc_callbacks.hpp"
using namespace norrath_sound_rpc_callbacks_detail;
extern "C" void* norrath_sound_rpc_get_free_packet(void* pool_state) {
    const Address state=address(pool_state);
    const unsigned cursor=read_word_fresh(state+36u);
    const unsigned count=read_word_fresh(state+24u);
    const long long signed_cursor=signed_word(cursor);
    const long long signed_count=signed_word(count);
    // Actual typed integer DIV, zero-divisor BREAK and MFHI, not host '%'.
    // The whole first object must retain this producer/consumer pair with no
    // intervening HI/LO-writing instruction or outlined call. HI/LO and memory
    // clobbers keep the division before the following real memory accesses.
    asm volatile("div $zero,%0,%1"
        : : "r"(signed_cursor),"r"(signed_count) : "hi","lo","memory");
    if(count==0u)asm volatile("break 0,7" : : : "memory");
    const Address base=read_word_fresh(state+20u);
    unsigned long long remainder_bits;
    asm volatile("mfhi %0" : "=r"(remainder_bits) : : "memory");
    const unsigned remainder=unsigned(remainder_bits);
    write_word_ordered(state+36u,remainder+1u);
    return bytes(base+(remainder<<6));
}
